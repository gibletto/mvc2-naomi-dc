/* Gambit's special moves. */
#ifndef PL28_DEF_H
#define PL28_DEF_H

/* pl_sp_type */
#define	pl28_sp_card	1	/* Kinetic Card */
#define	pl28_sp_trick	2	/* Trick Card */
#define	pl28_sp_slash	3	/* Cajun Slash */
#define	pl28_sp_strike	4	/* Cajun Strike */
#define	pl28_sp_royal	5	/* Royal Flush (hyper) */
#define	pl28_sp_chouhatsu	6	/* Taunt */
#define	pl28_sp_escape	7	/* Cajun Escape */
#define	pl28_sp_slash2	8	/* Cajun Slash, the two-button form */
#define	pl28_sp_jcard	9	/* Kinetic Card (air) */
#define	pl28_sp_explosion	10	/* Cajun Explosion (hyper) */
#define	pl28_sp_explosion_f	14	/* Cajun Explosion, jumping forward (hyper) */
#define	pl28_sp_crouch_hp		15	/* crouching heavy punch launcher: the gamma assist and the gamma variable counter */
#define	pl28_sp_snapback	16	/* Snapback */

/* atck_no: the character's own records in the hit table, after the common ATCK_ ones */
#define	pl28_at_stand_mp_2		0x19	/* Second hit of the standing MP */
#define	pl28_at_throw_p			0x20	/* P throw */
#define	pl28_at_throw_k			0x21	/* K throw: the final throw out of the pin */
#define	pl28_at_air_throw		0x22	/* K air throw */
#define	pl28_at_throw_k_hit		0x23	/* K throw: each extra hit while he holds the opponent down */
#define	pl28_at_card			0x30	/* Kinetic Card, light (player and card); also the player's record for Trick Card */
#define	pl28_at_card_vc			0x31	/* Kinetic Card as a variable counter (team_type 3) */
#define	pl28_at_card_h			0x32	/* Kinetic Card, heavy (card kinds 5 and 7) */
#define	pl28_at_t_kick			0x33	/* Partner's jump-in attack (pl28_t_kick); also the unused sp_t_kick hop */
#define	pl28_at_slash_vc		0x34	/* Cajun Slash as a variable counter (team_type 3) */
#define	pl28_at_escape			0x3d	/* Cajun Escape dive */
#define	pl28_at_strike			0x3e	/* Cajun Strike */
#define	pl28_at_royal_card8		0x3f	/* Royal Flush: every eighth card of the stream (sub_type 0) */
#define	pl28_at_royal			0x40	/* Royal Flush hyper: his record and the other streamed cards */
#define	pl28_at_royal_shower	0x47	/* Royal Flush: the shower of cards (shl28_00 kind 9) */
#define	pl28_at_explosion		0x48	/* Cajun Explosion hyper (player and shl28_01) */
#define	pl28_at_snapback		0x4a	/* Snapback (sp_snapback) */
#define	pl28_at_upper			0x4b	/* Launcher (pl28_upper, Char_Extra 5) */
#define	pl28_at_crouch_hp_vc	0x4c	/* Crouching HP (sp_crouch_hp) as a variable counter; otherwise it uses the common 8 */

#endif
