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
CFLAGS90S = $(CFLAGS) -maes
DFLAGS = -g3 -DDEBUG

NTTSOURCES = $(NTTSRCDIR)/ntt.c $(NTTSRCDIR)/ntt.S $(NTTSRCDIR)/invntt.S $(NTTSRCDIR)/basemul.S $(NTTSRCDIR)/consts7681.c $(NTTSRCDIR)/consts10753.c
NTTHEADERS = $(NTTINCDIR)/config.h $(NTTINCDIR)/ntt.h $(NTTINCDIR)/consts.h $(NTTINCDIR)/fq.inc $(NTTINCDIR)/shuffle.inc

SOURCES = $(SRCDIR)/ciphertext.c $(SRCDIR)/dg.c $(SRCDIR)/hwt.c $(SRCDIR)/indcpa.c $(SRCDIR)/io.c $(SRCDIR)/kem.c $(SRCDIR)/key.c $(SRCDIR)/pack.c $(SRCDIR)/poly.c $(SRCDIR)/verify.c $(SRCDIR)/cbd.c $(NTTSOURCES)
SOURCES90S = $(SOURCES) $(SRCDIR)/symmetric.c $(SRCDIR)/aes256ctr.c

HEADERS = $(INCDIR)/ciphertext.h $(INCDIR)/dg.h $(INCDIR)/hwt.h $(INCDIR)/indcpa.h $(INCDIR)/io.h $(INCDIR)/kem.h $(INCDIR)/key.h $(INCDIR)/pack.h $(INCDIR)/poly.h $(INCDIR)/verify.h $(INCDIR)/cbd.h $(INCDIR)/symmetric.h $(NTTHEADERS) 
HEADERS90S = $(HEADERS) $(INCDIR)/sha2.h $(INCDIR)/aes256ctr.h
INCLUDE = -I./$(INCDIR) -I./$(NTTINCDIR)

SOURCES_MAIN = $(SOURCESKECCAK) $(MAINDIR)/main.c $(SRCDIR)/randombytes.c
SOURCES_KAT = $(SOURCESKECCAK) $(KATDIR)/kat_kem.c $(KATDIR)/PQCgenKAT_kem.c $(SRCDIR)/rng.c
SOURCES_SPEED = $(SOURCESKECCAK) $(BENCHDIR)/PQC_bench.c $(SRCDIR)/randombytes.c
# SOURCES_SPEED = $(SOURCESKECCAK) $(BENCHDIR)/speed.c $(BENCHDIR)/cpucycles.c $(BENCHDIR)/speed_print.c $(SRCDIR)/randombytes.c

SOURCES90S_MAIN = $(SOURCES90S) $(MAINDIR)/main.c $(SRCDIR)/randombytes.c
SOURCES90S_KAT = $(SOURCES90S) $(KATDIR)/kat_kem.c $(KATDIR)/PQCgenKAT_kem.c $(SRCDIR)/rng.c
SOURCES90S_SPEED = $(SOURCES90S) $(BENCHDIR)/PQC_bench.c $(SRCDIR)/randombytes.c
# SOURCES90S_SPEED = $(SOURCES90S) $(BENCHDIR)/speed.c $(BENCHDIR)/cpucycles.c $(BENCHDIR)/speed_print.c $(SRCDIR)/randombytes.c


all: \
	main \
	speed \

shared: \
	TiMER-90s.so \

main: \
	TiMER-main-90s \

speed: \
	TiMER-speed-90s \

kat: \
	PQCgenKAT_TiMER-90s \

TiMER-90s.so: $(SOURCES90S) $(HEADERS90S)
	$(CC) -shared -fPIC $(CFLAGS90S) -DTiMER_MODE=1 $(SOURCES90S) $(INCLUDE) -o TiMER-90s.so -lcrypto

TiMER-main-90s: $(SOURCES90S_MAIN)
	$(CC) $(CFLAGS90S) -DTiMER_MODE=1 $(SOURCES90S_MAIN) $(INCLUDE) -o TiMER-main-90s -lcrypto

TiMER-speed-90s: $(SOURCES90S_SPEED)
	$(CC) $(CFLAGS90S) -DTiMER_MODE=1 $(SOURCES90S_SPEED) $(INCLUDE) -o TiMER-speed-90s -lcrypto

PQCgenKAT_TiMER-90s: $(SOURCES90S_KAT)
	$(CC) $(CFLAGS90S) -DTiMER_MODE=1 -DCRYPTO_ALGNAME=\"TiMER_KEM\" $(SOURCES90S_KAT) $(INCLUDE) -o PQCgenKAT_TiMER-90s -lcrypto


clean:
	-$(RM) -rf TiMER-main-90s
	-$(RM) -rf TiMER-speed-90s
	-$(RM) -rf PQCgenKAT_TiMER-90s
