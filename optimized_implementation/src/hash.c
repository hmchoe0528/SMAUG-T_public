#include "hash.h"

void shake256_absorb_twice_squeeze(uint8_t *out, size_t out_bytes,
                                   const uint8_t *in1, size_t in1_bytes,
                                   const uint8_t *in2, size_t in2_bytes) {
    keccak_state_avx state;
    shake256_init_avx(&state);
    shake256_absorb_avx(&state, in1, in1_bytes);
    shake256_absorb_avx(&state, in2, in2_bytes);
    shake256_finalize_avx(&state);
    shake256_squeeze_avx(out, out_bytes, &state);
}