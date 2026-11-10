/* Guile's animation numbers and special moves. */
#ifndef PL02_DEF_H
#define PL02_DEF_H

#define	CH02_WIN_KO0	0
#define	CH02_WIN_KO1	1
#define	CH02_WIN_KO2	2
#define	CH02_WIN_KO3	3
#define	CH02_WIN_KO4	4
#define	CH02_LOSE		5
#define	CH02_TM_DRAW	6

/* pl_sp_type */
#define	pl02_sp_sonic			0	/* Sonic Boom */
#define	pl02_sp_smr				1	/* Somersault Kick */
#define	pl02_sp_jsmr			2	/* Somersault Kick in the air */
#define	pl02_sp_strike			3	/* Somersault Strike (hyper) */
#define	pl02_sp_hurricane		4	/* Sonic Hurricane (hyper) */
#define	pl02_sp_snapback		5	/* The snapback */
#define	pl02_sp_chouhatsu		6	/* The taunt */
#define	pl02_sp_crossfire			7	/* Crossfire Assault (hyper, in the air) */

/* atck_no: the character's own records in the hit table, after the common ATCK_ ones */
#define	pl02_at_fwd_hp				0x12	/* Stand HP with the stick forward */
#define	pl02_at_back_hk				0x13	/* Stand HK with the stick back */
#define	pl02_at_back_lk				0x14	/* Stand LK with the stick back */
#define	pl02_at_fwd_hk				0x15	/* Stand HK with the stick forward */
#define	pl02_at_upper			0x16	/* The launcher (pl02_upper) */
#define	pl02_at_crouch_hk_2			0x19	/* Crouch HK, second hit */
#define	pl02_at_throw_k				0x20	/* Kick throw */
#define	pl02_at_throw_p				0x21	/* Punch throw */
#define	pl02_at_air_throw_k			0x22	/* Kick air throw: the throw after landing */
#define	pl02_at_air_throw_p			0x23	/* Punch air throw */
#define	pl02_at_air_throw_k_hit		0x24	/* Kick air throw: the hit on landing (cg_type < 0) */
#define	pl02_at_sonic				0x30	/* Sonic Boom: set on Guile as he throws it */
#define	pl02_at_strike				0x35	/* Somersault Strike (hyper): the first two kicks */
#define	pl02_at_strike_end			0x36	/* Somersault Strike's last kick */
#define	pl02_at_t_kick				0x37	/* Tag-in jumping kick (t_kick) */
#define	pl02_at_strike_crescent		0x3a	/* Somersault Kick crescent from Somersault Strike (shl02_06 type 1) */
#define	pl02_at_hurricane_3			0x3b	/* Sonic Hurricane: every fourth of the boom's six-frame hits */
#define	pl02_at_hurricane			0x3c	/* Sonic Hurricane's start: on Guile, the boom's first hit; also shl02_04's repeated hit */
#define	pl02_at_vc_smr				0x3d	/* Somersault Kick as a variable counter */
#define	pl02_at_vc_smr_crescent		0x3f	/* Somersault Kick crescent as a variable counter (shl02_06) */
#define	pl02_at_snapback			0x40	/* Snapback (sp_snapback) */
#define	pl02_at_hurricane_end		0x41	/* Sonic Hurricane's last hit */
#define	pl02_at_far_lp				0x42	/* Far stand LP */
#define	pl02_at_far_mp				0x43	/* Stand MP (no close check; the MP slot of the 0x42-0x47 far block) */
#define	pl02_at_back_hp				0x44	/* Stand HP with the stick back */
#define	pl02_at_far_lk				0x45	/* Far stand LK */
#define	pl02_at_far_mk				0x46	/* Stand MK (no close check; the MK slot of the far block) */
#define	pl02_at_far_hk				0x47	/* Far stand HK */
#define	pl02_at_crossfire			0x48	/* Crossfire Assault (hyper) */
#define	pl02_at_crossfire_crescent	0x4e	/* Somersault Kick crescent from Crossfire Assault (shl02_06 type 2) */
#define	pl02_at_hurricane_2			0x50	/* Sonic Hurricane: the boom's other six-frame hits */

/* the character's own work, in tlocal */
typedef struct PL02_LOCAL_W {
	Uint8	hurricane_flag;	/* 0: the Sonic Hurricane (shl02_01) is out */
	Uint8	em_type;	/* the opponent's character when the hyper combo caught it */
	Uint8	shot_flag;	/* 2: a Sonic Boom (shl02_00) is out */
} PL02_LOCAL_W;

#endif
