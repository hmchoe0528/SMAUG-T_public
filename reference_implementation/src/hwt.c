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
void hwt(int16_t *res, const uint8_t *input, const size_t input_size,
         const uint16_t hmwt) {

    uint32_t pos = 0, div = 0, remain = 0;
    uint32_t buf[SHAKE256_RATE * 2] = {0};

    uint8_t xof_block = (hmwt == HS) ? HS_XOF : HR_XOF;
    keccak_state state;
    shake256_init(&state);
    shake256_absorb_once(&state, input, input_size);
    shake256_squeezeblocks((uint8_t *)buf, xof_block, &state);

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
}
