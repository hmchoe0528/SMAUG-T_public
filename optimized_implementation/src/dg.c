#include "dg.h"

////////////////////////////////////////////////////////////////////////////////
///////////////////////////// NOISE DISTRIBUTION ///////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#ifdef NOISE_D1
#define RAND_BITS 10 // bits for RND + SIGN

#endif

#ifdef NOISE_D2
#define RAND_BITS 11 // bits for RND + SIGN

#endif

#ifdef NOISE_D3
#define RAND_BITS 12 // bits for RND + SIGN

#endif

#ifdef NOISE_D4
#define RAND_BITS 11 // bits for RND + SIGN

#endif

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
#ifdef NOISE_D1
        uint64_t s[2];
        s[0] = (x[0] & x[1] & x[2] & x[3] & x[4] & x[5] & x[7] & ~x[8]) |
               (x[0] & x[3] & x[4] & x[5] & x[6] & x[8]) |
               (x[1] & x[3] & x[4] & x[5] & x[6] & x[8]) |
               (x[2] & x[3] & x[4] & x[5] & x[6] & x[8]) |
               (~x[2] & ~x[3] & ~x[6] & x[8]) | (~x[1] & ~x[3] & ~x[6] & x[8]) |
               (x[6] & x[7] & ~x[8]) | (~x[5] & ~x[6] & x[8]) |
               (~x[4] & ~x[6] & x[8]) | (~x[7] & x[8]);
        s[1] = (x[1] & x[2] & x[4] & x[5] & x[7] & x[8]) |
               (x[3] & x[4] & x[5] & x[7] & x[8]) | (x[6] & x[7] & x[8]);
        for (size_t k = 0; k < 64; ++k) {
            op->coeffs[i + k] =
                ((s[0] >> k) & 0x01) | (((s[1] >> k) & 0x01) << 1);
            uint16_t sign = (x[9] >> k) & 0x01;
            op->coeffs[i + k] = (((-sign) ^ op->coeffs[i + k]) + sign)
                                << _16_LOG_Q;
        }
#endif
#ifdef NOISE_D2
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
#endif
#ifdef NOISE_D3
        uint64_t s[3];
        s[0] = (x[0] & ~x[2] & ~x[3] & x[4] & x[6] & x[7] & x[9]) |
               (x[1] & ~x[2] & ~x[3] & x[4] & x[6] & x[7] & x[9]) |
               (~x[0] & ~x[1] & ~x[3] & x[5] & x[6] & x[7] & x[9]) |
               (x[1] & x[2] & x[3] & x[5] & x[6] & x[7] & x[9]) |
               (~x[1] & ~x[2] & ~x[3] & ~x[4] & ~x[7] & x[8] & x[9]) |
               (x[2] & x[4] & ~x[5] & x[6] & x[8] & x[9]) |
               (~x[3] & ~x[4] & ~x[7] & ~x[8] & ~x[9] & x[10]) |
               (x[3] & x[4] & x[7] & x[8] & ~x[10]) |
               (x[3] & x[4] & ~x[5] & x[6] & x[9]) |
               (~x[4] & x[5] & x[6] & x[7] & x[9]) |
               (~x[6] & ~x[7] & ~x[8] & ~x[9] & x[10]) |
               (~x[5] & ~x[7] & ~x[8] & ~x[9] & x[10]) |
               (x[5] & x[7] & x[8] & ~x[10]) | (x[6] & x[7] & x[8] & ~x[10]) |
               (x[5] & x[6] & ~x[8] & x[9]) | (~x[6] & ~x[7] & x[8] & x[9]) |
               (~x[5] & ~x[7] & x[8] & x[9]) | (x[7] & ~x[8] & x[9]) |
               (x[9] & ~x[10]);
        s[1] = (x[0] & x[2] & x[4] & x[5] & x[6] & x[7] & x[10]) |
               (x[1] & x[2] & x[4] & x[5] & x[6] & x[7] & x[10]) |
               (~x[1] & ~x[2] & ~x[3] & ~x[4] & ~x[7] & x[9] & x[10]) |
               (x[3] & x[4] & x[5] & x[6] & x[7] & x[10]) |
               (x[3] & x[5] & x[6] & ~x[8] & x[10]) |
               (x[4] & x[5] & x[6] & ~x[8] & x[10]) |
               (~x[6] & ~x[7] & x[9] & x[10]) | (~x[5] & ~x[7] & x[9] & x[10]) |
               (x[7] & ~x[8] & x[10]) | (x[8] & ~x[9] & x[10]) |
               (~x[8] & x[9] & x[10]);
        s[2] = (x[1] & x[5] & x[6] & x[8] & x[9] & x[10]) |
               (x[2] & x[5] & x[6] & x[8] & x[9] & x[10]) |
               (x[3] & x[5] & x[6] & x[8] & x[9] & x[10]) |
               (x[4] & x[5] & x[6] & x[8] & x[9] & x[10]) |
               (x[7] & x[8] & x[9] & x[10]);
        for (size_t k = 0; k < 64; ++k) {
            op->coeffs[i + k] = ((s[0] >> k) & 0x01) |
                                (((s[1] >> k) & 0x01) << 1) |
                                (((s[2] >> k) & 0x01) << 2);
            uint16_t sign = (x[11] >> k) & 0x01;
            op->coeffs[i + k] = (((-sign) ^ op->coeffs[i + k]) + sign)
                                << _16_LOG_Q;
        }
#endif
#ifdef NOISE_D4
        uint64_t s[4];

        s[0] = (x[0] & x[1] & ~x[2] & x[3] & x[4] & ~x[6] & x[7] & ~x[9]) |
               (x[2] & x[3] & x[4] & ~x[5] & ~x[6] & x[7] & ~x[9]) |
               (~x[2] & ~x[3] & ~x[4] & ~x[5] & ~x[7] & x[8]) |
               (~x[1] & ~x[3] & ~x[4] & ~x[5] & ~x[7] & x[8]) |
               (~x[0] & ~x[3] & ~x[4] & ~x[5] & ~x[7] & x[8]) |
               (x[0] & ~x[2] & x[3] & x[5] & ~x[6] & x[8]) |
               (x[1] & ~x[2] & x[3] & x[5] & ~x[6] & x[8]) |
               (x[2] & x[3] & ~x[4] & x[5] & ~x[6] & x[8]) |
               (x[0] & x[1] & x[4] & x[5] & x[7] & x[9]) |
               (x[2] & ~x[3] & x[4] & x[6] & x[7] & x[9]) |
               (~x[2] & x[3] & x[4] & x[6] & x[7] & x[9]) |
               (~x[3] & x[5] & ~x[6] & x[7] & ~x[9]) |
               (x[0] & x[2] & x[5] & x[7] & ~x[8]) |
               (x[1] & x[2] & x[5] & x[7] & ~x[8]) |
               (x[4] & x[5] & ~x[6] & x[7] & x[9]) |
               (~x[4] & ~x[5] & x[6] & x[7] & x[9]) |
               (~x[2] & ~x[5] & x[6] & x[7] & x[9]) |
               (x[3] & x[5] & x[7] & ~x[8]) | (~x[5] & ~x[6] & x[8] & ~x[9]) |
               (~x[2] & ~x[6] & x[8] & ~x[9]) | (x[6] & x[7] & ~x[8]) |
               (~x[7] & x[8] & ~x[9]) | (~x[6] & ~x[7] & x[8]);
        s[1] = (x[2] & x[3] & x[4] & x[5] & x[7] & x[8] & ~x[9]) |
               (x[2] & x[3] & x[4] & ~x[5] & x[6] & x[7] & x[9]) |
               (~x[2] & ~x[3] & ~x[4] & ~x[5] & ~x[7] & x[9]) |
               (~x[1] & ~x[3] & ~x[4] & ~x[5] & ~x[7] & x[9]) |
               (~x[0] & ~x[3] & ~x[4] & ~x[5] & ~x[7] & x[9]) |
               (~x[4] & x[5] & x[6] & x[7] & x[9]) |
               (~x[3] & x[5] & x[6] & x[7] & x[9]) |
               (~x[2] & x[5] & x[6] & x[7] & x[9]) |
               (x[6] & x[7] & x[8] & ~x[9]) | (~x[6] & ~x[7] & x[9]) |
               (~x[8] & x[9]);
        s[2] = (x[0] & x[1] & x[2] & ~x[3] & x[6] & x[8] & x[9]) |
               (x[3] & x[6] & ~x[7] & x[8] & x[9]) |
               (x[4] & ~x[5] & x[6] & x[8] & x[9]) |
               (~x[3] & x[5] & x[6] & x[8] & x[9]) |
               (~x[6] & x[7] & x[8] & x[9]) | (~x[4] & x[7] & x[8] & x[9]) |
               (~x[2] & x[7] & x[8] & x[9]);
        s[3] = (x[2] & x[3] & x[4] & x[5] & x[6] & x[7] & x[8] & x[9]);
        for (size_t k = 0; k < 64; ++k) {
            op->coeffs[i + k] =
                ((s[0] >> k) & 0x01) | (((s[1] >> k) & 0x01) << 1) |
                (((s[2] >> k) & 0x01) << 2) | (((s[3] >> k) & 0x01) << 3);
            uint16_t sign = (x[10] >> k) & 0x01;
            op->coeffs[i + k] = (((-sign) ^ op->coeffs[i + k]) + sign)
                                << _16_LOG_Q;
        }
#endif
        j += RAND_BITS;
    }

    return 0;
}

void addGaussianErrorVec(polyvec *op, const uint8_t seed[CRYPTO_BYTES]) {
    ALIGNED_UINT8(CRYPTO_BYTES + sizeof(size_t)) extseed[4];
    __m256i f;
    f = _mm256_loadu_si256((__m256i *)seed);

#if MODULE_RANK == 2
    _mm256_store_si256(extseed[0].vec, f);
    _mm256_store_si256(extseed[1].vec, f);

    size_t nonce[2] = {MODULE_RANK * 0, MODULE_RANK * 1};

    cmov(extseed[0].coeffs + CRYPTO_BYTES, (uint8_t *)&nonce[0], sizeof(size_t),
         1);
    cmov(extseed[1].coeffs + CRYPTO_BYTES, (uint8_t *)&nonce[1], sizeof(size_t),
         1);

    ALIGNED_UINT64(SEED_LEN) seed_temp[4];
    shake128x4((uint8_t *)seed_temp[0].coeffs, (uint8_t *)seed_temp[1].coeffs,
               (uint8_t *)seed_temp[2].coeffs, (uint8_t *)seed_temp[3].coeffs,
               SEED_LEN * sizeof(uint64_t), extseed[0].coeffs,
               extseed[1].coeffs, extseed[2].coeffs, extseed[3].coeffs,
               CRYPTO_BYTES + sizeof(size_t));

    addGaussianError(&(op->vec[0]), seed_temp[0].coeffs);
    addGaussianError(&(op->vec[1]), seed_temp[1].coeffs);
#elif MODULE_RANK == 3
    _mm256_store_si256(extseed[0].vec, f);
    _mm256_store_si256(extseed[1].vec, f);
    _mm256_store_si256(extseed[2].vec, f);

    size_t nonce[3] = {MODULE_RANK * 0, MODULE_RANK * 1, MODULE_RANK * 2};

    cmov(extseed[0].coeffs + CRYPTO_BYTES, (uint8_t *)&nonce[0], sizeof(size_t),
         1);
    cmov(extseed[1].coeffs + CRYPTO_BYTES, (uint8_t *)&nonce[1], sizeof(size_t),
         1);
    cmov(extseed[2].coeffs + CRYPTO_BYTES, (uint8_t *)&nonce[2], sizeof(size_t),
         1);

    ALIGNED_UINT64(SEED_LEN) seed_temp[4];
    shake128x4((uint8_t *)seed_temp[0].coeffs, (uint8_t *)seed_temp[1].coeffs,
               (uint8_t *)seed_temp[2].coeffs, (uint8_t *)seed_temp[3].coeffs,
               SEED_LEN * sizeof(uint64_t), extseed[0].coeffs,
               extseed[1].coeffs, extseed[2].coeffs, extseed[3].coeffs,
               CRYPTO_BYTES + sizeof(size_t));

    addGaussianError(&(op->vec[0]), seed_temp[0].coeffs);
    addGaussianError(&(op->vec[1]), seed_temp[1].coeffs);
    addGaussianError(&(op->vec[2]), seed_temp[2].coeffs);
#elif MODULE_RANK == 5
    _mm256_store_si256(extseed[0].vec, f);
    _mm256_store_si256(extseed[1].vec, f);
    _mm256_store_si256(extseed[2].vec, f);
    _mm256_store_si256(extseed[3].vec, f);

    // for (0 <= i < 5)
    size_t nonce[5] = {MODULE_RANK * 0, MODULE_RANK * 1, MODULE_RANK * 2,
                       MODULE_RANK * 3, MODULE_RANK * 4};

    cmov(extseed[0].coeffs + CRYPTO_BYTES, (uint8_t *)&nonce[0], sizeof(size_t),
         1);
    cmov(extseed[1].coeffs + CRYPTO_BYTES, (uint8_t *)&nonce[1], sizeof(size_t),
         1);
    cmov(extseed[2].coeffs + CRYPTO_BYTES, (uint8_t *)&nonce[2], sizeof(size_t),
         1);
    cmov(extseed[3].coeffs + CRYPTO_BYTES, (uint8_t *)&nonce[3], sizeof(size_t),
         1);

    ALIGNED_UINT64(SEED_LEN) seed_temp[5];
    shake128x4((uint8_t *)seed_temp[0].coeffs, (uint8_t *)seed_temp[1].coeffs,
               (uint8_t *)seed_temp[2].coeffs, (uint8_t *)seed_temp[3].coeffs,
               SEED_LEN * sizeof(uint64_t), extseed[0].coeffs,
               extseed[1].coeffs, extseed[2].coeffs, extseed[3].coeffs,
               CRYPTO_BYTES + sizeof(size_t));

    // for remaining (i = 5)
    cmov(extseed[3].coeffs + CRYPTO_BYTES, (uint8_t *)&nonce[4], sizeof(size_t),
         1);
    shake128((uint8_t *)seed_temp[4].coeffs, SEED_LEN * sizeof(uint64_t),
             extseed[3].coeffs, CRYPTO_BYTES + sizeof(size_t));

    addGaussianError(&(op->vec[0]), seed_temp[0].coeffs);
    addGaussianError(&(op->vec[1]), seed_temp[1].coeffs);
    addGaussianError(&(op->vec[2]), seed_temp[2].coeffs);
    addGaussianError(&(op->vec[3]), seed_temp[3].coeffs);
    addGaussianError(&(op->vec[4]), seed_temp[4].coeffs);
#endif
}