/*
 * naomi_host.h  What the port supplies in place of the NAOMI board and Sega's libraries
 */

#ifndef NAOMI_HOST_H
#define NAOMI_HOST_H

#include <stddef.h>
#include <stdint.h>

/* mapped at its arcade address: the game keeps buffers there */
#define NAOMI_RAM_BASE  0x0C000000u
#define NAOMI_RAM_SIZE  0x02000000u

/* a word of the program's data that points at what the host defines (port/tools/gen_host.py) */
typedef struct {
    char* at;
    char* to;
} HOST_RELOC;

extern const HOST_RELOC host_reloc[];

/* .data and .bss, between port/game/span_begin.c and span_end.c */
typedef struct {
    char* data;
    char* data_end;
    char* bss;
    char* bss_end;
} HOST_SPAN;

/* port/game/program.c */
typedef struct {
    void (*main)(void);
    const HOST_RELOC* reloc;
    HOST_SPAN span;
} HOST_PROGRAM;

extern const HOST_PROGRAM arc_program;
extern const HOST_PROGRAM dc_program;

/* the cartridge's ROM as one address space (naomi_rom.c) */
int naomi_rom_open(const char* path);
extern uint32_t naomi_rom_cache_bytes;      /* the most the ROM cache holds (the 3DS front end sets it) */
const char* naomi_rom_check(const char* path);  /* a file missing or the wrong size, or NULL */
int naomi_rom_read(uint32_t offset, void* dst, uint32_t size);   /* 0 past the end */
uint32_t naomi_rom_size(void);
void naomi_rom_close(void);
/* a front end's own reads of the ROM's 64 KB blocks, from the image naomi_rom_make_image writes */
typedef struct {
    int (*open)(const char* dir);       /* 1: it reads the ROM in dir (else the files are read as usual) */
    int (*read)(uint32_t block, void* dst);     /* a block the cache lacks, now: 1 when read */
    void (*missed)(uint32_t block, uint32_t last);  /* after one: the request it was in runs to last */
    void (*close)(void);
} NAOMI_ROM_READER;
void naomi_rom_set_reader(const NAOMI_ROM_READER* reader);
int naomi_rom_make_image(const char* dir, const char* path, void (*progress)(uint32_t done, uint32_t total));
const char* naomi_rom_image_name(void);
int naomi_rom_cached(uint32_t block);
void naomi_rom_put(uint32_t block, const void* data);   /* a block read ahead, into the cache */
void naomi_m1_read(uint32_t src, uint8_t* dst, uint32_t size);     /* a packed file (naomi_m1.c) */

/* the board (naomi_board.c) */
int naomi_map_ram(void);
int naomi_init(const HOST_PROGRAM* program, const char* rom_path);
void board_start(const HOST_PROGRAM* program);  /* a program loaded in RAM */
void board_at_stop(void (*fn)(void));
void board_at_frame(void (*fn)(void));  /* before the game runs */
void board_stop(void);              /* the game and what it holds let go, RAM cleared */
void naomi_run_frame(void);         /* until the game waits for the next vertical blank */
void host_poll(void);               /* from the game's busy waits: the interrupts happen here */
void host_stub(const char* name);   /* a library routine the port lacks: says so once */
void host_stub_report(void);
unsigned host_frame(void);
void host_log(const char* fmt, ...);
void host_divzero_install(void);    /* a division by zero gives 0, as SHC's code did */
struct tm;
void host_clock(struct tm* tm);
extern int host_unlock;             /* unlock = all */

/* the Dreamcast (port/dc), from the disc's files in a folder */
int dc_init(const HOST_PROGRAM* program, const char* disc_path);
int dc_gd_open(const char* disc_path);
const char* dc_disc_check(const char* disc_path);  /* a file missing or the wrong size, or NULL */
long dc_gd_read_file(const char* name, void* buf, long max);  /* a whole file; its size, or -1 */

/* port/sys/restart.c */
void host_keep_start(void);         /* first thing, before any of them runs */
void host_restart(void);            /* with the board stopped */

/* the front end (main_sdl.c, main_3ds.c) */
void host_present(void);
int pick_game(int argc, char** argv);   /* start.c: from the data found */
const char* pick_message(void);         /* why none can run ("" when the player quit) */
const char* config_get(const char* key);             /* mvc2.cfg's, or NULL */
const char* host_save_path(const char* name);
void config_set(const char* key, const char* value); /* and saves mvc2.cfg */
int start_game(void);
int switch_game(void);              /* to the other one, when its data was found */
const char* game_title(void);

#endif
