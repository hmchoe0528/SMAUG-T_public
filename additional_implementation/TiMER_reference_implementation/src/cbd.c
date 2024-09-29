#include "cbd.h"

/*************************************************
 * Name:        sp_cbd1
 *
 * Description: Given an array of uniformly random bytes, compute
 *              polynomial with coefficients distributed according to
 *              a modified centered binomial distribution with parameter eta=2
 *              (p(0)=3/4, p(1)=p(-1)=1/8)
 *
 * Arguments:   - poly *r: pointer to output polynomial
 *              - const uint8_t *buf: pointer to input byte array
 **************************************************/
static void sp_cbd1(poly *r, const uint8_t buf[CBDSEED_BYTES]) {
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

void poly_cbd(poly *r, const uint8_t buf[CBDSEED_BYTES]) {
    sp_cbd1(r, buf);

}
