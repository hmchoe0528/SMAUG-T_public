#ifndef SMAUG_PACK_H
#define SMAUG_PACK_H

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "parameters.h"
#include "verify.h"

#define R11_DATA_OFFSET LOG_Q
#define R11_BYTE_OFFSET 8 // (bit size of uint8_t)
#define R10_DATA_OFFSET (LOG_Q / 2)
#define R10_BYTE_OFFSET (8 / 2) // (bit size of uint8_t) / 2

void Rq_to_bytes(uint8_t bytes[PKPOLY_BYTES], const uint16_t data[LWE_N],
                 const size_t dlen);
void bytes_to_Rq(uint16_t data[LWE_N], const uint8_t bytes[PKPOLY_BYTES],
                 const size_t dlen);
void Rq_vec_to_bytes(uint8_t bytes[PKPOLYVEC_BYTES],
                     const uint16_t data[MODULE_RANK][LWE_N]);
void bytes_to_Rq_vec(uint16_t data[MODULE_RANK][LWE_N],
                     const uint8_t bytes[PKPOLYVEC_BYTES]);
void Rq_mat_to_bytes(uint8_t bytes[PKPOLYMAT_BYTES],
                     const uint16_t data[MODULE_RANK][MODULE_RANK][LWE_N]);
void bytes_to_Rq_mat(uint16_t data[MODULE_RANK][MODULE_RANK][LWE_N],
                     const uint8_t bytes[PKPOLYMAT_BYTES]);

void Rp_to_bytes(uint8_t bytes[CTPOLY1_BYTES], const uint16_t data[LWE_N]);
void bytes_to_Rp(uint16_t data[LWE_N], const uint8_t bytes[CTPOLY1_BYTES]);
void Rp2_to_bytes(uint8_t bytes[CTPOLY2_BYTES], const uint16_t data[LWE_N]);
void bytes_to_Rp2(uint16_t data[LWE_N], const uint8_t bytes[CTPOLY2_BYTES]);
void Rp_vec_to_bytes(uint8_t bytes[CTPOLYVEC_BYTES],
                     const uint16_t data[MODULE_RANK][LWE_N]);
void bytes_to_Rp_vec(uint16_t data[MODULE_RANK][LWE_N],
                     const uint8_t bytes[CTPOLYVEC_BYTES]);

void Sx_to_bytes(uint8_t *bytes, const uint8_t *data, const uint8_t data_len);
void bytes_to_Sx(uint8_t *data, const uint8_t *bytes, const uint8_t bytes_len);
void Sx_vec_to_bytes(uint8_t *bytes, const uint8_t *data[MODULE_RANK],
                     const uint8_t *data_len_arr);
void bytes_to_Sx_vec(uint8_t *data[MODULE_RANK], const uint8_t *bytes,
                     const uint8_t *bytes_len_arr);

#endif // SMAUG_PACK_H