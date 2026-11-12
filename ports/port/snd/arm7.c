/*
 * arm7.c  The AICA's sound CPU, an ARM7DI (ARMv4, no Thumb), interpreted; the AICA's interrupt is its FIQ.
 * Cycles are counted roughly: one an instruction, more for memory and branches.
 */

#include <string.h>

#include "arm7.h"

enum { M_USR = 0x10, M_FIQ = 0x11, M_IRQ = 0x12, M_SVC = 0x13, M_ABT = 0x17, M_UND = 0x1B, M_SYS = 0x1F };
#define F_N 0x80000000u
#define F_Z 0x40000000u
#define F_C 0x20000000u
#define F_V 0x10000000u
#define F_I 0x80u
#define F_F 0x40u

static uint32_t r[16];
static uint32_t cpsr;
static uint32_t spsr;               /* the current mode's */
static uint32_t bank_r8[2][5];      /* r8-r12: [0] every mode but FIQ, [1] FIQ */
static uint32_t bank_r13[6], bank_r14[6], bank_spsr[6];   /* usr/sys, fiq, irq, svc, abt, und */
static int fiq_line;
static uint64_t cycles_run;
static uint16_t cond_ok[16];        /* for each condition, a bit for each NZCV value it passes */
static int run_left;                /* arm7_stop: the run ends after the instruction */
static int fiq_run;                 /* arm7_run_fiq: the run ends when the mode leaves FIQ */
static int event;                   /* the run must look: a stop, or an FIQ it can take */

/* built into arm7_run, so that its state stays in registers */
#define INLINE static inline __attribute__((always_inline))

static int bank_of(uint32_t mode) {
    switch (mode & 0x1F) {
    case M_FIQ: return 1;
    case M_IRQ: return 2;
    case M_SVC: return 3;
    case M_ABT: return 4;
    case M_UND: return 5;
    default: return 0;
    }
}

/* a new CPSR: the banked registers follow its mode */
static void set_cpsr(uint32_t v) {
    int from = bank_of(cpsr), to = bank_of(v);
    if (from != to) {
        bank_r13[from] = r[13];
        bank_r14[from] = r[14];
        bank_spsr[from] = spsr;
        if ((from == 1) != (to == 1)) {
            memcpy(bank_r8[from == 1], &r[8], sizeof bank_r8[0]);
            memcpy(&r[8], bank_r8[to == 1], sizeof bank_r8[0]);
        }
        r[13] = bank_r13[to];
        r[14] = bank_r14[to];
        spsr = bank_spsr[to];
    }
    if (fiq_run && (v & 0x1F) != M_FIQ) {
        run_left = 0;
        event = 1;
    }
    if (fiq_line && !(v & F_F)) {
        event = 1;
    }
    cpsr = v;
}

static void exception(uint32_t mode, uint32_t vector, uint32_t ret, uint32_t mask) {
    uint32_t old = cpsr;
    set_cpsr((cpsr & ~0x1Fu) | mode | mask);
    spsr = old;
    r[14] = ret;
    r[15] = vector;
}

void arm7_reset(void) {
    int c, f;
    memset(r, 0, sizeof r);
    memset(bank_r8, 0, sizeof bank_r8);
    memset(bank_r13, 0, sizeof bank_r13);
    memset(bank_r14, 0, sizeof bank_r14);
    memset(bank_spsr, 0, sizeof bank_spsr);
    spsr = 0;
    cpsr = M_SVC | F_I | F_F;
    fiq_line = 0;
    for (c = 0; c < 16; c++) {
        cond_ok[c] = 0;
        for (f = 0; f < 16; f++) {
            int n = f >> 3 & 1, z = f >> 2 & 1, cy = f >> 1 & 1, v = f & 1, ok;
            switch (c) {
            case 0: ok = z; break;
            case 1: ok = !z; break;
            case 2: ok = cy; break;
            case 3: ok = !cy; break;
            case 4: ok = n; break;
            case 5: ok = !n; break;
            case 6: ok = v; break;
            case 7: ok = !v; break;
            case 8: ok = cy && !z; break;
            case 9: ok = !cy || z; break;
            case 10: ok = n == v; break;
            case 11: ok = n != v; break;
            case 12: ok = !z && n == v; break;
            case 13: ok = z || n != v; break;
            case 14: ok = 1; break;
            default: ok = 0; break;
            }
            cond_ok[c] |= (uint16_t)(ok << f);
        }
    }
}

void arm7_set_fiq(int on) {
    fiq_line = on;
    if (on && !(cpsr & F_F)) {
        event = 1;
    }
}

uint32_t arm7_pc(void) {
    return r[15];
}

uint64_t arm7_cycles(void) {
    return cycles_run;
}

/* ---- memory: the RAM directly, the rest through the AICA ---- */

static inline uint32_t rd32(uint32_t a) {
    uint32_t v;
    if ((a & 0xFF800000u) == 0) {
        memcpy(&v, aica_ram + (a & aica_ram_mask & ~3u), 4);
        return v;
    }
    return aica_arm_read32(a & ~3u);
}

static inline uint32_t rd8(uint32_t a) {
    if ((a & 0xFF800000u) == 0) {
        return aica_ram[a & aica_ram_mask];
    }
    return aica_arm_read8(a);
}

static inline uint32_t rd16(uint32_t a) {
    uint16_t v;
    if ((a & 0xFF800000u) == 0) {
        memcpy(&v, aica_ram + (a & aica_ram_mask & ~1u), 2);
        return v;
    }
    return aica_arm_read16(a & ~1u);
}

static inline void wr32(uint32_t a, uint32_t v) {
    if ((a & 0xFF800000u) == 0) {
        memcpy(aica_ram + (a & aica_ram_mask & ~3u), &v, 4);
        return;
    }
    aica_arm_write32(a & ~3u, v);
}

static inline void wr16(uint32_t a, uint32_t v) {
    uint16_t h = (uint16_t)v;
    if ((a & 0xFF800000u) == 0) {
        memcpy(aica_ram + (a & aica_ram_mask & ~1u), &h, 2);
        return;
    }
    aica_arm_write16(a & ~1u, h);
}

static inline void wr8(uint32_t a, uint32_t v) {
    if ((a & 0xFF800000u) == 0) {
        aica_ram[a & aica_ram_mask] = (uint8_t)v;
        return;
    }
    aica_arm_write8(a, v & 0xFF);
}

/* a word load from an address not on a word: the word turned so that the addressed byte is lowest */
static inline uint32_t ldr_word(uint32_t a) {
    uint32_t v = rd32(a), s = (a & 3) * 8;
    return s ? v >> s | v << (32 - s) : v;
}

/* ---- the barrel shifter ---- */

/* operand 2 of a data processing instruction; *carry gets the shifter's carry out */
static inline uint32_t shifter(uint32_t op, uint32_t* carry) {
    uint32_t c = cpsr >> 29 & 1;
    if (op & (1u << 25)) {
        uint32_t rot = (op >> 7) & 0x1E, imm = op & 0xFF;
        if (rot) {
            imm = imm >> rot | imm << (32 - rot);
            c = imm >> 31;
        }
        *carry = c;
        return imm;
    } else {
        uint32_t rm = r[op & 15], type = (op >> 5) & 3, amt;
        if (op & 0x10) {
            amt = r[(op >> 8) & 15] & 0xFF;
            if ((op & 15) == 15) {
                rm += 4;            /* the PC reads 12 ahead with a register shift */
            }
            if (amt == 0) {
                *carry = c;
                return rm;
            }
            switch (type) {
            case 0:
                if (amt < 32) {
                    *carry = rm >> (32 - amt) & 1;
                    return rm << amt;
                }
                *carry = amt == 32 ? rm & 1 : 0;
                return 0;
            case 1:
                if (amt < 32) {
                    *carry = rm >> (amt - 1) & 1;
                    return rm >> amt;
                }
                *carry = amt == 32 ? rm >> 31 : 0;
                return 0;
            case 2:
                if (amt < 32) {
                    *carry = (uint32_t)((int32_t)rm >> (amt - 1)) & 1;
                    return (uint32_t)((int32_t)rm >> amt);
                }
                *carry = rm >> 31;
                return (uint32_t)((int32_t)rm >> 31);
            default:
                amt &= 31;
                if (amt == 0) {
                    *carry = rm >> 31;
                    return rm;
                }
                *carry = rm >> (amt - 1) & 1;
                return rm >> amt | rm << (32 - amt);
            }
        }
        amt = (op >> 7) & 31;
        switch (type) {
        case 0:
            if (amt) {
                c = rm >> (32 - amt) & 1;
                rm <<= amt;
            }
            *carry = c;
            return rm;
        case 1:
            if (amt == 0) {
                *carry = rm >> 31;
                return 0;
            }
            *carry = rm >> (amt - 1) & 1;
            return rm >> amt;
        case 2:
            if (amt == 0) {
                *carry = rm >> 31;
                return (uint32_t)((int32_t)rm >> 31);
            }
            *carry = (uint32_t)((int32_t)rm >> (amt - 1)) & 1;
            return (uint32_t)((int32_t)rm >> amt);
        default:
            if (amt == 0) {         /* RRX */
                *carry = rm & 1;
                return c << 31 | rm >> 1;
            }
            *carry = rm >> (amt - 1) & 1;
            return rm >> amt | rm << (32 - amt);
        }
    }
}

/* the offset of a single data transfer with a register: an immediate shift only */
static inline uint32_t reg_offset(uint32_t op) {
    uint32_t rm = r[op & 15], amt = (op >> 7) & 31;
    switch ((op >> 5) & 3) {
    case 0: return rm << amt;
    case 1: return amt ? rm >> amt : 0;
    case 2: return (uint32_t)((int32_t)rm >> (amt ? amt : 31));
    default: return amt ? rm >> amt | rm << (32 - amt) : (cpsr >> 29 & 1) << 31 | rm >> 1;
    }
}

static inline void set_nz(uint32_t v) {
    cpsr = (cpsr & ~(F_N | F_Z)) | (v & F_N) | (v ? 0 : F_Z);
}

static inline void set_nzcv(uint32_t res, uint32_t c, uint32_t v) {
    cpsr = (cpsr & 0x0FFFFFFFu) | (res & F_N) | (res ? 0 : F_Z) | (c << 29) | (v << 28);
}

/* ---- the instructions ---- */

INLINE int data_processing(uint32_t op, int* branched) {
    uint32_t carry, b, a, res = 0, rd = (op >> 12) & 15, opc = (op >> 21) & 15;
    int s = op >> 20 & 1, write = 1;
    uint32_t rn = (op >> 16) & 15;
    if ((op & 0x02000FF0u) == 0) {          /* a register, not shifted */
        b = r[op & 15];
        carry = cpsr >> 29 & 1;
    } else {
        b = shifter(op, &carry);
    }
    a = r[rn];
    if (rn == 15 && !(op & (1u << 25)) && (op & 0x10)) {
        a += 4;
    }
    switch (opc) {
    case 0x0: res = a & b; if (s) { set_nz(res); cpsr = (cpsr & ~F_C) | carry << 29; } break;
    case 0x1: res = a ^ b; if (s) { set_nz(res); cpsr = (cpsr & ~F_C) | carry << 29; } break;
    case 0x2: res = a - b; if (s) set_nzcv(res, a >= b, ((a ^ b) & (a ^ res)) >> 31); break;
    case 0x3: res = b - a; if (s) set_nzcv(res, b >= a, ((b ^ a) & (b ^ res)) >> 31); break;
    case 0x4: res = a + b; if (s) set_nzcv(res, res < a, (~(a ^ b) & (a ^ res)) >> 31); break;
    case 0x5: {
        uint32_t c = cpsr >> 29 & 1;
        uint64_t w = (uint64_t)a + b + c;
        res = (uint32_t)w;
        if (s) set_nzcv(res, (uint32_t)(w >> 32), (~(a ^ b) & (a ^ res)) >> 31);
        break;
    }
    case 0x6: {
        uint32_t c = cpsr >> 29 & 1;
        uint64_t w = (uint64_t)a - b - (1 - c);
        res = (uint32_t)w;
        if (s) set_nzcv(res, (uint32_t)(w >> 32) == 0, ((a ^ b) & (a ^ res)) >> 31);
        break;
    }
    case 0x7: {
        uint32_t c = cpsr >> 29 & 1;
        uint64_t w = (uint64_t)b - a - (1 - c);
        res = (uint32_t)w;
        if (s) set_nzcv(res, (uint32_t)(w >> 32) == 0, ((b ^ a) & (b ^ res)) >> 31);
        break;
    }
    case 0x8: res = a & b; set_nz(res); cpsr = (cpsr & ~F_C) | carry << 29; write = 0; break;
    case 0x9: res = a ^ b; set_nz(res); cpsr = (cpsr & ~F_C) | carry << 29; write = 0; break;
    case 0xA: res = a - b; set_nzcv(res, a >= b, ((a ^ b) & (a ^ res)) >> 31); write = 0; break;
    case 0xB: res = a + b; set_nzcv(res, res < a, (~(a ^ b) & (a ^ res)) >> 31); write = 0; break;
    case 0xC: res = a | b; if (s) { set_nz(res); cpsr = (cpsr & ~F_C) | carry << 29; } break;
    case 0xD: res = b; if (s) { set_nz(res); cpsr = (cpsr & ~F_C) | carry << 29; } break;
    case 0xE: res = a & ~b; if (s) { set_nz(res); cpsr = (cpsr & ~F_C) | carry << 29; } break;
    default: res = ~b; if (s) { set_nz(res); cpsr = (cpsr & ~F_C) | carry << 29; } break;
    }
    if (rd == 15 && s) {
        /* to the PC with S: back from an exception, the mode's saved status put back */
        if (write) {
            r[15] = res & ~3u;
            *branched = 1;
        }
        if (bank_of(cpsr) != 0) {
            set_cpsr(spsr);
        }
        return write ? 3 : 1;
    }
    if (write) {
        r[rd] = rd == 15 ? res & ~3u : res;
        *branched |= rd == 15;
        return rd == 15 ? 3 : 1;
    }
    return 1;
}

static void psr_transfer(uint32_t op) {
    if (!(op & (1u << 21))) {                     /* MRS */
        r[(op >> 12) & 15] = op & (1u << 22) ? spsr : cpsr;
        return;
    } else {                                    /* MSR */
        uint32_t v, mask = 0;
        if (op & (1u << 25)) {
            uint32_t rot = (op >> 7) & 0x1E, imm = op & 0xFF;
            v = rot ? imm >> rot | imm << (32 - rot) : imm;
        } else {
            v = r[op & 15];
        }
        if (op & (1u << 19)) mask |= 0xFF000000u;
        if (op & (1u << 16)) mask |= 0x000000FFu;
        if (op & (1u << 22)) {
            if (bank_of(cpsr) != 0) {
                spsr = (spsr & ~mask) | (v & mask);
            }
            return;
        }
        if ((cpsr & 0x1F) == M_USR) {
            mask &= 0xFF000000u;
        }
        set_cpsr((cpsr & ~mask) | (v & mask) | 0x10);
    }
}

static int multiply(uint32_t op) {
    uint32_t rd = (op >> 16) & 15, rn = (op >> 12) & 15, rs = r[(op >> 8) & 15], rm = r[op & 15];
    if (op & (1u << 23)) {                      /* long */
        uint64_t res;
        if (op & (1u << 22)) {
            res = (uint64_t)((int64_t)(int32_t)rm * (int32_t)rs);
        } else {
            res = (uint64_t)rm * rs;
        }
        if (op & (1u << 21)) {
            res += (uint64_t)r[rd] << 32 | r[rn];
        }
        r[rn] = (uint32_t)res;
        r[rd] = (uint32_t)(res >> 32);
        if (op & (1u << 20)) {
            cpsr = (cpsr & ~(F_N | F_Z)) | ((uint32_t)(res >> 32) & F_N) | (res ? 0 : F_Z);
        }
        return 4;
    } else {
        uint32_t res = rm * rs;
        if (op & (1u << 21)) {
            res += r[rn];
        }
        r[rd] = res;
        if (op & (1u << 20)) {
            set_nz(res);
        }
        return 3;
    }
}

INLINE int single_transfer(uint32_t op, int* branched) {
    if ((op & 0x03200000u) == 0x01000000u) {    /* an immediate offset, no write back */
        uint32_t off = op & 0xFFF, rd = (op >> 12) & 15;
        uint32_t addr = op & (1u << 23) ? r[(op >> 16) & 15] + off : r[(op >> 16) & 15] - off;
        if (op & (1u << 20)) {
            uint32_t v = op & (1u << 22) ? rd8(addr) : ldr_word(addr);
            if (rd == 15) {
                r[15] = v & ~3u;
                *branched = 1;
                return 5;
            }
            r[rd] = v;
            return 3;
        }
        if (op & (1u << 22)) {
            wr8(addr, r[rd] + (rd == 15 ? 4 : 0));
        } else {
            wr32(addr, r[rd] + (rd == 15 ? 4 : 0));
        }
        return 2;
    }
    uint32_t rn = (op >> 16) & 15, rd = (op >> 12) & 15, base = r[rn], off, addr;
    int up = op >> 23 & 1, pre = op >> 24 & 1, wb = op >> 21 & 1, load = op >> 20 & 1, byte = op >> 22 & 1;
    off = op & (1u << 25) ? reg_offset(op) : op & 0xFFF;
    addr = pre ? (up ? base + off : base - off) : base;
    if (load) {
        uint32_t v = byte ? rd8(addr) : ldr_word(addr);
        if (!pre) {
            r[rn] = up ? base + off : base - off;
        } else if (wb) {
            r[rn] = addr;
        }
        r[rd] = rd == 15 ? v & ~3u : v;
        *branched |= rd == 15;
        return rd == 15 ? 5 : 3;
    } else {
        uint32_t v = r[rd] + (rd == 15 ? 4 : 0);
        if (byte) {
            wr8(addr, v);
        } else {
            wr32(addr, v);
        }
        if (!pre) {
            r[rn] = up ? base + off : base - off;
        } else if (wb) {
            r[rn] = addr;
        }
        return 2;
    }
}

static int half_transfer(uint32_t op) {
    uint32_t rn = (op >> 16) & 15, rd = (op >> 12) & 15, base = r[rn], off, addr, v;
    int up = op >> 23 & 1, pre = op >> 24 & 1, wb = op >> 21 & 1, load = op >> 20 & 1, sh = (op >> 5) & 3;
    off = op & (1u << 22) ? ((op >> 4) & 0xF0) | (op & 15) : r[op & 15];
    addr = pre ? (up ? base + off : base - off) : base;
    if (load) {
        switch (sh) {
        case 1: v = rd16(addr); break;
        case 2: v = (uint32_t)(int32_t)(int8_t)rd8(addr); break;
        default: v = (uint32_t)(int32_t)(int16_t)rd16(addr); break;
        }
    } else {
        wr16(addr, r[rd] + (rd == 15 ? 4 : 0));
        v = 0;
    }
    if (!pre) {
        r[rn] = up ? base + off : base - off;
    } else if (wb) {
        r[rn] = addr;
    }
    if (load) {
        r[rd] = v;
    }
    return 3;
}

INLINE int block_transfer(uint32_t op, int* branched) {
    uint32_t rn = (op >> 16) & 15, list = op & 0xFFFF, base = r[rn], addr, n = 0, i;
    int up = op >> 23 & 1, pre = op >> 24 & 1, s = op >> 22 & 1, wb = op >> 21 & 1, load = op >> 20 & 1;
    int user = s && !(load && (list & 0x8000)) && bank_of(cpsr) != 0;
    uint32_t saved = cpsr, end;
    n = list - (list >> 1 & 0x5555u);           /* the registers in the list */
    n = (n & 0x3333u) + (n >> 2 & 0x3333u);
    n = (n + (n >> 4)) & 0x0F0Fu;
    n = (n + (n >> 8)) & 0x1Fu;
    if (up) {
        addr = base + (pre ? 4 : 0);
        end = base + n * 4;
    } else {
        addr = base - n * 4 + (pre ? 0 : 4);
        end = base - n * 4;
    }
    if (user) {
        set_cpsr((cpsr & ~0x1Fu) | M_USR);
    }
    if (load) {
        if (wb) {
            r[rn] = end;
        }
        for (i = 0; i < 16; i++) {
            if (list >> i & 1) {
                r[i] = rd32(addr);
                addr += 4;
            }
        }
        if (list & 0x8000) {
            r[15] &= ~3u;
            *branched = 1;
            if (s && bank_of(cpsr) != 0) {
                set_cpsr(spsr);
            }
        }
    } else {
        int first = 1;
        for (i = 0; i < 16; i++) {
            if (list >> i & 1) {
                uint32_t v = i == rn && first ? base : r[i];
                if (i == 15) {
                    v += 4;
                }
                wr32(addr, v);
                addr += 4;
                first = 0;
            }
        }
        if (wb) {
            r[rn] = end;
        }
    }
    if (user) {
        set_cpsr(saved);
    }
    return (int)n + 2 + (load && (list & 0x8000) ? 2 : 0);
}

void arm7_stop(void) {
    run_left = 0;
    event = 1;
}

/* the FIQ's handler, if it is due: taken now and run until it returns, or for about `cycles` */
int arm7_run_fiq(int cycles) {
    int done;
    if (!fiq_line || (cpsr & F_F)) {
        return 0;
    }
    fiq_run = 1;
    done = arm7_run(cycles);
    fiq_run = 0;
    return done;
}

/* ---- the commonest instructions, decoded once ---- */

/* a kind and its fields for each word of code, kept while the word is the same; K_STEP is the rest */
enum {
    K_STEP, K_B, K_BL, K_ADDI, K_SUBI, K_SUBSI, K_CMPI, K_TSTI, K_TSTI_C0, K_MOVI, K_MOVR, K_MOVSR,
    K_LDRB, K_LDR, K_LDR_PC, K_STRB, K_STR, K_PUSH_PC, K_POP_PC,
    K_ANDI, K_ORRI, K_BICI, K_ADD_PC, K_ADDR, K_CMPR, K_PSR, K_COUNT
};
typedef struct {
    uint32_t op;
    const void* go;                 /* the kind's code in arm7_run */
    int32_t v;
    uint8_t rd, rn, rm, cond;
} DEC;
#define DEC_N 8192                  /* the RAM's first 32 KB */
static DEC dec[DEC_N];

static int decode(uint32_t op, int32_t* v) {
    uint32_t rn = (op >> 16) & 15, rd = (op >> 12) & 15, rm = op & 15;
    switch ((op >> 25) & 7) {
    case 5:
        *v = (int32_t)(op << 8) >> 6;
        return op & (1u << 24) ? K_BL : K_B;
    case 1: {                       /* data processing, an immediate */
        uint32_t rot = (op >> 7) & 0x1E, imm = op & 0xFF;
        *v = (int32_t)(rot ? imm >> rot | imm << (32 - rot) : imm);
        if ((op & 0x01900000u) == 0x01000000u) {
            return K_PSR;
        }
        if (rd == 15) {
            return K_STEP;
        }
        if (rn == 15) {
            return ((op >> 20) & 0x1F) == 0x08 ? K_ADD_PC : K_STEP;
        }
        switch ((op >> 20) & 0x1F) {
        case 0x00: return K_ANDI;
        case 0x18: return K_ORRI;
        case 0x1C: return K_BICI;
        case 0x08: return K_ADDI;
        case 0x04: return K_SUBI;
        case 0x05: return K_SUBSI;
        case 0x15: return K_CMPI;
        case 0x11: return rot == 0 ? K_TSTI : *v >= 0 ? K_TSTI_C0 : K_STEP;
        case 0x1A: return K_MOVI;
        }
        return K_STEP;
    }
    case 0:
        if ((op & 0x0F9000F0u) == 0x01000000u) {
            return K_PSR;
        }
        if ((op & 0xFF0) != 0 || rd == 15 || rm == 15) {    /* a register, not shifted */
            return K_STEP;
        }
        switch ((op >> 20) & 0x1F) {
        case 0x1A: return rn == 0 ? K_MOVR : K_STEP;
        case 0x1B: return rn == 0 ? K_MOVSR : K_STEP;
        case 0x08: return rn != 15 ? K_ADDR : K_STEP;
        case 0x15: return rn != 15 ? K_CMPR : K_STEP;
        }
        return K_STEP;
    case 2:                         /* LDR, LDRB, STR, STRB, an immediate offset, no write back */
        if ((op & 0x01200000u) != 0x01000000u || rd == 15) {
            return K_STEP;
        }
        *v = op & (1u << 23) ? (int32_t)(op & 0xFFF) : -(int32_t)(op & 0xFFF);
        if (rn == 15) {
            return (op & 0x00500000u) == 0x00100000u ? K_LDR_PC : K_STEP;
        }
        switch ((op >> 20) & 5) {
        case 0: return K_STR;
        case 1: return K_LDR;
        case 4: return K_STRB;
        default: return K_LDRB;
        }
    case 4:                         /* STMFD rn!, {pc} and LDMFD rn!, {pc} */
        if (rn != 15 && (op & 0x01F0FFFFu) == 0x01208000u) {
            return K_PUSH_PC;
        }
        if (rn != 15 && (op & 0x01F0FFFFu) == 0x00B08000u) {
            return K_POP_PC;
        }
        return K_STEP;
    }
    return K_STEP;
}

/* one instruction whose condition passed; gives its cycles */
INLINE int step(uint32_t op, uint32_t pc, int* branched) {
    switch ((op >> 25) & 7) {
    case 0:
        if ((op & 0x0FC000F0u) == 0x00000090u || (op & 0x0F8000F0u) == 0x00800090u) {
            return multiply(op);
        } else if ((op & 0x0FB00FF0u) == 0x01000090u) {     /* SWP */
            uint32_t a = r[(op >> 16) & 15], v;
            if (op & (1u << 22)) {
                v = rd8(a);
                wr8(a, r[op & 15]);
            } else {
                v = ldr_word(a);
                wr32(a, r[op & 15]);
            }
            r[(op >> 12) & 15] = v;
            return 4;
        } else if ((op & 0x90u) == 0x90u) {
            return half_transfer(op);
        } else if ((op & 0x0FFFFFF0u) == 0x012FFF10u) {     /* BX */
            r[15] = r[op & 15] & ~3u;
            *branched = 1;
            return 3;
        } else if ((op & 0x01900000u) == 0x01000000u) {
            psr_transfer(op);
            return 1;
        }
        return data_processing(op, branched);
    case 1:
        if ((op & 0x01900000u) == 0x01000000u) {
            psr_transfer(op);
            return 1;
        }
        return data_processing(op, branched);
    case 2:
        return single_transfer(op, branched);
    case 3:
        if (op & 0x10) {
            exception(M_UND, 0x04, pc + 4, F_I);
            *branched = 1;
            return 3;
        }
        return single_transfer(op, branched);
    case 4:
        return block_transfer(op, branched);
    case 5: {
        uint32_t off = (op & 0x00FFFFFFu) << 2;
        if (off & 0x02000000u) {
            off |= 0xFC000000u;
        }
        if (op & (1u << 24)) {
            r[14] = pc + 4;
        }
        r[15] = pc + 8 + off;
        *branched = 1;
        return 3;
    }
    case 6:
        exception(M_UND, 0x04, pc + 4, F_I);
        *branched = 1;
        return 3;
    default:
        if (op & (1u << 24)) {
            exception(M_SVC, 0x08, pc + 4, F_I);
        } else {
            exception(M_UND, 0x04, pc + 4, F_I);
        }
        *branched = 1;
        return 3;
    }
}

/* a case of the dispatch for each value of bits 20-27, so that step is built for each with those bits known */
#define STEP1(n) case n: done += step((op & 0x000FFFFFu) | (uint32_t)(n) << 20, pc, &branched); break;
#define STEP4(n) STEP1(n) STEP1(n + 1) STEP1(n + 2) STEP1(n + 3)
#define STEP16(n) STEP4(n) STEP4(n + 4) STEP4(n + 8) STEP4(n + 12)
#define STEP64(n) STEP16(n) STEP16(n + 16) STEP16(n + 32) STEP16(n + 48)

/* runs about `cycles`, or until arm7_stop; the FIQ is taken between instructions */
int arm7_run(int cycles) {
    static const void* const go[K_COUNT] = {
        &&k_step, &&k_b, &&k_bl, &&k_addi, &&k_subi, &&k_subsi, &&k_cmpi, &&k_tsti, &&k_tsti_c0, &&k_movi, &&k_movr,
        &&k_movsr, &&k_ldrb, &&k_ldr, &&k_ldr_pc, &&k_strb, &&k_str, &&k_push_pc, &&k_pop_pc,
        &&k_andi, &&k_orri, &&k_bici, &&k_add_pc, &&k_addr, &&k_cmpr, &&k_psr
    };
    const uint8_t* ram = aica_ram;
    uint32_t code_mask = aica_ram_mask & ~3u;
    int done = 0;
    run_left = 1;
    event = fiq_line && !(cpsr & F_F);
    while (done < cycles) {
        uint32_t pc, op;
        const DEC* e;
        int branched = 0;
        if (event) {
            if (!run_left) {
                break;
            }
            event = 0;
            if (fiq_line && !(cpsr & F_F)) {
                exception(M_FIQ, 0x1C, r[15] + 4, F_I | F_F);
                done += 3;
            }
        }
        pc = r[15];
        if (pc < DEC_N * 4) {
            DEC* d = &dec[pc >> 2];
            memcpy(&op, ram + pc, 4);
            if (d->op != op) {
                int32_t v = 0;
                d->op = op;
                d->go = go[decode(op, &v)];
                d->v = v;
                d->rd = (op >> 12) & 15;
                d->rn = (op >> 16) & 15;
                d->rm = op & 15;
                d->cond = op >> 28;
            }
            e = d;
            if (e->cond != 14 && !(cond_ok[e->cond] >> (cpsr >> 28) & 1)) {
                r[15] = pc + 4;
                done++;
                continue;
            }
            goto *e->go;
        }
        if ((pc & 0xFF800000u) == 0) {
            memcpy(&op, ram + (pc & code_mask), 4);
        } else {
            op = aica_arm_read32(pc & ~3u);
        }
        if ((op & 0xF0000000u) != 0xE0000000u && !(cond_ok[op >> 28] >> (cpsr >> 28) & 1)) {
            r[15] = pc + 4;
            done++;
            continue;
        }
    k_step:
        r[15] = pc + 8;             /* as the instruction reads it */
        switch ((op >> 20) & 0xFF) {
        STEP64(0) STEP64(64) STEP64(128) STEP64(192)
        }
        /* no write to the PC: on to the next instruction */
        if (!branched) {
            r[15] = pc + 4;
        }
        continue;
    k_b:
        r[15] = pc + 8 + (uint32_t)e->v;
        done += 3;
        continue;
    k_bl:
        r[14] = pc + 4;
        r[15] = pc + 8 + (uint32_t)e->v;
        done += 3;
        continue;
    k_addi:
        r[e->rd] = r[e->rn] + (uint32_t)e->v;
        r[15] = pc + 4;
        done += 1;
        continue;
    k_subi:
        r[e->rd] = r[e->rn] - (uint32_t)e->v;
        r[15] = pc + 4;
        done += 1;
        continue;
    k_subsi: {
        uint32_t a = r[e->rn], b = (uint32_t)e->v, res = a - b;
        r[e->rd] = res;
        set_nzcv(res, a >= b, ((a ^ b) & (a ^ res)) >> 31);
        r[15] = pc + 4;
        done += 1;
        continue;
    }
    k_cmpi: {
        uint32_t a = r[e->rn], b = (uint32_t)e->v, res = a - b;
        set_nzcv(res, a >= b, ((a ^ b) & (a ^ res)) >> 31);
        r[15] = pc + 4;
        done += 1;
        continue;
    }
    k_tsti:
        set_nz(r[e->rn] & (uint32_t)e->v);
        r[15] = pc + 4;
        done += 1;
        continue;
    k_tsti_c0:                      /* a rotated immediate: the carry is its top bit */
        set_nz(r[e->rn] & (uint32_t)e->v);
        cpsr &= ~F_C;
        r[15] = pc + 4;
        done += 1;
        continue;
    k_andi:
        r[e->rd] = r[e->rn] & (uint32_t)e->v;
        r[15] = pc + 4;
        done += 1;
        continue;
    k_orri:
        r[e->rd] = r[e->rn] | (uint32_t)e->v;
        r[15] = pc + 4;
        done += 1;
        continue;
    k_bici:
        r[e->rd] = r[e->rn] & ~(uint32_t)e->v;
        r[15] = pc + 4;
        done += 1;
        continue;
    k_add_pc:
        r[e->rd] = pc + 8 + (uint32_t)e->v;
        r[15] = pc + 4;
        done += 1;
        continue;
    k_addr:
        r[e->rd] = r[e->rn] + r[e->rm];
        r[15] = pc + 4;
        done += 1;
        continue;
    k_cmpr: {
        uint32_t a = r[e->rn], b = r[e->rm], res = a - b;
        set_nzcv(res, a >= b, ((a ^ b) & (a ^ res)) >> 31);
        r[15] = pc + 4;
        done += 1;
        continue;
    }
    k_psr:                          /* MRS and MSR */
        r[15] = pc + 8;
        psr_transfer(e->op);
        r[15] = pc + 4;
        done += 1;
        continue;
    k_movi:
        r[e->rd] = (uint32_t)e->v;
        r[15] = pc + 4;
        done += 1;
        continue;
    k_movr:
        r[e->rd] = r[e->rm];
        r[15] = pc + 4;
        done += 1;
        continue;
    k_movsr:
        r[e->rd] = r[e->rm];
        set_nz(r[e->rd]);
        r[15] = pc + 4;
        done += 1;
        continue;
    k_ldrb:
        r[e->rd] = rd8(r[e->rn] + (uint32_t)e->v);
        r[15] = pc + 4;
        done += 3;
        continue;
    k_ldr:
        r[e->rd] = ldr_word(r[e->rn] + (uint32_t)e->v);
        r[15] = pc + 4;
        done += 3;
        continue;
    k_ldr_pc:
        r[e->rd] = ldr_word(pc + 8 + (uint32_t)e->v);
        r[15] = pc + 4;
        done += 3;
        continue;
    k_strb:
        wr8(r[e->rn] + (uint32_t)e->v, r[e->rd]);
        r[15] = pc + 4;
        done += 2;
        continue;
    k_str:
        wr32(r[e->rn] + (uint32_t)e->v, r[e->rd]);
        r[15] = pc + 4;
        done += 2;
        continue;
    k_push_pc:
        wr32(r[e->rn] - 4, pc + 12);
        r[e->rn] -= 4;
        r[15] = pc + 4;
        done += 3;
        continue;
    k_pop_pc: {
        uint32_t a = r[e->rn];
        r[e->rn] = a + 4;
        r[15] = rd32(a) & ~3u;
        done += 5;
        continue;
    }
    }
    cycles_run += (uint64_t)done;
    return done;
}
