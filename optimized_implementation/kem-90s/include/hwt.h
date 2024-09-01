#ifndef SMAUG_HWT_H
#define SMAUG_HWT_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "parameters.h"
#include "symmetric.h"

// https://github.com/dgazzoni/NTRU-sampling/blob/main/jupyter/NTRU-sampling.ipynb
// Use enough seed size 308
#define HWTSEEDBYTES ((16 * 308) / 8)

#define hwt SMAUG_NAMESPACE(hwt)
void hwt(int16_t *res, const uint8_t *seed);

#endif // SMAUG_HWT_H
