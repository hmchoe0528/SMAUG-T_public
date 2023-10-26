#include "pack.h"

/*************************************************
 * Name:        Rq_to_bytes
 *
 * Description: Transform to bytes array from polynomial in Rq
 *
 * Arguments:   - uint16_t *bytes: pointer to output bytes
 *              - uint16_t *data: pointer to input polynomial in Rq
 **************************************************/
void Rq_to_bytes(uint8_t bytes[PKPOLY_BYTES], const uint16_t data[LWE_N]) {
    int b_idx = 0, d_idx = 0;
    for (size_t i = 0; i < LWE_N / 4; i++) {
        b_idx = Rq_DATA_OFFSET * i;
        d_idx = Rq_BYTE_OFFSET * i;
        bytes[b_idx] = (data[d_idx] & 0xff);
        bytes[b_idx + 1] = (data[d_idx + 1] & 0xff);
        bytes[b_idx + 2] = (data[d_idx + 2] & 0xff);
        bytes[b_idx + 3] = (data[d_idx + 3] & 0xff);
        bytes[b_idx + 4] =
            ((data[d_idx] >> 8) & 0x03) | ((data[d_idx + 1] >> 6) & 0x0c) |
            ((data[d_idx + 2] >> 4) & 0x30) | ((data[d_idx + 3] >> 2) & 0xc0);
    }
}

/*************************************************
 * Name:        bytes_to_Rq
 *
 * Description: Transform to polynomial in Rq from bytes array
 *
 * Arguments:   - uint16_t *data: pointer to output polynomial in Rq
 *              - uint16_t *bytes: pointer to input bytes
 **************************************************/
void bytes_to_Rq(uint16_t data[LWE_N], const uint8_t bytes[PKPOLY_BYTES]) {

    int b_idx = 0, d_idx = 0;
    for (size_t i = 0; i < LWE_N / 4; i++) {
        b_idx = Rq_DATA_OFFSET * i;
        d_idx = Rq_BYTE_OFFSET * i;
        data[d_idx] =
            (((uint16_t)bytes[b_idx + 4] & 0x03) << 8) | (bytes[b_idx] & 0xff);
        data[d_idx + 1] =
            ((bytes[b_idx + 4] & 0x0c) << 6) | (bytes[b_idx + 1] & 0xff);
        data[d_idx + 2] =
            ((bytes[b_idx + 4] & 0x30) << 4) | (bytes[b_idx + 2] & 0xff);
        data[d_idx + 3] =
            ((bytes[b_idx + 4] & 0xc0) << 2) | (bytes[b_idx + 3] & 0xff);
    }
}

/*************************************************
 * Name:        Rq_vec_to_bytes
 *
 * Description: Transform to bytes array from a vector of  polynomial in Rq
 *
 * Arguments:   - uint16_t *bytes: pointer to output bytes
 *              - uint16_t *data: pointer to input vector of polynomial in Rq
 **************************************************/
void Rq_vec_to_bytes(uint8_t bytes[PKPOLYVEC_BYTES],
                     const uint16_t data[MODULE_RANK][LWE_N]) {
    for (size_t i = 0; i < MODULE_RANK; ++i)
        Rq_to_bytes(bytes + i * PKPOLY_BYTES, data[i]);
}

/*************************************************
 * Name:        bytes_to_Rq_vec
 *
 * Description: Transform to bytes array from a vector of  polynomial in Rq
 *
 * Arguments:   - uint16_t *data: pointer to output a vector of poly in Rq
 *              - uint16_t *bytes: pointer to input bytes
 **************************************************/
void bytes_to_Rq_vec(uint16_t data[MODULE_RANK][LWE_N],
                     const uint8_t bytes[PKPOLYVEC_BYTES]) {
    for (size_t i = 0; i < MODULE_RANK; ++i)
        bytes_to_Rq(data[i], bytes + i * PKPOLY_BYTES);
}

/*************************************************
 * Name:        Rq_mat_to_bytes
 *
 * Description: Transform to bytes array from a matrix of  polynomial in Rq
 *
 * Arguments:   - uint16_t *bytes: pointer to output bytes
 *              - uint16_t *data: pointer to input matrix of polynomial in Rq
 **************************************************/
void Rq_mat_to_bytes(uint8_t bytes[PKPOLYMAT_BYTES],
                     const uint16_t data[MODULE_RANK][MODULE_RANK][LWE_N]) {
    for (size_t i = 0; i < MODULE_RANK; ++i)
        Rq_vec_to_bytes(bytes + i * PKPOLYVEC_BYTES, data[i]);
}

/*************************************************
 * Name:        bytes_to_Rq_mat
 *
 * Description: Transform to bytes array from a matrix of  polynomial in Rq
 *
 * Arguments:   - uint16_t *data: pointer to output a matrix of poly in Rq
 *              - uint16_t *bytes: pointer to input bytes
 **************************************************/
void bytes_to_Rq_mat(uint16_t data[MODULE_RANK][MODULE_RANK][LWE_N],
                     const uint8_t bytes[PKPOLYMAT_BYTES]) {

    for (size_t i = 0; i < MODULE_RANK; ++i)
        bytes_to_Rq_vec(data[i], bytes + i * PKPOLYVEC_BYTES);
}

/*************************************************
 * Name:        Rp_to_bytes
 *
 * Description: Transform to bytes array from polynomial in Rp
 *
 * Arguments:   - uint16_t *bytes: pointer to output bytes
 *              - uint16_t *data: pointer to input polynomial in Rp
 **************************************************/
void Rp_to_bytes(uint8_t bytes[CTPOLY_BYTES], const uint16_t data[LWE_N]) {
    memset(bytes, 0, sizeof(uint8_t) * CTPOLY_BYTES);
    for (size_t i = 0; i < LWE_N; ++i)
        memcpy(&(bytes[i]), &(data[i]), sizeof(uint8_t));
}

/*************************************************
 * Name:        bytes_to_Rp
 *
 * Description: Transform to polynomial in Rq from bytes array
 *
 * Arguments:   - uint16_t *data: pointer to output polynomial in Rq
 *              - uint16_t *bytes: pointer to input bytes
 **************************************************/
void bytes_to_Rp(uint16_t data[LWE_N], const uint8_t bytes[CTPOLY_BYTES]) {
    memset(data, 0, sizeof(uint16_t) * LWE_N);
    for (size_t i = 0; i < LWE_N; ++i)
        memcpy(&(data[i]), &(bytes[i]), sizeof(uint8_t));
}

/*************************************************
 * Name:        Rp_vec_to_bytes
 *
 * Description: Transform to bytes array from a vector of  polynomial in Rp
 *
 * Arguments:   - uint16_t *bytes: pointer to output bytes
 *              - uint16_t *data: pointer to input vector of polynomial in Rp
 **************************************************/
void Rp_vec_to_bytes(uint8_t bytes[CTPOLYVEC_BYTES],
                     const uint16_t data[MODULE_RANK][LWE_N]) {
    for (size_t i = 0; i < MODULE_RANK; ++i)
        Rp_to_bytes(bytes + i * CTPOLY_BYTES, data[i]);
}

/*************************************************
 * Name:        bytes_to_Rp_vec
 *
 * Description: Transform to bytes array from a vector of  polynomial in Rp
 *
 * Arguments:   - uint16_t *data: pointer to output a vector of poly in Rp
 *              - uint16_t *bytes: pointer to input bytes
 **************************************************/
void bytes_to_Rp_vec(uint16_t data[MODULE_RANK][LWE_N],
                     const uint8_t bytes[CTPOLYVEC_BYTES]) {
    for (size_t i = 0; i < MODULE_RANK; ++i)
        bytes_to_Rp(data[i], bytes + i * CTPOLY_BYTES);
}

/*************************************************
 * Name:        Sx_to_bytes
 *
 * Description: Transform to bytes array from a degree of array of secrey poly
 *
 * Arguments:   - uint16_t *bytes: pointer to output bytes
 *              - uint16_t *data: pointer to input a degree of array of secret
 *                                poly
 **************************************************/
void Sx_to_bytes(uint8_t bytes[SKPOLY_BYTES], const uint8_t data[HS]) {
    memcpy(bytes, data, sizeof(uint8_t) * HS);
}

/*************************************************
 * Name:        bytes_to_Sx
 *
 * Description: Transform to a degree of array of secrey poly from bytes array
 *
 * Arguments:   - uint16_t *data: pointer to output a degree of array of secret
 *                                poly
 *              - uint16_t *bytes: pointer to input bytes
 **************************************************/
void bytes_to_Sx(uint8_t data[HS], const uint8_t bytes[SKPOLY_BYTES]) {
    memcpy(data, bytes, sizeof(uint8_t) * HS);
}

/*************************************************
 * Name:        Sx_to_bytes
 *
 * Description: Transform to bytes array from a vector of a degree of array of
 *              secret poly
 *
 * Arguments:   - uint16_t *bytes: pointer to output bytes
 *              - uint16_t *data: pointer to input a vector of arrays of degrees
 *                                of secret poly
 **************************************************/
void Sx_vec_to_bytes(uint8_t bytes[SKPOLYVEC_BYTES],
                     const uint8_t data[MODULE_RANK][HS]) {
    for (size_t i = 0; i < MODULE_RANK; ++i)
        Sx_to_bytes(bytes + i * SKPOLY_BYTES, data[i]);
}

/*************************************************
 * Name:        Sx_to_bytes
 *
 * Description: Transform to a vector of a degree of array of secret poly from
 *              bytes array
 *
 * Arguments:   - uint16_t *data: pointer to output a vector of arrays of
 *                                degrees of secret poly
 *              - uint16_t *bytes: pointer to input bytes
 **************************************************/
void bytes_to_Sx_vec(uint8_t data[MODULE_RANK][HS],
                     const uint8_t bytes[SKPOLYVEC_BYTES]) {
    for (size_t i = 0; i < MODULE_RANK; ++i)
        bytes_to_Sx(data[i], bytes + i * SKPOLY_BYTES);
}
