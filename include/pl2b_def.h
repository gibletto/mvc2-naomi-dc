/* Sabretooth's special moves. */
#ifndef PL2B_DEF_H
#define PL2B_DEF_H

/* pl_sp_type */
#define	pl2b_sp_berserk	0	/* Berserker Claw */
#define	pl2b_sp_birdie	1	/* Birdie */
#define	pl2b_sp_fang	2	/* Wild Fang */
#define	pl2b_sp_clawx	3	/* Berserker Claw X (hyper) */
#define	pl2b_sp_hbirdie	4	/* Hyper Birdie (hyper) */
#define	pl2b_sp_weapon	5	/* Weapon X Rush (hyper) */
#define	pl2b_sp_06		6	/* nothing sets it; its routine is Berserker Claw's */
#define	pl2b_sp_crouch_hp		7	/* crouching heavy punch (gamma assist) */
#define	pl2b_sp_snapback	8	/* Snapback */
#define	pl2b_sp_chouhatsu	9	/* Taunt */

/* atck_no: the character's own records in the hit table, after the common ATCK_ ones */
#define	pl2b_at_upper		0x12	/* The launcher (pl2b_upper), crouching */
#define	pl2b_at_stand_lp_2	0x19	/* Second hit of the standing light punch */
#define	pl2b_at_stand_hp_2	0x1a	/* Second hit of the standing heavy punch */
#define	pl2b_at_throw_2		0x20	/* Ground throw: the opponent tossed away after the slam */
#define	pl2b_at_air_throw	0x21	/* Air throw, slammed on landing */
#define	pl2b_at_throw		0x22	/* Ground throw: the slam (catch_damage) */
#define	pl2b_at_birdie		0x32	/* Birdie (him and the thrown ball, shl2b_01 type 0) */
#define	pl2b_at_fang		0x33	/* Wild Fang leap */
#define	pl2b_at_fang_throw	0x34	/* Wild Fang's throw of the opponent it catches */
#define	pl2b_at_clawx		0x35	/* Berserker Claw X rush, first hit (timer1 + 0x35 gives the next ones) */
#define	pl2b_at_clawx_last	0x38	/* Berserker Claw X last blow (him and shl2b_02 when pl_sp_type 3) */
#define	pl2b_at_hbirdie		0x39	/* Hyper Birdie (him, and shl2b_01's balls: 0x39 or 0x3a at random) */
#define	pl2b_at_weapon		0x3b	/* Weapon X Rush, first hit (cg_flag1 + 0x3b gives the next ones) */
#define	pl2b_at_weapon_last	0x41	/* Weapon X Rush's last blow (shl2b_02 type 0 set by weapon_drag) */
#define	pl2b_at_t_kick		0x42	/* The partner's jump-in attack (t_kick_init) */
#define	pl2b_at_snapback	0x43	/* Snapback (sp_snapback) */
#define	pl2b_at_crouch_hp	0x44	/* Crouching heavy punch as gamma assist (sp_crouch_hp) */
#define	pl2b_at_crouch_hp_2	0x45	/* Its second hit (crouch_hp_move) */

/* the character's own work, in tlocal, read by its effects */
typedef struct PL2B_LOCAL_W {
	Uint8	birdie_flag;	/* 00: set while Birdie's or Hyper Birdie's effect is out */
	Uint8	unk_01[7];
	PLAYER_WORK	*drag_w;	/* 08: the opponent being dragged */
} PL2B_LOCAL_W;

#define PL2B_LW( pl )	( ( PL2B_LOCAL_W * )( pl )->tlocal )

#endif
