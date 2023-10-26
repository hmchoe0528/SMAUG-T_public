#ifndef SMAUG_KEM_H
#define SMAUG_KEM_H

#include "ciphertext.h"
#include "hash.h"
#include "indcpa.h"
#include "key.h"
#include "parameters.h"
#include "rng.h"
#include "verify.h"

void crypto_kem_keypair(uint8_t *pk, uint8_t *sk);
int crypto_kem_encap(uint8_t *ctxt, uint8_t *ss, const uint8_t *pk);
int crypto_kem_decap(uint8_t *ss, const uint8_t *sk, const uint8_t *pk,
                     const uint8_t *ctxt);

#endif // SMAUG_KEM_H