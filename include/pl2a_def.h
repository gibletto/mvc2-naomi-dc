/* Storm's animation numbers and special moves. */
#ifndef PL2A_DEF_H
#define PL2A_DEF_H

#define	CH2A_WIN		0
#define	CH2A_LOSE		1

/* pl_sp_type */
#define	pl2a_sp_typhoon	0	/* Typhoon */
#define	pl2a_sp_lstorm	1	/* Lightning Storm (hyper) */
#define	pl2a_sp_lattack_a	2	/* Lightning Attack, as an assist */
#define	pl2a_sp_fly		3	/* Flight */
#define	pl2a_sp_lattack	4	/* Lightning Attack */
#define	pl2a_sp_hail	5	/* Hail Storm (hyper) */
#define	pl2a_sp_dtyphoon	6	/* Double Typhoon */
#define	pl2a_sp_07		7
#define	pl2a_sp_chouhatsu	8	/* Taunt */
#define	pl2a_sp_09		9
#define	pl2a_sp_10		10
#define	pl2a_sp_snapback	11	/* Snapback */
#define	pl2a_sp_jsphere	12	/* Lightning Sphere (air) */

/* atck_no: the character's own records in the hit table, after the common ATCK_ ones */
#define	pl2a_at_jsphere		0x18	/* Lightning Sphere in the air: the bolts shl2a_04 throws */
#define	pl2a_at_throw_k		0x20	/* K throw (player and opponent) */
#define	pl2a_at_throw_p		0x21	/* P throw, ground or air: the final throw */
#define	pl2a_at_throw_p_hit	0x22	/* P throw: each hit while holding */
#define	pl2a_at_typhoon		0x30	/* Typhoon, ground or air */
#define	pl2a_at_hail		0x5a	/* Hail Storm hyper */
#define	pl2a_at_dtyphoon	0x5b	/* Double Typhoon (player and shl2a_01) */
#define	pl2a_at_t_kick		0x5c	/* Partner's wall-leap kick (t_kick) */
#define	pl2a_at_lstorm		0x5d	/* Lightning Storm hyper */
#define	pl2a_at_snapback	0x60	/* Snapback (sp_snapback) */
#define	pl2a_at_upper		0x61	/* Launcher (pl2a_upper) */

/* the character's own work: s_pl2a.c reads it as tlocal[] words, its effects through
   PL2A_LOCAL_W */
#define	TL_ANM_FLAG		0		/* the animation's frame code, kept from the last frame */
#define	TL_DASH_CNT		1		/* air dashes left in this jump */
#define	TL_DASH_WAIT	2		/* frames before the next air dash */
#define	TL_IDLE_TM		3		/* frames to the next idle sound */
#define	TL_FLY_TM		4		/* frames of flight left */
#define	TL_JUMP_MP		8		/* set for the jumping medium punch */
#define	TL_LSTORM_COL_TM		9		/* frames to the next step of Lightning Storm's colour */
#define	TL_LSTORM_COL		10		/* Lightning Storm's colour */
#define	TL_COL_TM		11		/* frames to the next colour step */
#define	TL_COL_1		12		/* the first colour cycle's step */
#define	TL_COL_2		13		/* the second colour cycle's step */
#define	TL_DIR			14		/* the way a move goes */
#define	TL_DASH_AGAIN	15		/* an air dash straight after another */
#define	TL_COL_STOP		16		/* frames a special's colour holds the cycles off */
#define	TL_DASH_SW		17		/* the stick when an air dash starts */
#define	TL_NML_COL_TM	18		/* frames left of a punch's colour */
#define	TL_NML_CHAR		19		/* the punch's animation */

typedef struct PL2A_LOCAL_W {
	Sint32	last_code;		/* the animation's frame code, kept from the last frame */
	Sint32	dash_cnt;		/* air dashes left in this jump */
	Sint32	dash_timer;		/* frames before the next air dash */
	Sint32	idle_timer;		/* frames to the next idle sound */
	Sint32	fly_timer;		/* frames of flight left */
	Uint8	unk_14[0x20 - 0x14];
	Sint32	jump_mp_flag;	/* set for the jumping medium punch; nothing reads it */
	Sint32	lstorm_col_timer;	/* frames to the next step of Lightning Storm's colour */
	Sint32	lstorm_col;		/* Lightning Storm's colour */
	Sint32	col_timer;		/* frames to the next colour step */
	Sint32	col_1;			/* the first colour cycle's step */
	Sint32	col_2;			/* the second colour cycle's step */
	Sint32	dir;			/* the way a move goes */
	Sint32	dash_again_flag;	/* an air dash straight after another */
	Sint32	col_stop_timer;	/* frames a special's colour holds the cycles off */
	Sint32	dash_sw;		/* the stick when an air dash starts */
	Sint32	nml_col_timer;	/* frames left of a punch's colour */
	Sint32	nml_char;		/* the punch's animation */
} PL2A_LOCAL_W;

#define	PL2A_LW( pl )	( ( PL2A_LOCAL_W * )( pl )->tlocal )

#endif
