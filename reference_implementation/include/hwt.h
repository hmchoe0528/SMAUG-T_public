#ifndef SMAUG_HWT_H
#define SMAUG_HWT_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "fips202.h"
#include "parameters.h"

void hwt(uint8_t *res, uint8_t *cnt_arr, const uint8_t *input,
         const size_t input_size, const uint16_t hmwt);

#endif // SMAUG_HWT_H