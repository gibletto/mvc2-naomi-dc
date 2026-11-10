/* Felicia's special moves. */
#ifndef PL20_DEF_H
#define PL20_DEF_H

/* pl_sp_type */
#define	pl20_sp_delta	0	/* Delta Kick */
#define	pl20_sp_rolling	1	/* Rolling Buckler */
#define	pl20_sp_sand	2	/* Sand Splash */
#define	pl20_sp_spike	3	/* Cat Spike */
#define	pl20_sp_hellcat	4	/* Hell Cat */
#define	pl20_sp_dancing	6	/* Dancing Flash (hyper) */
#define	pl20_sp_help	7	/* Please Help Me (hyper) */
#define	pl20_sp_ssand	8	/* Super Sand Splash (hyper) */
#define	pl20_sp_snapback	9	/* Snapback */
#define	pl20_sp_chouhatsu	10	/* Taunt */

/* atck_no: the character's own records in the hit table, after the common ATCK_ ones */
#define	pl20_at_stand_lp		0x12	/* Standing LP (her own record, not the common 0x00) */
#define	pl20_at_stand_mp		0x13	/* Standing MP (her own record) */
#define	pl20_at_stand_hp_1		0x14	/* Standing HP, a chain of hits: first */
#define	pl20_at_stand_mk		0x16	/* Standing MK (her own record) */
#define	pl20_at_back_hk_1		0x17	/* Back + HK, first hit */
#define	pl20_at_stand_hp_alt	0x19	/* Standing HP hit for routine_2 0, which stand_punch never leaves, so it looks unreachable */
#define	pl20_at_stand_hp_2		0x1a	/* Standing HP, second hit (cg_type 1) */
#define	pl20_at_stand_hp_3		0x1b	/* Standing HP, third hit (cg_type 2) */
#define	pl20_at_back_hk_2		0x1c	/* Back + HK, second hit */
#define	pl20_at_jump_mp_2		0x1d	/* Jump MP, second hit */
#define	pl20_at_throw_p			0x20	/* Punch throw: the final throw after the hits, set on the opponent */
#define	pl20_at_throw_k			0x23	/* Kick throw (a leap with the opponent) */
#define	pl20_at_air_throw		0x24	/* Air throw */
#define	pl20_at_hellcat_throw	0x25	/* Hell Cat command throw, the final throw */
#define	pl20_at_hellcat_hit		0x26	/* Hell Cat, each hit on the held opponent */
#define	pl20_at_help_hit		0x27	/* Please Help Me, the hits while shl20_01's grab carries the opponent */
#define	pl20_at_help_last		0x28	/* Please Help Me, the last throw from shl20_01 */
#define	pl20_at_t_kick			0x46	/* The partner's jumping kick as she comes in */
#define	pl20_at_snapback		0x47	/* Snapback */
#define	pl20_at_upper			0x48	/* Launcher */
#define	pl20_at_dancing			0x49	/* Dancing Flash hyper, first hit (later hits are cg_flag1 + 73 / + 77) */
#define	pl20_at_help			0x50	/* Please Help Me hyper: her own record and shl20_01's grab */
#define	pl20_at_ssand			0x52	/* Super Sand Splash hyper */
#define	pl20_at_ssand_hit		0x53	/* Super Sand Splash, the hits of the row of shots (shl20_00 types 4 and 5) */
#define	pl20_at_ssand_last		0x5e	/* Super Sand Splash, the last shot's hit */

/* the character's own work, in tlocal */
typedef struct PL20_LOCAL_W {
	Float	dive_spd_x;	/* Delta Kick: twice the dive's speed */
	Float	dive_spd_y;
	Sint8	catch_flag;	/* Please Help Me: set by its effects, 1 caught, 2 hit, -1 missed */
	Sint8	help_phase;	/* Please Help Me's phase, for its effects */
} PL20_LOCAL_W;

#endif
