/* Silver Samurai's special moves. */
#ifndef PL2F_DEF_H
#define PL2F_DEF_H

/* pl_sp_type */
#define	pl2f_sp_shuriken	0	/* Shuriken */
#define	pl2f_sp_jshuriken	1	/* Shuriken (air) */
#define	pl2f_sp_raimei	2	/* Raimeiken (hyper) */
#define	pl2f_sp_rapid	3	/* Hyakuretsutou */
#define	pl2f_sp_bushin		4	/* Bushin (hyper): a chain of after-images; its command is never checked */
#define	pl2f_sp_hyouga	5	/* Hyougaken (hyper, ice element) */
#define	pl2f_sp_dash	6	/* Running Hyakuretsutou */
#define	pl2f_sp_hrapid		7	/* Hyper Hyakuretsutou (team hyper) */
#define	pl2f_sp_hshuriken	8	/* Cho-Shuriken (hyper) */
#define	pl2f_sp_09		9
#define	pl2f_sp_elem_k	10	/* Toki-Hyo (LK, ice) and Toki-En (HK, fire) (hyper) */
#define	pl2f_sp_elem_p	11	/* Toki-Rai (lightning element, hyper) */
#define	pl2f_sp_12		12
#define	pl2f_sp_chouhatsu	13	/* Taunt */
#define	pl2f_sp_snapback	14	/* Snapback */
#define	pl2f_sp_crouch_hp		15	/* crouching heavy punch (gamma assist) */
#define	pl2f_sp_homura	16	/* Homuraken (hyper, fire element) */

/* atck_no: the character's own records in the hit table, after the common ATCK_ ones */
#define	pl2f_at_stand_mp_2	0x1a	/* Second hit of the standing medium punch */
#define	pl2f_at_stand_hp_2	0x1b	/* Standing heavy punch follow-up lunge (nml_move_stand_hp) */
#define	pl2f_at_crouch_lp_2	0x1c	/* Crouching light punch repeated (nml_move_crouch_lp) */
#define	pl2f_at_throw_k		0x20	/* Ground kick throw */
#define	pl2f_at_throw_p		0x21	/* Ground punch throw: a leap, slammed on landing */
#define	pl2f_at_air_throw	0x22	/* Air kick throw */
#define	pl2f_at_shuriken	0x30	/* Shuriken (sp_shuriken) */
#define	pl2f_at_jshuriken	0x32	/* Shuriken in the air (sp_jshuriken) */
#define	pl2f_at_hshuriken	0x34	/* Cho-Shuriken (hyper, sp_hshuriken) */
#define	pl2f_at_rapid		0x35	/* Hyakuretsutou hits (sp_rapid; also the running sp_dash and the team hyper sp_hrapid) */
#define	pl2f_at_raimei		0x36	/* Raimeiken (hyper, sp_raimei) */
#define	pl2f_at_snapback	0x3a	/* Snapback (sp_snapback) */
#define	pl2f_at_t_kick		0x3b	/* The partner's jump-in attack (pl2f_t_kick) */
#define	pl2f_at_hrapid_end	0x3c	/* Hyper Hyakuretsutou's last dash (hrapid_move) */
#define	pl2f_at_hyouga		0x40	/* Hyougaken (hyper, ice; him and shl2f_04) */
#define	pl2f_at_homura		0x41	/* Homuraken (hyper, fire; him and shl2f_05) */

/* the character's own work, in tlocal */
typedef struct PL2F_LOCAL_W {
	Sint32	flash_step;		/* the flash's stage (FL_STEP) */
	Uint8	unk_04[0x14 - 0x04];
	Sint32	elem_flag;		/* an element is on */
	Sint32	elem_timer;		/* its time left (TM_ELEM) */
	Sint32	elem;			/* the element */
	Sint32	unk_20;
	Uint8	unk_24[0x28 - 0x24];
	Sint32	unk_28;
	Uint8	unk_2c[0x34 - 0x2c];
	Sint32	sound_timer;		/* frames to the next sound (TM_SOUND) */
	Uint8	unk_38[0x40 - 0x38];
	Sint32	flash_timer;		/* frames to the flash's next colour (FL_TIME) */
	Sint32	flash_col;		/* the flash's colour (FL_COL) */
	Sint32	unk_48;
	Sint32	shuriken_timer;		/* no new shuriken until it runs out (TM_SHURIKEN) */
	Sint32	dash_timer;		/* frames the dash slash can follow the rapid slashes */
	Sint32	elem_lv;		/* the element's level (ELEM_LV) */
	Uint8	unk_58[0x7c - 0x58];
	Sint32	nml_move;		/* the normal attack's move */
} PL2F_LOCAL_W;

#define PL2F_LW( pl )	( ( PL2F_LOCAL_W * )( pl )->tlocal )

#endif
