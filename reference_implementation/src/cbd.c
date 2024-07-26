#include "cbd.h"

#if SMAUG_MODE == 3
/*************************************************
 * Name:        load32_littleendian
 *
 * Description: load 4 bytes into a 32-bit integer
 *              in little-endian order
 *
 * Arguments:   - const uint8_t *x: pointer to input byte array
 *
 * Returns 32-bit unsigned integer loaded from x
 **************************************************/
static uint32_t load32_littleendian(const uint8_t x[4]) {
    uint32_t r;
    r = (uint32_t)x[0];
    r |= (uint32_t)x[1] << 8;
    r |= (uint32_t)x[2] << 16;
    r |= (uint32_t)x[3] << 24;
    return r;
}

/*************************************************
 * Name:        cbd
 *
 * Description: Given an array of uniformly random bytes, compute
 *              polynomial with coefficients distributed according to
 *              a centered binomial distribution with parameter eta=2
 *
 * Arguments:   - poly *r: pointer to output polynomial
 *              - const uint8_t *buf: pointer to input byte array
 **************************************************/
static void cbd(poly *r, const uint8_t buf[CBDSEED_BYTES]) {
    unsigned int i, j;
    uint32_t t, d;
    int16_t a, b;

    for (i = 0; i < LWE_N / 8; i++) {
        t = load32_littleendian(buf + 4 * i);
        d = t & 0x55555555;
        d += (t >> 1) & 0x55555555;

        for (j = 0; j < 8; j++) {
            a = (d >> (4 * j + 0)) & 0x3;
            b = (d >> (4 * j + 2)) & 0x3;
            r->coeffs[8 * i + j] = a - b;
        }
    }
}
#endif

#if SMAUG_MODE == 1
/*************************************************
 * Name:        mcbd1
 *
 * Description: Given an array of uniformly random bytes, compute
 *              polynomial with coefficients distributed according to
 *              a modified centered binomial distribution with parameter eta=2
 *              (p(0)=3/4, p(1)=p(-1)=1/8)
 *
 * Arguments:   - poly *r: pointer to output polynomial
 *              - const uint8_t *buf: pointer to input byte array
 **************************************************/
static void mcbd1(poly *r, const uint8_t buf[CBDSEED_BYTES]) {
    unsigned int i, j;
    uint8_t t, s;
    int16_t d;

    for (i = 0; i < LWE_N / 8; i++) {
        t = buf[3 * i] & buf[3 * i + 1];
        s = buf[3 * i + 2];

        for (j = 0; j < 8; j++) {
            d = (t >> j) & 0x01;
            r->coeffs[8 * i + j] = d * ((((s >> j) << 1) & 0x02) - 1);
        }
    }
}
#endif

#if SMAUG_MODE == 5
/*************************************************
 * Name:        mcbd2
 *
 * Description: Given an array of uniformly random bytes, compute
 *              polynomial with coefficients distributed according to
 *              a modified centered binomial distribution with parameter eta=2
 *              (p(0)=5/8, p(1)=p(-1)=3/16)
 *
 * Arguments:   - poly *r: pointer to output polynomial
 *              - const uint8_t *buf: pointer to input byte array
 **************************************************/
static void mcbd2(poly *r, const uint8_t buf[CBDSEED_BYTES]) {
    unsigned int i, j;
    uint8_t t, s;
    int16_t d;

    for (i = 0; i < LWE_N / 8; i++) {
        t = buf[4 * i] & (buf[4 * i + 1] | buf[4 * i + 2]);
        s = buf[4 * i + 3];

        for (j = 0; j < 8; j++) {
            d = (t >> j) & 0x01;
            r->coeffs[8 * i + j] = d * ((((s >> j) << 1) & 0x02) - 1);
        }
    }
}
#endif

void poly_cbd(poly *r, const uint8_t buf[CBDSEED_BYTES]) {
#if SMAUG_MODE == 1
    mcbd1(r, buf);
#elif SMAUG_MODE == 3
    cbd(r, buf);
#elif SMAUG_MODE == 5
    mcbd2(r, buf);
#endif
}
