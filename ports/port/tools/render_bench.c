/*
 * render_bench.c  A frame saved with MVC2_RDSAVE, drawn again and again by rd_soft.c
 *
 * render_bench FILE [TIMES] [OUT.ppm]: prints the best time a frame and writes the last one. Built by hand:
 *   gcc -O2 -Iport/render port/tools/render_bench.c port/tools/rd_frame_file.c port/render/rd.c
 *       port/render/rd_soft.c $(sdl2-config --cflags --libs) -o render_bench
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rd_sys.h"
#ifdef RD_SDL3
#include <SDL3/SDL_main.h>
#endif

#include "rd.h"
#include "rd_list.h"
#include "rd_frame_file.h"

static uint32_t frame[RD_WIDTH * RD_HEIGHT];

int main(int argc, char** argv) {
    int times = argc > 2 ? atoi(argv[2]) : 20, i;
    double start, ms, best;
    if (argc < 2 || !rd_frame_load(argv[1])) {
        fprintf(stderr, "usage: render_bench FILE [TIMES] [OUT.ppm] (a frame saved with MVC2_RDSAVE)\n");
        return 1;
    }
    rd_end_frame(frame, RD_WIDTH);      /* warm: the workers started, the pools grown */
    best = 1e30;
    for (i = 0; i < times; i++) {
        start = (double)SDL_GetPerformanceCounter();
        rd_end_frame(frame, RD_WIDTH);
        ms = ((double)SDL_GetPerformanceCounter() - start) * 1000.0 / (double)SDL_GetPerformanceFrequency();
        best = ms < best ? ms : best;
    }
    printf("%.2f ms a frame at best\n", best);
    if (argc > 3) {
        rd_frame_write_ppm(argv[3], frame);
    }
    return 0;
}
