#include "ciphertext.h"
#include "hash.h"
#include "pack.h"

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
void computeC1(uint16_t c1[MODULE_RANK][LWE_N],
               const uint16_t A[MODULE_RANK][MODULE_RANK][LWE_N],
               const uint8_t *r[MODULE_RANK],
               const uint8_t r_cnt_arr[MODULE_RANK],
               const uint8_t r_neg_start[MODULE_RANK]) {
    // c1 = A * r
    matrix_vec_mult_add(c1, A, r, r_cnt_arr, r_neg_start, 1);

    // Rounding q to p
    for (size_t i = 0; i < MODULE_RANK; ++i) {
        for (size_t j = 0; j < LWE_N; ++j) {
            c1[i][j] = ((c1[i][j] + RD_ADD) & RD_AND) >> _16_LOG_P;
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
void computeC2(uint16_t c2[LWE_N], const uint8_t delta[DELTA_BYTES],
               const uint16_t b[MODULE_RANK][LWE_N],
               const uint8_t *r[MODULE_RANK],
               const uint8_t r_cnt_arr[MODULE_RANK],
               const uint8_t r_neg_start[MODULE_RANK]) {
    // c2 = q/2 * delta
    for (size_t i = 0; i < DELTA_BYTES; ++i) {
        for (size_t j = 0; j < sizeof(uint8_t) * 8; ++j) {
            c2[8 * i + j] = (uint16_t)((delta[i] >> j) << _16_LOG_T);
        }
    }

    // c2 = q/2 * delta + (b * r)
    vec_vec_mult_add(c2, b, r, r_cnt_arr, r_neg_start);

    // Rounding q to p'
    for (uint16_t i = 0; i < LWE_N; ++i) {
        c2[i] = ((c2[i] + RD_ADD2) & RD_AND2) >> _16_LOG_P2;
    }
}