/* Shuma-Gorath's special moves. */
#ifndef PL2D_DEF_H
#define PL2D_DEF_H

/* pl_sp_type */
#define	pl2d_sp_stare	0	/* Mystic Stare */
#define	pl2d_sp_smash	1	/* Mystic Smash */
#define	pl2d_sp_devital	2	/* Devitalization */
#define	pl2d_sp_chaos	3	/* Chaos Dimension (hyper) */
#define	pl2d_sp_hsmash	5	/* Hyper Mystic Smash (hyper) */
#define	pl2d_sp_chouhatsu	6	/* Taunt */
#define	pl2d_sp_snapback	7	/* Snapback */

/* atck_no: the character's own records in the hit table, after the common ATCK_ ones */
#define	pl2d_at_crouch_mp_2		0x19	/* Second hit of the crouching medium punch */
#define	pl2d_at_throw_p			0x20	/* Ground punch throw (catch_no 0) */
#define	pl2d_at_air_throw		0x22	/* Air punch throw (catch_no 1) */
#define	pl2d_at_devital_throw	0x23	/* Devitalization: the opponent thrown (devital_throw) */
#define	pl2d_at_chaos_drop		0x24	/* Chaos Dimension grab: the opponent dropped back (chaos_catch_drop) */
#define	pl2d_at_stare			0x30	/* Mystic Stare (him and the eyes, shl2d_00) */
#define	pl2d_at_stare_2			0x31	/* Mystic Stare eye stuck to the opponent, its later hit */
#define	pl2d_at_devital			0x38	/* Devitalization: his own hit as he comes down with the opponent */
#define	pl2d_at_chaos			0x39	/* Chaos Dimension start (hyper flash, chaos_move) */
#define	pl2d_at_chaos_jump		0x3c	/* Powered-up jump attack after Chaos Dimension (0x3a/0x3b are the standing and crouching ones) */
#define	pl2d_at_t_kick			0x3d	/* The partner's jump-in attack (t_kick) */
#define	pl2d_at_hsmash			0x3e	/* Hyper Mystic Smash */
#define	pl2d_at_upper			0x44	/* The launcher (pl2d_upper) */
#define	pl2d_at_snapback		0x45	/* Snapback */

/* the character's own work, in tlocal */
typedef struct PL2D_LOCAL_W {
	Uint8	noki_flag;		/* set by the back dash's effect: the slide is over */
	Sint8	win_no;			/* the win pose */
	Sint8	orb_fire_flag;	/* a bit for each orb of shl2d_00 that has flown off: 63 for all six */
	Sint8	orb_flag;		/* 1: all six orbs are out; -1: they are to end */
	Uint8	orb_cnt;		/* orbs still to fly off */
	Sint8	smash_type;		/* the flying attack: which table, then homing in */
	Uint8	unk_06;
	Sint8	rise_flag;		/* the grab's rise: above 0 when it peaks, below 0 to land */
	Sint8	release_flag;	/* -1: Chaos Dimension's grab has let go */
	Sint8	power_step;		/* a power-up is on: the colour's step, 1 or -1 */
	Sint16	power_timer;	/* frames of the power-up left */
	Uint8	unk_0c;
	Sint8	col_timer;		/* the grab's colour: frames before the step turns */
	Sint8	col_cnt;		/* the power-up's frames to the next colour; the grab's colour */
	Sint8	col_no;			/* the power-up's colour; the grab's colour step */
	Uint8	dive_flag;		/* the heavy kick dives */
} PL2D_LOCAL_W;

#define PL2D_LW( pl )	( ( PL2D_LOCAL_W * )&( pl )->tlocal[0] )

#endif
