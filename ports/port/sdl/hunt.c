/*
 * hunt.c  A scripted run for comparing moves with an emulator, or a replay to watch: both players' input from a
 * script that waits on the game's memory, and the game's work dumped each game frame (tools/hunt, tools/fcreplay)
 *
 *   MVC2_HUNT=script  MVC2_HUNT_SYMS=name=addr,...  MVC2_HUNT_DUMP=name:len,...  MVC2_HUNT_TRACE=file
 *
 * The script's "sym NAME ADDR" lines name addresses as MVC2_HUNT_SYMS does, before the lines that use them.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "naomi_input.h"
#include "hunt.h"
#include "naomi_host.h"

#define MAX_OPS 400000
#define MAX_SYMS 32

enum { OP_WAIT, OP_WAITNE, OP_UNTIL, OP_FRAMES, OP_HOLD, OP_NOW, OP_COIN, OP_POKE, OP_KEEP, OP_UNKEEP, OP_TRACE, OP_QUIT };

typedef struct {
    int op, size, n, line;
    uint32_t addr, val, keys[2];
} OP;

static OP ops[MAX_OPS];
static int nops, pc, left, logged = -1;
static uint32_t held[2], late[2];   /* late: what the game reads, a frame behind, as an emulator's JVS read is */
static int coin_now, late_coin;
static struct {
    char name[32];
    uint32_t addr;
} syms[MAX_SYMS];
static int nsyms;
static struct {
    uint32_t addr, len;
} dumps[MAX_SYMS];
static int ndumps;
static FILE* trace;
static int tracing, quiet;
static uint32_t timer_addr, last_timer = 0xFFFFFFFF;

static uint32_t sym(const char* name) {
    int i;
    for (i = 0; i < nsyms; i++) {
        if (strcmp(syms[i].name, name) == 0) {
            return syms[i].addr;
        }
    }
    fprintf(stderr, "hunt: no symbol %s\n", name);
    exit(2);
}

/* NAME, NAME+OFF or a number */
static uint32_t addr_of(const char* s) {
    char name[32];
    const char* plus = strchr(s, '+');
    size_t n = plus ? (size_t)(plus - s) : strlen(s);
    if (s[0] >= '0' && s[0] <= '9') {
        return (uint32_t)strtoul(s, NULL, 0);
    }
    if (n >= sizeof name) {
        n = sizeof name - 1;
    }
    memcpy(name, s, n);
    name[n] = 0;
    return sym(name) + (plus ? (uint32_t)strtoul(plus + 1, NULL, 0) : 0);
}

static uint32_t keys_of(const char* s) {
    static const struct {
        const char* name;
        uint32_t bit;
    } keys[] = {
        { "start", JVS_START }, { "up", JVS_UP }, { "down", JVS_DOWN }, { "left", JVS_LEFT }, { "right", JVS_RIGHT },
        { "lp", JVS_B1 }, { "hp", JVS_B2 }, { "a1", JVS_B3 }, { "lk", JVS_B4 }, { "hk", JVS_B5 }, { "a2", JVS_B6 },
    };
    uint32_t bits = 0;
    char buf[128], *t;
    size_t i;
    snprintf(buf, sizeof buf, "%s", s);
    for (t = strtok(buf, "+"); t != NULL; t = strtok(NULL, "+")) {
        for (i = 0; i < sizeof keys / sizeof keys[0]; i++) {
            if (strcmp(t, keys[i].name) == 0) {
                bits |= keys[i].bit;
            }
        }
    }
    return bits;
}

static void load(const char* path) {
    FILE* f = fopen(path, "r");
    char line[256];
    int no = 0;
    if (f == NULL) {
        fprintf(stderr, "hunt: no script %s\n", path);
        exit(2);
    }
    while (fgets(line, sizeof line, f) != NULL && nops < MAX_OPS) {
        char w[6][64];
        int n = sscanf(line, "%63s %63s %63s %63s %63s %63s", w[0], w[1], w[2], w[3], w[4], w[5]);
        OP* o = &ops[nops];
        memset(o, 0, sizeof *o);
        o->line = ++no;
        if (n < 1 || w[0][0] == '#') {
            continue;
        }
        if (strcmp(w[0], "sym") == 0 && n == 3) {
            if (nsyms < MAX_SYMS) {
                snprintf(syms[nsyms].name, sizeof syms[nsyms].name, "%.31s", w[1]);
                syms[nsyms++].addr = (uint32_t)strtoul(w[2], NULL, 0);
            }
            continue;
        }
        if ((strcmp(w[0], "wait") == 0 || strcmp(w[0], "waitne") == 0 || strcmp(w[0], "poke") == 0 ||
             strcmp(w[0], "keep") == 0) && n == 4) {
            /* keep ADDR SIZE VALUE: poked again each frame, till unkeep */
            o->op = w[0][0] == 'p' ? OP_POKE : w[0][0] == 'k' ? OP_KEEP : strcmp(w[0], "wait") == 0 ? OP_WAIT : OP_WAITNE;
            o->addr = addr_of(w[1]);
            o->size = atoi(w[2]);
            o->val = (uint32_t)strtoul(w[3], NULL, 0);
        } else if (strcmp(w[0], "until") == 0 && n >= 5) {
            /* until ADDR SIZE VALUE KEYS1 [KEYS2]: tap the keys every 16 frames until it holds */
            o->op = OP_UNTIL;
            o->addr = addr_of(w[1]);
            o->size = atoi(w[2]);
            o->val = (uint32_t)strtoul(w[3], NULL, 0);
            o->keys[0] = keys_of(w[4]);
            o->keys[1] = n >= 6 ? keys_of(w[5]) : 0;
        } else if (strcmp(w[0], "frames") == 0 && n == 2) {
            o->op = OP_FRAMES;
            o->n = atoi(w[1]);
        } else if (strcmp(w[0], "hold") == 0 && n >= 3) {
            o->op = OP_HOLD;
            o->n = atoi(w[1]);
            o->keys[0] = keys_of(w[2]);
            o->keys[1] = n >= 4 ? keys_of(w[3]) : 0;
        } else if (strcmp(w[0], "now") == 0 && n >= 2) {
            /* now KEYS1 [KEYS2]: the keys the game reads this frame, not the next */
            o->op = OP_NOW;
            o->keys[0] = keys_of(w[1]);
            o->keys[1] = n >= 3 ? keys_of(w[2]) : 0;
        } else if (strcmp(w[0], "unkeep") == 0) {
            o->op = OP_UNKEEP;
        } else if (strcmp(w[0], "coin") == 0) {
            o->op = OP_COIN;
        } else if (strcmp(w[0], "trace") == 0 && n == 2) {
            o->op = OP_TRACE;
            o->n = strcmp(w[1], "on") == 0;
        } else if (strcmp(w[0], "quit") == 0) {
            o->op = OP_QUIT;
        } else {
            fprintf(stderr, "hunt: bad line: %s", line);
            exit(2);
        }
        nops++;
    }
    fclose(f);
}

static OP* kept[64];
static int nkept;

static void poke(const OP* o) {
    if (o->size == 1) {
        *(volatile uint8_t*)(uintptr_t)o->addr = (uint8_t)o->val;
    } else if (o->size == 2) {
        *(volatile uint16_t*)(uintptr_t)o->addr = (uint16_t)o->val;
    } else {
        *(volatile uint32_t*)(uintptr_t)o->addr = o->val;
    }
}

static uint32_t peek(uint32_t a, int size) {
    return size == 1 ? *(volatile uint8_t*)(uintptr_t)a : size == 2 ? *(volatile uint16_t*)(uintptr_t)a
                                                                     : *(volatile uint32_t*)(uintptr_t)a;
}

int hunt_init(int no_log) {
    const char* script = getenv("MVC2_HUNT");
    const char* s;
    if (script == NULL) {
        return 0;
    }
    quiet = no_log;
    for (s = getenv("MVC2_HUNT_SYMS"); s != NULL && *s != 0 && nsyms < MAX_SYMS;) {
        unsigned long a;
        if (sscanf(s, "%31[^=]=%li", syms[nsyms].name, (long*)&a) == 2) {
            syms[nsyms++].addr = (uint32_t)a;
        }
        s = strchr(s, ',');
        s = s ? s + 1 : NULL;
    }
    load(script);
    timer_addr = sym("sys_work") + 0x1C;    /* game_timer0 */
    for (s = getenv("MVC2_HUNT_DUMP"); s != NULL && *s != 0 && ndumps < MAX_SYMS;) {
        char name[64];
        unsigned len;
        if (sscanf(s, "%63[^:]:%i", name, (int*)&len) == 2) {
            dumps[ndumps].addr = addr_of(name);
            dumps[ndumps++].len = len;
        }
        s = strchr(s, ',');
        s = s ? s + 1 : NULL;
    }
    if (getenv("MVC2_HUNT_TRACE") != NULL) {
        trace = fopen(getenv("MVC2_HUNT_TRACE"), "wb");
    }
    return 1;
}

/* once a host frame, before the game's: this frame's input; nonzero to quit */
int hunt_frame(HOST_INPUT* in, unsigned frame) {
    uint32_t timer = peek(timer_addr, 4);
    int i;
    in->coin = 0;
    if (timer == last_timer) {
        /* the game did not move: keep what is held */
        in->player[0] = late[0], in->player[1] = late[1];
        return 0;
    }
    last_timer = timer;
    if (tracing && trace != NULL) {
        fwrite("HUNT", 4, 1, trace);
        fwrite(&frame, 4, 1, trace);
        for (i = 0; i < ndumps; i++) {
            fwrite((void*)(uintptr_t)dumps[i].addr, dumps[i].len, 1, trace);
        }
    }
    for (i = 0; i < nkept; i++) {
        poke(kept[i]);
    }
    held[0] = held[1] = 0;
    coin_now = 0;
    while (pc < nops) {
        OP* o = &ops[pc];
        if (o->op == OP_WAIT || o->op == OP_WAITNE) {
            if ((peek(o->addr, o->size) == o->val) != (o->op == OP_WAIT)) {
                break;
            }
            pc++;
        } else if (o->op == OP_UNTIL) {
            if (peek(o->addr, o->size) == o->val) {
                left = 0;
                pc++;
                continue;
            }
            if (left++ % 16 < 2) {
                held[0] = o->keys[0], held[1] = o->keys[1];
            }
            break;
        } else if (o->op == OP_POKE) {
            poke(o);
            pc++;
        } else if (o->op == OP_KEEP) {
            if (nkept < 64) {
                kept[nkept++] = o;
            }
            poke(o);
            pc++;
        } else if (o->op == OP_UNKEEP) {
            nkept = 0;
            pc++;
        } else if (o->op == OP_NOW) {
            late[0] = o->keys[0], late[1] = o->keys[1];
            pc++;
        } else if (o->op == OP_TRACE) {
            tracing = o->n;
            pc++;
        } else if (o->op == OP_QUIT) {
            return 1;
        } else if (o->op == OP_COIN) {
            coin_now = 1;
            pc++;
            break;
        } else {
            if (left == 0) {
                left = o->n;
            }
            if (o->op == OP_HOLD) {
                held[0] = o->keys[0], held[1] = o->keys[1];
            }
            if (--left == 0) {
                pc++;
            }
            break;
        }
    }
    if (pc != logged && !quiet) {
        logged = pc;
        host_log("hunt: frame %u timer %u line %d", frame, timer, pc < nops ? ops[pc].line : -1);
    }
    in->player[0] = late[0], in->player[1] = late[1];
    in->coin = late_coin;
    late[0] = held[0], late[1] = held[1];
    late_coin = coin_now;
    return pc >= nops;
}
