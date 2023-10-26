#ifndef SMAUG_DG_H
#define SMAUG_DG_H

#include <fips202.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "parameters.h"
#include "rng.h"
#include "verify.h"

////////////////////////////////////////////////////////////////////////////////
///////////////////////////// NOISE DISTRIBUTION ///////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// error distribution for LWE instances in public key generation:
// store probability table (CDF_TABLE)
// sample a single error from (RANDBITS)-length of random binary string

// A discrete error distribution close to the discrete Gaussian distribution
void addGaussianErrorVec(uint16_t op[MODULE_RANK][LWE_N], const uint8_t *seed);

#endif // SMAUG_DG_H