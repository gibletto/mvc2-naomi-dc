/* Servbot's special moves. */
#ifndef PL3A_DEF_H
#define PL3A_DEF_H

/* pl_sp_type */
#define	pl3a_sp_fire	0	/* Kobun Fire */
#define	pl3a_sp_engo	1	/* Engo Kougeki, punch */
#define	pl3a_sp_snapback	2	/* Snapback */
#define	pl3a_sp_lunch	3	/* Lunch Rush (hyper) */
#define	pl3a_sp_chouhatsu	4	/* Taunt */
#define	pl3a_sp_king	5	/* King Kobun (hyper) */
#define	pl3a_sp_rking	6	/* Rushing King Kobun (team hyper, beta assist) */
#define	pl3a_sp_engo_k	7	/* Engo Kougeki, kick */
#define	pl3a_sp_plane	8	/* Paper Planes */
#define	pl3a_sp_bull	9	/* Bull Mover */
#define	pl3a_sp_tackle	10	/* A dash through the opponent, after Tron's hit */

/* atck_no: the character's own records in the hit table, after the common ATCK_ ones */
#define	pl3a_at_throw			0x21	/* Ground and air throw's last throw (catch_throw) */
#define	pl3a_at_fire			0x30	/* Kobun Fire (fire_init); also the tackle (tackle_init) and shl3a_04's pieces */
#define	pl3a_at_lunch			0x3f	/* Lunch Rush, Servbot himself (lunch_init) */
#define	pl3a_at_lunch_first		0x4f	/* Lunch Rush's first piece (shl3a_01 mode 1, type 0) */
#define	pl3a_at_engo_k_h		0x50	/* Engo Kougeki kick, heavy: the servbot flying in from the far edge (mode 3, up to 3 hits) */
#define	pl3a_at_engo_h			0x51	/* Engo Kougeki punch, heavy: the servbot running in that grabs (mode 7) */
#define	pl3a_at_king			0x52	/* King Kobun (king_init) */
#define	pl3a_at_engo_k_l		0x53	/* Engo Kougeki kick, light: the servbot falling from above him (mode 4) */
#define	pl3a_at_lunch_burst_fly	0x54	/* The gamma type's Lunch Rush: the burst's pieces that fly in from the screen's edge (mode 6) */
#define	pl3a_at_lunch_burst			0x55	/* The gamma type's Lunch Rush (sp_lunch_burst) and the burst's pieces that bounce in (mode 2) */
#define	pl3a_at_lunch_drop			0x56	/* The beta type's Lunch Rush (sp_lunch_drop) and its piece (mode 18) */
#define	pl3a_at_rking_last		0x6d	/* Rushing King Kobun's last hit (rking_run) */
#define	pl3a_at_upper			0x76	/* Launcher (pl3a_upper) */
#define	pl3a_at_t_kick			0x77	/* Partner jump-in attack (pl3a_t_kick) */
#define	pl3a_at_snapback		0x78	/* Snapback (sp_snapback, crouching) */
#define	pl3a_at_plane			0x7b	/* Paper Planes: Servbot and the planes (shl3a_02) */
#define	pl3a_at_king_punch		0x7c	/* King Kobun's punch (king_punch) and Rushing King Kobun's hits (rking_init, rking_run) */
#define	pl3a_at_king_dash		0x7d	/* King Kobun's dash (king_dash, dash_ck) */

/* timers and counts, as word indexes of tlocal */
#define	TM_SHOT			0		/* frames before the next shot */
#define	TM_PIECE		4		/* Lunch Rush's pieces left, a byte for each kind (PL3A_LOCAL_W's piece_cnt) */
#define	TM_KING			8		/* frames of King Kobun left */
#define	TM_TACKLE			9		/* frames the tackle can be done in */

/* the character's own work, in tlocal; the timers (TM_SHOT, TM_KING, TM_TACKLE) are read as words
   of tlocal */
typedef struct PL3A_LOCAL_W {
	Uint8	unk_00[0x10 - 0x00];
	union {
		Uint32	w[4];		/* lunch_init sets them a word at a time */
		Sint8	b[16];		/* the pieces left to put out of each kind (shl3a_01) */
	} piece_cnt;		/* 010: Lunch Rush's pieces left of each kind */
	Uint8	unk_20[0x7c - 0x20];
	Sint32	unk_7c;			/* 07c */
} PL3A_LOCAL_W;

#define	PL3A_LW( pl )	( ( PL3A_LOCAL_W * )( pl )->tlocal )

#endif
