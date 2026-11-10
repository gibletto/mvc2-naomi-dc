/* Abyss's second form's special moves. */
#ifndef PL19_DEF_H
#define PL19_DEF_H

/* pl_sp_type */
#define	pl19_sp_bubble	0	/* bubbles that hold whoever they touch */
#define	pl19_sp_storm	1	/* shots in every direction (hyper) */
#define	pl19_sp_beam	2	/* a beam, longer for pressed buttons */
#define	pl19_sp_blast	3	/* a blast at his side */
#define	pl19_sp_change	4	/* he fades, to change into the third form */

/* atck_no: the character's own records in the hit table, after the common ATCK_ ones */
#define	pl19_at_bubble	0x30	/* The bubble that holds whoever it touches (shl19_00) */
#define	pl19_at_storm	0x31	/* Shots in every direction (hyper), Abyss and each shot (shl19_02) */
#define	pl19_at_t_kick	0x32	/* Tag-in kick that leaps off the wall */
#define	pl19_at_blast_h	0x38	/* The heavy blast at his side (55 + atck_level, level 1), struck by shl19_01's mark when its type is not 0 */

/* the character's own work, in tlocal */
typedef struct PL19_LOCAL_W {
	Uint8	col_step;	/* the colour cycle's step */
	Uint8	col_timer;	/* frames to the next colour */
	Sint8	renda_cnt;	/* presses left to make a move last longer */
	Uint8	ef_cnt;		/* the effects of pls19_00 type 0 to 2 alive, at most 5 */
	Uint8	unk_04[0x40 - 0x04];
	Sint32	bubble_catch_cnt;
} PL19_LOCAL_W;
#define PL19_LW( w )	( ( PL19_LOCAL_W * )( w )->tlocal )

#endif
