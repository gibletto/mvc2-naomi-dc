/* Blackheart's special moves. */
#ifndef PL35_DEF_H
#define PL35_DEF_H

/* pl_sp_type */
#define	pl35_sp_heart		0	/* Heart of Darkness (hyper) */
#define	pl35_sp_armageddon	1	/* Armageddon (hyper) */
#define	pl35_sp_thunder		2	/* Dark Thunder */
#define	pl35_sp_inferno		3	/* Inferno */
#define	pl35_sp_chouhatsu	5	/* the taunt */
#define	pl35_sp_judgement	8	/* Judgement Day (hyper) */
#define	pl35_sp_snapback	9	/* the snapback */
#define	pl35_sp_gamma		10	/* the gamma assist's and counter's attack */

/* atck_no: the character's own records in the hit table, after the common ATCK_ ones */
#define	pl35_at_fiend_hp				0x12	/* shl35_04 type 0: the five fiends of standing HP */
#define	pl35_at_fiend_hk				0x13	/* shl35_04 type 3: the six fiends of standing HK */
#define	pl35_at_fiend_chp				0x14	/* shl35_04 type 1: the three fiends of crouching HP */
#define	pl35_at_fiend_chk				0x15	/* shl35_04 type 4: the fiend of crouching HK */
#define	pl35_at_fiend_jhp				0x16	/* shl35_04 type 2: the three fiends of jumping HP */
#define	pl35_at_fiend_jhk				0x17	/* shl35_04 type 5: the five fiends of jumping HK */
#define	pl35_at_throw_p					0x20	/* Punch throw (p_catch_throw) */
#define	pl35_at_throw_k					0x21	/* Kick throw (k_catch_wait) */
#define	pl35_at_air_throw				0x22	/* Air throw (j_catch_wait) */
#define	pl35_at_judgement_1				0x27	/* Judgement Day fiend on the ground (shl35_04 type 10), odd frames */
#define	pl35_at_jjudgement_1			0x28	/* Judgement Day fiend in the air (type 11), odd frames */
#define	pl35_at_judgement_2				0x29	/* Judgement Day fiend on the ground (type 10), even frames */
#define	pl35_at_jjudgement_2			0x2a	/* Judgement Day fiend in the air (type 11), even frames */
#define	pl35_at_thunder_bolt			0x2b	/* Dark Thunder: the pieces the fiend hands on to (shl35_01 type 0xff) */
#define	pl35_at_thunder_bolt_counter	0x2c	/* Dark Thunder's pieces as the variable counter (team_type 3) */
#define	pl35_at_inferno_counter			0x30	/* Inferno fiends as the variable counter (team_type 3) */
#define	pl35_at_inferno					0x31	/* Inferno fiends (shl35_03 types 3 and 4) */
#define	pl35_at_inferno_assist			0x32	/* Inferno fiends as the partner's assist (team_type 8) */
#define	pl35_at_heart					0x33	/* Heart of Darkness (sp_heart): the puffs (shl35_00), set every 5 frames */
#define	pl35_at_thunder_counter			0x35	/* Dark Thunder's fiend as the counter; otherwise 52 light / 54 heavy */
#define	pl35_at_armageddon_drop			0x37	/* Armageddon (sp_armageddon): the dropped fiend (shl35_02 type 0) */
#define	pl35_at_armageddon_bat_2		0x38	/* Armageddon's bats (shl35_02 type 2), the 1-in-4 record */
#define	pl35_at_armageddon_bat			0x39	/* Armageddon's bats (shl35_02 type 2), the usual record */
#define	pl35_at_armageddon_blast		0x3b	/* Armageddon: the blast at the dropped fiend's place as the fiends leave (shl35_02 type 3) */
#define	pl35_at_judgement				0x3e	/* Judgement Day, Blackheart himself (judgement_init) */
#define	pl35_at_t_kick					0x3f	/* Partner jump-in attack */
#define	pl35_at_counter					0x40	/* The gamma assist's attack (sp_gamma) as the variable counter (team_type 3) */
#define	pl35_at_snapback				0x41	/* Snapback's fiend (shl35_04 type 7, sub_type 1, set by snapback_move) */
#define	pl35_at_gamma					0x42	/* The gamma assist's attack (sp_gamma) */
#define	pl35_at_upper					0x43	/* Launcher (pl35_upper) */


/* the character's own work, in tlocal */
typedef struct PL35_LOCAL_W {
	Uint8	fiend_end_flag;	/* his hyper combo's fiends are to leave (shl35_02) */
	Uint8	unk_01;
	Uint8	hold_cnt;		/* fiends holding the opponent; each takes the next number */
	Uint8	hold_timer;		/* frames the fiends may hold on */
	Uint8	shot_no;		/* counts shot_init; an effect ends when it changes */
	Uint8	free_cnt;		/* the holding fiends let go so far */
	Sint8	unk_06;
	Uint8	land_flag;		/* he has landed after his entrance */
	Uint8	lever_off_flag;	/* the stick was let go during a dash */
	Uint8	se_timer;		/* frames to a sound */
} PL35_LOCAL_W;

#endif
