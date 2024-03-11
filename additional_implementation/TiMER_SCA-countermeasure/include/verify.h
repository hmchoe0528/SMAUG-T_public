#ifndef TIMER_VERIFY_H
#define TIMER_VERIFY_H

#include <stddef.h>
#include <stdint.h>

#include "parameters.h"

#define verify TIMER_NAMESPACE(verify)
int verify(const uint8_t *a, const uint8_t *b, size_t len);
#define cmov TIMER_NAMESPACE(cmov)
void cmov(uint8_t *r, const uint8_t *x, size_t len, uint8_t b);

#endif
