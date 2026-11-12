/*
 * coro_arm.c  Coroutines for 32-bit ARM: coro_swap saves r4-r11, lr and d8-d15 and swaps stacks
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "coro.h"

#define STACK_FILL 0xA5A5A5A5u

struct coro {
    void* sp;
    void* stack;
    unsigned stack_size;
    void (*fn)(void*);
    void* arg;
};

static coro main_coro;
static coro* current;

#if defined(__ARM_PCS_VFP)
#define SAVE_FP "vpush {d8-d15}\n"
#define LOAD_FP "vpop {d8-d15}\n"
#define FP_BYTES 64
#else
#define SAVE_FP ""
#define LOAD_FP ""
#define FP_BYTES 0
#endif

__attribute__((naked, target("arm"))) static void coro_swap(void** save_sp, void* load_sp) {
    __asm__ volatile(
        "push {r4-r11, lr}\n"
        SAVE_FP
        "str sp, [r0]\n"
        "mov sp, r1\n"
        LOAD_FP
        "pop {r4-r11, pc}\n");
}

static void coro_start(void) {
    current->fn(current->arg);
    abort();                                            /* a coroutine never returns */
}

coro* coro_self(void) {
    if (current == NULL) {
        current = &main_coro;
    }
    return current;
}

/* as if it had switched away at the start of coro_start */
static void start_on_stack(coro* c, void (*fn)(void*), void* arg) {
    uint32_t* w = c->stack;
    uint32_t* top;
    unsigned i;
    c->fn = fn;
    c->arg = arg;
    for (i = 0; i < c->stack_size / 4; i++) {
        w[i] = STACK_FILL;
    }
    top = (uint32_t*)(((uintptr_t)c->stack + c->stack_size) & ~(uintptr_t)7);
    /* what coro_swap pops: d8-d15, r4-r11, pc; sp stays 8-byte aligned */
    top -= 1;
    *top = (uint32_t)(uintptr_t)coro_start;
    top -= 8;
    memset(top, 0, 8 * 4);
    top -= FP_BYTES / 4;
    memset(top, 0, FP_BYTES);
    c->sp = top;
}

coro* coro_create(void (*fn)(void*), void* arg, unsigned stack_size) {
    coro* c = calloc(1, sizeof(coro));
    coro_self();
    if (c != NULL) {
        c->stack = malloc(stack_size);
    }
    if (c == NULL || c->stack == NULL) {
        fprintf(stderr, "coro_create: no memory for a %u byte stack\n", stack_size);
        abort();
    }
    c->stack_size = stack_size;
    start_on_stack(c, fn, arg);
    return c;
}

void coro_restart(coro* c, void (*fn)(void*), void* arg) {
    if (c != current && c != &main_coro) {
        start_on_stack(c, fn, arg);
    }
}

void coro_delete(coro* c) {
    if (c != NULL && c != current && c != &main_coro) {
        free(c->stack);
        free(c);
    }
}

void coro_switch(coro* to) {
    coro* from = coro_self();
    if (to != from) {
        current = to;
        coro_swap(&from->sp, to->sp);
    }
}
