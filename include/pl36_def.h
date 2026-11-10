/* Thanos's animation numbers and special moves. */
#ifndef PL36_DEF_H
#define PL36_DEF_H

#define	CH36_WIN_KO0	0
#define	CH36_WIN_KO1	1
#define	CH36_LOSE		2

/* pl_sp_type */
#define	pl36_sp_titan	0	/* Titan Crush */
#define	pl36_sp_sphere	1	/* Death Sphere */
#define	pl36_sp_reality	2	/* Gauntlet Reality (hyper) */
#define	pl36_sp_space	3	/* Gauntlet Space (hyper) */
#define	pl36_sp_power	4	/* Power Gem (hyper) */
#define	pl36_sp_05		5
#define	pl36_sp_soul	6	/* Soul Gem (hyper) */
#define	pl36_sp_07		7
#define	pl36_sp_chouhatsu	8	/* Taunt */
#define	pl36_sp_upper	9	/* Uppercut (assist) */
#define	pl36_sp_snapback	10	/* Snapback */

/* atck_no: the character's own records in the hit table, after the common ATCK_ ones */
#define	pl36_at_jdown_hk		0x12	/* Jumping HK with down, high enough: the two-hit stomp (jump_kick_stomp) */
#define	pl36_at_back_hk			0x13	/* Back + standing HK */
#define	pl36_at_stand_mp_2		0x19	/* Second hit of standing MP (two_hits[1]; nml_move_2hit) */
#define	pl36_at_throw_p			0x20	/* Punch throw */
#define	pl36_at_throw_k			0x21	/* Kick throw */
#define	pl36_at_soul_release	0x22	/* Soul Gem's hand lets go of the drained opponent (shl36_04_hold) */
#define	pl36_at_sphere			0x33	/* Death Sphere: Thanos and the bubble (shl36_03) */
#define	pl36_at_reality			0x3d	/* Gauntlet Reality: Thanos and the blasts' burst (shl36_01) */
#define	pl36_at_space			0x3e	/* Gauntlet Space (space_init) */
#define	pl36_at_power			0x42	/* Power Gem: Thanos and each hit of the ball (shl36_00) */
#define	pl36_at_soul			0x44	/* Soul Gem: Thanos and the grabbing hand (shl36_04) */
#define	pl36_at_snapback		0x45	/* Snapback */
#define	pl36_at_t_kick			0x46	/* Partner jump-in attack */
#define	pl36_at_upper			0x47	/* Launcher (pl36_upper) */
#define	pl36_at_upper_assist	0x48	/* sp_upper, the uppercut assist (crouching, Char_Extra 5) */

/* the character's own work, in tlocal */
typedef struct PL36_LOCAL_W {
	Uint8	unk_00[0x08];
	Sint32	titan_timer;		/* frames before another air Titan Crush */
	Uint8	unk_0c[0x14 - 0x0c];
	Sint32	shot_timer[2];	/* [0] kept at 4 while a grab (shl36_03, _04) or pls36_02 is out */
	Sint32	hyper_timer;
	Sint32	unk_20;
	Uint8	unk_24[0x7c - 0x24];
	Sint32	two_hit_flag;	/* the attack hits a second time */
} PL36_LOCAL_W;

#define	PL36_LW( pl )	( ( PL36_LOCAL_W * )( pl )->tlocal )

#endif
