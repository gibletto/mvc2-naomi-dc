/* Colossus's special moves. */
#ifndef PL32_DEF_H
#define PL32_DEF_H

/* pl_sp_type */
#define	pl32_sp_tackle		0	/* Power Tackle */
#define	pl32_sp_swing			1	/* Giant Swing */
#define	pl32_sp_armor		2	/* Super Armor (hyper) */
#define	pl32_sp_dive		3	/* Super Dive (hyper) */
#define	pl32_sp_chouhatsu	4	/* the taunt */
#define	pl32_sp_snapback	5	/* the snapback */
#define	pl32_sp_stand_hk	6	/* standing heavy kick (gamma assist and counter) */

/* atck_no: the character's own records in the hit table, after the common ATCK_ ones */
#define	pl32_at_jfwd_hp		0x12	/* Air heavy punch with forward */
#define	pl32_at_upper		0x19	/* The launcher (pl32_upper) */
#define	pl32_at_swing_throw	0x20	/* Giant Swing's grab: thrown down at the top (rush_top, ground and air) */
#define	pl32_at_throw_k		0x21	/* Ground kick throw, slammed on landing */
#define	pl32_at_throw_p		0x22	/* Ground punch throw */
#define	pl32_at_air_throw	0x23	/* Air punch throw, slammed on landing */
#define	pl32_at_swing		0x32	/* Giant Swing (sp_swing): the rush that grabs */
#define	pl32_at_dive		0x33	/* Super Dive (hyper): the start and rise */
#define	pl32_at_dive_2		0x34	/* Super Dive: the dive down */
#define	pl32_at_t_kick		0x35	/* The partner's jump-in attack (t_kick) */
#define	pl32_at_snapback	0x36	/* Snapback (sp_snapback) */

/* the character's own work, in tlocal */
typedef struct PL32_LOCAL_W {
	Sint8	glow_step;		/* the step in the glow's table */
	Uint8	frame_cnt;		/* counts frames */
	Uint8	idle_step;		/* the idle glow's state: waiting or glowing */
	Sint8	armor_col;		/* the armour's glow colour, 0 to 7 */
	Sint16	armor_timer;		/* frames of the armour left */
	Uint8	voice_cnt;		/* counts frames for the idle voice */
	Uint8	dive_flag;		/* set while the dive glows */
	Sint8	dive_timer;		/* frames to the dive's next colour */
	Uint8	dive_col;		/* the dive's glow colour, 0 to 7 */
	Uint8	sp_armor_flag;		/* set while a special move takes hits */
	Uint8	mash_flip;		/* flips at each step of a throw's mashing */
	Sint8	step_timer;		/* frames to the mashing's next step */
	Sint8	mash_lv;		/* how hard the throw is mashed, 0 to 7 */
	Uint8	mash_flag;		/* set when the player mashed since the last step */
	Sint8	cpu_timer;		/* frames to a mash while cpu_flag is set */
	Sint8	team_timer;		/* frames to a mash while team_no is set */
	Uint8	unk_11[3];
	Uint8	mash_lv_copy[1];		/* mash_lv, kept */
} PL32_LOCAL_W;

#endif
