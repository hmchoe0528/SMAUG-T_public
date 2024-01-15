#include "io.h"

void save_to_string(uint8_t *output, const ciphertext *ctxt) {
    Rp_vec_to_bytes(output, &(ctxt->c1));
    Rp2_to_bytes(output + CTPOLYVEC_BYTES, &(ctxt->c2));
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
    bytes_to_Rp_vec(&(ctxt->c1), input);
    bytes_to_Rp2(&(ctxt->c2), input + CTPOLYVEC_BYTES);
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

void save_to_string_sk(uint8_t *output, const secret_key *sk) {
    size_t idx = 0;

    for (size_t i = 0; i < MODULE_RANK; ++i) {
        cmov(output + idx, &(sk->sp_vec[i].cnt), sizeof(uint8_t), 1);
        idx += sizeof(uint8_t);
    }

    for (size_t i = 0; i < MODULE_RANK; ++i) {
        Sx_to_bytes(output + idx, sk->sp_vec[i].sx, sk->sp_vec[i].cnt);
        idx += sk->sp_vec[i].cnt;
    }

    for (size_t i = 0; i < MODULE_RANK; ++i) {
        cmov(output + idx, &(sk->sp_vec[i].neg_start), sizeof(uint8_t), 1);
        idx += sizeof(uint8_t);
    }
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

void load_from_string_sk(secret_key *sk, const uint8_t *input) {
    size_t idx = 0;

    for (size_t i = 0; i < MODULE_RANK; ++i) {
        cmov(&(sk->sp_vec[i].cnt), input + idx, sizeof(uint8_t), 1);
        sk->sp_vec[i].sx =
            (uint8_t *)calloc(sk->sp_vec[i].cnt, sizeof(uint8_t));
        idx += sizeof(uint8_t);
    }

    for (size_t i = 0; i < MODULE_RANK; ++i) {
        bytes_to_Sx(sk->sp_vec[i].sx, input + idx, sk->sp_vec[i].cnt);
        idx += sk->sp_vec[i].cnt;
    }

    for (size_t i = 0; i < MODULE_RANK; ++i) {
        cmov(&(sk->sp_vec[i].neg_start), input + idx, sizeof(uint8_t), 1);
        idx += sizeof(uint8_t);
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
    polyvec vec_tmp;
    memset(&vec_tmp, 0, sizeof(polyvec));
    memcpy(output, pk->seed, sizeof(uint8_t) * PKSEED_BYTES);
    for (int i = 0; i < MODULE_RANK; i++)
        for (int j = 0; j < LWE_N; j++)
            vec_tmp.vec[i].coeffs[j] = (pk->b.vec[i].coeffs[j] >> _16_LOG_Q);
    Rq_vec_to_bytes(output + PKSEED_BYTES, &vec_tmp);
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
    polyvec poly_vec;
    memcpy(pk->seed, input, PKSEED_BYTES);
    genAx(pk->A, pk->seed);
    bytes_to_Rq_vec(&poly_vec, input + PKSEED_BYTES);
    for (int i = 0; i < MODULE_RANK; i++)
        for (int j = 0; j < LWE_N; j++)
            pk->b.vec[i].coeffs[j] = (poly_vec.vec[i].coeffs[j] << _16_LOG_Q);
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