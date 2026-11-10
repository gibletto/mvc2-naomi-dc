/* Ryu's animation numbers and special moves. */
#ifndef PL00_DEF_H
#define PL00_DEF_H

/* pl_sp_type */
#define	pl00_sp_hadou			0	/* Hadouken */
#define	pl00_sp_shouryu			1	/* Shoryuken */
#define	pl00_sp_tatsu			2	/* Tatsumaki Senpuukyaku */
#define	pl00_sp_jtatsu			3	/* Tatsumaki Senpuukyaku in the air */
#define	pl00_sp_jhadou			4	/* Hadouken in the air */
#define	pl00_sp_shinkuu			5	/* Shinkuu Hadouken (hyper) */
#define	pl00_sp_shinkuu_tatsu	6	/* Shinkuu Tatsumaki Senpuukyaku (hyper) */
#define	pl00_sp_chouhatsu		8	/* The taunt */
#define	pl00_sp_shin_shouryu	11	/* Shin Shoryuken (level 3 hyper) */
#define	pl00_sp_snapback		13	/* The snapback */

/* atck_no: the character's own records in the hit table, after the common ATCK_ ones */
#define	pl00_at_fwd_hp_1		0x12	/* Stand HP with the stick forward (stepping punch), first hit */
#define	pl00_at_fwd_hp_2		0x13	/* The stepping punch's second hit, on the animation's mark */
#define	pl00_at_fwd_hk			0x14	/* Stand HK with the stick forward (jumping kick) */
#define	pl00_at_jump_mp_1		0x15	/* Jump MP, first hit */
#define	pl00_at_jump_mp_2		0x16	/* Jump MP, second hit */
#define	pl00_at_jup_mk			0x18	/* Jump MK with the stick up */
#define	pl00_at_jump_hk_b		0x19	/* Jump HK without the stick up (up gives ATCK_JUMP_HK) */
#define	pl00_at_stand_mk_2		0x1a	/* Stand MK, second hit */
#define	pl00_at_upper		0x1b	/* The launcher (pl00_upper) */
#define	pl00_at_throw_k			0x20	/* Kick throw */
#define	pl00_at_throw_p			0x21	/* Punch throw (over the shoulder) */
#define	pl00_at_air_throw		0x22	/* Air throw */
#define	pl00_at_shinkuu_tatsu	0x51	/* Shinkuu Tatsumaki Senpuukyaku (hyper), first hit; later hits come from cg_flag1 */
#define	pl00_at_t_kick			0x54	/* Tag-in jumping kick (t_kick) */
#define	pl00_at_shinkuu			0x55	/* Shinkuu Hadouken (hyper), Ryu's own attack */
#define	pl00_at_shinkuu_beam	0x56	/* Shinkuu Hadouken's beam parts (shl00_01) */
#define	pl00_at_vc_hadou		0x57	/* Hadouken fired as a variable counter (shl00_00, team_type 3) */
#define	pl00_at_shin_shouryu	0x59	/* Shin Shoryuken (level 3 hyper: the dash and rush) */
#define	pl00_at_snapback		0x5e	/* Snapback */

/* the character's own work, in tlocal */
typedef struct PL00_LOCAL_W {
	Uint8	hit_cnt;	/* hits left of the Shoryuken when they hit */
	Uint8	guard_cnt;	/* hits left of the Shoryuken when guarded */
	Uint8	shot_flag;	/* 002: a fireball (shl00_00) is out */
	Sint8	renda_cnt;	/* presses left to make the Shinkuu Hadouken last longer */
} PL00_LOCAL_W;

#endif
