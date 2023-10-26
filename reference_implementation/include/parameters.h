// clang-format off
#ifndef SMAUG_PARAMETERS_H
#define SMAUG_PARAMETERS_H


#if SMAUG_MODE == 1
#define MODULE_RANK 2           // rank of the module, in (2, 3, 4)
#define NOISE_D1                // discrete Gaussian sampling option

#define HS 70                  // Hamming weight of coefficient vector s
#define HR 66                  // Hamming weight of coefficient vector r

#define LAMBDA 128              // security

#elif SMAUG_MODE == 3
#define MODULE_RANK 3           // rank of the module, in (2, 3, 4)
#define NOISE_D1                // discrete Gaussian sampling option

#define HS 50                  // Hamming weight of coefficient vector s
#define HR 49                  // Hamming weight of coefficient vector r

#define LAMBDA 192              // security

#elif SMAUG_MODE == 5
#define MODULE_RANK 5           // rank of the module, in (2, 3, 4)
#define NOISE_D1                // discrete Gaussian sampling option

#define HS 29                  // Hamming weight of coefficient vector s
#define HR 28                  // Hamming weight of coefficient vector r

#define LAMBDA 256              // security

#endif

#define LOG_LWE_N 8             // log dim
#define LWE_N (1 << LOG_LWE_N)  // LWE dim and LWR dim

#define LOG_Q 10                // public key modulus
#define LOG_P 8	                // ciphertext modulus

#define RD_ADD 0x80             // 2^(15 - LOG_P)
#define RD_AND 0xff00           // 2^16 - 2^(16 - LOG_P)

#define LOG_T 1                 // plaintext modulus
#define T (1 << LOG_T)          // binary
#define _16_LOG_Q (16 - LOG_Q)  // modulus (16 - LOG_Q)
#define _16_LOG_P (16 - LOG_P)  // modulus (16 - LOG_P)
#define _16_LOG_T (16 - LOG_T)  // modulus (16 - LOG_T)
#define DEC_ADD 0x4000          // 2^(15 - LOG_T)


// Size of keys and ciphertext
#define DELTA_BYTES (LWE_N / 8)                                                 // 32
#define T_BYTES (LWE_N / 8)                                                     // 32

#define SHARED_SECRETE_BYTES 32                                                 // 32
#define CRYPTO_BYTES SHARED_SECRETE_BYTES                                       // 32

#define SKPOLY_BYTES (sizeof(uint8_t) * HS)                                     // s(x)
#define CTPOLY_BYTES (sizeof(uint8_t) * LWE_N)                                  // element in R_p

#define SKPOLYVEC_BYTES (SKPOLY_BYTES * MODULE_RANK)                            // s(x) vector
#define CTPOLYVEC_BYTES (CTPOLY_BYTES * MODULE_RANK)                            // vector with element in R_p


#define PKE_SECRETKEY_BYTES (SKPOLYVEC_BYTES + sizeof(uint8_t) * MODULE_RANK)   // s(x) vector, neg_start vector
#define KEM_SECRETKEY_BYTES (PKE_SECRETKEY_BYTES + T_BYTES)                     // s(x) vector, neg_start vector, t

#define CIPHERTEXT_BYTES (CTPOLYVEC_BYTES + CTPOLY_BYTES)                       // (vector c21, c22)

#define PKSEED_BYTES 32                                                         // seed for a(x) 32
#define PKPOLY_BYTES ((LOG_Q * LWE_N) / 8)                                      // b(x)
#define PKPOLYVEC_BYTES (PKPOLY_BYTES * MODULE_RANK)                            // vector with element in R_q
#define PKPOLYMAT_BYTES (PKPOLYVEC_BYTES * MODULE_RANK)                         // matrix with element in R_q
#define PUBLICKEY_BYTES (PKSEED_BYTES + PKPOLYVEC_BYTES)                        // (A seed, b(x) vector)

// clang-format on

#endif // SMAUG_PARAMETERS_H
