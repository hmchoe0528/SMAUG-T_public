#ifndef SMAUG_KEY_H
#define SMAUG_KEY_H

#include <stdint.h>
#include <stdio.h>

#include "dg.h"
#include "fips202.h"
#include "hwt.h"
#include "pack.h"
#include "poly.h"

typedef struct SecretKey {
    uint8_t *s[MODULE_RANK];
    uint8_t t[T_BYTES];
    uint8_t neg_start[MODULE_RANK];
    uint8_t cnt_arr[MODULE_RANK];
} secret_key;

typedef struct PublicKey {
    uint8_t seed[PKSEED_BYTES];
    uint16_t A[MODULE_RANK][MODULE_RANK][LWE_N];
    uint16_t b[MODULE_RANK][LWE_N];
} public_key;

void genAx(uint16_t A[MODULE_RANK][MODULE_RANK][LWE_N],
           const unsigned char *seed);
void genSx_vec(secret_key *sk, const uint8_t *seed);
void genPubkey(public_key *pk, const secret_key *sk, const uint8_t *err_seed);

int checkSanity(const public_key *pk, const secret_key *sk);

#endif // SMAUG_KEY_H