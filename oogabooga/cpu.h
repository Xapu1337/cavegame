#ifndef OOGABOOGA_CPU_H
#define OOGABOOGA_CPU_H

#include <stdint.h>
#include <stdbool.h>

typedef struct Cpu_Capabilities {
    bool sse1;
    bool sse2;
    bool sse3;
    bool ssse3;
    bool sse41;
    bool sse42;
    bool any_sse;
    bool avx;
    bool avx2;
    bool avx512;
} Cpu_Capabilities;

uint64_t rdtsc(void);
Cpu_Capabilities query_cpu_capabilities(void);

#endif
