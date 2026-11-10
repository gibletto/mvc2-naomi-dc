/* Juggernaut's special moves. */
#ifndef PL29_DEF_H
#define PL29_DEF_H

/* pl_sp_type */
#define	pl29_sp_quake	0	/* Earthquake Punch */
#define	pl29_sp_headcrush	1	/* Juggernaut Headcrush (hyper) */
#define	pl29_sp_powerup	2	/* Cytorrak Power-up */
#define	pl29_sp_punch	3	/* Juggernaut Punch */
#define	pl29_sp_chouhatsu	4	/* Taunt */
#define	pl29_sp_05		5
#define	pl29_sp_06		6
#define	pl29_sp_press	7	/* Juggernaut Body Press */
#define	pl29_sp_08		8
#define	pl29_sp_snapback	9	/* Snapback */

/* atck_no: the character's own records in the hit table, after the common ATCK_ ones */
#define	pl29_at_fwd_hp			0x12	/* Forward + HP command normal (Char_Extra 3) */
#define	pl29_at_crouch_mp_2		0x19	/* Second hit of the crouching MP (shakes the screen) */
#define	pl29_at_throw_k			0x20	/* K throw */
#define	pl29_at_throw_p_hit		0x21	/* P throw: the hit while holding */
#define	pl29_at_throw_p			0x22	/* P throw: the release */
#define	pl29_at_air_throw		0x23	/* P air throw */
#define	pl29_at_headcrush		0x33	/* Juggernaut Headcrush hyper: the repeating charge */
#define	pl29_at_headcrush_last	0x34	/* Juggernaut Headcrush: the finishing hit */
#define	pl29_at_quake			0x38	/* Earthquake Punch */
#define	pl29_at_t_kick			0x3e	/* Partner's jump-in attack (pl29_t_kick) */
#define	pl29_at_punch_vc		0x3f	/* Juggernaut Punch as a variable counter (team_type 3) */
#define	pl29_at_snapback		0x40	/* Snapback (sp_snapback) */
#define	pl29_at_upper			0x41	/* Launcher (pl29_upper) */

#endif
