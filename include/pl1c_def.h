/* Mega Man's win poses and special moves. */
#ifndef PL1C_DEF_H
#define PL1C_DEF_H

/* pl_sp_type */
#define	pl1c_sp_buster		0	/* charged Mega Buster */
#define	pl1c_sp_upper		2	/* Mega Uppercut */
#define	pl1c_sp_chouhatsu	3	/* taunt */
#define	pl1c_sp_drill	4	/* Rush Drill (hyper) */
#define	pl1c_sp_beat		5	/* Beat Plane (hyper) */
#define	pl1c_sp_item		6	/* Weapon Activation */
#define	pl1c_sp_hyper_rock		7	/* Hyper Mega Man (hyper) */
#define	pl1c_sp_snapback	8	/* Snap Back */
#define	pl1c_sp_item_sel	12	/* Summon power-up (Rockball, Leaf Shield, Tornado Hold) */
#define	pl1c_sp_13			13

/* atck_no: the character's own records in the hit table, after the common ATCK_ ones */
#define	pl1c_at_upper				0x16	/* The launcher */
#define	pl1c_at_throw				0x20	/* Ground throw */
#define	pl1c_at_air_throw			0x21	/* Air throw */
#define	pl1c_at_buster				0x31	/* Charged Mega Buster, standing or crouching */
#define	pl1c_at_rockball			0x39	/* Rock Ball: the kick and the bouncing ball (shl1c_02) */
#define	pl1c_at_item				0x3a	/* Tornado Hold and Leaf Shield put up, and the tornado (shl1c_03) */
#define	pl1c_at_buster_air			0x3d	/* Charged Mega Buster in the air */
#define	pl1c_at_beat_2				0x3f	/* Beat Plane: the last arm's hit (shl1c_08 kind 4) */
#define	pl1c_at_hyper_rock_missile	0x43	/* Hyper Mega Man: the missiles (shl1c_06 kinds 3 and up) */
#define	pl1c_at_hyper_rock_1		0x44	/* Hyper Mega Man: the main fire, even frames */
#define	pl1c_at_drill				0x45	/* Rush Drill (hyper) */
#define	pl1c_at_hyper_rock			0x46	/* Hyper Mega Man (hyper): the start */
#define	pl1c_at_shield_throw		0x48	/* Leaf Shield thrown, ground or air, and its leaves (shl1c_04) */
#define	pl1c_at_hyper_rock_2		0x4c	/* Hyper Mega Man: the main fire, odd frames */
#define	pl1c_at_beat				0x4e	/* Beat Plane (hyper), Mega Man and the first arm (shl1c_08 kind 0) */
#define	pl1c_at_t_kick				0x4f	/* Tag-in kick that leaps off the wall */
#define	pl1c_at_snapback			0x50	/* Snapback */

/* one step of a colour cycle for ef_color_set(): its time, the colour and the step to the next */
typedef struct COLOR_STEP {
	Uint8	time;
	Uint8	no;
	Uint8	col;
	Uint8	cnt;
	Sint32	next;			/* 1 for the next step; anything else ends the cycle */
} COLOR_STEP;

/* the character's own work, in tlocal */
typedef struct PL1C_LOCAL_W {
	Uint8	item;				/* the item he holds (0 to 2), kept by pl1c_tlocal_init() */
	Uint8	charge_flag;		/* the buster's charge glow (pls1c_02) is on */
	Uint8	charge_lv;			/* the charged shot's power, 2 to 50, up every 120 frames */
	Uint8	item_ef_flag;		/* the item's picture (pls1c_04) is out */
	Uint8	ball_end_flag;	/* tells the Rock Ball (shl1c_02) to break up */
	Uint8	ball_flag;		/* the Rock Ball (shl1c_02) flies */
	Uint8	tornado_flag;		/* the Tornado Hold (shl1c_03) is out */
	Sint8	shield_st;			/* the shield (shl1c_04): 1 out, -1 thrown, 0 gone */
	Uint8	shield_flag;		/* the shield (shl1c_04) circles him */
	Uint8	shield_throw_flag;	/* the throw's animation lets the shield go */
	Uint8	apart_flag;			/* the helpers are apart from him; 0 while they ride on him */
	Uint8	join_sp;			/* the hyper combo the helpers join (2, 4 or 6), 0 when none */
	Uint8	sw_shot;			/* Beat Plane's shot buttons (1 punch, 2 kick); Hyper Mega Man's presses */
	Uint8	shake_flag;			/* the body shakes while set */
	Uint8	col_timer;			/* the colour step's time left */
	Uint8	col_last_flag;		/* set on the cycle's last step */
	COLOR_STEP	*col_step;		/* the next colour step */
	Sint16	*cpu_step;			/* the computer's next step in Rush Drill and Beat Plane */
	Uint8	sp_step;			/* the step in a hyper combo's moves */
	Uint8	turn;				/* set while turning round in Rush Drill and Beat Plane */
	Uint8	item_sel;			/* the item chosen, which its picture (pls1c_04) brings */
	Uint8	cpu_timer;			/* how long the computer keeps its step */
	Uint8	arm_fire;			/* bit 0 or 1 while the arm for that button (shl1c_08) fires */
	Uint8	unk_1d;
	Sint16	sp_time;			/* the giant and flying hyper combos' time left */
} PL1C_LOCAL_W;

#define	PL1C_LW( pl )	( ( PL1C_LOCAL_W * )( pl )->tlocal )

/* a buster shot's start (shl1c_00, shl1c_01): its speed (16.16 fixed point), its place from the
   shooter, its animation and its attack */
typedef struct PL1C_SHOT_DATA {
	Sint32	spd_x;
	Sint32	spd_y;
	Sint16	x;
	Sint16	y;
	Uint8	char_no;
	Uint8	atck_no;
} PL1C_SHOT_DATA;

#endif
