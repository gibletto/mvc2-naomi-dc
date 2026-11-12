/*
 * naomi_rom.c  The cartridge's ROM as one address space, as MAME's naomi.cpp lays out the mvsc2 set
 *
 * ic11 at 0, the mask ROMs from 0x800000 8 MB each, then ic31 again; from 0x8000000 ic31 and ic32s as one
 * 32-bit wide ROM; 0x400000 mirrors 0x1200000. Read 64 KB blocks at a time through a cache.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "naomi_host.h"

typedef struct {
    const char* name;
    uint32_t offset;
    uint32_t size;
} ROM_FILE;

static const ROM_FILE files[] = {
    { "epr-23085a.ic11", 0x0000000, 0x400000 },
    { "mpr-23048.ic17s", 0x0800000, 0x800000 },
    { "mpr-23049.ic18",  0x1000000, 0x800000 },
    { "mpr-23050.ic19s", 0x1800000, 0x800000 },
    { "mpr-23051.ic20",  0x2000000, 0x800000 },
    { "mpr-23052.ic21s", 0x2800000, 0x800000 },
    { "mpr-23053.ic22",  0x3000000, 0x800000 },
    { "mpr-23054.ic23s", 0x3800000, 0x800000 },
    { "mpr-23055.ic24",  0x4000000, 0x800000 },
    { "mpr-23056.ic25s", 0x4800000, 0x800000 },
    { "mpr-23057.ic26",  0x5000000, 0x800000 },
    { "mpr-23058.ic27s", 0x5800000, 0x800000 },
    { "mpr-23059.ic28",  0x6000000, 0x800000 },
    { "mpr-23060.ic29",  0x6800000, 0x800000 },
    { "mpr-23061.ic30s", 0x7000000, 0x800000 },
    { "mpr-23083.ic31",  0x7800000, 0x400000 },
};
#define NFILES (sizeof files / sizeof *files)
#define IC31 (NFILES - 1)           /* and ic32s, opened last */
#define IC32 NFILES
#define WIDE_BASE 0x8000000u        /* a 16-bit word from each in turn */
#define WIDE_SIZE 0x400000u
#define MIRROR_FROM 0x1200000u
#define MIRROR_TO 0x0400000u
#define MIRROR_SIZE 0x400000u
#define ROM_SIZE 0x8800000u

#define BLOCK_SHIFT 16
#define BLOCK_SIZE (1u << BLOCK_SHIFT)
#define ROM_BLOCKS (ROM_SIZE >> BLOCK_SHIFT)
#define CACHE_BLOCKS ROM_BLOCKS     /* at most */
#define MIN_BLOCKS 4

#define IMAGE "mvsc2.bin"

uint32_t naomi_rom_cache_bytes = ROM_SIZE;
static int cache_blocks;

static FILE* rom_file[NFILES + 1];
static FILE* image;
static const NAOMI_ROM_READER* reader;
static int use_reader;
static int is_open;
static int16_t slot_of[ROM_BLOCKS];     /* a block's slot in the cache, or -1 */
static struct {
    uint8_t* data;
    int32_t block;                      /* -1: free */
    uint32_t used;
} slot[CACHE_BLOCKS];
static uint32_t clock_now;

static FILE* open_file(const char* dir, const char* name) {
    char path[1024];
    FILE* f;
    snprintf(path, sizeof path, "%s/%s", dir, name);
    f = fopen(path, "rb");
    if (f == NULL) {
        host_log("missing ROM file %s", path);
    } else {
        setvbuf(f, NULL, _IONBF, 0);    /* whole blocks are read */
    }
    return f;
}

static int read_at(int file, uint32_t pos, uint8_t* dst, uint32_t size) {
    FILE* f = rom_file[file];
    return f != NULL && fseek(f, (long)pos, SEEK_SET) == 0 && fread(dst, 1, size, f) == size;
}

/* from the set's files */
static void fill_from_files(uint32_t b, uint8_t* dst) {
    uint32_t at = b << BLOCK_SHIFT;
    size_t i;
    if (at >= MIRROR_TO && at < MIRROR_TO + MIRROR_SIZE) {
        at += MIRROR_FROM - MIRROR_TO;
    }
    if (at >= WIDE_BASE) {
        /* ic31's words at 0, 4, 8..., ic32s's at 2, 6, 10... */
        static uint8_t half[BLOCK_SIZE / 2];
        uint32_t from = (at - WIDE_BASE) / 2, j;
        int k;
        for (k = 0; k < 2; k++) {
            if (!read_at(k == 0 ? IC31 : IC32, from, half, sizeof half)) {
                memset(half, 0xFF, sizeof half);
            }
            for (j = 0; j < sizeof half; j += 2) {
                dst[j * 2 + k * 2] = half[j];
                dst[j * 2 + k * 2 + 1] = half[j + 1];
            }
        }
        return;
    }
    for (i = 0; i < NFILES; i++) {
        if (at >= files[i].offset && at < files[i].offset + files[i].size) {
            if (!read_at((int)i, at - files[i].offset, dst, BLOCK_SIZE)) {
                host_log("could not read %s at %X", files[i].name, at - files[i].offset);
                memset(dst, 0xFF, BLOCK_SIZE);
            }
            return;
        }
    }
    memset(dst, 0xFF, BLOCK_SIZE);
}

static void fill_block(uint32_t b, uint8_t* dst) {
    if (use_reader) {
        if (!reader->read(b, dst)) {
            host_log("could not read the ROM's block %X", b);
            memset(dst, 0xFF, BLOCK_SIZE);
        }
    } else if (image != NULL) {
        if (fseek(image, (long)(b << BLOCK_SHIFT), SEEK_SET) != 0 || fread(dst, 1, BLOCK_SIZE, image) != BLOCK_SIZE) {
            host_log("could not read " IMAGE " at %X", b << BLOCK_SHIFT);
            memset(dst, 0xFF, BLOCK_SIZE);
        }
    } else {
        fill_from_files(b, dst);
    }
}

/* a free slot, else the least recently read; NULL without memory */
static uint8_t* take_slot(uint32_t b) {
    int s = 0, i;
    for (i = 0; i < cache_blocks; i++) {
        if (slot[i].block < 0) {
            s = i;
            break;
        }
        if (slot[i].used < slot[s].used) {
            s = i;
        }
    }
    if (slot[s].data == NULL) {
        slot[s].data = malloc(BLOCK_SIZE);
        if (slot[s].data == NULL) {
            return NULL;
        }
    }
    if (slot[s].block >= 0) {
        slot_of[slot[s].block] = -1;
    }
    slot[s].block = (int32_t)b;
    slot[s].used = ++clock_now;
    slot_of[b] = (int16_t)s;
    return slot[s].data;
}

/* last is the request's last block, for a reader reading ahead */
static const uint8_t* block(uint32_t b, uint32_t last) {
    int s = slot_of[b];
    uint8_t* d;
    if (s >= 0) {
        slot[s].used = ++clock_now;
        return slot[s].data;
    }
    d = take_slot(b);
    if (d == NULL) {
        return NULL;
    }
    fill_block(b, d);
    if (use_reader && reader->missed != NULL) {
        reader->missed(b, last);
    }
    return d;
}

int naomi_rom_cached(uint32_t b) {
    return b < ROM_BLOCKS && slot_of[b] >= 0;
}

/* a block the reader read ahead */
void naomi_rom_put(uint32_t b, const void* data) {
    uint8_t* d;
    if (is_open && b < ROM_BLOCKS && slot_of[b] < 0 && (d = take_slot(b)) != NULL) {
        memcpy(d, data, BLOCK_SIZE);
    }
}

void naomi_rom_set_reader(const NAOMI_ROM_READER* r) {
    reader = r;
}

static int has_file(const char* dir, const char* name, uint32_t size) {
    char path[1024];
    struct stat st;
    snprintf(path, sizeof path, "%s/%s", dir, name);
    return stat(path, &st) == 0 && (uint32_t)st.st_size == size;
}

/* the first of the set's files missing from dir or the wrong size, or NULL (or the image there) */
const char* naomi_rom_check(const char* dir) {
    size_t i;
    if (has_file(dir, IMAGE, ROM_SIZE)) {
        return NULL;
    }
    for (i = 0; i < NFILES; i++) {
        if (!has_file(dir, files[i].name, files[i].size)) {
            return files[i].name;
        }
    }
    return has_file(dir, "mpr-23084.ic32s", WIDE_SIZE) ? NULL : "mpr-23084.ic32s";
}

static int open_files(const char* dir) {
    size_t i;
    for (i = 0; i <= NFILES; i++) {
        rom_file[i] = open_file(dir, i < NFILES ? files[i].name : "mpr-23084.ic32s");
        if (rom_file[i] == NULL) {
            return 0;
        }
    }
    return 1;
}

static void close_files(void) {
    size_t i;
    for (i = 0; i <= NFILES; i++) {
        if (rom_file[i] != NULL) {
            fclose(rom_file[i]);
            rom_file[i] = NULL;
        }
    }
}

/* the image, from the set's files in dir; 1 when done */
int naomi_rom_make_image(const char* dir, const char* path, void (*progress)(uint32_t done, uint32_t total)) {
    char tmp[1100];
    uint8_t* buf = malloc(BLOCK_SIZE);
    FILE* f;
    uint32_t b;
    int ok;
    snprintf(tmp, sizeof tmp, "%s.part", path);
    f = fopen(tmp, "wb");
    ok = buf != NULL && f != NULL && open_files(dir);
    for (b = 0; ok && b < ROM_BLOCKS; b++) {
        fill_from_files(b, buf);
        ok = fwrite(buf, 1, BLOCK_SIZE, f) == BLOCK_SIZE;
        if (progress != NULL && (b % 32 == 31 || b == ROM_BLOCKS - 1)) {
            progress(b + 1, ROM_BLOCKS);
        }
    }
    close_files();
    if (f != NULL && fclose(f) != 0) {
        ok = 0;
    }
    free(buf);
    remove(path);
    if (!ok || rename(tmp, path) != 0) {
        remove(tmp);
        return 0;
    }
    return 1;
}

const char* naomi_rom_image_name(void) {
    return IMAGE;
}

int naomi_rom_open(const char* dir) {
    char path[1100];
    size_t i;
    for (i = 0; i < ROM_BLOCKS; i++) {
        slot_of[i] = -1;
    }
    cache_blocks = (int)(naomi_rom_cache_bytes >> BLOCK_SHIFT);
    cache_blocks = cache_blocks < MIN_BLOCKS ? MIN_BLOCKS : cache_blocks > CACHE_BLOCKS ? CACHE_BLOCKS : cache_blocks;
    for (i = 0; i < CACHE_BLOCKS; i++) {
        slot[i].block = -1;
        slot[i].used = 0;
    }
    snprintf(path, sizeof path, "%s/%s", dir, IMAGE);
    if (reader != NULL && reader->open(dir)) {
        use_reader = 1;
    } else if (has_file(dir, IMAGE, ROM_SIZE)) {
        image = fopen(path, "rb");
        if (image == NULL) {
            return 0;
        }
        setvbuf(image, NULL, _IONBF, 0);
    } else if (!open_files(dir)) {
        close_files();
        return 0;
    }
    is_open = 1;
    return 1;
}

void naomi_rom_close(void) {
    size_t i;
    if (use_reader) {
        reader->close();
        use_reader = 0;
    }
    if (image != NULL) {
        fclose(image);
        image = NULL;
    }
    close_files();
    for (i = 0; i < CACHE_BLOCKS; i++) {
        free(slot[i].data);
        slot[i].data = NULL;
        slot[i].block = -1;
    }
    for (i = 0; i < ROM_BLOCKS; i++) {
        slot_of[i] = -1;
    }
    is_open = 0;
}

/* 0 past the end */
int naomi_rom_read(uint32_t offset, void* dst, uint32_t size) {
    uint8_t* d = dst;
    if (!is_open || offset >= ROM_SIZE || size > ROM_SIZE - offset) {
        return 0;
    }
    while (size > 0) {
        uint32_t in = offset & (BLOCK_SIZE - 1), n = BLOCK_SIZE - in;
        const uint8_t* p = block(offset >> BLOCK_SHIFT, (offset + size - 1) >> BLOCK_SHIFT);
        if (n > size) {
            n = size;
        }
        if (p == NULL) {
            return 0;
        }
        memcpy(d, p + in, n);
        d += n;
        offset += n;
        size -= n;
    }
    return 1;
}

uint32_t naomi_rom_size(void) {
    return ROM_SIZE;
}
