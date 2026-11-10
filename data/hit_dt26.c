/* M. Bison (character 26): the attacks (HIT_DATA, 0x1c bytes).
   What each hit does. The character's module sets the attack's number (atck_no) with the move,
   and the row is read when the attack box meets a body. After a row is its number, in
   hexadecimal, and the program's name for it:
   ATCK_ for the normals every character has (player.h), pl26_at_ for its own (pl26_def.h).
   The values' names are Capcom's own for these tables (hit_equ.h) and the program's (player.h).

     dmg     the damage
     type    the reaction: ZU pushed back, BU_O knocked off the feet, AS swept,
             or the number of another in pl_dmg.c's dm_tbl (here 2 dm_buttobi_ng, 10 dm_away,
             12 dm_snap).
             NO_VEC, NO_ZU and FLY_SCREEN change the knock-back
     kind    how it must be guarded (A_NOM either way, A_HI standing, A_LO crouching, A_GUARD_NG
             not at all), and A_SPECIAL or A_HYPER for those moves
     elem    the strength (K_S, K_M, K_L; K_LS, K_LM and K_LL strike low) and the element
             (K_ELEM_MASK)
     piyo    stun points
     stop    the hit stop for both sides
     sand    frames before an air recovery
     sleep   the down time
     vec     the knock-back's direction, in 32 steps
     pow     the knock-back's strength
     range   the attack's reach across (not read)
     mark    the hit mark (MARK_N1 to MARK_N3 are the plain ones), 0x80 and above for none
     score   SC100 to SC300 (not read)
     se      the hit's sound (SD_NOR_ a punch's, SD_KIK_ a kick's)
     shc     the hyper combo gauge it gives
     jug     what it adds to an armour's hit count
     box, box2  boxes of hit_26 that the computer measures the attack's reach by
     flag    extra rules (NO_CHIP to AERIAL_FINISH)
     guard   the guard's frames, 0 to go by the strength
     catch   frames the victim can't be thrown after
     abyss   for the Abyss's armour, in place of jug
     cnt     the hits it counts for, 0 for one
     answer  what a computer opponent does when it sees the attack coming: a record of its own
             yokXX.c (YK_GD030 and YK_GD060 guard)
     13, 15, 1a  bytes the program is not seen to read */
#include "player.h"
#include "hit_equ.h"

const HIT_DATA hit_dt_26[] = {
	/* dmg                 type              kind      elem  piyo  stop  sand  sleep   vec  pow  range     mark  score        se  shc  jug  box  box2                     flag  13  guard  15  catch  abyss  cnt    answer        1a */
	{   5,                  ZU,            A_NOM,      K_S,    4,   10,   10,     0, 0x1a,   4,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  16,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 00: ATCK_STAND_LP */
	{   8,                  ZU,            A_NOM,  K_M | 6,    2,   10,   16,     0, 0x1a,   6,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40,  17,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 01: ATCK_STAND_MP */
	{  13,                  ZU,            A_NOM,      K_L,    1,   10,   24,     0, 0x1a,   8,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  50,  18,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 02: ATCK_STAND_HP */
	{   5,                  ZU,            A_NOM,      K_S,    4,   10,   10,     0, 0x1a,   5,  0x60, MARK_N1, SC100, SD_KIK_S,   0,  20,  22,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 03: ATCK_STAND_LK */
	{   8,                  ZU,            A_NOM,      K_S,    2,   10,   16,     0, 0x1a,   7,  0x60, MARK_N2, SC200, SD_KIK_M,   0,  40,  23,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 04: ATCK_STAND_MK */
	{  10,                  ZU,            A_NOM,     K_LL,    1,   10,   24,     0, 0x1c,   9,  0x80, MARK_N3, SC300, SD_KIK_L,   4,  40,  72,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 05: ATCK_STAND_HK */
	{   5,         ZU | NO_VEC,            A_NOM,     K_LS,    4,   10,   10,     0, 0x1a,   4,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  19,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 06: ATCK_CROUCH_LP */
	{   8,                  ZU,            A_NOM, K_LM | 6,    2,   10,   16,     0, 0x1a,   6,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40,  20,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 07: ATCK_CROUCH_MP */
	{  14,       BU_O | NO_VEC,            A_NOM,  K_L | 6,    1,   10,   24,     0, 0x00,  11,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  50,  21,    0, NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 08: ATCK_CROUCH_HP */
	{   5,         ZU | NO_VEC,             A_LO,     K_LS,    4,   10,   10,     0, 0x18,   5,  0x70, MARK_N1, SC100, SD_KIK_S,   0,  20,  25,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 09: ATCK_CROUCH_LK */
	{   8,                  ZU,             A_LO,     K_LM,    2,   10,   16,     0, 0x1a,   7,  0x40, MARK_N2, SC200, SD_KIK_M,   0,  40,  26,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0a: ATCK_CROUCH_MK */
	{  14,          AS | NO_ZU,             A_LO,     K_LL,    1,   10,   24,     9, 0x18,   8,  0x70, MARK_N3, SC300, SD_KIK_L,   0,  90,  27,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0b: ATCK_CROUCH_HK */
	{   5,         ZU | NO_VEC,             A_HI,      K_S,    1,   10,   10,     0, 0x00,   7,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  28,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0c: ATCK_JUMP_LP */
	{   8,         ZU | NO_VEC,             A_HI,  K_M | 6,    1,   10,   16,     0, 0x00,   6,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40,  29,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0d: ATCK_JUMP_MP */
	{  14,         ZU | NO_VEC,             A_HI,  K_L | 6,    1,   10,   24,     0, 0x16,   8,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  60,  30,    0,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0e: ATCK_JUMP_HP */
	{   5,         ZU | NO_VEC,             A_HI,      K_S,    1,   10,   10,     0, 0x00,   7,  0x60, MARK_N1, SC100, SD_KIK_S,   0,  20,  31,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0f: ATCK_JUMP_LK */
	{   8,         ZU | NO_VEC,             A_HI,      K_M,    1,   10,   16,     0, 0x1d,   0,  0x60, MARK_N2, SC200, SD_KIK_M,   0,  40,  32,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 10: ATCK_JUMP_MK */
	{  14,         ZU | NO_VEC,             A_HI,      K_L,    1,   10,   24,     0, 0x16,   8,  0x80, MARK_N3, SC300, SD_KIK_L,   0,  60,  33,    0,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 11: ATCK_JUMP_HK */
	{ 0 },	/* 12 */
	{ 0 },	/* 13 */
	{ 0 },	/* 14 */
	{ 0 },	/* 15 */
	{ 0 },	/* 16 */
	{ 0 },	/* 17 */
	{ 0 },	/* 18 */
	{   4,                  ZU,            A_NOM,      K_L,    1,   10,   24,     0, 0x1c,   9,  0x80, MARK_N3, SC300, SD_KIK_L,   4,  40,  11,   24,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 19: pl26_at_stand_hk_2, Second hit of the standing HK */
	{ 0 },	/* 1a */
	{ 0 },	/* 1b */
	{ 0 },	/* 1c */
	{ 0 },	/* 1d */
	{ 0 },	/* 1e */
	{ 0 },	/* 1f */
	{  16,                   2,     A_NOM | 0x40,        0,    0,    4,    0,     0, 0x07,   3,  0x80, MARK_N1, SC300, SD_NOR_S,   0,   0,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 20: pl26_at_throw, Ground throw */
	{  16,                   2,     A_NOM | 0x40,        0,    0,   16,    0,     0, 0x15,  10,  0x80, MARK_N1, SC300, SD_NOR_S,   0,   0,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 21: pl26_at_air_throw, Air throw */
	{ 0 },	/* 22 */
	{ 0 },	/* 23 */
	{ 0 },	/* 24 */
	{ 0 },	/* 25 */
	{   2, ZU | NO_VEC | NO_ZU,        A_SPECIAL,  K_S | 6,    0,    4,   10,     0, 0x00,   2,  0x60,       3, SC100, SD_NOR_M,   0,  20,  61,    0,                 NO_CHIP,  1,     0,  0,     0,     0,   4, YK_GD060, { 0, 0 } },	/* 26: pl26_at_field_hit, Psycho Field: each hit of the turning ring (shl26_00 type 1) */
	{ 0 },	/* 27 */
	{ 0 },	/* 28 */
	{  10,          2 | NO_VEC,        A_SPECIAL,      K_S,    0,   10,   20,     0, 0x1a,   0,  0x80,       3, SC300, SD_KIK_M,   5,  90,  63,    0, NO_WEIGHT | AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 29 */
	{ 0 },	/* 2a */
	{  10,          2 | NO_VEC,        A_SPECIAL,      K_S,    0,   10,   20,     0, 0x1a,   0,  0x80,       3, SC300, SD_KIK_L,   5,  90,  63,    0, NO_WEIGHT | AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 2b */
	{ 0 },	/* 2c */
	{ 0 },	/* 2d */
	{ 0 },	/* 2e */
	{ 0 },	/* 2f */
	{  10,          ZU | NO_ZU,        A_SPECIAL,      K_S,    0,   10,   10,     0, 0x18,   0,  0x60,       3, SC100, SD_KIK_S,   5,  90,  35,    0,                       0,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 30: pl26_at_knee_l_1, Double Knee Press first kick (also the air version, knee_air) */
	{  10,           2 | NO_ZU,        A_SPECIAL,      K_S,    0,   10,   20,     0, 0x1a,   4,  0x80,       3, SC300, SD_KIK_M,   5,  90,  36,    0, NO_WEIGHT | AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 31 */
	{ 0 },	/* 32 */
	{ 0 },	/* 33 */
	{  12,          ZU | NO_ZU,        A_SPECIAL,      K_S,    0,   10,   10,     0, 0x18,   0,  0x60,       3, SC100, SD_KIK_M,   5,  90,  35,    0,                       0,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 34 */
	{  12,           2 | NO_ZU,        A_SPECIAL,      K_S,    0,   10,   20,     0, 0x1a,   4,  0x80,       3, SC300, SD_KIK_L,   5,  90,  36,    0, NO_WEIGHT | AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 35 */
	{  15,          ZU | NO_ZU,        A_SPECIAL,      K_L,    0,    4,   10,     0, 0x18,   2,  0x60,       3, SC100, SD_NOR_L,   0,  50,  34,    0,                       0,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 36 */
	{ 0 },	/* 37 */
	{  15,          ZU | NO_ZU,        A_SPECIAL,      K_L,    0,    4,   10,     0, 0x18,   2,  0x60,       3, SC100, SD_NOR_L,   0,  50,  34,    0,                       0,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 38 */
	{  12,         ZU | NO_VEC,        A_SPECIAL,      K_S,    0,   10,   20,     0, 0x1a,   0,  0x80,       3, SC300, SD_KIK_L,   0,  90,  37,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        5, { 0, 0 } },	/* 39: pl26_at_head, Head Press stomp (pl26_main follows up when it hits) */
	{  16,          2 | NO_VEC, A_HI | A_SPECIAL,  K_S | 6,    0,   10,   20,     0, 0x1a,   8,  0x80,       3, SC300, SD_NOR_L,   0,  90,  38,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 3a: pl26_at_head_dive, Head Press: the dive after the bounce */
	{  16,          2 | NO_VEC, A_HI | A_SPECIAL,  K_S | 6,    0,   10,   20,     0, 0x1a,   8,  0x80,       3, SC300, SD_NOR_L,   0,  90,  38,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        5, { 0, 0 } },	/* 3b: pl26_at_skull, Somersault Skull Diver dive */
	{   5, ZU | NO_VEC | NO_ZU,        A_SPECIAL,  K_S | 6,    0,    4,   10,     0, 0x00,   2,  0x60,       3, SC100, SD_NOR_M,   0,  20,  61,    0,                       0,  1,     0,  0,     0,     0,   4,        4, { 0, 0 } },	/* 3c: pl26_at_field, Psycho Field (sp_field, and shl26_00 type 1 parts) */
	{   4,         ZU | NO_VEC,          A_HYPER,      K_M,    0,    6,   10,     0, 0x18,   0,  0x60,       3, SC100, SD_KIK_M,   5,  90,  75,   76,                 NO_CHIP,  1,     0, 37,     0,     0,   8,        3, { 0, 0 } },	/* 3d: pl26_at_nightmare_kick, Knee Press Nightmare hyper: the jumping kicks */
	{  30,          2 | NO_VEC,          A_HYPER,      K_M,    0,    6,   20,     0, 0x1a,   9,  0x80,       3, SC300, SD_KIK_L,   5,  90,  77,    0,               NO_WEIGHT,  1,     0, 37,     0,     0,   8,        3, { 0, 0 } },	/* 3e: pl26_at_nightmare_crush, Knee Press Nightmare's closing psycho crusher, which hits again and again (also shl26_01's slide) */
	{   3,         ZU | NO_VEC,          A_HYPER,      K_M,    0,    6,   10,     0, 0x18,   0,  0x60,       3, SC100, SD_KIK_M,   5,  90,  75,   76,                       0,  1,     0, 37,     0,     0,   8,        3, { 0, 0 } },	/* 3f: pl26_at_nightmare_side, the two shl26_01 works beside Knee Press Nightmare, as he flies and falls */
	{  10,         10 | NO_VEC,        A_SPECIAL,      K_L,    0,   10,   10,     0, 0x1f,  10,  0x60,       3, SC200,       12,   0,  90,  33,    0, NO_CHIP | NO_WEIGHT | AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 40: pl26_at_t_kick, Partner's jump-in attack (t_kick) */
	{   6,          2 | NO_VEC,          A_HYPER,  K_L | 6,    0,    8,   10,     0, 0x1b,   2,  0x60,       3, SC100, SD_NOR_L,   0,  90,  53,    0,               NO_WEIGHT,  1,     0, 36,     0,     0,   8,        3, { 0, 0 } },	/* 41: pl26_at_crusher, Psycho Crusher hyper, ground and air versions */
	{  15,         ZU | NO_VEC,        A_SPECIAL,      K_S,    2,   10,   10,     0, 0x18,   0,  0x60,       3, SC100, SD_KIK_M,   0,  90,  35,    0,                       0,  0,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 42: pl26_at_knee_vc_1, Double Knee Press first kick as a variable counter (team_type 3) */
	{  15,          2 | NO_VEC,        A_SPECIAL,      K_S,    0,   10,   20,     0, 0x1a,   8,  0x80,       3, SC300, SD_KIK_L,   0,  90,  36,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 43: pl26_at_knee_vc_2, Double Knee Press second kick as a variable counter */
	{  13,       BU_O | NO_VEC,            A_NOM,      K_S,    0,   10,   24,     0, 0x00,  11,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  90,  18,    0, NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 44 */
	{  10,         12 | NO_VEC,        A_SPECIAL,      K_L,    0,    4,   24,     0, 0x1c,  12,  0x80,       3, SC300,       12,  64,  90,  11,   24,               NO_WEIGHT,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 45: pl26_at_snapback, Snapback (sp_snapback) */
	{  14,       BU_O | NO_VEC,        A_SPECIAL,  K_L | 6,    0,   10,   24,     0, 0x00,  11,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  40, 144,    0, NO_CHIP | NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 46: pl26_at_upper, Launcher (pl26_upper, Char_Extra 5) */
	{   3,          ZU | NO_ZU,          A_HYPER,  K_L | 6,    0,    4,   20,     0, 0x1e,   0,  0x80,       3, SC300, SD_NOR_L,   0,  90,  34,   35,               NO_WEIGHT,  1,     0, 30,     0,     0,   6,        3, { 0, 0 } },	/* 47: pl26_at_explosion, Psycho Explosion hyper: his start, the flame's first hit and its odd repeat hits */
	{   3,          ZU | NO_ZU,          A_HYPER,  K_L | 6,    0,    4,   20,     0, 0x1e,   0,  0x80,       3, SC300, SD_NOR_L,   0,  90,  34,   35,     NO_CHIP | NO_WEIGHT,  1,     0, 30,     0,     0,   6,        3, { 0, 0 } },	/* 48: pl26_at_explosion_2, Psycho Explosion flame: the hit that catches and its even repeat hits */
	{  30,           2 | NO_ZU,          A_HYPER,  K_L | 6,    0,    8,   20,     0, 0x1d,  13,  0x80,       3, SC300, SD_NOR_L,   0,  90,  34,   35,               NO_WEIGHT,  1,     0, 30,     0,     0,   6,        3, { 0, 0 } },	/* 49: pl26_at_explosion_3, Psycho Explosion flame: the last hit */
};
