// clang-format off
#ifndef SMAUG_PARAMETERS_H
#define SMAUG_PARAMETERS_H

#define LOG_LWE_N 8             // log dim
#define LWE_N (1 << LOG_LWE_N)  // LWE dim and LWR dim

#if SMAUG_MODE == 1
#define SMAUG_NAMESPACE(s) cryptolab_smaug1_##s

#define LAMBDA 128              // security
#define MODULE_RANK 2           // rank of the module, in (2, 3, 5)
#define DIMENSION MODULE_RANK * LWE_N
#define NOISE_D1                // discrete Gaussian sampling option

#define LOG_Q 10                // public key modulus
#define LOG_P2 5                // ciphertext2 modulus
#define HS 140                  // Hamming weight of coefficient vector s
#define HR 132                  // Hamming weight of coefficient vector r

#define RD_ADD2 0x0400          // 2^(15 - LOG_P2)
#define RD_AND2 0xf800          // 2^16 - 2^(16 - LOG_P2)

#elif SMAUG_MODE == 3
#define SMAUG_NAMESPACE(s) cryptolab_smaug3_##s

#define LAMBDA 192              // security
#define MODULE_RANK 3           // rank of the module, in (2, 3, 5)
#define DIMENSION MODULE_RANK * LWE_N
#define NOISE_D2                // discrete Gaussian sampling option

#define LOG_Q 11                // public key modulus
#define LOG_P2 8                // ciphertext2 modulus
#define HS 198                  // Hamming weight of coefficient vector s
#define HR 151                  // Hamming weight of coefficient vector r

#define RD_ADD2 0x0080          // 2^(15 - LOG_P2)
#define RD_AND2 0xff00          // 2^16 - 2^(16 - LOG_P2)

#elif SMAUG_MODE == 5
#define SMAUG_NAMESPACE(s) cryptolab_smaug5_##s

#define LAMBDA 256              // security
#define MODULE_RANK 5           // rank of the module, in (2, 3, 5)
#define DIMENSION MODULE_RANK * LWE_N
#define NOISE_D1                // discrete Gaussian sampling option

#define LOG_Q 11                // public key modulus
#define LOG_P2 6                // ciphertext2 modulus
#define HS 176                  // Hamming weight of coefficient vector s
#define HR 160                  // Hamming weight of coefficient vector r

#define RD_ADD2 0x0200          // 2^(15 - LOG_P2)
#define RD_AND2 0xfc00          // 2^16 - 2^(16 - LOG_P2)
#endif

#define LOG_P 8	                // ciphertext modulus

#define RD_ADD 0x80             // 2^(15 - LOG_P)
#define RD_AND 0xff00           // 2^16 - 2^(16 - LOG_P)

#define LOG_T 1                     // plaintext modulus
#define T (1 << LOG_T)              // binary
#define _16_LOG_Q  (16 - LOG_Q)     // modulus (16 - LOG_Q)
#define _16_LOG_P  (16 - LOG_P)     // modulus (16 - LOG_P)
#define _16_LOG_P2 (16 - LOG_P2)    // modulus (16 - LOG_P2)
#define _16_LOG_T  (16 - LOG_T)     // modulus (16 - LOG_T)
#define DEC_ADD 0x4000              // 2^(15 - LOG_T)


// Size of keys and ciphertext
#define DELTA_BYTES (LWE_N / 8)                                                 // 32
#define T_BYTES (LWE_N / 8)                                                     // 32

#define SHARED_SECRETE_BYTES (32)                                               // 32
#define CRYPTO_BYTES SHARED_SECRETE_BYTES                                       // 32

#define CTPOLY1_BYTES (LWE_N)                                                   // element in R_p
#define CTPOLY2_BYTES (LOG_P2 * LWE_N / 8)                                      // element in R_p'

#define SKPOLY_BYTES (LWE_N / 4)                                                // 64
#define SKPOLYVEC_BYTES (SKPOLY_BYTES * MODULE_RANK)                            // vector of secret polynomials
#define CTPOLYVEC_BYTES (CTPOLY1_BYTES * MODULE_RANK)                           // vector with element in R_p


#define CIPHERTEXT_BYTES (CTPOLYVEC_BYTES + CTPOLY2_BYTES)                      // (vector c21, c22)

#define PKSEED_BYTES (32)                                                       // seed for a(x) 32
#define PKPOLY_BYTES ((LOG_Q * LWE_N) / 8)                                      // b(x)
#define PKPOLYVEC_BYTES (PKPOLY_BYTES * MODULE_RANK)                            // vector with element in R_q
#define PKPOLYMAT_BYTES (PKPOLYVEC_BYTES * MODULE_RANK)                         // matrix with element in R_q
#define PUBLICKEY_BYTES (PKSEED_BYTES + PKPOLYVEC_BYTES)                        // (A seed, b(x) vector)

#define PKE_SECRETKEY_BYTES (SKPOLYVEC_BYTES)                                   // s(x) vector
#define KEM_SECRETKEY_BYTES (PKE_SECRETKEY_BYTES + T_BYTES + PUBLICKEY_BYTES)   // s(x) vector, t, pk

// clang-format on

#endif // SMAUG_PARAMETERS_H
