#ifndef SMAUG_HASH_H
#define SMAUG_HASH_H

#include "fips202.h"
#include "parameters.h"

#define shake256_absorb_twice_squeeze                                          \
    SMAUG_NAMESPACE(shake256_absorb_twice_squeeze)
void shake256_absorb_twice_squeeze(uint8_t *out, size_t out_bytes,
                                   const uint8_t *in1, size_t in1_bytes,
                                   const uint8_t *in2, size_t in2_bytes);

#define shake256_prf SMAUG_NAMESPACE(shake256_prf)
void shake256_prf(uint8_t *out, size_t outlen, const uint8_t key[CRYPTO_BYTES],
                  uint8_t nonce);

#endif // SMAUG_HASH_H
