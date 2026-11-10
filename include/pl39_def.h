/* Bonerine's special moves (character 39). */
#ifndef PL39_DEF_H
#define PL39_DEF_H

/* pl_sp_type */
#define	pl39_sp_tornado	0	/* Tornado Claw */
#define	pl39_sp_drill	1	/* Drill Claw */
#define	pl39_sp_berserker	2	/* Berserker Barrage */
#define	pl39_sp_slash	3	/* Berserker Slash */
#define	pl39_sp_weapon_x	4	/* Weapon X (hyper) */
#define	pl39_sp_berserker_x	5	/* Berserker Barrage X (hyper) */
#define	pl39_sp_fatal	6	/* Fatal Claw (hyper) */
#define	pl39_sp_charge	7	/* Berserker Charge (hyper) */
#define	pl39_sp_snapback	8	/* Snapback */
#define	pl39_sp_chouhatsu	9	/* Taunt */
#define	pl39_sp_stand_hp		10	/* standing heavy punch (alpha assist) */
#define	pl39_sp_stand_hk		11	/* standing heavy kick (beta assist) */
#define	pl39_sp_slide		12	/* Sliding Claw (gamma assist) */

/* atck_no: the character's own records in the hit table, after the common ATCK_ ones */
#define	pl39_at_upper			0x14	/* Launcher (pl39_upper) */
#define	pl39_at_stand_mk_2		0x1b	/* Standing MK's follow-up hit, button held (stand_kick_move) */
#define	pl39_at_stand_mp_2		0x1c	/* Standing MP's chained second hit (cg_type 1) */
#define	pl39_at_sjump_mp_2		0x1d	/* Super jump MP's chained second hit */
#define	pl39_at_throw_p			0x20	/* Punch throw's toss (nml_p_throw) */
#define	pl39_at_throw_p_hold	0x21	/* Punch throw's hold (nml_p_hold_hit) */
#define	pl39_at_throw_k			0x22	/* Kick throw: jumping up with the opponent (nml_k_jump) */
#define	pl39_at_throw_k_slam	0x23	/* Kick throw: slamming him down (nml_k_slam) */
#define	pl39_at_throw_k_hold	0x24	/* Kick throw's hold (nml_k_hold_hit) */
#define	pl39_at_air_throw		0x25	/* Air throw: leaping up with the opponent (jump_p_leap_init) */
#define	pl39_at_air_throw_hold	0x26	/* Air throw's hold (jump_p_hold_hit) */
#define	pl39_at_weapon_x		0x30	/* Weapon X (sp_weapon_x) */
#define	pl39_at_berserker		0x3b	/* Berserker Barrage (sp_berserker) */
#define	pl39_at_berserker_x		0x41	/* Berserker Barrage X (sp_berserker_x) */
#define	pl39_at_fatal			0x44	/* Fatal Claw (sp_fatal) */
#define	pl39_at_t_kick			0x4a	/* Partner jump-in attack */
#define	pl39_at_tornado_last	0x4b	/* Tornado Claw's last hit (tornado_up, tornado_keep) */
#define	pl39_at_charge			0x60	/* Berserker Charge (sp_charge) */
#define	pl39_at_snapback		0x63	/* Snapback (sp_snapback) */
#define	pl39_at_assist_hp		0x64	/* Standing HP as the alpha assist (sp_stand_hp) */
#define	pl39_at_assist_hk		0x65	/* Standing HK as the beta assist (sp_stand_hk) */
#define	pl39_at_slide			0x66	/* Sliding Claw, the gamma assist (sp_slide) */

#endif
