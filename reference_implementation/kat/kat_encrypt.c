#include "api.h"
#include "indcpa.h"

int crypto_encrypt_keypair(unsigned char *pk, unsigned char *sk) {
    indcpa_keypair(pk, sk);
    return 0;
}

int crypto_encrypt(unsigned char *c, unsigned long long *clen, unsigned char *m,
                   unsigned long long mlen, unsigned char *pk) {
    printf("Encrypting message of length %lu\n", mlen);
    indcpa_enc(c, pk, m);
    return 0;
}

int crypto_encrypt_open(unsigned char *m1, unsigned long long *mlen1,
                        unsigned char *c, unsigned long long clen,
                        unsigned char *sk) {
    indcpa_dec(m1, sk, c);
    return 0;
}
