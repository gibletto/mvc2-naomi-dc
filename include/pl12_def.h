/* Hayato's special moves. */
#ifndef PL12_DEF_H
#define PL12_DEF_H

/* pl_sp_type */
#define	pl12_sp_shiden			0	/* Shiden */
#define	pl12_sp_guren			1	/* Guren */
#define	pl12_sp_byakko			2	/* Byakkou Hou */
#define	pl12_sp_engetsu				3	/* Engetsu (hyper) */
#define	pl12_sp_rasetsu			4	/* Rasetsu Zan (hyper) */
#define	pl12_sp_field			5	/* Plasma Field (hyper) */
#define	pl12_sp_snapback			6	/* the snapback */
#define	pl12_sp_counter			7	/* Guren (variable counter) */
#define	pl12_sp_black			8	/* Black Hayato (hyper) */
#define	pl12_sp_chouhatsu		9	/* the taunt */

/* atck_no: the character's own records in the hit table, after the common ATCK_ ones */
#define	pl12_at_upper		0x12	/* The launcher (pl12_upper) */
#define	pl12_at_fwd_hp		0x13	/* Forward + standing HP (stand_punch, Char_Stand_P 3) */
#define	pl12_at_jdown_hk	0x14	/* Jumping down + HK, the dive (jump_kick) */
#define	pl12_at_dash_hp		0x15	/* Standing HP that lunges forward (stand_punch_dash, used when atck_type is 3 and jump_dir is 0, Char_Extra 3) */
#define	pl12_at_rush_hp		0x16	/* The third HP's rush-in hit when HP is held (rush_move) */
#define	pl12_at_stand_hk_2	0x17	/* Standing HK's second hit (stand_hk_move) */
#define	pl12_at_throw_p		0x20	/* Ground punch throw (nml_p_catch) */
#define	pl12_at_throw_k		0x21	/* Ground kick throw (nk_land) */
#define	pl12_at_air_throw_p	0x22	/* Air punch throw (jp_land) */
#define	pl12_at_air_throw_k	0x23	/* Air kick throw (jump_k_catch) */
#define	pl12_at_black_end	0x24	/* Black Hayato's grab: last hit on the held opponent (black_cut_end) */
#define	pl12_at_shiden		0x30	/* Shiden's first slash, light version (atck_level + 0x30); Byakkou Hou's throwing slash (byakko_init) sets it too */
#define	pl12_at_field		0x36	/* Plasma Field (hyper): Hayato and the field (field_init here and in shl12_00) */
#define	pl12_at_rasetsu		0x37	/* Rasetsu Zan (hyper), the run of slashes (rasetsu_init) */
#define	pl12_at_engetsu_2	0x3e	/* Engetsu's waves (shl12_00's engetsu_init) */
#define	pl12_at_engetsu		0x3f	/* Engetsu (hyper): Hayato himself (engetsu_init) */
#define	pl12_at_t_kick		0x41	/* The partner's jump-in attack (t_kick) */
#define	pl12_at_snapback	0x42	/* Snapback (sp_snapback) */
#define	pl12_at_black		0x43	/* Black Hayato (sp_black): the lunging grab */
#define	pl12_at_back_lp		0x44	/* Back + standing LP, chain opener (Char_Extra 5) */
#define	pl12_at_chain_06	0x45	/* Chained attack, nml_no 3 (Char_Extra 6) */
#define	pl12_at_chain_07	0x46	/* Chained attack, nml_no 4 (Char_Extra 7) */
#define	pl12_at_chain_08	0x47	/* Chained attack, nml_no 5 (Char_Extra 8) */
#define	pl12_at_chain_09	0x48	/* Chained attack, nml_no 6 (Char_Extra 9) */
#define	pl12_at_chain_10	0x49	/* Chained kick, nml_no 3 (Char_Extra 10) */
#define	pl12_at_chain_11	0x4a	/* Chained attack, nml_no 7 (Char_Extra 11) */
#define	pl12_at_chain_12	0x4b	/* Chained attack, nml_no 8 (Char_Extra 12) */
#define	pl12_at_back_hp		0x4c	/* Back + standing HP when no light or medium attack started the string (Char_Extra 13) */
#define	pl12_at_chain_14	0x4d	/* Chained attack, nml_no 9 (Char_Extra 14) */
#define	pl12_at_chain_15	0x4e	/* Chained attack, nml_no 10 (Char_Extra 15) */
#define	pl12_at_chain_16	0x4f	/* Chained attack, nml_no 11 (Char_Extra 16) */
#define	pl12_at_chain_17	0x50	/* Chained kick, nml_no 4 (Char_Extra 17) */
#define	pl12_at_chain_18	0x51	/* Chained attack, nml_no 12 (Char_Extra 18) */
#define	pl12_at_shiden_vc	0x54	/* Shiden's first dash as a variable counter (shiden_init, team_type 3) */
#define	pl12_at_shiden_vc_2	0x55	/* Shiden's second dash as a variable counter (shiden_next, team_type 3) */
#define	pl12_at_guren_vc	0x56	/* Guren as a variable counter, a rising slash (sp_counter) */

/* the character's own work, in tlocal */
typedef struct PL12_LOCAL_W {
	Sint16	hyper_timer;	/* counts down while the hyper combos are free (shl12_00 starts it) */
	Sint16	weak_flag;		/* a light or medium attack began the string */
	Sint16	flash_flag;		/* the body flashes */
} PL12_LOCAL_W;

#endif
