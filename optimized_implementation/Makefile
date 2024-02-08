INCDIR := include
SRCDIR := src
MAINDIR := test
BENCHDIR := benchmark
KATDIR := kat

# CC = clang
CC ?= /usr/bin/cc
RM = /bin/rm


CFLAGS = -march=native -mtune=native -O3 -fomit-frame-pointer -mavx2 -mbmi2 -mpopcnt
CFLAGS90S = $(CFLAGS) -maes
DFLAGS = -g3 -DDEBUG

SOURCES = $(SRCDIR)/ciphertext.c $(SRCDIR)/dg.c $(SRCDIR)/hwt.c $(SRCDIR)/indcpa.c $(SRCDIR)/io.c $(SRCDIR)/kem.c $(SRCDIR)/key.c $(SRCDIR)/pack.c $(SRCDIR)/poly.c $(SRCDIR)/verify.c
SOURCESKECCAK = $(SOURCES) $(SRCDIR)/hash.c $(SRCDIR)/fips202x4.c $(SRCDIR)/fips202.c $(SRCDIR)/keccak/KeccakP-1600-times4-SIMD256.c $(SRCDIR)/keccak/KeccakP-1600-AVX2.S
SOURCES90S = $(SOURCES) $(SRCDIR)/symmetric.c $(SRCDIR)/aes256ctr.c

HEADERS = $(INCDIR)/ciphertext.h $(INCDIR)/dg.h $(INCDIR)/hwt.h $(INCDIR)/indcpa.h $(INCDIR)/io.h $(INCDIR)/kem.h $(INCDIR)/key.h $(INCDIR)/pack.h $(INCDIR)/poly.h $(INCDIR)/verify.h $(INCDIR)/symmetric.h
HEADERSKECCAK = $(HEADERS) $(INCDIR)/hash.h $(INCDIR)/fips202x4.h $(INCDIR)/fips202.h
HEADERS90S = $(HEADERS) $(INCDIR)/sha2.h $(INCDIR)/aes256ctr.h
INCLUDE = -I./$(INCDIR) -I./$(INCDIR)/keccak

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
	smaug1.so \
	smaug3.so \
	smaug5.so \
	smaug1-90s.so \
	smaug3-90s.so \
	smaug5-90s.so \

main: \
	smaug1-main \
	smaug3-main \
	smaug5-main \
	smaug1-main-90s \
	smaug3-main-90s \
	smaug5-main-90s \

speed: \
	smaug1-speed \
	smaug3-speed \
	smaug5-speed \
	smaug1-speed-90s \
	smaug3-speed-90s \
	smaug5-speed-90s \

kat: \
	PQCgenKAT_smaug1 \
	PQCgenKAT_smaug3 \
	PQCgenKAT_smaug5 \
	PQCgenKAT_smaug1-90s \
	PQCgenKAT_smaug3-90s \
	PQCgenKAT_smaug5-90s \


smaug1.so: $(SOURCESKECCAK) $(HEADERSKECCAK)
	$(CC) -shared -fPIC $(CFLAGS) -DSMAUG_MODE=1 $(SOURCESKECCAK) $(INCLUDE) -o smaug1.so
smaug3.so: $(SOURCESKECCAK) $(HEADERSKECCAK)
	$(CC) -shared -fPIC $(CFLAGS) -DSMAUG_MODE=3 $(SOURCESKECCAK) $(INCLUDE) -o smaug3.so
smaug5.so: $(SOURCESKECCAK) $(HEADERSKECCAK)
	$(CC) -shared -fPIC $(CFLAGS) -DSMAUG_MODE=5 $(SOURCESKECCAK) $(INCLUDE) -o smaug5.so

smaug1-90s.so: $(SOURCES90S) $(HEADERS90S)
	$(CC) -shared -fPIC $(CFLAGS90S) -DSMAUG_MODE=1 -D SYMMETRIC_90S $(SOURCES90S) $(INCLUDE) -o smaug1-90s.so -lcrypto
smaug3-90s.so: $(SOURCES90S) $(HEADERS90S)
	$(CC) -shared -fPIC $(CFLAGS90S) -DSMAUG_MODE=3 -D SYMMETRIC_90S $(SOURCES90S) $(INCLUDE) -o smaug3-90s.so -lcrypto
smaug5-90s.so: $(SOURCES90S) $(HEADERS90S)
	$(CC) -shared -fPIC $(CFLAGS90S) -DSMAUG_MODE=5 -D SYMMETRIC_90S $(SOURCES90S) $(INCLUDE) -o smaug5-90s.so -lcrypto


smaug1-main: $(SOURCES_MAIN)
	$(CC) $(CFLAGS) -DSMAUG_MODE=1 $(SOURCES_MAIN) $(INCLUDE) -o smaug1-main
smaug3-main: $(SOURCES_MAIN)
	$(CC) $(CFLAGS) -DSMAUG_MODE=3 $(SOURCES_MAIN) $(INCLUDE) -o smaug3-main
smaug5-main: $(SOURCES_MAIN)
	$(CC) $(CFLAGS) -DSMAUG_MODE=5 $(SOURCES_MAIN) $(INCLUDE) -o smaug5-main

smaug1-main-90s: $(SOURCES90S_MAIN)
	$(CC) $(CFLAGS90S) -DSMAUG_MODE=1 -D SYMMETRIC_90S $(SOURCES90S_MAIN) $(INCLUDE) -o smaug1-main-90s -lcrypto
smaug3-main-90s: $(SOURCES90S_MAIN)
	$(CC) $(CFLAGS90S) -DSMAUG_MODE=3 -D SYMMETRIC_90S $(SOURCES90S_MAIN) $(INCLUDE) -o smaug3-main-90s -lcrypto
smaug5-main-90s: $(SOURCES90S_MAIN)
	$(CC) $(CFLAGS90S) -DSMAUG_MODE=5 -D SYMMETRIC_90S $(SOURCES90S_MAIN) $(INCLUDE) -o smaug5-main-90s -lcrypto


smaug1-speed: $(SOURCES_SPEED)
	$(CC) $(CFLAGS) -DSMAUG_MODE=1 $(SOURCES_SPEED) $(INCLUDE) -o smaug1-speed
smaug3-speed: $(SOURCES_SPEED)
	$(CC) $(CFLAGS) -DSMAUG_MODE=3 $(SOURCES_SPEED) $(INCLUDE) -o smaug3-speed
smaug5-speed: $(SOURCES_SPEED)
	$(CC) $(CFLAGS) -DSMAUG_MODE=5 $(SOURCES_SPEED) $(INCLUDE) -o smaug5-speed

smaug1-speed-90s: $(SOURCES90S_SPEED)
	$(CC) $(CFLAGS90S) -DSMAUG_MODE=1 -D SYMMETRIC_90S $(SOURCES90S_SPEED) $(INCLUDE) -o smaug1-speed-90s -lcrypto
smaug3-speed-90s: $(SOURCES90S_SPEED)
	$(CC) $(CFLAGS90S) -DSMAUG_MODE=3 -D SYMMETRIC_90S $(SOURCES90S_SPEED) $(INCLUDE) -o smaug3-speed-90s -lcrypto
smaug5-speed-90s: $(SOURCES90S_SPEED)
	$(CC) $(CFLAGS90S) -DSMAUG_MODE=5 -D SYMMETRIC_90S $(SOURCES90S_SPEED) $(INCLUDE) -o smaug5-speed-90s -lcrypto


PQCgenKAT_smaug1: $(SOURCES_KAT)
	$(CC) $(CFLAGS) -DSMAUG_MODE=1 -DCRYPTO_ALGNAME=\"SMAUG1_KEM\" $(SOURCES_KAT) $(INCLUDE) -o PQCgenKAT_smaug1 -lcrypto
PQCgenKAT_smaug3: $(SOURCES_KAT)
	$(CC) $(CFLAGS) -DSMAUG_MODE=3 -DCRYPTO_ALGNAME=\"SMAUG3_KEM\" $(SOURCES_KAT) $(INCLUDE) -o PQCgenKAT_smaug3 -lcrypto
PQCgenKAT_smaug5: $(SOURCES_KAT)
	$(CC) $(CFLAGS) -DSMAUG_MODE=5 -DCRYPTO_ALGNAME=\"SMAUG5_KEM\" $(SOURCES_KAT) $(INCLUDE) -o PQCgenKAT_smaug5 -lcrypto

PQCgenKAT_smaug1-90s: $(SOURCES90S_KAT)
	$(CC) $(CFLAGS90S) -DSMAUG_MODE=1 -D SYMMETRIC_90S -DCRYPTO_ALGNAME=\"SMAUG1_KEM\" $(SOURCES90S_KAT) $(INCLUDE) -o PQCgenKAT_smaug1-90s -lcrypto
PQCgenKAT_smaug3-90s: $(SOURCES90S_KAT)
	$(CC) $(CFLAGS90S) -DSMAUG_MODE=3 -D SYMMETRIC_90S -DCRYPTO_ALGNAME=\"SMAUG3_KEM\" $(SOURCES90S_KAT) $(INCLUDE) -o PQCgenKAT_smaug3-90s -lcrypto
PQCgenKAT_smaug5-90s: $(SOURCES90S_KAT)
	$(CC) $(CFLAGS90S) -DSMAUG_MODE=5 -D SYMMETRIC_90S -DCRYPTO_ALGNAME=\"SMAUG5_KEM\" $(SOURCES90S_KAT) $(INCLUDE) -o PQCgenKAT_smaug5-90s -lcrypto



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
	-$(RM) -rf smaug1-main-90s
	-$(RM) -rf smaug3-main-90s
	-$(RM) -rf smaug5-main-90s
	-$(RM) -rf smaug1-speed-90s
	-$(RM) -rf smaug3-speed-90s
	-$(RM) -rf smaug5-speed-90s
	-$(RM) -rf PQCgenKAT_smaug1-90s
	-$(RM) -rf PQCgenKAT_smaug3-90s
	-$(RM) -rf PQCgenKAT_smaug5-90s
