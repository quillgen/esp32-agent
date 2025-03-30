#ifndef __LED_H_
#define __LED_H_

#include <cstdint>

void hsv2rgb_fast(uint16_t hue, uint8_t sat, uint8_t val, uint8_t *r, uint8_t *g, uint8_t *b);

int16_t int_sin(uint16_t angle);

#endif