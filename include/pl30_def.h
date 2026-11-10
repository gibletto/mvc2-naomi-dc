/* Omega Red's special moves. */
#ifndef PL30_DEF_H
#define PL30_DEF_H

/* animation numbers */
#define	CH30_WIN_KO0	6
#define	CH30_WIN_KO1	0
#define	CH30_WIN_TIME	7
#define	CH30_LOSE		8

/* pl_sp_type */
#define	pl30_sp_destroyer	0	/* Omega Destroyer (hyper) */
#define	pl30_sp_coil		2	/* Carbonadium Coil */
#define	pl30_sp_jcoil		3	/* Kuuchuu Carbonadium Coil (air, kick) */
#define	pl30_sp_strike		4	/* Omega Strike */
#define	pl30_sp_chouhatsu	6	/* the taunt */
#define	pl30_sp_snapback	7	/* the snapback */
#define	pl30_sp_burrow		9	/* Burrowing Coil (gamma assist) */
#define	pl30_sp_jdestroyer	10	/* Omega Destroyer in the air (hyper) */
#define	pl30_sp_counter_a	11	/* the alpha type's Variable Counter */
#define	pl30_sp_counter_g	12	/* the gamma type's Variable Counter */
#define	pl30_sp_coil2		13	/* Carbonadium Coil with both punches */
#define	pl30_sp_jcoil2	14	/* Kuuchuu Carbonadium Coil, both kicks */
#define	pl30_sp_strike2		15	/* Omega Strike with both kicks */

/* atck_no: the character's own records in the hit table, after the common ATCK_ ones */
#define	pl30_at_coil_slam	0x2a	/* Carbonadium Coil: the caught opponent pulled in and slammed (shl30_01_chase_2) */
#define	pl30_at_t_kick		0x30	/* The partner's jump-in attack (t_kick) */
#define	pl30_at_coil_throw	0x38	/* Carbonadium Coil hold: the opponent hurt and thrown (shl30_01_catch_hold) */
#define	pl30_at_burrow		0x3c	/* Burrowing Coil (gamma assist; him and shl30_02 type 1) */
#define	pl30_at_jdestroyer	0x3d	/* Omega Destroyer in the air: the dash (jdestroyer_dash_init) */
#define	pl30_at_snapback	0x42	/* Snapback */
#define	pl30_at_counter_a	0x43	/* Alpha type's Variable Counter */
#define	pl30_at_counter_g	0x44	/* Gamma type's Variable Counter */
#define	pl30_at_upper		0x45	/* The launcher (pl30_upper) */

/* the character's own work, in tlocal */
typedef struct PL30_LOCAL_W {
	Uint16	level;			/* 00: the move's strength */
	Uint16	pull_flag;		/* 02: set by shl30_03 when the pull is done */
	VEC3	ret_pos;		/* 04: a position to come back to */
	Uint8	unk_10;
	Uint8	turn;			/* 11: flips pl_dir */
	Uint16	key_flag;		/* 12: set when a button is taken in the catch */
	Uint16	lever;			/* 14: the stick kept from the catch */
	Uint16	type;			/* 16: punch (0) or kick (1), for pl_w->type */
} PL30_LOCAL_W;
#define PL30_LW( w )	( ( PL30_LOCAL_W * )( w )->tlocal )

#endif
