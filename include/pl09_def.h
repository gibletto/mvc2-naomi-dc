/* Iceman's special moves. */
#ifndef PL09_DEF_H
#define PL09_DEF_H

/* pl_sp_type */
#define	pl09_sp_avalanche	0	/* Ice Avalanche */
#define	pl09_sp_beam	1	/* Ice Beam */
#define	pl09_sp_arctic	2	/* Arctic Attack (hyper) */
#define	pl09_sp_fist	3	/* Ice Fist */
#define	pl09_sp_jbeam	4	/* Ice Beam, in the air */
#define	pl09_sp_chouhatsu	5	/* Taunt */
#define	pl09_sp_snapback	6	/* Snapback */

/* atck_no: the character's own records in the hit table, after the common ATCK_ ones */
#define	pl09_at_jdown_hk	0x12	/* Jumping HK with the stick down */
#define	pl09_at_throw_p		0x20	/* The punch throw, set on the opponent */
#define	pl09_at_throw_k		0x21	/* The kick throw, set on the opponent */
#define	pl09_at_air_throw	0x22	/* The air throw, set on the opponent */
#define	pl09_at_avalanche_l	0x30	/* Ice Avalanche from the ground, light (0x30 + level, its shot shl09_02 too); Arctic Attack's start sets it as well */
#define	pl09_at_t_kick		0x3b	/* The jump-in kick when he is tagged in (pl09_t_kick) */
#define	pl09_at_snapback	0x3c	/* Snapback (also shl09_00 kind 5) */
#define	pl09_at_jdown_hp	0x41	/* Jumping HP with the stick down */
#define	pl09_at_upper		0x42	/* The launcher, crouching */
#define	pl09_at_fist_lp		0x43	/* LP, standing or crouching, with the Ice Fist on (shl09_04) */
#define	pl09_at_fist_mp		0x44	/* M punch with the Ice Fist on */
#define	pl09_at_fist_hp		0x45	/* HP with the Ice Fist on */
#define	pl09_at_fist_jlp	0x46	/* Jumping LP with the Ice Fist on (shl09_00 gives j.LP attack 63) */
#define	pl09_at_fist_jmp	0x47	/* Jumping M punch with the Ice Fist on */
#define	pl09_at_fist_jhp	0x48	/* Jumping HP with the Ice Fist on */

/* the character's own work, in tlocal */
typedef struct PL09_LOCAL_W {
	Uint8	arm_flag;		/* the ice arm (shl09_04) is on */
	Uint8	arm_hit_cnt;	/* hits the ice arm takes before it breaks */
	Uint8	unk_2;
	Uint8	unk_3;
	Sint8	col_timer;		/* frames to the next colour step */
	Uint8	col_step;		/* the colour step, 0 to 6 */
} PL09_LOCAL_W;

#endif
