#ifndef TIMER_KEM_H
#define TIMER_KEM_H

#include "ciphertext.h"
#include "hash.h"
#include "indcpa.h"
#include "key.h"
#include "parameters.h"
#include "randombytes.h"
#include "verify.h"

#define crypto_kem_keypair TIMER_NAMESPACE(crypto_kem_keypair)
void crypto_kem_keypair(uint8_t *pk, uint8_t *sk);
#define crypto_kem_encap TIMER_NAMESPACE(crypto_kem_encap)
int crypto_kem_encap(uint8_t *ctxt, uint8_t *ss, const uint8_t *pk);
#define crypto_kem_decap TIMER_NAMESPACE(crypto_kem_decap)
int crypto_kem_decap(uint8_t *ss, const uint8_t *sk, const uint8_t *pk,
                     const uint8_t *ctxt);

#endif // TIMER_KEM_H