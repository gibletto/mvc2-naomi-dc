/* Dhalsim's special moves. */
#ifndef PL25_DEF_H
#define PL25_DEF_H

/* the character's own work, in tlocal */
typedef struct PL25_LOCAL_W {
	Sint8	turn_timer;	/* 00: frames to the flame's next turn */
	Sint8	turn_dir;	/* 01: the way the flame turns by itself */
	Uint8	ef_end_flag;	/* 02: set by the effect when it is done */
	Uint8	unk_03;		/* 03 */
	Uint8	flame_cnt;	/* 04: frames to the next flame, in fours */
	Uint8	flame_no;	/* 05: the next flame's number, 0 to 3 */
	Uint8	shot_flag;	/* 06: a fireball is on the screen */
	Uint8	unk_07;
	Sint16	float_timer;	/* 08: frames of float left */
} PL25_LOCAL_W;
#define PL25_LW( pl )	( ( PL25_LOCAL_W * )( pl )->tlocal )

/* pl_sp_type */
#define	pl25_sp_fire	0	/* Yoga Fire */
#define	pl25_sp_frei	1	/* Yoga Flame */
#define	pl25_sp_brast	2	/* Yoga Blast */
#define	pl25_sp_teleport	3	/* Yoga Teleport */
#define	pl25_sp_float	4	/* Yoga Float */
#define	pl25_sp_inferno	5	/* Yoga Inferno (hyper) */
#define	pl25_sp_strike	6	/* Yoga Strike (hyper) */
#define	pl25_sp_chouhatsu	9	/* Taunt */
#define	pl25_sp_jinferno	12	/* Yoga Inferno in the air (hyper) */
#define	pl25_sp_snapback	13	/* Snapback */

/* atck_no: the character's own records in the hit table, after the common ATCK_ ones */
#define	pl25_at_jdown_hp		0x12	/* Down + HP in the air */
#define	pl25_at_jdown_lk		0x13	/* Down + LK in the air (drill) */
#define	pl25_at_jdown_mk		0x14	/* Down + MK in the air (drill) */
#define	pl25_at_jdown_hk		0x15	/* Down + HK in the air (drill) */
#define	pl25_at_upper			0x16	/* Launcher (pl25_upper, Char_Extra 16) */
#define	pl25_at_com_throw		0x20	/* Command throw (half circle + HP): the final throw */
#define	pl25_at_throw_p			0x21	/* P ground throw */
#define	pl25_at_strike_release	0x22	/* Yoga Strike hyper: lifts and lets go of the caught opponent */
#define	pl25_at_air_throw		0x23	/* P air throw: the release */
#define	pl25_at_throw_k			0x24	/* K ground throw: the release */
#define	pl25_at_com_throw_hit	0x26	/* Command throw: each hit while held (can be mashed out of) */
#define	pl25_at_strike_slam		0x27	/* Yoga Strike hyper: the slam */
#define	pl25_at_far_lp			0x30	/* Far standing LP */
#define	pl25_at_far_mp			0x31	/* Far standing MP */
#define	pl25_at_far_hp			0x32	/* Far standing HP */
#define	pl25_at_far_lk			0x33	/* Far standing LK */
#define	pl25_at_far_mk			0x34	/* Far standing MK */
#define	pl25_at_far_hk			0x35	/* Far standing HK */
#define	pl25_at_far_crouch_lp	0x36	/* Far crouching LP */
#define	pl25_at_far_crouch_mp	0x37	/* Far crouching MP */
#define	pl25_at_far_crouch_hp	0x38	/* Far crouching HP */
#define	pl25_at_far_crouch_lk	0x39	/* Far crouching LK */
#define	pl25_at_far_crouch_mk	0x3a	/* Far crouching MK */
#define	pl25_at_far_crouch_hk	0x3b	/* Far crouching HK */
#define	pl25_at_far_jump_lp		0x3c	/* Far jumping LP */
#define	pl25_at_far_jump_mp		0x3d	/* Far jumping MP */
#define	pl25_at_far_jump_hp		0x3e	/* Far jumping HP */
#define	pl25_at_far_jump_lk		0x3f	/* Far jumping LK */
#define	pl25_at_far_jump_mk		0x40	/* Far jumping MK */
#define	pl25_at_far_jump_hk		0x41	/* Far jumping HK */
#define	pl25_at_fire			0x42	/* Yoga Fire fireball (shl25_00); the player also sets it for both Yoga Infernos */
#define	pl25_at_frei_l			0x45	/* Yoga Flame, light (player and shl25_01) */
#define	pl25_at_brast_l			0x46	/* Yoga Blast, light (player and shl25_03) */
#define	pl25_at_inferno_1		0x47	/* Yoga Inferno flame (shl25_04, flame_no 0) */
#define	pl25_at_t_kick			0x49	/* Partner's wall-leap kick (t_kick) */
#define	pl25_at_throw_k_hit		0x4a	/* K ground throw: the hit while held */
#define	pl25_at_air_throw_hit	0x4b	/* P air throw: the hit while held */
#define	pl25_at_inferno_2		0x4d	/* Yoga Inferno flame, the other attack number (shl25_04, flame_no set) */
#define	pl25_at_snapback		0x4e	/* Snapback */
#define	pl25_at_frei_h			0x4f	/* Yoga Flame, higher level (player and shl25_01) */
#define	pl25_at_brast_h			0x50	/* Yoga Blast, higher level (player and shl25_03) */

#endif
