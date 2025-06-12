#ifndef OOGABOOGA_CPU_H
#define OOGABOOGA_CPU_H

#include <stdint.h>
#include <stdbool.h>

typedef struct Cpu_Info_X86 {
    uint32_t eax;
    uint32_t ebx;
    uint32_t ecx;
    uint32_t edx;
} Cpu_Info_X86;

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

// Core functions
uint64_t rdtsc(void);
Cpu_Info_X86 cpuid(uint32_t function_id);
Cpu_Capabilities query_cpu_capabilities(void);
void crash(void) __attribute__((noreturn));

// Atomic operations
bool compare_and_swap_8(volatile uint8_t *a, uint8_t b, uint8_t old);
bool compare_and_swap_16(volatile uint16_t *a, uint16_t b, uint16_t old);
bool compare_and_swap_32(volatile uint32_t *a, uint32_t b, uint32_t old);
bool compare_and_swap_64(volatile uint64_t *a, uint64_t b, uint64_t old);
bool compare_and_swap_bool(volatile bool *a, bool b, bool old);

#endif
