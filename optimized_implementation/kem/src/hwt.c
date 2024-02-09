#include "hwt.h"
#include <stdio.h>

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
 *              - uint16_t hmwt: hamming weight to sample
 **************************************************/
void hwt(uint8_t *res, uint8_t *cnt_arr, const uint8_t *input,
         const size_t input_size, const uint16_t hmwt) {

    uint32_t i = 0, pos = 0;
    uint32_t buf[SHAKE256_RATE * 2] = {0};

    uint8_t xof_block = (hmwt == HS) ? HS_XOF : HR_XOF;
    shake256((uint8_t *)buf, xof_block * SHAKE256_RATE, input, input_size);

    for (i = DIMENSION - hmwt; i < DIMENSION; ++i) {
        uint64_t deg_tmp = 0;
        deg_tmp = (uint64_t)buf[pos] * (i + 1);
        uint32_t deg = (uint32_t)(deg_tmp >> 32);

        res[i] = res[deg];
        res[deg] = ((buf[(hmwt + (pos >> 4))] >> (pos & 0x0f)) & 0x02) - 1;
        pos++;
    }

    size_t cnt_arr_idx = 0;
    for (i = 0; i < DIMENSION; ++i) {
        cnt_arr_idx = ((i & 0x700) >> 8) & (-(res[i] & 0x01));
        cnt_arr[cnt_arr_idx] += (res[i] & 0x01);
    }
}
