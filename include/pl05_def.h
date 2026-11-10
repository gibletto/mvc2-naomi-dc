/* Strider's special moves. */
#ifndef PL05_DEF_H
#define PL05_DEF_H

/* pl_sp_type */
#define	pl05_sp_murakumo			0	/* Ame no Murakumo */
#define	pl05_sp_warp			3	/* Warp */
#define	pl05_sp_ouroboros		4	/* Ouroboros (hyper) */
#define	pl05_sp_gram		5	/* Gram */
#define	pl05_sp_lgram		6	/* Low Gram */
#define	pl05_sp_jgram		7	/* Gram (in the air) */
#define	pl05_sp_kabe			8	/* Kabe Hari Tsuki */
#define	pl05_sp_formation_a		9	/* Formation A */
#define	pl05_sp_legion			10	/* Legion (hyper) */
#define	pl05_sp_jexcalibur		11	/* Excalibur (in the air) */
#define	pl05_sp_ragnarok		12	/* Ragnarok (hyper) */
#define	pl05_sp_vajra			13	/* Vajra */
#define	pl05_sp_formation_c			14	/* Formation C */
#define	pl05_sp_formation_b			15	/* Formation B */
#define	pl05_sp_formation_b_shot		16
#define	pl05_sp_chouhatsu		17	/* The taunt */
#define	pl05_sp_snapback		22	/* The snapback */

/* atck_no: the character's own records in the hit table, after the common ATCK_ ones */
#define	pl05_at_dfwd_hk				0x12	/* Crouch HK with the stick forward */
#define	pl05_at_throw_k				0x20	/* Kick throw */
#define	pl05_at_air_throw_p			0x21	/* Punch air throw */
#define	pl05_at_ragnarok_throw		0x22	/* Ragnarok's throw when the rush connects (ragnarok_up) */
#define	pl05_at_throw_p				0x23	/* Punch throw */
#define	pl05_at_t_kick				0x33	/* Tag-in kick off the wall (t_kick) */
#define	pl05_at_murakumo_2			0x34	/* Ame no Murakumo when pl_sp_type is not 0 (types 1, 2, 18-21; type 0 uses 0x30/0x32) */
#define	pl05_at_ouroboros			0x35	/* Ouroboros (hyper) */
#define	pl05_at_kabe_p				0x3f	/* Punch from the wall cling (stick_punch) */
#define	pl05_at_kabe_slide_k		0x43	/* Kick sliding down the wall (stick_slide) */
#define	pl05_at_formation_a_l		0x44	/* Formation A, light: the animal that runs in (shl05_02 type 0) */
#define	pl05_at_legion				0x45	/* Legion (hyper) and its running animals (shl05_02 type 1) */
#define	pl05_at_ouroboros_shot		0x46	/* The shot the Ouroboros options fire (shl05_01) */
#define	pl05_at_ragnarok			0x47	/* Ragnarok (hyper): the rush */
#define	pl05_at_formation_b			0x48	/* Formation B and its charging panther (shl05_02 type 4); sp_formation_b_shot sets it too */
#define	pl05_at_ragnarok_end		0x4a	/* Ragnarok's copies' last, aimed slash (shl05_04) */
#define	pl05_at_vajra				0x4b	/* Vajra: leap, warp above the far side, dive */
#define	pl05_at_formation_c			0x52	/* Formation C, on Strider and the bird's bomb (shl05_03 type 2) */
#define	pl05_at_formation_c_blast	0x55	/* The Formation C bomb's burst on landing */
#define	pl05_at_legion_2			0x56	/* Legion's high animals (shl05_02 type 2) */
#define	pl05_at_snapback			0x58	/* Snapback */
#define	pl05_at_upper			0x59	/* The launcher (pl05_upper) */
#define	pl05_at_vc_murakumo			0x5a	/* Ame no Murakumo as a variable counter */
#define	pl05_at_vc_vajra			0x5b	/* Vajra as a variable counter */
#define	pl05_at_vc_formation_a		0x5c	/* Formation A as a variable counter, on Strider and its animal (shl05_02 type 0) */
#define	pl05_at_formation_a_h		0x5d	/* Formation A, heavy: the robot (shl05_02 type 6) */

/* the character's own work, in tlocal */
typedef struct PL05_LOCAL_W {
	Uint8	option_flag;	/* 00: the Ouroboros options (shl05_00) are out */
	Sint8	panther_flag;	/* 01: the panther (shl05_02 type 4) waits by him */
	Uint8	panther_go_flag;	/* 02: set when the panther is sent off */
	Uint8	bird_flag;		/* 03: the bird (shl05_03) is out */
	Uint8	away_flag;		/* 04: set while an entrance has him off screen */
	Uint8	rush_cnt;		/* 05: hits left of the rush (sp_jexcalibur) */
	Sint8	zanzo_timer;	/* 06: counts down to the next afterimage */
	Uint8	pl05_com_st;	/* 07: the step in the wall dash command */
	Uint8	com_timer;		/* 08: frames left for the wall dash command's next step */
	Uint8	pl05_wall_flag;	/* 09: clinging to a wall */
	Uint8	pl05_wall_flag1;	/* 0a */
	Uint8	wall_cnt;		/* 0b: the wall moves left */
	Sint8	leap_timer;		/* 0c: counts down a leap between walls */
	Uint8	unk_0d[0x10 - 0x0d];
	Float	end_x;			/* 10: where an entrance ends */
	Float	end_y;			/* 14: and its height */
} PL05_LOCAL_W;

#endif
