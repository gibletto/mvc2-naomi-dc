/* SonSon's special moves. */
#ifndef PL14_DEF_H
#define PL14_DEF_H

/* pl_sp_type */
#define	pl14_sp_shienbu			0	/* Shienbu */
#define	pl14_sp_fuusetsu			1	/* Fuusetsu Zan */
#define	pl14_sp_seiten			2	/* Seiten Rengeki */
#define	pl14_sp_hofuku				3	/* Hofuku Zenshin */
#define	pl14_sp_tenchi			4	/* Tenchi Tsuukan (hyper) */
#define	pl14_sp_hisago			5	/* Kingin no Hisago */
#define	pl14_sp_pow			6	/* Pow (hyper) */
#define	pl14_sp_enou			7	/* En'Ou (hyper) */
#define	pl14_sp_chouhatsu		8	/* the taunt */
#define	pl14_sp_peach				9	/* the assist's healing peach */
#define	pl14_sp_snapback			10	/* the snapback */
#define	pl14_sp_wall			11	/* Wall Walk */
#define	pl14_sp_enou_auto		12	/* En'Ou, automatic (team hyper) */

/* atck_no: the character's own records in the hit table, after the common ATCK_ ones */
#define	pl14_at_fwd_hp		0x12	/* Forward + HP: the staff stretched far out (stand_punch, nml_no 2 with forward) */
#define	pl14_at_mp_2		0x19	/* Standing MP's second hit, set on cg_type */
#define	pl14_at_mk_2		0x1a	/* Standing MK's second hit, set on cg_type */
#define	pl14_at_jump_hp_2	0x1c	/* Jumping HP's later hit (cg_flag1 2; cg_flag1 1 sets common 0x0e) */
#define	pl14_at_upper		0x1d	/* The launcher */
#define	pl14_at_throw_p		0x20	/* Punch throw's hit while she holds the opponent (p_catch_lift, cg_type 1) */
#define	pl14_at_throw_k		0x21	/* Kick throw */
#define	pl14_at_throw_p_2	0x22	/* Punch throw's release that sends them off (ground and air throw) */
#define	pl14_at_shienbu		0x30	/* Shienbu: SonSon and the three monkey shots (shl14_00) */
#define	pl14_at_fuusetsu	0x33	/* Fuusetsu Zan, each mashed hit, ground or air */
#define	pl14_at_seiten		0x36	/* Seiten Rengeki's first hit */
#define	pl14_at_seiten_2	0x37	/* Seiten Rengeki's later hits (up to atck_level + 1) */
#define	pl14_at_tenchi		0x39	/* Tenchi Tsuukan: the staff thrown up (SonSon and shl14_01's head) */
#define	pl14_at_tenchi_2	0x3a	/* Tenchi Tsuukan: the giant staff's hits as it comes down (shl14_01) */
#define	pl14_at_tenchi_3	0x3b	/* Tenchi Tsuukan: the giant staff's last hit (timer1 4) */
#define	pl14_at_hisago		0x3c	/* Kingin no Hisago: the gourd that catches */
#define	pl14_at_hisago_2	0x3d	/* Kingin no Hisago: the caught opponent sent out (sp_catch_hit, on the opponent) */
#define	pl14_at_pow			0x3f	/* POW: SonSon and the flying word (shl14_02) */
#define	pl14_at_pow_hold	0x40	/* POW: hold on the caught opponent (shl14_02_hold) */
#define	pl14_at_pow_throw	0x41	/* POW: the throw out at the end (shl14_02_throw_rise) */
#define	pl14_at_enou		0x42	/* En'Ou: growing into the giant monkey */
#define	pl14_at_enou_lp		0x43	/* En'Ou: the giant monkey's LP downward punch (also sp_enou_auto) */
#define	pl14_at_enou_hp		0x44	/* En'Ou: the giant monkey's HP uppercut (also sp_enou_auto) */
#define	pl14_at_enou_rush	0x45	/* En'Ou: the rush and the attacks riding on it (shl14_04/05/06) */
#define	pl14_at_peach		0x46	/* The assist's healing peach (shl14_03, set for the other side) */
#define	pl14_at_t_kick		0x47	/* Tag-in kick that leaps off the wall */
#define	pl14_at_snapback	0x48	/* Snapback */
#define	pl14_at_wall		0x49	/* Wall Walk (wall_init) */

/* the character's own work, in tlocal */
typedef struct PL14_LOCAL_W {
	Float	entry_x;				/* 000: where the entrance ends */
	Uint8	unk_04[0x08 - 0x04];
	Sint16	step_timer;				/* 008: En'Ou's timer for each step */
	Sint16	monkey_timer;			/* 00a: how long En'Ou lasts */
	Float	monkey_scale_x;			/* 00c: the giant monkey's width */
	Float	unk_10;
	Float	monkey_scale_y;			/* 014: the giant monkey's height */
	Uint8	unk_18[0x1c - 0x18];
	Sint16	wall_rot;				/* 01c: Wall Walk's spin */
	Sint16	shot_cnt;				/* 01e: Shienbu only while this is 0; shl14_00's shots out */
	struct PLAYER_WORK *part_w[13];	/* 020: shl14_01's parts, 1 to 12 */
} PL14_LOCAL_W;

#endif
