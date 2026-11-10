/* Magneto's special moves. */
#ifndef PL2C_DEF_H
#define PL2C_DEF_H

/* pl_sp_type */
#define	pl2c_sp_disrupt	0	/* EM Disruptor */
#define	pl2c_sp_grav	1	/* Hyper Grav */
#define	pl2c_sp_shock	2	/* Magnetic Shockwave (hyper) */
#define	pl2c_sp_tempest	3	/* Magnetic Tempest (hyper) */
#define	pl2c_sp_fly	4	/* Flight */
#define	pl2c_sp_chouhatsu	6	/* Taunt */
#define	pl2c_sp_jblast	7	/* Magnetic Blast (air) */
#define	pl2c_sp_field	8	/* Magnetic Forcefield: catches the one who hits it */
#define	pl2c_sp_snapback	12	/* Snapback */
#define	pl2c_sp_crouch_hp		13	/* crouching heavy punch (gamma assist) */

/* atck_no: the character's own records in the hit table, after the common ATCK_ ones */
#define	pl2c_at_jdown_lk	0x12	/* Air light kick with down: the dive */
#define	pl2c_at_stand_hk_2	0x19	/* Second hit of the standing heavy kick */
#define	pl2c_at_grav_free	0x25	/* Hyper Grav magnet lets the held opponent go (shl2c_03_release) */
#define	pl2c_at_disrupt		0x30	/* EM Disruptor, light (him and the spark, shl2c_00 type 2); +2 for the heavy one */
#define	pl2c_at_grav		0x33	/* Hyper Grav on the ground (him and the magnets, shl2c_03) */
#define	pl2c_at_shock		0x34	/* Magnetic Shockwave (hyper; him and shl2c_02's columns) */
#define	pl2c_at_tempest		0x35	/* Magnetic Tempest (hyper; him and the shl2c_01 sparks, 1 frame in 4) */
#define	pl2c_at_jblast		0x36	/* Magnetic Blast in the air, light (him and the shl2c_00 type 0 bolt); +2 for the heavy one */
#define	pl2c_at_tempest_2	0x39	/* Magnetic Tempest's other spark hit (shl2c_01, 3 frames in 4) */
#define	pl2c_at_field		0x3a	/* Magnetic Forcefield's wave after it catches (shl2c_02 types 3 and 4, from field_catch) */
#define	pl2c_at_jgrav		0x3b	/* Hyper Grav's magnets in the air (shl2c_03) */
#define	pl2c_at_t_kick		0x3d	/* The partner's jump-in attack (t_kick) */
#define	pl2c_at_snapback	0x3f	/* Snapback (sp_snapback) */
#define	pl2c_at_crouch_hp	0x40	/* Crouching heavy punch as gamma assist (sp_crouch_hp) */
#define	pl2c_at_upper		0x41	/* The launcher (pl2c_upper) */

/* the character's own work, in tlocal */
typedef struct PL2C_LOCAL_W {
	Uint16	catch_sw;		/* the stick when a throw starts */
	Uint8	unk_02[2];
	Sint16	catch_ng_timer;	/* frames left that he can't be thrown */
	Sint16	fly_timer;		/* frames of flight left */
	Sint8	hold_flag;		/* -1: a Hyper Grav magnet has caught the opponent */
	Uint8	unk_09[0x0c - 0x09];
	Uint8	shot_flag;		/* set while the first bolt of a shot is out */
	Uint8	catch_ng_flag;	/* set: counts catch_ng_timer down */
	Sint8	magnet_flag;	/* a bit for each Hyper Grav magnet that is out */
} PL2C_LOCAL_W;

#define PL2C_LW( pl )	( ( PL2C_LOCAL_W * )( pl )->tlocal )

#endif
