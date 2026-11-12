/*
 * ctr_rom.c  The cartridge ROM on the 3DS: one image file on the SD card (mvsc2.bin, unpacked the first
 * time), its blocks read ahead on the system core after each miss
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <3ds.h>

#include "naomi_host.h"
#include "ctr_front.h"
#include "prof.h"

#define CACHE_LEAST (2u << 20)
#define BLOCK_SIZE 0x10000u
#define BUFS (CTR_ROM_BUFFERS / BLOCK_SIZE)
#define AHEAD 8                 /* blocks read past a request's end */

enum { B_FREE, B_QUEUED, B_READING, B_DONE };

typedef struct {
    uint32_t block;
    uint32_t age;               /* the order queued */
    volatile uint32_t state;
    uint8_t* data;
} BUF;

static BUF bufs[BUFS];
static uint8_t* buf_mem;
static LightLock lock;
static LightEvent wake;
static Thread thread;
static volatile int quit;
static Handle fh_thread, fh_main;
static uint32_t file_blocks, queue_clock;
static int is_open;

static int raw_read(Handle h, uint32_t b, void* dst) {
    u32 got = 0;
    return R_SUCCEEDED(FSFILE_Read(h, &got, (u64)b * BLOCK_SIZE, dst, BLOCK_SIZE)) && got == BLOCK_SIZE;
}

static int open_raw(const char* path, Handle* h) {
    return strncmp(path, "sdmc:", 5) == 0 &&
           R_SUCCEEDED(FSUSER_OpenFileDirectly(h, ARCHIVE_SDMC, fsMakePath(PATH_EMPTY, ""),
                                               fsMakePath(PATH_ASCII, path + 5), FS_OPEN_READ, 0));
}

static void reader_main(void* arg) {
    (void)arg;
    while (!quit) {
        BUF* next = NULL;
        int i;
        LightLock_Lock(&lock);
        for (i = 0; i < BUFS; i++) {
            if (bufs[i].state == B_QUEUED && (next == NULL || bufs[i].age < next->age)) {
                next = &bufs[i];
            }
        }
        if (next != NULL) {
            next->state = B_READING;
        }
        LightLock_Unlock(&lock);
        if (next == NULL) {
            LightEvent_Wait(&wake);
            continue;
        }
        {
            PROF_START(t);
            if (!raw_read(fh_thread, next->block, next->data)) {
                memset(next->data, 0xFF, BLOCK_SIZE);
            }
            PROF_STOP(PZ_ROM_THREAD, t);
        }
        LightLock_Lock(&lock);
        next->state = B_DONE;
        LightLock_Unlock(&lock);
    }
}

static BUF* find(uint32_t b) {
    int i;
    for (i = 0; i < BUFS; i++) {
        if (bufs[i].state != B_FREE && bufs[i].block == b) {
            return &bufs[i];
        }
    }
    return NULL;
}

/* from a buffer that has it or is reading it, else read now */
static int rom_read(uint32_t b, void* dst) {
    BUF* found;
    int ok;
    PROF_START(t);
    LightLock_Lock(&lock);
    found = find(b);
    if (found != NULL && found->state == B_QUEUED) {
        found->state = B_FREE;
        found = NULL;
    }
    LightLock_Unlock(&lock);
    if (found == NULL) {
        ok = b < file_blocks && raw_read(fh_main, b, dst);
        PROF_COUNT(PZ_ROM_MISS, 1);
    } else {
        while (found->state == B_READING) {
            svcSleepThread(100000);
        }
        memcpy(dst, found->data, BLOCK_SIZE);
        LightLock_Lock(&lock);
        found->state = B_FREE;
        LightLock_Unlock(&lock);
        ok = 1;
    }
    PROF_STOP(PZ_ROM_MISS, t);
    return ok;
}

/* after a miss: the rest of the request it was in, then the blocks after */
static void rom_missed(uint32_t b, uint32_t last) {
    uint32_t p;
    int i, queued = 0;
    if (thread == NULL) {
        return;
    }
    LightLock_Lock(&lock);
    for (p = b + 1; p <= last + AHEAD && p < file_blocks; p++) {
        BUF* use = NULL;
        if (naomi_rom_cached(p) || find(p) != NULL) {
            continue;
        }
        for (i = 0; i < BUFS && use == NULL; i++) {
            if (bufs[i].state == B_FREE) {
                use = &bufs[i];
            }
        }
        if (use == NULL) {
            /* else the oldest queued one past the request */
            for (i = 0; i < BUFS; i++) {
                if (bufs[i].state == B_QUEUED && bufs[i].block > last && (use == NULL || bufs[i].age < use->age)) {
                    use = &bufs[i];
                }
            }
        }
        if (use == NULL) {
            break;
        }
        use->block = p;
        use->age = ++queue_clock;
        use->state = B_QUEUED;
        queued = 1;
    }
    LightLock_Unlock(&lock);
    if (queued) {
        LightEvent_Signal(&wake);
    }
}

/* between frames */
void ctr_rom_poll(void) {
    int i;
    PROF_START(t);
    if (!is_open) {
        return;
    }
    for (i = 0; i < BUFS; i++) {
        if (bufs[i].state == B_DONE) {
            naomi_rom_put(bufs[i].block, bufs[i].data);
            LightLock_Lock(&lock);
            bufs[i].state = B_FREE;
            LightLock_Unlock(&lock);
        }
    }
    PROF_STOP(PZ_ROM_POLL, t);
}

static void unpack_progress(uint32_t done, uint32_t total) {
    const uint32_t width = 30;
    uint32_t i, n = done * width / total;
    printf("\x1b[14;1H[");
    for (i = 0; i < width; i++) {
        putchar(i < n ? '#' : '-');
    }
    printf("] %3lu%%", (unsigned long)(done * 100 / total));
    gfxFlushBuffers();
    gspWaitForVBlank();
}

const char* ctr_rom_error;

static int rom_open(const char* dir) {
    char path[1100];
    s32 prio = 0x30;
    u64 size = 0;
    u32 limit = 0;
    int i;
    if (naomi_rom_cache_bytes < CACHE_LEAST) {
        host_log("arcade mode: a ROM cache of %lu KB, less than the %u KB it needs", (unsigned long)(naomi_rom_cache_bytes >> 10),
                 CACHE_LEAST >> 10);
        ctr_rom_error = "Arcade mode needs more memory than a 3dsx has on an Old 3DS. Install the CIA (with FBI) and "
                        "start it from the HOME Menu, or play the Dreamcast version.";
        return 0;
    }
    snprintf(path, sizeof path, "%s%s/%s", strncmp(dir, "sdmc:", 5) != 0 ? CTR_HOME "/" : "", dir, naomi_rom_image_name());
    if (!open_raw(path, &fh_main)) {
        consoleClear();
        printf("Marvel vs. Capcom 2\n\nONE-TIME ROM UNPACK\n\nThe ROM set's files are joined into\n%s/%s\n"
               "(136 MB), so the game can read any\npart of it quickly.\n\nThis happens only once. Please don't\n"
               "turn off the 3DS or take out the\nSD card.\n", dir, naomi_rom_image_name());
        unpack_progress(0, 1);
        host_log("unpacking the ROM set into %s", path);
        if (!naomi_rom_make_image(dir, path, unpack_progress) || !open_raw(path, &fh_main)) {
            host_log("could not unpack the ROM set into %s", path);
            consoleClear();
            return 0;
        }
        printf("\n\nDone. The set's other files can be\ndeleted now.\n");
        consoleClear();
    }
    if (R_FAILED(FSFILE_GetSize(fh_main, &size)) || !open_raw(path, &fh_thread)) {
        FSFILE_Close(fh_main);
        return 0;
    }
    file_blocks = (uint32_t)(size / BLOCK_SIZE);
    buf_mem = malloc(BUFS * BLOCK_SIZE);
    if (buf_mem == NULL) {
        FSFILE_Close(fh_main);
        FSFILE_Close(fh_thread);
        return 0;
    }
    for (i = 0; i < BUFS; i++) {
        bufs[i].data = buf_mem + i * BLOCK_SIZE;
        bufs[i].state = B_FREE;
    }
    LightLock_Init(&lock);
    LightEvent_Init(&wake, RESET_ONESHOT);
    quit = 0;
    svcGetThreadPriority(&prio, CUR_THREAD_HANDLE);
    thread = NULL;
    /* the system core, at 30% or more */
    if (R_SUCCEEDED(APT_GetAppCpuTimeLimit(&limit)) && (limit >= 30 || R_SUCCEEDED(APT_SetAppCpuTimeLimit(30)))) {
        thread = threadCreate(reader_main, NULL, 16 * 1024, prio - 1, 1, false);
    }
    if (thread == NULL) {
        thread = threadCreate(reader_main, NULL, 16 * 1024, prio - 1, -2, false);
    }
    is_open = 1;
    host_log("ROM from %s, read ahead %s", path, thread != NULL ? "on another thread" : "not at all (no thread)");
    return 1;
}

static void rom_close(void) {
    if (!is_open) {
        return;
    }
    if (thread != NULL) {
        quit = 1;
        LightEvent_Signal(&wake);
        threadJoin(thread, U64_MAX);
        threadFree(thread);
        thread = NULL;
    }
    FSFILE_Close(fh_main);
    FSFILE_Close(fh_thread);
    free(buf_mem);
    buf_mem = NULL;
    is_open = 0;
}

static const NAOMI_ROM_READER reader = { rom_open, rom_read, rom_missed, rom_close };

void ctr_rom_start(void) {
    naomi_rom_set_reader(&reader);
}
