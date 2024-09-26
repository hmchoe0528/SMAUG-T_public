#ifndef SMAUG_SYMMETRIC_H
#define SMAUG_SYMMETRIC_H

#include "parameters.h"
#include <stdint.h>

#include "aes256ctr.h"
#include "sha2.h"

#define XOF_MAXBLOCK                                                           \
    ((PKPOLYMAT_BYTES + (AES256CTR_BLOCKBYTES - 1)) / AES256CTR_BLOCKBYTES)
#define HASH_G_INBYTES (T_BYTES + SHA_256_HashSize)

#define aes256ctr_xof SMAUG_NAMESPACE(aes256ctr_xof)
void aes256ctr_xof(uint8_t *output, size_t out_bytes, const uint8_t *input,
                   size_t in_bytes);

#define sha512_absorb_twice SMAUG_NAMESPACE(sha512_absorb_twice)
void sha512_absorb_twice(uint8_t *out, size_t out_bytes, const uint8_t *in1,
                         size_t in1_bytes, const uint8_t *in2,
                         size_t in2_bytes);

#define xof(OUT, OUTBYTES, IN, INBYTES)                                        \
    aes256ctr_xof(OUT, OUTBYTES, IN, INBYTES)
#define prf(OUT, OUTBYTES, KEY, NONCE) aes256ctr_prf(OUT, OUTBYTES, KEY, NONCE)

#define hash_h(OUT, IN, INBYTES) sha256(OUT, IN, INBYTES)
#define hash_g(OUT, OUTBYTES, IN1, IN1BYTES, IN2, IN2BYTES)                    \
    sha512_absorb_twice(OUT, OUTBYTES, IN1, IN1BYTES, IN2, IN2BYTES)
#endif // SMAUG_SYMMETRIC_H
