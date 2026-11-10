/* Ruby Heart's special moves. */
#ifndef PL13_DEF_H
#define PL13_DEF_H

/* pl_sp_type */
#define	pl13_sp_schwarz				0	/* Schwarzaile */
#define	pl13_sp_fantome				1	/* Fantome */
#define	pl13_sp_sublimation			2	/* Sublimation */
#define	pl13_sp_flammeile			3	/* Flammeile (hyper) */
#define	pl13_sp_mille				4	/* Mille Fantomes (hyper) */
#define	pl13_sp_paretonnre				5	/* Pare-tonn're (hyper) */
#define	pl13_sp_rafale				6	/* Rafale Cannon */
#define	pl13_sp_throw			7	/* a throw that carries the opponent (assist) */
#define	pl13_sp_power			8	/* a power-up for the partner (assist) */
#define	pl13_sp_magie			9	/* Tour De Magie (hyper) */
#define	pl13_sp_snapback			10	/* the snapback */
#define	pl13_sp_chouhatsu		11	/* the taunt */
#define	pl13_sp_flammeile_team			12	/* Flammeile as her part of a team hyper */

/* atck_no: the character's own records in the hit table, after the common ATCK_ ones */
#define	pl13_at_t_kick				0x13	/* The jump-in kick when she is tagged in (t_kick_init) */
#define	pl13_at_snapback			0x14	/* Snapback (snapback_init) */
#define	pl13_at_upper				0x15	/* The launcher (pl13_upper) */
#define	pl13_at_stand_mk_2			0x19	/* Standing MK's second hit (stand_mk_move) */
#define	pl13_at_throw_p				0x20	/* Ground throw (nml_p_catch) */
#define	pl13_at_air_throw			0x21	/* Air throw (jump_p_catch_throw) */
#define	pl13_at_magie_catch			0x23	/* Tour De Magie: the opponent caught by kind 14 (ef_magie_catch_hold) */
#define	pl13_at_magie				0x30	/* Tour De Magie (hyper): kind 14, which drops and catches (ef_magie_init) */
#define	pl13_at_throw				0x31	/* The carrying-throw assist and its kind 4 (throw_init, ef_throw_init); rafale_throw_init sets it too */
#define	pl13_at_power				0x32	/* The power-up assist's kind 5, which powers up a player it hits (ef_power_init) */
#define	pl13_at_schwarz				0x33	/* Schwarzaile flight: start and each turn (schwarz_init, schwarz_turn) */
#define	pl13_at_schwarz_2			0x34	/* Schwarzaile flight's later hits (schwarz_fly, hit_cnt 0) */
#define	pl13_at_schwarz_3			0x35	/* Schwarzaile flight's later hits (schwarz_fly, hit_cnt 1) */
#define	pl13_at_flammeile			0x38	/* Flammeile (hyper) air flight: start and each turn (flammeile_init, flammeile_turn) */
#define	pl13_at_flammeile_2			0x39	/* Flammeile's later hits (flammeile_fly, air_hit_cnt 0) */
#define	pl13_at_flammeile_3			0x3a	/* Flammeile's later hits (flammeile_fly, air_hit_cnt 1) */
#define	pl13_at_rafale				0x3d	/* Rafale Cannon's hand (kind 6), and its capture (ef_rafale_init, ef_rafale_catch_hold) */
#define	pl13_at_fantome				0x3e	/* Fantome: Ruby and the ghost (fantome_init, ef_fantome_shot_init); mille_init sets it too */
#define	pl13_at_sublimation			0x3f	/* Sublimation's strike (sublimation_init, ef_sublimation_init); power_init sets it too */
#define	pl13_at_mille				0x40	/* Mille Fantomes (hyper): the three-hit shots from kind 22 (kind 23, ef_mille_shot_init) */
#define	pl13_at_paretonnre			0x41	/* Pare-tonn're (hyper): start and its charge's hits (paretonnre_init, ef_paretonnre_atck_init); magie_init too */
#define	pl13_at_paretonnre_last		0x42	/* Pare-tonn're's last hit, with a quake (ef_paretonnre_atck_hit) */
#define	pl13_at_paretonnre_blast_2	0x43	/* Pare-tonn're: the blast on the opponent, second part (ef_paretonnre_blast_init, sub_no set) */
#define	pl13_at_paretonnre_blast	0x44	/* Pare-tonn're: the blast on the opponent (ef_paretonnre_blast_init, sub_no 0) */
#define	pl13_at_mille_2				0x45	/* Mille Fantomes shot's middle hit (ef_mille_shot_fly, routine_3 6) */
#define	pl13_at_flammeile_team				0x47	/* Flammeile as her part of a team hyper: its nine hits (sp_flammeile_team) */
#define	pl13_at_schwarz_vc			0x48	/* Schwarzaile as a variable counter (schwarz_init, team_type 3) */

/* the character's own work, in tlocal */
typedef struct PL13_LOCAL_W {
	Uint8	fantome_shot_flag;	/* Fantome's shot (shl13_00 kind 0) is out */
	Uint8	mille_shot_flag;	/* Mille Fantomes' effect (shl13_00 kind 22) is out */
	Uint8	unk_02[0x06 - 0x02];
	Uint8	hit_cnt;		/* hits done in this flight */
	Uint8	unk_07;
	Uint16	hit_time;		/* frames to the next hit */
	Uint8	air_hit_cnt;	/* hits done in the air flight */
	Uint8	unk_0b;
	Uint16	air_hit_time;	/* frames to its next hit */
	Uint8	shot_on;		/* set until the shot has gone */
	Uint8	unk_0f;
	Sint16	paretonnre_wait_cnt;	/* frames Pare-tonn're's effect (kind 16) has waited, which lengthen its time */
	Uint8	unk_12[0x14 - 0x12];
	Uint8	sublimation_shot_flag;	/* Sublimation's strike (shl13_00 kind 2) is out */
} PL13_LOCAL_W;

#endif
