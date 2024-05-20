#ifndef KEM_SMAUG_H
#define KEM_SMAUG_H

#include <stdint.h>
#include <stdio.h>

#define cryptolab_smaug1_SECRETKEYBYTES 176
#define cryptolab_smaug1_PUBLICKEYBYTES 672
#define cryptolab_smaug1_CIPHERTEXTBYTES 672
#define cryptolab_smaug1_BYTES 32

int cryptolab_smaug1_crypto_kem_keypair(uint8_t *pk, uint8_t *sk);
int cryptolab_smaug1_crypto_kem_enc(uint8_t *ct, uint8_t *ss,
                                    const uint8_t *pk);
int cryptolab_smaug1_crypto_kem_dec(uint8_t *ss, const uint8_t *sk,
                                    const uint8_t *pk, const uint8_t *ctxt);

#define cryptolab_smaug3_SECRETKEYBYTES 236
#define cryptolab_smaug3_PUBLICKEYBYTES 1088
#define cryptolab_smaug3_CIPHERTEXTBYTES 1024
#define cryptolab_smaug3_BYTES 32

int cryptolab_smaug3_crypto_kem_keypair(uint8_t *pk, uint8_t *sk);
int cryptolab_smaug3_crypto_kem_enc(uint8_t *ct, uint8_t *ss,
                                    const uint8_t *pk);
int cryptolab_smaug3_crypto_kem_dec(uint8_t *ss, const uint8_t *sk,
                                    const uint8_t *pk, const uint8_t *ctxt);
#define cryptolab_smaug5_SECRETKEYBYTES 218
#define cryptolab_smaug5_PUBLICKEYBYTES 1792
#define cryptolab_smaug5_CIPHERTEXTBYTES 1472
#define cryptolab_smaug5_BYTES 32

int cryptolab_smaug5_crypto_kem_keypair(uint8_t *pk, uint8_t *sk);
int cryptolab_smaug5_crypto_kem_enc(uint8_t *ct, uint8_t *ss,
                                    const uint8_t *pk);
int cryptolab_smaug5_crypto_kem_dec(uint8_t *ss, const uint8_t *sk,
                                    const uint8_t *pk, const uint8_t *ctxt);
#endif // KEM_SMAUG_H
