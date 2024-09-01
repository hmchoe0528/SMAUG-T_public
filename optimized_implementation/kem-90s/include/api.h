#ifndef KEM_SMAUG_H
#define KEM_SMAUG_H

#include <stdint.h>
#include <stdio.h>

#define cryptolab_smaug1_SECRETKEYBYTES 160 + 672
#define cryptolab_smaug1_PUBLICKEYBYTES 672
#define cryptolab_smaug1_CIPHERTEXTBYTES 672
#define cryptolab_smaug1_BYTES 32

int cryptolab_smaug1_crypto_kem_keypair(uint8_t *pk, uint8_t *sk);
int cryptolab_smaug1_crypto_kem_enc(uint8_t *ct, uint8_t *ss,
                                    const uint8_t *pk);
int cryptolab_smaug1_crypto_kem_dec(uint8_t *ss, const uint8_t *ctxt,
                                    const uint8_t *sk);

#define cryptolab_smaug3_SECRETKEYBYTES 224 + 1088
#define cryptolab_smaug3_PUBLICKEYBYTES 1088
#define cryptolab_smaug3_CIPHERTEXTBYTES 992
#define cryptolab_smaug3_BYTES 32

int cryptolab_smaug3_crypto_kem_keypair(uint8_t *pk, uint8_t *sk);
int cryptolab_smaug3_crypto_kem_enc(uint8_t *ct, uint8_t *ss,
                                    const uint8_t *pk);
int cryptolab_smaug3_crypto_kem_dec(uint8_t *ss, const uint8_t *ctxt,
                                    const uint8_t *sk);

#define cryptolab_smaug5_SECRETKEYBYTES 352 + 1440
#define cryptolab_smaug5_PUBLICKEYBYTES 1440
#define cryptolab_smaug5_CIPHERTEXTBYTES 1376
#define cryptolab_smaug5_BYTES 32

int cryptolab_smaug5_crypto_kem_keypair(uint8_t *pk, uint8_t *sk);
int cryptolab_smaug5_crypto_kem_enc(uint8_t *ct, uint8_t *ss,
                                    const uint8_t *pk);
int cryptolab_smaug5_crypto_kem_dec(uint8_t *ss, const uint8_t *ctxt,
                                    const uint8_t *sk);
#endif // KEM_SMAUG_H
