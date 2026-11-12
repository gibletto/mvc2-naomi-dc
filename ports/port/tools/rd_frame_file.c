/*
 * rd_frame_file.c  Frames saved with MVC2_RDSAVE, read back for render_bench and gpu_bench
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rd.h"
#include "rd_list.h"
#include "rd_frame_file.h"

int rd_frame_load(const char* name) {
    FILE* f = fopen(name, "rb");
    char magic[4];
    RD_TEXTURE* tex;
    int* texno;
    int i, k, ntex, ok = 1;
    if (f == NULL) {
        return 0;
    }
    ok &= fread(magic, 1, 4, f) == 4 && memcmp(magic, "RDF1", 4) == 0;
    ok &= fread(rd_frame.ntris, sizeof(int), RD_LISTS, f) == RD_LISTS;
    ok &= fread(&rd_frame.nstates, sizeof(int), 1, f) == 1;
    for (i = 0; ok && i < RD_LISTS; i++) {
        ok &= fread(rd_frame.tris[i], sizeof(RD_TRI), (size_t)rd_frame.ntris[i], f) == (size_t)rd_frame.ntris[i];
    }
    ok &= fread(rd_frame.background, sizeof rd_frame.background, 1, f) == 1;
    ok &= fread(&rd_fog, sizeof rd_fog, 1, f) == 1;
    ok &= fread(rd_palette, sizeof rd_palette, 1, f) == 1;
    texno = malloc(sizeof(int) * (size_t)(rd_frame.nstates + 1));
    for (i = 0; ok && i < rd_frame.nstates; i++) {
        ok &= fread(&rd_frame.states[i], sizeof(RD_STATE), 1, f) == 1;
        ok &= fread(&texno[i], sizeof(int), 1, f) == 1;
    }
    ok &= fread(&ntex, sizeof ntex, 1, f) == 1;
    tex = calloc((size_t)(ntex + 1), sizeof *tex);
    for (i = 0; ok && i < ntex; i++) {
        RD_TEXTURE* t = &tex[i];
        int bytes;
        ok &= fread(&t->width, sizeof(int), 1, f) == 1 && fread(&t->height, sizeof(int), 1, f) == 1;
        ok &= fread(&t->format, sizeof(int), 1, f) == 1 && fread(&t->levels, sizeof(int), 1, f) == 1;
        bytes = t->format == RD_PAL4 || t->format == RD_PAL8 ? 1 : 2;
        for (k = 0; ok && k <= t->levels; k++) {
            int w = t->width >> k, h = t->height >> k;
            size_t n = (size_t)(w < 1 ? 1 : w) * (h < 1 ? 1 : h);
            void* p = malloc(n * (size_t)bytes);
            ok &= p != NULL && fread(p, (size_t)bytes, n, f) == n;
            if (k == 0) {
                t->texels = p;
            } else {
                t->mip[k] = p;
            }
        }
    }
    for (i = 0; ok && i < rd_frame.nstates; i++) {
        rd_frame.states[i].tex = texno[i] >= 0 && texno[i] < ntex ? &tex[texno[i]] : NULL;
    }
    free(texno);
    fclose(f);
    return ok;
}

void rd_frame_write_ppm(const char* name, const uint32_t* frame) {
    FILE* f = fopen(name, "wb");
    int i;
    if (f == NULL) {
        return;
    }
    fprintf(f, "P6 %d %d 255\n", RD_WIDTH, RD_HEIGHT);
    for (i = 0; i < RD_WIDTH * RD_HEIGHT; i++) {
        fputc((int)(frame[i] >> 16 & 255), f), fputc((int)(frame[i] >> 8 & 255), f), fputc((int)(frame[i] & 255), f);
    }
    fclose(f);
}
