/* Spider-Man's special move numbers. */
#ifndef PL0C_DEF_H
#define PL0C_DEF_H

/* pl_sp_type */
#define	pl0c_sp_sting	0	/* Spider Sting */
#define	pl0c_sp_webball	1	/* Web Ball */
#define	pl0c_sp_swing	2	/* Web Swing */
#define	pl0c_sp_webthrow	3	/* Web Throw */
#define	pl0c_sp_maximum	4	/* Maximum Spider (hyper) */
#define	pl0c_sp_crawler	5	/* Crawler Assault (hyper) */
#define	pl0c_sp_hwebthrow	6	/* Ultimate Web Throw (hyper) */
#define	pl0c_sp_chouhatsu	7	/* Taunt */
#define	pl0c_sp_snapback	8	/* Snapback */

/* atck_no: the character's own records in the hit table, after the common ATCK_ ones */
#define	pl0c_at_throw_p		0x20	/* The punch throw, set on the opponent */
#define	pl0c_at_throw_k		0x21	/* The kick throw, set on the opponent */
#define	pl0c_at_air_throw	0x22	/* The air throw, set on the opponent */
#define	pl0c_at_webthrow	0x23	/* Web Throw: the opponent swung over, set on the opponent */
#define	pl0c_at_hwebthrow_2	0x24	/* Ultimate Web Throw (hyper): the throw, set on the opponent */
#define	pl0c_at_t_kick		0x2c	/* The jump-in kick when he is tagged in (t_kick) */
#define	pl0c_at_maximum		0x45	/* Maximum Spider (hyper): the start */
#define	pl0c_at_crawler		0x5e	/* Crawler Assault (hyper): the start, before the animation's hits */
#define	pl0c_at_hwebthrow	0x63	/* Ultimate Web Throw (hyper): the web's grab (sp_hwebthrow and shl0c_02) */
#define	pl0c_at_snapback	0x67	/* Snapback (sp_snapback) */


/* the character's own work, in tlocal */
typedef struct PL0C_LOCAL_W {
	Uint8	shot_flag;				/* 0: a web ball (shl0c_00) is out */
	Uint8	web_flag[3];			/* one for each of the opponent's team (WEB_HOLD, WEB_HIT) */
	Uint8	hit2_flag;				/* 4: set when the uppercut's second hit is due */
	Uint8	swing_st;				/* 5: the web swing's state */
	Uint8	swing_cnt;				/* 6: web swings left in this jump */
	Sint8	line_st;				/* 7: the web throw's line, above 0 when it catches and below when it misses */
	Uint8	unk_8[2];
	Sint16	spin_speed;				/* a: the web throw's spin speed-up, which his mashing raises */
	Sint16	damage;					/* c: the web throw's vital change, more with his mashing, less with the opponent's */
	Sint16	spin_cnt;				/* e: the web throw's spins left, which mashing adds */
	Uint8	unk_10[2];
	Sint16	spin_max;				/* 12: the web throw's spins at most */
	Uint8	unk_14[2];
	Uint8	maximum_dash_cnt;			/* 16: dashes left in Maximum Spider */
	Uint8	maximum_aim_timer;			/* 17: frames to the next turn toward the opponent */
	Uint8	maximum_hit_cnt;			/* 18: hits before the last dash */
	Uint8	maximum_hit_flag;			/* 19: set once a dash has hit */
	Uint8	maximum_aim_flag;			/* 1a: aim the next dash at the opponent */
	Uint8	maximum_edge;				/* 1b: the edges reached: 1 and 2 the walls, 4 the floor, 8 the top */
	Uint8	maximum_wall_flag;			/* 1c: the edge reached is a wall */
	Uint8	maximum_old_edge;			/* 1d: the edges just left, not taken again */
	Uint8	maximum_old_dir;
	Uint8	maximum_disp_dir;			/* 1f: the facing to take back after it */
	struct PLAYER_WORK *maximum_em_w;	/* 20: the opponent it follows */
	Float	hand_x;					/* 24: where the web leaves his hand, x and y */
	Float	hand_y;
	Float	web_dir;				/* 2c: the web's line, as the animation sets it */
	Float	web_line;				/* 30: the line drawn, turning toward web_dir */
	Float	web_turn;				/* 34: its turn each frame */
	Sint8	hweb_st;				/* 38: Ultimate Web Throw's web: above 0 when it catches, below when it misses */
	Uint8	win_cut_flag;			/* 39: set when the web swing win is cut short */
	Sint8	web_swing_st;				/* 3a: the air dash's web (pl0c_web_swing): below 0 once he lets go */
} PL0C_LOCAL_W;

/* web_flag: one for each of the opponent's players */
#define WEB_HOLD	0x02	/* the web ball holds him now; no second web catches him */
#define WEB_HIT		0x80	/* he was hit while webbed: the next web lets go at once */

/* the step the web is at, in the field the effect has no other use for */
#define PLS0C_02_KEY( w )	( *( Sint16 *** )&( w )->em_w )

#endif
