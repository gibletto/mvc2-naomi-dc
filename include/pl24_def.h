/* Cammy's special moves. */
#ifndef PL24_DEF_H
#define PL24_DEF_H

#define	CH24_WIN_KO0	0
#define	CH24_WIN_KO_E	1	/* against Bison */
#define	CH24_WIN_KO2	2
#define	CH24_WIN_KO1	3
#define	CH24_WIN_TIME_E	4
#define	CH24_TM_DRAW	5
#define	CH24_LOSE		6

/* pl_sp_type */
#define	pl24_sp_spike	0	/* Cannon Spike */
#define	pl24_sp_arrow	1	/* Spiral Arrow */
#define	pl24_sp_knuckle	2	/* Axle Spin Knuckle */
#define	pl24_sp_hooligan	3	/* Hooligan Combination */
#define	pl24_sp_chouhatsu	5	/* Taunt */
#define	pl24_sp_revenge	6	/* Cannon Revenge */
#define	pl24_sp_smasher	7	/* Spin Drive Smasher (hyper) */
#define	pl24_sp_strike	8	/* Cannon Strike */
#define	pl24_sp_drill	9	/* Cannon Drill */
#define	pl24_sp_killerbee	11	/* Killer Bee Assault (hyper) */
#define	pl24_sp_snapback	13	/* Snapback */
#define	pl24_sp_shaft	14	/* Reverse Shaft Breaker (hyper) */

/* atck_no: the character's own records in the hit table, after the common ATCK_ ones */
#define	pl24_at_stand_lp			0x12	/* Her standing LP; she never uses the common 0x00 */
#define	pl24_at_stand_mp			0x13	/* Her standing MP (second press); she never uses the common 0x01 */
#define	pl24_at_stand_hp			0x14	/* Her standing HP; she never uses the common 0x02 */
#define	pl24_at_stand_mk			0x16	/* Her standing MK (second press of a kick) */
#define	pl24_at_back_hk				0x17	/* Back + HK command normal (the code tests KEY_BACK) */
#define	pl24_at_throw_k				0x20	/* K ground throw (catch_no 0) */
#define	pl24_at_air_throw_k			0x21	/* K air throw (catch_no 1) */
#define	pl24_at_air_throw_p			0x22	/* P air throw (catch_no 2) */
#define	pl24_at_hooligan_throw_h	0x23	/* Hooligan Combination throw on an airborne or low-held opponent (catch_no 0xc3) */
#define	pl24_at_hooligan_throw_l	0x24	/* Hooligan Combination throw on a standing or crouching opponent (catch_no 0xc4) */
#define	pl24_at_throw_p				0x25	/* P ground throw (catch_no 5) */
#define	pl24_at_killerbee_throw		0x26	/* Killer Bee Assault's final throw (set on the thrown opponent) */
#define	pl24_at_t_kick				0x3c	/* Partner's wall-leap kick (t_kick) */
#define	pl24_at_smasher_1			0x40	/* Spin Drive Smasher hyper: the spinning dash */
#define	pl24_at_smasher_2			0x41	/* Spin Drive Smasher hyper: the rising kick */
#define	pl24_at_revenge				0x46	/* Cannon Revenge counter kick, lower level */
#define	pl24_at_killerbee			0x4e	/* Killer Bee Assault hyper: the first dash (also the level entry in the direction table) */
#define	pl24_at_killerbee_dive		0x5c	/* Killer Bee Assault: a dive at the opponent from the wall (killerbee_dive_init) */
#define	pl24_at_killerbee_last		0x5d	/* Killer Bee Assault: the last hit, which catches (catch_no 0xc6) */
#define	pl24_at_snapback			0x5e	/* Snapback (sp_snapback) */
#define	pl24_at_upper				0x5f	/* Launcher (pl24_upper, Char_Extra 3) */
#define	pl24_at_hooligan_kick		0x60	/* Hooligan Combination: the kick down when K is pressed */
#define	pl24_at_revenge_spike		0x61	/* Cannon Revenge, higher level: the rising Cannon Spike */
#define	pl24_at_shaft_1				0x62	/* Reverse Shaft Breaker hyper: start and first rising hits */
#define	pl24_at_shaft_2				0x63	/* Reverse Shaft Breaker: the repeated rising hits */
#define	pl24_at_shaft_3				0x64	/* Reverse Shaft Breaker: the hit at the top */

/* the character's own work, in tlocal */
typedef struct PL24_LOCAL_W {
	Float	start_y;	/* 00: the height Cannon Drill started at */
	Float	wall_x;		/* 04: x from the stage's middle, while on a wall */
	Float	wall_y;		/* 08: y from the stage's top, while on a wall */
	Uint8	hit_cnt;	/* 0c: hits left in Killer Bee Assault */
	Uint8	unk_0d;
	Sint8	last_dir;	/* 0e: the last direction of Killer Bee Assault */
	Uint8	win_flag;	/* 0f: set when the win pose at the stage's middle may go on */
} PL24_LOCAL_W;

#define	PL24_LW( pl )	( ( PL24_LOCAL_W * )&( pl )->tlocal[0] )

#endif
