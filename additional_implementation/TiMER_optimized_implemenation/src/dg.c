#include "dg.h"

////////////////////////////////////////////////////////////////////////////////
///////////////////////////// NOISE DISTRIBUTION ///////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#define RAND_BITS 11 // bits for RND + SIGN

#define SEED_LEN (RAND_BITS * LWE_N / 64) // 64bit seed length

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
    uint16_t j = 0;

    for (size_t i = 0; i < LWE_N; i += 64) {
        uint64_t *x = seed + j;
        uint64_t s[3];
        s[0] = (x[0] & x[1] & x[2] & x[3] & x[5] & x[7] & x[8]) |
               (x[1] & x[2] & x[3] & x[5] & ~x[6] & x[7] & x[9]) |
               (~x[1] & ~x[2] & ~x[3] & x[6] & x[7] & x[8]) |
               (~x[1] & ~x[2] & ~x[3] & ~x[5] & ~x[8] & x[9]) |
               (~x[0] & ~x[2] & ~x[3] & ~x[5] & ~x[8] & x[9]) |
               (x[4] & x[5] & ~x[6] & x[7] & x[9]) |
               (x[3] & x[4] & x[8] & ~x[9]) | (~x[5] & x[6] & x[7] & x[8]) |
               (~x[4] & x[6] & x[7] & x[8]) | (~x[4] & ~x[5] & ~x[8] & x[9]) |
               (x[5] & x[8] & ~x[9]) | (x[6] & x[8] & ~x[9]) |
               (x[7] & x[8] & ~x[9]) | (~x[7] & ~x[8] & x[9]) |
               (~x[6] & ~x[8] & x[9]);
        s[1] = (x[0] & x[1] & x[4] & ~x[5] & x[6] & x[7] & x[9]) |
               (x[2] & x[4] & ~x[5] & x[6] & x[7] & x[9]) |
               (x[3] & x[4] & ~x[5] & x[6] & x[7] & x[9]) |
               (x[5] & x[6] & x[7] & ~x[8] & x[9]) |
               (~x[1] & ~x[2] & ~x[3] & x[8] & x[9]) | (~x[7] & x[8] & x[9]) |
               (~x[6] & x[8] & x[9]) | (~x[5] & x[8] & x[9]) |
               (~x[4] & x[8] & x[9]);
        s[2] = (x[1] & x[4] & x[5] & x[6] & x[7] & x[8] & x[9]) |
               (x[2] & x[4] & x[5] & x[6] & x[7] & x[8] & x[9]) |
               (x[3] & x[4] & x[5] & x[6] & x[7] & x[8] & x[9]);
        for (size_t k = 0; k < 64; ++k) {
            op->coeffs[i + k] = ((s[0] >> k) & 0x01) |
                                (((s[1] >> k) & 0x01) << 1) |
                                (((s[2] >> k) & 0x01) << 2);
            uint16_t sign = (x[10] >> k) & 0x01;
            op->coeffs[i + k] = (((-sign) ^ op->coeffs[i + k]) + sign)
                                << _16_LOG_Q;
        }
        j += RAND_BITS;
    }

    return 0;
}

void addGaussianErrorVec(polyvec *op, const uint8_t seed[CRYPTO_BYTES]) {
    ALIGNED_UINT8(CRYPTO_BYTES + sizeof(size_t)) extseed[4];
    __m256i f;
    f = _mm256_loadu_si256((__m256i *)seed);

    _mm256_store_si256(extseed[0].vec, f);
    _mm256_store_si256(extseed[1].vec, f);

    size_t nonce[2] = {MODULE_RANK * 0, MODULE_RANK * 1};

    cmov(extseed[0].coeffs + CRYPTO_BYTES, (uint8_t *)&nonce[0], sizeof(size_t),
         1);
    cmov(extseed[1].coeffs + CRYPTO_BYTES, (uint8_t *)&nonce[1], sizeof(size_t),
         1);

    ALIGNED_UINT64(SEED_LEN) seed_temp[4];
    shake256x4((uint8_t *)seed_temp[0].coeffs, (uint8_t *)seed_temp[1].coeffs,
               (uint8_t *)seed_temp[2].coeffs, (uint8_t *)seed_temp[3].coeffs,
               SEED_LEN * sizeof(uint64_t), extseed[0].coeffs,
               extseed[1].coeffs, extseed[2].coeffs, extseed[3].coeffs,
               CRYPTO_BYTES + sizeof(size_t));

    addGaussianError(&(op->vec[0]), seed_temp[0].coeffs);
    addGaussianError(&(op->vec[1]), seed_temp[1].coeffs);
}
