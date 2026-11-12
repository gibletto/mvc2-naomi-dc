/*
 * divzero_arm.c  Division by zero gives 0 quotient and remainder, as SHC's code did: the AEABI helpers,
 * wrapped by the linker (-Wl,--wrap)
 */

#include <stdint.h>

void host_divzero_install(void) {
}

int __real___aeabi_idiv(int a, int b);
unsigned __real___aeabi_uidiv(unsigned a, unsigned b);
uint64_t __real___aeabi_idivmod(int a, int b);
uint64_t __real___aeabi_uidivmod(unsigned a, unsigned b);

int __wrap___aeabi_idiv(int a, int b) {
    if (b == 0) {
        return 0;
    }
    return __real___aeabi_idiv(a, b);
}

unsigned __wrap___aeabi_uidiv(unsigned a, unsigned b) {
    if (b == 0) {
        return 0;
    }
    return __real___aeabi_uidiv(a, b);
}

uint64_t __wrap___aeabi_idivmod(int a, int b) {
    if (b == 0) {
        return 0;
    }
    return __real___aeabi_idivmod(a, b);
}

uint64_t __wrap___aeabi_uidivmod(unsigned a, unsigned b) {
    if (b == 0) {
        return 0;
    }
    return __real___aeabi_uidivmod(a, b);
}
