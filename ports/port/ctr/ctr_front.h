/* ctr_front.h  The 3DS front end's messages (main_3ds.c), pictures (ctr_art.c) and ROM reads (ctr_rom.c) */

#ifndef CTR_FRONT_H
#define CTR_FRONT_H

#include <stdint.h>

void ctr_tell(const char* title, const char* text);    /* shown on the bottom screen until START */
void ctr_stop(const char* text);                        /* said as ctr_tell, then the program ends (main thread) */
void ctr_hung(const char* text, int cpu_too);           /* the same with the GPU stuck (or another thread too) */
extern volatile int ctr_ending;                         /* the program is ending: no stop reported after */
extern const char* ctr_alloc_error;                     /* another thread's failed allocation, or NULL */
int ctr_ask(const char* text);                          /* 0 arcade, 1 Dreamcast, -1 quit (START) */
uint32_t* ctr_art_pillars(int target_w, int bar);
#include <stddef.h>
void* ctr_art_unpack(const unsigned char* lz, unsigned size, size_t bytes);   /* a tex3ds picture, or NULL */
int ctr_art_texel(int x, int y, int w);                 /* x, y's place in such a picture w wide */
int ctr_ask_bottom(int current);                        /* the version chooser alone: 0, 1, or -1 for neither */
extern volatile int ctr_bottom_lost;                    /* the bottom screen to draw again (after the HOME Menu) */

/* the options on the bottom screen while the game runs (ctr_options.c) */
enum { OPT_NONE, OPT_SWITCH, OPT_RESTART };
void ctr_options_show(void);                            /* after a start or a switch */
int ctr_options_input(void);                            /* each frame, after hidScanInput: OPT_ */
uint32_t ctr_options_keys(uint32_t held);               /* the 3DS's buttons as the player's (JVS_) */

#define CTR_HOME "sdmc:/3ds/mvc2"
#ifdef OLD_3DS_TEST
#define CTR_LOG "mvc2_o3ds.log"
#define CTR_PROF "prof_o3ds"
#else
#define CTR_LOG "mvc2.log"
#define CTR_PROF "prof"
#endif
#define CTR_ROM_BUFFERS (16u << 16)                     /* the read-ahead's memory */
void ctr_rom_start(void);
void ctr_rom_poll(void);                                /* between frames */
void ctr_unpack_screen(const char* text);               /* a long unpack's text on the bottom screen, or NULL to clear it */
void ctr_unpack_progress(uint32_t done, uint32_t total);   /* its bar */
extern const char* ctr_rom_error;                       /* or NULL */
extern int ctr_memcpy_fast;                             /* MVC2_MEMCPY_FAST=1: memcpy by ldm and stm (ctr_memcpy.c) */
extern int ctr_vram_writable;                           /* the CPU may write VRAM: not in the Homebrew Launcher's 3dsx */
void* ctr_memcpy(void* to, const void* from, size_t n);
void* __real_memcpy(void* to, const void* from, size_t n);
#include <stdio.h>
void ctr_log_start(FILE* f);                            /* host_log's lines written to f by a thread (ctr_log.c) */
void ctr_log_flush(void);                               /* all written, and the rest as they come */

#endif
