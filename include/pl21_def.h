/* Charlie's animation numbers and special moves. */
#ifndef PL21_DEF_H
#define PL21_DEF_H

#define	CH21_WIN_KO0	0
#define	CH21_WIN_KO1	1
#define	CH21_WIN_KO2	2
#define	CH21_WIN_KO3	3
#define	CH21_WIN_TIME	3
#define	CH21_LOSE		4
#define	CH21_TM_DRAW	5

/* the character's own work, in tlocal */
typedef struct PL21_LOCAL_W {
	Uint8	renda_cnt;	/* Sonic Break: presses left; each eight add a boom */
	Uint8	target_type;	/* Crossfire Blitz: the opponent it set out for */
	Uint8	shot_flag;	/* a Sonic Boom is in the air */
} PL21_LOCAL_W;

/* pl_sp_type */
#define	pl21_sp_sonic	0	/* Sonic Boom */
#define	pl21_sp_smr		1	/* Somersault Shell */
#define	pl21_sp_break	2	/* Sonic Break (hyper) */
#define	pl21_sp_justice	3	/* Somersault Justice (hyper) */
#define	pl21_sp_blitz	4	/* Crossfire Blitz (hyper) */
#define	pl21_sp_moonsault	6	/* Moonsault Slash */
#define	pl21_sp_chouhatsu	7	/* Taunt */
#define	pl21_sp_snapback	11	/* Snapback */

/* atck_no: the character's own records in the hit table, after the common ATCK_ ones */
#define	pl21_at_fwd_hp			0x12	/* Forward + HP */
#define	pl21_at_back_hk			0x13	/* Back + HK, a hop kick */
#define	pl21_at_fwd_hk			0x14	/* Forward + HK, a slide */
#define	pl21_at_jdown_lk		0x15	/* Down + LK in the air */
#define	pl21_at_throw_p			0x20	/* Ground punch throw */
#define	pl21_at_throw_k			0x21	/* Ground kick throw, the final throw */
#define	pl21_at_air_throw		0x22	/* Air throw, the final throw after landing */
#define	pl21_at_throw_k_hit		0x23	/* Ground kick throw, the hit before the throw */
#define	pl21_at_air_throw_hit	0x24	/* Air throw, the hit after landing before the throw */
#define	pl21_at_sonic			0x30	/* Sonic Boom: his own record as he throws it */
#define	pl21_at_smr_2			0x36	/* Somersault Shell, the later hit as it rises (all strengths) */
#define	pl21_at_justice_1		0x37	/* Somersault Justice hyper, the rush's first hits */
#define	pl21_at_justice_2		0x38	/* Somersault Justice, the later hits and the final shell */
#define	pl21_at_blitz			0x39	/* Crossfire Blitz hyper, the dash that grabs */
#define	pl21_at_t_kick			0x41	/* The partner's jumping kick as he comes in */
#define	pl21_at_justice_ef		0x45	/* Somersault Justice, the shot that flies up and ahead (shl21_00 type 8) */
#define	pl21_at_moonsault_2		0x49	/* Moonsault Slash, the second hit */
#define	pl21_at_smr_counter		0x51	/* Somersault Shell as a variable counter (team_type 3), first hit */
#define	pl21_at_smr_counter_2	0x52	/* Somersault Shell as a variable counter, the later hit */
#define	pl21_at_smr_counter_ef	0x53	/* The Somersault Shell's effect (shl21_00 type 7), its hit as a variable counter */
#define	pl21_at_snapback		0x54	/* Snapback (sp_snapback) */
#define	pl21_at_upper			0x55	/* Launcher */
#define	pl21_at_break			0x56	/* Sonic Break hyper */

#endif
