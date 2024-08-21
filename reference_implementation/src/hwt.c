#include "hwt.h"
#include <stdio.h>

/*************************************************
 * Name:        load16_littleendian
 *
 * Description: load 2 bytes into a 16-bit integer
 *              in little-endian order
 *
 * Arguments:   - uint16_t *out: pointer to output int16_t array
 *              - int outlen: output length
 *              - uint8_t *in: pointer to input byte array
 **************************************************/
static void load16_littleendian(uint16_t *out, const int outlen,
                                const uint8_t *in) {
    int pos = 0;
    for (int i = 0; i < outlen; ++i) {
        out[i] = ((uint16_t)(in[pos])) | ((uint16_t)(in[pos + 1]) << 8);
        pos += 2;
    }
}

// referenced
// Décio Luiz Gazzoni Filho and Tomás S. R. Silva and Julio López
// “Efficient isochronous fixed-weight sampling with applications to {NTRU},” in
// Cryptology {ePrint} Archive, Paper 2024/548. 2024,
// url: eprint.iacr.org/2024/548.
/*************************************************
 * Name:        rejsampling_mod
 *
 * Description: Sample array of random integers such that res[i] is in the range
 *              [0, LWE_N - i] for 0 < i < LWE_N
 *
 * Arguments:   - uint8_t *res: pointer to ouptput polynomial r(x)
 *                (of length LWE), assumed to be already initialized
 *              - uint8_t *seed: pointer to input seed (of length
 *input_size)
 **************************************************/
static void rejsampling_mod(int16_t res[LWE_N], const uint16_t *rand) {
    unsigned int i, j = LWE_N;
    uint32_t m;
    uint16_t s, t, l;

    for (i = 0; i < LWE_N; i++) {
        s = LWE_N - i;
        t = 65536 % s;

        m = (uint32_t)rand[i] * s;
        l = m;

        while (l < t) {
            m = (uint32_t)rand[j++] * s;
            l = m;
        }

        res[i] = m >> 16;
    }
}

/*************************************************
 * Name:        hwt
 *
 * Description: Hamming weight sampling deterministically to generate sparse
 *              polynomial r(x) from a seed. shake256 is the Extendable-Output
 *              Function from the SHA-3 family.
 *
 * Arguments:   - int16_t *res: pointer to ouptput polynomial r(x)
 *                (of length LWE), assumed to be already initialized
 *              - uint8_t *seed: pointer to input seed (of length CRYPTO_BYTES)
 **************************************************/
void hwt(int16_t *res, const uint8_t *seed) {
    unsigned int i;
    int16_t si[LWE_N] = {0};
    uint16_t rand[HWTSEEDBYTES / 2] = {0};
    uint8_t sign[LWE_N / 4] = {0};
    uint8_t buf[SHAKE256_RATE * 5] = {0};

    keccak_state state;
    shake256_init(&state);
    shake256_absorb_once(&state, seed, CRYPTO_BYTES + 1);
    shake256_squeezeblocks(buf, 5, &state);

    load16_littleendian(rand, HWTSEEDBYTES / 2, buf);
    memcpy(sign, buf + HWTSEEDBYTES, LWE_N / 4);

    rejsampling_mod(si, rand);

    int t0, t1;
    int c0 = LWE_N - HS, c01 = LWE_N;
    for (i = 0; i < LWE_N; i++) {
        t0 = (si[i] - c0) >> 31;
        t1 = (si[i] - c01) >> 31;

        c0 += t0;
        c01 += t1;

        res[i] = 2 + t0 + t1;
    }

    for (i = 0; i < LWE_N / 4; ++i) {
        res[4 * i] = (-res[4 * i]) & ((sign[i] & 0x02) - 1);
        res[4 * i + 1] = (-res[4 * i + 1]) & (((sign[i] >> 1) & 0x02) - 1);
        res[4 * i + 2] = (-res[4 * i + 2]) & (((sign[i] >> 2) & 0x02) - 1);
        res[4 * i + 3] = (-res[4 * i + 3]) & (((sign[i] >> 3) & 0x02) - 1);
    }
}
