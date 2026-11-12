/*
 * dc_gd.c  The Dreamcast's disc (Shinobi's gdFs) over a folder of its files, read at once in whole 2048-byte
 * sectors; a request is complete when gdFsGetStat next looks, and the drive's door never opens.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "naomi_host.h"
#include "shinobi.h"

#define GD_MAX_OPEN 16
#define GDD_DRVSTAT_PAUSE 1

struct GDS_FS_HANDLE {
    FILE* f;
    Sint32 size;
    long pos;           /* where the next sector is read from */
    Sint32 stat;
};

static char root[1024];
static struct GDS_FS_HANDLE files[GD_MAX_OPEN];
static struct GDS_FS_HANDLE sys_handle = { NULL, 0, 0, GDD_STAT_COMPLETE };

/* the USA disc is told by its program's size and a few of the files it reads first */
static const struct {
    const char* name;
    long size;
} disc_files[] = {
    { "1ST_READ.BIN", 1809817 }, { "AICADRV.BIN", 59264 },   { "FONT.BIN", 73792 },     { "SELTEX.BIN", 1591990 },
    { "PL00_DAT.BIN", 557408 },  { "PL3A_DAT.BIN", 464448 }, { "STG00POL.BIN", 173976 }, { "ADX_SELC.BIN", 417792 },
};

/* the first of those files that is missing from dir or the wrong size, or NULL when all are there */
const char* dc_disc_check(const char* dir) {
    char path[1200];
    struct stat st;
    size_t i;
    for (i = 0; i < sizeof disc_files / sizeof disc_files[0]; i++) {
        snprintf(path, sizeof path, "%s/%s", dir, disc_files[i].name);
        if (stat(path, &st) != 0 || st.st_size != disc_files[i].size) {
            return disc_files[i].name;
        }
    }
    return NULL;
}

int dc_gd_open(const char* disc_path) {
    snprintf(root, sizeof root, "%s", disc_path);
    return 1;
}

static FILE* open_file(const char* name) {
    char path[1200];
    snprintf(path, sizeof path, "%s/%s", root, name);
    return fopen(path, "rb");
}

long dc_gd_read_file(const char* name, void* buf, long max) {
    FILE* f = open_file(name);
    long n;
    if (f == NULL) {
        return -1;
    }
    n = (long)fread(buf, 1, (size_t)max, f);
    fclose(f);
    return n;
}

Sint32 gdFsInit(Sint32 max_open, void* work, Sint32 max_dirent, void* dirbuf) {
    (void)max_open;
    (void)work;
    (void)max_dirent;
    (void)dirbuf;
    return GDD_ERR_OK;
}

void gdFsFinish(void) {
    int i;
    for (i = 0; i < GD_MAX_OPEN; i++) {
        if (files[i].f != NULL) {
            fclose(files[i].f);
            files[i].f = NULL;
        }
    }
}

GDFS gdFsOpen(const char* fname, void* dirrec) {
    int i;
    (void)dirrec;
    for (i = 0; i < GD_MAX_OPEN; i++) {
        if (files[i].f == NULL) {
            GDFS gd = &files[i];
            gd->f = open_file(fname);
            if (gd->f == NULL) {
                host_log("gdFsOpen: no %s on the disc", fname);
                return NULL;
            }
            fseek(gd->f, 0, SEEK_END);
            gd->size = (Sint32)ftell(gd->f);
            if (getenv("MVC2_TRACE_GD")) {
                host_log("gd %s %X (frame %u)", fname, gd->size, host_frame());
            }
            gd->pos = 0;
            gd->stat = GDD_STAT_IDLE;
            return gd;
        }
    }
    host_log("gdFsOpen: more than %d files open", GD_MAX_OPEN);
    return NULL;
}

void gdFsClose(GDFS gd) {
    if (gd != NULL && gd->f != NULL) {
        fclose(gd->f);
        gd->f = NULL;
    }
}

Sint32 gdFsGetFileSize(GDFS gd, Sint32* fsize) {
    if (gd == NULL || gd->f == NULL) {
        return 0;
    }
    *fsize = gd->size;
    return 1;
}

static void read_sectors(GDFS gd, Sint32 nsct, void* buf) {
    size_t want = (size_t)nsct * GDD_SCT_SIZE, got = 0;
    if (gd->pos < gd->size) {
        fseek(gd->f, gd->pos, SEEK_SET);
        got = fread(buf, 1, want, gd->f);
    }
    memset((char*)buf + got, 0, want - got);
    gd->pos += (long)want;
    gd->stat = GDD_STAT_COMPLETE;
}

Sint32 gdFsRead(GDFS gd, Sint32 nsct, void* buf) {
    if (gd == NULL || gd->f == NULL) {
        return -1;
    }
    read_sectors(gd, nsct, buf);
    return GDD_ERR_OK;
}

Sint32 gdFsReqRd32(GDFS gd, Sint32 nsct, void* buf) {
    if (gd == NULL || gd->f == NULL) {
        return -1;
    }
    read_sectors(gd, nsct, buf);
    return nsct;
}

/* moves to a sector: from the file's start (origin 0), from where it is (1) or from its end (2) (the ADX player) */
Sint32 gdFsSeek(GDFS gd, Sint32 offset, Sint32 origin) {
    long base;
    if (gd == NULL || gd->f == NULL) {
        return -1;
    }
    base = origin == 1 ? gd->pos : origin == 2 ? (gd->size + GDD_SCT_SIZE - 1) / GDD_SCT_SIZE * GDD_SCT_SIZE : 0;
    gd->pos = base + (long)offset * GDD_SCT_SIZE;
    return GDD_ERR_OK;
}

Sint32 gdFsGetStat(GDFS gd) {
    return gd != NULL ? gd->stat : GDD_STAT_IDLE;
}

GDFS gdFsGetSysHn(void) {
    return &sys_handle;
}

Sint32 gdFsGetDrvStat(void) {
    return GDD_DRVSTAT_PAUSE;
}

Sint32 gdFsReqDrvStat(void) {
    return GDD_ERR_OK;
}

void gdFsEntryErrFuncAll(GDFS_ERRFUNC func, void* obj) {
    (void)func;
    (void)obj;
}
