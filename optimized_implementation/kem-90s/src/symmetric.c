#include "symmetric.h"
#include "aes256ctr.h"
#include "parameters.h"
#include "sha2.h"
#include "verify.h"

#include <stddef.h>
#include <stdint.h>

void aes256ctr_xof(uint8_t *output, size_t out_bytes, const uint8_t *input,
                   size_t in_bytes) {
    uint8_t buf[AES256CTR_BLOCKBYTES * XOF_MAXBLOCK] = {0};
    size_t nblocks =
        (out_bytes + (AES256CTR_BLOCKBYTES - 1)) / AES256CTR_BLOCKBYTES;
    (void)in_bytes;

    aes256ctr_ctx state;
    aes256ctr_init(&state, input, 0);
    aes256ctr_squeezeblocks(buf, nblocks, &state);
    cmov(output, buf, out_bytes, 1);
}

void sha512_absorb_twice(uint8_t *out, size_t out_bytes, const uint8_t *in1,
                         size_t in1_bytes, const uint8_t *in2,
                         size_t in2_bytes) {
    uint8_t buf[HASH_G_INBYTES] = {0};
    (void)out_bytes;

    for (size_t i = 0; i < in1_bytes; ++i)
        buf[i] = in1[i];
    for (size_t i = 0; i < in2_bytes; ++i)
        buf[in1_bytes + i] = in2[i];

    sha512(out, buf, HASH_G_INBYTES);
}
