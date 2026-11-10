/* Spiral's special moves. */
#ifndef PL31_DEF_H
#define PL31_DEF_H

/* pl_sp_type */
#define	pl31_sp_sword		0	/* Dancing Sword: the swords come out */
#define	pl31_sp_tobashi		1	/* Tsurugi Tobashi */
#define	pl31_sp_tele		2	/* Teleport Dance */
#define	pl31_sp_grapple		3	/* Six-hand Grapple */
#define	pl31_sp_metamor	4	/* Metamorphose (level 3 hyper): the speed up, then a punch's hyper throw */
#define	pl31_sp_snapback	5	/* the snapback */
#define	pl31_sp_chouhatsu	6	/* the taunt */
#define	pl31_sp_swords		7	/* the swords out and fired at once (alpha and beta assist and counter) */
#define	pl31_sp_stand_hp	8	/* standing heavy punch (gamma assist) */
#define	pl31_sp_crouch_hp	9	/* crouching heavy punch (gamma counter) */
#define	pl31_sp_power		10	/* Power Dance (hyper) */
#define	pl31_sp_speed		11	/* Speed Dance (hyper) */
#define	pl31_sp_hmetamor		12	/* Hyper Metamorphose: the gamma type's team hyper, the glow ending in a dash */
#define	pl31_sp_stampede		13	/* Stampede Sword (hyper, and the alpha and beta types' team hyper) */

/* atck_no: the character's own records in the hit table, after the common ATCK_ ones */
#define	pl31_at_jdown_mp	0x12	/* Air medium punch with down */
#define	pl31_at_jdown_hp	0x13	/* Air heavy punch with down */
#define	pl31_at_jup_mk		0x14	/* Air medium kick with up */
#define	pl31_at_upper		0x19	/* The launcher (pl31_upper), crouching */
#define	pl31_at_stand_hp_2	0x1a	/* Second hit of the standing heavy punch (also the gamma assist, sp_stand_hp) */
#define	pl31_at_throw_p		0x20	/* Ground punch throw's hits (nml_p_move) */
#define	pl31_at_air_throw_k	0x24	/* Air kick throw: slammed on landing */
#define	pl31_at_t_kick		0x30	/* The partner's jump-in attack (pl31_t_kick) */
#define	pl31_at_snapback	0x31	/* Snapback (sp_snapback) */
#define	pl31_at_tobashi		0x32	/* Tsurugi Tobashi: the swords fired (tobashi_init, the assist's swords_init, shl31_00) */
#define	pl31_at_metamor		0x33	/* Metamorphose's speed-up start (metamor_maxspeed_init) */
#define	pl31_at_metamor_1	0x34	/* Metamorphose's hyper throw, first hit (metamor_throw, cg_type 1) */
#define	pl31_at_metamor_2	0x35	/* Its second hit (cg_type 2) */
#define	pl31_at_metamor_3	0x36	/* Its third hit (cg_type 3) */
#define	pl31_at_hmetamor_1	0x38	/* Hyper Metamorphose (team hyper): the dash and first hit */
#define	pl31_at_hmetamor_2	0x39	/* Its second hit */
#define	pl31_at_hmetamor_3	0x3a	/* Its third hit */
#define	pl31_at_stampede	0x3b	/* Stampede Sword: her and the odd-numbered swords (shl31_01) */
#define	pl31_at_stampede_2	0x3c	/* Stampede Sword's even-numbered swords (shl31_01) */

/* the character's own work, in tlocal */
typedef struct PL31_LOCAL_W {
	Uint8	sword_flag[6];		/* the swords out */
	Uint8	fire_flag[6];		/* the swords fired */
	Uint8	fire_level;		/* the strength + 1 the swords are fired with */
	Uint8	glow;			/* the glow: 0 none, 1 power, 2 speed, 3 Maximum Speed */
	Sint16	glow_timer;		/* frames of glow left */
	Uint8	unk_10[4];
	Uint8	col_timer;		/* frames to the glow's next colour */
	Uint8	glow_col;		/* the glow's colour */
	Sint8	maxspeed_flag;		/* set during Maximum Speed */
} PL31_LOCAL_W;
#define PL31_LW( pl )	( ( PL31_LOCAL_W * )( pl )->tlocal )

#endif
