#ifndef TIMER_HWT_H
#define TIMER_HWT_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "fips202.h"
#include "parameters.h"

#define HS_XOF 4
#define HR_XOF 5

#define hwt TIMER_NAMESPACE(hwt)
void hwt(uint8_t *res, uint8_t *cnt_arr, const uint8_t *input,
         const size_t input_size, const uint16_t hmwt);

#endif // TIMER_HWT_H
