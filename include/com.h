/* The command tables (pl_com.c): { time or count, kind, move number, 0, the stick and buttons }.
   The first word is the steps of a motion, the frames of a charge, the presses of a mash or the
   quarter turns of a rotation. The second is a mash's frames between presses, or else these. */
#ifndef COM_H
#define COM_H

#define COM_ATCK_NG	0x0020	/* not while cg_atck_flag & CG_ATCK_ALL: an attack under way */
#define COM_SW_NOW	0x0040	/* the buttons need no time left on the clock (com_sw_ok) */
#define COM_NO_SW	0x0080	/* done on the stick alone */
#define COM_HYPER	0x0100	/* a hyper combo: may cancel a special move (sp_ok_hyper_ck) */
#define COM_CUT		0x0800	/* may cut into a move (sp_ok_ck2) */
#define COM_GROUND	0x1000	/* on the ground only */
#define COM_AIR		0x2000	/* in the air only */
#define COM_FREE	0x4000	/* no check that a move may start */
#define COM_ALL_SW	0x8000	/* every button in the word, together */

#endif
