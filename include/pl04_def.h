/* Anakaris's animation numbers and special moves. */
#ifndef PL04_DEF_H
#define PL04_DEF_H

#define	CH04_WIN_KO0	0
#define	CH04_WIN_KO1	1
#define	CH04_WIN_TIME	4
#define	CH04_LOSE		5
#define	CH04_TM_DRAW	6

/* pl_sp_type */
#define	pl04_sp_coffin			0	/* Coffin Dance */
#define	pl04_sp_judgement			1	/* Royal Judgement */
#define	pl04_sp_cobra			2	/* Cobra Blow */
#define	pl04_sp_mummy			3	/* Mummy Drop */
#define	pl04_sp_magic		4	/* Pharaoh Magic (hyper) */
#define	pl04_sp_cobras			5	/* Pharaoh Cobra Throw (hyper) */
#define	pl04_sp_illusion			6	/* Pharaoh Illusion (hyper) */
#define	pl04_sp_coffins			7	/* Coffin Fall (hyper) */
#define	pl04_sp_snapback		8	/* The snapback */
#define	pl04_sp_chouhatsu		9	/* The taunt */

/* atck_no: the character's own records in the hit table, after the common ATCK_ ones */
#define	pl04_at_back_lk			0x12	/* Stand LK with the stick back */
#define	pl04_at_jdown_k			0x14	/* Jump kick with the stick down (drop) */
#define	pl04_at_jdiag_k			0x15	/* Jump kick with the stick down-forward or down-back */
#define	pl04_at_sjump_lk		0x16	/* Super jump LK */
#define	pl04_at_sjump_mk		0x17	/* Super jump MK */
#define	pl04_at_sjump_hk		0x18	/* Super jump HK */
#define	pl04_at_jfwd_k			0x19	/* Jump kick with the stick forward only (the dive) */
#define	pl04_at_coffin			0x30	/* Coffin Dance on the ground (Anakaris and the coffin shot) */
#define	pl04_at_jcoffin			0x31	/* Coffin Dance in the air; also the coffin shot as a variable counter */
#define	pl04_at_throw_hit		0x38	/* Throw's hits on the ground (catch_hit) */
#define	pl04_at_air_throw_hit	0x39	/* Air throw's hits (catch_hit) */
#define	pl04_at_throw			0x3a	/* The throw that ends it on the ground (catch_end) */
#define	pl04_at_air_throw		0x3b	/* The throw that ends it in the air (catch_end) */
#define	pl04_at_vc_cobra		0x3c	/* Cobra Blow as a variable counter (otherwise 52+level) */
#define	pl04_at_magic			0x40	/* Pharaoh Magic (hyper), ground and air */
#define	pl04_at_magic_coffin	0x43	/* The coffin dropped after Pharaoh Magic's fourth shot (shl04_00 type 2) */
#define	pl04_at_cobras			0x44	/* Pharaoh Cobra Throw (on Anakaris); its rows' heads and parts one frame in four */
#define	pl04_at_cobras_2		0x45	/* The rows' heads and parts on the other frames (shl04_03) */
#define	pl04_at_illusion		0x4d	/* Pharaoh Illusion (hyper) */
#define	pl04_at_coffins			0x50	/* Coffin Fall (hyper), ground and air, and its coffins (shl04_00 type 1) */
#define	pl04_at_t_kick			0x54	/* Tag-in kick off the wall (t_kick) */
#define	pl04_at_snapback		0x55	/* Snapback (sp_snapback) */
#define	pl04_at_upper		0x56	/* The launcher (pl04_upper) */

#endif
