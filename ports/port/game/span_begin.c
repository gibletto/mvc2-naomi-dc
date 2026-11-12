/*
 * span_begin.c  The first data of a relocatable (a program, or the host's layers): with span_end.c linked
 * last, its names bracket the relocatable's .data and .bss, which are kept as they start so that it can
 * be started again (port/sys/restart.c)
 */

char span_data_begin[4] = { 1 };
char span_bss_begin[4];
