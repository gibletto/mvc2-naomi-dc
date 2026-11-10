/* Venom's special moves. */
#ifndef PL0E_DEF_H
#define PL0E_DEF_H

/* pl_sp_type */
#define	pl0e_sp_fang	0	/* Venom Fang */
#define	pl0e_sp_rush	1	/* Venom Rush */
#define	pl0e_sp_webthrow	2	/* Web Throw */
#define	pl0e_sp_web	3	/* Venom Web (hyper) */
#define	pl0e_sp_bite	4	/* Death Bite (hyper) */
#define	pl0e_sp_chouhatsu	5	/* Taunt */
#define	pl0e_sp_gamma		6
#define	pl0e_sp_snapback	7	/* Snapback */

/* atck_no: the character's own records in the hit table, after the common ATCK_ ones */
#define	pl0e_at_upper			0x12	/* The launcher (pl0e_upper, Char_Extra 4) */
#define	pl0e_at_stand_mk_2		0x19	/* Standing MK's second hit (stand_kick_move, nml_no 1) */
#define	pl0e_at_stand_hk_2		0x1a	/* Standing HK's second hit (stand_kick_move, nml_no 2) */
#define	pl0e_at_throw_p			0x20	/* Ground punch throw (nml_p_move, set on the held opponent) */
#define	pl0e_at_throw_k			0x21	/* Ground kick throw: lets the opponent go up with a quake (nml_k_move) */
#define	pl0e_at_air_throw		0x22	/* Air throw: lands, then throws them up (jump_p_move) */
#define	pl0e_at_webthrow_end	0x23	/* Web Throw's last hit, the release (webthrow_release) */
#define	pl0e_at_t_kick			0x25	/* T_kick, the jump-in attack (t_kick_init) */
#define	pl0e_at_assist			0x26	/* The attack on the spot of the gamma assist and counter (Char_Special 16) */
#define	pl0e_at_webthrow		0x35	/* Web Throw's slam on the caught opponent (webthrow_throw) */
#define	pl0e_at_webthrow_renda	0x36	/* Web Throw's slam after button mashing (webthrow_throw, timer0 set) */
#define	pl0e_at_web				0x37	/* Venom Web (hyper): the start and the web that catches (web_init, shl0e_01) */
#define	pl0e_at_web_dive		0x38	/* Venom Web's dive onto the webbed opponent (web_dive) */
#define	pl0e_at_bite			0x3e	/* Death Bite (hyper): each hit of the chain (shl0e_02) */
#define	pl0e_at_rush			0x41	/* Venom Rush: Venom and the symbiote hit along the ground (rush_init, shl0e_00) */
#define	pl0e_at_snapback		0x48	/* Snapback (sp_snapback) */

/* the character's own work, in tlocal */
typedef struct PL0E_LOCAL_W {
	Sint8	rush_st;		/* Venom Rush's effect: -1 once it is over */
	Sint8	hyper_col_flag;	/* the hyper combo's colour is on */
	Sint8	web_line_st;	/* Venom Web's web: above 0 caught, below 0 missed */
	Uint8	web_web_flag;	/* Venom Web's web is out */
	Sint8	bite_st;		/* Death Bite's effect: below 0 once it is over */
	Sint8	bite_hit_cnt;	/* Death Bite's hits left */
	Sint8	line_st;		/* Web Throw's web: above 0 caught, below 0 missed */
	Uint8	unk_7;
	PLAYER_WORK	*web_em_w;	/* Venom Web's web: the enemy it caught */
	Float	hand_x;			/* where the web leaves the hand */
	Float	hand_y;
	Float	web_dir;		/* the web's angle: the animation's */
	Float	web_line;		/* the web's angle now */
	Float	web_turn;		/* how far it turns each frame */
	Sint8	swing_st;		/* the swing's web: -1 once he lets go */
} PL0E_LOCAL_W;

#endif
