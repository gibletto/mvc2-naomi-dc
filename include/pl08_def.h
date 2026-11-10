/* Psylocke's special moves (character 08). */
#ifndef PL08_DEF_H
#define PL08_DEF_H

/* pl_sp_type */
#define	pl08_sp_blast	0	/* Psy-Blast */
#define	pl08_sp_spin	1	/* Psy-Blade */
#define	pl08_sp_butterfly	2	/* Kocho-Gakure (hyper) */
#define	pl08_sp_thrust	3	/* Psy-Thrust (hyper) */
#define	pl08_sp_ninjitsu	4	/* Ninjutsu */
#define	pl08_sp_chouhatsu	5	/* Taunt */
#define	pl08_sp_maelstrom	6	/* Psy-Maelstrom (hyper) */
#define	pl08_sp_leap	7	/* a leaping slash, LP then back and HK; not in the move list */
#define	pl08_sp_snapback	8	/* Snapback */

/* atck_no: the character's own records in the hit table, after the common ATCK_ ones */
#define	pl08_at_jup_hk			0x12	/* Jumping HK with the stick up */
#define	pl08_at_back_hk			0x13	/* Standing HK with the stick back: a flip */
#define	pl08_at_fwd_hk			0x14	/* Standing HK with the stick forward: a hop */
#define	pl08_at_jump_hk_2		0x15	/* Jumping HK's second hit */
#define	pl08_at_throw_p			0x20	/* The punch throw (nml_p_catch), also the air throw (jump_k_catch), set on the opponent */
#define	pl08_at_throw_k			0x21	/* The kick throw (nml_k_catch), set on the opponent */
#define	pl08_at_throw_3			0x22	/* The throw of catch3 (on target_w); nothing in s_pl08 sets catch_no 3 */
#define	pl08_at_butterfly		0x30	/* Kocho-Gakure (hyper): her start and the even blades (shl08_01) */
#define	pl08_at_chouhatsu		0x31	/* The taunt, which has a hit */
#define	pl08_at_thrust			0x36	/* Psy-Thrust (hyper): the start, before the dashes */
#define	pl08_at_blast_h			0x3d	/* Psy-Blast: heavy from the ground (0x3b + level), and light in the air */
#define	pl08_at_jblast			0x3f	/* Psy-Blast in the air, M and H */
#define	pl08_at_maelstrom		0x4e	/* Psy-Maelstrom (hyper): the repeated hits */
#define	pl08_at_maelstrom_end	0x4f	/* Psy-Maelstrom: the last hit */
#define	pl08_at_leap			0x50	/* The leaping slash */
#define	pl08_at_t_kick			0x51	/* The jump-in kick when she is tagged in (t_kick) */
#define	pl08_at_snapback		0x52	/* Snapback */
#define	pl08_at_upper			0x53	/* The launcher, a crouching punch */
#define	pl08_at_blast_vc		0x55	/* Psy-Blast as a variable counter (team_type 3) */
#define	pl08_at_spin_vc			0x56	/* Psy-Blade as a variable counter: the first part */
#define	pl08_at_spin_vc_2		0x57	/* Psy-Blade as a variable counter: the follow-up */
#define	pl08_at_butterfly_2		0x58	/* Kocho-Gakure: the odd blades (shl08_01 type & 1) */

/* the character's own work, in tlocal */
typedef struct PL08_LOCAL_W {
	Uint8	unk_00[0x08];
	Uint8	dash_cnt;	/* 08: Psy-Thrust's dashes left */
	Uint8	old_cg;		/* 09: cg_type the frame before */
	Uint8	shot_flag;	/* 0a: a Psy-Blast (shl08_00) is out; no other until it is gone */
	Uint8	hit_cnt;	/* 0b: hits left (Psy-Blade, Psy-Maelstrom) */
	Uint8	follow_cnt;	/* 0c: Psy-Blade's follow-ups done */
	Uint8	warp_flag;	/* 0d: set by the copy (pls08_00) when it has carried her across */
	Uint8	color_flag;		/* 0e: the colours are set */
	Uint8	thrust_hit_cnt;		/* 0f: Psy-Thrust's hits */
	Uint8	unk_10[0x14 - 0x10];
	Sint8	blade_timer;	/* 14: Kocho-Gakure's frames to the next blades */
	Uint8	unk_15[0x1c - 0x15];
	Uint16	thrust_dir;	/* 1c: Psy-Thrust's direction, 0 to 15 */
	Uint8	unk_1e[0x22 - 0x1e];
	Uint16	spin_cnt;	/* 22: follow-ups left in the spin */
	Uint16	dash_timer;	/* 24: Psy-Thrust's dash time */
	Uint16	dash_add;	/* 26: frames added to the next dash time */
	Sint8	blade_cnt;	/* 28: Kocho-Gakure's blades fired */
	Uint8	unk_29;
	Uint16	atck_sw;	/* 2a: the stick when the move began */
} PL08_LOCAL_W;

#endif
