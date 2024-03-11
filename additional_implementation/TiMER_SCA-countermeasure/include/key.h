#ifndef TIMER_KEY_H
#define TIMER_KEY_H

#include <stdint.h>
#include <stdio.h>

#include "dg.h"
#include "fips202.h"
#include "hwt.h"
#include "pack.h"
#include "poly.h"

typedef struct SecretKey {
    sppoly sp_vec[MODULE_RANK];
} secret_key;

typedef struct PublicKey {
    uint8_t seed[PKSEED_BYTES];
    polyvec A[MODULE_RANK];
    polyvec b;
} public_key;

#define genAx TIMER_NAMESPACE(genAx)
void genAx(polyvec A[MODULE_RANK], const unsigned char seed[PKSEED_BYTES]);
#define genBx TIMER_NAMESPACE(genBx)
void genBx(polyvec *b, const polyvec A[MODULE_RANK],
           const sppoly s_vec[MODULE_RANK], const uint8_t e_seed[CRYPTO_BYTES]);
#define genSx_vec TIMER_NAMESPACE(genSx_vec)
void genSx_vec(secret_key *sk, const uint8_t seed[CRYPTO_BYTES]);
#define genPubkey TIMER_NAMESPACE(genPubkey)
void genPubkey(public_key *pk, const secret_key *sk,
               const uint8_t err_seed[CRYPTO_BYTES]);

#define checkSanity TIMER_NAMESPACE(checkSanity)
int checkSanity(const public_key *pk, const secret_key *sk);

#endif // TIMER_KEY_H