#include <stdint.h>

// useful definitions
#define RADIUS	0.5

// return number of cicles since start
uint64_t rdtsc(){
    unsigned int lo,hi;
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    return ((uint64_t)hi << 32) | lo;
}