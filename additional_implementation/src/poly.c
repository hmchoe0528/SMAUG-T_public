#include "poly.h"

static uint16_t flipabs(uint16_t x);

/*************************************************
 * Name:        poly_frommsg
 *
 * Description: Convert 16-byte message to polynomial
 *
 * Arguments:   - poly *r:                  pointer to output polynomial
 *              - const unsigned char *msg: pointer to input message
 **************************************************/
void poly_frommsg(poly *r, const uint8_t *msg) {
    unsigned int mask;
    unsigned Mod = Modulus_Q >> 1;
    for (size_t i = 0; i < MSG_BYTES; i++) {
        for (size_t j = 0; j < 8; j++) {
            mask = (msg[i] >> j) & 1;
            mask = (mask * Mod) & Mod;
            r->coeffs[8 * i + j + 0] = mask;
            r->coeffs[8 * i + j + 128] = mask;
        }
    }
}

/*************************************************
 * Name:        poly_tomsg
 *
 * Description: Convert polynomial to 16-byte message
 *
 * Arguments:   - unsigned char *msg: pointer to output message
 *              - const poly *x:      pointer to input polynomial
 **************************************************/
void poly_tomsg(unsigned char *msg, const poly *x) {
    uint16_t t;
    unsigned Mod = Modulus_Q >> 1;
    for (size_t i = 0; i < MSG_BYTES; i++)
        msg[i] = 0;

    for (size_t i = 0; i < LWE_N / 2; i++) {
        t = flipabs(x->coeffs[i + 0]);
        t += flipabs(x->coeffs[i + 128]);
        t = t - Mod;
        t >>= 15;
        msg[i >> 3] |= t << (i & 7);
    }
}

/*************************************************
 * Name:        flipabs
 *
 * Description: Computes |(x mod q) - Q/2|
 *
 * Arguments:   uint16_t x: input coefficient
 *
 * Returns |(x mod q) - Q/2|
 **************************************************/
static uint16_t flipabs(uint16_t x) {
    int16_t r, m;
    r = x - (Modulus_Q >> 1);
    m = r >> 15;
    return (r + m) ^ m;
}

/*************************************************
 * Name:        poly_add
 *
 * Description: Compute res += op1 * X^deg.
 *
 * Arguments:   - uint16_t *res: pointer to output polynomial
 *              - uint16_t *op1: pointer to input polynomial
 *              - uint16_t deg: degree from sparse polynomial
 **************************************************/
inline void __attribute__((always_inline))
poly_add(uint16_t res[2 * LWE_N], const poly *op1, const uint8_t deg) {
    for (size_t i = 0; i < LWE_N; ++i)
        res[deg + i] += op1->coeffs[i];
}

/*************************************************
 * Name:        poly_sub
 *
 * Description: Compute res -= op1 * X^deg.
 *
 * Arguments:   - uint16_t *res: pointer to output polynomial
 *              - uint16_t *op1: pointer to input polynomial
 *              - uint16_t deg: degree from sparse polynomial
 **************************************************/
inline void __attribute__((always_inline))
poly_sub(uint16_t res[2 * LWE_N], const poly *op1, const uint8_t deg) {
    for (size_t i = 0; i < LWE_N; ++i)
        res[deg + i] -= op1->coeffs[i];
}

/*************************************************
 * Name:        poly_reduce
 *
 * Description: Reduce the input poly temp with (X^LWE_N + 1) and add it to res
 *
 * Arguments:   - uint16_t *res: pointer to output polynomial
 *              - uint16_t *temp: pointer to input polynomial
 *              - uint16_t deg: degree from sparse polynomial
 **************************************************/
inline void __attribute__((always_inline))
poly_reduce(poly *res, const uint16_t temp[2 * LWE_N]) {
    for (size_t j = 0; j < LWE_N; ++j) {
        res->coeffs[j] += temp[j] - temp[j + LWE_N];
    }
}

/*************************************************
 * Name:        poly_mult_add
 *
 * Description: Compute the multiplication of two polynomials and add it to the
 *              result (i.e. res += (op1 * op2)). Second input polynomial is a
 *              compressed sparse polynomial.
 *
 * Arguments:   - uint16_t *res: pointer to output polynomial
 *              - uint16_t *op1: pointer to first input polynomial
 *              - uint8_t *op2: pointer to second input polynomial
 *              - size_t op2_length: length of second input polynomial
 *              - uint8_t neg_start: neg_start corresponding
 *                to second input polynomial
 **************************************************/
void poly_mult_add(poly *res, const poly *op1, const sppoly *op2) {
    uint16_t temp[LWE_N * 2] = {0};

    for (size_t j = 0; j < op2->neg_start; ++j) {
        poly_add(temp, op1, (op2->sx)[j]);
    }

    for (size_t j = op2->neg_start; j < op2->cnt; ++j) {
        poly_sub(temp, op1, (op2->sx)[j]);
    }
    poly_reduce(res, temp);
}

/*************************************************
 * Name:        poly_mult_sub
 *
 * Description: Compute the multiplication of two polynomials and subtract it
 *              from the result (i.e. res -= (op1 * op2)). Second input
 *              polynomial is a compressed sparse polynomial.
 *
 * Arguments:   - uint16_t *res: pointer to output polynomial
 *              - uint16_t *op1: pointer to first input polynomial
 *              - uint8_t *op2: pointer to second input polynomial
 *              - size_t op2_length: length of second input polynomial
 *              - uint8_t neg_start: neg_start corresponding
 *                to second input polynomial
 **************************************************/
void poly_mult_sub(poly *res, const poly *op1, const sppoly *op2) {
    uint16_t temp[LWE_N * 2] = {0};

    for (size_t j = 0; j < op2->neg_start; ++j) {
        poly_sub(temp, op1, (op2->sx)[j]);
    }

    for (size_t j = op2->neg_start; j < op2->cnt; ++j) {
        poly_add(temp, op1, (op2->sx)[j]);
    }
    poly_reduce(res, temp);
}

/*************************************************
 * Name:        matrix_vec_mult_add
 *
 * Description: Compute the multiplication & addition for polynomial matrix and
 *              vector (i.e. res += (op1 * op2)). Each element of the input
 *              polynomial vector is a compressed sparse polynomial.
 *
 * Arguments:   - uint16_t *res: pointer to output vector
 *              - uint16_t *op1: pointer to input polynomial matrix
 *              - uint8_t *op2: pointer to input polynomial vector
 *                (void * used for pointing uint8_t array with
 *                [MODULE_RANK][op2_length])
 *              - size_t op2_length: length of input polynomial vector
 *              - uint8_t neg_start: vector of negstart
 *                (each element of vector corresponds to the same index element
 *                in the input polynomial vector)
 *              - int16_t transpose: boolean deciding whether
 *                input matrix transposed or not
 **************************************************/
void matrix_vec_mult_add(polyvec *res, const polyvec op1[MODULE_RANK],
                         const sppoly op2[MODULE_RANK], int16_t transpose) {
    for (int i = 0; i < MODULE_RANK; i++) {
        for (int j = 0; j < MODULE_RANK; j++) {
            if (transpose == 1) {
                poly_mult_add(&(res->vec[i]), &(op1[j].vec[i]), &(op2[j]));
            } else {
                poly_mult_add(&(res->vec[i]), &(op1[i].vec[j]), &(op2[j]));
            }
        }
    }
}

/*************************************************
 * Name:        matrix_vec_mult_sub
 *
 * Description: Compute the multiplication & subtraction for polynomial matrix
 *              and vector (i.e. res -= (op1 * op2)). Each element of the input
 *              polynomial vector is a compressed sparse polynomial.
 *
 * Arguments:   - uint16_t *res: pointer to output vector
 *              - uint16_t *op1: pointer to input polynomial matrix
 *              - uint8_t *op2: pointer to input polynomial vector
 *                (void * used for pointing uint8_t array with
 *                [MODULE_RANK][op2_length])
 *              - size_t op2_length: length of polynomials
 *                in input polynomial vector
 *              - uint8_t neg_start: vector of negstart (each element
 *corresponds to the same index element in the input polynomial vector)
 *              - int16_t transpose: boolean deciding whether
 *                input matrix transposed or not
 **************************************************/
void matrix_vec_mult_sub(polyvec *res, const polyvec op1[MODULE_RANK],
                         const sppoly op2[MODULE_RANK], int16_t transpose) {

    for (int i = 0; i < MODULE_RANK; i++) {
        for (int j = 0; j < MODULE_RANK; j++) {
            if (transpose == 1) {
                poly_mult_sub(&(res->vec[i]), &(op1[j].vec[i]), &(op2[j]));
            } else {
                poly_mult_sub(&(res->vec[i]), &(op1[i].vec[j]), &(op2[j]));
            }
        }
    }
}

/*************************************************
 * Name:        vec_vec_mult_add
 *
 * Description: Compute the multiplication & addition for two polynomial
 *              vectors (i.e. res += (op1 * op2)). Each element of the second
 *              input polynomial vector is a compressed sparse polynomial.
 *
 * Arguments:   - uint16_t *res: pointer to output polynomial
 *              - uint16_t *op1: pointer to first input polynomial vector
 *              - uint8_t *op2: pointer to second input polynomial vector
 *                (void * used for pointing uint8_t array with
 *                [MODULE_RANK][op2_length])
 *              - size_t op2_length: length of polynomials
 *                in second input polynomial vector
 *              - uint8_t neg_start: vector of negstart (each element
 *corresponds to the same index element in the input polynomial vector)
 **************************************************/
void vec_vec_mult_add(poly *res, const polyvec *op1,
                      const sppoly op2[MODULE_RANK]) {
    for (int j = 0; j < MODULE_RANK; j++) {
        poly_mult_add(res, &(op1->vec[j]), &(op2[j]));
    }
}

/*************************************************
 * Name:        convToIdx
 *
 * Description: Compress a sparse polynomial to array of the degrees
 *              of nonzero coefficients (type == 0 for s(x), type == 1 for
 *              r(x)). neg_start is the smallest index to indicate the degree of
 *              coefficient -1.
 *
 * Arguments:   - uint16_t *res: pointer to output
 *              - size_t res_length: length of output
 *              - uint8_t *op: pointer to input polynomial
 *              - size_t op_length: length of input polynomial
 *
 * Returns neg_start(success) or 0(failure).
 **************************************************/
uint8_t convToIdx(uint8_t *res, const uint8_t res_length, const uint8_t *op,
                  const size_t op_length) {
    uint8_t index = 0, b = 0;
    uint8_t index_arr[2] = {0, res_length - 1}; // 0 for positive, 1 for
                                                // negative

    for (size_t i = 0; i < op_length; ++i) {
        index = ((op[i] & 0x80) >> 7) & 0x01;
        b = (-(uint64_t)op[i]) >> 63;
        res[index_arr[index]] ^= (-b) & (res[index_arr[index]] ^ i);
        index_arr[index] += op[i];
    }

    return index_arr[0];
}
