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
    keccak_state state;
    shake256_init(&state);
    shake256_absorb_once(&state, input, input_size);
    shake256_squeezeblocks((uint8_t *)buf, xof_block, &state);

    uint32_t div;
    uint32_t garbage;
    for (i = 0; i < xof_block * 32; i++) {
        uint32_t deg = buf[i];
        uint32_t remain;
        remain = 0xfffffffff / (DIMENSION - hmwt + pos);
        div = 0xffffffff - remain * (DIMENSION - hmwt + pos);
        div++;
        if (((0xffffffff - div) > deg) && (pos < hmwt)) {
            res[DIMENSION - hmwt + pos] = res[div];
            res[div] =
                ((buf[(xof_block * 32 + (i >> 4))] >> (i & 0x0f)) & 0x02) - 1;
            pos++;
        } else {
            garbage = res[div];
            garbage =
                ((buf[(xof_block * 32 + (i >> 4))] >> (i & 0x0f)) & 0x02) - 1;
        }
    }

    if (pos != hmwt)
        fprintf(stderr, "hwt sampling error\n");

    size_t cnt_arr_idx = 0;
    for (i = 0; i < DIMENSION; ++i) {
        cnt_arr_idx = ((i & 0x700) >> 8) & (-(res[i] & 0x01));
        cnt_arr[cnt_arr_idx] += (res[i] & 0x01);
    }
}
