/* Amingo's special moves. */
#ifndef PL15_DEF_H
#define PL15_DEF_H

/* pl_sp_type */
#define	pl15_sp_daichi_catch			0	/* Daichi no Kodomo (capture) */
#define	pl15_sp_midori			1	/* Midori no Kanki */
#define	pl15_sp_kaze			2	/* Kaze no Sakebi */
#define	pl15_sp_taiyou			3	/* Taiyou no Megumi (hyper) */
#define	pl15_sp_bite			4	/* a bite (assist) */
#define	pl15_sp_okite			5	/* Shokubutsu no Okite (hyper) */
#define	pl15_sp_chouhatsu		6	/* the taunt */
#define	pl15_sp_snapback			7	/* the snapback */
#define	pl15_sp_daichi			8	/* Daichi no Kodomo (punch/kick) */

/* atck_no: the character's own records in the hit table, after the common ATCK_ ones */
#define	pl15_at_throw_p			0x20	/* Punch throw's hit while held (catch_damage) */
#define	pl15_at_throw_p_2		0x21	/* Punch throw's release */
#define	pl15_at_okite			0x22	/* Shokubutsu no Okite: each hit on the caught opponent */
#define	pl15_at_daichi_catch	0x30	/* Daichi no Kodomo (capture): the seed throws off the opponent it holds (shl15_01) */
#define	pl15_at_t_kick			0x36	/* Tag-in kick that leaps off the wall */
#define	pl15_at_taiyou			0x38	/* Taiyou no Megumi (hyper) */
#define	pl15_at_okite_reach		0x3b	/* Shokubutsu no Okite: the reaching grab (Amingo and the arm shl15_02) */
#define	pl15_at_bite			0x3c	/* The bite (shl15_00, first part) */
#define	pl15_at_bite_2			0x3d	/* The bite's second part that flies out and bites (shl15_00_head_init) */
#define	pl15_at_snapback		0x3e	/* Snapback */
#define	pl15_at_okite_end		0x3f	/* Shokubutsu no Okite: the opponent let go at the end */
#define	pl15_at_upper_p			0x40	/* The launcher, punch version */
#define	pl15_at_upper_k			0x44	/* The launcher, kick version */

/* the character's own work, in tlocal */
typedef struct PL15_LOCAL_W {
	Sint8	shot_cnt;		/* how many seeds of Daichi no Kodomo are out */
	Uint8	midori_land_flag;	/* Midori no Kanki has landed */
	Uint8	kaze_hit_cnt;	/* Kaze no Sakebi's hits */
	Uint8	kaze_end_flag;	/* Kaze no Sakebi has stopped */
	Uint8	unk_04;
	Sint8	renda_cnt;		/* presses to make Midori no Kanki last longer */
	Uint8	midori_long_flag;	/* Midori no Kanki's longer form has been set */
} PL15_LOCAL_W;

#endif
