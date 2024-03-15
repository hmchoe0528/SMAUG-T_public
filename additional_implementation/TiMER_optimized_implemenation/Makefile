INCDIR := include
SRCDIR := src
MAINDIR := test
BENCHDIR := benchmark
KATDIR := kat

# CC = clang
CC ?= /usr/bin/cc
RM = /bin/rm


CFLAGS = -mtune=native -O3 -fomit-frame-pointer -mavx2 -mbmi2 -mpopcnt
DFLAGS = -g3 -DDEBUG

SOURCES = $(SRCDIR)/ciphertext.c $(SRCDIR)/dg.c $(SRCDIR)/hash.c $(SRCDIR)/hwt.c $(SRCDIR)/indcpa.c $(SRCDIR)/io.c $(SRCDIR)/kem.c $(SRCDIR)/key.c $(SRCDIR)/pack.c $(SRCDIR)/poly.c $(SRCDIR)/verify.c
SOURCESKECCAK = $(SOURCES) $(SRCDIR)/fips202x4.c $(SRCDIR)/fips202.c $(SRCDIR)/keccak/KeccakP-1600-times4-SIMD256.c $(SRCDIR)/keccak/KeccakP-1600-AVX2.S

HEADERS = $(INCDIR)/ciphertext.h $(INCDIR)/dg.h $(INCDIR)/hash.h $(INCDIR)/hwt.h $(INCDIR)/indcpa.h $(INCDIR)/io.h $(INCDIR)/kem.h $(INCDIR)/key.h $(INCDIR)/pack.h $(INCDIR)/poly.h $(INCDIR)/verify.h $(INCDIR)/align.h
HEADERSKECCAK = $(HEADERS) $(INCDIR)/fips202x4.h $(INCDIR)/fips202.h
INCLUDE = -I./$(INCDIR) -I./$(INCDIR)/keccak

SOURCES_MAIN = $(SOURCESKECCAK) $(MAINDIR)/main.c $(SRCDIR)/randombytes.c
SOURCES_KAT = $(SOURCESKECCAK) $(KATDIR)/kat_kem.c $(KATDIR)/PQCgenKAT_kem.c $(SRCDIR)/rng.c
SOURCES_SPEED = $(SOURCESKECCAK) $(BENCHDIR)/PQC_bench.c $(SRCDIR)/randombytes.c
#SOURCES_SPEED = $(SOURCESKECCAK) $(BENCHDIR)/speed.c $(BENCHDIR)/cpucycles.c $(BENCHDIR)/speed_print.c $(SRCDIR)/randombytes.c

all: \
	main \
	speed \

shared: timer.so \

main: timer-main \

speed: timer-speed \

kat: PQCgenKAT_timer \


timer.so: $(SOURCESKECCAK) $(HEADERSKECCAK)
	$(CC) -shared -fPIC $(CFLAGS) -DTIMER_MODE=1 $(SOURCESKECCAK) $(INCLUDE) -o timer.so

timer-main: $(SOURCES_MAIN)
	$(CC) $(CFLAGS) -DTIMER_MODE=1 $(SOURCES_MAIN) $(INCLUDE) -o timer-main

timer-speed: $(SOURCES_SPEED)
	$(CC) $(CFLAGS) -DTIMER_MODE=1 $(SOURCES_SPEED) $(INCLUDE) -o timer-speed


PQCgenKAT_timer: $(SOURCES_KAT)
	$(CC) $(CFLAGS) -DTIMER_MODE=1 -DCRYPTO_ALGNAME=\"TiMER_KEM\" $(SOURCES_KAT) $(INCLUDE) -o PQCgenKAT_timer -lcrypto


clean:
	-$(RM) -rf timer-main
	-$(RM) -rf timer-speed
	-$(RM) -rf PQCgenKAT_timer
