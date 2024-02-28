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

    uint32_t pos = 0;
    uint32_t buf[SHAKE256_RATE * 2] = {0};

    uint8_t xof_block = (hmwt == HS) ? HS_XOF : HR_XOF;
    shake256((uint8_t *)buf, xof_block * SHAKE256_RATE, input, input_size);

    uint32_t dimension_minus_hmwt = DIMENSION - hmwt;
    uint32_t total_iterations = xof_block * 32;
    uint32_t buf_offset = xof_block * 32;
    uint32_t max_degree = 0xffffffff;

    for (size_t i = 0; i < total_iterations; i++) {
        uint32_t deg = buf[i];
        uint32_t remain = max_degree / (dimension_minus_hmwt + pos);
        uint32_t div = max_degree - remain * (dimension_minus_hmwt + pos) + 1;

        if ((max_degree - div) > deg && (pos < hmwt)) {
            res[dimension_minus_hmwt + pos] = res[div];
            res[div] = ((buf[buf_offset + (i >> 4)] >> (i & 0x0f)) & 0x02) - 1;
            pos++;
        } else {
            uint32_t garbage = res[div];
            garbage = ((buf[buf_offset + (i >> 4)] >> (i & 0x0f)) & 0x02) - 1;
        }
    }

    if (pos != hmwt)
        fprintf(stderr, "hwt sampling error\n");

    memset(cnt_arr, 0, sizeof(uint8_t) * (DIMENSION / 8));
    for (size_t i = 0; i < DIMENSION; ++i) {
        uint8_t bit = res[i] & 0x01;
        size_t cnt_arr_idx = (i >> 8) & (-bit);
        cnt_arr[cnt_arr_idx] += bit;
    }
}
