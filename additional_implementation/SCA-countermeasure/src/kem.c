#include "kem.h"

/*************************************************
 * Name:        crypto_kem_keypair
 *
 * Description: Generates public and private key
 *              for CCA-secure Module-Lizard key encapsulation mechanism.
 *
 * Arguments:   - public_key *pk: pointer to output public key
 *                (a structure composed of (seed of A, matrix A, vector b))
 *              - secret_key *sk: pointer to output private key
 *                (a structure composed of (vector s, t, vector negstart))
 **************************************************/
void crypto_kem_keypair(uint8_t *pk, uint8_t *sk) {
    indcpa_keypair(pk, sk);
    randombytes(sk + PKE_SECRETKEY_BYTES, T_BYTES);
}

/*************************************************
 * Name:        crypto_kem_encap
 *
 * Description: Generates ciphertext and shared
 *              secret for given public key.
 *
 * Arguments:   - ciphertext *ctxt: pointer to output ciphertext
 *                (a structure composed of (vector c21, c22))
 *              - unsigned char *ss: pointer to output shared secret
 *                (an already allocated array of CRYPTO_BYTES bytes)
 *              - public_key *pk: pointer to output public key
 *                (a structure composed of (seed of A, matrix A, vector b))
 *
 * Returns 0(success) or 1(failure).
 **************************************************/
int crypto_kem_encap(uint8_t *ctxt, uint8_t *ss, const uint8_t *pk) {
    uint8_t mu[DELTA_BYTES] = {0}; // shared secret and seed
    uint8_t buf[DELTA_BYTES + CRYPTO_BYTES] = {0};

    randombytes(mu, DELTA_BYTES);
    hash_h(buf, pk, PUBLICKEY_BYTES);
    hash_g(buf, DELTA_BYTES + CRYPTO_BYTES, mu, DELTA_BYTES, buf,
           SHA3_256_HashSize);

    indcpa_enc(ctxt, pk, mu, buf);
    cmov(ss, buf + DELTA_BYTES, CRYPTO_BYTES, 1);

    return 0;
}

/*************************************************
 * Name:        crypto_kem_decap
 *
 * Description: Generates shared secret for given
 *              ciphertext and private key.
 *
 * Arguments:   - unsigned char *ss: pointer to output shared secret
 *                (an already allocated array of CRYPTO_BYTES bytes)
 *              - secret_key *sk: pointer to input private key
 *                (a structure composed of (vector s, t, vector negstart))
 *              - public_key *pk: pointer to input public key
 *                (a structure composed of (seed of A, matrix A, vector b))
 *              - ciphertext *ctxt: pointer to input ciphertext
 *                (a structure composed of (vector c21, c22))
 *
 * Returns 0(success) or 1(failure).
 * On failure, ss will contain a pseudo-random value.
 **************************************************/
int crypto_kem_decap(uint8_t *ss, const uint8_t *sk, const uint8_t *pk,
                     const uint8_t *ctxt) {
    uint8_t mu[DELTA_BYTES] = {0}; // shared secret and seed
    uint8_t buf[DELTA_BYTES + CRYPTO_BYTES] = {0};
    uint8_t buf_tmp[DELTA_BYTES + CRYPTO_BYTES] = {0};
    uint8_t hash_res[SHA3_256_HashSize] = {0};

    indcpa_dec(mu, sk, ctxt);
    hash_h(hash_res, pk, PUBLICKEY_BYTES);
    hash_g(buf, DELTA_BYTES + CRYPTO_BYTES, mu, DELTA_BYTES, hash_res,
           SHA3_256_HashSize);

    uint8_t ctxt_temp[CIPHERTEXT_BYTES] = {0};
    indcpa_enc(ctxt_temp, pk, mu, buf);

    int fail = verify(ctxt, ctxt_temp, CIPHERTEXT_BYTES);

    hash_h(hash_res, ctxt, CIPHERTEXT_BYTES);
    hash_g(buf_tmp, DELTA_BYTES + CRYPTO_BYTES,
           sk + 2 * MODULE_RANK + SKPOLYVEC_BYTES, T_BYTES, hash_res,
           SHA3_256_HashSize);

    cmov(buf + DELTA_BYTES, buf_tmp + DELTA_BYTES, CRYPTO_BYTES, fail);
    cmov(ss, buf + DELTA_BYTES, CRYPTO_BYTES, 1);
    return 0;
}
