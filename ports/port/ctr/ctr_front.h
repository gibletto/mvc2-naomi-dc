/* ctr_front.h  The 3DS front end's messages (main_3ds.c) and ROM reads (ctr_rom.c) */

#ifndef CTR_FRONT_H
#define CTR_FRONT_H

void ctr_tell(const char* title, const char* text);    /* shown on the bottom screen until START */
int ctr_ask(const char* text);                          /* 0 arcade (A), 1 Dreamcast (B), -1 quit (START) */

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
extern const char* ctr_rom_error;                       /* or NULL */

#endif
