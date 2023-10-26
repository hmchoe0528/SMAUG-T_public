#ifndef SMAUG_HASH_H
#define SMAUG_HASH_H

#include "fips202.h"

#define hash_h(OUT, IN, INBYTES) sha3_256(OUT, IN, INBYTES)
#define hash_g(OUT, IN, INBYTES) sha3_512(OUT, IN, INBYTES)

#endif // SMAUG_HASH_H
