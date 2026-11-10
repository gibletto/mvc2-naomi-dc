/* the book-keeping kept in the backup memory (system.c, vms.c) */
#ifndef BOOK_H
#define BOOK_H

#define BOOK_ID	0x42424730	/* "0GBB" */
typedef struct BOOK_WORK {
	Sint32	id;		/* 000: BOOK_ID */
	Sint32	coin_all;	/* 004: the coins put in */
	Sint32	service_all;	/* 008: the service credits */
	Sint32	play_all;	/* 00c: the games played */
	Sint32	coin;		/* 010: the coins since the last clear */
	Sint32	service;	/* 014: the service credits since then */
	Sint32	play;		/* 018: the games played since then */
	Sint32	point;		/* 01c: vms_work's points */
	Sint32	char_open[2];	/* 020: Sys->char_open0 and 1, the characters opened, a bit each */
	Sint32	char_open_all[2];	/* 028: Sys->char_open_all0 and 1, the characters opened again by char_open_all */
	Uint32	play_time;	/* 030: vms_work's time played */
	Uint8	unk_34[0x40 - 0x34];
} BOOK_WORK;

extern void point_lv_set( void ) ;

#endif
