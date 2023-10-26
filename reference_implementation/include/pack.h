#ifndef SMAUG_PACK_H
#define SMAUG_PACK_H

#include "parameters.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define Rq_DATA_OFFSET (LOG_Q / 2)
#define Rq_BYTE_OFFSET (8 / 2) // (bit size of uint8_t) / 2

void Rq_to_bytes(uint8_t bytes[PKPOLY_BYTES], const uint16_t data[LWE_N]);
void bytes_to_Rq(uint16_t data[LWE_N], const uint8_t bytes[PKPOLY_BYTES]);
void Rq_vec_to_bytes(uint8_t bytes[PKPOLYVEC_BYTES],
                     const uint16_t data[MODULE_RANK][LWE_N]);
void bytes_to_Rq_vec(uint16_t data[MODULE_RANK][LWE_N],
                     const uint8_t bytes[PKPOLYVEC_BYTES]);
void Rq_mat_to_bytes(uint8_t bytes[PKPOLYMAT_BYTES],
                     const uint16_t data[MODULE_RANK][MODULE_RANK][LWE_N]);
void bytes_to_Rq_mat(uint16_t data[MODULE_RANK][MODULE_RANK][LWE_N],
                     const uint8_t bytes[PKPOLYMAT_BYTES]);

void Rp_to_bytes(uint8_t bytes[CTPOLY_BYTES], const uint16_t data[LWE_N]);
void bytes_to_Rp(uint16_t data[LWE_N], const uint8_t bytes[CTPOLY_BYTES]);
void Rp_vec_to_bytes(uint8_t bytes[CTPOLYVEC_BYTES],
                     const uint16_t data[MODULE_RANK][LWE_N]);
void bytes_to_Rp_vec(uint16_t data[MODULE_RANK][LWE_N],
                     const uint8_t bytes[CTPOLYVEC_BYTES]);

void Sx_to_bytes(uint8_t bytes[SKPOLY_BYTES], const uint8_t data[HS]);
void bytes_to_Sx(uint8_t data[HS], const uint8_t bytes[SKPOLY_BYTES]);
void Sx_vec_to_bytes(uint8_t bytes[SKPOLYVEC_BYTES],
                     const uint8_t data[MODULE_RANK][HS]);
void bytes_to_Sx_vec(uint8_t data[MODULE_RANK][HS],
                     const uint8_t bytes[SKPOLYVEC_BYTES]);

#endif // SMAUG_PACK_H