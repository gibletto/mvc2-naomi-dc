/* PLAYER_WORK, the work of a player and of every effect: the records it holds, the struct, and
   the values of its fields. player.h includes it. */
#ifndef PL_WORK_H
#define PL_WORK_H

/* one command being read from the stick and buttons */
typedef struct COM_WORK {
	Uint8	step;	/* 000: the step reached, an index of the reader's jump table; 0 waits for the start */
	Sint8	timer;	/* 001: frames left for the next step; a charge's frames left */
	Uint8	cnt;	/* 002: the steps read (com[cnt + 4] is the next); a turn's steps left; a charge's pl_dir at its start */
	Uint8	rot_dir;	/* 003: com_rot: the stick's way reached, 0 up, 1 forward, 2 down, 3 back */
	Uint8	dir;	/* 004: com_rot: the way it turns, 1 or -1; a dash: 0 forward, 1 back */
	Uint8	ok_flag;	/* 005: the command was done this frame: 1 a dash, 2 a super jump */
	Uint16	sw;	/* 006: the stick or buttons that started or finished it */
} COM_WORK;

/* an animation: its number and group, also compared as one halfword */
typedef union {
	Uint16	w;
	Sint16	s;	/* the same, read signed to compare with an effect's timer0 */
	struct { Uint8 no ; Uint8 grp ; } b;
} CHAR_NO;

/* a script of the computer's commands being read (PLAYER_WORK has four from cpu_norm_sc, one per mode, read as an array) */
typedef struct CPU_SCRIPT {
	Uint8	*adrs;		/* 000: the script */
	Uint16	ofs;		/* 004: the place reached in it */
	Uint8	no;		/* 006: the script, in the character's table */
	Uint8	cmd_cnt;	/* 007: the commands read from it */
} CPU_SCRIPT;

/* the parameter read with a script command: a number, or a distance once scaled */
typedef union {
	Sint32	l;
	Float	f;
} CPU_PRM;

typedef struct PLAYER_WORK {
	Uint8	be_flag;		/* 000: set while the work is in the fight */
	Uint8	pl_type;	/* 001: the character's number, the same as its module's */
	Uint8	pl_no;			/* 002: the player number, 0 or 1 */
	Uint8	work_kind;	/* 003: the kind of work: 0 a player, 1 to 12 the effect kinds */
	Uint8	routine_0;		/* 004: the state machine's top level, above routine_1 */
	Uint8	routine_1;		/* 005 */
	Uint8	routine_2;		/* 006 */
	Uint8	routine_3;		/* 007 */
	struct PLAYER_WORK *before_w;	/* 008: the work before it in its list, or the next free one */
	struct PLAYER_WORK *behind_w;	/* 00c: the work after it in its list */
	void	( *work_func )( struct PLAYER_WORK *w );	/* 010: the routine its list runs each frame */
	struct PLAYER_WORK *ef_w;	/* 014: the effect work this player last made and keeps hold of; an effect's: the effect it rides on */
	struct PLAYER_WORK *master_w;	/* 018: an effect's: the player who made it; in the select, the team member being chosen */
	Sint16	timer0;			/* 01c */
	Sint16	timer1;	/* 01e: a second general timer/counter, used beside timer0 */
	Uint8	type;			/* 020: how the round ended for this player */
	Uint8	sub_type;	/* 021: beside type: an effect's number in its set, a player's variant of a move or win pose */
	Uint8	dir;	/* 022: a direction, mostly the stick's left and right for a throw */
	Uint8	sub_no;
	Sint8	prio;	/* 024: the draw priority */
	Uint8	pl_color;		/* 025: which of the six colours */
	Uint16	ef_no;		/* 026: an effect's: its module, 0xXXYY for shlXX_YY */
	void	( *disp_func )( struct PLAYER_WORK *w );	/* 028: a model's routine, run before it is drawn (obj_func_set) */
	Uint8	unk_2c[0x30 - 0x2c];
	Sint8	col_cnt;	/* 030: the palette lines the character uses */
	Sint8	sub_prio;	/* 031: the order among works of the same prio, lowest drawn first */
	Uint8	unk_32[2];
	VEC3	pos;			/* 034 */
	Sint32	rot[3];	/* 040: the turns about x, y and z, 16-bit angles; rot[2] is in the screen's plane */
	Sint16	rot_ofs_x;	/* 04c: the centre the work turns about, across (rot_ofs_y is the height) */
	Sint16	rot_ofs_y;			/* 04e */
	VEC3	scale;	/* 050: the drawing scale on x, y and z, 1.0 for normal size */
	VEC3	spd;			/* 05c: speed, x and y (z is spare, and holds a saved x) */
	VEC3	acc;			/* 068: the speed's change each frame, copied whole as a point */
	FADE_COL	fade;	/* 074: the model's alpha (trnsl, for nlObjPutTrnsl) and its own fade colour (obj_fade_set); copied whole */
	void	*obj;		/* 084: the model drawn, a Kamui object, 0 for none */
	MATRIX	obj_mtx;			/* 088: the model's matrix, made each frame */
	MATRIX	*base_mtx;		/* 0c8: a matrix the model's is made from, 0 for none */
	/* how a model is drawn (obj_calc, obj_disp): 1 placed, 2/4/8 turned on x/y/z, 0x10 scaled,
	   0x20 see-through, 0x80/0x100 turned to the camera, 0x400/0x800 its own and the stage's fade
	   colour; a player or sprite effect, which has no model, keeps what it likes here */
	Sint32	obj_flag;	/* 0cc: a model's draw flags (OBJ_POS...); other works overlay their own data (EF_WK) */
	Sint32	work1;			/* 0d0 */
	Sint32	work2;			/* 0d4 */
	Sint32	work3;			/* 0d8 */
	Sint32	tile_top;		/* 0dc: its first tile in the frame's tile list */
	Float	disp_x;		/* 0e0: where it is drawn on the screen */
	Float	disp_y;		/* 0e4 */
	Float	disp_z;		/* 0e8: its depth */
	Float	disp_zm_x;	/* 0ec: the sprite's width scale on the screen (NLsprarg zm_x: nlSprPut is passed &tile_top) */
	Float	disp_zm_y;	/* 0f0: the sprite's height scale on the screen (NLsprarg zm_y) */
	Float	disp_u0;	/* 0f4: the part of its texture drawn: left (NLsprarg u0) */
	Float	disp_v0;	/* 0f8: top (NLsprarg v0) */
	Float	disp_u1;	/* 0fc: right (NLsprarg u1) */
	Float	disp_v1;	/* 100: bottom (NLsprarg v1) */
	Sint32	disp_ang;		/* 104: the angle it is drawn at */
	Float	alpha;	/* 108: how solid the work is drawn, 0 to 1 */
	Sint32	disp_list;	/* 10c: the display list it is drawn in (NLsprarg listType): 0 sprites, 2 the credits' letters and pictures */
	Sint32	disp_flip;		/* 110: drawn mirrored */
	Uint8	unk_114[0x12c - 0x114];
	Sint8	disp_flag;	/* 12c: whether the work is drawn: 1 drawn, 0 hidden */
	Sint8	col_flag;	/* 12d: nonzero: the sprite's palette lines are counted from col_no; 0: the lines as the data has them */
	Sint16	col_no;	/* 12e: the first of the player's eight palette lines */
	Sint16	disp_dir;	/* 130: the way the work is drawn facing, normally pl_dir */
	Uint16	char_ofs;	/* 132: where the animation groups' table is in char_adrs */
	Sint16	disp_ofs_x;	/* 134: the centre the sprite turns about, across, from its position (rot_ofs_x or the frame's SPR_PRM) */
	Sint16	disp_ofs_y;	/* 136: the same, up and down */
	struct PLAYER_WORK *unk_138;	/* 138: ef12.c keeps the side's first player here */
	Uint8	size_top;	/* 13c: the body's height above its position (dots, scaled by scale.y) */
	Uint8	size_bottom;		/* 13d: the body's reach below its position, as the screen edge checks read it */
	Uint8	size_front;	/* 13e: the body's width in front of it (dots, scaled by scale.x) */
	Uint8	size_back;	/* 13f: the body's width behind it */
	Sint8	cg_prm;	/* 140: from the animation frame: a number for the program beside cg_type (a speed, an attack, a voice), 0 none */
	Sint8	cg_type;	/* 141: the code the animation's current frame passes to the program */
	Sint8	cg_ctr;	/* 142: frames left on the animation's current frame */
	Sint8	cg_end_flag;	/* 143: from the animation frame: CG_END marks the last frame */
	Sint32	spr_no;		/* 144: the sprite drawn (SPR_NO_MASK), -1 for none; SPR_NO_SET one of the set loaded for it */
	Sint8	cg_prio;	/* 148: from the animation frame: added to the draw priority */
	Uint8	cg_shadow;	/* 149: from the animation frame: the shadow's offset and size, 0xff none */
	Uint8	cg_atck_flag;	/* 14a: from the animation frame: the attack's phase (CG_ATCK_ALL) */
	Uint8	cg_flag1;		/* 14b: set by the animation */
	Uint8	cg_se;		/* 14c: the sound the animation frame asks for: a voice (1 to 63) or a sound effect (0x80 on) */
	Uint8	cg_zoom;	/* 14d: from the animation frame: its SPR_PRM record (sizes, turn, centre) in char_adrs, 0 none */
	Uint8	unk_14e;		/* 14e */
	Uint8	cg_flag2;	/* 14f: from the animation frame: a second code for the character's effects */
	Sint16	cg_rival;	/* 150: from the animation frame: the hold position (low byte), and a code */
	Uint8	unk_152[2];
	Sint8	*cg_adrs;	/* 154: the animation's current frame */
	CHAR_NO	char_no;	/* 158: the animation pl_char_set() last set (.b.no, and .b.grp at 159) */
	Sint8	catch_grip_old;		/* 15a */
	Uint8	unk_15b;
	Uint8	*cell_adrs;		/* 15c: the sprites' cells, packed */
	Uint8	*spr_adrs;		/* 160: the sprites, each a list of cells */
	Uint8	*col_adrs;	/* 164: the colours: six sets of eight lines of 16 words, then the effects' */
	Uint8	*char_adrs;	/* 168: the animation data, which its offsets are counted from */
	FRAME_BOX	*frame_box_tbl;	/* 16c: the table frame_box points into, chosen by each frame */
	HIT_BOX	*hit_box_tbl;		/* 170: the boxes of its animation frames, and from PUSH_BOX the body's */
	HIT_DATA *hit_data_tbl;		/* 174: its attacks' hit data, by atck_no */
	CATCH_POS ( *catch_pos_tbl )[128];	/* 178: where it holds a victim, by the victim's pl_type */
	void	*ef_pos_tbl;	/* 17c: a block of its loaded data */
	Uint8	*spr_set_buff;		/* 180: the buffer for its loaded sprite set */
	Sint32	*pic_adrs;	/* 184: the player's picture file, its parts as offsets from its start */
	struct CPU_DATA *cpu_adrs[3];	/* 188: the computer's data: in mode 0, mode 1, and by event mode */
	struct CPU_DATA *cpu_answer_adrs;	/* 194: its answers to attacks, by HIT_DATA cpu_answer_no */
	Sint32	*cpu_ex_adrs;	/* 198: its command lists for cpu_ex_flag's mode, as offsets */
	Sint8	atck_flag;	/* 19c: how this work attacks, the partner of dm_flag */
	Sint8	dm_flag;	/* 19d: how this work can be hit; 0 not at all */
	Sint8	hit_flag;	/* 19e: the attack's result (HR_HIT on any contact, HR_GUARD...) */
	Sint8	hit_box_no;		/* 19f: which body box was struck, 0 none */
	Sint8	hit_stop;	/* 1a0: hit stop: frames the work stays frozen after a hit or clash */
	Uint8	atck_no;	/* 1a1: the attack, which picks its hit data (ATCK_NO_MASK) */
	Uint8	dm_atck_no;	/* 1a2: the atck_no of the attack that hit it */
	Uint8	atck_level;		/* 1a3 */
	Uint8	work_no;	/* 1a4: the work's place among the six player works */
	Uint8	dm_pl_dir;	/* 1a5: its own pl_dir when it was hit */
	Uint8	dm_at_dir;	/* 1a6: the facing of the player who hit it: which way the knock-back goes */
	Uint8	nml_level;	/* 1a7: a normal attack's strength, 0 to 2, for the gauge it earns */
	Uint16	at_box_no;		/* 1a8: the box that hit */
	Uint16	dm_box_no;		/* 1aa: the box that was hit */
	Sint16	atck_attr;	/* 1ac: the attack's attributes (ATR_), copied to the target on a hit */
	Uint16	dm_attr;	/* 1ae: the atck_attr of the attack that last hit this work */
	struct PLAYER_WORK *em_w;	/* 1b0: the opponent */
	struct PLAYER_WORK *dm_hit_w;	/* 1b4: the work that last hit this one (a player or a shot), or the player throwing it */
	struct PLAYER_WORK *dm_at_w;	/* 1b8: the player behind the attack that last hit it (dm_hit_w is the attacking work itself) */
	HIT_DATA *hit_data_adrs;	/* 1bc: the HIT_DATA of the attack that hit (or that this work's attack gave) */
	FRAME_BOX	*frame_box;	/* 1c0: the current frame's boxes, in frame_box_tbl */
#ifdef UNK_1C4_OLD
	Uint32	unk_1c4;		/* 1c4: unsigned in the older header s_pl06 was built against */
#else
	Sint32	unk_1c4;		/* 1c4: cleared at each attack's start with atck_attr and hit_flag; never read */
#endif
	struct PLAYER_WORK *catch_w;	/* 1c8: the player being thrown */
	union {
		struct PLAYER_WORK *w;	/* 1cc: the other player in a throw (set both ways by pl_catch_set) */
		struct { Uint8 unk_00[2] ; Uint8 unk_1ce ; Uint8 unk_1cf ; } b;	/* 1ce read as a byte by s_pl00 */
	} catch_pair;	/* 1cc: the other side of a hold (the thrower's victim, the victim's thrower), set both ways */
	Uint8	act_st;	/* 1d0: the player's act, the state act_st_set() sets */
	Uint8	old_act_st;	/* 1d1: the act before, kept by act_st_set() */
	Uint8	pl_dir;			/* 1d2: the way the player faces, 1 for right */
	Sint8	jump_dir;	/* 1d3: the jump's direction, picked from the stick at take-off */
	Sint8	jump_com_cnt;	/* 1d4: air specials done in this jump */
	Uint8	jump_dash_cnt;	/* 1d5: air dashes done in this jump */
	Sint8	jump_atk_cnt;	/* 1d6: jump attacks left, punches low and kicks high (JATK_P_MASK, JATK_K_MASK) */
	Uint8	jump_hit_flag;	/* 1d7: a hit in this super jump has already slowed its rise */
	Sint8	sjump_cnt;	/* 1d8: frames into a super jump, up to 7; the stick steers it from then */
	Uint8	air_jump_cnt;	/* 1d9: air jumps (double/triple jumps) made in this jump, low 7 bits */
	Uint8	down_hit_flag;	/* 1da: 0xff once hit while down (not by atck_attr 0x800); the knock-down then can't be hit or thrown */
	Uint8	down_lift_flag;	/* 1db: 0xff when a hit with atck_attr 0x40 struck the player while down; the next hit takes him standing */
	Uint8	ko_flag;	/* 1dc: 0xff once the player is KO'd (the KO flash and voice are done) */
	Sint8	guard_back_flag;	/* 1dd: the advancing guard: set (20) by a guarded hit, -1 once both punches ask for it, 0 when it starts */
	Uint8	guard_timer;	/* 1de: guard stun, from the hit's guard animation; no attacks, and only a variable counter, while set */
	Uint8	stun_ng_flag;	/* 1df: 0xff once dizzied in this combo, so no more stun is taken; cleared by combo_reset */
	Sint8	stun_ng_timer;	/* 1e0: frames after a dizzy (180) in which no stun is taken */
	Uint8	stun_cnt;	/* 1e1: stun left before a dizzy: 80 full, each hit's stun comes off */
	Sint8	stun_reset_timer;	/* 1e2: frames after a hit (60) until stun_cnt is set back to 80 */
	Sint8	renda_cnt;	/* 1e3: the count mashed down to get up sooner from a dizzy, crumple or knock-down */
	Sint16	dizzy_timer;	/* 1e4: frames a dizzy (150) or crumple lasts */
	Uint16	down_timer;	/* 1e6: frames the player stays down after a knock-down (from dm_down_time) */
	Uint8	nml_no;	/* 1e8: which normal attack: 0 light, 1 medium, 2 heavy */
	Uint8	pl_sp_type;		/* 1e9: the special move being done */
	Uint8	catch_flag;	/* 1ea: set by a thrower on every frame it is still holding its victim */
	Uint8	nml_catch_ng_timer;	/* 1eb: frames left during which normal throws cannot catch this player */
	Uint8	hit_ng_timer;	/* 1ec: frames left during which this player's own attacks cannot hit */
	Uint8	catch_ng_timer;	/* 1ed: frames left in which nothing can throw or grab this player */
	Sint8	vcounter_msg_timer;
	Uint8	auto_guard_timer;	/* 1ef: frames (8) after getting up, landing from a guard or being let go: guards by itself, can't attack */
	Uint8	guard_ng_timer;	/* 1f0: frames in which no guard or air dash can start; counted down but nothing sets it */
	Uint8	team_ng_timer;	/* 1f1: frames in which no team move (variable combination, partner call, change) can start */
	Uint8	sp_ng_timer;	/* 1f2: frames left during which no special move or hyper combo can start */
	Uint8	atck_ng_timer;	/* 1f3: frames left during which no attack can start */
	Uint8	wall_ng_timer;	/* 1f4: frames left in which the screen edges do not hold the player */
	Uint8	push_ng_timer;	/* 1f5: frames left in which the players do not push each other apart */
	Uint8	catch_type;		/* 1f6: the kind of throw being taken, which the caught routines dispatch on */
	Uint8	catch_no;		/* 1f7: the throw (CATCH_NO_MASK), and when it can't be broken */
	Sint8	win_flag;	/* 1f8: set once the win pose has started; 0x80 the start button cut it short */
	Uint8	pl_stance;		/* 1f9: STANCE_STAND and the rest */
	Uint16	atck_lever;	/* 1fa: the stick held when the attack or command was given */
	Uint8	sjump_flag;	/* 1fc: a super jump: 1 rising, 2 past its peak */
	Sint8	wall_flag;	/* 1fd: the screen edge the player is against (WALL_RIGHT, WALL_LEFT) */
	Uint8	atck_pk;	/* 1fe: punch or kick for the attack under way: 0 punch, 1 kick */
	Uint8	atck_type;	/* 1ff: the attack's kind: 0 ground, 1 jump, 2 super jump, 3 from a dash */
	Uint8	speed_up_flag;	/* 200: a timed speed-up is on */
	Uint8	fly_flag;	/* 201: set while flying */
	Uint8	armor_flag;	/* 202: armour until cleared (ARMOR_ON, ARMOR_HEAVY; the timed armour is at 3e4) */
	Uint8	float_flag;	/* 203: the character stands hovering (Storm, Magneto): it stands at float_y and bobs */
	Uint8	unk_204;
	Uint8	atck_pow;	/* 205: the attacker's damage rate for the current move, 32 = normal */
	Uint8	def_pow;	/* 206: the defender's damage-taken rate, 32 = normal */
	Uint8	weight;		/* 207: how heavy the character is: 5 or more gives the heavier landing, fall and throw sounds */
	Float	float_y;	/* 208: the height a hovering stand is based at */
	struct PLAYER_WORK *target_w;	/* 20c: the opposing team's point character */
	Uint8	air_guard_cnt;	/* 210: guards in this jump; no jump attack after one until landing */
	Uint8	guard_cnt;	/* 211: hits guarded in a row; cleared by a hit and when guard stun ends */
	Uint8	unk_212;		/* 212 */
	Uint8	unk_213;
	void	*step_adrs;		/* 214: steps read one a frame: the knock-back's (Sint8) or the guard's command data */
	Float	back_spd_x;		/* 218: the flying knock-back's x speed, stored before the reaction starts (and a throw's) */
	Float	back_spd_y;		/* 21c: and its y speed */
	Float	blow_spd;		/* 220: the knock-back's speed */
	Float	blow_acc;		/* 224: and its change */
	Uint8	unk_228;		/* 228 */
	Sint8	elem_ef_cnt;	/* 229: element effects out on the player (dm00_0x, pls28_00), at most 4 */
	Uint8	dm_elem;	/* 22a: the element of the last hit this player took, 1 to 5 */
	Uint8	dm_elem_old;	/* 22b: dm_elem before the last hit or throw cleared it */
	Sint16	dm_down_time;	/* 22c: the hit's down time (HIT_DATA sleep_time), copied to down_timer when knocked down */
	Uint8	dm_dir;		/* 22e: the knock-back angle, 32 to a circle */
	Uint8	dm_lv;		/* 22f: the knock-back strength */
	Uint8	dm_guard_type;	/* 230: the hit's atck_kind (A_GUARD_MASK...) */
	Uint8	dm_kind;	/* 231: the damage_kind of the hit that struck it */
	Uint8	dm_recover_time;	/* 232: the hit's frames before an air recovery (HIT_DATA sandbag_time), copied to recover_timer */
	Uint8	dm_no;		/* 233: the damage reaction, the dm_xx routine that set it */
	Uint8	recover_ng_flag;	/* 234: the knock-back can't be recovered from in the air (launcher, air combo end) */
	Uint8	fly_screen_flag;	/* 235: an air combo's finisher sends the player flying; the screen follows and no moves start on him */
	Sint8	ukemi_flag;	/* 236: a thrown player may still recover; -1 once too late */
	Uint8	catch_atck_ng_timer;	/* 237: frames (5) after a dash in which the player cannot throw */
	Uint8	wall_hit_flag;		/* 238 */
	Sint8	recover_timer;	/* 239: frames left before an air recovery is possible in a knock-back */
	Sint8	catch_cnt;		/* 23a */
	Uint8	catch_dm_cnt;		/* 23b: goes up by 2 for each throw taken, to 0xff at most */
	Uint8	fly_lever;		/* 23c: the stick's direction bits while flying */
	Sint8	float_wait;		/* 23d: frames to the next step of the standing bob */
	Sint8	float_st;		/* 23e: the standing bob's step, 0 to 39 */
	Uint8	unk_23f;		/* 23f: counts down */
	Uint8	unk_240[0x248 - 0x240];
	Uint8	hyper_stop_flag;	/* 248: the fight is stopped for a hyper combo's flash: a bit for each side */
	Sint8	hyper_stop_timer;	/* 249: frames into the hyper stop */
	Sint8	assist_timer[2];		/* 24a: frames left to let go of each assist button */
	VEC2	guard_pos;	/* 24c: where the last guard's mark was, for the advancing guard's wave */
	Uint8	change_req;	/* 254: the team change asked for this frame (1 to 9), run by team_change_ck */
	Uint8	team_type;	/* 255: the team move that started this act: 3 variable counter, 4 team hyper */
	Uint8	change_type;	/* 256: how the partner comes in, which act_28 step it runs */
	Uint8	partner_no;		/* 257: the partner called, 1 or 2 */
	Uint8	delay_sp_type;	/* 258: the hyper combo a delay_ck chose, done once the partner is in */
	Sint8	v_combi_cnt;		/* 259: the partners in a variable combination */
	Sint8	change_timer[2];		/* 25a: frames left to let go of each tag button pair */
	Sint16	renda_timer;	/* 25c: the mash count to break free (held) or to hold on (thrower) */
	Sint16	renda_max;	/* 25e: the thrower's mash count as set, which renda_timer starts over from */
	Sint16	combo_cnt;		/* 260: the hits of the player's combo */
	Uint8	unk_262[2];
	Sint16	combo_kind;	/* 264: the kinds of hit in the combo (hit data unk_02 & 0x30); 1 once it is a long air combo */
	Uint16	combo_kind_old;	/* 266: combo_kind before the last hit that added a kind */
	Sint16	combo_vital;	/* 268: vital_new when the combo on the player began, kept for the whole combo */
	Sint16	combo_cnt_max;	/* 26a: the player's longest combo (most combo_cnt) this round */
	Sint16	combo_dm_cnt;	/* 26c: the hits of the combo on the player */
	Sint16	combo_dm_cnt_old;	/* 26e: combo_dm_cnt before the last hit */
	Sint16	dm_hit_cnt;		/* 270: the hits taken in the combo, cleared by the opponent */
	Uint8	air_combo_cnt;	/* 272: the super jump juggle hits in the combo; cleared when it ends */
	Uint8	combo_hit_type;	/* 273: the last hit: 1 from the air, 2 two air hits in a row, 4 on a player in the air, 8 no kind */
	Uint8	combo_hit_type_old;	/* 274: combo_hit_type before it */
	Uint8	combo_reset_flag;	/* 275: set while combo_add's hits go on */
	Uint8	combo_hit_types;	/* 276: combo_hit_type of every hit in the combo, or'd */
	Uint8	assist_dm_cnt;	/* 277: hits taken in the combo from the other side's partners; while any has some, no partner can be called */
	Sint16	escape_timer;		/* 278: a throw escape's timer */
	Sint8	cancel_timer;	/* 27a: frames left to cancel a special move into a hyper combo */
	Uint8	unk_27b;		/* 27b */
	Sint16	combo_damage;	/* 27c: vital_new when the combo on the player began */
	Sint8	armor_time;		/* 27e: from the armour table's first entry: the frames before the armour count resets */
	Uint8	armor_max;		/* 27f: the armour's hit limit */
	Uint8	armor_cnt;		/* 280: the hits the armour has taken */
	Uint8	armor_hit_flag;	/* 281: a hit landed on the armour during this hit stop */
	Uint8	armor_back_timer;	/* 282: frames left of the knock-back taken on armour, 5 at the hit */
	Sint8	armor_wait;		/* 283: loaded from armor_time; armor_hit_0 counts it down and clears armor_cnt at 0 */
	VEC3	pl_scale;	/* 284: the player's own scale, which scale is reset to */
	Float	armor_back_spd;	/* 290: the knock-back's x speed taken on armour */
	Float	armor_back_acc;	/* 294: and its change */
	Float	em_dist;		/* 298: the distance to the opponent's nearest box (pl_em_dist_set) */
	Sint8	delay_change_flag;	/* 29c: set when a delayed hyper combo calls partner_no in; pl_team runs change_delay and clears it */
	Uint8	walk_ng_flag;		/* 29d: while set, the player goes back to standing */
	Uint8	jump_move_flag;	/* 29e: set once pl_jump_move has moved the player this frame; cleared each frame */
	Uint8	snap_partner;	/* 29f: the partner a snapback brings in (1 or 2), set while the player is knocked off the screen by it */
	Sint16	vital_red_wait;		/* 2a0: counted down by vital_red_timer; red vital comes back only at 0 */
	Uint8	unk_2a2;		/* 2a2 */
	Uint8	unk_2a3;
	Sint32	tlocal[0x80 / 4];	/* 2a4: the character's own work */
	Uint8	wall_wide_timer;	/* 324: frames left in which pl_wall_wide_ck keeps the player on the screen with more room (a teleport) */
	Uint8	prio_ofs;	/* 325: taken from the player's drawing priority: the character's table entry * 2 */
	Uint8	win_wait;	/* 326: frames before START can end the win pose; 0xff holds it while a pose plays out */
	Sint8	hyper_bg;	/* 327: the hyper combo's screen effect */
	Sint8	hyper_bg_tm;	/* 328: frames left of hyper_bg */
	Uint8	hyper_team_bit;	/* 329: the work_no / 2 bit of each player in a team hyper combo */
	Uint8	unk_32a[2];
	Sint16	vital_red_dm;	/* 32c: the red part taken off by the last hit (3/4 of its damage, at least 1) */
	Sint16	vital_dm;	/* 32e: the vital the last hit took, which the bar shows falling */
	Uint8	unk_330[0x340 - 0x330];
	Uint16	pl_new_lvbt;	/* 340: stick and buttons held this frame, as read */
	Uint16	pl_last_lvbt;	/* 342: stick and buttons held the frame before, as read (pl_old_lvbt holds the frame before that) */
	Uint16	pl_old_lvbt;		/* 344: buttons newly pressed the frame before */
	Uint16	pl_old2_lvbt;	/* 346: and the frame before pl_old_lvbt */
	Uint16	pl_now_lvbt;	/* 348: stick and buttons newly pressed this frame, as read */
	Uint16	pl_sw_new;	/* 34a: stick and buttons held this frame, back and forward by the facing */
	Uint16	pl_sw_old;	/* 34c: stick and buttons held the frame before, turned by the facing (as pl_sw_new) */
	Uint16	pl_sw_now;		/* 34e: the stick and buttons this frame */
	Uint16	pl_stop_new;	/* 350: buttons held during the hit stop, kept for after it (as pl_stop_now) */
	Uint16	pl_stop_now;	/* 352: buttons pressed during the hit stop, kept for after it */
	COM_WORK	dash_w;	/* 354: the dash command */
	COM_WORK	sjump_w;	/* 35c: the super jump command */
	COM_WORK	com_w;	/* 364: the first of the 13 command works com_ck reads moves into */
	COM_WORK	com_w0;		/* 36c: one work for each command the character reads */
	COM_WORK	com_w1;		/* 374 */
	COM_WORK	com_w2;		/* 37c */
	COM_WORK	com_w3;		/* 384 */
	COM_WORK	com_w4;		/* 38c */
	COM_WORK	com_w5;		/* 394 */
	COM_WORK	com_w6;		/* 39c */
	COM_WORK	com_w7;		/* 3a4 */
	COM_WORK	com_w8;		/* 3ac */
	COM_WORK	com_w9;		/* 3b4 */
	COM_WORK	com_w10;	/* 3bc */
	COM_WORK	com_w11;	/* 3c4 */
	COM_WORK	com_w12;	/* 3cc */
	COM_WORK	snapback_w;	/* 3d4: the snapback command */
	COM_WORK	vcounter_w;	/* 3dc: the variable counter command */
	Sint16	armor_timer;	/* 3e4: frames of timed super armour left */
	Sint16	atck_plus_timer;	/* 3e6: frames left of the timed attack power-up */
	Sint16	def_plus_timer;	/* 3e8: frames left of the timed defence power-up */
	Sint16	speed_up_timer;	/* 3ea: frames left of a hyper combo's speed-up */
	Sint16	speed_plus_timer;	/* 3ec: frames left of the timed speed-up (speed_up_set), which keeps speed_up_flag on */
	Uint8	atck_plus;	/* 3ee: the timed attack power-up's damage rate, 32 = normal (40 = +25%) */
	Uint8	def_plus;	/* 3ef: the timed defence power-up's damage-taken rate, 32 = normal */
	Uint8	stop_move_flag;	/* 3f0: lets the player move during its own side's hyper flash */
	Sint8	stop_move_timer;	/* 3f1: frames left for stop_move_flag (3f0); clears it when it runs out */
	Uint8	armor_timer_flag;	/* 3f2: set while the timed armour (armor_timer) is on */
	Uint8	unk_3f3;
	Sint8	*chain_tbl;	/* 3f4: the normal attack's chain table, by button */
	Sint8	delay_timer;	/* 3f8: frames left in the delayed-hyper-combo window */
	Uint8	delay_flag;	/* 3f9: a partner may come in with a delayed hyper combo */
	Uint8	unk_3fa[0x408 - 0x3fa];
	Sint16	*gauge;		/* 408: the side's hyper combo gauge within its level, 0 to 144 */
	Sint8	*gauge_level;	/* 40c: the side's hyper combo gauge level, 0 to 5 */
	Sint8	gauge_no;		/* 410: the gauge a hit gives when its hit data gives none (set by pl_act) */
	Uint8	team_no;	/* 411: the player's place in the team: 0 on point, then 1 and 2 */
	Uint8	team_no_org;	/* 412: the place in the team the player started in */
	Uint8	unk_413;
	Uint32	pl_bit[2];	/* 414: the character's own bit (1 << its number) of 64, tested with FLAG_CK */
	Float	ground_y;		/* 41c */
	Sint16	vital_new;		/* 420 */
	Uint8	unk_422[2];
	Sint16	vital_red;		/* 424: the red part of the vital bar that comes back */
	Sint16	vital_red_cnt;		/* 426: vital_red_recover counts it down from 50, a point of vital each time */
	void	(**pl_tbl)( struct PLAYER_WORK *pl_w );	/* 428: the character's routines */
	Uint8	unk_42c[0x43c - 0x42c];
	Uint8	cpu_routine_1;	/* 43c: the computer's first state level: its mode */
	Uint8	cpu_routine_2;	/* 43d: the computer's second state level: the step within its mode */
	Uint8	cpu_routine_3;	/* 43e: the computer's third state level: the step in the command */
	Uint8	cpu_routine_4;	/* 43f: the computer's fourth state level */
	Uint8	cpu_cmd;	/* 440: the command the computer is running, an index into cmd_jmp */
	Uint8	cpu_cmd_old;	/* 441: the computer's command before cpu_cmd */
	Sint16	cpu_sp_timer;	/* 442: frames until the special move a script set up (sp_cmd_set) is done */
	Uint16	cpu_sp_ofs;	/* 444: where that special move's numbers are in the script */
	Uint8	cpu_crouch_flag;	/* 446: the computer holds KEY_DOWN while set (the crouch and stand commands) */
	Uint8	cpu_crouch_old;	/* 447: cpu_crouch_flag the frame before */
	Sint8	cpu_sp_no;		/* 448: the computer's chosen command number */
	Sint8	cpu_wait_rnd;	/* 449: the most extra random frames added to a rest command's wait */
	Sint8	cpu_loop_cnt;	/* 44a: passes of the computer's mode loop this frame */
	Sint8	cpu_loop_max;	/* 44b: the most passes in one frame; the loop stops past 31 */
	CPU_PRM	cpu_work0;		/* 44c: how long a walk can last; for a script's walk, how far */
	CPU_PRM	cpu_work1;		/* 450: the script command's work: where the walk began, or the act */
	CPU_PRM	cpu_work2;		/* 454: the act, or the stick */
	CPU_PRM	cpu_work3;		/* 458: the stick, or the side to walk from */
	Uint8	cpu_lv;		/* 45c: the computer's level, 0 to 3 */
	Uint8	cpu_sp_flag;		/* 45d: set while it does a special move */
	Uint8	cpu_cmd_timer;	/* 45e: frames left in the computer's current command */
	Uint8	cpu_vcounter_cnt;
	CPU_SCRIPT	cpu_norm_sc;	/* 460: the script read in mode 0 (cpu_norm) */
	CPU_SCRIPT	cpu_range_sc;	/* 468: in mode 1 (cpu_range) */
	CPU_SCRIPT	cpu_event_sc;	/* 470: in mode 2, by event (cpu_event) */
	CPU_SCRIPT	cpu_ex_sc;	/* 478: the one cpu_ex_flag runs, from cpu_ex_adrs */
	Uint8	cpu_ex_flag;	/* 480: while set, the computer runs its fourth script instead of its mode */
	Uint8	unk_481;		/* 481 */
	Uint8	cpu_lock_flag;	/* 482: while set, the computer keeps its current mode and command list */
	Uint8	old_routine_1;	/* 483: routine_1 the frame before (the computer's event checks compare it) */
	Float	em_dist_x;		/* 484: em_len_x less the nearest opponent's width */
	Float	em_len_x;	/* 488: how far away the nearest opponent is in x, centre to centre; em_dist_x is this less its width */
	Float	em_dist_y;		/* 48c: and in y */
	Float	atck_len;		/* 490: how far away the nearest attack is */
	Uint8	cpu_ev_lv;	/* 494: the level of the computer's event mode; event checks of a lower level don't interrupt it */
	Sint8	cpu_cancel_flag;	/* 495: a cancel the script waits on (CC_) */
	Sint8	cpu_jump_flag;	/* 496: 1 a jump, -128 a super jump: once it is over, the script skips to its endif */
	Uint8	cpu_fly_flag;	/* 497: while set, the script skips to its hiendif once fly_flag clears */
	int	( *cpu_renda )( struct PLAYER_WORK *pl_w );	/* 498: run each frame of a special move command */
	Uint8	unk_49c[0x4a0 - 0x49c];
	Uint8	em_atck_no;	/* 4a0: the attack the computer answers (atck_no & ATCK_NO_MASK), EM_ATCK_NONE none */
	Uint8	unk_4a1;
	Sint8	cpu_walk_timer;	/* 4a2: frames left for a script's walk or dash (180 at its start) */
	Sint8	cpu_walk_act;	/* 4a3: the act a script's walk keeps; the walk ends if it changes */
	Uint8	cpu_guard_timer;	/* 4a4: while set the computer guards without its chance check (script 40 sets it) */
	Uint8	cpu_guard_ng_timer;		/* 4a5: counts down */
	Sint8	cpu_air_jump_flag;	/* 4a6: 2 lets the computer make one air jump */
	Sint8	cpu_guard_back_cnt;		/* 4a7 */
	Sint8	unk_4a8;
	Sint8	fight_sec;		/* 4a9: the seconds the point player has fought */
	Uint8	cpu_atck_level;		/* 4aa: the computer's atck_level for its command */
	Uint8	cpu_atck_pk;		/* 4ab: the computer's atck_pk */
	Uint16	cpu_atck_lever;		/* 4ac: the computer's atck_lever */
	Sint16	cpu_hold_sw;		/* 4ae: the stick and buttons the computer holds for a hold command */
	Uint32	ev_try_flag;		/* 4b0: what the computer has done this round */
	CPU_PRM	cpu_prm;	/* 4b4: the parameter of the script command being read */
	struct PLAYER_WORK *em_atck_w;	/* 4b8: the work whose attack the computer answers */
	Sint8	entry_routine_1;	/* 4bc: the side's entry state: 0 waiting, 1 joining, 2 continue, 3 name entry, 4 game over */
	Sint8	entry_routine_2;	/* 4bd: the step within it */
	Sint8	name_cursor;	/* 4be: the name entry's cursor, 0 to 3; 4 decided, 5 done */
	Sint8	blink_flag;	/* 4bf: the cursor's blink: set while it shows the letter */
	Sint16	entry_timer;	/* 4c0: the entry's timer: the message flash, the continue count, the name entry's time, the game over's */
	Sint16	entry_timer2;	/* 4c2: a second timer: the message phase, the continue's frames per count, the name entry's idle time */
	Sint16	name_letter;	/* 4c4: the letter under the name entry's cursor */
	Sint16	blink_timer;	/* 4c6: frames until the cursor's blink changes (40 shown, 20 hidden) */
	Sint8	side_bit;	/* 4c8: the side's bit: 1 or 2 */
	Sint8	pl_personal;	/* 4c9 */
	Uint8	sel_speed;		/* 4ca */
	Uint8	unk_4cb;
	Uint32	cont_credit;	/* 4cc: the credits counted when the continue started; a coin restarts the count */
	Uint32	cont_remain;	/* 4d0: and the coins toward the next credit */
	Sint32	score[2];	/* 4d4: the side's score in BCD, the low eight digits, then the high three */
	SW_WORK	pad_sw;	/* 4dc: the side's buttons as the player reads them; the computer writes its own */
	SW_WORK	side_sw;	/* 4f0: the side's own buttons, a copy of sw_work[side] each frame */
	Uint32	beat_flag[2];	/* 504: a bit for each character the side has beaten in the arcade, so the computer picks new ones */
	Sint8	name_flag;	/* 50c: a bit for each letter of the name entered */
	Uint8	unk_50d;
	Sint16	cpu_level_pts;		/* 50e: the side's points toward the computer's level (cpu_level.c) */
	Sint8	route_no;	/* 510: the arcade route: the row of stage_tbl, picked at random */
	Uint8	unk_511[0x524 - 0x511];
	Sint8	pl_index;	/* 524: the work's place in pl_work, set as a game starts; for the first two, the side */
	Sint8	cpu_flag;	/* 525: set (1) while the computer controls this player */
	Uint8	unk_526;
	Sint8	win_max;		/* 527: the most wins in a row against another player */
	Sint32	*pl_data_adrs;	/* 528: where the player's data file was loaded, a table of offsets at its start */
	Sint8	load_type;	/* 52c: the character whose files the player loads: the one chosen, or a form it turns into */
	Sint8	load_color;	/* 52d: the colour chosen, whose palette is loaded; copied to pl_color */
	Uint8	stage_cnt;	/* 52e: the side's arcade stage reached, kept in Sys->stage_cnt while it plays */
	Sint8	win_cnt;		/* 52f: the wins in a row against another player, up to 99 */
	Sint8	name[4];	/* 530: the name entered for the ranking, as letter numbers */
	Sint32	old_score_hi;	/* 534: the score's high digits kept between fights (score[1]) */
	Sint32	old_score;	/* 538: and its low eight digits (score[0]), whose last digit counts the continues */
	Uint8	best_rank;	/* 53c: the better of score_rank and win_rank, 20 for none */
	Uint8	score_rank;	/* 53d: the side's place in rank_score, 20 for none */
	Uint8	win_rank;	/* 53e: the side's place in rank_win, 20 for none */
	Uint8	team_order;	/* 53f: the place in the team chosen, copied to team_no for each fight */
	Uint8	unk_540[3];	/* 540 */
	Uint8	unk_543;
	Uint32	card_open[2];	/* 544: the characters opened by the side's memory card, a bit for each */
	Uint32	card_ex[2];	/* 54c */
	Uint8	unk_554[0x558 - 0x554];
	Uint32	play_time;	/* 558: frames the side has played in the arcade */
	Uint8	unk_55c[0x5a4 - 0x55c];
} PLAYER_WORK;

/* pl_tbl: a character's routines */
#define TBL_MAIN	0
#define TBL_COM_CK	1
#define TBL_ATCK_SET	2
#define TBL_ATCK	3
#define TBL_DASH	4
#define TBL_NOKI	5
#define TBL_APPEAR	6
#define TBL_WIN		7
#define TBL_SP_ATCK	8
#define TBL_CATCH_CK	9
#define TBL_CATCH_SET	10
#define TBL_CATCH	11
#define TBL_HOLD	12
#define TBL_DELAY_CK	13
#define TBL_T_KICK	14
#define TBL_UPPER	15
#define TBL_SP_UNISON	16
#define TBL_SP_GATTAI	17
#define TBL_SP_ASSIST	18
#define TBL_SP_ZERO	19
#define TBL_AIR_DASH	21
#define TBL_TJUMP	22
#define TBL_TLOCAL_INIT	23
#define TBL_EX0		24	/* 24 to 26: the character's own, a different thing in each */
#define TBL_EX1		25
#define TBL_EX2		26

/* entry_routine_1: a side's entry */
#define ENTRY_WAIT	0
#define ENTRY_PLAY	1
#define ENTRY_CONT	2	/* continue? */
#define ENTRY_NAME	3	/* name entry */
#define ENTRY_OVER	4	/* game over */

/* atck_attr and dm_attr: the attack's attributes, copied to the target on a hit */
#define ATR_ELEM_MASK	0x000f	/* the attack's element (ATR_ICE, ATR_FIRE, ATR_THUNDER); 0 takes the hit data's */
#define ATR_ICE		0x0001	/* ice */
#define ATR_FIRE	0x0002	/* fire */
#define ATR_THUNDER	0x0003	/* lightning */
#define ATR_FREEZE	0x0008	/* freezes: the reaction becomes dm_no 9, stuck until mashed out, with the ice element */
#define ATR_BACK_POS	0x0010	/* knocked away from where the attacking player stands, not by its facing */
#define ATR_BACK_TURN	0x0020	/* knocked back behind the struck player, by his own facing */
#define ATR_DOWN_LIFT	0x0040	/* a hit on a player lying down lifts him, so the next hit takes him standing */
#define ATR_COUNTER	0x0080	/* dm_attr only: a counter stance is waiting; push_ck clears it when an attack touches it */
#define ATR_CHIP	0x0100	/* chip damage on a guard even if the hit data has none; it can't take the last point */
#define ATR_SHOT_HOLD	0x0200	/* a shot that holds its target: its hits don't strike a player a shot already holds */
#define ATR_KNOCK_AWAY	0x0400	/* the hit becomes dm_no 10 (knocked away, kept on the screen) */
#define ATR_DOWN_HIT_NG	0x0800	/* a hit on a player who is down doesn't set down_hit_flag, so he can be hit again */
#define ATR_BACK_WORK	0x1000	/* knocked away from where the striking work (dm_hit_w) stands; also on a guard */

/* hit_flag: the attack's result */
#define HR_GUARD	0x01	/* the attack was guarded */
#define HR_KIND_MASK	0x06	/* the kind of work it struck (its dm_flag & 6); 0 a player */
#define HR_ONCE		0x08	/* a hit's one-time follow-up has been done */
#define HR_ARMOR	0x10	/* it struck armour or a shield */
#define HR_HIT		0x80	/* it touched a work (on every contact, a guard too) */

/* atck_flag and dm_flag: how a work attacks and how it is struck; 0 not at all */
#define HIT_OWNER	0x01	/* the hit is its player's too (pl_work[work_no]): he gets the result and the hit stop */
#define HIT_KIND_MASK	0x06	/* the work's kind, which hit_ng_tbl pairs; 0 a player's own body */
#define HIT_SHOT	0x02	/* a shot: a player's attack passes through it, shots meet it */
#define HIT_OBJ		0x04	/* a solid thing: a player's attack strikes it too */
#define HIT_PL_ONLY	0x06	/* meets players only (grabs) */
#define HIT_TOUCH	0x20	/* atck_flag: a touch: pl_hit_ck marks hit_box_no 0x40 and does no damage */
#define HIT_ON		0x40	/* set in an effect's flags; nothing reads it */
#define HIT_PL		0x80	/* a player's: pl_hit_ck does its hit, not hit_data_set */

/* atck_no, dm_atck_no and em_atck_no */
#define ATCK_NO_MASK	0x7f	/* the attack's number, which indexes hit_data_tbl */
#define ATCK_DIR	0x80	/* the target is knocked along the attacker's dir, not the hit data's vector */

/* the hit table's first records, the same for every character (M is a chain's second press); each
   character's own records follow, from 0x12, as plXX_at_ names in plXX_def.h */
#define ATCK_STAND_LP	0x00
#define ATCK_STAND_MP	0x01
#define ATCK_STAND_HP	0x02
#define ATCK_STAND_LK	0x03
#define ATCK_STAND_MK	0x04
#define ATCK_STAND_HK	0x05
#define ATCK_CROUCH_LP	0x06
#define ATCK_CROUCH_MP	0x07
#define ATCK_CROUCH_HP	0x08
#define ATCK_CROUCH_LK	0x09
#define ATCK_CROUCH_MK	0x0a
#define ATCK_CROUCH_HK	0x0b
#define ATCK_JUMP_LP	0x0c
#define ATCK_JUMP_MP	0x0d
#define ATCK_JUMP_HP	0x0e
#define ATCK_JUMP_LK	0x0f
#define ATCK_JUMP_MK	0x10
#define ATCK_JUMP_HK	0x11
#define EM_ATCK_NONE	0xff	/* no attack to answer */

/* cpu_cancel_flag: the computer script's cancel */
#define CC_ON		0x01	/* a cancel is set */
#define CC_GUARD	0x02	/* the attack was guarded */
#define CC_SKIP		0x10	/* if the cancel fails on a guard, skip to the endif */
#define CC_GO		0x80	/* the next command may start */

/* armor_flag */
#define ARMOR_ON	0x01	/* plain armour, until cleared */
#define ARMOR_HEAVY	0x80	/* the heavy armoured form (Mecha Zangief, giant SonSon, King Servbot, Rogue's Sentinel power) */

/* catch_no: the throw's number, and when it can't be broken */
#define CATCH_NO_MASK	0x3f	/* the throw's number */
#define CATCH_ESC_NG	0x40	/* can't be broken at the start */
#define CATCH_ESC2_NG	0x80	/* can't be broken later */

/* from the animation frame: cg_atck_flag and cg_end_flag */
#define CG_ATCK_NG	0x20	/* no chain, special or hyper cancel on this frame */
#define CG_ATCK_ON	0x80	/* the attack's active frames (its hit boxes are out) */
#define CG_ATCK_ALL	0xe0	/* mask: any part of an attack under way */
#define CG_END		0x80	/* the animation's last frame: it goes on from CHAR_DATA's loop_ofs */

/* spr_no */
#define SPR_NO_MASK	0x7fff	/* mask: the sprite's number in spr_adrs */
#define SPR_NO_SET	0x8000	/* drawn from the work's loaded sprite set */

/* jump_atk_cnt: the jump attacks left, punches in the low nibble and kicks in the high */
#define JATK_P_MASK	0x0f	/* punch attacks left in this jump */
#define JATK_K_MASK	0xf0	/* kick attacks left */
#define JATK_ONE	0x11	/* one of each: a jump's allowance */

/* wall_flag (and vs_sys_work's): the screen edge it is held at */
#define WALL_RIGHT	0x01	/* at the right edge */
#define WALL_LEFT	0x02	/* at the left edge */

/* obj_flag, for a model work: how disp.c places and draws it (other works keep their own data there) */
#define OBJ_POS		0x0001	/* the model is placed at pos */
#define OBJ_ROT_X	0x0002	/* turned on x by rot[0] */
#define OBJ_ROT_Y	0x0004	/* turned on y by rot[1] */
#define OBJ_ROT_Z	0x0008	/* turned on z by rot[2] */
#define OBJ_SCALE	0x0010	/* scaled by scale */
#define OBJ_TRNSL	0x0020	/* drawn see-through at fade.trnsl */
#define OBJ_FACE_X	0x0080	/* turned on x to face the camera */
#define OBJ_FACE_Y	0x0100	/* turned on y to face the camera */
#define OBJ_FADE	0x0400	/* its own fade colour */
#define OBJ_BRIGHT	0x0800	/* the stage's brightness, times its own fade with OBJ_FADE */
#define OBJ_SHADOW	0x2000	/* drawn as a modifier volume (a shadow), not as a model */
#define OBJ_MTX		0x4000	/* its own obj_mtx on top of base_mtx */
#define OBJ_ENV		0x8000	/* drawn with a cheap environment map first */

/* the drawing and animation part of a work, tile_top to cpu_ex_adrs, which an effect copies whole
   from its master */
typedef struct CHAR_PART {
	Sint32	l[( 0x19c - 0xdc ) / 4];
} CHAR_PART;
#define CHAR_PART_COPY( d, s )	( *( CHAR_PART * )&( d )->tile_top = *( CHAR_PART * )&( s )->tile_top )

/* the animation's state, cg_prm to cg_rival (before cg_adrs), copied as one record */
typedef struct CHAR_ANIM {
	Sint32	l[5];
} CHAR_ANIM;
#define CHAR_ANIM_COPY( d, s )	( *( CHAR_ANIM * )&( d )->cg_prm = *( CHAR_ANIM * )&( s )->cg_prm )

/* the body's four sizes, size_top to size_back, set as one word (0x20202424: 0x24 0x24 0x20 0x20) */
#define BODY_SIZE_SET( w, l )	( *( Uint32 * )&( w )->size_top = ( l ) )

#endif
