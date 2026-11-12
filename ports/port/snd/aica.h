/*
 * aica.h  The AICA: the sound RAM, the 64 channels, the DSP, timers and the sound CPU's interrupt
 */

#ifndef AICA_H
#define AICA_H

#include <stdint.h>

#define AICA_RATE 44100

int aica_init(int ram_8mb);                 /* 8 MB on the NAOMI, 2 MB on the Dreamcast, the CPU in reset; 0: no RAM */
void aica_free(void);                       /* the RAM given back (a restart's board_stop) */
void aica_render(int16_t* out, int n);      /* n stereo samples, the sound CPU run alongside */
void aica_run_cpu(int cycles);              /* the sound CPU alone, no time passing (the SH4 waiting on it) */
int aica_cpu_running(void);

/* the SH4's view: registers at 0x00700000, the RAM at 0x00800000 (any of the areas' mirrors) */
uint32_t aica_sh4_read32(uint32_t addr);
void aica_sh4_write32(uint32_t addr, uint32_t v);
uint8_t* aica_sh4_ptr(uint32_t addr);       /* a RAM address's host pointer */
uint32_t aica_ram_size(void);

/* for measuring */
typedef struct {
    uint64_t cpu_cycles;
    unsigned active_channels;
    unsigned dsp_steps;
} AICA_STATS;
void aica_stats(AICA_STATS* s);
void aica_dump(void (*log)(const char* fmt, ...));
void aica_set_dsp(int on);
void aica_set_reverb(int on);     /* a cheap reverb in the DSP's place, when it is off */
void aica_set_filter(int on);
void aica_set_cpu_rate(int cycles);

#endif
