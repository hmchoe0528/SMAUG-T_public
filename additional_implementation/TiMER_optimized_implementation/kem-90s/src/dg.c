#include "dg.h"

/*************************************************
 * Name:        load64_littleendian
 *
 * Description: load 8 bytes into a 64-bit integer
 *              in little-endian order
 *
 * Arguments:   - uint64_t *out: pointer to output int64_t array
 *              - int outlen: output length
 *              - uint8_t *in: pointer to input byte array
 **************************************************/
static void load64_littleendian(uint64_t *out, const unsigned int outlen,
                                const uint8_t *in) {
    unsigned int i, pos = 0;
    for (i = 0; i < outlen; ++i) {
        out[i] =
            ((uint64_t)(in[pos])) | ((uint64_t)(in[pos + 1]) << 8) |
            ((uint64_t)(in[pos + 2]) << 16) | ((uint64_t)(in[pos + 3]) << 24) |
            ((uint64_t)(in[pos + 4]) << 32) | ((uint64_t)(in[pos + 5]) << 40) |
            ((uint64_t)(in[pos + 6]) << 48) | ((uint64_t)(in[pos + 7]) << 56);
        pos += 8;
    }
}

////////////////////////////////////////////////////////////////////////////////
///////////////////////////// NOISE DISTRIBUTION ///////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#ifdef NOISE_D1
#define RAND_BITS 10 // bits for RND + SIGN
#define SLEN 2

#endif

#define SEED_LEN (RAND_BITS * LWE_N / 64) // 64-bit seed length
#define SEED_BYTES (SEED_LEN * sizeof(uint64_t))
#define NBLOCKS (SEED_BYTES + (AES256CTR_BLOCKBYTES - 1)) / AES256CTR_BLOCKBYTES

// referenced
// A. Karmakar, S. S. Roy, O. Reparaz, F. Vercauteren and I.
// Verbauwhede, "Constant-Time Discrete Gaussian Sampling," in IEEE Transactions
// on Computers, vol. 67, no. 11, pp. 1561-1571, 1 Nov. 2018,
// doi: 10.1109/TC.2018.2814587.

// for python metacode that generates this code, please refer to notion page
// https://www.notion.so/Constant-Time-Discrete-Gaussian-Sampling-25cc46cdf40549eabd4923d01d8ce259

/*************************************************
 * Name:        addGaussianError
 *
 * Description: Sample discret Gaussian noise e and add e to op
 *
 * Arguments:   - uint16_t *op: pointer to output vector op
 *              - uint64_t *seed: pointer to input seed of SEED_LEN)
 **************************************************/
int addGaussianError(poly *op, uint64_t *seed) {
    unsigned int i = 0, j = 0, k = 0;
    uint64_t s[SLEN] = {0};
    uint64_t *x = NULL;

    for (i = 0; i < LWE_N; i += 64) {
        x = seed + j;
#ifdef NOISE_D1
        s[0] = (x[0] & x[1] & x[2] & x[3] & x[4] & x[5] & x[7] & ~x[8]) |
               (x[0] & x[3] & x[4] & x[5] & x[6] & x[8]) |
               (x[1] & x[3] & x[4] & x[5] & x[6] & x[8]) |
               (x[2] & x[3] & x[4] & x[5] & x[6] & x[8]) |
               (~x[2] & ~x[3] & ~x[6] & x[8]) | (~x[1] & ~x[3] & ~x[6] & x[8]) |
               (x[6] & x[7] & ~x[8]) | (~x[5] & ~x[6] & x[8]) |
               (~x[4] & ~x[6] & x[8]) | (~x[7] & x[8]);
        s[1] = (x[1] & x[2] & x[4] & x[5] & x[7] & x[8]) |
               (x[3] & x[4] & x[5] & x[7] & x[8]) | (x[6] & x[7] & x[8]);
        for (k = 0; k < 64; ++k) {
            op->coeffs[i + k] =
                ((s[0] >> k) & 0x01) | (((s[1] >> k) & 0x01) << 1);
            uint16_t sign = (x[9] >> k) & 0x01;
            op->coeffs[i + k] = (((-sign) ^ op->coeffs[i + k]) + sign)
                                << _16_LOG_Q;
        }
#endif
        j += RAND_BITS;
    }

    return 0;
}

void addGaussianErrorVec(polyvec *op, const uint8_t seed[CRYPTO_BYTES]) {
    unsigned int i;
    uint8_t nonce = 0;
    ALIGNED_UINT64(SEED_LEN) seed_temp;
    ALIGNED_UINT8(NBLOCKS * AES256CTR_BLOCKBYTES) buf;

    aes256ctr_ctx state;
    aes256ctr_init(&state, seed, nonce);
    for (i = 0; i < MODULE_RANK; ++i) {
        aes256ctr_squeezeblocks(buf.coeffs, NBLOCKS, &state);
        state.n = _mm_loadl_epi64((__m128i *)&nonce);
        nonce += MODULE_RANK;

        load64_littleendian(seed_temp.coeffs, SEED_LEN, buf.coeffs);
        addGaussianError(&(op->vec[i]), seed_temp.coeffs);
    }
}
