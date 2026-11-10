/* Sentinel's special moves. */
#ifndef PL34_DEF_H
#define PL34_DEF_H

/* animation numbers */
#define	CH34_WIN		0
#define	CH34_LOSE		1

/* pl_sp_type */
#define	pl34_sp_rocket		0	/* Rocket Punch */
#define	pl34_sp_plasma		1	/* Plasma Storm (hyper) */
#define	pl34_sp_force		2	/* Sentinel Force */
#define	pl34_sp_snapback	3	/* the snapback */
#define	pl34_sp_fly			4	/* Flight */
#define	pl34_sp_stand_hk	5	/* standing heavy kick (beta assist and counter) */
#define	pl34_sp_hforce		6	/* Hyper Sentinel Force (hyper) */
#define	pl34_sp_chouhatsu	7	/* the taunt */
#define	pl34_sp_hdrive		8	/* Hard Drive (hyper) */
#define	pl34_sp_rocket2		9	/* Rocket Punch at its strongest */

/* atck_no: the character's own records in the hit table, after the common ATCK_ ones */
#define	pl34_at_stand_mp_2	0x13	/* Second hit of the standing medium punch */
#define	pl34_at_crouch_mp_2	0x14	/* Second hit of the crouching medium punch */
#define	pl34_at_throw		0x20	/* The throws (ground and air): the grabbing fist slams the opponent into the wall (shl34_00_catch_fly) */
#define	pl34_at_force		0x21	/* Sentinel Force (sp_force): him and the drones (shl34_01 types 0 and 1) */
#define	pl34_at_force_shot	0x22	/* The shot a drone drops (shl34_02) */
#define	pl34_at_rocket2		0x2e	/* Strongest Rocket Punch's fist on the ground (shl34_00 case 2); plasma_init (Plasma Storm) also sets it */
#define	pl34_at_jrocket2	0x2f	/* Strongest Rocket Punch's fist in the air (shl34_00 case 5) */
#define	pl34_at_rocket		0x30	/* Rocket Punch: him, and the ground heavy fist (also the Variable Counter's light fist) */
#define	pl34_at_rocket_l	0x31	/* Rocket Punch's ground light fist */
#define	pl34_at_jrocket		0x32	/* Rocket Punch's air heavy fist */
#define	pl34_at_jrocket_l	0x33	/* Rocket Punch's air light fist */
#define	pl34_at_snapback	0x34	/* Snapback (sp_snapback), a kick that leaves the heavy kick's effect (also shl34_03 type 2) */
#define	pl34_at_upper		0x35	/* The kick launcher (pl34_upper, atck_pk 1); also sp_stand_hk's standing heavy kick as Variable Counter (team_type 3) */
#define	pl34_at_t_kick		0x36	/* The partner's jump-in attack (t_kick) */
#define	pl34_at_plasma		0x37	/* Plasma Storm's blow in front of him (shl34_04, set by plasma_wait) */
#define	pl34_at_hforce		0x39	/* Hyper Sentinel Force (sp_hforce): him and the drones (shl34_01 type 2) */
#define	pl34_at_hdrive		0x3a	/* Hard Drive's rush hits (sp_hdrive) */
#define	pl34_at_hdrive_end	0x3b	/* Hard Drive's last blow at the end of the rush */

/* the character's own work, in tlocal */
typedef struct PL34_LOCAL_W {
	Sint16	fly_timer;		/* counts down while fly_flag is set */
	Uint8	unk_02[2];
	Uint8	fist_end_flag;	/* the rocket fist is gone, or could not be fired */
	Uint8	drone_cnt;		/* drones (shl34_01) out */
	Uint8	unk_06;
	Uint8	unk_07;
	Uint8	color_flag;		/* flight has changed the colour */
	Sint8	punch_cg;		/* the heavy punch's cg_type for shl34_03; -1 ends it */
	Uint8	unk_0a;
	Uint8	unk_0b;
	Uint8	unk_0c;
	Uint8	unk_0d;
	Uint8	unk_0e;
	Uint8	unk_0f;
	Uint8	kick_timer;		/* frames the heavy kick's effect lasts */
	Uint8	unk_11;
	CHAR_NO	kick_char;		/* the animation kick_timer belongs to */
} PL34_LOCAL_W;

#endif
