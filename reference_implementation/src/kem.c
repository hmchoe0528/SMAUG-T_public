#include "kem.h"

/*************************************************
 * Name:        KDF
 *
 * Description: Derive a shared secret for given ciphertext and delta.
 *
 * Arguments:   - unsigned char *res: pointer to output shared secret
 *                (an already allocated array of CRYPTO_BYTES bytes)
 *              - ciphertext *ctxt: pointer to input, packed ciphertext
 *              - uint8_t *delta: pointer to input random delta
 *                (an already allocated array of delta_size bytes)
 *              - size_t delta_size: length of input delta
 **************************************************/
int KDF(unsigned char *res, const uint8_t *ctxt, const uint8_t *delta,
        const size_t delta_size) {
    const size_t input_size = delta_size + SHA3_256_HashSize;
    uint8_t input[input_size];

    memcpy(input, delta, DELTA_BYTES);
    hash_h(input + DELTA_BYTES, ctxt, CIPHERTEXT_BYTES);

    shake256(res, CRYPTO_BYTES, input, input_size);
    return 0;
}

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
    // Generate delta
    uint8_t delta[DELTA_BYTES];
    randombytes(delta, DELTA_BYTES);

    indcpa_enc(ctxt, pk, delta);

    if (KDF(ss, ctxt, delta, DELTA_BYTES))
        return 1;

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
    uint8_t delta[DELTA_BYTES] = {0};
    indcpa_dec(delta, sk, ctxt);

    uint8_t ctxt_temp[CIPHERTEXT_BYTES] = {0};
    indcpa_enc(ctxt_temp, pk, delta);

    if ((memcmp(ctxt, ctxt_temp, CIPHERTEXT_BYTES))) {
        printf("*** ERROR: Failed to recover ctxt\n");
        KDF(ss, ctxt, sk + PKE_SECRETKEY_BYTES, T_BYTES);
        return 1;
    }

    // Compute a shared secret key
    if (KDF(ss, ctxt, delta, DELTA_BYTES))
        return 1;

    return 0;
}
