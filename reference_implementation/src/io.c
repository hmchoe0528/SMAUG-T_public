#include "io.h"

void save_to_string(uint8_t *output, const ciphertext *ctxt) {
    Rp_vec_to_bytes(output, ctxt->c1);
    Rp2_to_bytes(output + CTPOLYVEC_BYTES, ctxt->c2);
}

void save_to_file(char *file_path, const uint8_t *ctxt) {
    FILE *f = fopen(file_path, "wb");
    if (f == NULL) {
        printf("Cannot open file in save_to_file\n");
        return;
    }
    fwrite(ctxt, sizeof(uint8_t), CIPHERTEXT_BYTES, f);
    fclose(f);
}

void load_from_string(ciphertext *ctxt, const uint8_t *input) {
    bytes_to_Rp_vec(ctxt->c1, input);
    bytes_to_Rp2(ctxt->c2, input + CTPOLYVEC_BYTES);
}

void load_from_file(uint8_t *ctxt, const char *file_path) {
    FILE *f = fopen(file_path, "rb");
    if (f == NULL) {
        printf("Cannot open file in load_from_file\n");
        return;
    }
    size_t res = fread(ctxt, sizeof(uint8_t), CIPHERTEXT_BYTES, f);
    if (res != (sizeof(uint8_t) * CIPHERTEXT_BYTES)) {
        printf("Ctxt File reading error\n");
        memset(ctxt, 0, CIPHERTEXT_BYTES);
    }
    fclose(f);
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

void save_to_string_sk(uint8_t *output, const secret_key *sk, const int isPKE) {
    cmov(output, sk->cnt_arr, MODULE_RANK, 1);
    Sx_vec_to_bytes(output + MODULE_RANK, (const uint8_t **)sk->s, sk->cnt_arr);
    cmov(output + MODULE_RANK + SKPOLYVEC_BYTES, sk->neg_start, MODULE_RANK, 1);
    if (!isPKE)
        cmov(output + 2 * MODULE_RANK + SKPOLYVEC_BYTES, sk->t, T_BYTES, 1);
}

void save_to_file_sk(char *file_path, const uint8_t *sk, const int isPKE) {
    FILE *f = fopen(file_path, "wb");
    if (f == NULL) {
        printf("Cannot open file in save_to_file_sk\n");
        return;
    }
    size_t size = isPKE ? PKE_SECRETKEY_BYTES : KEM_SECRETKEY_BYTES;
    fwrite(sk, sizeof(uint8_t), size, f);
    fclose(f);
}

void load_from_string_sk(secret_key *sk, const uint8_t *input,
                         const int isPKE) {
    cmov(sk->cnt_arr, input, MODULE_RANK, 1);
    for (size_t i = 0; i < MODULE_RANK; ++i)
        sk->s[i] = (uint8_t *)malloc(sk->cnt_arr[i]);
    bytes_to_Sx_vec(sk->s, input + MODULE_RANK, sk->cnt_arr);
    cmov(sk->neg_start, input + MODULE_RANK + SKPOLYVEC_BYTES, MODULE_RANK, 1);
    if (!isPKE) {
        cmov(sk->t, input + 2 * MODULE_RANK + SKPOLYVEC_BYTES, T_BYTES, 1);
    }
}

void load_from_file_sk(uint8_t *sk, const char *file_path, const int isPKE) {
    FILE *f = fopen(file_path, "rb");
    if (f == NULL) {
        printf("Cannot open file in load_from_file_sk\n");
        return;
    }

    size_t size = isPKE ? PKE_SECRETKEY_BYTES : KEM_SECRETKEY_BYTES;
    size_t res = fread(sk, sizeof(uint8_t), size, f);
    if (res != size) {
        printf("SK File reading error\n");
        memset(sk, 0, size);
    }
    fclose(f);
}

/*----------------------------------------------------------------------------*/

void save_to_string_pk(uint8_t *output, const public_key *pk) {
    uint16_t vec[MODULE_RANK][LWE_N] = {0};
    memcpy(output, pk->seed, sizeof(uint8_t) * PKSEED_BYTES);
    for (int i = 0; i < MODULE_RANK; i++)
        for (int j = 0; j < LWE_N; j++)
            vec[i][j] = (pk->b[i][j] >> _16_LOG_Q);
    Rq_vec_to_bytes(output + PKSEED_BYTES, vec);
}

void save_to_file_pk(char *file_path, const uint8_t *pk) {
    FILE *f = fopen(file_path, "wb");
    if (f == NULL) {
        printf("Cannot open file in save_to_file_pk\n");
        return;
    }
    fwrite(pk, sizeof(uint8_t), PUBLICKEY_BYTES, f);
    fclose(f);
}

void load_from_string_pk(public_key *pk, const uint8_t *input) {
    uint16_t vec[MODULE_RANK][LWE_N] = {0};
    memcpy(pk->seed, input, PKSEED_BYTES);
    genAx(pk->A, pk->seed);
    bytes_to_Rq_vec(vec, input + PKSEED_BYTES);
    for (int i = 0; i < MODULE_RANK; i++)
        for (int j = 0; j < LWE_N; j++)
            pk->b[i][j] = (vec[i][j] << _16_LOG_Q);
}

void load_from_file_pk(uint8_t *pk, const char *file_path) {
    FILE *f = fopen(file_path, "rb");
    if (f == NULL) {
        printf("Cannot open file in load_from_file_pk\n");
        return;
    }
    size_t res = fread(pk, sizeof(uint8_t), PUBLICKEY_BYTES, f);
    if (res != PUBLICKEY_BYTES) {
        printf("PK File reading error\n");
        memset(pk, 0, PUBLICKEY_BYTES);
    }
    fclose(f);
}