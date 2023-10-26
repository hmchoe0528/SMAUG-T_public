#ifndef SMAUG_IO_H
#define SMAUG_IO_H

#include <stdlib.h>

#include "ciphertext.h"
#include "key.h"
#include "parameters.h"
#include "verify.h"

// For ciphertext
void save_to_string(uint8_t *output, const ciphertext *ctxt);
void save_to_file(char *file_path, const uint8_t *ctxt);
void load_from_string(ciphertext *ctxt, const uint8_t *input);
void load_from_file(uint8_t *ctxt, const char *file_path);

// For secret key
void save_to_string_sk(uint8_t *output, const secret_key *sk, const int isPKE);
void save_to_file_sk(char *file_path, const uint8_t *sk, const int isPKE);
void load_from_string_sk(secret_key *sk, const uint8_t *input, const int isPKE);
void load_from_file_sk(uint8_t *sk, const char *file_path, const int isPKE);

// For public key
void save_to_string_pk(uint8_t *output, const public_key *pk);
void save_to_file_pk(char *file_path, const uint8_t *pk);
void load_from_string_pk(public_key *pk, const uint8_t *input);
void load_from_file_pk(uint8_t *pk, const char *file_path);

#endif // SMAUG_IO_H