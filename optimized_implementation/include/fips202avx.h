#ifndef FIPS202AVX_H
#define FIPS202AVX_H

#include <immintrin.h>
#include <stddef.h>
#include <stdint.h>

#define FIPS202AVX_NAMESPACE(s) fips202avx_##s

#define _ALIGNED_UINT64(N)                                                     \
  union {                                                                      \
    uint64_t s[N];                                                             \
    __m256i vec[(N + 63) / 64];                                                \
  }

typedef _ALIGNED_UINT64(26) keccak_state_avx;

/* shake128 */
#define shake128_init_avx FIPS202AVX_NAMESPACE(shake128_init_avx)
void shake128_init_avx(keccak_state_avx *state);
#define shake128_absorb_avx FIPS202AVX_NAMESPACE(shake128_absorb_avx)
void shake128_absorb_avx(keccak_state_avx *state, const uint8_t *input,
                         size_t inlen);
#define shake128_finalize_avx FIPS202AVX_NAMESPACE(shake128_finalize_avx)
void shake128_finalize_avx(keccak_state_avx *state);
#define shake128_squeeze_avx FIPS202AVX_NAMESPACE(shake128_squeeze_avx)
void shake128_squeeze_avx(uint8_t *output, size_t outlen,
                          keccak_state_avx *state);
#define shake128_avx FIPS202AVX_NAMESPACE(shake128_avx)
void shake128_avx(uint8_t *output, size_t outlen, const uint8_t *input,
                  size_t inlen);

/* shake256 */
#define shake256_init_avx FIPS202AVX_NAMESPACE(shake256_init_avx)
void shake256_init_avx(keccak_state_avx *state);
#define shake256_absorb_avx FIPS202AVX_NAMESPACE(shake256_absorb_avx)
void shake256_absorb_avx(keccak_state_avx *state, const uint8_t *input,
                         size_t inlen);
#define shake256_finalize_avx FIPS202AVX_NAMESPACE(shake256_finalize_avx)
void shake256_finalize_avx(keccak_state_avx *state);
#define shake256_squeeze_avx FIPS202AVX_NAMESPACE(shake256_squeeze_avx)
void shake256_squeeze_avx(uint8_t *output, size_t outlen,
                          keccak_state_avx *state);
#define shake256_avx FIPS202AVX_NAMESPACE(shake256_avx)
void shake256_avx(uint8_t *output, size_t outlen, const uint8_t *input,
                  size_t inlen);

/* sha3-256 */
#define sha3_256_init_avx FIPS202AVX_NAMESPACE(sha3_256_init_avx)
void sha3_256_init_avx(keccak_state_avx *state);
#define sha3_256_absorb_avx FIPS202AVX_NAMESPACE(sha3_256_absorb_avx)
void sha3_256_absorb_avx(keccak_state_avx *state, const uint8_t *input,
                         size_t inlen);
#define sha3_256_finalize_avx FIPS202AVX_NAMESPACE(sha3_256_finalize_avx)
void sha3_256_finalize_avx(uint8_t *output, keccak_state_avx *state);
#define sha3_256_avx FIPS202AVX_NAMESPACE(sha3_256_avx)
void sha3_256_avx(uint8_t *output, const uint8_t *input, size_t inlen);

#endif // FIPS202AVX_H
