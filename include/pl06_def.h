/* Cyclops's special moves. */
#ifndef PL06_DEF_H
#define PL06_DEF_H

#define	CH06_WIN_KO0	0
#define	CH06_WIN_KO1	1
#define	CH06_LOSE		2
#define	CH06_TM_DRAW	3
#define	CH06_WIN_TIME	5

/* pl_sp_type */
#define	pl06_sp_blast			0	/* Optic Blast */
#define	pl06_sp_super			1	/* Super Optic Blast (hyper) */
#define	pl06_sp_mega			2	/* Mega Optic Blast (hyper) */
#define	pl06_sp_gene			3	/* Gene Splice */
#define	pl06_sp_neckbreaker			4	/* Running Neckbreaker */
#define	pl06_sp_cyclone			7	/* Cyclone Kick */
#define	pl06_sp_sweep			8	/* Optic Sweep */
#define	pl06_sp_rapid			9	/* Rapid Punches */
#define	pl06_sp_chouhatsu		11	/* The taunt */
#define	pl06_sp_snapback		12	/* The snapback */

/* atck_no: the character's own records in the hit table, after the common ATCK_ ones */
#define	pl06_at_jdown_mp		0x12	/* Jumping M punch (second press) with the stick down */
#define	pl06_at_jdown_hp		0x13	/* Jumping HP with the stick down */
#define	pl06_at_jdown_lk		0x14	/* Jumping LK with the stick down */
#define	pl06_at_jdown_mk		0x15	/* Jumping M kick (second press) with the stick down */
#define	pl06_at_jdown_hk		0x16	/* Jumping HK with the stick down */
#define	pl06_at_stand_hk_2		0x1b	/* Standing HK's second kick, with the button still held */
#define	pl06_at_neckbreaker		0x20	/* The Running Neckbreaker's throw, set on the opponent (neckbreaker_catch) */
#define	pl06_at_crouch_throw_k	0x21	/* Kick throw from a crouch (crouch_k_catch), set on the opponent */
#define	pl06_at_throw_p			0x22	/* The punch throw (nml_p_catch), set on the opponent */
#define	pl06_at_beam_throw		0x23	/* The command throw (half circle back + P, beam_throw_catch) that aims the opponent with the stick; set with ATCK_DIR (0xa3) */
#define	pl06_at_air_throw		0x24	/* The air throw (jump_p_catch), set on the opponent */
#define	pl06_at_throw_k			0x25	/* The standing kick throw (nml_k_catch), a point-blank beam, set on the opponent */
#define	pl06_at_jblast			0x34	/* Optic Blast in the air */
#define	pl06_at_sweep			0x35	/* Optic Sweep */
#define	pl06_at_super			0x36	/* Super Optic Blast (hyper, sp_super, the beam that can be aimed) */
#define	pl06_at_mega			0x38	/* Mega Optic Blast (hyper, sp_mega) */
#define	pl06_at_t_kick			0x3a	/* The jump-in kick when he is tagged in (t_kick) */
#define	pl06_at_rapid			0x3b	/* Rapid Punches: the first punch, and the repeated hits in each stage */
#define	pl06_at_rapid_2			0x3c	/* Rapid Punches, second stage */
#define	pl06_at_rapid_3			0x3d	/* Rapid Punches, third stage */
#define	pl06_at_rapid_4			0x3e	/* Rapid Punches, last punch (it fires the blast) */
#define	pl06_at_snapback		0x43	/* Snapback */
#define	pl06_at_upper_p			0x44	/* The launcher, punch, standing */
#define	pl06_at_upper_k			0x45	/* The launcher, kick, crouching */

/* the character's own work, in tlocal */
typedef struct PL06_LOCAL_W {
	Uint8	beam_dir;		/* 00: the beam's aim, 0 to 15 */
	Sint8	beam_st;		/* 01: 64 or 32 while the beam fires, 0 as it stops */
	Uint16	beam_len;			/* 02 */
	Uint8	beam_flag;		/* 04: 0xff while the beam is shown */
	Uint8	renda_cnt;		/* 05: frames a hyper beam can still be stretched */
	Sint8	col_step;		/* 06: the step of the entrance's colours, and of the beam's flicker */
	Sint8	flash_timer;	/* 07: the beam's flicker timer */
} PL06_LOCAL_W;
#define	PL06_LW( pl )	( ( PL06_LOCAL_W * )( pl )->tlocal )

#endif
