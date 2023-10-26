#ifndef SMAUG_CIPHERTEXT_H
#define SMAUG_CIPHERTEXT_H

#include <stdint.h>

#include "parameters.h"
#include "poly.h"

typedef struct Ciphertext {
    uint16_t c1[MODULE_RANK][LWE_N];
    uint16_t c2[LWE_N];
} ciphertext;

void computeC1(uint16_t c1[MODULE_RANK][LWE_N],
               const uint16_t A[MODULE_RANK][MODULE_RANK][LWE_N],
               const uint8_t *r[MODULE_RANK],
               const uint8_t r_cnt_arr[MODULE_RANK],
               const uint8_t r_neg_start[MODULE_RANK]);
void computeC2(uint16_t c2[LWE_N], const uint8_t delta[DELTA_BYTES],
               const uint16_t b[MODULE_RANK][LWE_N],
               const uint8_t *r[MODULE_RANK],
               const uint8_t r_cnt_arr[MODULE_RANK],
               const uint8_t r_neg_start[MODULE_RANK]);

#endif // SMAUG_CIPHERTEXT_H