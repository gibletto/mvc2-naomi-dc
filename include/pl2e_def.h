/* War Machine's special moves. */
#ifndef PL2E_DEF_H
#define PL2E_DEF_H

/* pl_sp_type */
#define	pl2e_sp_repulsor	0	/* Repulsor Blast */
#define	pl2e_sp_proton	1	/* Proton Cannon (hyper) */
#define	pl2e_sp_cannon	2	/* Shoulder Cannon */
#define	pl2e_sp_bomb	3	/* Smart Bomb */
#define	pl2e_sp_chouhatsu	4	/* Taunt */
#define	pl2e_sp_fly	5	/* Flight */
#define	pl2e_sp_destroyer	6	/* War Destroyer (hyper) */
#define	pl2e_sp_snapback	8	/* Snapback */
#define	pl2e_sp_ccannon	12	/* Shoulder Cannon, crouching */

/* atck_no: the character's own records in the hit table, after the common ATCK_ ones */
#define	pl2e_at_jup_hp			0x12	/* Air heavy punch with up */
#define	pl2e_at_jdown_hp		0x13	/* Air heavy punch with down */
#define	pl2e_at_jup_hk			0x14	/* Air heavy kick with up */
#define	pl2e_at_jdown_hk		0x15	/* Air heavy kick with down, high enough: the dive */
#define	pl2e_at_upper			0x1c	/* The launcher (pl2e_upper) */
#define	pl2e_at_throw_k			0x20	/* Ground kick throw */
#define	pl2e_at_air_throw		0x21	/* Air punch throw */
#define	pl2e_at_throw_p			0x22	/* Ground punch throw */
#define	pl2e_at_crouch_hp_2		0x30	/* Crouching heavy punch beam: the parts flying off it (shl2e_00 type 1) */
#define	pl2e_at_bomb			0x32	/* Smart Bomb, ground and air (him and the shell, shl2e_02) */
#define	pl2e_at_proton_beam		0x34	/* Laser row of shl2e_04 (Iron Man's Proton Cannon beam); shl2e_04 is never set */
#define	pl2e_at_crouch_hp		0x35	/* Crouching heavy punch: the beam (him and shl2e_00 type 0) */
#define	pl2e_at_repulsor		0x3c	/* Repulsor Blast (sp_repulsor) */
#define	pl2e_at_proton			0x3d	/* Proton Cannon (hyper): him and the cannon that drops (shl2e_03) */
#define	pl2e_at_destroyer		0x3e	/* War Destroyer (hyper): him and the bombs going up (shl2e_05 type 0) */
#define	pl2e_at_t_kick			0x3f	/* The partner's jump-in attack (t_kick), a dash through the air */
#define	pl2e_at_destroyer_2		0x40	/* War Destroyer bomb's fast drop onto the target */
#define	pl2e_at_snapback		0x41	/* Snapback (sp_snapback) */
#define	pl2e_at_proton_shot		0x42	/* Proton Cannon's shots, first (shl2e_06 type 1, sub_type 0) */
#define	pl2e_at_proton_shot2	0x43	/* Proton Cannon's shots, the rest (shl2e_06 type 1, sub_type != 0) */
#define	pl2e_at_cannon			0x44	/* Shoulder Cannon: standing, air and crouching (sp_cannon, sp_ccannon) and its shots (shl2e_05 type 2) */

/* the character's own work, in tlocal */
typedef struct PL2E_LOCAL_W {
	Uint8	beam_end_flag;	/* set when the crouching heavy punch's beam ends */
	Uint8	land_flag;		/* set by the entrance's effect as its thing flies off; its piece on the ground ends */
	Uint8	bomb_cnt;		/* Smart Bombs out */
	Uint8	ready_flag;		/* set by shl2e_03 when Proton Cannon is ready to fire */
	Uint16	fly_timer;		/* frames of flight left */
	Uint8	proton_end_flag;	/* set when Proton Cannon's beam ends */
	Uint8	unk_07;
	Uint8	shot_cnt;		/* shots left to fire */
} PL2E_LOCAL_W;

#endif
