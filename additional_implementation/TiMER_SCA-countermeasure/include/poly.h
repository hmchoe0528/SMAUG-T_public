#ifndef TIMER_POLY_H
#define TIMER_POLY_H

#include "parameters.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

typedef struct {
    uint16_t coeffs[LWE_N];
} poly;

typedef struct {
    poly vec[MODULE_RANK];
} polyvec;

typedef struct {
    uint8_t *sx;
    uint8_t neg_start;
    uint8_t cnt;
} sppoly; // sparse poly

uint16_t flipabs(uint16_t x);
void poly_frommsg(poly *r, const uint8_t *msg);
void poly_tomsg(unsigned char *msg, const poly *x);
void poly_reduce_keyGen(poly *res, const uint16_t temp[2 * LWE_N]);
void FisherYates(uint8_t *arr, int n);

#define convToIdx TIMER_NAMESPACE(convToIdx)
uint8_t convToIdx(uint8_t *res, const uint8_t res_length, const uint8_t *op,
                  const size_t op_length);

#define poly_mult_add TIMER_NAMESPACE(poly_mult_add)
void poly_mult_add(poly *res, const poly *op1, const sppoly *op2);
#define poly_mult_sub TIMER_NAMESPACE(poly_mult_sub)
void poly_mult_sub(poly *res, const poly *op1, const sppoly *op2);
#define vec_vec_mult_add TIMER_NAMESPACE(vec_vec_mult_add)
void vec_vec_mult_add(poly *res, const polyvec *op1,
                      const sppoly op2[MODULE_RANK]);
#define matrix_vec_mult_add TIMER_NAMESPACE(matrix_vec_mult_add)
void matrix_vec_mult_add(polyvec *res, const polyvec op1[MODULE_RANK],
                         const sppoly op2[MODULE_RANK], int16_t transpose);
#define matrix_vec_mult_sub TIMER_NAMESPACE(matrix_vec_mult_sub)
void matrix_vec_mult_sub(polyvec *res, const polyvec op1[MODULE_RANK],
                         const sppoly op2[MODULE_RANK], int16_t transpose);

#endif // TIMER_POLY_H
