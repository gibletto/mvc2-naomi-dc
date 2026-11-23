/*
 * naomi_rom.c  The cartridge's ROM as one address space, as MAME's naomi.cpp lays out the mvsc2 set
 *
 * ic11 at 0, the mask ROMs from 0x800000 8 MB each, then ic31 again; from 0x8000000 ic31 and ic32s as one
 * 32-bit wide ROM; 0x400000 mirrors 0x1200000. Read 64 KB blocks at a time through a cache. The files
 * can be in MAME's zip of the set instead, which is unpacked once into an image of the address space.
 * On the MiSTer the files (or the image) are mapped: the kernel's page cache holds the blocks, and our cache only
 * the 32-bit wide ROM's, which are put together.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#ifdef _WIN32
#include <direct.h>
#define make_dir(d) _mkdir(d)
#else
#define make_dir(d) mkdir(d, 0777)
#endif

#ifdef MVC2_MISTER
#define ROM_MAPPED
#include <sys/mman.h>
#endif

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
#define IC32_NAME "mpr-23084.ic32s"
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
#define ZIP "mvsc2.zip"

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
#ifdef ROM_MAPPED
static const uint8_t* mapped[NFILES + 1];   /* each file's, or NULL */
static const uint8_t* mapped_image;
#endif

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

static void fill_block(uint32_t b, uint32_t last, uint8_t* dst) {
    if (use_reader) {
        if (!reader->read(b, last, dst)) {
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

/* a free slot, else the least recently read; the cache stops growing where memory runs out */
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
    if (slot[s].data == NULL && (slot[s].data = host_try_malloc(BLOCK_SIZE)) == NULL) {
        /* the slots before s all hold blocks */
        if (s < MIN_BLOCKS) {
            host_out_of_memory("the ROM cache's block", BLOCK_SIZE);
            return NULL;
        }
        host_log("ROM cache: no memory for more than %d KB of its %d KB", s << (BLOCK_SHIFT - 10),
                 cache_blocks << (BLOCK_SHIFT - 10));
        cache_blocks = s;
        return take_slot(b);
    }
    if (slot[s].block >= 0) {
        slot_of[slot[s].block] = -1;
    }
    slot[s].block = (int32_t)b;
    slot[s].used = ++clock_now;
    slot_of[b] = (int16_t)s;
    return slot[s].data;
}

#ifdef ROM_MAPPED
static const uint8_t* map_file(FILE* f, uint32_t size) {
    void* p = mmap(NULL, size, PROT_READ, MAP_PRIVATE, fileno(f), 0);
    return p != MAP_FAILED ? p : NULL;
}

/* block b where it is mapped as it is, else NULL (the wide ROM's, past the files, a file not mapped) */
static const uint8_t* mapped_block(uint32_t b) {
    uint32_t at = b << BLOCK_SHIFT;
    size_t i;
    if (mapped_image != NULL) {
        return mapped_image + at;
    }
    if (at >= MIRROR_TO && at < MIRROR_TO + MIRROR_SIZE) {
        at += MIRROR_FROM - MIRROR_TO;
    }
    for (i = 0; i < NFILES && at < WIDE_BASE; i++) {
        if (at >= files[i].offset && at < files[i].offset + files[i].size) {
            return mapped[i] != NULL ? mapped[i] + (at - files[i].offset) : NULL;
        }
    }
    return NULL;
}
#endif

/* last is the request's last block, for a reader reading ahead */
static const uint8_t* block(uint32_t b, uint32_t last) {
    int s = slot_of[b];
    uint8_t* d;
#ifdef ROM_MAPPED
    const uint8_t* m = mapped_block(b);
    if (m != NULL) {
        return m;
    }
#endif
    if (s >= 0) {
        slot[s].used = ++clock_now;
        return slot[s].data;
    }
    d = take_slot(b);
    if (d == NULL) {
        return NULL;
    }
    fill_block(b, last, d);
    if (use_reader && reader->missed != NULL) {
        reader->missed(b, last);
    }
    return d;
}

int naomi_rom_cached(uint32_t b) {
    return b < ROM_BLOCKS && slot_of[b] >= 0;
}

uint32_t naomi_rom_held(void) {
    uint32_t n = 0;
    int i;
    for (i = 0; i < CACHE_BLOCKS; i++) {
        n += slot[i].data != NULL ? BLOCK_SIZE : 0;
    }
    return n;
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

/* the first of the set's files missing from dir or the wrong size, or NULL */
static const char* missing_file(const char* dir) {
    size_t i;
    for (i = 0; i < NFILES; i++) {
        if (!has_file(dir, files[i].name, files[i].size)) {
            return files[i].name;
        }
    }
    return has_file(dir, IC32_NAME, WIDE_SIZE) ? NULL : IC32_NAME;
}

/* the set's zip, in dir or beside it */
static NAOMI_ZIP* open_zip(const char* dir) {
    char path[1100];
    size_t n = strlen(dir);
    NAOMI_ZIP* z;
    snprintf(path, sizeof path, "%s/" ZIP, dir);
    if ((z = naomi_zip_open(path)) != NULL) {
        return z;
    }
    while (n > 0 && (dir[n - 1] == '/' || dir[n - 1] == '\\')) {
        n--;
    }
    snprintf(path, sizeof path, "%.*s.zip", (int)n, dir);
    return naomi_zip_open(path);
}

static int zip_whole(const NAOMI_ZIP* z) {
    size_t i;
    for (i = 0; i < NFILES; i++) {
        if (!naomi_zip_has(z, files[i].name, files[i].size)) {
            return 0;
        }
    }
    return naomi_zip_has(z, IC32_NAME, WIDE_SIZE);
}

/* the first of the set's files missing from dir or the wrong size, or NULL (or the image or a whole zip there) */
const char* naomi_rom_check(const char* dir) {
    const char* missing;
    NAOMI_ZIP* z;
    int whole;
    if (has_file(dir, IMAGE, ROM_SIZE) || (missing = missing_file(dir)) == NULL) {
        return NULL;
    }
    z = open_zip(dir);
    whole = z != NULL && zip_whole(z);
    naomi_zip_close(z);
    return whole ? NULL : missing;
}

static int open_files(const char* dir) {
    size_t i;
    for (i = 0; i <= NFILES; i++) {
        rom_file[i] = open_file(dir, i < NFILES ? files[i].name : IC32_NAME);
        if (rom_file[i] == NULL) {
            return 0;
        }
#ifdef ROM_MAPPED
        mapped[i] = map_file(rom_file[i], i < NFILES ? files[i].size : WIDE_SIZE);
#endif
    }
    return 1;
}

static void close_files(void) {
    size_t i;
    for (i = 0; i <= NFILES; i++) {
#ifdef ROM_MAPPED
        if (mapped[i] != NULL) {
            munmap((void*)mapped[i], i < NFILES ? files[i].size : WIDE_SIZE);
            mapped[i] = NULL;
        }
#endif
        if (rom_file[i] != NULL) {
            fclose(rom_file[i]);
            rom_file[i] = NULL;
        }
    }
}

typedef void (*PROGRESS)(uint32_t done, uint32_t total);

static int from_files(const char* dir, FILE* f, PROGRESS progress) {
    uint8_t* buf = malloc(BLOCK_SIZE);
    uint32_t b;
    int ok = buf != NULL && open_files(dir);
    for (b = 0; ok && b < ROM_BLOCKS; b++) {
        fill_from_files(b, buf);
        ok = fwrite(buf, 1, BLOCK_SIZE, f) == BLOCK_SIZE;
        if (progress != NULL && (b % 32 == 31 || b == ROM_BLOCKS - 1)) {
            progress(b + 1, ROM_BLOCKS);
        }
    }
    close_files();
    free(buf);
    return ok;
}

static int put(FILE* f, const uint8_t* p, uint32_t n, uint32_t* done, PROGRESS progress) {
    *done += n;
    if (progress != NULL) {
        progress(*done >> BLOCK_SHIFT, ROM_BLOCKS);
    }
    return fwrite(p, 1, n, f) == n;
}

/* the progress through a file being read from the zip, as part of the image */
static PROGRESS zip_progress;
static uint32_t zip_done, zip_part;

static void zip_step(uint32_t done) {
    if (zip_progress != NULL) {
        zip_progress((zip_done + (uint32_t)((uint64_t)done * zip_part / 0x800000)) >> BLOCK_SHIFT, ROM_BLOCKS);
    }
}

/* name into buf, for part bytes of the image after done */
static int zip_read(NAOMI_ZIP* z, const char* name, uint8_t* buf, uint32_t size, uint32_t done, uint32_t part) {
    zip_done = done;
    zip_part = part * (0x800000 / size);
    return naomi_zip_read(z, name, buf, size, zip_step);
}

/* the image from the zip in address order, a file at a time; the mirrored file is read twice */
static int from_zip(NAOMI_ZIP* z, FILE* f, PROGRESS progress) {
    uint8_t* buf = malloc(0x800000);
    uint8_t* out = malloc(BLOCK_SIZE);
    uint32_t done = 0, at, w;
    size_t i, m = 0;
    int ok = buf != NULL && out != NULL;
    zip_progress = progress;
    while (files[m].offset + files[m].size <= MIRROR_FROM) {
        m++;
    }
    ok = ok && zip_read(z, files[0].name, buf, files[0].size, done, files[0].size)
         && put(f, buf, files[0].size, &done, progress);
    ok = ok && zip_read(z, files[m].name, buf, files[m].size, done, MIRROR_SIZE)
         && put(f, buf + (MIRROR_FROM - files[m].offset), MIRROR_SIZE, &done, progress);
    for (i = 1; ok && i < NFILES; i++) {
        ok = zip_read(z, files[i].name, buf, files[i].size, done, files[i].size)
             && put(f, buf, files[i].size, &done, progress);
    }
    /* ic31 is still in buf; from WIDE_BASE it and ic32s are one 32-bit wide ROM */
    memset(out, 0xFF, BLOCK_SIZE);
    while (ok && done < WIDE_BASE) {
        ok = put(f, out, BLOCK_SIZE, &done, progress);
    }
    ok = ok && zip_read(z, IC32_NAME, buf + WIDE_SIZE, WIDE_SIZE, done, 0);
    for (at = 0; ok && at < WIDE_SIZE; at += BLOCK_SIZE / 2) {
        for (w = 0; w < BLOCK_SIZE / 2; w += 2) {
            memcpy(out + w * 2, buf + at + w, 2);
            memcpy(out + w * 2 + 2, buf + WIDE_SIZE + at + w, 2);
        }
        ok = put(f, out, BLOCK_SIZE, &done, progress);
    }
    free(buf);
    free(out);
    return ok && done == ROM_SIZE;
}

/* the image, from the set's files in dir, else from its zip; 1 when done */
int naomi_rom_make_image(const char* dir, const char* path, PROGRESS progress) {
    char tmp[1100];
    NAOMI_ZIP* z = NULL;
    FILE* f;
    int ok;
    if (missing_file(dir) != NULL) {
        make_dir(dir);
        if ((z = open_zip(dir)) == NULL) {
            return 0;
        }
    }
    snprintf(tmp, sizeof tmp, "%s.part", path);
    f = fopen(tmp, "wb");
    ok = f != NULL && (z != NULL ? from_zip(z, f, progress) : from_files(dir, f, progress));
    naomi_zip_close(z);
    if (f != NULL && fclose(f) != 0) {
        ok = 0;
    }
    remove(path);
    if (!ok || rename(tmp, path) != 0) {
        remove(tmp);
        return 0;
    }
    return 1;
}

#ifdef MVC2_MISTER
void mister_unpacking(const char* name, uint32_t done_kb, uint32_t total_kb);  /* port/mister: on the OSD */

static void unpack_progress(uint32_t done, uint32_t total) {
    mister_unpacking(ZIP, done << (BLOCK_SHIFT - 10), total << (BLOCK_SHIFT - 10));
}
#else
#define unpack_progress NULL
#endif

const char* naomi_rom_image_name(void) {
    return IMAGE;
}

#ifdef MVC2_MISTER
/* a zip on a drive that can't take its image: the image on the SD card, in the game's own folder */
#define SD_DIR "roms/mvsc2"
static int unpack_failed;

int naomi_rom_unpack_failed(void) {
    return unpack_failed;
}
#endif

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
#ifdef MVC2_MISTER
    unpack_failed = 0;
    if (!has_file(dir, IMAGE, ROM_SIZE) && missing_file(dir) != NULL && has_file(SD_DIR, IMAGE, ROM_SIZE)) {
        host_log("the set's image from %s", SD_DIR);
        dir = SD_DIR;
    }
#endif
    snprintf(path, sizeof path, "%s/%s", dir, IMAGE);
    if (reader != NULL && reader->open(dir)) {
        use_reader = 1;
    } else {
        if (!has_file(dir, IMAGE, ROM_SIZE) && missing_file(dir) != NULL) {
            host_log("unpacking the set's zip into %s", path);
            if (!naomi_rom_make_image(dir, path, unpack_progress)) {
                host_log("could not unpack the set's zip into %s", path);
#ifdef MVC2_MISTER
                if (strcmp(dir, SD_DIR) != 0) {
                    make_dir("roms");
                    make_dir(SD_DIR);
                    snprintf(path, sizeof path, "%s/%s", SD_DIR, IMAGE);
                    host_log("unpacking the set's zip into %s", path);
                    if (naomi_rom_make_image(dir, path, unpack_progress)) {
                        dir = SD_DIR;
                    } else {
                        host_log("could not unpack the set's zip into %s", path);
                    }
                }
                unpack_failed = !has_file(dir, IMAGE, ROM_SIZE);
#endif
            }
        }
        if (has_file(dir, IMAGE, ROM_SIZE)) {
            image = fopen(path, "rb");
            if (image == NULL) {
                return 0;
            }
            setvbuf(image, NULL, _IONBF, 0);
#ifdef ROM_MAPPED
            mapped_image = map_file(image, ROM_SIZE);
#endif
        } else if (!open_files(dir)) {
            close_files();
            return 0;
        }
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
#ifdef ROM_MAPPED
        if (mapped_image != NULL) {
            munmap((void*)mapped_image, ROM_SIZE);
            mapped_image = NULL;
        }
#endif
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
