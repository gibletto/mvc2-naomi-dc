/* Captain Commando's special moves. */
#ifndef PL38_DEF_H
#define PL38_DEF_H

/* pl_sp_type */
#define	pl38_sp_sword	0	/* Captain Sword (hyper) */
#define	pl38_sp_fire	1	/* Captain Fire */
#define	pl38_sp_corridor	2	/* Captain Corridor */
#define	pl38_sp_jfire	4	/* Captain Fire in the air */
#define	pl38_sp_storm	6	/* Captain Storm (hyper) */
#define	pl38_sp_chouhatsu	7	/* Taunt */
#define	pl38_sp_strike	8	/* Commando Strike */
#define	pl38_sp_kick	12	/* Captain Kick */
#define	pl38_sp_snapback	13	/* Snapback */

/* atck_no: the character's own records in the hit table, after the common ATCK_ ones */
#define	pl38_at_stand_hk		0x19	/* Standing HK has its own record instead of 5 */
#define	pl38_at_throw_p			0x20	/* Ground punch throw's grab (catch_no 0, 4, 5) */
#define	pl38_at_air_throw		0x21	/* Air throw (jump_p_catch) */
#define	pl38_at_throw_k			0x23	/* Kick throw's mashed hold and last throw (catch_no 2) */
#define	pl38_at_throw_prop		0x26	/* The helper's hits in the punch throw of catch_no 4 (shl38_00 type 16) */
#define	pl38_at_fire_l			0x30	/* Captain Fire on the ground, light */
#define	pl38_at_corridor_l		0x31	/* Captain Corridor, light */
#define	pl38_at_t_kick			0x32	/* Partner jump-in attack */
#define	pl38_at_jfire_l			0x33	/* Captain Fire in the air, light */
#define	pl38_at_sword			0x34	/* Captain Sword: Commando and shl38_02 */
#define	pl38_at_storm			0x3d	/* Captain Storm's rush, up to five hits */
#define	pl38_at_storm_helper_1	0x3e	/* Captain Storm: helper type 1 flying across the opponent (shl38_01 type 1) */
#define	pl38_at_storm_helper	0x3f	/* Captain Storm: helper types 0 and 2 flying across the opponent */
#define	pl38_at_storm_meet		0x40	/* Captain Storm when partner_get finds a partner of the same character (storm_meet) */
#define	pl38_at_strike_l		0x42	/* Commando Strike, strike_level 0: Commando and the helper (shl38_00 type 5) */
#define	pl38_at_strike_h		0x43	/* Commando Strike, strike_level 1: Commando and the helper (shl38_00 type 6) */
#define	pl38_at_strike_x		0x44	/* Commando Strike, strike_level 2 (the HP+LK command): Commando, the helper and its shot (shl38_00 types 7 and 9) */
#define	pl38_at_throw_p_toss2	0x45	/* Ground throw catch_no 5: tossing the opponent (catch5_toss) */
#define	pl38_at_throw_p_toss	0x46	/* Ground throw catch_no 0: throwing the opponent down (catch0_toss) */
#define	pl38_at_fire_h			0x4a	/* Captain Fire on the ground, heavy */
#define	pl38_at_jfire_h			0x4c	/* Captain Fire in the air, heavy */
#define	pl38_at_corridor_h		0x4e	/* Captain Corridor, heavy */
#define	pl38_at_snapback		0x57	/* Snapback */
#define	pl38_at_upper			0x58	/* Launcher (pl38_upper) */

/* the character's own work, in tlocal */
typedef struct PL38_LOCAL_W {
	Sint8	wait_timer;		/* a count down */
	Sint8	flash_wait;		/* what wait_timer starts again from when flashing */
	Sint8	unk_02;			/* bit 0: flashing; a count down in Captain Storm */
	Sint8	hit_cnt;		/* Captain Storm's hits */
	Sint8	rush_result;	/* Captain Storm for shl38_01: 1 hit, -1 missed; then counts its works done */
	Sint8	rush_hit_flag;	/* Captain Storm has hit */
	Sint8	catch_sel;		/* the throw chosen at random */
	Uint8	shot_timer;		/* kept at 2 while his helpers are out: no Captain Sword, Captain Storm, Commando Strike or helper throw */
	Sint16	strike_level;		/* Commando Strike's strength */
	Uint16	catch_lever;	/* the stick when a throw was tried */
} PL38_LOCAL_W;

#define	PL38_LW( pl )	( ( PL38_LOCAL_W * )( pl )->tlocal )

#endif
