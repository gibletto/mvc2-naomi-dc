/*
 * naomi_host.h  What the port supplies in place of the NAOMI board and Sega's libraries
 */

#ifndef NAOMI_HOST_H
#define NAOMI_HOST_H

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

/* main RAM, mapped at its arcade address (the game keeps buffers there) unless the build moves it
   (MVC2_RAM_BASE, which the game's C takes too); the Dreamcast's program alone needs only its 16 MB
   (MVC2_RAM_SIZE) */
#ifndef MVC2_RAM_BASE
#define MVC2_RAM_BASE   0x0C000000
#endif
#ifndef MVC2_RAM_SIZE
#define MVC2_RAM_SIZE   0x02000000
#endif
#define NAOMI_RAM_BASE  (0u + MVC2_RAM_BASE)
#define NAOMI_RAM_SIZE  (0u + MVC2_RAM_SIZE)
#define SH4_RAM_BASE    0x0C000000u
#define SH4_RAM_SIZE    0x02000000u     /* the board's; the Dreamcast's is 0x01000000 */
/* the host's place of the SH-4's main RAM address a */
#define HOST_RAM(a)     ((void*)(uintptr_t)((a) - SH4_RAM_BASE + NAOMI_RAM_BASE))
/* the main RAM mapped: NAOMI_RAM_SIZE, or the Dreamcast's 16 MB where the host has room for no more */
extern uint32_t host_ram_size;

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
    int (*read)(uint32_t block, uint32_t last, void* dst);  /* a block the cache lacks, now: 1 when read; the
                                                               request it is in runs to last */
    void (*missed)(uint32_t block, uint32_t last);  /* after one: the request it was in runs to last */
    void (*close)(void);
} NAOMI_ROM_READER;
void naomi_rom_set_reader(const NAOMI_ROM_READER* reader);
int naomi_rom_make_image(const char* dir, const char* path, void (*progress)(uint32_t done, uint32_t total));
const char* naomi_rom_image_name(void);
int naomi_rom_unpack_failed(void);         /* MiSTer: the zip fitted neither its drive nor the SD card */
int naomi_rom_cached(uint32_t block);
uint32_t naomi_rom_held(void);              /* the bytes the ROM cache holds */
void naomi_rom_put(uint32_t block, const void* data);   /* a block read ahead, into the cache */
/* the set's files from its zip (naomi_zip.c) */
typedef struct NAOMI_ZIP NAOMI_ZIP;
NAOMI_ZIP* naomi_zip_open(const char* path);    /* NULL if no zip */
int naomi_zip_has(const NAOMI_ZIP* zip, const char* name, uint32_t size);
int naomi_zip_read(NAOMI_ZIP* zip, const char* name, uint8_t* dst, uint32_t size,
                   void (*step)(uint32_t done));   /* 1 when read, CRC checked; step is called as it goes */
void naomi_zip_close(NAOMI_ZIP* zip);
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
extern int host_stubs_quiet;    /* the MiSTer: the stubs' first calls not logged */
unsigned host_frame(void);
extern uint32_t host_fills;         /* counts the port's writes into the game's memory: ROM and disc reads */
void host_log(const char* fmt, ...);
/* how far the start has got, for a front end that keeps it (the PSP's crumbs); NULL elsewhere */
extern void (*host_crumb)(const char* what);
#define HOST_CRUMB(what) do { if (host_crumb != NULL) host_crumb(what); } while (0)
void host_divzero_install(void);    /* a division by zero gives 0, as SHC's code did */
struct tm;
void host_clock(struct tm* tm);
extern int host_unlock;             /* unlock = all */
extern int host_freeplay;           /* freeplay = on */
void* host_user_eeprom(void);       /* the game's settings, as the EEPROM keeps them (nl_am.c), or NULL */
void* host_user_backup(void);       /* and its book-keeping, as the backup memory does */
void nlaUpdateUserEEPROM(void);     /* each saved to its file */
void nlaUpdateUserBackup(void);

/* the Dreamcast (port/dc), from the disc's files in a folder */
int dc_init(const HOST_PROGRAM* program, const char* disc_path);
int dc_gd_open(const char* disc_path);
const char* dc_disc_check(const char* disc_path);  /* a file missing or the wrong size, or NULL (or an image there) */
const char* dc_disc_missing(const char* disc_path);   /* the same, the files alone */
const char* dc_image_find(const char* disc_path);     /* a disc image's name there (dc_image.c), or NULL */
/* the disc's files from that image, into disc_path, with progress(done, total) in KB; NULL when done, else why not */
const char* dc_image_unpack(const char* disc_path, void (*progress)(uint32_t done, uint32_t total));
long dc_gd_read_file(const char* name, void* buf, long max);  /* a whole file; its size, or -1 */
const char* dc_gd_path(const char* name);   /* a file's path on the host */
void dc_vmu_flush(void);    /* the memory card written now, if MVC2_VMU_LATER=1 has held it back */
int dc_gd_mount(const char* disc_path);     /* 1 when the disc is read in place from its image there */
/* the image read in place (dc_image.c): chd_ok, a CHD too; a file's place; its bytes from off, safe on any thread */
int dc_image_mount(const char* disc_path, int chd_ok);
void dc_image_unmount(void);
int dc_image_mounted(void);
int dc_image_chd(void);         /* the image read in place is a CHD */
extern const char* const dc_unpack_mark;    /* the file beside the files while an unpack goes on */
int host_dir_list(const char* dir, void (*add)(const char* name, long size));   /* a folder's files; 0 if unread */
int dc_image_file(const char* name, uint32_t* lba, uint32_t* bytes);
size_t dc_image_read(uint32_t lba, uint32_t bytes, uint32_t off, void* to, size_t want);
/* a disc file as the game's thread and the readers open it: from the image when it is read in place, else from the
   folder (path as dc_gd_path gives it); each handle used by one thread at a time */
typedef struct DC_FILE DC_FILE;
DC_FILE* dc_file_open(const char* path, long* size);
size_t dc_file_read(DC_FILE* f, long off, void* to, size_t want);
void dc_file_close(DC_FILE* f);

/* the 3DS's read-ahead of one file (ctr_stream.c) */
int ctr_stream_open(const char* path, uint32_t most);
void ctr_stream_drop(void);
void ctr_stream_close(void);
int ctr_stream_queue(uint32_t off, uint32_t len);
int ctr_stream_put(uint32_t off, uint32_t len, const void* bytes);
int ctr_stream_find(uint32_t off, uint32_t n);
const uint8_t* ctr_stream_take(int i, uint32_t* off);
void ctr_stream_free(int i);
uint32_t ctr_stream_times(float* mean, float* most, uint32_t* dropped);   /* the reads since the last call; mean
                                                                            and most in ms; those read short */
int ctr_rom_dma(uint32_t offset, void* dst, uint32_t size);   /* the game's DMA from the ROM, 1 when read (ctr_rom.c) */
int ctr_save_later(const char* name, const void* data, int size); /* a save written by a thread, 1 when taken */
void ctr_save_flush(void);                                       /* every save written (ctr_save.c) */
int mister_save_later(const char* name, const void* data, int size);  /* the same on the MiSTer (mister_save.c) */
void mister_save_flush(void);
int sdl_save_later(const char* name, const void* data, int size);     /* the same on the PC (sdl_save.c) */
void sdl_save_flush(void);
void mister_log_flush(void);    /* the log's queued lines written (mister_log.c) */
void mister_log_rescue(void);   /* and from a fatal signal's handler */
void host_save_file(const char* name, const void* p, int size);  /* written whole or not at all (nl_am.c) */
int ctr_rom_packed(uint32_t offset, void* dst, uint32_t size); /* and its packed reads, 1 when given */
void dc_gd_read_elsewhere(const char* name);   /* a file read on another reader, as if by gdFsRead */
/* the host's disc reader, one read at a time (MVC2_GD_ASYNC): ctr_stream.c on the 3DS, psp_reader.c on the PSP,
   mister_reader.c on the MiSTer */
int host_read_start(DC_FILE* f, long off, void* to, size_t want);
int host_read_busy(void);
size_t host_read_end(float* ms);
int host_ahead_start(const char* path, size_t most);
int host_ahead_has(const char* path);
int host_ahead_idle(void);
DC_FILE* host_ahead_file(const char* path, long* size);
int host_ahead_take(size_t off, void* to, size_t want, float* ms);
void host_ahead_drop(int all);
void host_close_later(DC_FILE* f);
int host_small_start(const char* path, size_t most);
DC_FILE* host_small_open(const char* path, long* size);
int host_small_take(const char* path, size_t off, void* to, size_t want);
void dc_file_let_go(DC_FILE* d);
int dc_file_has_handle(const DC_FILE* d);        /* the PSP: a listed file's handle closed until its next read */
int dc_files_open(void);                /* the disc folder's files open now */
void* psp_copy(void* to, const void* from, size_t n);   /* the PSP: memcpy, by DMA when large */
int host_on_reader(void);               /* the PSP: 1 on the reader's thread */
int host_stick_wait(void);              /* the PSP: 1 after a short wait while the memory stick is away for a sleep */
const char* host_reader_why(void);      /* the PSP: why the reader's last read ahead failed, once; else NULL */
void host_small_drop(void);
/* the PSP's: a piece of the music read ahead on the reader, first of its work (dc_adx.c) */
int host_music_start(DC_FILE* f, long off, void* to, size_t want);
int host_music_busy(void);
void host_music_wait(void);

/* port/sys/restart.c */
void host_keep_start(void);         /* first thing, before any of them runs */
void host_keep_program(const HOST_PROGRAM* program);   /* before it first runs */
void host_restart(void);            /* with the board stopped */
extern void (*host_restart_hook)(int after);   /* the front end's settings in the layers taken before it (0), given back after (1) */
void* host_keep_block(size_t size); /* memory kept through restarts (the sound's RAM), or NULL */

/* the front end (main_sdl.c, main_3ds.c) */
void host_present(void);
int pick_game(int argc, char** argv);   /* start.c: from the data found */
const char* pick_message(void);         /* why none can run ("" when the player quit) */
enum { PICK_QUIT, PICK_NO_DATA, PICK_NO_ARCADE, PICK_NO_DC, PICK_BAD_DISC, PICK_NO_ROOM };
int pick_why(void);                     /* why pick_game or start_game gave up: a PICK_ */
const char* config_get(const char* key);             /* mvc2.cfg's, or NULL */
const char* host_save_path(const char* name);
void config_set(const char* key, const char* value); /* and saves mvc2.cfg */
int start_game(void);
int switch_game(void);              /* to the other one, when its data was found */
const char* game_title(void);
/* memory the caller can do without: NULL when there is none. On the 3DS any other allocation that fails stops
   the program with host_out_of_memory. */
void* host_try_malloc(size_t bytes);
void* host_try_calloc(size_t n, size_t size);
void host_out_of_memory(const char* what, size_t bytes);   /* says so, and the program ends */

#endif
