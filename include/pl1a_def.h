/* Abyss's third form's special moves. */
#ifndef PL1A_DEF_H
#define PL1A_DEF_H

/* pl_sp_type */
#define	pl1a_sp_storm	0	/* a storm of shots (hyper) */
#define	pl1a_sp_stomp	1	/* a leap that comes down on the opponent */
#define	pl1a_sp_jump	2	/* jumps at the opponent, firing */
#define	pl1a_sp_wave	3	/* a shot along the floor */
#define	pl1a_sp_spray	4	/* shots sprayed out at random */
#define	pl1a_sp_blast	5	/* a lasting blast in front of him */
#define	pl1a_sp_win	6	/* sinks and comes up before the opponent (win) */
#define	pl1a_sp_win_voice	7	/* sinks and comes up before the opponent (win, with a voice) */
#define	pl1a_sp_throw	8	/* grabs, leaps and drops on the opponent */
#define	pl1a_sp_sink	9	/* sinks away for a time */
#define	pl1a_sp_fade	10	/* fades away (lose) */
#define	pl1a_sp_flame	11	/* hides in dark flames */

/* atck_no: the character's own records in the hit table, after the common ATCK_ ones */
#define	pl1a_at_wave		0x30	/* The shot along the floor: Abyss himself */
#define	pl1a_at_wave_shot	0x31	/* The shot along the floor and its seven pieces (shl1a_00_wave and shl1a_00_wave_part) */
#define	pl1a_at_stomp		0x32	/* The leap that comes down on the opponent */
#define	pl1a_at_jump		0x34	/* Jumps at the opponent, firing */
#define	pl1a_at_spray_shot	0x36	/* Each shot sprayed out by the storm and the spray (shl1a_00_spray and shl1a_00_shot) */
#define	pl1a_at_spray		0x38	/* The random spray: Abyss himself; also the storm's opening effect (shl1a_00_storm) */
#define	pl1a_at_blast		0x3b	/* The lasting blast in front of him, Abyss and the blast (shl1a_00_blast) */
#define	pl1a_at_throw		0x3d	/* Grabs, leaps and drops on the opponent (sp_throw) */

/* the character's own work, in tlocal */
typedef struct PL1A_LOCAL_W {
	Uint8	damage_flag;	/* set while he takes a hit; his body box (shl1a_01) then strikes */
	Uint8	body_atck_flag;	/* set in moves where his body box (shl1a_01) strikes */
	Sint8	spark_cnt;		/* 002: the hit sparks of pls1a_00 on this player, six at most */
	Uint8	unk_03;
	Float	body_alpha;		/* 1.0 but while he walks out; the body box strikes only at 1.0 */
} PL1A_LOCAL_W;

#define	PL1A_LW( pl )	( ( PL1A_LOCAL_W * )( pl )->tlocal )

#endif
