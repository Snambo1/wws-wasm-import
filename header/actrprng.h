#ifndef ACTRPRNG_H
#define ACTRPRNG_H
#include "actrtime.h"

// this is probably a "bad" idea, some guy on SO said making my own prng was bad so I did it
// extern void prngf(float f);
float actr_prng_max = 4294967296;
unsigned int actr_prng_prime =   2147483629;
unsigned int actr_prng_seed = 419420;

void actr_sprng(unsigned int seed) {
    actr_prng_seed = seed;
}
unsigned int actr_prng() {
    actr_prng_seed *= actr_prng_prime;
    return actr_prng_seed;
}

float actr_prngf() {
    actr_prng_seed *= actr_prng_prime;
    float result = actr_prng_seed / actr_prng_max;
    // prngf(result);
    return result;
}

#endif