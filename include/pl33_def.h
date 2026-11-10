/* Iron Man's special moves. */
#ifndef PL33_DEF_H
#define PL33_DEF_H

/* pl_sp_type */
#define	pl33_sp_repulsor	0	/* Repulsor Blast */
#define	pl33_sp_proton		1	/* Proton Cannon (hyper) */
#define	pl33_sp_unibeam		2	/* Unibeam */
#define	pl33_sp_bomb		3	/* Smart Bomb */
#define	pl33_sp_chouhatsu	4	/* the taunt */
#define	pl33_sp_fly			5	/* Flight */
#define	pl33_sp_snapback	8	/* the snapback */
#define	pl33_sp_stand_hk	9	/* standing heavy kick (gamma assist and counter) */

/* atck_no: the character's own records in the hit table, after the common ATCK_ ones */
#define	pl33_at_jup_hp		0x12	/* Air heavy punch with up */
#define	pl33_at_jdown_hp	0x13	/* Air heavy punch with down */
#define	pl33_at_jup_hk		0x14	/* Air heavy kick with up */
#define	pl33_at_jdown_hk	0x15	/* Air heavy kick with down, high enough: the dive */
#define	pl33_at_upper		0x1c	/* The launcher (pl33_upper) */
#define	pl33_at_throw_k		0x20	/* Ground kick throw */
#define	pl33_at_air_throw	0x21	/* Air punch throw */
#define	pl33_at_throw_p		0x22	/* Ground punch throw */
#define	pl33_at_bomb		0x32	/* Smart Bomb, ground and air (him and the bomb, shl33_02) */
#define	pl33_at_repulsor	0x33	/* Repulsor Blast (sp_repulsor) */
#define	pl33_at_proton_beam	0x34	/* Proton Cannon's beams (shl33_04) */
#define	pl33_at_proton		0x3d	/* Proton Cannon (hyper, sp_proton): him and the cannon that drops (shl33_03) */
#define	pl33_at_t_kick		0x3f	/* The partner's jump-in attack (t_kick), a dash through the air */
#define	pl33_at_snapback	0x47	/* Snapback (sp_snapback) */

/* the character's own work, in tlocal */
typedef struct PL33_LOCAL_W {
	Uint8	beam_end_flag;		/* set when the Unibeam's shot ends */
	Uint8	land_flag;		/* set by the entrance's effect as its thing flies off; its piece on the ground ends */
	Uint8	bomb_cnt;		/* Smart Bombs out */
	Sint8	ready_flag;		/* set by shl33_03 when Proton Cannon is ready to fire */
	Uint16	fly_timer;		/* frames of flight left */
	Sint8	proton_end_flag;		/* set when Proton Cannon's beam ends */
} PL33_LOCAL_W;

#endif
