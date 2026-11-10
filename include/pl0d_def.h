/* Hulk's special moves. */
#ifndef PL0D_DEF_H
#define PL0D_DEF_H

/* pl_sp_type */
#define	pl0d_sp_charge	0	/* Gamma Charge */
#define	pl0d_sp_01		1
#define	pl0d_sp_slam	2	/* Gamma Slam */
#define	pl0d_sp_crush	3	/* Gamma Crush (hyper) */
#define	pl0d_sp_tornado	4	/* Gamma Tornado */
#define	pl0d_sp_ucharge	5	/* Gamma Charge, upward */
#define	pl0d_sp_wave	6	/* Gamma Wave (hyper) */
#define	pl0d_sp_07		7
#define	pl0d_sp_08		8
#define	pl0d_sp_chouhatsu	9	/* Taunt */
#define	pl0d_sp_quake	10	/* Gamma Quake (hyper) */
#define	pl0d_sp_snapback	11	/* Snapback */

/* atck_no: the character's own records in the hit table, after the common ATCK_ ones */
#define	pl0d_at_crouch_hp_2	0x1a	/* Crouching HP's second hit */
#define	pl0d_at_upper_p		0x1b	/* The launcher, crouching punch: first hit */
#define	pl0d_at_upper_p_2	0x1c	/* The launcher, crouching punch: second hit */
#define	pl0d_at_upper_k		0x1d	/* The launcher, standing kick */
#define	pl0d_at_throw_p		0x20	/* The punch throw, set on him and the opponent */
#define	pl0d_at_air_throw	0x21	/* The air throw, set on him and the opponent */
#define	pl0d_at_t_kick		0x3c	/* The jump-in kick when he is tagged in (t_kick) */
#define	pl0d_at_crush		0x3d	/* Gamma Crush (hyper): the start */
#define	pl0d_at_crush_rise	0x3e	/* Gamma Crush: the rise, after its timer */
#define	pl0d_at_tornado		0x42	/* Gamma Tornado: the grabbing spin (the throw after it is 0x22 + level) */
#define	pl0d_at_wave		0x4f	/* Gamma Wave (hyper): his move and the wave's rock columns (shl0d_01) */
#define	pl0d_at_slam		0x51	/* Gamma Slam: his move and the wave's rock columns (shl0d_01) */
#define	pl0d_at_quake_rock	0x52	/* Gamma Quake: the rocks (shl0d_02) */
#define	pl0d_at_quake		0x54	/* Gamma Quake (hyper): his move */
#define	pl0d_at_crush_fall	0x57	/* Gamma Crush: the hit that rides on him as he comes down (shl0d_00) */
#define	pl0d_at_snapback	0x58	/* Snapback */

/* the character's own work, in tlocal */
typedef struct PL0D_LOCAL_W {
	Uint8	crush_bound_flag;	/* set when Gamma Crush bounces off the ground */
	Uint8	crush_land_flag;		/* set when Gamma Crush lands */
	Sint8	crush_time;			/* counts down while Gamma Crush runs */
} PL0D_LOCAL_W;

#endif
