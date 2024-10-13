INCDIR := include
SRCDIR := src
MAINDIR := test
BENCHDIR := benchmark
KATDIR := kat

NTTINCDIR := $(INCDIR)/mul
NTTSRCDIR := $(SRCDIR)/mul

# CC = clang
CC ?= /usr/bin/cc
RM = /bin/rm


CFLAGS = -march=native -mtune=native -O3 -fomit-frame-pointer -mavx2 -mbmi2 -mpopcnt
DFLAGS = -g3 -DDEBUG


NTTSOURCES = $(NTTSRCDIR)/ntt.c $(NTTSRCDIR)/ntt.S $(NTTSRCDIR)/invntt.S $(NTTSRCDIR)/basemul.S $(NTTSRCDIR)/consts7681.c $(NTTSRCDIR)/consts10753.c
NTTHEADERS = $(NTTINCDIR)/config.h $(NTTINCDIR)/ntt.h $(NTTINCDIR)/consts.h $(NTTINCDIR)/fq.inc $(NTTINCDIR)/shuffle.inc

SOURCES = $(SRCDIR)/ciphertext.c $(SRCDIR)/dg.c $(SRCDIR)/hash.c $(SRCDIR)/hwt.c $(SRCDIR)/indcpa.c $(SRCDIR)/io.c $(SRCDIR)/kem.c $(SRCDIR)/key.c $(SRCDIR)/pack.c $(SRCDIR)/poly.c $(SRCDIR)/verify.c $(SRCDIR)/cbd.c $(NTTSOURCES)
SOURCESKECCAK = $(SOURCES) $(SRCDIR)/fips202x4.c $(SRCDIR)/fips202.c $(SRCDIR)/keccak/KeccakP-1600-times4-SIMD256.c $(SRCDIR)/keccak/KeccakP-1600-AVX2.S

HEADERS = $(INCDIR)/ciphertext.h $(INCDIR)/dg.h $(INCDIR)/hash.h $(INCDIR)/hwt.h $(INCDIR)/indcpa.h $(INCDIR)/io.h $(INCDIR)/kem.h $(INCDIR)/key.h $(INCDIR)/pack.h $(INCDIR)/poly.h $(INCDIR)/verify.h $(INCDIR)/cbd.h $(NTTHEADERS)
HEADERSKECCAK = $(HEADERS) $(INCDIR)/fips202x4.h $(INCDIR)/fips202.h
INCLUDE = -I./$(INCDIR) -I./$(INCDIR)/keccak -I./$(NTTINCDIR)

SOURCES_MAIN = $(SOURCESKECCAK) $(MAINDIR)/main.c $(SRCDIR)/randombytes.c
SOURCES_KAT = $(SOURCESKECCAK) $(KATDIR)/kat_kem.c $(KATDIR)/PQCgenKAT_kem.c $(SRCDIR)/rng.c
SOURCES_SPEED = $(SOURCESKECCAK) $(BENCHDIR)/PQC_bench.c $(SRCDIR)/randombytes.c
# SOURCES_SPEED = $(SOURCESKECCAK) $(BENCHDIR)/speed.c $(BENCHDIR)/cpucycles.c $(BENCHDIR)/speed_print.c $(SRCDIR)/randombytes.c

all: \
	main \
	speed \

shared: \
	TiMER.so \

main: \
	TiMER-main \

speed: \
	TiMER-speed \

kat: \
	PQCgenKAT_TiMER \


TiMER.so: $(SOURCESKECCAK) $(HEADERSKECCAK)
	$(CC) -shared -fPIC $(CFLAGS) -DTiMER_MODE=1 $(SOURCESKECCAK) $(INCLUDE) -o TiMER.so

TiMER-main: $(SOURCES_MAIN)
	$(CC) $(CFLAGS) -DTiMER_MODE=1 $(SOURCES_MAIN) $(INCLUDE) -o TiMER-main

TiMER-speed: $(SOURCES_SPEED)
	$(CC) $(CFLAGS) -DTiMER_MODE=1 $(SOURCES_SPEED) $(INCLUDE) -o TiMER-speed

PQCgenKAT_TiMER: $(SOURCES_KAT)
	$(CC) $(CFLAGS) -DTiMER_MODE=1 -DCRYPTO_ALGNAME=\"TiMER_KEM\" $(SOURCES_KAT) $(INCLUDE) -o PQCgenKAT_TiMER -lcrypto


clean:
	-$(RM) -rf TiMER-main
	-$(RM) -rf TiMER-speed
	-$(RM) -rf PQCgenKAT_TiMER
