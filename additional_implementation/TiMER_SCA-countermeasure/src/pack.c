#include "pack.h"
#include <stdlib.h>

/*************************************************
 * Name:        Rq_to_bytes
 *
 * Description: Transform to bytes array from polynomial in Rq
 *
 * Arguments:   - uint16_t *bytes: pointer to output bytes
 *              - uint16_t *data: pointer to input polynomial in Rq
 *              - size_t dlen: date length
 **************************************************/
void Rq_to_bytes(uint8_t bytes[PKPOLY_BYTES], const poly *data) {
    uint16_t tmp[LWE_N] = {0};
    int b_idx = 0, d_idx = 0;

    for (int i = 0; i < LWE_N; ++i) {
        bytes[i] = data->coeffs[i] >> 8;
        tmp[i] = data->coeffs[i] & 0x00c0;
    }
    uint16_t buf[DATA_OFFSET * 2] = {0};
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < DATA_OFFSET; ++j) {
            buf[b_idx + j] = tmp[d_idx + j] << 8;
            buf[b_idx + j] |= tmp[d_idx + DATA_OFFSET + j] << 6;
            buf[b_idx + j] |= tmp[d_idx + DATA_OFFSET * 2 + j] << 4;
            buf[b_idx + j] |= tmp[d_idx + DATA_OFFSET * 3 + j] << 2;
            buf[b_idx + j] |= tmp[d_idx + DATA_OFFSET * 4 + j];
            buf[b_idx + j] |= tmp[d_idx + DATA_OFFSET * 5 + j] >> 2;
            buf[b_idx + j] |= tmp[d_idx + DATA_OFFSET * 6 + j] >> 4;
            buf[b_idx + j] |= tmp[d_idx + DATA_OFFSET * 7 + j] >> 6;
        }
        b_idx += DATA_OFFSET;
        d_idx += DATA_OFFSET * 8;
    }
    memcpy(bytes + LWE_N, (uint8_t *)buf, DATA_OFFSET * 2 * sizeof(uint16_t));
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
void bytes_to_Rq(poly *data, const uint8_t bytes[PKPOLY_BYTES]) {
    uint16_t tmp[LWE_N] = {0};
    int b_idx = 0, d_idx = 0;

    for (int i = 0; i < LWE_N; ++i)
        data->coeffs[i] = (uint16_t)bytes[i] << 8;

    uint16_t buf[DATA_OFFSET * 2] = {0};
    memcpy((uint8_t *)buf, bytes + LWE_N, DATA_OFFSET * 2 * sizeof(uint16_t));

    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < DATA_OFFSET; ++j) {
            tmp[d_idx + j] = buf[b_idx + j] >> 8;
            tmp[d_idx + DATA_OFFSET + j] = buf[b_idx + j] >> 6;
            tmp[d_idx + DATA_OFFSET * 2 + j] = buf[b_idx + j] >> 4;
            tmp[d_idx + DATA_OFFSET * 3 + j] = buf[b_idx + j] >> 2;
            tmp[d_idx + DATA_OFFSET * 4 + j] = buf[b_idx + j];
            tmp[d_idx + DATA_OFFSET * 5 + j] = buf[b_idx + j] << 2;
            tmp[d_idx + DATA_OFFSET * 6 + j] = buf[b_idx + j] << 4;
            tmp[d_idx + DATA_OFFSET * 7 + j] = buf[b_idx + j] << 6;
        }
        b_idx += DATA_OFFSET;
        d_idx += DATA_OFFSET * 8;
    }
    for (int i = 0; i < LWE_N; ++i)
        data->coeffs[i] |= tmp[i] & 0x00c0;
}

/*************************************************
 * Name:        Rq_vec_to_bytes
 *
 * Description: Transform to bytes array from a vector of  polynomial in Rq
 *
 * Arguments:   - uint16_t *bytes: pointer to output bytes
 *              - uint16_t *data: pointer to input vector of polynomial in Rq
 **************************************************/
void Rq_vec_to_bytes(uint8_t bytes[PKPOLYVEC_BYTES], const polyvec *data) {
    for (size_t i = 0; i < MODULE_RANK; ++i)
        Rq_to_bytes(bytes + i * PKPOLY_BYTES, &(data->vec[i]));
}

/*************************************************
 * Name:        bytes_to_Rq_vec
 *
 * Description: Transform to bytes array from a vector of  polynomial in Rq
 *
 * Arguments:   - uint16_t *data: pointer to output a vector of poly in Rq
 *              - uint16_t *bytes: pointer to input bytes
 **************************************************/
void bytes_to_Rq_vec(polyvec *data, const uint8_t bytes[PKPOLYVEC_BYTES]) {
    for (size_t i = 0; i < MODULE_RANK; ++i)
        bytes_to_Rq(&(data->vec[i]), bytes + i * PKPOLY_BYTES);
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
                     const polyvec data[MODULE_RANK]) {
    for (size_t i = 0; i < MODULE_RANK; ++i)
        Rq_vec_to_bytes(bytes + i * PKPOLYVEC_BYTES, &(data[i]));
}

/*************************************************
 * Name:        bytes_to_Rq_mat
 *
 * Description: Transform to bytes array from a matrix of  polynomial in Rq
 *
 * Arguments:   - uint16_t *data: pointer to output a matrix of poly in Rq
 *              - uint16_t *bytes: pointer to input bytes
 **************************************************/
void bytes_to_Rq_mat(polyvec data[MODULE_RANK],
                     const uint8_t bytes[PKPOLYMAT_BYTES]) {

    for (size_t i = 0; i < MODULE_RANK; ++i)
        bytes_to_Rq_vec(&(data[i]), bytes + i * PKPOLYVEC_BYTES);
}

/*************************************************
 * Name:        Rp_to_bytes
 *
 * Description: Transform to bytes array from polynomial in Rp
 *
 * Arguments:   - uint16_t *bytes: pointer to output bytes
 *              - uint16_t *data: pointer to input polynomial in Rp
 **************************************************/
void Rp_to_bytes(uint8_t bytes[CTPOLY1_BYTES], const poly *data) {
    memset(bytes, 0, sizeof(uint8_t) * CTPOLY1_BYTES);
    for (size_t i = 0; i < LWE_N; ++i)
        memcpy(&(bytes[i]), &(data->coeffs[i]), sizeof(uint8_t));
}

void Rp2_to_bytes(uint8_t bytes[CTPOLY2_BYTES], const poly *data) {
    memset(bytes, 0, sizeof(uint8_t) * CTPOLY2_BYTES);

#if LOG_P2 == 8
    for (size_t i = 0; i < LWE_N; ++i)
        memcpy(&(bytes[i]), &(data->coeffs[i]), sizeof(uint8_t));
#endif

#if LOG_P2 == 3
    size_t b_idx = 0;
    for (size_t i = 0; i < LWE_N; i += 8) {

        uint32_t temp = 0;
        for (int j = 0; j < 8; ++j) {

            temp |= ((data->coeffs[i + j] & 0x07) << (3 * (7 - j)));
        }

        bytes[b_idx++] = (temp >> 16) & 0xFF;
        bytes[b_idx++] = (temp >> 8) & 0xFF;
        bytes[b_idx++] = temp & 0xFF;
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
void bytes_to_Rp(poly *data, const uint8_t bytes[CTPOLY1_BYTES]) {
    memset(data, 0, sizeof(poly));
    for (size_t i = 0; i < LWE_N; ++i)
        memcpy(&(data->coeffs[i]), &(bytes[i]), sizeof(uint8_t));
}

void bytes_to_Rp2(poly *data, const uint8_t bytes[CTPOLY2_BYTES]) {
    memset(data, 0, sizeof(uint16_t) * LWE_N);

#if LOG_P2 == 8
    for (size_t i = 0; i < LWE_N; ++i)
        memcpy(&(data->coeffs[i]), &(bytes[i]), sizeof(uint8_t));
#endif

#if LOG_P2 == 3
    size_t d_idx = 0;
    for (size_t b_idx = 0; b_idx < CTPOLY2_BYTES; b_idx += 3) {
        data->coeffs[d_idx++] = bytes[b_idx] >> 5;
        data->coeffs[d_idx++] = (bytes[b_idx] >> 2) & 0x07;
        uint8_t temp = (bytes[b_idx] & 0x03) << 1;

        if (b_idx + 1 < CTPOLY2_BYTES) {
            temp |= bytes[b_idx + 1] >> 7;
            data->coeffs[d_idx++] = temp;
            data->coeffs[d_idx++] = (bytes[b_idx + 1] >> 4) & 0x07;
            data->coeffs[d_idx++] = (bytes[b_idx + 1] >> 1) & 0x07;
            temp = (bytes[b_idx + 1] & 0x01) << 2;
        }

        if (b_idx + 2 < CTPOLY2_BYTES) {
            temp |= bytes[b_idx + 2] >> 6;
            data->coeffs[d_idx++] = temp;
            data->coeffs[d_idx++] = (bytes[b_idx + 2] >> 3) & 0x07;
            data->coeffs[d_idx++] = bytes[b_idx + 2] & 0x07;
        }
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
void Rp_vec_to_bytes(uint8_t bytes[CTPOLYVEC_BYTES], const polyvec *data) {
    for (size_t i = 0; i < MODULE_RANK; ++i)
        Rp_to_bytes(bytes + i * CTPOLY1_BYTES, &(data->vec[i]));
}

/*************************************************
 * Name:        bytes_to_Rp_vec
 *
 * Description: Transform to bytes array from a vector of  polynomial in Rp
 *
 * Arguments:   - uint16_t *data: pointer to output a vector of poly in Rp
 *              - uint16_t *bytes: pointer to input bytes
 **************************************************/
void bytes_to_Rp_vec(polyvec *data, const uint8_t bytes[CTPOLYVEC_BYTES]) {
    for (size_t i = 0; i < MODULE_RANK; ++i)
        bytes_to_Rp(&(data->vec[i]), bytes + i * CTPOLY1_BYTES);
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
