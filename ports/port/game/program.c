/*
 * program.c  The one name a program's relocatable keeps (PROGRAM: arc_program or dc_program): what the host
 * takes from it, the game's main() and the relocations of its data
 */

#include "naomi_host.h"

extern void naomi_main(void);       /* the game's main(), renamed */
extern char span_data_begin[], span_data_end[], span_bss_begin[], span_bss_end[];

const HOST_PROGRAM PROGRAM = { naomi_main, host_reloc,
                               { span_data_begin, span_data_end, span_bss_begin, span_bss_end } };
