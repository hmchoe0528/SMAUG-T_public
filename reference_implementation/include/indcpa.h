#ifndef SMAUG_IND_CPA_PKE_H
#define SMAUG_IND_CPA_PKE_H

#include "ciphertext.h"
#include "hash.h"
#include "hwt.h"
#include "io.h"
#include "key.h"
#include "parameters.h"
#include "verify.h"

void indcpa_keypair(uint8_t pk[PUBLICKEY_BYTES],
                    uint8_t sk[PKE_SECRETKEY_BYTES]);

void indcpa_enc(uint8_t ctxt[CIPHERTEXT_BYTES],
                const uint8_t pk[PUBLICKEY_BYTES],
                const uint8_t mu[DELTA_BYTES], const uint8_t seed[DELTA_BYTES]);

void indcpa_dec(uint8_t delta[DELTA_BYTES],
                const uint8_t sk[PKE_SECRETKEY_BYTES],
                const uint8_t ctxt[CIPHERTEXT_BYTES]);

#endif // SMAUG_IND_CPA_PKE_H