/* Zangief's special moves. */
#ifndef PL01_DEF_H
#define PL01_DEF_H

/* pl_sp_type */
#define	pl01_sp_lariat			0	/* Double Lariat */
#define	pl01_sp_qlariat			1	/* Quick Double Lariat */
#define	pl01_sp_banish			2	/* Banishing Flat */
#define	pl01_sp_screw			3	/* Spinning Pile Driver (ground and air) */
#define	pl01_sp_powerbomb			4	/* Flying Powerbomb (Atomic Suplex up close) */
#define	pl01_sp_fab				5	/* Final Atomic Buster (hyper) */
#define	pl01_sp_russian		6	/* Aerial Russian Slam */
#define	pl01_sp_team			8	/* His part in a team hyper combo */
#define	pl01_sp_chouhatsu		9	/* The taunt */
#define	pl01_sp_uab				17	/* Ultra Final Atomic Buster (hyper) */
#define	pl01_sp_iron			18	/* Iron Body: Mecha Zangief and back (hyper) */
#define	pl01_sp_vodka			19	/* Vodka Fire */
#define	pl01_sp_blizzard		20	/* Siberian Blizzard (Mecha Zangief's hyper) */
#define	pl01_sp_snapback		21	/* The snapback */

/* atck_no: the character's own records in the hit table, after the common ATCK_ ones */
#define	pl01_at_jfwd_hp				0x12	/* Jump HP with the stick forward */
#define	pl01_at_jup_mp				0x13	/* Jump MP with the stick up (ends on landing) */
#define	pl01_at_jdown_hp			0x14	/* Jump HP with the stick down (ends on landing) */
#define	pl01_at_jdown_lp			0x15	/* Jump LP with the stick down: the dive to the floor */
#define	pl01_at_jdown_lk			0x16	/* Jump LK with the stick down (ends on landing) */
#define	pl01_at_back_lp				0x17	/* Stand LP with the stick back (step, screen shake, turns round) */
#define	pl01_at_upper			0x18	/* The launcher (pl01_upper) */
#define	pl01_at_stand_hk_2			0x19	/* Stand HK, second hit on the animation's mark */
#define	pl01_at_throw_p				0x20	/* Punch throw; also the punch that ends the running grab (dash_end) */
#define	pl01_at_air_throw			0x21	/* Air throw (punch) */
#define	pl01_at_com_throw				0x22	/* The command throw (catch_com_ck sets catch_no 9) */
#define	pl01_at_throw_k				0x23	/* Kick throw: the hold's mashable hits and the throw */
#define	pl01_at_crouch_throw_k		0x24	/* Crouching kick throw (hold and throw) */
#define	pl01_at_jscrew				0x25	/* Spinning Pile Driver in the air (screw_air) */
#define	pl01_at_dash_throw			0x27	/* Running grab's release when its timer runs out */
#define	pl01_at_blizzard			0x28	/* Siberian Blizzard (Mecha Zangief's rising lariat hyper) */
#define	pl01_at_vodka				0x2b	/* Vodka Fire (Zangief and the breath shot shl01_01) */
#define	pl01_at_banish				0x30	/* Banishing Flat */
#define	pl01_at_lariat				0x32	/* Double Lariat on the ground */
#define	pl01_at_qlariat				0x33	/* Quick Double Lariat on the ground */
#define	pl01_at_jlariat				0x34	/* Double Lariat in the air */
#define	pl01_at_jqlariat			0x35	/* Quick Double Lariat in the air */
#define	pl01_at_t_kick				0x36	/* Tag-in jumping kick (pl01_t_kick) */
#define	pl01_at_team_1				0x37	/* His part in a team hyper (sp_team): first hit of the charge */
#define	pl01_at_team_2				0x38	/* The team hyper, second hit of the charge */
#define	pl01_at_snapback			0x3a	/* Snapback (sp_snapback) */
#define	pl01_at_russian_h			0x3c	/* Aerial Russian Slam, heavy (59+level); also its variable counter */
#define	pl01_at_powerbomb_h			0x40	/* Flying Powerbomb's grab hit, heavy (63+level); also its variable counter */
#define	pl01_at_powerbomb_slam_h	0x42	/* Flying Powerbomb's slam after the standing grab, heavy (65+level); also VC */
#define	pl01_at_powerbomb_run_h		0x44	/* Flying Powerbomb's slam after the grab at the end of the run, heavy (67+level); also VC */
#define	pl01_at_fab_1				0x45	/* Final Atomic Buster, first slam (fab_g_lift) */
#define	pl01_at_fab_2				0x46	/* Final Atomic Buster, second slam (fab_g_lift2) */
#define	pl01_at_fab_3				0x47	/* Final Atomic Buster, the throw up (fab_g_up) */
#define	pl01_at_uab_1				0x50	/* Ultra Final Atomic Buster: slam on landing (uab_g_land) */
#define	pl01_at_uab_2				0x51	/* Ultra Final Atomic Buster: lift (uab_g_lift) */
#define	pl01_at_uab_3				0x52	/* Ultra Final Atomic Buster: second landing (uab_g_land2) */
#define	pl01_at_uab_4				0x53	/* Ultra Final Atomic Buster: the throw up (uab_g_up) */

/* the character's own work, in tlocal */
typedef struct PL01_LOCAL_W {
	Uint16	lever;		/* 0: the stick and buttons (atck_lever) when a throw or lariat started */
	Sint8	mash;		/* 2: presses during a grab, which make the slam stronger */
	Uint8	breath_end;	/* 3: set when the breath is done */
	Uint8	kick_hit;	/* 4: set once the heavy kick has hit */
} PL01_LOCAL_W;

#endif
