#ifndef TIMER_CIPHERTEXT_H
#define TIMER_CIPHERTEXT_H

#include <stdint.h>

#include "parameters.h"
#include "poly.h"

typedef struct Ciphertext {
    polyvec c1;
    poly c2;
} ciphertext;

#define computeC1 TIMER_NAMESPACE(computeC1)
void computeC1(polyvec *c1, const polyvec A[MODULE_RANK],
               const sppoly r[MODULE_RANK]);
#define computeC2 TIMER_NAMESPACE(computeC2)
void computeC2(poly *c2, const uint8_t delta[MSG_BYTES], const polyvec *b,
               const sppoly r[MODULE_RANK]);

#endif // TIMER_CIPHERTEXT_H
