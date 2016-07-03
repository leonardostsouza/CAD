#include <math.h>

// useful definitions
#define RADIUS	0.5

// return number of cicles since start
int rdtsc() 
{ 
    __asm__ __volatile__ ("rdtsc"); 
}
