/* Sakura's special moves. */
#ifndef PL22_DEF_H
#define PL22_DEF_H

/* pl_sp_type */
#define	pl22_sp_shouou	0	/* Shououken */
#define	pl22_sp_shunpu	1	/* Shunpukyaku */
#define	pl22_sp_chouhatsu	2	/* Taunt */
#define	pl22_sp_hadou	5	/* Hadouken */
#define	pl22_sp_midare	6	/* Midare Zakura (hyper) */
#define	pl22_sp_jhadou	7	/* Hadouken in the air */
#define	pl22_sp_shinkuu	8	/* Shinkuu Hadouken (hyper) */
#define	pl22_sp_dshinkuu	11	/* Shinkuu Hadouken, dark form (hyper) */
#define	pl22_sp_jshunpu	12	/* Shunpukyaku in the air */
#define	pl22_sp_jshouou	13	/* Shououken in the air */
#define	pl22_sp_haru	14	/* Haru Ichiban (hyper) */
#define	pl22_sp_ashura_f	16	/* Ashura Senkuu forward */
#define	pl22_sp_ashura_b	17	/* Ashura Senkuu back */
#define	pl22_sp_shungoku	18	/* Shun Goku Satsu (level 3 hyper) */
#define	pl22_sp_snapback	19	/* Snapback */
#define	pl22_sp_dark	20	/* The change to and from the dark form (level 3) */

/* atck_no: the character's own records in the hit table, after the common ATCK_ ones */
#define	pl22_at_fwd_hk			0x12	/* Forward + HK standing */
#define	pl22_at_throw_p_hit		0x20	/* Punch throw, the hits on the held opponent */
#define	pl22_at_throw_p			0x21	/* Punch throw, the final throw */
#define	pl22_at_throw_k_1		0x22	/* Kick throw, first record set on the opponent */
#define	pl22_at_throw_k			0x23	/* Kick throw, the throw itself (catch_type 1) */
#define	pl22_at_shungoku		0x24	/* Shun Goku Satsu: the opponent's record when the flurry ends */
#define	pl22_at_chouhatsu		0x3c	/* The taunt, which has a hit (sp_chouhatsu) */
#define	pl22_at_t_kick			0x3d	/* The partner's jumping kick as she comes in */
#define	pl22_at_win_ball		0x43	/* A win pose's falling, bouncing effect (shl22_00 types 2 and 3) */
#define	pl22_at_midare			0x47	/* Midare Zakura hyper, first record */
#define	pl22_at_shunpu_assist	0x4c	/* Shunpukyaku as a partner's attack (team_type 8) */
#define	pl22_at_shinkuu			0x51	/* Shinkuu Hadouken hyper (sp_shinkuu): her record and the sparks (shl22_00 type 5) */
#define	pl22_at_dshinkuu		0x55	/* Dark-form Shinkuu Hadouken hyper (sp_dshinkuu): her record and every third petal hit (shl22_01) */
#define	pl22_at_dshinkuu_2		0x56	/* Dark-form Shinkuu Hadouken, the other petal hits */
#define	pl22_at_jshunpu_land	0x5d	/* Air Shunpukyaku, the hit once she reaches the floor */
#define	pl22_at_haru			0x65	/* Haru Ichiban hyper, first record */
#define	pl22_at_shinkuu_last	0x67	/* Shinkuu Hadouken, the last round of sparks (sub_type 1) */
#define	pl22_at_snapback		0x6b	/* Snapback (sp_snapback) */
#define	pl22_at_upper_p			0x6c	/* Crouching HP launcher */
#define	pl22_at_upper_k			0x71	/* The kick launcher (standing) */

/* the character's own work, in tlocal */
typedef struct PL22_LOCAL_W {
	Float	save_x;			/* where the entrance stops; Midare Zakura's distance to the opponent */
	Uint16	cmd_cnt;		/* how far the win pose's command has got */
	Uint16	unk_06;
	Uint16	cmd_flag;		/* set once that command is done */
	Uint16	win5_flag;		/* set when win pose 5's effect has come near */
	Uint16	win6_flag;		/* and win pose 6's */
	Sint8	armor_end;		/* how the Shououken's run ended: 1 she rose, 2 it was guarded or ran out */
	Sint8	armor_flag;		/* set while a move takes hits */
	Uint8	unk_10[0x15 - 0x10];
	Uint8	renda_cnt;		/* the dark Shinkuu Hadouken: presses left that hold it longer */
	Uint8	unk_16[0x18 - 0x16];
	PLAYER_WORK	*carry_w;		/* the opponent Midare Zakura carries */
} PL22_LOCAL_W;

#define PL22_LW( pl )	( ( PL22_LOCAL_W * )( pl )->tlocal )

#endif
