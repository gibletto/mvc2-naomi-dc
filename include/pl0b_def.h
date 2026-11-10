/* Captain America's special moves. */
#ifndef PL0B_DEF_H
#define PL0B_DEF_H

/* pl_sp_type */
#define	pl0b_sp_slash	0	/* Shield Slash */
#define	pl0b_sp_stars	1	/* Stars & Stripes */
#define	pl0b_sp_cartwheel	2	/* Roll */
#define	pl0b_sp_charge	3	/* Charging Star */
#define	pl0b_sp_chouhatsu	4	/* Taunt */
#define	pl0b_sp_hstars	5	/* Hyper Stars & Stripes (hyper) */
#define	pl0b_sp_justice	6	/* Final Justice (hyper) */
#define	pl0b_sp_hcharge	8	/* Hyper Charging Star (hyper) */
#define	pl0b_sp_snapback	14	/* Snapback */

/* atck_no: the character's own records in the hit table, after the common ATCK_ ones */
#define	pl0b_at_shield_mp		0x12	/* Standing M punch with the shield in hand */
#define	pl0b_at_shield_hp		0x13	/* Standing HP with the shield in hand */
#define	pl0b_at_shield_clp		0x14	/* Crouching LP with the shield in hand */
#define	pl0b_at_shield_cmp		0x15	/* Crouching M punch with the shield in hand */
#define	pl0b_at_shield_chp		0x16	/* Crouching HP with the shield in hand */
#define	pl0b_at_shield_jmp		0x17	/* Jumping M punch with the shield in hand (never used: overwritten by ATCK_JUMP_MP) */
#define	pl0b_at_shield_jhp		0x18	/* Jumping HP with the shield in hand */
#define	pl0b_at_stand_mk_2		0x19	/* Standing M kick's second kick, with the button pressed again */
#define	pl0b_at_jdown_hk		0x1e	/* Jumping HK with the stick down */
#define	pl0b_at_jup_hk			0x1f	/* Jumping HK with the stick up */
#define	pl0b_at_throw_p			0x20	/* The punch throw, set on the opponent */
#define	pl0b_at_throw_k			0x21	/* The kick throw, set on the opponent */
#define	pl0b_at_air_throw		0x22	/* The air throw, set on the opponent */
#define	pl0b_at_justice_hits	0x23	/* Final Justice (hyper): the blows on the caught opponent */
#define	pl0b_at_justice_toss	0x24	/* Final Justice: he lets go and the opponent goes up */
#define	pl0b_at_justice_slam	0x25	/* Final Justice: the opponent hits the ground */
#define	pl0b_at_stars			0x30	/* Stars & Stripes */
#define	pl0b_at_charge_h		0x41	/* Charging Star, heavy: the start (0x40 + level); the variable counter uses it too */
#define	pl0b_at_charge_dash_h	0x43	/* Charging Star, heavy: the dash (0x42 + level); the variable counter uses it too */
#define	pl0b_at_slash_l			0x44	/* Shield Slash, light (0x44 + level; 0x48 + level in the air); the variable counter uses it too */
#define	pl0b_at_justice			0x4c	/* Final Justice (hyper): the dash and the catch */
#define	pl0b_at_hstars			0x50	/* Hyper Stars & Stripes (hyper): the first hit; the later ones are 0x4f + n */
#define	pl0b_at_hcharge			0x52	/* Hyper Charging Star (hyper) */
#define	pl0b_at_t_kick			0x55	/* The jump-in kick when he is tagged in (t_kick_init) */
#define	pl0b_at_snapback		0x56	/* Snapback (sp_snapback) */
#define	pl0b_at_upper_noshield	0x57	/* The launcher, crouching punch, with the shield away */
#define	pl0b_at_upper			0x58	/* The launcher, crouching punch, with the shield in hand */

/* the character's own work, in tlocal */
typedef struct PL0B_LOCAL_W {
	struct PLAYER_WORK *shield_w;	/* 00: the shield */
	struct PLAYER_WORK *em_w;	/* 04: the one Final Justice caught */
	Sint16	appear_x;		/* 08: where he lands when he appears */
	Sint16	appear_y;		/* 0a: and the height */
	Uint8	unk_0c[0x0e - 0x0c];
	Uint8	shield_st;		/* 0e: the state asked of the shield, 0 while it is with him */
	Uint8	shield_sub;		/* 0f: and its sub_type */
	Uint8	shield_type;	/* 10: the shield's state, as it last saw it */
	Uint8	away_flag;		/* 11: set by a piece (pls0b_00) once it is 80 away from him */
	Sint8	dash_hit_flag;	/* 12: the dash has hit */
	Uint8	unk_13;			/* 13 */
	Sint8	hit_cnt;		/* 14: hits left */
	Sint8	guard_flag;		/* 15: the shield guards during a dash: 1, or -1 in the hyper */
} PL0B_LOCAL_W;
#define	PL0B_LW( pl )	( ( PL0B_LOCAL_W * )( pl )->tlocal )

/* a frame of the shield's data, which shl0b_00 keeps a pointer to in obj_mtx */
typedef struct SHL0B_00_FRAME {
	Sint16	x, y;		/* 000: the shield's place */
	Uint8	sub_prio;
	Uint8	part_flag;		/* 005: 0 hides the parts */
	Uint8	anm[2];		/* 006: the shield's animation, and which frame of it */
	Sint8	part[2][4];	/* 008: each part's offset x and y, its sub_prio and its animation frame */
} SHL0B_00_FRAME;

/* the shield's own work */
typedef struct SHL0B_00_LOCAL_W {
	SHL0B_00_FRAME *frame;	/* 000 */
	Sint16	x, y;		/* 004: the frame's place */
	Uint16	no;		/* 008: the frame, 0xffff for none */
	Uint16	unk_0a;		/* 00a */
	Uint8	unk_0c;		/* 00c */
	Sint8	leave_flag;		/* 00d: set when he leaves the fight while the shield is away */
	Sint8	blink_timer;		/* 00e: frames the shield blinks before it goes back to him */
	Uint8	sub_prio;		/* 00f: the frame's sub_prio */
} SHL0B_00_LOCAL_W;
#define SHL0B_00_LW( w )	( ( SHL0B_00_LOCAL_W * )&( w )->obj_mtx )

#endif
