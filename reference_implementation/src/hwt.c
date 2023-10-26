#include "hwt.h"

/*************************************************
 * Name:        hwt
 *
 * Description: Hamming weight sampling deterministically to generate sparse
 *              polynomial r(x) from a seed. shake256 is the Extendable-Output
 *              Function from the SHA-3 family.
 *
 * Arguments:   - uint8_t *res: pointer to ouptput polynomial r(x)
 *                (of length LWE), assumed to be already initialized
 *              - uint8_t *input: pointer to input seed (of length input_size)
 *              - size_t input_size: length of input seed
 **************************************************/
void hwt(uint8_t *res, const uint8_t *input, const size_t input_size,
         const uint16_t hmwt) {
    uint64_t *hash_t = NULL;
    hash_t = (uint64_t *)calloc(LWE_N >> 6, sizeof(uint64_t));

    shake256((uint8_t *)hash_t, LWE_N >> 3, input, input_size);

    uint16_t hw = 0;
    uint16_t hash_idx = 0;

    do {
        uint8_t degree = (uint8_t)hash_t[hash_idx];

        if (res[degree] == 0) {
            res[degree] = ((uint8_t)(hash_t[hash_idx] >> 8) & 0x02) - 1;
            ++hw;
            if (hw == hmwt) {
                ++hash_idx;
                break;
            }
        }

        degree = (uint8_t)(hash_t[hash_idx] >> 10);
        if (res[degree] == 0) {
            res[degree] = ((uint8_t)(hash_t[hash_idx] >> 18) & 0x02) - 1;
            ++hw;
            if (hw == hmwt) {
                ++hash_idx;
                break;
            }
        }

        degree = (uint8_t)(hash_t[hash_idx] >> 20);
        if (res[degree] == 0) {
            res[degree] = ((uint8_t)(hash_t[hash_idx] >> 28) & 0x02) - 1;
            ++hw;
            if (hw == hmwt) {
                ++hash_idx;
                break;
            }
        }

        degree = (uint8_t)(hash_t[hash_idx] >> 30);
        if (res[degree] == 0) {
            res[degree] = ((uint8_t)(hash_t[hash_idx] >> 38) & 0x02) - 1;
            ++hw;
            if (hw == hmwt) {
                ++hash_idx;
                break;
            }
        }

        degree = (uint8_t)(hash_t[hash_idx] >> 40);
        if (res[degree] == 0) {
            res[degree] = ((uint8_t)(hash_t[hash_idx] >> 48) & 0x02) - 1;
            ++hw;
            if (hw == hmwt) {
                ++hash_idx;
                break;
            }
        }

        degree = (uint8_t)(hash_t[hash_idx] >> 50);
        if (res[degree] == 0) {
            res[degree] = ((uint8_t)(hash_t[hash_idx] >> 58) & 0x02) - 1;
            ++hw;
            if (hw == hmwt) {
                ++hash_idx;
                break;
            }
        }

        ++hash_idx;

        if (hash_idx == LWE_N / 64) {
            hash_idx = 0;
            shake256((uint8_t *)hash_t, LWE_N >> 3, (const uint8_t *)hash_t,
                     LWE_N >> 3);
        }
    } while (hw < hmwt);
    free(hash_t);
}