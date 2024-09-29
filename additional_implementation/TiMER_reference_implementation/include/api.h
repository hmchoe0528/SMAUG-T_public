#ifndef KEM_TiMER_H
#define KEM_TiMER_H

#include <stdint.h>
#include <stdio.h>

#define TiMER_SECRETKEYBYTES 160 + 672
#define TiMER_PUBLICKEYBYTES 672
#define TiMER_CIPHERTEXTBYTES 608
#define TiMER_BYTES 16

int TiMER_crypto_kem_keypair(uint8_t *pk, uint8_t *sk);
int TiMER_crypto_kem_enc(uint8_t *ct, uint8_t *ss,
                                    const uint8_t *pk);
int TiMER_crypto_kem_dec(uint8_t *ss, const uint8_t *ctxt,
                                    const uint8_t *sk);

#endif // KEM_TiMER_H
