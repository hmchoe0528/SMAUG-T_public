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

SOURCES = $(SRCDIR)/ciphertext.c $(SRCDIR)/dg.c $(SRCDIR)/hash.c $(SRCDIR)/hwt.c $(SRCDIR)/indcpa.c $(SRCDIR)/io.c $(SRCDIR)/kem.c $(SRCDIR)/key.c $(SRCDIR)/pack.c $(SRCDIR)/poly.c $(SRCDIR)/verify.c
SOURCESKECCAK = $(SOURCES) $(SRCDIR)/fips202.c

HEADERS = $(INCDIR)/ciphertext.h $(INCDIR)/dg.h $(INCDIR)/hash.h $(INCDIR)/hwt.h $(INCDIR)/indcpa.h $(INCDIR)/io.h $(INCDIR)/kem.h $(INCDIR)/key.h $(INCDIR)/pack.h $(INCDIR)/poly.h $(INCDIR)/verify.h
HEADERSKECCAK = $(HEADERS) $(INCDIR)/fips202.h
INCLUDE = -I./$(INCDIR)

SOURCES_MAIN = $(SOURCESKECCAK) $(MAINDIR)/main.c $(SRCDIR)/randombytes.c
SOURCES_SPEED = $(SOURCESKECCAK) $(BENCHDIR)/speed.c $(BENCHDIR)/cpucycles.c $(BENCHDIR)/speed_print.c $(SRCDIR)/randombytes.c
SOURCES_KAT = $(SOURCESKECCAK) $(KATDIR)/kat_kem.c $(KATDIR)/PQCgenKAT_kem.c $(SRCDIR)/rng.c

all: \
	main

shared: \
	smaug1.so \
	smaug3.so \
	smaug5.so \

main: \
	smaug1-main \
	smaug3-main \
	smaug5-main \

speed: \
	smaug1-speed \
	smaug3-speed \
	smaug5-speed \

kat: \
	PQCgenKAT_smaug1 \
	PQCgenKAT_smaug3 \
	PQCgenKAT_smaug5 \


smaug1.so: $(SOURCESKECCAK) $(HEADERSKECCAK)
	$(CC) -shared -fPIC $(CFLAGS) -DSMAUG_MODE=1 $(SOURCESKECCAK) $(INCLUDE) -o smaug1.so

smaug3.so: $(SOURCESKECCAK) $(HEADERSKECCAK)
	$(CC) -shared -fPIC $(CFLAGS) -DSMAUG_MODE=3 $(SOURCESKECCAK) $(INCLUDE) -o smaug3.so

smaug5.so: $(SOURCESKECCAK) $(HEADERSKECCAK)
	$(CC) -shared -fPIC $(CFLAGS) -DSMAUG_MODE=5 $(SOURCESKECCAK) $(INCLUDE) -o smaug5.so


smaug1-main: $(SOURCES_MAIN)
	$(CC) $(CFLAGS) -DSMAUG_MODE=1 $(SOURCES_MAIN) $(INCLUDE) -o smaug1-main

smaug3-main: $(SOURCES_MAIN)
	$(CC) $(CFLAGS) -DSMAUG_MODE=3 $(SOURCES_MAIN) $(INCLUDE) -o smaug3-main

smaug5-main: $(SOURCES_MAIN)
	$(CC) $(CFLAGS) -DSMAUG_MODE=5 $(SOURCES_MAIN) $(INCLUDE) -o smaug5-main


smaug1-speed: $(SOURCES_SPEED)
	$(CC) $(CFLAGS) -DSMAUG_MODE=1 $(SOURCES_SPEED) $(INCLUDE) -o smaug1-speed

smaug3-speed: $(SOURCES_SPEED)
	$(CC) $(CFLAGS) -DSMAUG_MODE=3 $(SOURCES_SPEED) $(INCLUDE) -o smaug3-speed

smaug5-speed: $(SOURCES_SPEED)
	$(CC) $(CFLAGS) -DSMAUG_MODE=5 $(SOURCES_SPEED) $(INCLUDE) -o smaug5-speed


PQCgenKAT_smaug1: $(SOURCES_KAT)
	$(CC) $(CFLAGS) -DSMAUG_MODE=1 -DCRYPTO_ALGNAME=\"SMAUG1_KEM\" $(SOURCES_KAT) $(INCLUDE) -o PQCgenKAT_smaug1 -lcrypto

PQCgenKAT_smaug3: $(SOURCES_KAT)
	$(CC) $(CFLAGS) -DSMAUG_MODE=3 -DCRYPTO_ALGNAME=\"SMAUG3_KEM\" $(SOURCES_KAT) $(INCLUDE) -o PQCgenKAT_smaug3 -lcrypto

PQCgenKAT_smaug5: $(SOURCES_KAT)
	$(CC) $(CFLAGS) -DSMAUG_MODE=5 -DCRYPTO_ALGNAME=\"SMAUG5_KEM\" $(SOURCES_KAT) $(INCLUDE) -o PQCgenKAT_smaug5 -lcrypto



clean:
	-$(RM) -rf smaug1-main
	-$(RM) -rf smaug3-main
	-$(RM) -rf smaug5-main
	-$(RM) -rf smaug1-speed
	-$(RM) -rf smaug3-speed
	-$(RM) -rf smaug5-speed
	-$(RM) -rf PQCgenKAT_smaug1
	-$(RM) -rf PQCgenKAT_smaug3
	-$(RM) -rf PQCgenKAT_smaug5
