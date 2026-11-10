/* M. Bison's special moves. */
#ifndef PL26_DEF_H
#define PL26_DEF_H

/* pl_sp_type */
#define	pl26_sp_shot	0	/* Psycho Shot */
#define	pl26_sp_knee	1	/* Double Knee Press */
#define	pl26_sp_warp	2	/* Bison Warp */
#define	pl26_sp_head	3	/* Head Press */
#define	pl26_sp_skull	4	/* Somersault Skull Diver */
#define	pl26_sp_nightmare	5	/* Knee Press Nightmare (hyper) */
#define	pl26_sp_crusher	6	/* Psycho Crusher (hyper) */
#define	pl26_sp_field	7	/* Psycho Field */
#define	pl26_sp_fly		9	/* Flight */
#define	pl26_sp_chouhatsu	11	/* Taunt */
#define	pl26_sp_snapback	14	/* Snapback */
#define	pl26_sp_explosion	15	/* Psycho Explosion (hyper) */

/* atck_no: the character's own records in the hit table, after the common ATCK_ ones */
#define	pl26_at_stand_hk_2		0x19	/* Second hit of the standing HK */
#define	pl26_at_throw			0x20	/* Ground throw */
#define	pl26_at_air_throw		0x21	/* Air throw */
#define	pl26_at_field_hit		0x26	/* Psycho Field: each hit of the turning ring (shl26_00 type 1) */
#define	pl26_at_knee_l_1		0x30	/* Double Knee Press first kick (also the air version, knee_air) */
#define	pl26_at_head			0x39	/* Head Press stomp (pl26_main follows up when it hits) */
#define	pl26_at_head_dive		0x3a	/* Head Press: the dive after the bounce */
#define	pl26_at_skull			0x3b	/* Somersault Skull Diver dive */
#define	pl26_at_field			0x3c	/* Psycho Field (sp_field, and shl26_00 type 1 parts) */
#define	pl26_at_nightmare_kick	0x3d	/* Knee Press Nightmare hyper: the jumping kicks */
#define	pl26_at_nightmare_crush	0x3e	/* Knee Press Nightmare's closing psycho crusher, which hits again and again (also shl26_01's slide) */
#define	pl26_at_nightmare_side	0x3f	/* The two shl26_01 works beside Knee Press Nightmare, as he flies and falls */
#define	pl26_at_t_kick			0x40	/* Partner's jump-in attack (t_kick) */
#define	pl26_at_crusher			0x41	/* Psycho Crusher hyper, ground and air versions */
#define	pl26_at_knee_vc_1		0x42	/* Double Knee Press first kick as a variable counter (team_type 3) */
#define	pl26_at_knee_vc_2		0x43	/* Double Knee Press second kick as a variable counter */
#define	pl26_at_snapback		0x45	/* Snapback (sp_snapback) */
#define	pl26_at_upper			0x46	/* Launcher (pl26_upper, Char_Extra 5) */
#define	pl26_at_explosion		0x47	/* Psycho Explosion hyper: his start, the flame's first hit and its odd repeat hits */
#define	pl26_at_explosion_2		0x48	/* Psycho Explosion flame: the hit that catches and its even repeat hits */
#define	pl26_at_explosion_3		0x49	/* Psycho Explosion flame: the last hit */

/* the character's own work, in tlocal */
typedef struct PL26_LOCAL_W {
	Uint8	shot_flag;	/* the psycho shot (shl26_00 type 0) is out */
	Uint8	field_flag;	/* the Psycho Field (shl26_00 type 1) is out */
	Uint8	hit_voice;		/* set once the attack's hit has played its voice */
	Uint8	air;			/* the head press or skull crusher began in the air */
	Uint8	unk_04;			/* set in the entrance and at marks of his moves; his modules never read it */
	Uint8	hit_timer;		/* frames until the attack hits again */
	Uint8	hit_full;		/* the psycho crusher has all its hits */
	Uint8	hold_timer;		/* frames the psycho crusher is held back */
	Uint8	hit_cnt;		/* the psycho crusher's hits */
	Uint8	press_cnt;		/* presses toward the next hit */
	Sint16	fly_timer;		/* frames of flight left */
	Uint8	start_flag;		/* Knee Press Nightmare: set when shl26_01 may set off */
	Uint8	ef_end_flag;		/* set when the Psycho Explosion's flame is done */
} PL26_LOCAL_W;

#endif
