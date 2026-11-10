/* Marrow's animation numbers and special moves. */
#ifndef PL16_DEF_H
#define PL16_DEF_H

#define	CH16_WIN_KO0	0
#define	CH16_WIN_KO1	1
#define	CH16_LOSE		2
#define	CH16_TM_DRAW	2

/* pl_sp_type */
#define	pl16_sp_bone			0	/* Bone-merang */
#define	pl16_sp_spine				1	/* Towering Spine */
#define	pl16_sp_ricochet			2	/* Ricochet Slash */
#define	pl16_sp_ride			3	/* Ride and Slash */
#define	pl16_sp_stinger				4	/* Stinger Bones (hyper) */
#define	pl16_sp_burst			5	/* Bone Burst (hyper) */
#define	pl16_sp_snapback			6	/* the snapback */
#define	pl16_sp_chouhatsu		7	/* the taunt */

/* atck_no: the character's own records in the hit table, after the common ATCK_ ones */
#define	pl16_at_upper		0x12	/* The launcher */
#define	pl16_at_throw_p		0x20	/* Ground punch throw's release */
#define	pl16_at_air_throw	0x21	/* Air throw's release */
#define	pl16_at_ride_1		0x22	/* Ride and Slash: the first two slashes on the caught opponent */
#define	pl16_at_ride_2		0x23	/* Ride and Slash: the third slash */
#define	pl16_at_ride_3		0x24	/* Ride and Slash: jumping off, sending the opponent away */
#define	pl16_at_bone		0x30	/* Bone-merang: Marrow and the thrown bone (shl16_00) */
#define	pl16_at_spine		0x31	/* Towering Spine */
#define	pl16_at_burst		0x3e	/* Bone Burst (hyper), ground and air, and the thorns (shl16_01) */
#define	pl16_at_stinger		0x43	/* Stinger Bones (hyper) */
#define	pl16_at_ride		0x47	/* Ride and Slash: the leap that catches, ground and air */
#define	pl16_at_t_kick		0x48	/* Tag-in kick that leaps off the wall */
#define	pl16_at_snapback	0x49	/* Snapback (sp_snapback, pl_snap_flash) */

/* the character's own work, in tlocal */
typedef struct PL16_LOCAL_W {
	Float	scr_x;		/* 00: the screen's x last frame, while Ricochet Slash rises */
	Uint8	shot_flag;	/* 04: a bone (shl16_00) is out */
} PL16_LOCAL_W;

#endif
