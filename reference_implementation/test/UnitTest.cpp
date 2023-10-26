#include "gtest/gtest.h"
#include <string>

extern "C" {
#include "ciphertext.h"
#include "io.h"
#include "kem.h"
#include "pack.h"
#include "parameters.h"
#include "poly.h"
#include "rng.h"
}

template <class Ty>
static bool arrayEq(const Ty *a, const Ty *b, unsigned len) {
    for (unsigned i = 0; i < len; ++i) {
        if (a[i] != b[i])
            return false;
    }
    return true;
}

bool RvecEq(const uint16_t a[MODULE_RANK][LWE_N],
            const uint16_t b[MODULE_RANK][LWE_N]) {
    for (size_t i = 0; i < MODULE_RANK; ++i) {
        if (!arrayEq(a[i], b[i], LWE_N))
            return false;
    }
    return true;
}

bool RmatEq(const uint16_t a[MODULE_RANK][MODULE_RANK][LWE_N],
            const uint16_t b[MODULE_RANK][MODULE_RANK][LWE_N]) {
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

bool isSkDiff(const secret_key &sk1, const secret_key &sk2, bool isPKE) {
    return !arrayEq(sk1.cnt_arr, sk2.cnt_arr, MODULE_RANK) ||
           !SxvecEq((const uint8_t **)sk1.s, (const uint8_t **)sk2.s,
                    sk1.cnt_arr) ||
           (isPKE && !arrayEq(sk1.t, sk2.t, T_BYTES / sizeof(sk1.t[0]))) ||
           !arrayEq(sk1.neg_start, sk2.neg_start, MODULE_RANK);
}

void checkCtxtEq(const uint8_t ctxt1[CIPHERTEXT_BYTES],
                 const uint8_t ctxt2[CIPHERTEXT_BYTES]) {
    ASSERT_FALSE(memcmp(ctxt1, ctxt2, CIPHERTEXT_BYTES))
        << "Ciphertexts are different";
}

bool isCtxtDiff(const ciphertext &ctxt1, const ciphertext &ctxt2, bool isPKE) {
    if (isPKE) {
        if (!arrayEq(ctxt1.c1, ctxt2.c1, CRYPTO_BYTES))
            return true;
    }

    return !RvecEq(ctxt1.c1, ctxt2.c1) || !arrayEq(ctxt1.c2, ctxt2.c2, LWE_N);
}

void testPacking() {
    const unsigned count = 10000;
    for (size_t i = 0; i < count; i++) {
        uint8_t bytes1[PKPOLYMAT_BYTES];
        uint8_t bytes2[PKPOLYMAT_BYTES];
        uint16_t data[MODULE_RANK][MODULE_RANK][LWE_N];
        randombytes(bytes1, PKPOLYMAT_BYTES);
        memcpy(bytes2, bytes1, PKPOLYMAT_BYTES);
        bytes_to_Rq_mat(data, bytes1);
        Rq_mat_to_bytes(bytes1, data);
        ASSERT_TRUE(arrayEq(bytes1, bytes2, PKPOLYMAT_BYTES));
    }
    for (size_t i = 0; i < count; i++) {
        uint8_t bytes1[CTPOLYVEC_BYTES];
        uint8_t bytes2[CTPOLYVEC_BYTES];
        uint16_t data[MODULE_RANK][LWE_N];
        randombytes(bytes1, CTPOLYVEC_BYTES);
        memcpy(bytes2, bytes1, CTPOLYVEC_BYTES);
        bytes_to_Rp_vec(data, bytes1);
        Rp_vec_to_bytes(bytes1, data);
        ASSERT_TRUE(arrayEq(bytes1, bytes2, CTPOLYVEC_BYTES));
    }
    for (size_t i = 0; i < count; i++) {
        uint8_t bytes1[SKPOLYVEC_BYTES];
        uint8_t bytes2[SKPOLYVEC_BYTES];
        uint8_t *data[MODULE_RANK];
        uint8_t data_len[MODULE_RANK];
        uint8_t sum_data_len = 0;

        randombytes(bytes1, SKPOLYVEC_BYTES);
        memcpy(bytes2, bytes1, SKPOLYVEC_BYTES);
        randombytes(data_len, MODULE_RANK);
        for (size_t j = 0; j < MODULE_RANK - 1; ++j) {
            data_len[j] = data_len[j] % (MODULE_RANK * HS - sum_data_len);
            sum_data_len += data_len[j];
        }
        data_len[MODULE_RANK - 1] = HS * MODULE_RANK - sum_data_len;
        for (size_t j = 0; j < MODULE_RANK; ++j) {
            data[j] = (uint8_t *)malloc(data_len[j]);
        }
        bytes_to_Sx_vec(data, bytes1, data_len);
        Sx_vec_to_bytes(bytes1, (const uint8_t **)data, data_len);
        for (size_t j = 0; j < MODULE_RANK; ++j) {
            free(data[j]);
        }
        ASSERT_TRUE(arrayEq(bytes1, bytes2, SKPOLYVEC_BYTES));
    }
}

void testMultOneVector() {
    const unsigned count = 10000;
    for (size_t i = 0; i < count; i++) { // check A * [1] ^ (MODULE_RANK)
        uint8_t matbytes[PKPOLYMAT_BYTES];
        uint16_t A[MODULE_RANK][MODULE_RANK][LWE_N];
        uint16_t vec1[MODULE_RANK][LWE_N];
        uint16_t vec2[MODULE_RANK][LWE_N];
        uint8_t *s[MODULE_RANK];
        uint8_t s_cnt_arr[MODULE_RANK];
        uint8_t neg_start[MODULE_RANK];

        randombytes(matbytes, PKPOLYMAT_BYTES);
        bytes_to_Rq_mat(A, matbytes);
        memset(vec1, 0, sizeof vec1);
        memset(vec2, 0, sizeof vec2);
        for (size_t j = 0; j < MODULE_RANK; j++) {
            s[j] = (uint8_t *)malloc(sizeof(uint8_t));
            s[j][0] = 0;
            s_cnt_arr[j] = 1;
            neg_start[j] = 1;
            for (size_t k = 0; k < MODULE_RANK; k++) {
                for (size_t l = 0; l < LWE_N; l++) {
                    vec2[j][l] += A[j][k][l];
                }
            }
        }
        matrix_vec_mult_add(vec1, A, (const uint8_t **)s, s_cnt_arr, neg_start,
                            0);
        for (size_t j = 0; j < MODULE_RANK; j++) {
            free(s[j]);
        }
        ASSERT_TRUE(RvecEq(vec1, vec2));
    }
    for (size_t i = 0; i < count; i++) { // check -(A * -[1] ^ (MODULE_RANK))
        uint8_t matbytes[PKPOLYMAT_BYTES];
        uint16_t A[MODULE_RANK][MODULE_RANK][LWE_N];
        uint16_t vec1[MODULE_RANK][LWE_N];
        uint16_t vec2[MODULE_RANK][LWE_N];
        uint8_t *s[MODULE_RANK];
        uint8_t s_cnt_arr[MODULE_RANK];
        uint8_t neg_start[MODULE_RANK] = {0};

        randombytes(matbytes, PKPOLYMAT_BYTES);
        bytes_to_Rq_mat(A, matbytes);
        memset(vec1, 0, sizeof vec1);
        memset(vec2, 0, sizeof vec2);
        for (size_t j = 0; j < MODULE_RANK; j++) {
            s[j] = (uint8_t *)malloc(sizeof(uint8_t));
            s[j][0] = 0;
            s_cnt_arr[j] = 1;
            for (size_t k = 0; k < MODULE_RANK; k++) {
                for (size_t l = 0; l < LWE_N; l++) {
                    vec2[j][l] += A[j][k][l];
                }
            }
        }
        matrix_vec_mult_sub(vec1, A, (const uint8_t **)s, s_cnt_arr, neg_start,
                            0);
        for (size_t j = 0; j < MODULE_RANK; ++j)
            free(s[j]);
        ASSERT_TRUE(RvecEq(vec1, vec2));
    }
    for (size_t i = 0; i < count; i++) { // check b^(T) * [1] ^ (MODULE_RANK)
        uint16_t vec[MODULE_RANK][LWE_N];
        uint8_t *s[MODULE_RANK];
        uint8_t s_cnt_arr[MODULE_RANK];
        uint8_t neg_start[MODULE_RANK];
        uint16_t res[LWE_N];
        uint16_t sum[LWE_N];

        memset(res, 0, sizeof res);
        memset(sum, 0, sizeof sum);
        for (size_t j = 0; j < MODULE_RANK; j++) {
            s[j] = (uint8_t *)malloc(sizeof(uint8_t));
            s[j][0] = 0;
            s_cnt_arr[j] = 1;
            neg_start[j] = 1;
            for (size_t k = 0; k < LWE_N; k++) {
                sum[k] += vec[j][k];
            }
        }
        vec_vec_mult_add(res, vec, (const uint8_t **)s, s_cnt_arr, neg_start);
        ASSERT_TRUE(arrayEq(res, sum, LWE_N));
    }
}

void testMultAddSub() {
    const unsigned count = 10000;
    for (size_t i = 0; i < count; i++) {
        uint8_t matbytes[PKPOLYMAT_BYTES];
        uint16_t A[MODULE_RANK][MODULE_RANK][LWE_N];
        uint8_t vecbytes[PKPOLYVEC_BYTES];
        uint16_t vec1[MODULE_RANK][LWE_N];
        uint16_t vec2[MODULE_RANK][LWE_N];
        uint8_t sk_seed[CRYPTO_BYTES] = {0};
        SecretKey sk;
        randombytes(matbytes, PKPOLYMAT_BYTES);
        randombytes(vecbytes, PKPOLYVEC_BYTES);
        randombytes(sk_seed, CRYPTO_BYTES);
        bytes_to_Rq_mat(A, matbytes);
        bytes_to_Rq_vec(vec1, vecbytes);
        bytes_to_Rq_vec(vec2, vecbytes);
        genSx_vec(&sk, sk_seed);
        matrix_vec_mult_add(vec1, A, (const uint8_t **)sk.s, sk.cnt_arr,
                            sk.neg_start, 0);
        matrix_vec_mult_sub(vec1, A, (const uint8_t **)sk.s, sk.cnt_arr,
                            sk.neg_start, 0);
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
            load_from_string_sk(&sk_tmp, sk, 0);
            save_to_string_sk(sk2, &sk_tmp, 0);
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

        uint8_t mx[CRYPTO_BYTES] = {0}, seed[DELTA_BYTES] = {0};
        randombytes(mx, CRYPTO_BYTES);
        randombytes(seed, DELTA_BYTES);

        uint8_t ctxt[CIPHERTEXT_BYTES] = {0};
        if (isPKE)
            indcpa_enc(ctxt, pk, mx, seed);
        else {
            int encryptRes = crypto_kem_encap(ctxt, mx, pk);
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

        uint8_t mx[CRYPTO_BYTES] = {0}, seed[DELTA_BYTES] = {0};
        randombytes(mx, CRYPTO_BYTES);
        randombytes(seed, DELTA_BYTES);

        uint8_t ctxt[CIPHERTEXT_BYTES] = {0};
        if (isPKE) {
            indcpa_enc(ctxt, pk, mx, seed);
        } else {
            int encapRes = crypto_kem_encap(ctxt, mx, pk);
            ASSERT_EQ(encapRes, 0) << "crypto_kem_encapsulation failed";
        }

        uint8_t mx2[CRYPTO_BYTES] = {0};
        if (isPKE) {
            indcpa_dec(mx2, sk, ctxt);
        } else {
            int decapRes = crypto_kem_decap(mx2, sk, pk, ctxt);
            ASSERT_EQ(decapRes, 0) << "crypto_kem_decapsulation failed";
        }

        ASSERT_TRUE(arrayEq(mx, mx2, CRYPTO_BYTES)) << "Messages are different";
    }
}

TEST(General, Packing) { testPacking(); }

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
