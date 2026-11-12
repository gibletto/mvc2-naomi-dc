/*
 * dc_vmu.c  The VMU in port A's first socket (Shinobi's backup library), kept as its 128 KB flash image
 * beside the executable, as emulators keep one. The game's callback is told at the next frame.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "naomi_host.h"
#include "types.h"

enum {
    BUD_OP_MOUNT = 1, BUD_OP_DEFRAGDISK = 9, BUD_OP_LOADFILE = 11, BUD_OP_SAVEFILE = 12, BUD_OP_SAVEEXECFILE = 13,
    BUD_OP_LOADFILEEX = 14, BUD_OP_CONNECT = 18, BUD_OP_REWRITEEXECFILE = 20,
};
#define BUD_ERR_OK 0
#define BUD_ERR_INVALID_PARAM (-256)
#define BUD_ERR_NO_DISK (-255)
#define BUD_ERR_DISK_FULL (-252)
#define BUD_ERR_FILE_NOT_FOUND (-251)
#define BUD_ERR_EXECFILE_EXIST (-247)
#define BUD_ERR_BUPFILE_CRC (-224)
#define BUD_ERR_BUPFILE_ILLEGAL (-223)
#define BUD_CAPACITY_128KB 15       /* the card's size, in the units buInit's capacity has */

/* the VMU's flash: 256 blocks of 512 bytes */
enum {
    BLOCK = 512, BLOCKS = 256, ROOT = 255, FAT = 254, DIR = 253, DIR_BLOCKS = 13, USER_BLOCKS = 200,
    ENTRY = 32, ENTRIES = DIR_BLOCKS * BLOCK / ENTRY,
};
#define FAT_FREE 0xFFFC
#define FAT_END 0xFFFA
#define TYPE_DATA 0x33
#define TYPE_GAME 0xCC

typedef struct {
    Uint16 year;
    Uint8 month, day, hour, minute, second, dayofweek;
} BUS_TIME;

typedef struct {
    Uint32 filesize;
    Uint16 blocks;
    Uint8 type;         /* 0 a data file, 1 the executable */
    Uint8 copyflag;
    Uint16 headerofs;
    BUS_TIME time;
} BUS_FILEINFO;

/* a file's header, as the game describes it to buMakeBackupFileImage */
typedef struct {
    char vms_comment[18];
    char btr_comment[34];
    Uint8 game_name[16];
    void* icon_palette;
    void* icon_data;
    Uint16 icon_num;
    Uint16 icon_speed;
    void* visual_data;
    Uint16 visual_type;
    Uint16 reserved;
    void* save_data;
    Uint32 save_size;
} BUS_BACKUPFILEHEADER;

static Uint8 card[BLOCKS * BLOCK];
static int present, keep;   /* a card is in; it is saved to its file */
static char path[1100];
static int (*complete)(int, int, int, int);

/* what is done, told to the game at the next frame */
static struct {
    int drive, op, status;
} done[16];
static int ndone;

static Uint16 get16(const Uint8* p) {
    return p[0] | p[1] << 8;
}

static void put16(Uint8* p, unsigned v) {
    p[0] = v & 0xFF;
    p[1] = v >> 8;
}

static Uint8* block(int b) {
    return card + b * BLOCK;
}

static Uint16 fat(int b) {
    return get16(block(FAT) + b * 2);
}

static void set_fat(int b, unsigned v) {
    put16(block(FAT) + b * 2, v);
}

static Uint8 bcd(int v) {
    return (Uint8)(v / 10 * 16 + v % 10);
}

static int unbcd(Uint8 v) {
    return v / 16 * 10 + v % 16;
}

static void bcd_time(Uint8* p, const BUS_TIME* t) {
    p[0] = bcd(t->year / 100);
    p[1] = bcd(t->year % 100);
    p[2] = bcd(t->month);
    p[3] = bcd(t->day);
    p[4] = bcd(t->hour);
    p[5] = bcd(t->minute);
    p[6] = bcd(t->second);
    p[7] = bcd(t->dayofweek);
}

static void now(BUS_TIME* t) {
    struct tm clock, *tm = &clock;
    host_clock(&clock);
    t->year = (Uint16)(tm->tm_year + 1900);
    t->month = (Uint8)(tm->tm_mon + 1);
    t->day = (Uint8)tm->tm_mday;
    t->hour = (Uint8)tm->tm_hour;
    t->minute = (Uint8)tm->tm_min;
    t->second = (Uint8)tm->tm_sec;
    t->dayofweek = (Uint8)tm->tm_wday;
}

/* a blank card, as the console formats one */
static void format(void) {
    Uint8* root = block(ROOT);
    BUS_TIME t;
    int b;
    memset(card, 0, sizeof card);
    memset(root, 0x55, 16);
    now(&t);
    bcd_time(root + 0x30, &t);
    put16(root + 0x46, FAT);
    put16(root + 0x48, 1);
    put16(root + 0x4A, DIR);
    put16(root + 0x4C, DIR_BLOCKS);
    put16(root + 0x50, USER_BLOCKS);
    for (b = 0; b < BLOCKS; b++) {
        set_fat(b, FAT_FREE);
    }
    set_fat(ROOT, FAT_END);
    set_fat(FAT, FAT_END);
    for (b = DIR; b > DIR - DIR_BLOCKS + 1; b--) {
        set_fat(b, b - 1);
    }
    set_fat(DIR - DIR_BLOCKS + 1, FAT_END);
}

static void save_card(void) {
    char tmp[1110];
    FILE* f;
    if (!keep) {
        return;
    }
    snprintf(tmp, sizeof tmp, "%s.new", path);
    f = fopen(tmp, "wb");
    if (f == NULL || fwrite(card, sizeof card, 1, f) != 1) {
        host_log("could not save the memory card to %s", tmp);
        if (f != NULL) {
            fclose(f);
        }
        return;
    }
    fclose(f);
    remove(path);
    rename(tmp, path);
}

/* the directory's entries, from block 253 down */
static Uint8* entry(int i) {
    return block(DIR - i / (BLOCK / ENTRY)) + i % (BLOCK / ENTRY) * ENTRY;
}

static Uint8* find(const char* name) {
    int i;
    char want[12];
    memset(want, ' ', sizeof want);
    memcpy(want, name, strlen(name) < 12 ? strlen(name) : 12);
    for (i = 0; i < ENTRIES; i++) {
        Uint8* e = entry(i);
        if ((e[0] == TYPE_DATA || e[0] == TYPE_GAME) && memcmp(e + 4, want, 12) == 0) {
            return e;
        }
    }
    return NULL;
}

static Uint8* find_exec(void) {
    int i;
    for (i = 0; i < ENTRIES; i++) {
        if (entry(i)[0] == TYPE_GAME) {
            return entry(i);
        }
    }
    return NULL;
}

static int free_blocks(void) {
    int b, n = 0;
    for (b = 0; b < USER_BLOCKS; b++) {
        n += fat(b) == FAT_FREE;
    }
    return n;
}

/* the blocks free from block 0 up, from block "from" (past an executable file's own) */
static int free_from_0(int from) {
    int b;
    for (b = from; b < USER_BLOCKS && fat(b) == FAT_FREE; b++) {
    }
    return b;
}

/* the blocks an executable file could have, if there is none yet */
static int exec_free(void) {
    return find_exec() != NULL ? 0 : free_from_0(0);
}

static void free_chain(int b) {
    while (b < BLOCKS && fat(b) != FAT_FREE) {
        int next = fat(b);
        set_fat(b, FAT_FREE);
        if (next == FAT_END) {
            break;
        }
        b = next;
    }
}

/* the file's n'th block */
static int file_block(const Uint8* e, int n) {
    int b = get16(e + 2);
    while (n-- > 0 && b < BLOCKS) {
        b = fat(b);
    }
    return b < BLOCKS ? b : -1;
}

static void tell(int drive, int op, int status) {
    if (ndone < (int)(sizeof done / sizeof done[0])) {
        done[ndone].drive = drive;
        done[ndone].op = op;
        done[ndone].status = status;
        ndone++;
    }
}

static void bu_server(void) {
    int i, n = ndone;
    ndone = 0;
    for (i = 0; i < n && complete != NULL; i++) {
        complete(done[i].drive, done[i].op, done[i].status, 0);
    }
}

static int card_in(int drive) {
    return drive == 0 && present;
}

/* ---- the library's calls ---- */

int buInit(int capacity, int drives, void* work, void (*func)(void)) {
    FILE* f;
    (void)capacity;
    (void)work;
    ndone = 0;
    complete = NULL;
    present = (drives & 1) && (getenv("MVC2_FRAMES") == NULL || getenv("MVC2_VMU") != NULL);
    keep = present;
    format();
    if (keep) {
        snprintf(path, sizeof path, "%s",
                 getenv("MVC2_FRAMES") != NULL ? getenv("MVC2_VMU") : host_save_path("vmu_save_A1.bin"));
        f = fopen(path, "rb");
        if (f != NULL) {
            if (fread(card, sizeof card, 1, f) != 1) {
                host_log("%s is not a 128 KB memory card; a blank one is used", path);
                format();
                keep = 0;   /* leave it as it is */
            }
            fclose(f);
        }
    }
    board_at_frame(bu_server);
    if (func != NULL) {
        func();
    }
    if (present) {
        tell(0, BUD_OP_CONNECT, BUD_CAPACITY_128KB);
    }
    return BUD_ERR_OK;
}

int buExit(void) {
    board_at_frame(NULL);
    return BUD_ERR_OK;
}

int buSetCompleteCallback(int (*func)(int, int, int, int)) {
    complete = func;
    return BUD_ERR_OK;
}

int buMountDisk(int drive, void* work, int size) {
    (void)work;
    (void)size;
    tell(drive, BUD_OP_MOUNT, card_in(drive) ? BUD_ERR_OK : BUD_ERR_NO_DISK);
    return BUD_ERR_OK;
}

int buUnmount(int drive) {
    return card_in(drive) ? BUD_ERR_OK : BUD_ERR_NO_DISK;
}

int buIsExistFile(int drive, const char* name) {
    if (!card_in(drive)) {
        return BUD_ERR_NO_DISK;
    }
    return find(name) != NULL ? BUD_ERR_OK : BUD_ERR_FILE_NOT_FOUND;
}

int buFindExecFile(int drive, char* name) {
    Uint8* e;
    int n;
    if (!card_in(drive)) {
        return BUD_ERR_NO_DISK;
    }
    e = find_exec();
    if (e == NULL) {
        return BUD_ERR_FILE_NOT_FOUND;
    }
    memcpy(name, e + 4, 12);
    for (n = 12; n > 0 && name[n - 1] == ' '; n--) {
    }
    name[n] = 0;
    return BUD_ERR_OK;
}

int buGetFileInfo(int drive, const char* name, BUS_FILEINFO* info) {
    const Uint8* e;
    if (!card_in(drive)) {
        return BUD_ERR_NO_DISK;
    }
    if ((e = find(name)) == NULL) {
        return BUD_ERR_FILE_NOT_FOUND;
    }
    info->blocks = get16(e + 0x18);
    info->filesize = info->blocks * BLOCK;
    info->type = e[0] == TYPE_GAME;
    info->copyflag = e[1];
    info->headerofs = get16(e + 0x1A);
    info->time.year = (Uint16)(unbcd(e[0x10]) * 100 + unbcd(e[0x11]));
    info->time.month = (Uint8)unbcd(e[0x12]);
    info->time.day = (Uint8)unbcd(e[0x13]);
    info->time.hour = (Uint8)unbcd(e[0x14]);
    info->time.minute = (Uint8)unbcd(e[0x15]);
    info->time.second = (Uint8)unbcd(e[0x16]);
    info->time.dayofweek = (Uint8)unbcd(e[0x17]);
    return BUD_ERR_OK;
}

int buGetFileSize(int drive, const char* name) {
    const Uint8* e;
    if (!card_in(drive)) {
        return BUD_ERR_NO_DISK;
    }
    e = find(name);
    return e != NULL ? get16(e + 0x18) : BUD_ERR_FILE_NOT_FOUND;
}

int buGetDiskFree(int drive, int type) {
    if (!card_in(drive)) {
        return BUD_ERR_NO_DISK;
    }
    return type == 1 ? exec_free() : free_blocks();
}

/* blocks start to start + n of a file into buf (n 0: to its end) */
static int load(int drive, const char* name, void* buf, int start, int n) {
    const Uint8* e;
    int i, blocks;
    if (!card_in(drive)) {
        return BUD_ERR_NO_DISK;
    }
    if ((e = find(name)) == NULL) {
        return BUD_ERR_FILE_NOT_FOUND;
    }
    blocks = get16(e + 0x18);
    if (n <= 0 || start + n > blocks) {
        n = blocks - start;
    }
    for (i = 0; i < n; i++) {
        int b = file_block(e, start + i);
        if (b < 0) {
            return BUD_ERR_FILE_NOT_FOUND;
        }
        memcpy((Uint8*)buf + i * BLOCK, block(b), BLOCK);
    }
    return BUD_ERR_OK;
}

int buLoadFile(int drive, const char* name, void* buf, int nblock) {
    tell(drive, BUD_OP_LOADFILE, load(drive, name, buf, 0, nblock));
    return BUD_ERR_OK;
}

int buLoadFileEx(int drive, const char* name, void* buf, int start, int nblock) {
    tell(drive, BUD_OP_LOADFILEEX, load(drive, name, buf, start, nblock));
    return BUD_ERR_OK;
}

/* a file written: a data file in free blocks from the top down, the executable from block 0 up */
static int save(int drive, const char* name, const void* buf, int n, const BUS_TIME* t, Uint32 flag, int exec) {
    Uint8* e;
    int i, b, prev, had = 0;
    if (!card_in(drive)) {
        return BUD_ERR_NO_DISK;
    }
    if (n <= 0 || n > USER_BLOCKS) {
        return BUD_ERR_INVALID_PARAM;
    }
    e = find(name);
    if (e != NULL && (e[0] == TYPE_GAME) != exec) {
        return BUD_ERR_INVALID_PARAM;
    }
    if (exec && e == NULL && find_exec() != NULL) {
        return BUD_ERR_EXECFILE_EXIST;
    }
    if (e != NULL) {
        had = get16(e + 0x18);
    }
    if ((exec ? (e != NULL ? free_from_0(had) : exec_free()) : free_blocks() + had) < n) {
        return BUD_ERR_DISK_FULL;
    }
    if (e != NULL) {
        free_chain(get16(e + 2));
    } else {
        for (i = 0; i < ENTRIES && (entry(i)[0] == TYPE_DATA || entry(i)[0] == TYPE_GAME); i++) {
        }
        if (i == ENTRIES) {
            return BUD_ERR_DISK_FULL;
        }
        e = entry(i);
    }
    if (exec) {
        for (b = 0; b < n; b++) {
            set_fat(b, b + 1 < n ? b + 1 : FAT_END);
            memcpy(block(b), (const Uint8*)buf + b * BLOCK, BLOCK);
        }
        put16(e + 2, 0);
    } else {
        for (b = USER_BLOCKS - 1, prev = -1, i = 0; i < n; b--) {
            if (fat(b) != FAT_FREE) {
                continue;
            }
            if (prev < 0) {
                put16(e + 2, b);
            } else {
                set_fat(prev, b);
            }
            set_fat(b, FAT_END);
            memcpy(block(b), (const Uint8*)buf + i * BLOCK, BLOCK);
            prev = b;
            i++;
        }
    }
    e[0] = exec ? TYPE_GAME : TYPE_DATA;
    e[1] = flag & 0xFF;    /* the copy flag: 0xFF, not to be copied */
    memset(e + 4, ' ', 12);
    memcpy(e + 4, name, strlen(name) < 12 ? strlen(name) : 12);
    bcd_time(e + 0x10, t);
    put16(e + 0x18, n);
    put16(e + 0x1A, exec ? 1 : 0);
    memset(e + 0x1C, 0, 4);
    save_card();
    return BUD_ERR_OK;
}

int buSaveFile(int drive, const char* name, void* buf, int nblock, void* time, Uint32 flag) {
    tell(drive, BUD_OP_SAVEFILE, save(drive, name, buf, nblock, time, flag, 0));
    return BUD_ERR_OK;
}

int buSaveExecFile(int drive, const char* name, void* buf, int nblock, void* time, Uint32 flag) {
    tell(drive, BUD_OP_SAVEEXECFILE, save(drive, name, buf, nblock, time, flag, 1));
    return BUD_ERR_OK;
}

/* blocks start to start + n of the executable file written over */
int buRewriteExecFile(int drive, const char* name, void* buf, int start, int nblock) {
    const Uint8* e;
    int i, status = BUD_ERR_OK;
    if (!card_in(drive)) {
        status = BUD_ERR_NO_DISK;
    } else if ((e = find(name)) == NULL || e[0] != TYPE_GAME) {
        status = BUD_ERR_FILE_NOT_FOUND;
    } else {
        int blocks = get16(e + 0x18);
        if (nblock <= 0 || start + nblock > blocks) {
            nblock = blocks - start;
        }
        for (i = 0; i < nblock; i++) {
            memcpy(block(file_block(e, start + i)), (const Uint8*)buf + i * BLOCK, BLOCK);
        }
        save_card();
    }
    tell(drive, BUD_OP_REWRITEEXECFILE, status);
    return BUD_ERR_OK;
}

/* the data files moved to the top, so that the free blocks follow block 0 or the executable file */
static void defrag(void) {
    static Uint8 old[BLOCKS * BLOCK];
    int i, b, top = USER_BLOCKS - 1;
    memcpy(old, card, sizeof old);
    for (i = 0; i < ENTRIES; i++) {
        if (entry(i)[0] == TYPE_DATA) {
            free_chain(get16(entry(i) + 2));
        }
    }
    for (i = 0; i < ENTRIES; i++) {
        Uint8* e = entry(i);
        int n = get16(e + 0x18), from = get16(e + 2);
        if (e[0] != TYPE_DATA) {
            continue;
        }
        put16(e + 2, top);
        for (b = 0; b < n && from < BLOCKS; b++, top--) {
            memcpy(block(top), old + from * BLOCK, BLOCK);
            set_fat(top, b + 1 < n ? top - 1 : FAT_END);
            from = get16(old + FAT * BLOCK + from * 2);
        }
    }
}

int buDefragDisk(int drive, void* work) {
    (void)work;
    if (card_in(drive)) {
        defrag();
        save_card();
    }
    tell(drive, BUD_OP_DEFRAGDISK, card_in(drive) ? BUD_ERR_OK : BUD_ERR_NO_DISK);
    return BUD_ERR_OK;
}

/* ---- a file's image: its header (the card menu's comments, the icons), then the game's data ---- */

static Uint16 crc16(unsigned crc, const Uint8* p, int n) {
    int i;
    while (n-- > 0) {
        crc ^= *p++ << 8;
        for (i = 0; i < 8; i++) {
            crc = crc & 0x8000 ? (crc << 1) ^ 0x1021 : crc << 1;
        }
    }
    return (Uint16)crc;
}

/* the size of each kind of picture shown on the console's menu */
static int visual_size(int type) {
    static const int size[4] = { 0, 72 * 56 * 2, 512 + 72 * 56, 32 + 72 * 56 / 2 };
    return type >= 0 && type < 4 ? size[type] : -1;
}

int buMakeBackupFileImage(void* buf, const BUS_BACKUPFILEHEADER* hdr) {
    Uint8* p = buf;
    int visual = visual_size(hdr->visual_type);
    int at, size;
    if (visual < 0 || hdr->icon_num < 1 || hdr->icon_num > 3) {
        return BUD_ERR_INVALID_PARAM;
    }
    at = 0x80 + hdr->icon_num * 512 + visual;
    size = at + hdr->save_size;
    memset(p, 0, (size + BLOCK - 1) / BLOCK * BLOCK);
    memset(p, ' ', 0x30);
    memcpy(p, hdr->vms_comment, strnlen(hdr->vms_comment, 16));
    memcpy(p + 0x10, hdr->btr_comment, strnlen(hdr->btr_comment, 32));
    memcpy(p + 0x30, hdr->game_name, 16);
    put16(p + 0x40, hdr->icon_num);
    put16(p + 0x42, hdr->icon_speed);
    put16(p + 0x44, hdr->visual_type);
    p[0x48] = hdr->save_size & 0xFF;
    p[0x49] = hdr->save_size >> 8 & 0xFF;
    p[0x4A] = hdr->save_size >> 16 & 0xFF;
    p[0x4B] = hdr->save_size >> 24;
    memcpy(p + 0x60, hdr->icon_palette, 32);
    memcpy(p + 0x80, hdr->icon_data, hdr->icon_num * 512);
    if (visual > 0 && hdr->visual_data != NULL) {
        memcpy(p + 0x80 + hdr->icon_num * 512, hdr->visual_data, visual);
    }
    memcpy(p + at, hdr->save_data, hdr->save_size);
    put16(p + 0x46, crc16(0, p, size));
    return (size + BLOCK - 1) / BLOCK;
}

int buAnalyzeBackupFileImage(BUS_BACKUPFILEHEADER* hdr, const void* buf) {
    const Uint8* p = buf;
    Uint8 head[0x48];
    int icons = get16(p + 0x40), visual = visual_size(get16(p + 0x44)), at;
    Uint32 size = p[0x48] | p[0x49] << 8 | p[0x4A] << 16 | (Uint32)p[0x4B] << 24;
    Uint16 crc;
    if (icons < 1 || icons > 3 || visual < 0 || size > USER_BLOCKS * BLOCK) {
        return BUD_ERR_BUPFILE_ILLEGAL;
    }
    at = 0x80 + icons * 512 + visual;
    /* the CRC is over the image with its own place as 0 */
    memcpy(head, p, sizeof head);
    put16(head + 0x46, 0);
    crc = crc16(crc16(0, head, sizeof head), p + sizeof head, at + (int)size - (int)sizeof head);
    if (crc != get16(p + 0x46)) {
        return BUD_ERR_BUPFILE_CRC;
    }
    memset(hdr, 0, sizeof *hdr);
    memcpy(hdr->vms_comment, p, 16);
    memcpy(hdr->btr_comment, p + 0x10, 32);
    memcpy(hdr->game_name, p + 0x30, 16);
    hdr->icon_palette = (void*)(p + 0x60);
    hdr->icon_data = (void*)(p + 0x80);
    hdr->icon_num = (Uint16)icons;
    hdr->icon_speed = get16(p + 0x42);
    hdr->visual_type = get16(p + 0x44);
    hdr->visual_data = visual > 0 ? (void*)(p + 0x80 + icons * 512) : NULL;
    hdr->save_data = (void*)(p + at);
    hdr->save_size = size;
    return BUD_ERR_OK;
}
