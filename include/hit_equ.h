/* The names Capcom's tables gave the numbers of an attack's row (data/hit_dtXX.c); the rest are in player.h. */
#ifndef HIT_EQU_H
#define HIT_EQU_H

/* damage_type: the reaction, with NO_VEC, NO_ZU and FLY_SCREEN */
#define ZU	0	/* pushed back along the ground */
#define BU_O	1	/* knocked off the feet */
#define AS	4	/* the feet swept away */

/* atck_kind: how it must be guarded, with A_HI, A_LO and A_GUARD_NG */
#define A_NOM	0	/* standing or crouching */

/* damage_kind: the strength; K_LS to K_LL are the same with K_LOW */
#define K_S	0x10
#define K_M	0x20
#define K_L	0x30
#define K_LS	0x90
#define K_LM	0xa0
#define K_LL	0xb0

/* hit_mark: a plain hit's mark, small to large */
#define MARK_N1	0
#define MARK_N2	1
#define MARK_N3	2

/* score */
#define SC100	4
#define SC200	8
#define SC300	12

/* hit_se: a punch's sound and a kick's, light to heavy */
#define SD_NOR_S	5
#define SD_NOR_M	6
#define SD_NOR_L	7
#define SD_KIK_S	9
#define SD_KIK_M	10
#define SD_KIK_L	11

/* cpu_answer_no: the computer's answers that guard */
#define YK_GD030	0
#define YK_GD060	1

#endif
