INCDIR := include
SRCDIR := src
MAINDIR := test
BENCHDIR := benchmark
KATDIR := kat

# CC = clang
CC ?= /usr/bin/cc
RM = /bin/rm


CFLAGS = -mtune=native -O3 -fomit-frame-pointer
DFLAGS = -g3 -DDEBUG

SOURCES = $(SRCDIR)/ciphertext.c $(SRCDIR)/dg.c $(SRCDIR)/hash.c $(SRCDIR)/hwt.c $(SRCDIR)/indcpa.c $(SRCDIR)/io.c $(SRCDIR)/kem.c $(SRCDIR)/key.c $(SRCDIR)/pack.c $(SRCDIR)/poly.c $(SRCDIR)/verify.c $(SRCDIR)/toomcook.c $(SRCDIR)/cbd.c
SOURCESKECCAK = $(SOURCES) $(SRCDIR)/fips202.c

HEADERS = $(INCDIR)/ciphertext.h $(INCDIR)/dg.h $(INCDIR)/hash.h $(INCDIR)/hwt.h $(INCDIR)/indcpa.h $(INCDIR)/io.h $(INCDIR)/kem.h $(INCDIR)/key.h $(INCDIR)/pack.h $(INCDIR)/poly.h $(INCDIR)/verify.h $(INCDIR)/toomcook.h $(INCDIR)/cbd.c
HEADERSKECCAK = $(HEADERS) $(INCDIR)/fips202.h
INCLUDE = -I./$(INCDIR)

SOURCES_MAIN = $(SOURCESKECCAK) $(MAINDIR)/main.c $(SRCDIR)/randombytes.c
SOURCES_KAT = $(SOURCESKECCAK) $(KATDIR)/kat_kem.c $(KATDIR)/PQCgenKAT_kem.c $(SRCDIR)/rng.c
SOURCES_SPEED = $(SOURCESKECCAK) $(BENCHDIR)/PQC_bench.c $(SRCDIR)/randombytes.c
#SOURCES_SPEED = $(SOURCESKECCAK) $(BENCHDIR)/speed.c $(BENCHDIR)/cpucycles.c $(BENCHDIR)/speed_print.c $(SRCDIR)/randombytes.c

all: \
	main \
	speed \

shared: \
	smaug-t1.so \
	smaug-t3.so \
	smaug-t5.so \

main: \
	smaug-t1-main \
	smaug-t3-main \
	smaug-t5-main \

speed: \
	smaug-t1-speed \
	smaug-t3-speed \
	smaug-t5-speed \

kat: \
	PQCgenKAT_smaug-t1 \
	PQCgenKAT_smaug-t3 \
	PQCgenKAT_smaug-t5 \


smaug-t1.so: $(SOURCESKECCAK) $(HEADERSKECCAK)
	$(CC) -shared -fPIC $(CFLAGS) -DSMAUG_MODE=1 $(SOURCESKECCAK) $(INCLUDE) -o smaug-t1.so

smaug-t3.so: $(SOURCESKECCAK) $(HEADERSKECCAK)
	$(CC) -shared -fPIC $(CFLAGS) -DSMAUG_MODE=3 $(SOURCESKECCAK) $(INCLUDE) -o smaug-t3.so

smaug-t5.so: $(SOURCESKECCAK) $(HEADERSKECCAK)
	$(CC) -shared -fPIC $(CFLAGS) -DSMAUG_MODE=5 $(SOURCESKECCAK) $(INCLUDE) -o smaug-t5.so


smaug-t1-main: $(SOURCES_MAIN)
	$(CC) $(CFLAGS) -DSMAUG_MODE=1 $(SOURCES_MAIN) $(INCLUDE) -o smaug-t1-main

smaug-t3-main: $(SOURCES_MAIN)
	$(CC) $(CFLAGS) -DSMAUG_MODE=3 $(SOURCES_MAIN) $(INCLUDE) -o smaug-t3-main

smaug-t5-main: $(SOURCES_MAIN)
	$(CC) $(CFLAGS) -DSMAUG_MODE=5 $(SOURCES_MAIN) $(INCLUDE) -o smaug-t5-main


smaug-t1-speed: $(SOURCES_SPEED)
	$(CC) $(CFLAGS) -DSMAUG_MODE=1 $(SOURCES_SPEED) $(INCLUDE) -o smaug-t1-speed

smaug-t3-speed: $(SOURCES_SPEED)
	$(CC) $(CFLAGS) -DSMAUG_MODE=3 $(SOURCES_SPEED) $(INCLUDE) -o smaug-t3-speed

smaug-t5-speed: $(SOURCES_SPEED)
	$(CC) $(CFLAGS) -DSMAUG_MODE=5 $(SOURCES_SPEED) $(INCLUDE) -o smaug-t5-speed


PQCgenKAT_smaug-t1: $(SOURCES_KAT)
	$(CC) $(CFLAGS) -DSMAUG_MODE=1 -DCRYPTO_ALGNAME=\"SMAUG-T1_KEM\" $(SOURCES_KAT) $(INCLUDE) -o PQCgenKAT_smaug-t1 -lcrypto

PQCgenKAT_smaug-t3: $(SOURCES_KAT)
	$(CC) $(CFLAGS) -DSMAUG_MODE=3 -DCRYPTO_ALGNAME=\"SMAUG-T3_KEM\" $(SOURCES_KAT) $(INCLUDE) -o PQCgenKAT_smaug-t3 -lcrypto

PQCgenKAT_smaug-t5: $(SOURCES_KAT)
	$(CC) $(CFLAGS) -DSMAUG_MODE=5 -DCRYPTO_ALGNAME=\"SMAUG-T5_KEM\" $(SOURCES_KAT) $(INCLUDE) -o PQCgenKAT_smaug-t5 -lcrypto



clean:
	-$(RM) -rf smaug-t1-main
	-$(RM) -rf smaug-t3-main
	-$(RM) -rf smaug-t5-main
	-$(RM) -rf smaug-t1-speed
	-$(RM) -rf smaug-t3-speed
	-$(RM) -rf smaug-t5-speed
	-$(RM) -rf PQCgenKAT_smaug-t1
	-$(RM) -rf PQCgenKAT_smaug-t3
	-$(RM) -rf PQCgenKAT_smaug-t5
