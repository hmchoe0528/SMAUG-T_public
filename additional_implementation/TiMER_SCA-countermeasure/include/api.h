#ifndef KEM_TIMER_H
#define KEM_TIMER_H

#include <stdint.h>
#include <stdio.h>

#define timer_SECRETKEYBYTES 144
#define timer_PUBLICKEYBYTES 608
#define timer_CIPHERTEXTBYTES 672
#define timer_BYTES 16

int timer_crypto_kem_keypair(uint8_t *pk, uint8_t *sk);
int timer_crypto_kem_enc(uint8_t *ct, uint8_t *ss, const uint8_t *pk);
int timer_crypto_kem_dec(uint8_t *ss, const uint8_t *ctxt, const uint8_t *sk);

#endif // KEM_TIMER_H
