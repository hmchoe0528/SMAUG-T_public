#include "pack.h"

/*************************************************
 * Name:        Rq_to_bytes
 *
 * Description: Transform to bytes array from polynomial in Rq
 *
 * Arguments:   - uint16_t *bytes: pointer to output bytes
 *              - uint16_t *data: pointer to input polynomial in Rq
 *              - size_t dlen: date length
 **************************************************/
void Rq_to_bytes(uint8_t bytes[PKPOLY_BYTES], const uint16_t data[LWE_N],
                 const size_t dlen) {
    int b_idx = 0, d_idx = 0;

#if LOG_Q == 10
    for (size_t i = 0; i < dlen / 4; i++) {
        b_idx = R10_DATA_OFFSET * i;
        d_idx = R10_BYTE_OFFSET * i;
        bytes[b_idx] = (data[d_idx] & 0xff);
        bytes[b_idx + 1] = (data[d_idx + 1] & 0xff);
        bytes[b_idx + 2] = (data[d_idx + 2] & 0xff);
        bytes[b_idx + 3] = (data[d_idx + 3] & 0xff);
        bytes[b_idx + 4] =
            ((data[d_idx] >> 8) & 0x03) | ((data[d_idx + 1] >> 6) & 0x0c) |
            ((data[d_idx + 2] >> 4) & 0x30) | ((data[d_idx + 3] >> 2) & 0xc0);
    }
#endif

#if LOG_Q == 11
    for (size_t i = 0; i < dlen / 8; ++i) {
        b_idx = R11_DATA_OFFSET * i;
        d_idx = R11_BYTE_OFFSET * i;
        bytes[b_idx] = data[d_idx] & 0xff;
        bytes[b_idx + 1] =
            ((data[d_idx] >> 3) & 0xe0) | (data[d_idx + 1] & 0x1f);
        bytes[b_idx + 2] =
            ((data[d_idx + 1] >> 3) & 0xfc) | (data[d_idx + 2] & 0x03);
        bytes[b_idx + 3] = (data[d_idx + 2] >> 2) & 0xff;
        bytes[b_idx + 4] =
            ((data[d_idx + 2] >> 3) & 0x80) | (data[d_idx + 3] & 0x7f);
        bytes[b_idx + 5] =
            ((data[d_idx + 3] >> 3) & 0xf0) | (data[d_idx + 4] & 0x0f);
        bytes[b_idx + 6] =
            ((data[d_idx + 4] >> 3) & 0xfe) | (data[d_idx + 5] & 0x01);
        bytes[b_idx + 7] = (data[d_idx + 5] >> 1) & 0xff;
        bytes[b_idx + 8] =
            ((data[d_idx + 5] >> 3) & 0xc0) | (data[d_idx + 6] & 0x3f);
        bytes[b_idx + 9] =
            ((data[d_idx + 6] >> 3) & 0xf8) | (data[d_idx + 7] & 0x07);
        bytes[b_idx + 10] = (data[d_idx + 7] >> 3) & 0xff;
    }
#endif
}

/*************************************************
 * Name:        bytes_to_Rq
 *
 * Description: Transform to polynomial in Rq from bytes array
 *
 * Arguments:   - uint16_t *data: pointer to output polynomial in Rq
 *              - uint16_t *bytes: pointer to input bytes
 *              - size_t dlen: date length
 **************************************************/
void bytes_to_Rq(uint16_t data[LWE_N], const uint8_t bytes[PKPOLY_BYTES],
                 const size_t dlen) {

    int b_idx = 0, d_idx = 0;
#if LOG_Q == 10
    for (size_t i = 0; i < dlen / 4; i++) {
        b_idx = R10_DATA_OFFSET * i;
        d_idx = R10_BYTE_OFFSET * i;
        data[d_idx] =
            (((uint16_t)bytes[b_idx + 4] & 0x03) << 8) | (bytes[b_idx] & 0xff);
        data[d_idx + 1] =
            ((bytes[b_idx + 4] & 0x0c) << 6) | (bytes[b_idx + 1] & 0xff);
        data[d_idx + 2] =
            ((bytes[b_idx + 4] & 0x30) << 4) | (bytes[b_idx + 2] & 0xff);
        data[d_idx + 3] =
            ((bytes[b_idx + 4] & 0xc0) << 2) | (bytes[b_idx + 3] & 0xff);
    }
#endif

#if LOG_Q == 11
    for (size_t i = 0; i < dlen / 8; ++i) {
        b_idx = R11_DATA_OFFSET * i;
        d_idx = R11_BYTE_OFFSET * i;
        data[d_idx] =
            (((uint16_t)bytes[b_idx + 1] & 0xe0) << 3) | (bytes[b_idx] & 0xff);
        data[d_idx + 1] = (((uint16_t)bytes[b_idx + 2] & 0xfc) << 3) |
                          (bytes[b_idx + 1] & 0x1f);
        data[d_idx + 2] = (((uint16_t)bytes[b_idx + 4] & 0x80) << 3) |
                          (((uint16_t)bytes[b_idx + 3] & 0xff) << 2) |
                          (bytes[b_idx + 2] & 0x03);
        data[d_idx + 3] = (((uint16_t)bytes[b_idx + 5] & 0xf0) << 3) |
                          (bytes[b_idx + 4] & 0x7f);
        data[d_idx + 4] = (((uint16_t)bytes[b_idx + 6] & 0xfe) << 3) |
                          (bytes[b_idx + 5] & 0x0f);
        data[d_idx + 5] = (((uint16_t)bytes[b_idx + 8] & 0xc0) << 3) |
                          (((uint16_t)bytes[b_idx + 7] & 0xff) << 1) |
                          (bytes[b_idx + 6] & 0x01);
        data[d_idx + 6] = (((uint16_t)bytes[b_idx + 9] & 0xf8) << 3) |
                          (bytes[b_idx + 8] & 0x3f);
        data[d_idx + 7] = (((uint16_t)bytes[b_idx + 10] & 0xff) << 3) |
                          (bytes[b_idx + 9] & 0x07);
    }
#endif
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
        Rq_to_bytes(bytes + i * PKPOLY_BYTES, data[i], LWE_N);
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
        bytes_to_Rq(data[i], bytes + i * PKPOLY_BYTES, LWE_N);
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
void Rp_to_bytes(uint8_t bytes[CTPOLY1_BYTES], const uint16_t data[LWE_N]) {
    memset(bytes, 0, sizeof(uint8_t) * CTPOLY1_BYTES);
    for (size_t i = 0; i < LWE_N; ++i)
        memcpy(&(bytes[i]), &(data[i]), sizeof(uint8_t));
}

void Rp2_to_bytes(uint8_t bytes[CTPOLY2_BYTES], const uint16_t data[LWE_N]) {
    int b_idx = 0;
    int d_idx = 0;
    memset(bytes, 0, sizeof(uint8_t) * CTPOLY2_BYTES);
#if LOG_P2 == 5
    for (size_t i = 0; i < (LWE_N >> 3); ++i) {
        b_idx = 5 * i;
        d_idx = 8 * i;

        bytes[b_idx] = (data[d_idx] & 0x1f) | ((data[d_idx + 1] & 0x7) << 5);
        bytes[b_idx + 1] = (data[d_idx + 1] & 0x18) >> 3 |
                           ((data[d_idx + 2] & 0x1f) << 2) |
                           ((data[d_idx + 3] & 0x01) << 7);
        bytes[b_idx + 2] =
            ((data[d_idx + 3] & 0x1e) >> 1) | ((data[d_idx + 4] & 0xf) << 4);
        bytes[b_idx + 3] = ((data[d_idx + 4] & 0x10) >> 4) |
                           ((data[d_idx + 5] & 0x1f) << 1) |
                           ((data[d_idx + 6] & 0x3) << 6);
        bytes[b_idx + 4] =
            ((data[d_idx + 6] & 0x1c) >> 2) | ((data[d_idx + 7] & 0x1f) << 3);
    }
#endif

#if LOG_P2 == 8
    for (size_t i = 0; i < LWE_N; ++i)
        memcpy(&(bytes[i]), &(data[i]), sizeof(uint8_t));
#endif

#if LOG_P2 == 6
    for (size_t i = 0; i < (LWE_N >> 2); ++i) {
        b_idx = 3 * i;
        d_idx = 4 * i;

        bytes[b_idx] = (data[d_idx] & 0x3f) | ((data[d_idx + 1] & 0x3) << 6);
        bytes[b_idx + 1] =
            ((data[d_idx + 1] & 0x3c) >> 2) | ((data[d_idx + 2] & 0xf) << 4);
        bytes[b_idx + 2] =
            ((data[d_idx + 2] & 0x30) >> 4) | ((data[d_idx + 3] & 0x3f) << 2);
    }
#endif
}

/*************************************************
 * Name:        bytes_to_Rp
 *
 * Description: Transform to polynomial in Rq from bytes array
 *
 * Arguments:   - uint16_t *data: pointer to output polynomial in Rq
 *              - uint16_t *bytes: pointer to input bytes
 **************************************************/
void bytes_to_Rp(uint16_t data[LWE_N], const uint8_t bytes[CTPOLY1_BYTES]) {
    memset(data, 0, sizeof(uint16_t) * LWE_N);
    for (size_t i = 0; i < LWE_N; ++i)
        memcpy(&(data[i]), &(bytes[i]), sizeof(uint8_t));
}

void bytes_to_Rp2(uint16_t data[LWE_N], const uint8_t bytes[CTPOLY2_BYTES]) {
    int b_idx = 0;
    int d_idx = 0;
    memset(data, 0, sizeof(uint16_t) * LWE_N);
#if LOG_P2 == 5
    for (size_t i = 0; i < (LWE_N >> 3); ++i) {
        b_idx = 5 * i;
        d_idx = 8 * i;

        data[d_idx] = bytes[b_idx] & 0x1f;
        data[d_idx + 1] =
            ((bytes[b_idx] & 0xe0) >> 5) | ((bytes[b_idx + 1] & 0x3) << 3);
        data[d_idx + 2] = ((bytes[b_idx + 1] & 0x7c) >> 2);
        data[d_idx + 3] =
            ((bytes[b_idx + 1] & 0x80) >> 7) | ((bytes[b_idx + 2] & 0xf) << 1);
        data[d_idx + 4] =
            ((bytes[b_idx + 2] & 0xf0) >> 4) | ((bytes[b_idx + 3] & 0x1) << 4);
        data[d_idx + 5] = ((bytes[b_idx + 3] & 0x3e) >> 1);
        data[d_idx + 6] =
            ((bytes[b_idx + 3] & 0xc0) >> 6) | ((bytes[b_idx + 4] & 0x7) << 2);
        data[d_idx + 7] = (bytes[b_idx + 4] & 0xf8) >> 3;
    }
#endif

#if LOG_P2 == 8
    for (size_t i = 0; i < LWE_N; ++i)
        memcpy(&(data[i]), &(bytes[i]), sizeof(uint8_t));
#endif

#if LOG_P2 == 6
    for (size_t i = 0; i < (LWE_N >> 2); ++i) {
        b_idx = 3 * i;
        d_idx = 4 * i;

        data[d_idx] = bytes[b_idx] & 0x3f;
        data[d_idx + 1] =
            ((bytes[b_idx] & 0xc0) >> 6) | ((bytes[b_idx + 1] & 0xf) << 2);
        data[d_idx + 2] =
            ((bytes[b_idx + 1] & 0xf0) >> 4) | ((bytes[b_idx + 2] & 0x3) << 4);
        data[d_idx + 3] = (bytes[b_idx + 2] & 0xfc) >> 2;
    }
#endif
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
        Rp_to_bytes(bytes + i * CTPOLY1_BYTES, data[i]);
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
        bytes_to_Rp(data[i], bytes + i * CTPOLY1_BYTES);
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
void Sx_to_bytes(uint8_t *bytes, const uint8_t *data, const uint8_t data_len) {
    cmov(bytes, data, data_len, 1);
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
void bytes_to_Sx(uint8_t *data, const uint8_t *bytes, const uint8_t bytes_len) {
    cmov(data, bytes, bytes_len, 1);
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
void Sx_vec_to_bytes(uint8_t *bytes, const uint8_t *data[MODULE_RANK],
                     const uint8_t *data_len_arr) {
    size_t idx = 0;
    for (size_t i = 0; i < MODULE_RANK; ++i) {
        Sx_to_bytes(bytes + idx, data[i], data_len_arr[i]);
        idx += data_len_arr[i];
    }
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
void bytes_to_Sx_vec(uint8_t *data[MODULE_RANK], const uint8_t *bytes,
                     const uint8_t *bytes_len_arr) {
    size_t idx = 0;
    for (size_t i = 0; i < MODULE_RANK; ++i) {
        bytes_to_Sx(data[i], bytes + idx, bytes_len_arr[i]);
        idx += bytes_len_arr[i];
    }
}
