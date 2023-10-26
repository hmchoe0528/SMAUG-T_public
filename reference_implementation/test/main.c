#include "ciphertext.h"
#include "indcpa.h"
#include "io.h"
#include "kem.h"
#include "pack.h"
#include "parameters.h"
#include "poly.h"
#include "rng.h"
#include <stdio.h>
#include <time.h>

#define m_size 10

int kem_test();
int indcpa_test();
void simple_test();

int main(void) {
    time_t mytime = time(NULL);
    char *time_str = ctime(&mytime);
    time_str[strlen(time_str) - 1] = '\0';
    printf("\n*** %s with mode %d starts at %s\n", "SMAUG KEM", SMAUG_MODE,
           time_str);

    // simple_test();
    // indcpa_test();
    size_t count = 1;
    // const size_t iteration = 10000000;
    const size_t iteration = 100;

    uint8_t entropy_input[48] = {0};
    for (size_t i = 0; i < 48; ++i) {
        entropy_input[i] = i;
    }
    randombytes_init(entropy_input, NULL, 256);

    for (size_t i = 0; i < iteration; ++i) {
        if (!(i % (iteration / 10))) {
            printf("...%lu%%", count * 10);
            fflush(stdout);
            ++count;
        }

        if (indcpa_test()) {
            printf("PKE test fails at %lu-th tries\n", i);
            break;
        }

        // if (kem_test()) {
        //     printf("KEM test fails at %lu-th tries\n", i);
        //     break;
        // }
    }

    return 0;
}

// void simple_test() {
//     uint8_t pk[PUBLICKEY_BYTES] = {0}, pk2[PUBLICKEY_BYTES] = {0};
//     uint8_t sk[KEM_SECRETKEY_BYTES] = {0};

//     indcpa_keypair(pk, sk);
//     printf("indcpa_keypair done\n");

//     public_key pk_tmp;
//     load_from_string_pk(&pk_tmp, pk);
//     save_to_string_pk(pk2, &pk_tmp);
//     if (memcmp(pk, pk2, PUBLICKEY_BYTES)) {
//         printf("Diff pk's\n");

//         printf("pk : ");
//         for (int i = 0; i < m_size; ++i)
//             printf("%02x ", pk[i]);
//         printf("\n");

//         printf("pk2: ");
//         for (int i = 0; i < m_size; ++i)
//             printf("%02x ", pk2[i]);
//         printf("\n");
//     }

//     // ind-cpa pke enc
//     uint8_t mx[DELTA_BYTES] = {0};
//     randombytes(mx, DELTA_BYTES);

//     // uint8_t ctxt[CIPHERTEXT_BYTES] = {0};
//     ciphertext ctxt;
//     indcpa_enc(&ctxt, pk, mx);
//     printf("indcpa_enc done\n");

//     // ind-cpa pke dec
//     uint8_t mx2[DELTA_BYTES] = {0};
//     indcpa_dec(mx2, sk, &ctxt);
//     printf("indcpa_dec done\n");

//     if (memcmp(mx, mx2, DELTA_BYTES))
//         printf("Diff mx\n");
// }

int indcpa_test() {
    int res = 0;
    uint8_t pk[PUBLICKEY_BYTES] = {0};
    uint8_t sk[PKE_SECRETKEY_BYTES] = {0};
    uint8_t ctxt[CIPHERTEXT_BYTES] = {0};
    // ciphertext ctxt;

    indcpa_keypair(pk, sk);
    // printf("indcpa_keypair done\n");

    uint8_t delta[DELTA_BYTES] = {0}, delta2[DELTA_BYTES] = {0};
    randombytes(delta, DELTA_BYTES);

    indcpa_enc(ctxt, pk, delta);
    // indcpa_enc(&ctxt, pk, delta);
    // printf("indcpa_enc done\n");

    indcpa_dec(delta2, sk, ctxt);
    // indcpa_dec(delta2, sk, &ctxt);
    // printf("indcpa_dec done\n");

    if (memcmp(delta, delta2, DELTA_BYTES)) {
        for (int i = 0; i < m_size; ++i)
            printf("%d ", delta[i]);
        printf("\n");

        for (int i = 0; i < m_size; ++i)
            printf("%d ", delta2[i]);
        printf("\n");
        res = 1;
    }

    return res;
}

int kem_test() {
    uint8_t pk[PUBLICKEY_BYTES] = {0};
    uint8_t sk[KEM_SECRETKEY_BYTES] = {0};
    // public_key pk;
    // secret_key sk;

    crypto_kem_keypair(pk, sk);
    // printf("Keygen_kem done\n");

    uint8_t ctxt[CIPHERTEXT_BYTES] = {0};
    // ciphertext ctxt;
    uint8_t ss[CRYPTO_BYTES] = {0}, ss2[CRYPTO_BYTES] = {0};
    crypto_kem_encap(ctxt, ss, pk);
    // crypto_kem_encap(&ctxt, ss, &pk);
    // printf("Encap done\n");

    int res = crypto_kem_decap(ss2, sk, pk, ctxt);
    // int res = crypto_kem_decap(ss2, &sk, &pk, &ctxt);
    // printf("Decap done\n");

    if (memcmp(ss, ss2, CRYPTO_BYTES)) {
        for (int i = 0; i < m_size; ++i) {
            printf("%d ", ss[i]);
        }
        printf("\n");

        for (int i = 0; i < m_size; ++i) {
            printf("%d ", ss2[i]);
        }
        printf("\n");
    }

    return res;
}
