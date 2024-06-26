#include "kat_api.h"
#include "kem.h"

int kat_crypto_kem_keypair(unsigned char *pk, unsigned char *sk) {
    crypto_kem_keypair(pk, sk);
    return 0;
}

int kat_crypto_kem_enc(unsigned char *ct, unsigned char *ss,
                       unsigned char *pk) {
    return crypto_kem_enc(ct, ss, pk);
}

int kat_crypto_kem_dec(unsigned char *ss, unsigned char *ct,
                       unsigned char *sk) {
    return crypto_kem_dec(ss, ct, sk);
}
