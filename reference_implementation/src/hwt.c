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
 **************************************************/
void hwt(uint8_t *res, uint8_t *cnt_arr, const uint8_t *input,
         const size_t input_size, const uint16_t hmwt) {
    uint32_t i, pos = 0;
    uint16_t deg_mask = 0, deg = 0;
    uint16_t buf[SHAKE256_RATE / 2] = {0};

#if SMAUG_MODE == 1
    deg_mask = 0x3ff;
#elif SMAUG_MODE == 3
    deg_mask = 0x7ff;
#elif SMAUG_MODE == 5
    deg_mask = 0xfff;
#endif

    keccak_state state;
    shake256_init(&state);
    shake256_absorb_once(&state, input, input_size);
    shake256_squeezeblocks((uint8_t *)buf, 1, &state);

    for (i = 0; i < DIMENSION; ++i)
        res[i] = 0;

    for (i = DIMENSION - hmwt; i < DIMENSION; ++i) {
        do {
            if (pos >= SHAKE256_RATE / 2) {
                shake256_squeezeblocks((uint8_t *)buf, 1, &state);
                pos = 0;
            }

            deg = buf[pos++] & deg_mask;
        } while (deg > i);

        res[i] = res[deg];
        res[deg] = ((buf[pos - 1] >> 14) & 0x02) - 1;
    }

    size_t cnt_arr_idx = 0;
    for (i = 0; i < DIMENSION; ++i) {
        cnt_arr_idx = ((i & 0x700) >> 8) & (-(res[i] & 0x01));
        cnt_arr[cnt_arr_idx] += (res[i] & 0x01);
    }
}