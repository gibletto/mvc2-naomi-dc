/* Wolverine's special moves (character 07). */
#ifndef PL07_DEF_H
#define PL07_DEF_H

/* pl_sp_type */
#define	pl07_sp_tornado			0	/* Tornado Claw */
#define	pl07_sp_drill			1	/* Drill Claw */
#define	pl07_sp_barrage			2	/* Berserker Barrage */
#define	pl07_sp_slash			3	/* Berserker Slash */
#define	pl07_sp_weapon_x		4	/* Weapon X (hyper) */
#define	pl07_sp_barrage_x		5	/* Berserker Barrage X (hyper) */
#define	pl07_sp_fatal			6	/* Fatal Claw (hyper) */
#define	pl07_sp_charge			7	/* Berserker Charge (hyper) */
#define	pl07_sp_snapback		8	/* The snapback */
#define	pl07_sp_chouhatsu		9	/* The taunt */

/* atck_no: the character's own records in the hit table, after the common ATCK_ ones */
#define	pl07_at_upper			0x14	/* The launcher */
#define	pl07_at_stand_mk_2		0x1b	/* Standing M kick's second hit, with the button pressed again */
#define	pl07_at_stand_mp_2		0x1c	/* Standing M punch's second hit, with the button pressed again */
#define	pl07_at_sjump_mp_2		0x1d	/* Jumping M punch's second hit, in a super jump only */
#define	pl07_at_throw_p			0x20	/* The punch throw (nml_p_throw), set on the opponent */
#define	pl07_at_throw_p_hold	0x21	/* The punch throw's mashed hold hits (nml_p_hold_hit) */
#define	pl07_at_throw_k			0x22	/* The kick throw: he jumps up with the opponent (nml_k_jump) */
#define	pl07_at_throw_k_slam	0x23	/* The kick throw's slam down (nml_k_slam) */
#define	pl07_at_throw_k_hold	0x24	/* The kick throw's mashed hold hits (nml_k_hold_hit) */
#define	pl07_at_air_throw		0x25	/* The air throw: he leaps up with the opponent (jump_p_leap_init) */
#define	pl07_at_air_throw_hold	0x26	/* The air throw's mashed hold hits (jump_p_hold_hit) */
#define	pl07_at_weapon_x		0x30	/* Weapon X (hyper): the rush */
#define	pl07_at_weapon_x_slash	0x3a	/* Weapon X: the two-layer slash on the caught opponent (shl07_01) */
#define	pl07_at_barrage			0x3b	/* Berserker Barrage */
#define	pl07_at_barrage_x		0x41	/* Berserker Barrage X (hyper): first hit; the later ones are 0x41 + timer0 */
#define	pl07_at_fatal			0x44	/* Fatal Claw (hyper): his move and its slash (shl07_02) */
#define	pl07_at_t_kick			0x4a	/* The jump-in kick when he is tagged in (pl07_t_kick) */
#define	pl07_at_tornado_end		0x4b	/* Tornado Claw's last hit; the earlier hits are 0x4c + 2*level + n */
#define	pl07_at_charge			0x60	/* Berserker Charge (hyper): the start */
#define	pl07_at_snapback		0x63	/* Snapback (crouching slash) */

/* the character's own work, in tlocal */
typedef struct PL07_LOCAL_W {
	Uint8	dash_ng_flag;	/* 00: blocks another claw dash */
	Uint8	atck_flag;	/* 01: a button pressed during the dash */
	Uint8	air_combo_old;		/* 02 */
	Uint8	unk_03;
	struct PLAYER_WORK *em_w;	/* 04: the opponent caught by Weapon X */
	Sint16	catch_timer;	/* 08: frames since the catch */
	Uint8	unk_0a[2];
	Sint8	hold_st;	/* 0c: 1 while a throw holds on, -1 once it lets go */
	Uint8	unk_0d;		/* 0d */
} PL07_LOCAL_W;

#endif
