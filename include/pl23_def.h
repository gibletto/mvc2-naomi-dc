/* Dan's special moves. */
#ifndef PL23_DEF_H
#define PL23_DEF_H

#define	CH23_WIN_TIME	2
#define	CH23_LOSE		3

/* pl_sp_type */
#define	pl23_sp_gadou	0	/* Gadouken */
#define	pl23_sp_kouryuu	1	/* Kouryuuken */
#define	pl23_sp_danku	2	/* Dankuukyaku */
#define	pl23_sp_shinkuu	3	/* Shinkuu Gadouken (hyper) */
#define	pl23_sp_rekka	4	/* Kouryuu Rekka (hyper) */
#define	pl23_sp_buraiken	5	/* Hisshou Buraiken (hyper) */
#define	pl23_sp_chouhatsu	8	/* Taunt */
#define	pl23_sp_zenten	9	/* Zenten Chouhatsu (rolling taunt) */
#define	pl23_sp_kouten	10	/* Kouten Chouhatsu (back roll taunt) */
#define	pl23_sp_densetsu	11	/* Chouhatsu Densetsu (hyper taunt) */
#define	pl23_sp_jburaiken	12	/* Hisshou Buraiken in the air (hyper, unused) */
#define	pl23_sp_jdanku	13	/* Dankuukyaku in the air */
#define	pl23_sp_sign	14	/* Premium Sign */
#define	pl23_sp_otoko	16	/* Otoko Michi (level 3 hyper) */
#define	pl23_sp_snapback	17	/* Snapback */

/* atck_no: the character's own records in the hit table, after the common ATCK_ ones */
#define	pl23_at_throw_p			0x20	/* Ground punch throw */
#define	pl23_at_air_throw		0x21	/* Air punch throw */
#define	pl23_at_throw_k			0x22	/* Ground kick throw */
#define	pl23_at_otoko			0x23	/* Otoko Michi: the opponent's record in the blast */
#define	pl23_at_otoko_self		0x24	/* Otoko Michi: Dan's own record as he knocks himself out (dm_ko_self) */
#define	pl23_at_gadou_l			0x30	/* Gadouken, light: his punch, and every light shot (shl23_00 type 0) */
#define	pl23_at_gadou_m			0x31	/* Gadouken, medium: his punch, and the shot of medium and heavy */
#define	pl23_at_gadou_h			0x32	/* Gadouken, heavy punch, also the variable counter's */
#define	pl23_at_kouryuu_l		0x33	/* Kouryuuken, light */
#define	pl23_at_kouryuu_h		0x37	/* Kouryuuken, medium and heavy */
#define	pl23_at_shinkuu			0x3f	/* Shinkuu Gadouken hyper: his record and the many-hit shot (shl23_00 type 1) */
#define	pl23_at_rekka			0x40	/* Kouryuu Rekka hyper, the rush */
#define	pl23_at_rekka_2			0x41	/* Kouryuu Rekka, the later hit (cg_type) */
#define	pl23_at_buraiken		0x42	/* Hisshou Buraiken hyper, first record */
#define	pl23_at_buraiken_last	0x4b	/* Hisshou Buraiken, the closing leap kick */
#define	pl23_at_t_kick			0x4c	/* The partner's jumping kick as he comes in */
#define	pl23_at_jburaiken		0x4d	/* Air Hisshou Buraiken (unused hyper), first record */
#define	pl23_at_jburaiken_last	0x4e	/* Air Hisshou Buraiken, the last hit */
#define	pl23_at_sign_hit		0x4f	/* Premium Sign, the throw (both arms) and the light thrown piece (shl23_00 type 2) */
#define	pl23_at_sign_piece		0x51	/* Premium Sign, the thrown piece of medium and heavy (shl23_00 type 2) */
#define	pl23_at_sign			0x52	/* Premium Sign, first record (sign_init) */
#define	pl23_at_snapback		0x54	/* Snapback (sp_snapback) */
#define	pl23_at_upper			0x55	/* Crouching HK launcher */
#define	pl23_at_kouryuu_counter	0x57	/* Kouryuuken as a variable counter (team_type 3, not light) */

#endif
