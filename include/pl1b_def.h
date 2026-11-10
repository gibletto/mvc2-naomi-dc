/* Chun-Li's win poses and special moves. */
#ifndef PL1B_DEF_H
#define PL1B_DEF_H

#define	CH1B_WIN_KO0	0
#define	CH1B_WIN_KO1	1
#define	CH1B_WIN_KO2	2
#define	CH1B_WIN_KO3	3
#define	CH1B_WIN_TIME	2
#define	CH1B_LOSE		4

/* pl_sp_type */
#define	pl1b_sp_kikou		0	/* Kikou Ken */
#define	pl1b_sp_tenshou		1	/* Tenshoukyaku */
#define	pl1b_sp_senen		2	/* Sen'en Shuu */
#define	pl1b_sp_kikoushou	3	/* Kikoushou (hyper) */
#define	pl1b_sp_senretsu	4	/* Senretsukyaku (hyper) */
#define	pl1b_sp_hazan		5	/* Hazan Tenshoukyaku (hyper) */
#define	pl1b_sp_hyakuretsu	6	/* Hyakuretsukyaku */
#define	pl1b_sp_chouhatsu	8	/* taunt */
#define	pl1b_sp_shichisei	11	/* Shichisei Ranka (air hyper) */
#define	pl1b_sp_snapback	15	/* Snap Back */

/* atck_no: the character's own records in the hit table, after the common ATCK_ ones */
#define	pl1b_at_downback_hk		0x12	/* Down-back + HK, the axe kick (Kaku Kyaku Raku) */
#define	pl1b_at_jdown_hk		0x13	/* Jumping down + HK, the head stomp */
#define	pl1b_at_jfwd_hk			0x14	/* Jumping forward + HK (Kaku Yoku Da) */
#define	pl1b_at_jump_hk			0x1a	/* Her own jumping HK, used where others use the common 0x11 */
#define	pl1b_at_throw			0x20	/* Ground throw */
#define	pl1b_at_air_throw		0x21	/* Air throw */
#define	pl1b_at_shichisei_2		0x22	/* Shichisei Ranka: the caught opponent thrown away (shichisei_throw) */
#define	pl1b_at_kikou			0x30	/* Kikou Ken, Chun-Li and the shot (shl1b_00 kind 0) */
#define	pl1b_at_kikoushou		0x40	/* Kikoushou (hyper), Chun-Li and the ball (shl1b_00 kind 1) */
#define	pl1b_at_senretsu		0x45	/* Senretsu Kyaku (hyper): the kicks */
#define	pl1b_at_senretsu_end	0x46	/* Senretsu Kyaku: the last kick after the mashing */
#define	pl1b_at_hazan			0x4b	/* Hazan Tenshou Kyaku (hyper) */
#define	pl1b_at_hyakuretsu		0x4d	/* Hyakuretsu Kyaku */
#define	pl1b_at_t_kick			0x50	/* Tag-in kick that leaps off the wall */
#define	pl1b_at_kikou_counter	0x52	/* Kikou Ken's shot as a variable counter (team_type 3) */
#define	pl1b_at_chouhatsu		0x56	/* The taunt, which can hit */
#define	pl1b_at_fwd_hp			0x57	/* Forward + HP, Reishiki Kikou Ken */
#define	pl1b_at_shichisei		0x5d	/* Shichisei Ranka (air hyper) */
#define	pl1b_at_snapback		0x61	/* Snapback, Chun-Li and its ball (shl1b_00 kind 6) */
#define	pl1b_at_upper			0x62	/* The launcher */

/* the character's own work, in tlocal */
typedef struct PL1B_LOCAL_W {
	Uint8	shot_flag;		/* no Kikouken while set */
	Uint8	kikoushou_shot_flag;	/* set while the hyper's shot lasts */
	Uint8	kikoushou_renda_cnt;	/* presses left to make it bigger */
	Uint8	unk_3;
	Uint8	senretsu_renda_cnt;	/* presses left to make the kicks last */
	Sint8	tap_timer;		/* frames left to press the kicks again in Hyakuretsukyaku */
} PL1B_LOCAL_W;

#endif
