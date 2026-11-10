/* Cable's special moves. */
#ifndef PL17_DEF_H
#define PL17_DEF_H

/* pl_sp_type */
#define	pl17_sp_hviper			0	/* Hyper Viper Beam */
#define	pl17_sp_timeflip		1	/* Time Flip (hyper) */
#define	pl17_sp_psycharge			2	/* Psy-Charge */
#define	pl17_sp_psimitar			3	/* Psimitar */
#define	pl17_sp_viper			4	/* Viper Beam */
#define	pl17_sp_crackdown		5	/* Crackdown */
#define	pl17_sp_electrap		6	/* Electrap */
#define	pl17_sp_chouhatsu		7	/* the taunt */
#define	pl17_sp_snapback			8	/* the snapback */

/* atck_no: the character's own records in the hit table, after the common ATCK_ ones */
#define	pl17_at_fwd_hk			0x12	/* Forward + HK */
#define	pl17_at_throw_k			0x20	/* Kick throw */
#define	pl17_at_psycharge_2		0x23	/* Psy-Charge: the opponent let go at the end */
#define	pl17_at_psycharge_hit	0x24	/* Psy-Charge: the electric hit on the held opponent (cg_type & 8) */
#define	pl17_at_electrap		0x31	/* Electrap: the grenade (shl17_01_init) */
#define	pl17_at_psycharge		0x32	/* Psy-Charge: the grab */
#define	pl17_at_crackdown_l		0x33	/* Crackdown, light */
#define	pl17_at_t_kick			0x34	/* Tag-in kick that leaps off the wall */
#define	pl17_at_upper			0x35	/* The launcher, 3HP */
#define	pl17_at_crackdown_blast	0x36	/* Crackdown: the blast at the end of the charge (shl17_04) */
#define	pl17_at_electrap_burst	0x37	/* Electrap: Cable's throw and the grenade's shock (shl17_01_burst) */
#define	pl17_at_psimitar_l_2	0x3b	/* Psimitar, light: the hit cued by the animation */
#define	pl17_at_psimitar_shot_l	0x3c	/* Psimitar, light: the shot fired up (shl17_02) */
#define	pl17_at_psimitar_h_2	0x3d	/* Psimitar, heavy: the hit cued by the animation */
#define	pl17_at_psimitar_shot_h	0x3e	/* Psimitar, heavy: the shot fired up (shl17_02) */
#define	pl17_at_snapback		0x3f	/* Snapback (sp_snapback); also shl17_06's flash kind 3, which nothing sets */
#define	pl17_at_timeflip		0x40	/* Time Flip: Cable and the catching shot (shl17_05) */
#define	pl17_at_timeflip_shot	0x42	/* Time Flip: gunshot on the held opponent (shl17_06 kind 4) */
#define	pl17_at_timeflip_shot_2	0x43	/* Time Flip: the other gunshot (shl17_06 kind 5) */
#define	pl17_at_psimitar_l		0x49	/* Psimitar, light: the rising leap */
#define	pl17_at_psimitar_h		0x4a	/* Psimitar, heavy: the rising leap */
#define	pl17_at_crackdown_h		0x4b	/* Crackdown, heavy */

/* the character's own work, in tlocal; the arcade's code reads each word as an array element */
typedef struct PL17_LOCAL_W {
	Sint32	beam_lv[1];			/* the beam's level, 0 to 3 */
	Sint32	beam_time[1];		/* frames until the level drops */
	Sint32	start_cnt[1];		/* frames START was held in the taunt; it loops while above 0 */
	Uint32	sw_hold[1];			/* buttons still held since the move began */
	Sint32	sw_off_flag[1];		/* the buttons were let go as the grenade left: it won't burst on release */
	Sint32	grenade_flag[1];	/* a grenade (shl17_01) is out */
	Sint32	hp_btn[1];			/* the heavy punch was pressed again */
	Sint32	hp_shot[1];			/* shots fired by the heavy punch */
	Sint32	timeflip_wait_timer[1];	/* frames before Time Flip may start again */
	Sint32	beam_hit[1];		/* frames the Viper Beam stops hitting after a hit; in the Hyper Viper Beam, a hit is due */
	Sint32	timeflip_fly_flag[1];	/* Time Flip goes on while its shot (shl17_05) flies */
	Sint32	timeflip_shot_flag[1];	/* Time Flip's shot (shl17_05) is out */
	Sint32	timeflip_catch_flag[1];	/* Time Flip's shot caught the opponent */
	Sint32	beam_hit_cnt[1];	/* the Hyper Viper Beam's hits, counted to 5; the fifth is the stronger */
} PL17_LOCAL_W;

#define PL17_LW( pl )	( ( PL17_LOCAL_W * )( pl )->tlocal )

#endif
