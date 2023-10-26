#include "indcpa.h"
#include "rng.h"

/*************************************************
 * Name:        genRx
 *
 * Description: Deterministically generate sparse polynomial r(x) from a seed,
 *              and compress it.
 *
 * Arguments:   - uint8_t *res: pointer to ouptput compressed r(x) (of length
 *                              HR)
 *              - uint8_t *input: pointer to input seed (of length input_size)
 *              - size_t input_size: length of input seed
 *
 * Returns neg_start(success) or 0(failure).
 **************************************************/
uint16_t genRx(uint8_t *res, const uint8_t *input, const size_t input_size) {
    uint8_t r[LWE_N] = {0};
    hwt(r, input, input_size, HR);

    return (convToIdx(res, HR, r, LWE_N, 1));
}

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
void genRx_vec(uint8_t r[MODULE_RANK][HR], uint8_t neg_start[MODULE_RANK],
               uint8_t *input, const size_t input_size) {
    for (size_t i = 0; i < MODULE_RANK; i++) {
        input[input_size - 1] = i; // work as nonce and make r[i] different
        neg_start[i] = genRx(r[i], input, input_size);
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

    genSx_vec(&sk_tmp, seed);

    memcpy(&pk_tmp.seed, seed + CRYPTO_BYTES, PKSEED_BYTES);
    genPubkey(&pk_tmp, &sk_tmp, seed);

    save_to_string_pk(pk, &pk_tmp);
    save_to_string_sk(sk, &sk_tmp, 1);
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
                const uint8_t delta[DELTA_BYTES]) {
    public_key pk_tmp;
    load_from_string_pk(&pk_tmp, pk);

    // Compute a vector r = H(delta, H'(pk))
    uint8_t r[MODULE_RANK][HR] = {0};
    uint8_t r_neg_start[MODULE_RANK] = {0};

    // delta || H'(pk)

    const size_t rx_input_size = DELTA_BYTES + SHA3_256_HashSize + 1;
    uint8_t rx_input[rx_input_size];

    if (delta == NULL)
        randombytes(rx_input, DELTA_BYTES);
    else
        memcpy(rx_input, delta, DELTA_BYTES);
    hash_h(rx_input + DELTA_BYTES, pk, PUBLICKEY_BYTES);

    genRx_vec(r, r_neg_start, rx_input, rx_input_size);

    // Compute c1(x)
    ciphertext ctxt_tmp;
    memset(ctxt_tmp.c1, 0, sizeof(uint16_t) * MODULE_RANK * LWE_N);
    computeC1(ctxt_tmp.c1, pk_tmp.A, r, r_neg_start);

    // Compute c2(x)
    memset(ctxt_tmp.c2, 0, sizeof(uint16_t) * LWE_N);
    computeC2(ctxt_tmp.c2, (uint8_t *)delta, pk_tmp.b, r, r_neg_start);

    save_to_string(ctxt, &ctxt_tmp);
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
    uint16_t delta_temp[LWE_N] = {0};
    uint16_t c1_temp[MODULE_RANK][LWE_N] = {0};

    secret_key sk_tmp;
    load_from_string_sk(&sk_tmp, sk, 1);

    ciphertext ctxt_tmp;
    load_from_string(&ctxt_tmp, ctxt);

    memcpy(c1_temp, &ctxt_tmp.c1, sizeof(uint16_t) * MODULE_RANK * LWE_N);
    memcpy(delta_temp, &ctxt_tmp.c2, sizeof(uint16_t) * LWE_N);
    for (uint16_t i = 0; i < LWE_N; ++i)
        delta_temp[i] <<= _16_LOG_P;
    for (size_t i = 0; i < MODULE_RANK; ++i)
        for (size_t j = 0; j < LWE_N; ++j)
            c1_temp[i][j] <<= _16_LOG_P;

    // Compute delta = (delta + c1^T * s)
    vec_vec_mult_add(delta_temp, c1_temp, sk_tmp.s, HS, sk_tmp.neg_start);

    // Compute delta = 2/p * delta
    for (uint16_t i = 0; i < LWE_N; ++i) {
        delta_temp[i] += DEC_ADD;
        delta_temp[i] >>= _16_LOG_T;
    }

    // Set delta
    for (uint16_t i = 0; i < DELTA_BYTES; ++i) {
        for (uint8_t j = 0; j < 8; ++j) {
            delta[i] ^= ((uint8_t)(delta_temp[8 * i + j]) << j);
        }
    }
}