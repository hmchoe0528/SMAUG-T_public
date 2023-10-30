#include "indcpa.h"
#include "rng.h"

/*************************************************
 * Name:        genRx_vec
 *
 * Description: Deterministically generate a vector of sparse polynomial r(x)
 *              from a seed.
 *
 * Arguments:   - uint8_t *r: pointer to ouptput vector r
 *              - uint8_t *input: pointer to input seed (of length input_size)
 *              - size_t input_size: length of input seed
 *              - uint8_t * neg_start: pointer to output vector neg_start
 **************************************************/
void genRx_vec(sppoly r[MODULE_RANK], const uint8_t *input,
               const size_t input_size) {
    uint8_t res[DIMENSION] = {0};
    uint8_t cnt_arr[MODULE_RANK] = {0};

    hwt(res, cnt_arr, input, input_size, HR);

    for (size_t i = 0; i < MODULE_RANK; ++i) {
        r[i].cnt = cnt_arr[i];
        r[i].sx = (uint8_t *)calloc(cnt_arr[i], sizeof(uint8_t));
        r[i].neg_start = convToIdx(r[i].sx, r[i].cnt, res + (i * LWE_N), LWE_N);
    }
}

/*************************************************
 * Name:        indcpa_keypair
 *
 * Description: Generates public and private key for the CPA-secure
 *              Module-Lizard public-key encryption scheme.
 *
 * Arguments:   - public_key *pk: pointer to output public key
 *                (a structure composed of (seed of A, matrix A, vector b))
 *              - secret_key *sk: pointer to output private key
 *                (a structure composed of (vector s, t, vector negstart))
 **************************************************/
void indcpa_keypair(uint8_t pk[PUBLICKEY_BYTES],
                    uint8_t sk[PKE_SECRETKEY_BYTES]) {
    public_key pk_tmp;
    secret_key sk_tmp;

    uint8_t seed[CRYPTO_BYTES + PKSEED_BYTES] = {0};
    randombytes(seed, CRYPTO_BYTES);
    shake128(seed, CRYPTO_BYTES + PKSEED_BYTES, seed, CRYPTO_BYTES);

    memset(&sk_tmp, 0, sizeof(secret_key));
    genSx_vec(&sk_tmp, seed);

    memcpy(&pk_tmp.seed, seed + CRYPTO_BYTES, PKSEED_BYTES);
    genPubkey(&pk_tmp, &sk_tmp, seed);

    memset(pk, 0, PUBLICKEY_BYTES);
    memset(sk, 0, PKE_SECRETKEY_BYTES);
    save_to_string_pk(pk, &pk_tmp);
    save_to_string_sk(sk, &sk_tmp);

    for (size_t i = 0; i < MODULE_RANK; ++i) {
        memset(sk_tmp.sp_vec[i].sx, 0, sk_tmp.sp_vec[i].cnt);
        free(sk_tmp.sp_vec[i].sx);
    }
}

/*************************************************
 * Name:        indcpa_enc
 *
 * Description: Encryption function of the CPA-secure
 *              Module-Lizard public-key encryption scheme.
 *
 * Arguments:   - ciphertext *ctxt: pointer to output ciphertext
 *                (a structure composed of (vector c1, c2))
 *              - public_key *pk: pointer to input public key
 *                (a structure composed of (seed of A, matrix A, vector b))
 *              - uint8_t *delta: pointer to input random delta (of length
 *                DELTA_BYTES) to deterministically generate all randomness
 **************************************************/
void indcpa_enc(uint8_t ctxt[CIPHERTEXT_BYTES],
                const uint8_t pk[PUBLICKEY_BYTES],
                const uint8_t mu[DELTA_BYTES],
                const uint8_t seed[DELTA_BYTES]) {

    uint8_t seed_r[DELTA_BYTES] = {0};
    public_key pk_tmp;
    load_from_string_pk(&pk_tmp, pk);

    // Compute a vector r = hwt(delta, H'(pk))
    sppoly r[MODULE_RANK];
    memset(r, 0, sizeof(sppoly) * MODULE_RANK);

    if (seed == NULL)
        randombytes(seed_r, DELTA_BYTES);
    else
        cmov(seed_r, seed, DELTA_BYTES, 1);
    genRx_vec(r, seed_r, DELTA_BYTES);

    // Compute c1(x), c2(x)
    ciphertext ctxt_tmp;
    memset(&ctxt_tmp, 0, sizeof(ciphertext));
    computeC1(&(ctxt_tmp.c1), pk_tmp.A, r);
    computeC2(&ctxt_tmp.c2, mu, &pk_tmp.b, r);

    save_to_string(ctxt, &ctxt_tmp);
    for (size_t i = 0; i < MODULE_RANK; ++i) {
        memset(r[i].sx, 0, r[i].cnt);
        free(r[i].sx);
    }
}

/*************************************************
 * Name:        indcpa_dec
 *
 * Description: Decryption function of the CPA-secure
 *              Module-Lizard public-key encryption scheme.
 *
 * Arguments:   - uint8_t *delta: pointer to output decrypted delta
 *                (of length DELTA_BYTES), assumed to be already initialized
 *              - secret_key *sk: pointer to input private key
 *                (a structure composed of (vector s, t, vector negstart)
 *              - ciphertext *ctxt: pointer to input ciphertext
 *                (a structure composed of (vector c1, c2))
 **************************************************/
void indcpa_dec(uint8_t delta[DELTA_BYTES],
                const uint8_t sk[PKE_SECRETKEY_BYTES],
                const uint8_t ctxt[CIPHERTEXT_BYTES]) {
    poly delta_temp;
    polyvec c1_temp;

    secret_key sk_tmp;
    memset(&sk_tmp, 0, sizeof(secret_key));
    load_from_string_sk(&sk_tmp, sk);

    ciphertext ctxt_tmp;
    load_from_string(&ctxt_tmp, ctxt);

    c1_temp = ctxt_tmp.c1;
    delta_temp = ctxt_tmp.c2;
    for (uint16_t i = 0; i < LWE_N; ++i)
        delta_temp.coeffs[i] <<= _16_LOG_P2;
    for (size_t i = 0; i < MODULE_RANK; ++i)
        for (size_t j = 0; j < LWE_N; ++j)
            c1_temp.vec[i].coeffs[j] <<= _16_LOG_P;

    // Compute delta = (delta + c1^T * s)
    vec_vec_mult_add(&delta_temp, &c1_temp, sk_tmp.sp_vec);

    // Compute delta = 2/p * delta
    for (uint16_t i = 0; i < LWE_N; ++i) {
        delta_temp.coeffs[i] += DEC_ADD;
        delta_temp.coeffs[i] >>= _16_LOG_T;
    }

    // Set delta
    memset(delta, 0, DELTA_BYTES);
    for (size_t i = 0; i < DELTA_BYTES; ++i) {
        for (uint8_t j = 0; j < 8; ++j) {
            delta[i] ^= ((uint8_t)(delta_temp.coeffs[8 * i + j]) << j);
        }
    }

    for (size_t i = 0; i < MODULE_RANK; ++i) {
        memset(sk_tmp.sp_vec[i].sx, 0, sk_tmp.sp_vec[i].cnt);
        free(sk_tmp.sp_vec[i].sx);
    }
}