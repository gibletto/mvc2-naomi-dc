/* Morrigan's special moves. */
#ifndef PL03_DEF_H
#define PL03_DEF_H

/* pl_sp_type */
#define	pl03_sp_soul			1	/* Soul Fist */
#define	pl03_sp_blade			2	/* Shadow Blade */
#define	pl03_sp_vector			3	/* Vector Drain */
#define	pl03_sp_illusion		6	/* Darkness Illusion (hyper) */
#define	pl03_sp_shower			7	/* Finishing Shower (hyper) */
#define	pl03_sp_chouhatsu		8	/* The taunt */
#define	pl03_sp_silhouette		9	/* Silhouette Blade (hyper) */
#define	pl03_sp_snapback		14	/* The snapback */
#define	pl03_sp_shell			15	/* Shell Kick (in the air) */

/* atck_no: the character's own records in the hit table, after the common ATCK_ ones */
#define	pl03_at_jdown_hk		0x12	/* Jump HK with the stick down (stops the jump) */
#define	pl03_at_fwd_hk			0x13	/* Stand HK with the stick forward */
#define	pl03_at_shell			0x17	/* Shell Kick in the air (sp_shell) */
#define	pl03_at_crouch_lp_2		0x1b	/* Crouch LP, second hit */
#define	pl03_at_dback_hp		0x1c	/* Crouch HP with the stick back */
#define	pl03_at_throw_p			0x20	/* Punch throw */
#define	pl03_at_vector			0x21	/* Vector Drain's throw (vector_catch_leave) */
#define	pl03_at_air_throw_p		0x22	/* Punch air throw */
#define	pl03_at_throw_k			0x23	/* Kick throw's grip (pls03_01) */
#define	pl03_at_blade_h			0x31	/* Shadow Blade, heavy (48+level); also its variable counter */
#define	pl03_at_upper		0x36	/* The launcher (pl03_upper) */
#define	pl03_at_soul			0x37	/* Soul Fist on the ground (Morrigan and shl03_00) */
#define	pl03_at_jsoul			0x38	/* Soul Fist in the air (Morrigan and shl03_00) */
#define	pl03_at_t_kick			0x39	/* Tag-in jumping kick (t_kick_init) */
#define	pl03_at_snapback		0x3a	/* Snapback (sp_snapback) */
#define	pl03_at_illusion		0x3b	/* Darkness Illusion's dash, ground and air */
#define	pl03_at_illusion_air	0x3f	/* Darkness Illusion's hits in the air (cg_type 63) */
#define	pl03_at_jillusion_end	0x44	/* Darkness Illusion in the air: the last hit */
#define	pl03_at_shower			0x45	/* Finishing Shower (sp_shower, on Morrigan) and its shots' heads and tails */
#define	pl03_at_shower_hit		0x46	/* A Finishing Shower head's hit on its animation's mark */
#define	pl03_at_shower_next		0x47	/* Finishing Shower shots put out by an effect (pls03_00): head, tail and burst */
#define	pl03_at_silhouette		0x48	/* Silhouette Blade (hyper, sp_silhouette) */

/* the character's own work, in tlocal */
typedef struct PL03_LOCAL_W {
	PLAYER_WORK	*em_w;		/* the opponent caught */
	PLAYER_WORK	*double_w;	/* the double of her throw (shl03_03) */
	Uint8	unk_08[0x0a - 0x08];
	Uint16	dash_sw;		/* the stick when an air dash starts */
	Sint8	bat_mode;		/* the mode the bats (pls03_02) follow */
	Uint8	double_flag;	/* set by the double (shl03_03) when it has appeared, and when it is gone */
	Sint8	drop_flag;		/* tells the double to drop */
	Sint8	kick_cnt;		/* extra kicks */
	Sint8	release_flag;	/* set when the hyper throw's last hit lets the opponent go */
	Uint8	unk_11;
	Sint8	col_timer;		/* a colour change counting down */
	Sint8	gacha_flag;		/* the stick is shaken this frame; the flashes (pls03_00) turn faster */
	Uint8	renda_cnt;		/* presses left to make a hyper combo last longer */
	Uint8	shot_cnt;		/* shots still out */
	Sint8	spark_no;		/* which pair of hit sparks */
	Uint8	unk_17[0x19 - 0x17];
	Uint8	dash_flag;		/* in an air dash */
	Uint8	shot_flag;		/* a Soul Fist (shl03_00) is out */
	Sint8	leave_flag;		/* set when the throw's grip (pls03_01) lets go */
	Uint8	dash_ef_flag;	/* the dash effect is on */
} PL03_LOCAL_W;

#endif
