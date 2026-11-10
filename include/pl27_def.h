/* Ken's special moves. */
#ifndef PL27_DEF_H
#define PL27_DEF_H

/* pl_sp_type */
#define	pl27_sp_hadou	0	/* Hadouken */
#define	pl27_sp_shouryuu	1	/* Shouryuuken */
#define	pl27_sp_tatsumaki	2	/* Tatsumaki Senpuukyaku */
#define	pl27_sp_jtatsumaki	3	/* Tatsumaki Senpuukyaku in the air */
#define	pl27_sp_jhadou	4	/* Hadouken in the air */
#define	pl27_sp_zenpou	5	/* Zenpou Tenshin (roll) */
#define	pl27_sp_reppa	6	/* Shouryuu Reppa (hyper) */
#define	pl27_sp_shinryuu	7	/* Shinryuuken (hyper) */
#define	pl27_sp_chouhatsu	9	/* Taunt */
#define	pl27_sp_jshouryuu	11	/* Shouryuuken in the air */
#define	pl27_sp_shippu	14	/* Shippuu Jinraikyaku (hyper) */
#define	pl27_sp_snapback	16	/* Snapback */

/* atck_no: the character's own records in the hit table, after the common ATCK_ ones */
#define	pl27_at_fwd_hk_1		0x12	/* Forward + HK command normal, first hit */
#define	pl27_at_fwd_hk_2		0x13	/* Forward + HK command normal, second hit */
#define	pl27_at_jump_lk2		0x14	/* His own jumping LK, used every time */
#define	pl27_at_jump_mk2		0x15	/* His own jumping MK (holding up gives the common 0x10) */
#define	pl27_at_jump_hk2		0x16	/* His own jumping HK (holding up gives the common 0x11) */
#define	pl27_at_upper			0x17	/* Launcher (pl27_upper, crouching, Char_Extra 8) */
#define	pl27_at_stand_lk_2		0x19	/* Second hit of the standing LK and MK */
#define	pl27_at_throw			0x20	/* Throw: the P ground throw and the K air throw both use it */
#define	pl27_at_throw_k_hit		0x21	/* K throw: each hit of the rolls */
#define	pl27_at_throw_k			0x22	/* K throw: letting go after the rolls */
#define	pl27_at_reppa			0x49	/* Shouryuu Reppa hyper */
#define	pl27_at_shinryuu		0x4c	/* Shinryuuken hyper, start (later hits come from the animation's cg_flag1) */
#define	pl27_at_shinryuu_top	0x4e	/* Shinryuuken hit high in the air, the finish */
#define	pl27_at_jshouryuu		0x50	/* Shouryuuken in the air */
#define	pl27_at_t_kick			0x51	/* Partner's wall-leap kick (t_kick) */
#define	pl27_at_tatsumaki_last	0x57	/* Tatsumaki's last turn, only after the spin has hit */
#define	pl27_at_shippu			0x58	/* Shippuu Jinraikyaku hyper */
#define	pl27_at_snapback		0x5b	/* Snapback */

/* the character's own work, in tlocal */
typedef struct PL27_LOCAL_W {
	Uint8	hit_cnt;	/* Shouryuuken: hits left that land */
	Uint8	guard_cnt;	/* Shouryuuken: hits left that are guarded */
	Uint8	shot_flag;	/* the hadouken (shl27_00 type 0) is out */
	Sint8	unk_3;
} PL27_LOCAL_W;

#endif
