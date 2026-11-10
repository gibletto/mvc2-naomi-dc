/* Abyss's first form's special moves. */
#ifndef PL18_DEF_H
#define PL18_DEF_H

/* pl_sp_type */
#define	pl18_sp_punch	0	/* a dark fist thrown out */
#define	pl18_sp_thunder	1	/* dark bolts dropped on the opponent (hyper) */
#define	pl18_sp_ball	2	/* three homing dark balls */
#define	pl18_sp_pillar	3	/* a row of dark pillars (hyper) */
#define	pl18_sp_change	4	/* his fall, to change into the second form */
#define	pl18_sp_sweep	5	/* a long sweeping blow */
#define	pl18_sp_rush	6	/* a charging rush */

/* atck_no: the character's own records in the hit table, after the common ATCK_ ones */
#define	pl18_at_t_kick		0x30	/* Tag-in kick that leaps off the wall (tk_init) */
#define	pl18_at_thunder		0x31	/* Dark bolts dropped on the opponent (hyper), and each spark (shl18_00) */
#define	pl18_at_ball		0x32	/* Three homing dark balls */
#define	pl18_at_sweep		0x33	/* The long sweeping blow */
#define	pl18_at_rush_1		0x34	/* The charging rush: odd hits */
#define	pl18_at_pillar		0x35	/* Row of dark pillars (hyper), Abyss and the pillars (shl18_03) */
#define	pl18_at_rush_end	0x3a	/* The charging rush: hits in its last steps (timer0 under 6) */
#define	pl18_at_punch		0x3b	/* The dark fist thrown out */
#define	pl18_at_rush_2		0x3d	/* The charging rush: even hits */


/* the character's own work, in tlocal */
typedef struct PL18_LOCAL_W {
	Sint8	hold_cnt;		/* presses left to hold a move longer */
	Uint8	hit_flag;		/* which of four things have been hit */
} PL18_LOCAL_W;

#define	PL18_LW( pl )	( ( PL18_LOCAL_W * )( pl )->tlocal )

#endif
