/* Roll's (character 1d) win poses and special moves. */
#ifndef PL1D_DEF_H
#define PL1D_DEF_H

/* pl_sp_type */
#define	pl1d_sp_buster		0	/* Roll Buster */
#define	pl1d_sp_flower		2	/* Flowers */
#define	pl1d_sp_chouhatsu	3	/* taunt */
#define	pl1d_sp_drill	4	/* Rush Drill (hyper) */
#define	pl1d_sp_beat		5	/* Beat Plane (hyper) */
#define	pl1d_sp_item		6	/* Weapon Activation */
#define	pl1d_sp_hyper_roll		7	/* Hyper Roll (hyper) */
#define	pl1d_sp_snapback	8	/* Snap Back */
#define	pl1d_sp_item_sel	12	/* Summon power-up (Rockball, Leaf Shield, Tornado Hold) */

/* atck_no: the character's own records in the hit table, after the common ATCK_ ones */
#define	pl1d_at_jup_hk			0x12	/* Jumping up + HK */
#define	pl1d_at_upper			0x16	/* The launcher */
#define	pl1d_at_throw			0x20	/* Ground throw */
#define	pl1d_at_air_throw		0x21	/* Air throw */
#define	pl1d_at_buster			0x30	/* Roll Buster standing */
#define	pl1d_at_flower			0x33	/* Flowers thrown standing */
#define	pl1d_at_flower_air		0x34	/* Flowers thrown in the air */
#define	pl1d_at_rockball		0x39	/* Rock Ball (item_ball) */
#define	pl1d_at_item			0x3a	/* Tornado Hold and Leaf Shield put up */
#define	pl1d_at_buster_air		0x3d	/* Roll Buster in the air */
#define	pl1d_at_hyper_roll_1	0x44	/* Hyper Roll: the main fire, even frames */
#define	pl1d_at_drill			0x45	/* Rush Drill (hyper) */
#define	pl1d_at_hyper_roll		0x46	/* Hyper Roll (hyper): the start */
#define	pl1d_at_shield_throw	0x48	/* Leaf Shield thrown, ground or air */
#define	pl1d_at_hyper_roll_2	0x4c	/* Hyper Roll: the main fire, odd frames */
#define	pl1d_at_beat			0x4e	/* Beat Plane (hyper) */
#define	pl1d_at_t_kick			0x4f	/* Tag-in kick that leaps off the wall */
#define	pl1d_at_snapback		0x50	/* Snapback */

/* one step of a colour cycle for ef_color_set(): its time, the colour and the step to the next */
typedef struct COLOR_STEP {
	Uint8	time;
	Uint8	no;
	Uint8	col;
	Uint8	cnt;
	Sint32	next;			/* 1 for the next step; anything else ends the cycle */
} COLOR_STEP;

/* the character's own work, in tlocal */
typedef struct PL1D_LOCAL_W {
	Uint8	item;				/* the item she holds (0 to 2), kept by pl1d_tlocal_init() */
	Uint8	charge_flag;		/* the buster's charge glow (pls1c_02) is on */
	Uint8	charge_lv;			/* the charged shot's power, 2 to 50, up every 120 frames */
	Uint8	item_ef_flag;		/* the item's picture (pls1c_04) is out */
	Uint8	ball_end_flag;	/* tells the Rock Ball (shl1c_02) to break up */
	Uint8	ball_flag;		/* the Rock Ball (shl1c_02) flies */
	Uint8	tornado_flag;		/* the Tornado Hold (shl1c_03) is out */
	Sint8	shield_st;			/* the shield (shl1c_04): 1 out, -1 thrown, 0 gone */
	Uint8	shield_flag;		/* the shield (shl1c_04) circles her */
	Uint8	shield_throw_flag;	/* the throw's animation lets the shield go */
	Uint8	apart_flag;			/* the helpers are apart from her; 0 while they ride on her */
	Uint8	join_sp;			/* the hyper combo the helpers join (2, 4 or 6), 0 when none */
	Uint8	sw_shot;			/* Beat Plane's shot buttons (1 punch, 2 kick); Hyper Roll's presses */
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
	Uint8	shot_cnt;			/* her shots (shl1d_00) out */
	Sint16	sp_time;			/* the giant and flying hyper combos' time left */
} PL1D_LOCAL_W;

#define	PL1D_LW( pl )	( ( PL1D_LOCAL_W * )( pl )->tlocal )

#endif
