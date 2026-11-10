/* Akuma's special moves. */
#ifndef PL1E_DEF_H
#define PL1E_DEF_H

/* pl_sp_type */
#define	pl1e_sp_hadou			0	/* Gou Hadouken */
#define	pl1e_sp_shouryuu		1	/* Gou Shouryuuken */
#define	pl1e_sp_tatsumaki		2	/* Tatsumaki Zankuukyaku */
#define	pl1e_sp_jtatsumaki		3	/* Tatsumaki Zankuukyaku in the air */
#define	pl1e_sp_zankuu			4	/* Zankuu Hadouken */
#define	pl1e_sp_tenma			5	/* Tenma Gou Zankuu (air hyper) */
#define	pl1e_sp_ashura			6	/* Ashura Senkuu forward */
#define	pl1e_sp_ashura_b		7	/* Ashura Senkuu back */
#define	pl1e_sp_shungoku		8	/* Shun Goku Satsu (hyper) */
#define	pl1e_sp_gou_shouryuu	11	/* Messatsu Gou Shouryuu (hyper) */
#define	pl1e_sp_gou_hadou		13	/* Messatsu Gou Hadou (hyper) */
#define	pl1e_sp_kuujin			14	/* Tenma Kuujinkyaku */
#define	pl1e_sp_chouhatsu		15	/* taunt */
#define	pl1e_sp_snapback		16	/* Snap Back */
#define	pl1e_sp_gou_rasen		17	/* Messatsu Gou Rasen (hyper) */

/* atck_no: the character's own records in the hit table, after the common ATCK_ ones */
#define	pl1e_at_fwd_hp_1		0x12	/* Forward + HP overhead chop (step in), first hit */
#define	pl1e_at_fwd_hp_2		0x13	/* Forward + HP overhead chop, second hit on the animation's mark */
#define	pl1e_at_fwd_hk			0x14	/* Forward + HK hop kick */
#define	pl1e_at_kuujin			0x15	/* Tenma Kuujinkyaku (air dive kick, sp_kuujin) */
#define	pl1e_at_jump_lk			0x16	/* Jump LK (his own animation for every jump) */
#define	pl1e_at_jump_mk			0x17	/* Jump MK without up held (up+MK uses the common 0x10) */
#define	pl1e_at_jump_hk			0x18	/* Jump HK without up held (up+HK uses the common 0x11) */
#define	pl1e_at_stand_mk_2		0x19	/* Standing MK, second hit on the animation's mark */
#define	pl1e_at_upper			0x1a	/* Launcher (pl1e_upper) */
#define	pl1e_at_throw_k			0x20	/* Ground kick throw, set on the opponent */
#define	pl1e_at_throw_p			0x21	/* Ground punch throw, set on the opponent */
#define	pl1e_at_shungoku		0x22	/* Shun Goku Satsu (Raging Demon): the opponent's record when the flurry of hits ends */
#define	pl1e_at_guruma			0x23	/* Jigoku Guruma command throw (HCB+HK, catch_com_throw) */
#define	pl1e_at_air_throw		0x24	/* Air throw: lands with the opponent, then throws */
#define	pl1e_at_gou_shouryuu	0x4f	/* Messatsu Gou Shouryuu hyper, first record (later hits come from cg_flag1) */
#define	pl1e_at_t_kick			0x52	/* The partner's jumping kick as he comes in (t_kick) */
#define	pl1e_at_gou_rasen		0x53	/* Messatsu Gou Rasen hyper, its repeated rising hits */
#define	pl1e_at_gou_rasen_last	0x54	/* Messatsu Gou Rasen, the last and strongest hit */
#define	pl1e_at_gou_hadou		0x55	/* Messatsu Gou Hadou hyper beam (Akuma and shl1e_01's beam) */
#define	pl1e_at_tenma			0x57	/* Tenma Gou Zankuu (air fireball hyper) */
#define	pl1e_at_snapback		0x59	/* Snapback (sp_snapback) */

/* the character's own work, in tlocal */
typedef struct PL1E_LOCAL_W {
	Uint8	hit_cnt;		/* Gou Shouryuuken: hits left that land */
	Uint8	guard_cnt;	/* Gou Shouryuuken: hits left that are guarded */
	Uint8	shot_flag;	/* the hadouken (shl1e_00 type 0) is out */
	Sint8	renda_cnt;	/* Messatsu Gou Hadou: presses left that hold the beam longer */
} PL1E_LOCAL_W;

#define	PL1E_LW( pl )	( ( PL1E_LOCAL_W * )( pl )->tlocal )

#endif
