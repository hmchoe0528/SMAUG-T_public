#include "poly.h"
#include <string.h>
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
    for (size_t i = 0; i < MSG_BYTES; i++) {
        for (size_t j = 0; j < 8; j++) {
	        size_t index = 8 * i + j;
            mask = (msg[i] >> j) & 1;
            mask = (mask * Modulus_Q) & Modulus_Q; //Modulus_Q is define Q/2
            r->coeffs[index] = mask;
            r->coeffs[index + 128] = mask;
        }
    }
}
/*************AVX2 Optimized Code***************/
/*
void poly_frommsg(poly *r, const uint8_t *msg) {
    __m256i mask = _mm256_set1_epi16(Modulus_Q); // Set mask to Modulus_Q
    __m256i zero = _mm256_setzero_si256(); // Zero vector for comparison
    
    for (size_t i = 0; i < MSG_BYTES; i++) {
        uint8_t byte = msg[i]; // Process one byte at a time
        
        for (size_t j = 0; j < 8; j++) {
            // Create a vector with all bits of the byte replicated across
            __m256i bits = _mm256_set1_epi16((byte >> j) & 1);
            // Compare bits to zero, resulting in either 0xFFFF or 0x0000 for each element
            __m256i compare = _mm256_cmpgt_epi16(bits, zero);
            // Bitwise AND with mask to get either 0 or Modulus_Q
            __m256i result = _mm256_and_si256(compare, mask);
            
            // Store the result into the polynomial coefficients
            r->coeffs[8 * i + j] = _mm256_extract_epi16(result, 0); // Extracting one element for demonstration
            r->coeffs[8 * i + j + 128] = _mm256_extract_epi16(result, 0);
        }
    }
}
*/
/********************END***********************/


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
    for (size_t i = 0; i < MSG_BYTES; i++)
        msg[i] = 0;

    for (size_t i = 0; i < LWE_N / 2; i++) {
        t = flipabs(x->coeffs[i]);
        t += flipabs(x->coeffs[i + 128]);
        t = t - Modulus_Q;
        t >>= 15;
        msg[i >> 3] |= t << (i & 7);
    }
}

/*************AVX2 Optimized Code***************/

/*void poly_tomsg(unsigned char *msg, const poly *x) {
    memset(msg, 0, MSG_BYTES);

    // Process in chunks of 16, since AVX2 can handle 16 uint16_t values at once
    for (size_t i = 0; i < LWE_N / 2; i += 16) {
        // Load 16 coefficients from each half
        __m256i coeff1 = _mm256_loadu_si256((__m256i *)&x->coeffs[i]);
        __m256i coeff2 = _mm256_loadu_si256((__m256i *)&x->coeffs[i + 128]);

        // Vectorized flipabs operation for both halves
        __m256i half_modulus = _mm256_set1_epi16(Modulus_Q);
        __m256i diff1 = _mm256_sub_epi16(coeff1, half_modulus);
        __m256i diff2 = _mm256_sub_epi16(coeff2, half_modulus);
        __m256i neg1 = _mm256_srai_epi16(diff1, 15);
        __m256i neg2 = _mm256_srai_epi16(diff2, 15);
        __m256i abs1 = _mm256_xor_si256(_mm256_add_epi16(diff1, neg1), neg1);
        __m256i abs2 = _mm256_xor_si256(_mm256_add_epi16(diff2, neg2), neg2);

        // Add abs1 and abs2, subtract Modulus_Q, and shift right to get the bit
        __m256i sum = _mm256_add_epi16(abs1, abs2);
        sum = _mm256_sub_epi16(sum, half_modulus);
        __m256i bit = _mm256_srli_epi16(sum, 15);

        // Scalar processing to set bits in the message
        // This part does not leverage AVX2 due to the granularity of operations
        for (size_t j = 0; j < 16; ++j) {
            int bit_val = (_mm256_extract_epi16(bit, j) & 1) << ((i + j) & 7);
            msg[(i + j) >> 3] |= bit_val;
        }
    }
}*/
/********************END***********************/

/*************************************************
 * Name:        flipabs
 *
 * Description: Computes |(x mod q) - Q/2|
 *
 * Arguments:   uint16_t x: input coefficient
 *
 * Returns |(x mod q) - Q/2|
 **************************************************/
uint16_t flipabs(uint16_t x) {
    int16_t r, m;
    r = x - (Modulus_Q);  //Modulus_Q is already define Q/2
    m = r >> 15;
    return (r + m) ^ m;
}

/*************AVX2 Optimized Code***************
   IF YOU WANT TO AVX2, PLEASE DELETE(OR COMMENT) "filbabs" function
   ALSO, CHECK THE poly.h (delete the filpabs header)  */
/********************END***********************/


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
