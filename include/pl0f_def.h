/* Doctor Doom's special moves. */
#ifndef PL0F_DEF_H
#define PL0F_DEF_H

/* pl_sp_type */
#define	pl0f_sp_plasma	0	/* Plasma Beam */
#define	pl0f_sp_photon	1	/* Photon Shot */
#define	pl0f_sp_shield	2	/* Molecular Shield */
#define	pl0f_sp_array	3	/* Photon Array (hyper) */
#define	pl0f_sp_cage	4	/* Electric Cage (hyper) */
#define	pl0f_sp_flame	5	/* Sphere Flame (hyper) */
#define	pl0f_sp_chouhatsu	6	/* Taunt */
#define	pl0f_sp_snapback	7	/* Snapback */
#define	pl0f_sp_fly	8	/* Flight */

/* atck_no: the character's own records in the hit table, after the common ATCK_ ones */
#define	pl0f_at_air_hk		0x12	/* Jumping HK without stick-down (Char_Extra 3); the down+HK dive keeps ATCK_JUMP_HK */
#define	pl0f_at_stand_hk_2	0x19	/* Standing HK's second hit */
#define	pl0f_at_upper_p		0x1a	/* Launcher, punch version (pl0f_upper, Char_Extra 6) */
#define	pl0f_at_upper_k		0x1b	/* Launcher, kick version (pl0f_upper, atck_pk set, Char_Extra 7) */
#define	pl0f_at_throw_p		0x20	/* Ground punch throw (nml_p_throw) */
#define	pl0f_at_throw_k		0x21	/* Ground kick throw (nml_k_catch) */
#define	pl0f_at_air_throw_p	0x22	/* Air punch throw (jump_p_throw) */
#define	pl0f_at_air_throw_k	0x23	/* Air kick throw (jump_k_catch) */
#define	pl0f_at_photon		0x30	/* Photon Shot's shots (shl0f_02 type 0) */
#define	pl0f_at_shield		0x36	/* Molecular Shield's rocks (shl0f_05) */
#define	pl0f_at_array		0x38	/* Photon Array (hyper): its shots (shl0f_02 type 1), also set on Doom when it starts in the air */
#define	pl0f_at_t_kick		0x39	/* The partner's jump-in attack (pl0f_t_kick) */
#define	pl0f_at_cage		0x3a	/* Electric Cage (hyper): the flying cage and its hits while it holds (cage_init, shl0f_00) */
#define	pl0f_at_cage_last	0x3b	/* Electric Cage's last hit (shl0f_00_hold, timer out) */
#define	pl0f_at_flame		0x3c	/* Sphere Flame (hyper): a hit every 8 frames (shl0f_01) */
#define	pl0f_at_flame_last	0x3d	/* Sphere Flame's last hit (shl0f_01_hit, timer0 8) */
#define	pl0f_at_snapback	0x3e	/* Snapback (sp_snapback) */
#define	pl0f_at_photon_gnd	0x3f	/* Set on Doom as Photon Shot starts on the ground (photon_gnd_init); the shots use 0x30 */
#define	pl0f_at_flame_start	0x40	/* Set on Doom as Sphere Flame starts (flame_init) */
#define	pl0f_at_array_gnd	0x41	/* Set on Doom as Photon Array starts on the ground (array_gnd_init); the shots use 0x38 */

/* the character's own work, in tlocal */
typedef struct PL0F_LOCAL_W {
	Uint8	unk_00[4];
	Uint8	cage_end_flag;	/* the Electric Cage (shl0f_00) is over */
	Uint8	cage_hit_flag;	/* the Electric Cage has hit */
	Uint8	flame_end_flag;	/* the Sphere Flame (shl0f_01) is over */
	Uint8	shot_cnt;		/* bursts of shots left */
	Uint8	beam_stop_flag;	/* stops the air beam (shl0f_04) */
	Uint8	shield_fly_flag;	/* sends the Molecular Shield's rocks (shl0f_05) off */
	Uint16	fly_timer;		/* frames of flight left */
} PL0F_LOCAL_W;

#define PL0F_LW( pl )	( ( PL0F_LOCAL_W * )&( pl )->tlocal[0] )

#endif
