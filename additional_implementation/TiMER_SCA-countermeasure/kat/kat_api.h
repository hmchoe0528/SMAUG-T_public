#ifndef kat_api_h
#define kat_api_h

#include <stdint.h>

#include "parameters.h"

#define CRYPTO_PUBLICKEYBYTES PUBLICKEY_BYTES

// For encrypt
#define CRYPTO_PKE_SECRETKEYBYTES PKE_SECRETKEY_BYTES

// For kem
#define CRYPTO_KEM_SECRETKEYBYTES KEM_SECRETKEY_BYTES
#define CRYPTO_CIPHERTEXTBYTES CIPHERTEXT_BYTES

// For KEM
int kat_crypto_kem_keypair(unsigned char *pk, unsigned char *sk);

int kat_crypto_kem_enc(unsigned char *ct, unsigned char *ss, unsigned char *pk);

int kat_crypto_kem_dec(unsigned char *ss, unsigned char *ct, unsigned char *sk);

#endif /* kat_api_h */
