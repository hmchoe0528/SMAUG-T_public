#ifndef SHA_2_H
#define SHA_2_H

#include <openssl/sha.h>

#define SHA_256_HashSize 32
#define SHA_512_HashSize 64

#define sha256(OUT, IN, INBYTES) SHA256(IN, INBYTES, OUT)
#define sha512(OUT, IN, INBYTES) SHA512(IN, INBYTES, OUT)

#endif
