/* The records of a character's data: hit boxes, hit data, hold positions, animation frames and
   the computer's data; and the program's tables by character. player.h includes it. */
#ifndef PL_DATA_H
#define PL_DATA_H

/* a box of an animation frame, in a work's hit_box_tbl table: its centre's offset and half its size */
typedef struct HIT_BOX {
	Sint16	x, w, y, h;
} HIT_BOX;

/* in the hit_box_tbl table, the body's box for each stance, against the other side */
#define PUSH_BOX	( 0x7e0 / 8 )

/* the data of the hit that struck a player */
typedef struct HIT_DATA {
	Uint8	damage;	/* 000: the damage; its points for the score are damage / 5, over 13 a strong vibration */
	Uint8	damage_type;	/* 001: the reaction (DM_NO_MASK) and how the knock-back goes */
	Uint8	atck_kind;	/* 002: how it must be guarded (A_GUARD_MASK), and whether it is a special move or a hyper */
	Uint8	damage_kind;	/* 003: the hit's element and strength (K_ELEM_MASK, K_LV_MASK) */
	Uint8	piyo;	/* 004: stun points, less a random amount, taken from stun_cnt */
	Uint8	hit_stop;	/* 005: the hit stop it gives both sides */
	Uint8	sandbag_time;	/* 006: frames before an air recovery, copied to recover_timer */
	Uint8	sleep_time;	/* 007: the down time, copied to down_timer */
	Sint8	vector;		/* 008: the knock-back's direction, in 32 steps; also the hit mark's angle */
	Uint8	power;		/* 009: the knock-back's strength */
	Uint8	atck_range;	/* 00a: the attack's reach across, for the guard; not read */
	Uint8	hit_mark;	/* 00b: the hit mark, negative for none */
	Uint8	score;		/* 00c: its score (SC100 to SC300); not read: the points come from damage */
	Uint8	hit_se;		/* 00d: the hit's sound, through hit_se_tbl */
	Uint8	shc;	/* 00e: the hyper combo gauge it gives, low 6 bits; 0x40 none */
	Uint8	jug;	/* 00f: what it adds to the armour's hit count (armor_cnt) */
	Uint8	range_box;	/* 010: a box in hit_box_tbl the computer measures the attack's reach by, 0 none */
	Uint8	range_box2;	/* 011: and a second one, 0 none */
	Uint8	hit_flag;	/* 012: the hit's extra rules (NO_CHIP to AERIAL_FINISH) */
	Uint8	unk_13;		/* 013 */
	Uint8	guard_time;	/* 014: the guard's frames (guard_timer), 0 for the default by damage_kind (18, 14 or 11) */
	Uint8	unk_15;
	Uint8	catch_ng_time;	/* 016: the time it can't be thrown after (CATCH_NG_MASK, to catch_ng_timer), 0 none */
	Uint8	abyss_jug;	/* 017: what it adds to the Abyss's armour instead of jug, 0 for jug */
	Uint8	hit_cnt;	/* 018: the hits it counts for, 0 for one */
	Uint8	cpu_answer_no;	/* 019: the computer's answer to the attack, a record of its cpu_answer_adrs data */
	Uint8	unk_1a[0x1c - 0x1a];
} HIT_DATA;

/* HIT_DATA's damage_type: the reaction (dm_no) and how the knock-back goes; ZU, BU_O and AS in Capcom's tables are reactions 0, 1 and 4 */
#define DM_NO_MASK	0x1f	/* the reaction, copied to dm_no */
#define FLY_SCREEN	0x20	/* the victim flies off the screen and the screen follows */
#define NO_VEC		0x40	/* the knock-back speed is fixed and does not follow the vector */
#define NO_ZU		0x80	/* at the wall the attacker is not pushed back in the victim's place */

/* its atck_kind (and dm_guard_type): how it must be guarded, and the attack's class (Capcom's a_nom is 0) */
#define A_GUARD_MASK	0x0f	/* how the hit must be guarded */
#define A_HI		0x02	/* an overhead: guarded only standing */
#define A_LO		0x04	/* a low: guarded only crouching */
#define A_GUARD_NG	0x08	/* can't be guarded */
#define A_SPECIAL	0x10	/* a special move: chip damage, gauge, the combo's kind */
#define A_HYPER		0x20	/* a hyper combo: the hyper damage table, no gauge, armour can't take it */

/* its damage_kind (and dm_kind): the element, the strength (Capcom's k_s, k_m, k_l are 0x10, 0x20, 0x30) */
#define K_ELEM_MASK	0x07	/* the hit's element */
#define K_BACK_MAX	0x08	/* the strongest knock-back steps, whatever the strength */
#define K_LV_MASK	0x70	/* the hit's strength, which picks the reaction, the knock-back and the guard time */
#define K_LOW		0x80	/* a standing victim takes the lower-body reaction */

/* its hit_flag */
#define NO_CHIP		0x02	/* a guarded hit does no chip damage */
#define NO_WEIGHT	0x04	/* the knock-back doesn't change with the players' weights */
#define NO_SCALE	0x08	/* the damage isn't scaled down by the combo's length */
#define NO_COMBO	0x10	/* not counted in the combo */
#define NO_KO		0x20	/* can't KO: leaves 1 vital */
#define BLACK_COMBO	0x40	/* a launcher: hit flash, and a super jump may follow */
#define AERIAL_FINISH	0x80	/* ends an air combo after a super jump */

/* its catch_ng_time */
#define CATCH_NG_MASK	0x3f	/* the frames, copied to catch_ng_timer */

/* where a thrower holds its victim, by the victim's character and the grip in the thrower's frame */
typedef struct CATCH_POS {
	Sint16	x;		/* 000: in front of the thrower */
	Sint16	y;		/* 002: above it */
	Uint8	no;		/* 004: the victim's animation in group 16 */
	Uint8	dir;		/* 005: bit 0 turns the victim round */
	Sint8	prio;		/* 006: the victim's draw priority against the thrower's */
	Uint8	rot;		/* 007: the victim's rotation, in 256ths of the 16-bit angle */
} CATCH_POS;

/* a frame's boxes (frame_box_tbl, and frame_box for the current frame): numbers that box_ck compares in pairs */
typedef struct FRAME_BOX {
	Sint16	atck_box;	/* 000: the attack box (BOX_MASK); 0 none */
	Sint16	atck_box2;	/* 002: a second attack box (> 0); negative, a throw's box (low 13 bits) */
	Sint16	body_box0;	/* 004: the first of four body boxes (BOX_MASK), the upper bits its type */
	Sint16	body_box1;	/* 006: the second body box */
	Sint16	body_box2;	/* 008: the third body box */
	Uint16	body_box3;	/* 00a: the fourth body box, 0 none */
	Uint16	catch_box;	/* 00c: the box a throw can catch, 0 none */
	Sint16	unk_0e;		/* 00e */
} FRAME_BOX;

/* each box word: a box in hit_box_tbl in the low 13 bits, its type above */
#define BOX_MASK	0x1fff	/* the box's number in hit_box_tbl */

/* a player's animation frame, copied whole to cg_prm */
typedef struct CHAR_DATA {
	Sint32	loop_ofs;	/* 000: the frame's first codes (cg_prm on); after a last frame, the offset in char_adrs the animation goes on from */
	Uint8	unk_04[0x12 - 0x04];
	Uint16	frame_box_no;	/* 012: the frame's entry in frame_box_tbl */
} CHAR_DATA;

/* a record of the computer's data: offsets, from the data's start, of its chance table, its
   table of command picks (by level, in modes 0 and 1) and its command lists */
typedef struct CPU_DATA {
	Sint32	rate;
	Sint32	sel;
	Sint32	cmd;
} CPU_DATA;

/* each character's walking speed, forward and back, in 256ths: normally, sped up and in armour */
extern Sint16 walk_spd_tbl[][2];
extern Sint16 walk_spd_up_tbl[][2];
extern Sint16 walk_spd_armor_tbl[][2];

/* each character's files, and where each player work's are loaded */
extern const Uint16 pl_file_tbl[];	/* the player data file */
extern const Uint16 pic_file_tbl[];	/* the picture file */
extern const Uint16 vs_file_tbl[];	/* the versus model's pictures */
extern Uint8 * const pl_data_adrs_tbl[];	/* by work: where its player data is loaded */
extern Uint8 * const spr_set_buff_tbl[];	/* and its sprite set's buffer */
extern Sint32 * const pic_adrs_tbl[];	/* and its picture file */
extern const Sint8 col_no_tbl[];	/* by work: the first of its eight palette lines */
extern const VEC3 pl_start_pos_tbl[8];	/* where each side's players start */
extern const Sint8 col_cnt_tbl[];	/* by character: the palette lines it uses */

#endif
