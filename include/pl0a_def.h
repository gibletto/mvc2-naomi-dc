/* Rogue's special moves. */
#ifndef PL0A_DEF_H
#define PL0A_DEF_H

/* pl_sp_type */
#define	pl0a_sp_repeat	3	/* Repeating Punches */
#define	pl0a_sp_leap	8	/* Ken's power: a leap forward and down */
#define	pl0a_sp_drain	11	/* Power Drain */
#define	pl0a_sp_screw	12	/* Screw Piledriver, Zangief's power */
#define	pl0a_sp_rising	13	/* Rising Repeating Punches */
#define	pl0a_sp_spike	17	/* Cannon Spike, Cammy's power */
#define	pl0a_sp_barrage	18	/* Berserker Barrage, Wolverine's power */
#define	pl0a_sp_chouhatsu	19	/* Taunt */
#define	pl0a_sp_sugah	20	/* Goodnight Sugah (hyper) */
#define	pl0a_sp_dive	22	/* Dive Punch */
#define	pl0a_sp_shungoku	23	/* Shun Goku Satsu, Akuma's power (hyper) */
#define	pl0a_sp_team	24	/* the team hyper combo */
#define	pl0a_sp_snapback	26	/* Snapback */
#define	pl0a_sp_barragex	27	/* Berserker Barrage X, Wolverine's power (team hyper) */
#define	pl0a_sp_reppa	28	/* Shoryu-Reppa, Ken's power (team hyper) */

/* atck_no: the character's own records in the hit table, after the common ATCK_ ones */
#define	pl0a_at_throw_p		0x22	/* Her punch throw, ground or air (nml_catch), set on the opponent */
#define	pl0a_at_shungoku	0x23	/* Shun Goku Satsu (Akuma's power): the last blow, set on her and the opponent */
#define	pl0a_at_sugah		0x24	/* Goodnight Sugah (hyper): the drop; the drain with super_flag uses it too */
#define	pl0a_at_screw		0x26	/* Screw Piledriver (Zangief's power): the slam */
#define	pl0a_at_dive		0x30	/* The start of Dive Punch (sp_dive); Shoryu-Reppa (sp_reppa) and t_kick also start with it, and the animation's marks (hit_set) give the hits after it */
#define	pl0a_at_barrage		0x62	/* Berserker Barrage (Wolverine's power, sp_barrage), also Berserker Barrage X (sp_barragex) */
#define	pl0a_at_kiss		0x6b	/* The taunt's kiss (shl0a_00 kind 0) */
#define	pl0a_at_snapback	0x77	/* Snapback (sp_snapback) */
#define	pl0a_at_upper		0x78	/* The launcher */

/* the powers' timers, as word indexes of tlocal; a power's number is the one
   power_set gives obj_flag */
#define	TM_POWER_1		11
#define	TM_POWER_2		12
#define	TM_POWER_3		13
#define	TM_POWER_8		14
#define	TM_POWER_4		15
#define	TM_POWER_7		16
#define	TM_POWER_5		17
#define	TM_POWER_9		18
#define	TM_WAIT			19		/* holds the others for a few frames */

/* the character's own work, in tlocal */
typedef struct PL0A_LOCAL_W {
	Uint8	unk_00[0x04];
	Sint32	power_type;		/* the character whose power she has taken */
	Uint8	unk_08[0x14 - 0x08];
	Sint32	win_no;			/* the win pose */
	Sint32	super_flag;		/* 0xff in the hyper combo's drain, 0 in the special's */
	Uint8	unk_1c[0x24 - 0x1c];
	Sint32	glow_timer;		/* frames the glow (pls0a_02) stays on */
	Sint32	rise_sw;		/* the button that keeps the Dive Punch rising */
	Uint8	power_timer[0x50 - 0x2c];	/* the powers' timers, tlocal[TM_POWER_1] and on */
	Uint8	unk_50[0x7c - 0x50];
	Sint32	nml_move;		/* the attack's own moves */
} PL0A_LOCAL_W;

#define	PL0A_LW( pl )	( ( PL0A_LOCAL_W * )( pl )->tlocal )

#endif
