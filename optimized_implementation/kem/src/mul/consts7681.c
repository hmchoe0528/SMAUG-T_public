#include "config.h"
#include "consts.h"
#include <stdint.h>

#define P 7681
#define PINV -7679 // p^-1 mod 2^16
#define MONT -3593 // 2^16 mod p
#define MONT_PINV -9
#define V 17474 // floor(2^27/p + 0.5)
#define SHIFT 16
#define F 1912       // mont^2/256
#define F_PINV -2184 // PINV*F

__attribute__((aligned(32))) const int16_t pdata7681[] = {
#define _16XP 0
    P,         P,         P,         P,         P,         P,         P,
    P,         P,         P,         P,         P,         P,         P,
    P,         P,

#define _16XPINV 16
    PINV,      PINV,      PINV,      PINV,      PINV,      PINV,      PINV,
    PINV,      PINV,      PINV,      PINV,      PINV,      PINV,      PINV,
    PINV,      PINV,

#define _16XV 32
    V,         V,         V,         V,         V,         V,         V,
    V,         V,         V,         V,         V,         V,         V,
    V,         V,

#define _16XSHIFT 48
    SHIFT,     SHIFT,     SHIFT,     SHIFT,     SHIFT,     SHIFT,     SHIFT,
    SHIFT,     SHIFT,     SHIFT,     SHIFT,     SHIFT,     SHIFT,     SHIFT,
    SHIFT,     SHIFT,

#define _16XMONT_PINV 64
    MONT_PINV, MONT_PINV, MONT_PINV, MONT_PINV, MONT_PINV, MONT_PINV, MONT_PINV,
    MONT_PINV, MONT_PINV, MONT_PINV, MONT_PINV, MONT_PINV, MONT_PINV, MONT_PINV,
    MONT_PINV, MONT_PINV,

#define _16XMONT 80
    MONT,      MONT,      MONT,      MONT,      MONT,      MONT,      MONT,
    MONT,      MONT,      MONT,      MONT,      MONT,      MONT,      MONT,
    MONT,      MONT,

#define _16XF_PINV 96
    F_PINV,    F_PINV,    F_PINV,    F_PINV,    F_PINV,    F_PINV,    F_PINV,
    F_PINV,    F_PINV,    F_PINV,    F_PINV,    F_PINV,    F_PINV,    F_PINV,
    F_PINV,    F_PINV,

#define _16XF 112
    F,         F,         F,         F,         F,         F,         F,
    F,         F,         F,         F,         F,         F,         F,
    F,         F,

#define _ZETAS 128
    28865,     28865,     28865,     28865,     28865,     28865,     28865,
    28865,     28865,     28865,     28865,     28865,     28865,     28865,
    28865,     28865,     3777,      3777,      3777,      3777,      3777,
    3777,      3777,      3777,      3777,      3777,      3777,      3777,
    3777,      3777,      3777,      3777,      -10350,    -10350,    -10350,
    -10350,    -10350,    -10350,    -10350,    -10350,    -10350,    -10350,
    -10350,    -10350,    -10350,    -10350,    -10350,    -10350,    -3182,
    -3182,     -3182,     -3182,     -3182,     -3182,     -3182,     -3182,
    -3182,     -3182,     -3182,     -3182,     -3182,     -3182,     -3182,
    -3182,     4496,      4496,      4496,      4496,      4496,      4496,
    4496,      4496,      -7244,     -7244,     -7244,     -7244,     -7244,
    -7244,     -7244,     -7244,     -3696,     -3696,     -3696,     -3696,
    -3696,     -3696,     -3696,     -3696,     -1100,     -1100,     -1100,
    -1100,     -1100,     -1100,     -1100,     -1100,     16425,     16425,
    16425,     16425,     16425,     16425,     16425,     16425,     16425,
    16425,     16425,     16425,     16425,     16425,     16425,     16425,
    3625,      3625,      3625,      3625,      3625,      3625,      3625,
    3625,      3625,      3625,      3625,      3625,      3625,      3625,
    3625,      3625,      14744,     14744,     14744,     14744,     14744,
    14744,     14744,     14744,     -4974,     -4974,     -4974,     -4974,
    -4974,     -4974,     -4974,     -4974,     2456,      2456,      2456,
    2456,      2456,      2456,      2456,      2456,      2194,      2194,
    2194,      2194,      2194,      2194,      2194,      2194,

#define _TWIST32 288
    -9,        -529,      32738,     -1851,     -9,        29394,     -7508,
    -20435,    -9,        26288,     9855,      -19215,    -9,        16006,
    -12611,    -964,      -3593,     -17,       -1054,     3781,      -3593,
    3794,      2732,      -2515,     -3593,     1712,      2175,      -3343,
    -3593,     -3450,     -2883,     1084,      16279,     26288,     -8558,
    -6297,     11783,     -25648,    14351,     -25733,    21066,     -23882,
    -17440,    -7304,     -26279,    16791,     22124,     -20435,    -3689,
    1712,      -1390,     -1689,     7,         -1072,     -1521,     1403,
    -438,      -2378,     -1056,     -3208,     1881,      -3177,     -404,
    -2515,     2816,      -22039,    9855,      21168,     -19394,    30255,
    -27132,    17013,     23489,     -18506,    1869,      10145,     -3114,
    9650,      -15358,    -24232,    2816,      -2071,     2175,      -3408,
    -1986,     -2001,     3588,      -1931,     -1599,     2998,      3405,
    1441,      2006,      434,       2,         -3752,     1724,      -24214,
    6032,      -19215,    -21467,    29453,     -16655,    32124,     4505,
    13686,     -25946,    -12790,    -23668,    -31518,    14351,     12449,
    3772,      3434,      -2160,     -3343,     549,       -1779,     -783,
    1404,      -103,      2422,      3750,      -1526,     2956,      226,
    -1521,     3745,      -11655,    -1715,     24743,     26766,     23754,
    22943,     -2722,     4880,      18242,     26621,     -32329,    -10333,
    -22593,    -16715,    30426,     2858,      121,       -179,      -3417,
    3214,      2250,      -1121,     -1698,     -3312,     834,       3581,
    -3145,     -3677,     2495,      -2891,     730,       -2262,     21066,
    -4624,     -24573,    -16186,    29667,     -30597,    23225,     10333,
    -15998,    6510,      -3558,     17491,     11792,     30255,     -4693,
    21723,     -438,      3568,      -1533,     -2874,     3555,      -3461,
    2233,      3677,      -638,      -658,      -486,      -429,      3600,
    -2001,     -2133,     -293,      -20469,    -23882,    26663,     14718,
    -9488,     -16885,    -26220,    17636,     -19351,    -17082,    2722,
    2807,      10972,     -5990,     29871,     -5299,     -1525,     -2378,
    -1497,     -642,      -1296,     2059,      -3692,     -796,      617,
    -3770,     1698,      -777,      -3364,     -2918,     -2385,     -3763,
    -4983,     18745,     -17440,    -32695,    -4505,     -12261,    -32252,
    23933,     2073,      -30938,    30136,     16083,     -21467,    -32414,
    -8908,     -947,      -1399,     -2247,     -1056,     3657,      103,
    -1509,     -1532,     893,       -2535,     -1242,     1464,      -1837,
    549,       -670,      -2764,     589,       -9,        -1851,     -8558,
    -22039,    -9,        4573,      -26441,    16791,     -9,        -6297,
    6032,      -4624,     -9,        -9513,     9360,      16006,     -3593,
    3781,      -1390,     -2071,     -3593,     -2083,     2743,      -3177,
    -3593,     -1689,     -2160,     3568,      -3593,     3287,      1168,
    -3450,     1724,      -19215,    24743,     -4624,     -21766,    1007,
    -15358,    -25648,    -4983,     -7304,     -16092,    -13711,    21399,
    4573,      -12611,    29394,     3772,      -3343,     -3417,     3568,
    -2310,     1519,      2,         -1072,     -1399,     -3208,     -1756,
    2161,      1431,      -2083,     -2883,     3794,      -20469,    14718,
    -17440,    16638,     -15307,    12449,     12269,     -22764,    -26382,
    -5452,     -25946,    -11996,    5759,      -964,      -26441,    9087,
    -1525,     -642,      -1056,     1278,      -1483,     3745,      -2579,
    -236,      -2830,     692,       3750,      2340,      -1921,     1084,
    2743,      1407,      5930,      -23933,    -16092,    -18506,    11792,
    -28805,    -27132,    -5990,     -5913,     27243,     -13933,    6510,
    -26279,    -6766,     -7508,     16791,     810,       -893,      -1756,
    2998,      3600,      -1669,     3588,      -2918,     -1305,     -2965,
    915,       -658,      1881,      402,       2732,      -3177,     -18191,
    -15221,    -26262,    2739,      -828,      -15145,    -8908,     -9633,
    20315,     -15111,    -10478,    802,       -20870,    -4565,     22124,
    26783,     -2319,     3723,      1386,      1203,      -2876,     -2345,
    -2764,     -929,      -1701,     -3335,     -3310,     -222,      -1414,
    -2005,     -404,      2719,      4505,      -5452,     -3456,     -28958,
    -14121,    32124,     17602,     2526,      2073,      22790,     -24052,
    9633,      -21766,    -20435,    21868,     3524,      -103,      692,
    -3456,     2786,      -1321,     1404,      194,       3550,      -2535,
    3334,      2572,      929,       -2310,     -2515,     -660,      1476,
    7491,      -12790,    -22875,    16885,     22568,     27858,     10478,
    20119,     31177,     5299,      -21860,    -10495,    -3114,     1007,
    8472,      9650,      -2237,     -1526,     -859,      -2059,     2088,
    2258,      3310,      151,       1993,      3763,      -3428,     -2815,
    2006,      1519,      -3816,     434,       -5913,     27636,     -32329,
    -2952,     29667,     23984,     -10409,    8831,      -11792,    14138,
    13541,     31518,     11783,     30844,     -15358,    -19274,    -1305,
    1012,      -3145,     1144,      3555,      -592,      2391,      1151,
    -3600,     826,       2789,      -226,      7,         124,       2,
    2230,

#define _TWIST4 800
    -9,        -16425,    -28865,    10350,     -3593,     -3625,     -3777,
    3182,      -9,        -10350,    28865,     16425,     -3593,     -3182,
    3777,      3625,      -9,        4496,      -10350,    14744,     -3593,
    -3696,     -3182,     2456,      -9,        4974,      -16425,    7244,
    -3593,     -2194,     -3625,     1100,      -9,        -11655,    4496,
    -18191,    -3593,     121,       -3696,     -2319,     -9,        -22593,
    7244,      -20315,    -3593,     2495,      1100,      1701,      -9,
    -18191,    14744,     -23754,    -3593,     -2319,     2456,      -2250,
    -9,        -20870,    4974,      -22593,    -3593,     -1414,     -2194,
    2495};
