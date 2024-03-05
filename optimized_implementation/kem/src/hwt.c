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

    uint32_t pos = 0, div = 0, remain = 0;
    uint32_t buf[SHAKE256_RATE * 2] = {0};

    uint8_t xof_block = (hmwt == HS) ? HS_XOF : HR_XOF;
    shake256((uint8_t *)buf, xof_block * SHAKE256_RATE, input, input_size);

    for (int i = 0; i < xof_block * 32; i++) {
        uint32_t deg = buf[i];
        remain = 0xffffffff / (DIMENSION - hmwt + pos + 1);
        div = 0xffffffff - remain * (DIMENSION - hmwt + pos + 1);
        div++;
        deg = deg / remain;
        if (((0xffffffff - div) > deg) && pos < hmwt) {
            res[DIMENSION - hmwt + pos] = res[deg];
            res[deg] =
                ((buf[(xof_block * 32 + (i >> 4))] >> (i & 0x0f)) & 0x02) - 1;
            pos++;
        }
    }

    if (pos != hmwt)
        fprintf(stderr, "hwt sampling error\n");

    size_t cnt_arr_idx = 0;
    for (int i = 0; i < DIMENSION; ++i) {
        cnt_arr_idx = ((i & 0x700) >> 8) & (-(res[i] & 0x01));
        cnt_arr[cnt_arr_idx] += (res[i] & 0x01);
    }
}
