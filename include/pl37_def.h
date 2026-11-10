/* Jin's special moves. */
#ifndef PL37_DEF_H
#define PL37_DEF_H

/* pl_sp_type */
#define	pl37_sp_typhoon	0	/* Saotome Typhoon */
#define	pl37_sp_dynamite	1	/* Saotome Dynamite */
#define	pl37_sp_vulcan	2	/* Blodia Vulcan (hyper) */
#define	pl37_sp_blodia	3	/* Blodia Punch (hyper) */
#define	pl37_sp_cyclone	4	/* Saotome Cyclone (hyper) */
#define	pl37_sp_crush	5	/* Saotome Crash */
#define	pl37_sp_06		6
#define	pl37_sp_shine	7	/* Saotome Shine (at low vitality) */
#define	pl37_sp_chouhatsu	8	/* Taunt, which charges him up */
#define	pl37_sp_snapback	9	/* Snapback */
#define	pl37_sp_punch	10	/* Standing medium punch (assist) */

/* atck_no: the character's own records in the hit table, after the common ATCK_ ones */
#define	pl37_at_jdown_hk			0x12	/* Jumping HK with down, high enough: the dive, three hits (jump_hk_dive) */
#define	pl37_at_upper				0x15	/* Launcher, the standing punch */
#define	pl37_at_upper_k				0x16	/* Launcher, the crouching kick */
#define	pl37_at_upper_k_2			0x17	/* Crouching kick launcher's later hit (cg_type bit 8) */
#define	pl37_at_stand_mk_2			0x19	/* Standing MK's later hit, with the flame (cg_type bit 8) */
#define	pl37_at_crouch_mk_2			0x1a	/* Crouching MK's later hit, with the flame */
#define	pl37_at_jump_hk_2			0x1b	/* Jumping HK's later hit, with the flame */
#define	pl37_at_throw_k				0x20	/* Kick throw, mashed hits and the throw (nml_k_mash, catch_no 0) */
#define	pl37_at_throw_p				0x21	/* Punch throw, run to the screen's edge (nml_p_run) */
#define	pl37_at_crush_throw			0x22	/* Saotome Crash's grab slam (crush_c_throw, catch_no 2) */
#define	pl37_at_air_throw			0x23	/* Air throw, run to the edge (jump_p_run) */
#define	pl37_at_cyclone_throw		0x24	/* Saotome Cyclone's throw after the spin (cyclone_hold) */
#define	pl37_at_vulcan				0x36	/* Blodia Vulcan: Jin and the sparks (shl37_01 type 3, sub_type 0) */
#define	pl37_at_blodia_beam			0x37	/* Blodia Punch's beam, each hit as it runs out (shl37_02) */
#define	pl37_at_blodia_beam_end		0x38	/* Blodia Punch's beam at the far end */
#define	pl37_at_cyclone				0x39	/* Saotome Cyclone's spin, every 16 frames */
#define	pl37_at_t_kick				0x3a	/* Partner jump-in attack */
#define	pl37_at_shine				0x3b	/* Saotome Shine (sp_shine, at low vitality) */
#define	pl37_at_blodia				0x46	/* Blodia Punch, Jin himself (blodia_init) */
#define	pl37_at_vulcan_flame_1		0x48	/* Blodia Vulcan's flame, flame_no 0 (shl37_01 type 7) */
#define	pl37_at_vulcan_flame_2		0x49	/* Blodia Vulcan's flame, flame_no 1 */
#define	pl37_at_vulcan_spark_2		0x4a	/* Blodia Vulcan's spark with a nonzero sub_type (shl37_01 type 3; vulcan_spark only sets 0) */
#define	pl37_at_blodia_beam_team	0x4b	/* The beam's end hit in a team hyper (team_type 4 or 5) */
#define	pl37_at_chouhatsu			0x4c	/* The taunt that charges him, each step (sp_chouhatsu) */
#define	pl37_at_crush				0x4d	/* Saotome Crash, the dash grab (crush_init) */
#define	pl37_at_snapback			0x51	/* Snapback (snapback_init, pl_snap_flash) */

/* the character's own work, in tlocal */
typedef struct PL37_LOCAL_W {
	Uint8	hit_timer;		/* Saotome Typhoon: frames to the next hit */
	Uint8	hit_wait_flag;	/* Saotome Typhoon: a hit waits for hit_timer */
	Uint8	unk_02;
	Uint8	atck_alt;		/* Saotome Typhoon: which of the two attacks */
	Sint16	land_x;			/* the appearance: where he lands */
	Uint8	appear_flag;
	Uint8	appear_ef_flag;	/* the appearance: his effect (pls37_00) shows */
	Uint8	burning_flag;	/* Saotome Shine is on; kept between rounds */
	Uint8	col_step;		/* Saotome Dynamite: the colour step */
	Uint8	col_timer;		/* Saotome Dynamite: frames to the next step */
	Uint8	sway_dir;		/* Saotome Cyclone: which way he sways */
	Uint8	beam_flag;		/* Blodia Vulcan: the beam is out */
	Uint8	catch_flag;		/* Blodia Vulcan: the opponent is caught */
	Uint8	burn_flag;		/* Blodia Vulcan: the beam burns the caught opponent */
	Uint8	stand_hit_flag;	/* Blodia Vulcan: the opponent was hit standing */
	Sint16	rise_x;			/* Blodia Vulcan: where he took off */
	Uint8	shot_end_flag;	/* Blodia Punch: shl37_02 has run its time */
	Uint8	spin_end_flag;	/* Saotome Cyclone: the spin is over */
	Uint8	edge_flag;		/* a throw: he has reached the screen's edge */
	Uint8	unk_15;			/* Saotome Cyclone: hits while holding the opponent */
	Uint8	swing_flag;		/* Saotome Cyclone's hold: the opponent swings back */
	Uint8	swing_timer;	/* Saotome Cyclone's hold: frames to the next step */
	Float	swing_spd;		/* Saotome Cyclone's hold: the swing's speed */
	Sint16	swing_x;		/* Saotome Cyclone's hold: where the swing turns */
	Uint8	flame_no;		/* Blodia Vulcan: which flame is next */
	Uint8	charge_level;	/* the taunt: the level charged */
	Sint8	press_cnt;		/* the taunt: frames a button was held in this run */
	Sint8	burning_col;	/* Saotome Shine: its colour step */
	Sint8	burning_timer;	/* Saotome Shine: frames to the next colour step */
	Uint8	burning_end_flag;	/* Saotome Shine has run out; not again */
} PL37_LOCAL_W;
#define	PL37_LW( pl )	( ( PL37_LOCAL_W * )( pl )->tlocal )

#endif
