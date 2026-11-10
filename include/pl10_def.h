/* Tron Bonne's special moves. */
#ifndef PL10_DEF_H
#define PL10_DEF_H

/* pl_sp_type */
#define	pl10_sp_strike			0	/* Bonne Strike */
#define	pl10_sp_beacon			1	/* Beacon Bomb */
#define	pl10_sp_kobun			2	/* Kobun Launch */
#define	pl10_sp_chouhatsu		3	/* the taunt */
#define	pl10_sp_king				4	/* King Kobun (hyper) */
#define	pl10_sp_lunch			5	/* Lunch Rush (hyper) */
#define	pl10_sp_snapback			6	/* the snapback */
#define	pl10_sp_throw			7	/* a throw (assist) */
#define	pl10_sp_kick			8	/* a kick (assist) */
#define	pl10_sp_fire			9	/* Gustaff's fire (assist) */

/* atck_no: the character's own records in the hit table, after the common ATCK_ ones */
#define	pl10_at_upper			0x16	/* The launcher (upper_init, crouching, Char_Extra 3) */
#define	pl10_at_stand_lk_2		0x19	/* Standing LK's second hit (stand_kick_move, nml_no 0) */
#define	pl10_at_throw_p			0x20	/* Ground throw (nml_p_catch) */
#define	pl10_at_air_throw		0x21	/* Air throw (jump_p_throw) */
#define	pl10_at_assist_throw	0x22	/* The assist's throw (throw_throw, pl10_sp_throw) */
#define	pl10_at_kobun			0x31	/* Kobun Launch (kobun_init) */
#define	pl10_at_t_kick			0x32	/* The jump-in attack when she is tagged in */
#define	pl10_at_strike			0x37	/* Bonne Strike's rolling hits on the ground; mashing speeds them up (strike_init, strike_renda) */
#define	pl10_at_strike_air		0x38	/* Bonne Strike's rolling hits in the air (strike_air_init, strike_renda) */
#define	pl10_at_king			0x39	/* King Kobun (hyper): start, and the big Servbot's first hit (king_init, pls10_03_king_wait) */
#define	pl10_at_king_2			0x3a	/* King Kobun: the grown Servbot's run and later hits (pls10_03_king_grow, pls10_03_king_rush) */
#define	pl10_at_kick			0x3b	/* The kick assist (pl10_sp_kick) */
#define	pl10_at_fire			0x3c	/* Gustaff's fire assist (pl10_sp_fire) */
#define	pl10_at_lunch			0x3d	/* Lunch Rush (hyper): the short beam (lunch_init) */
#define	pl10_at_beacon			0x3f	/* Beacon Bomb: Tron and the claw that catches (beacon_init, shl10_04_catch_init) */
#define	pl10_at_strike_last		0x41	/* Bonne Strike's last hit on the ground (strike_rush, timer0 2) */
#define	pl10_at_strike_air_last	0x42	/* Bonne Strike's last hit in the air (strike_air_rush) */
#define	pl10_at_snapback		0x43	/* Snapback */

/* the character's own work, in tlocal */
typedef struct PL10_LOCAL_W {
	struct PLAYER_WORK *servbot_w;	/* 000: the Servbot of pls10_03, kept by pl10_tlocal_init() */
	Uint8	servbot_flag;			/* 004: the Servbot of pls10_03 has been set; kept by pl10_tlocal_init() */
	Uint8	throw_cnt;				/* 005: the Servbots of shl10_01 out and not yet landed */
	Uint8	renda_cnt;				/* 006: buttons pressed while Bonne Strike runs */
	Uint8	run_cnt;				/* 007: the Servbots of shl10_03 set in Lunch Rush */
	Uint8	unk_8[0x40 - 0x08];
	Sint32	claw_hop_cnt;			/* 040: hops for the Servbot of pls10_03 while the claw (shl10_04) holds */
} PL10_LOCAL_W;

#define PL10_LW( pl )	( ( PL10_LOCAL_W * )( pl )->tlocal )

#endif
