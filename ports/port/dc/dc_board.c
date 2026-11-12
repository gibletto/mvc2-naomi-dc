/*
 * dc_board.c  The Dreamcast as the game sees it: its program from the disc, and the NAOMI library's
 * Dreamcast entries
 *
 * The console's boot ROM reads 1ST_READ.BIN to 8C010000. The program is linked there, in the P1
 * mirror of main RAM, and the port maps main RAM at 0C000000, so the image goes to 0C010000, and
 * gen_host masks the addresses in its data the same way. The rest of the board (the frame, the
 * interrupts, the timer) is the NAOMI's, which is the same chip set.
 */

#include <stdio.h>
#include <stdlib.h>

#include "naomi_host.h"

void gdFsFinish(void);

#define DC_PROGRAM_ADDRESS 0x0C010000u
#define DC_PROGRAM_MAX 0x00400000

int dc_init(const HOST_PROGRAM* program, const char* disc_path) {
    long size;
    if (!dc_gd_open(disc_path)) {
        return 0;
    }
    size = dc_gd_read_file("1ST_READ.BIN", (void*)(uintptr_t)DC_PROGRAM_ADDRESS, DC_PROGRAM_MAX);
    if (size <= 0) {
        host_log("no 1ST_READ.BIN in %s", disc_path);
        return 0;
    }
    board_at_stop(gdFsFinish);
    board_start(program);
    return 1;
}

/* ---- the NAOMI library's Dreamcast entries ---- */

void nlInitLibrary(void);
void nlInitInterrupt(void);

/* the library's start for a video mode (VGA, NTSC or PAL): the port's screen is VGA's */
void nlInitLibraryDC(int tvmode, int fbmode, int vwait) {
    (void)tvmode;
    (void)fbmode;
    (void)vwait;
    nlInitLibrary();
}

void nlInitInterruptDC(void) {
    nlInitInterrupt();
}

/* the interrupts' own stack: the host's interrupts run on the game's */
void nlSetIntrStack(void* stack) {
    (void)stack;
}

/* the way out of the game, back to the console's menu */
void nlExitSystem(void) {
}
