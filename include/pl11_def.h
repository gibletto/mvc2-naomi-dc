/* Jill's special moves. */
#ifndef PL11_DEF_H
#define PL11_DEF_H

/* pl_sp_type */
#define	pl11_sp_cerberus			0	/* Cerberus Escape */
#define	pl11_sp_crow			1	/* Crow Escape */
#define	pl11_sp_zombie			2	/* Zombie Escape */
#define	pl11_sp_return			3	/* Return Fire */
#define	pl11_sp_stars			4	/* Charging Stars */
#define	pl11_sp_grenade			5	/* Grenade Launcher */
#define	pl11_sp_hstars			6	/* Hyper Charging Stars (hyper) */
#define	pl11_sp_tyrant			7	/* Tyrant Escape (hyper) */
#define	pl11_sp_rocket			8	/* Rocket Launcher (hyper) */
#define	pl11_sp_heal				9	/* the heal: vital back for the partner on the point (assist) */
#define	pl11_sp_chouhatsu		10	/* the taunt */
#define	pl11_sp_snapback			11	/* the snapback */

/* atck_no: the character's own records in the hit table, after the common ATCK_ ones */
#define	pl11_at_counter_hit			0x22	/* Each hit on the opponent held by sp_catch (the counter's throw) */
#define	pl11_at_counter_release		0x23	/* The counter's throw lets the opponent go (sp_catch_release) */
#define	pl11_at_counter_end			0x24	/* The counter's throw's last shot (sp_catch_end) */
#define	pl11_at_cerberus			0x30	/* Cerberus Escape: Jill and the dog (cerberus_init, shl11_00 type 0) */
#define	pl11_at_crow				0x31	/* Crow Escape: Jill and the crows (crow_init, shl11_00 type 1) */
#define	pl11_at_zombie				0x32	/* Zombie Escape: the thing that catches and carries (zombie_init, shl11_01 type 0) */
#define	pl11_at_t_kick				0x33	/* The jump-in attack (pl11_t_kick) */
#define	pl11_at_snapback			0x34	/* Snapback (sp_snapback) */
#define	pl11_at_upper				0x35	/* The launcher (pl11_upper, crouching) */
#define	pl11_at_grenade				0x36	/* Grenade Launcher's shot (grenade_init, shl11_02_a_shot) */
#define	pl11_at_grenade_blast		0x37	/* Grenade Launcher's shot at the end of its flight (shl11_02_a_fly) */
#define	pl11_at_stars_air			0x3a	/* Charging Stars in the air: start and first rush hits (stars_j_init, stars_j_rush) */
#define	pl11_at_stars_air_end		0x3b	/* Air Charging Stars' finishing hit as the rush ends (stars_j_rush) */
#define	pl11_at_hstars				0x3c	/* Hyper Charging Stars on the ground: its rush hits (hstars_start, hstars_rush) */
#define	pl11_at_hstars_end			0x3d	/* Ground Hyper Charging Stars' finishing hit (hstars_rush) */
#define	pl11_at_hstars_air			0x3e	/* Hyper Charging Stars in the air: its rush hits (hstars_j_start, hstars_j_rush) */
#define	pl11_at_hstars_air_end		0x3f	/* Air Hyper Charging Stars' finishing hit (hstars_j_rush) */
#define	pl11_at_tyrant_shot			0x40	/* Tyrant Escape's follow-up command: Jill and the shot (tyrant_com_ck, shl11_02 type 2) */
#define	pl11_at_tyrant				0x42	/* Tyrant Escape (hyper): start and the reaching hand (tyrant_init, shl11_03_b_wait) */
#define	pl11_at_tyrant_hit			0x43	/* Tyrant Escape: the hits while the hand holds (shl11_03_b_near_hit, _far_hit and _last_hit) */
#define	pl11_at_tyrant_hit_last		0x44	/* Tyrant Escape: the last hit of the hold (shl11_03_b_last_hit) */
#define	pl11_at_tyrant_finish		0x45	/* Tyrant Escape's follow-up attack (shl11_03_b_shot_wait, after tyrant_throw_flag) */
#define	pl11_at_zombie_h			0x46	/* Zombie Escape, the stronger version (zombie_init by atck_level, shl11_01 type 2) */
#define	pl11_at_counter				0x48	/* The counter's hit after taking a blow (return_flash) */
#define	pl11_at_rocket				0x4b	/* Rocket Launcher (hyper) (rocket_init) */
#define	pl11_at_stars_air_2			0x4d	/* Air Charging Stars' later rush hits (stars_j_rush, stars_hit_cnt > 1) */
#define	pl11_at_zombie_fall			0x4e	/* Zombie Escape's piece landing from its fall (shl11_01_a_fall_bound) */
#define	pl11_at_crow_vc				0x4f	/* Crow Escape as a variable counter (team_type 3) */
#define	pl11_at_grenade_vc			0x53	/* Grenade Launcher's shot as a variable counter (team_type 3) */
#define	pl11_at_grenade_blast_vc	0x54	/* Grenade Launcher's end-of-flight hit as a variable counter (team_type 3) */

/* the character's own work, in tlocal */
typedef struct PL11_LOCAL_W {
	Uint8	cerberus_shot_flag;		/* Cerberus Escape's dog (shl11_00 type 0) is out */
	Uint8	crow_shot_flag;		/* Crow Escape's crow (shl11_00 type 1) is out */
	Uint8	zombie_shot_flag;		/* Zombie Escape's zombie (shl11_01) is out */
	Uint8	cerberus_near_flag;		/* the dog has come near her */
	Uint8	crow_near_flag;		/* the crow has come near her */
	Uint8	charge_flag;		/* a charge is being held */
	Uint8	charge_timer;		/* frames it has been held */
	Uint8	stars_hit_cnt;		/* the rush's hits */
	Uint8	grenade_end_flag;		/* the grenade (shl11_02) is done */
	Uint8	tyrant_flag;			/* Tyrant Escape's hand (shl11_03) is out */
	Sint8	tyrant_reach_st;		/* the hand's reach: above 0 it caught, below 0 it missed */
	Sint8	tyrant_hold_st;		/* the hold: 1, then 2 when it is done, below 0 when it fails */
	Uint8	tyrant_throw_flag;	/* the follow-up command was given */
	Uint8	zombie_land_flag;		/* the zombie (shl11_01) has landed */
	Sint8	charge_cnt;			/* how far the charge has gone */
} PL11_LOCAL_W;

#endif
