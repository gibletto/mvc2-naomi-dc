/* The VMU / backup memory code (vms.c): what it keeps of each of the eight places a memory card can be in. */
#ifndef VMS_H
#define VMS_H

#include "player.h"

#define VMS_PORT_MAX	8

/* 1 where a file is not on the card, 2 where it is; 0 when not looked at */
typedef struct VMS_SLOT {
	Sint32	port;		/* 000: which place the card is in */
	Sint32	func;		/* 004: the backup library's last call that came back for it; 18 none yet */
	Sint32	result;		/* 008: how it came out; 0xffff0000 for nothing yet */
	Sint32	disk_blocks;	/* 00c: the blocks the card is mounted for, 122 by default; the mount's result */
	Sint32	unk_10;		/* 010 */
	Sint32	sys_file;	/* 014: MVLVS.C2_SYS */
	Sint32	up_file;	/* 018: MVLVS.C2__UP */
	Sint32	net_file;	/* 01c: CAP_NET_.NET */
	Sint32	dow_file;	/* 020: MVLVS.C2_DOW */
	Sint32	vm_file;	/* 024: MVLVS.C2__VM, our exec file: 1 not on the card, 2 on it */
	Sint32	other_exec;	/* 028: 2 when the card holds another game's exec file, else 1 */
	Sint32	free_blocks;	/* 02c: the card's free blocks */
	Sint32	exec_free;	/* 030: the free blocks an exec file can use; under 64 with 64 free calls a defrag */
	Uint16	year;		/* 034: the save file's date */
	Uint8	month, day, hour, minute, second, week;
} VMS_SLOT;

#endif
