#ifndef SMAUG_POLY_H
#define SMAUG_POLY_H

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

#define convToIdx SMAUG_NAMESPACE(convToIdx)
uint8_t convToIdx(uint8_t *res, const uint8_t res_length, const uint8_t *op,
                  const size_t op_length);

#define poly_mult_add SMAUG_NAMESPACE(poly_mult_add)
void poly_mult_add(poly *res, const poly *op1, const sppoly *op2);
#define poly_mult_sub SMAUG_NAMESPACE(poly_mult_sub)
void poly_mult_sub(poly *res, const poly *op1, const sppoly *op2);
#define vec_vec_mult_add SMAUG_NAMESPACE(vec_vec_mult_add)
void vec_vec_mult_add(poly *res, const polyvec *op1,
                      const sppoly op2[MODULE_RANK]);
#define matrix_vec_mult_add SMAUG_NAMESPACE(matrix_vec_mult_add)
void matrix_vec_mult_add(polyvec *res, const polyvec op1[MODULE_RANK],
                         const sppoly op2[MODULE_RANK], int16_t transpose);
#define matrix_vec_mult_sub SMAUG_NAMESPACE(matrix_vec_mult_sub)
void matrix_vec_mult_sub(polyvec *res, const polyvec op1[MODULE_RANK],
                         const sppoly op2[MODULE_RANK], int16_t transpose);

void poly_reduce_keyGen(poly *res, const uint16_t temp[2 * LWE_N]);
void FisherYates(uint8_t *arr, int n);


#endif // SMAUG_POLY_H
