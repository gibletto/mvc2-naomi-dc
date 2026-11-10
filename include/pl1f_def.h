/* B.B. Hood's special moves. */
#ifndef PL1F_DEF_H
#define PL1F_DEF_H

/* pl_sp_type */
#define	pl1f_sp_memory		0	/* Beautiful Memory (hyper) */
#define	pl1f_sp_hunting		1	/* Cool Hunting (hyper) */
#define	pl1f_sp_02			2
#define	pl1f_sp_apple		3	/* Apple for You (hyper) */
#define	pl1f_sp_shyness		4	/* Shyness & Strike */
#define	pl1f_sp_cheer		5	/* Cheer & Fire (angled) */
#define	pl1f_sp_missile		6	/* Smile & Missile */
#define	pl1f_sp_happy		7	/* Smile & Missile (low; Happy & Missile) */
#define	pl1f_sp_missile_j		8	/* Smile & Missile (jumping) */
#define	pl1f_sp_tellme		9	/* Tell Me Why */
#define	pl1f_sp_snapback	10	/* Snap Back */
#define	pl1f_sp_chouhatsu	11	/* taunt */
#define	pl1f_sp_cheer_k	12	/* Cheer & Fire (horizontal, also in the air) */

/* atck_no: the character's own records in the hit table, after the common ATCK_ ones */
#define	pl1f_at_fwd_hp			0x12	/* Forward or back + HP standing: a hop attack */
#define	pl1f_at_dash_hp			0x15	/* Forward or back + HP during the dash (nml_atck_dash) */
#define	pl1f_at_stand_hp_2		0x19	/* Standing HP, second hit (cg_type 2) */
#define	pl1f_at_crouch_hp_2		0x1a	/* Crouching HP, second hit (cg_type 2) */
#define	pl1f_at_throw			0x20	/* Her only normal throw (punch, ground or air), set on the opponent */
#define	pl1f_at_missile			0x30	/* Smile & Missile, all three versions (sp_missile, sp_happy, sp_missile_j): her own record as she fires */
#define	pl1f_at_cheer			0x32	/* Cheer & Fire, punch (angled) version */
#define	pl1f_at_shyness			0x34	/* Shyness & Strike */
#define	pl1f_at_hunting			0x38	/* Cool Hunting hyper */
#define	pl1f_at_memory			0x39	/* Beautiful Memory hyper, the first hit that grabs */
#define	pl1f_at_memory_hit_1	0x3a	/* Beautiful Memory, hits of the first round on the held opponent */
#define	pl1f_at_memory_hit_2	0x3b	/* Beautiful Memory, hits of the second round */
#define	pl1f_at_memory_last		0x3c	/* Beautiful Memory, the last hit on landing (her and the opponent) */
#define	pl1f_at_apple			0x3d	/* Apple for You hyper: the dash and the catch (her and the opponent) */
#define	pl1f_at_apple_hit		0x3e	/* Apple for You, the hits on the held opponent (apple_hit_*) */
#define	pl1f_at_apple_scrap		0x40	/* Apple for You, the falling scraps of a broken dish (shl1f_03 type 1) */
#define	pl1f_at_snapback		0x41	/* Snapback (sp_snapback) */
#define	pl1f_at_upper			0x42	/* Launcher */
#define	pl1f_at_t_kick			0x43	/* The partner's jumping kick as she comes in */
#define	pl1f_at_apple_dish		0x47	/* Apple for You, each dish that hits the held opponent */
#define	pl1f_at_cheer_k			0x48	/* Cheer & Fire, kick (horizontal, also in the air) version */
#define	pl1f_at_apple_last		0x4a	/* Apple for You, the last dish */
#define	pl1f_at_cheer_assist	0x4b	/* Cheer & Fire's flame (shl1f_00 spark), with a hit only as a partner's attack (team_type 8) */

/* the character's own work, in tlocal */
typedef struct PL1F_LOCAL_W {
	Float	hold_x;		/* Apple for You: where she stood when she caught the opponent */
	Float	hold_y;		/* Apple for You: and the height she lands back at */
	Uint8	unk_08[0x40 - 0x08];
	Sint32	dish_hit;	/* Apple for You: set by a dish (shl1f_03) as it hits, -1 for the last */
	Sint32	unk_44;
	Uint8	unk_48[0x4c - 0x48];
	Sint32	ef_char_no;	/* Apple for You: the animation for pls1f_03 to show */
	Sint32	spark_end_flag;	/* set when the spark (shl1f_00) is done */
	Sint32	shot_flag;	/* the shot (shl1f_01) is out */
} PL1F_LOCAL_W;

#define PL1F_LW( pl )	( ( PL1F_LOCAL_W * )( pl )->tlocal )

#endif
