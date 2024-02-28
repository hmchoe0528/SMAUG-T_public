#include "ciphertext.h"
#include "hash.h"
#include "pack.h"
#include "parameters.h"
#include "poly.h"

/*************************************************
 * Name:        computeC1
 *
 * Description: Compute the first ciphertext c1 = round(p/q * (A * r))
 *
 * Arguments:   - uint8_t *c1: pointer to ouptput vector c1. The c1 should be
 *                             already initialized
 *              - uint16_t A[][][]: pointer to input matrix A
 *              - uint8_t r[][]: pointer to input vecetor of ephemeral key r
 *              - uint8_t *r_neg_start: pointer to input vector of neg_start
 **************************************************/
void computeC1(polyvec *c1, const polyvec A[MODULE_RANK],
               const sppoly r[MODULE_RANK]) {
    // c1 = A * r
    matrix_vec_mult_add(c1, A, r, 1);

    // Rounding q to p
    for (size_t i = 0; i < MODULE_RANK; ++i) {
        for (size_t j = 0; j < LWE_N; ++j) {
            c1->vec[i].coeffs[j] =
                ((c1->vec[i].coeffs[j] + RD_ADD) & RD_AND) >> _16_LOG_P;
        }
    }
}

/*************************************************
 * Name:        computeC2
 *
 * Description: Compute the secode ciphertext c2 = round(p/q * (b^T * r) + msg)
 *
 * Arguments:   - uint8_t *c2: pointer to ouptput poly c2. The c2 should be
 *                             already initialized
 *              - uint8_t delta: pointer to input message delta
 *              - uint8_t r[][]: pointer to input vecetor of ephemeral key r
 *              - uint8_t *r_neg_start: pointer to input vector of neg_start
 **************************************************/
void computeC2(poly *c2, const uint8_t delta[MSG_BYTES], const polyvec *b,
               const sppoly r[MODULE_RANK]) {
    // c2 = q/2 * delta
    #if MSG_BYTES == 16
    poly_frommsg(c2, delta);
	#elif MSG_BYTES == 32    
    for (size_t i = 0; i < MSG_BYTES; ++i) {
        for (size_t j = 0; j < sizeof(uint8_t) * 8; ++j) {
            c2->coeffs[8 * i + j] = (uint16_t)((delta[i] >> j) << _16_LOG_T);     //_16_LOG_T = 2^(16-1)     
        }
    }
 	#endif

    // c2 = q/2 * delta + (b * r)
    vec_vec_mult_add(c2, b, r);

    // Rounding q to p'
    for (uint16_t i = 0; i < LWE_N; ++i) {
        c2->coeffs[i] = ((c2->coeffs[i] + RD_ADD2) & RD_AND2) >> _16_LOG_P2;
    }
}
