/* The select scene's work, shared by select.c and the select code at 0C1C4000. */
#ifndef SELECT_H
#define SELECT_H

typedef struct SEL_WORK {
	Uint8	work_step[2];	/* 000: each side's step as its works report it: 1 faces in, 2 door ready, 3 member done, 4 speed door ready, 5 speed door gone */
	Uint8	pick_flag[2];	/* 002: each side's picks: bit n once member n's character is chosen, bit 4+n once its assist type is; 0x77 a whole team */
	Sint8	cur_col[2];	/* 004: each side's cursor, its column in the grid (0 to 7, wraps) */
	Sint8	cur_row[2];	/* 006: and its row (0 to 7, wraps) */
	Sint8	end_flag[2];	/* 008: each side's flags: 0x80 once its third member is chosen */
	Uint8	card_flag;	/* 00a: the memory card flags: side_bit (1, 2) while a side's card is read, 4 or 8 once its read has begun */
	Uint8	ready_flag;	/* 00b: 1 once the portraits are in place, 2 once the opening is over */
	Uint8	unk_0c;		/* 00c */
	Uint8	same_flag;	/* 00d: nonzero: every side may take the same character; only ever cleared */
	Uint8	unk_0e[0x10 - 0x0e];
	Uint32	char_open[2][2];	/* 010: each side's mask of characters that can be chosen, a word for 0-31 and one for 32 and up */
	Uint32	char_free[2][2];	/* 020: each side's characters still free to take: char_open less those its members have taken */
	Uint8	cur_char[2];	/* 030: the character each side's cursor is on, plus 1 (0 none) */
	Uint8	speed_flag;	/* 032: 1 once the speed is chosen or not asked for, 0 while it is being chosen */
	Uint8	join_flag;	/* 033: a bit for each side on the screen's layout (3 both), which places the names, cursors and back */
	Uint8	pick_no[2];	/* 034: the member each side is choosing, plus 1 (0 none yet) */
	Uint8	cur_go_flag;	/* 036: a bit for each side while its cursor's pieces fly off after a pick */
	Sint8	cpu_side;	/* 037: the computer's side in a one-player select */
	Uint8	heart_flag;	/* 038: 1 once a new challenger has come in; the scene then only plays Sound_Heart */
	Uint8	unk_39[0x3c - 0x39];
	Uint32	char_ex[2][2];	/* 03c: each side's mask of characters opened again: they get the extra colours and a piece on their cell */
	Sint16	sel_timer;	/* 04c: the select's time in frames, 0xe10 at the start; at 0 each side takes what its cursor is on */
	Uint8	unk_4e[0x50 - 0x4e];
	Sint8	card_side;	/* 050: the sides with a memory card in, a bit each, this frame */
	Sint8	card_side_old;	/* 051: card_side of the frame before */
	Uint8	unk_52[0x58 - 0x52];
} SEL_WORK;
extern SEL_WORK sel_w;
extern PLAYER_WORK *sel_back_w;

typedef struct EF05_CELL {
	Uint8	col, row;
} EF05_CELL;
extern const EF05_CELL ef05_cell_tbl[60];	/* ef05.c: by character, a portrait's turn and its picture's size */
extern VEC3 ef05_pos_tbl[59];	/* ef05.c: where each portrait stands */
extern void ef05_rot_set( PLAYER_WORK *ew, Uint8 no );

#endif
