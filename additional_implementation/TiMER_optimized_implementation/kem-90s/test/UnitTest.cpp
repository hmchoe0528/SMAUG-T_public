#include "gtest/gtest.h"
#include <string>

extern "C" {
#include "cbd.h"
#include "ciphertext.h"
#include "io.h"
#include "kem.h"
#include "pack.h"
#include "parameters.h"
#include "poly.h"
#include "randombytes.h"
}

template <class Ty>
static bool arrayEq(const Ty *a, const Ty *b, unsigned len) {
    for (unsigned i = 0; i < len; ++i) {
        if (a[i] != b[i])
            return false;
    }
    return true;
}

static bool polyEq(const poly &a, const poly &b, unsigned len) {
    for (unsigned i = 0; i < len; ++i) {
        if (a.coeffs[i] != b.coeffs[i])
            return false;
    }
    return true;
}

bool RvecEq(const polyvec &a, const polyvec &b) {
    for (size_t i = 0; i < MODULE_RANK; ++i) {
        if (!polyEq(a.vec[i], b.vec[i], LWE_N))
            return false;
    }
    return true;
}

bool RmatEq(const polyvec a[MODULE_RANK], const polyvec b[MODULE_RANK]) {
    for (size_t i = 0; i < MODULE_RANK; ++i) {
        if (!RvecEq(a[i], b[i]))
            return false;
    }
    return true;
}

bool SxvecEq(const uint8_t *a[MODULE_RANK], const uint8_t *b[MODULE_RANK],
             const uint8_t cnt_arr[MODULE_RANK]) {
    for (size_t i = 0; i < MODULE_RANK; ++i) {
        if (!arrayEq(a[i], b[i], cnt_arr[i]))
            return false;
    }
    return true;
}

void checkPkEq(const uint8_t pk1[PUBLICKEY_BYTES],
               const uint8_t pk2[PUBLICKEY_BYTES]) {
    ASSERT_FALSE(memcmp(pk1, pk2, PUBLICKEY_BYTES))
        << "Public keys must be equal";
}

bool isPkDiff(const public_key &pk1, const public_key &pk2) {
    return !RmatEq(pk1.A, pk2.A) || !RvecEq(pk1.b, pk2.b);
}

void checkSkEq(const uint8_t sk1[KEM_SECRETKEY_BYTES],
               const uint8_t sk2[KEM_SECRETKEY_BYTES], bool isPKE) {
    if (!isPKE) {
        ASSERT_FALSE(memcmp(sk1, sk2, PKE_SECRETKEY_BYTES))
            << "Secret keys of PKE must be equal";
    } else {
        ASSERT_FALSE(memcmp(sk1, sk2, KEM_SECRETKEY_BYTES))
            << "Secret keys of KEM must be equal";
    }
}

bool isSkDiff(const secret_key &sk1, const secret_key &sk2) {
    bool res = false;
    for (size_t i = 0; i < MODULE_RANK; ++i)
        res |= !arrayEq(sk1.vec[i].coeffs, sk2.vec[i].coeffs, LWE_N);

    return res;
}

void checkCtxtEq(const uint8_t ctxt1[CIPHERTEXT_BYTES],
                 const uint8_t ctxt2[CIPHERTEXT_BYTES]) {
    ASSERT_FALSE(memcmp(ctxt1, ctxt2, CIPHERTEXT_BYTES))
        << "Ciphertexts are different";
}

bool isCtxtDiff(const ciphertext &ctxt1, const ciphertext &ctxt2, bool isPKE) {
    return !RvecEq(ctxt1.c1, ctxt2.c1) ||
           !arrayEq(ctxt1.c2.coeffs, ctxt2.c2.coeffs, LWE_N);
}

void testPacking() {
    const unsigned count = 10000;
    for (size_t i = 0; i < count; i++) {
        uint8_t bytes1[PKPOLYMAT_BYTES];
        uint8_t bytes2[PKPOLYMAT_BYTES];
        polyvec data[MODULE_RANK] = {0};

        randombytes(bytes1, PKPOLYMAT_BYTES);
        memcpy(bytes2, bytes1, PKPOLYMAT_BYTES);
        bytes_to_Rq_mat(data, bytes1);
        Rq_mat_to_bytes(bytes1, data);
        ASSERT_TRUE(arrayEq(bytes1, bytes2, PKPOLYMAT_BYTES));
    }
    for (size_t i = 0; i < count; i++) {
        uint8_t bytes1[CTPOLYVEC_BYTES];
        uint8_t bytes2[CTPOLYVEC_BYTES];
        polyvec data;

        randombytes(bytes1, CTPOLYVEC_BYTES);
        memcpy(bytes2, bytes1, CTPOLYVEC_BYTES);
        bytes_to_Rp_vec(&data, bytes1);
        Rp_vec_to_bytes(bytes1, &data);
        ASSERT_TRUE(arrayEq(bytes1, bytes2, CTPOLYVEC_BYTES));
    }
    for (size_t i = 0; i < count; i++) {
        uint8_t bytes[SKPOLYVEC_BYTES] = {0};
        secret_key data1, data2;

        uint8_t seed[CRYPTO_BYTES] = {0};
        randombytes(seed, CRYPTO_BYTES);
        genSx_vec(&data1, seed);

        for (size_t j = 0; j < MODULE_RANK; ++j) {
            Sx_to_bytes(bytes, &data1.vec[j]);
            bytes_to_Sx(&data2.vec[j], bytes);

            ASSERT_TRUE(
                arrayEq(data1.vec[j].coeffs, data2.vec[j].coeffs, LWE_N));
        }
    }
}

static inline int weight(int *plus, int *minus, const int16_t p[LWE_N]) {
    int zero = 0;
    for (size_t i = 0; i < LWE_N; i++) {
        if (p[i] == 1)
            *plus += 1;
        else if (p[i] == -1)
            *minus += 1;
        else if (p[i] == 0)
            zero += 1;
        else
            return p[i];
    }
    return zero;
}

void testCBD() {
    const unsigned count = 10000;
    for (size_t i = 0; i < count; i++) {
        poly r;
        int plus = 0, minus = 0;
        uint8_t seed[CRYPTO_BYTES] = {0};
#if defined(AVX_KECCAK) || defined(AVX_90S)
        ALIGNED_UINT8(CBDSEED_BYTES) extseed;
#else
        uint8_t extseed[CBDSEED_BYTES];
#endif
        randombytes(seed, CRYPTO_BYTES);

#if defined(AVX_KECCAK)
        shake256(extseed.coeffs, CBDSEED_BYTES, seed, CRYPTO_BYTES);
        poly_cbd(&r, extseed.CBDSEED_FIELD);
#elif defined(AVX_90S)
        prf(extseed.coeffs, CBDSEED_BYTES, seed, 0);
        poly_cbd(&r, extseed.CBDSEED_FIELD);
#else
        shake256(extseed, CBDSEED_BYTES, seed, CRYPTO_BYTES);
        poly_cbd(&r, extseed);
#endif

        int zero = weight(&plus, &minus, r.coeffs);
        ASSERT_TRUE((zero + plus + minus) == LWE_N);
    }
}

void testMultOneVector() {
    const unsigned count = 10000;
#if defined(AVX_KECCAK) || defined(AVX_90S)
    nttpolyvec shat[2];
#endif
    for (size_t i = 0; i < count; i++) { // check A * [1] ^ (MODULE_RANK)
        uint8_t matbytes[PKPOLYMAT_BYTES];
        polyvec A[MODULE_RANK];
        polyvec vec1, vec2;
        polyvec s;
        memset(&s, 0, sizeof s);

        randombytes(matbytes, PKPOLYMAT_BYTES);
        bytes_to_Rq_mat(A, matbytes);
        memset(&vec1, 0, sizeof vec1);
        memset(&vec2, 0, sizeof vec2);
        for (size_t j = 0; j < MODULE_RANK; j++) {
            s.vec[j].coeffs[0] = 1;
            for (size_t k = 0; k < MODULE_RANK; k++) {
                for (size_t l = 0; l < LWE_N; l++) {
                    vec2.vec[k].coeffs[l] += A[j].vec[k].coeffs[l];
                }
            }
        }
#if defined(AVX_KECCAK) || defined(AVX_90S)
        matrix_vec_mult_add(&vec1, shat, A, &s);
#else
        matrix_vec_mult_add(&vec1, A, &s);
#endif
        ASSERT_TRUE(RvecEq(vec1, vec2));
    }
    for (size_t i = 0; i < count; i++) { // check -(A * -[1] ^ (MODULE_RANK))
        uint8_t matbytes[PKPOLYMAT_BYTES];
        polyvec A[MODULE_RANK];
        polyvec vec1, vec2;
        polyvec s;
        memset(&s, 0, sizeof s);

        randombytes(matbytes, PKPOLYMAT_BYTES);
        bytes_to_Rq_mat(A, matbytes);
        memset(&vec1, 0, sizeof vec1);
        memset(&vec2, 0, sizeof vec2);
        for (size_t j = 0; j < MODULE_RANK; j++) {
            s.vec[j].coeffs[0] = -1;
            for (size_t k = 0; k < MODULE_RANK; k++) {
                for (size_t l = 0; l < LWE_N; l++) {
                    vec2.vec[j].coeffs[l] += A[j].vec[k].coeffs[l];
                }
            }
        }
        matrix_vec_mult_sub(&vec1, A, &s);
        ASSERT_TRUE(RvecEq(vec1, vec2));
    }
    for (size_t i = 0; i < count; i++) { // check b^(T) * [1] ^ (MODULE_RANK)
        polyvec vec1;
        polyvec s;
        poly res;
        poly sum;
        memset(&s, 0, sizeof s);

        uint8_t vecbytes[PKPOLYVEC_BYTES];
        randombytes(vecbytes, PKPOLYVEC_BYTES);
        bytes_to_Rq_vec(&vec1, vecbytes);
        memset(&res, 0, sizeof res);
        memset(&sum, 0, sizeof sum);
        for (size_t j = 0; j < MODULE_RANK; j++) {
            s.vec[j].coeffs[0] = 1;
            for (size_t k = 0; k < LWE_N; k++) {
                sum.coeffs[k] += vec1.vec[j].coeffs[k];
            }
        }
#if defined(AVX_KECCAK) || defined(AVX_90S)
        polyvec_ntt(&shat[0], &s, PDATA0);
        polyvec_ntt(&shat[1], &s, PDATA1);
        vec_vec_mult_add_q(&res, &vec1, shat);
#else
        vec_vec_mult_add(&res, &vec1, &s, _16_LOG_Q);
#endif
        ASSERT_TRUE(polyEq(res, sum, LWE_N));
    }
}

void testMultAddSub() {
    const unsigned count = 1;
    for (size_t i = 0; i < count; i++) {
        uint8_t matbytes[PKPOLYMAT_BYTES];
        polyvec A[MODULE_RANK], At[MODULE_RANK];
        uint8_t vecbytes[PKPOLYVEC_BYTES];
        polyvec vec1, vec2;
        uint8_t sk_seed[CRYPTO_BYTES] = {0};
        secret_key sk;
        randombytes(matbytes, PKPOLYMAT_BYTES);
        randombytes(vecbytes, PKPOLYVEC_BYTES);
        randombytes(sk_seed, CRYPTO_BYTES);
        bytes_to_Rq_mat(A, matbytes);
        bytes_to_Rq_vec(&vec1, vecbytes);
        bytes_to_Rq_vec(&vec2, vecbytes);
        genSx_vec(&sk, sk_seed);
        for (size_t l = 0; l < MODULE_RANK; ++l) {
            for (size_t j = 0; j < MODULE_RANK; ++j)
                for (size_t k = 0; k < LWE_N; ++k)
                    At[l].vec[j].coeffs[k] = A[j].vec[l].coeffs[k];
        }

        polyvec res;
        memset(&res, 0, sizeof res);
#if defined(AVX_KECCAK) || defined(AVX_90S)
        nttpolyvec shat;
        matrix_vec_mult_add(&res, &shat, A, &sk);
#else
        matrix_vec_mult_add(&res, A, &sk);
#endif
        for (size_t l = 0; l < MODULE_RANK; ++l) {
            for (size_t k = 0; k < LWE_N; ++k)
                vec1.vec[l].coeffs[k] += res.vec[l].coeffs[k];
        }
        matrix_vec_mult_sub(&vec1, At, &sk);
        ASSERT_TRUE(RvecEq(vec1, vec2));
    }
}

// Test storing + loading random public and secret keys
void testKeyLoadStore(bool isPKE, bool useFile) {
    const unsigned count = 10;
    char pk_filepath[100] = {0};
    char sk_filepath[100] = {0};
    sprintf(pk_filepath, "./%s_pk.bin", isPKE ? "pke" : "kem");
    sprintf(sk_filepath, "./%s_sk.bin", isPKE ? "pke" : "kem");

    for (unsigned i = 0; i < count; ++i) {
        uint8_t sk[KEM_SECRETKEY_BYTES] = {0};
        uint8_t pk[PUBLICKEY_BYTES] = {0};
        if (isPKE) {
            indcpa_keypair(pk, sk);
        } else {
            crypto_kem_keypair(pk, sk);
        }

        uint8_t pk2[PUBLICKEY_BYTES] = {0};
        if (useFile) {
            save_to_file_pk(pk_filepath, pk);
            load_from_file_pk(pk2, pk_filepath);
        } else {
            public_key pk_tmp;
            load_from_string_pk(&pk_tmp, pk);
            save_to_string_pk(pk2, &pk_tmp);
        }
        checkPkEq(pk, pk2);

        uint8_t sk2[KEM_SECRETKEY_BYTES] = {0};
        if (useFile) {
            save_to_file_sk(sk_filepath, sk, isPKE);
            load_from_file_sk(sk2, sk_filepath, isPKE);
        } else {
            secret_key sk_tmp;
            load_from_string_sk(&sk_tmp, sk);
            save_to_string_sk(sk2, &sk_tmp);
            memcpy(sk2 + PKE_SECRETKEY_BYTES, sk + PKE_SECRETKEY_BYTES,
                   T_BYTES);
        }
        checkSkEq(sk, sk2, isPKE);
    }
}

// Test storing + loading random ciphertexts (shared secrets)
void testCiphertextLoadStore(bool isPKE, bool useFile) {
    const unsigned count = 10000;
    char filepath[100];
    sprintf(filepath, "./%s_ctxt.bin", isPKE ? "pke" : "kem");

    for (unsigned i = 0; i < count; ++i) {
        uint8_t sk[KEM_SECRETKEY_BYTES] = {0};
        uint8_t pk[PUBLICKEY_BYTES] = {0};
        if (isPKE) {
            indcpa_keypair(pk, sk);
        } else {
            crypto_kem_keypair(pk, sk);
        }

        uint8_t mx[MSG_BYTES] = {0}, seed[DELTA_BYTES] = {0};
        randombytes(mx, MSG_BYTES);
        randombytes(seed, DELTA_BYTES);

        uint8_t ctxt[CIPHERTEXT_BYTES] = {0};
        if (isPKE)
            indcpa_enc(ctxt, pk, mx, seed);
        else {
            int encryptRes = crypto_kem_enc(ctxt, mx, pk);
            ASSERT_EQ(encryptRes, 0) << "crypto_kem_encapsulation failed";
        }

        uint8_t ctxt2[CIPHERTEXT_BYTES] = {0};
        if (useFile) {
            save_to_file(filepath, ctxt);
            load_from_file(ctxt2, filepath);
        } else {
            ciphertext ctxt_tmp;
            load_from_string(&ctxt_tmp, ctxt);
            save_to_string(ctxt2, &ctxt_tmp);
        }

        checkCtxtEq(ctxt, ctxt2);
    }
}

// Test encoding + decoding random plaintexts
void testCiphertextEncDec(bool isPKE) {
    const unsigned count = 10000;

    for (unsigned i = 0; i < count; ++i) {
        uint8_t sk[KEM_SECRETKEY_BYTES] = {0};
        uint8_t pk[PUBLICKEY_BYTES] = {0};
        if (isPKE) {
            indcpa_keypair(pk, sk);
        } else {
            crypto_kem_keypair(pk, sk);
        }

        uint8_t mx[MSG_BYTES] = {0}, seed[DELTA_BYTES] = {0};
        randombytes(mx, MSG_BYTES);
        randombytes(seed, DELTA_BYTES);

        uint8_t ctxt[CIPHERTEXT_BYTES] = {0};
        if (isPKE) {
            indcpa_enc(ctxt, pk, mx, seed);
        } else {
            int encapRes = crypto_kem_enc(ctxt, mx, pk);
            ASSERT_EQ(encapRes, 0) << "crypto_kem_encapsulation failed";
        }

        uint8_t mx2[MSG_BYTES] = {0};
        if (isPKE) {
            indcpa_dec(mx2, sk, ctxt);
        } else {
            int decapRes = crypto_kem_dec(mx2, ctxt, sk);
            ASSERT_EQ(decapRes, 0) << "crypto_kem_decapsulation failed";
        }

        ASSERT_TRUE(arrayEq(mx, mx2, MSG_BYTES)) << "Messages are different";
    }
}

TEST(General, Packing) { testPacking(); }

TEST(General, CBD) { testCBD(); }

TEST(General, MultOneVector) { testMultOneVector(); }

TEST(General, MultAddSub) { testMultAddSub(); }

TEST(PKE, EncDec) { testCiphertextEncDec(true); }

TEST(PKE, KeyLoadStoreString) { testKeyLoadStore(true, false); }

TEST(PKE, KeyLoadStoreFile) { testKeyLoadStore(true, true); }

TEST(PKE, CiphertextLoadStoreString) { testCiphertextLoadStore(true, false); }

TEST(PKE, CiphertextLoadStoreFile) { testCiphertextLoadStore(true, true); }

TEST(KEM, EncDec) { testCiphertextEncDec(false); }

TEST(KEM, KeyLoadStoreString) { testKeyLoadStore(false, false); }

TEST(KEM, KeyLoadStoreFile) { testKeyLoadStore(false, true); }

TEST(KEM, CiphertextLoadStoreString) { testCiphertextLoadStore(false, false); }

TEST(KEM, CiphertextLoadStoreFile) { testCiphertextLoadStore(false, true); }
