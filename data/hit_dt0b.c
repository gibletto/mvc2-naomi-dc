/* Captain America (character 0b): the attacks (HIT_DATA, 0x1c bytes).
   What each hit does. The character's module sets the attack's number (atck_no) with the move,
   and the row is read when the attack box meets a body. After a row is its number, in
   hexadecimal, and the program's name for it:
   ATCK_ for the normals every character has (player.h), pl0b_at_ for its own (pl0b_def.h).
   The values' names are Capcom's own for these tables (hit_equ.h) and the program's (player.h).

     dmg     the damage
     type    the reaction: ZU pushed back, BU_O knocked off the feet, AS swept,
             or the number of another in pl_dmg.c's dm_tbl (here 2 dm_buttobi_ng, 5 dm_hold,
             10 dm_away, 12 dm_snap).
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
     box, box2  boxes of hit_0b that the computer measures the attack's reach by
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

const HIT_DATA hit_dt_0b[] = {
	/* dmg                   type              kind              elem  piyo  stop  sand  sleep   vec  pow  range     mark  score  se  shc  jug  box  box2                     flag  13  guard  15  catch  abyss  cnt    answer        1a */
	{   5,           ZU | NO_VEC,            A_NOM,              K_S,    4,   10,   10,     0, 0x18,   6,  0x70, MARK_N1, SC100,  1,   0,  20,   7,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 00: ATCK_STAND_LP */
	{   6,           ZU | NO_VEC,            A_NOM,              K_M,    2,   11,   16,     0, 0x00,  10,  0x80, MARK_N2, SC200,  2,   0,  40,   8,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 01: ATCK_STAND_MP */
	{  12,                    ZU,            A_NOM,              K_L,    1,   12,   24,     0, 0x1e,  10,  0x80, MARK_N3, SC300,  3,   0,  50,   9,   25,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 02: ATCK_STAND_HP */
	{   5,           ZU | NO_VEC,            A_NOM,             K_LS,    4,   10,   10,     0, 0x18,   8,  0x60, MARK_N1, SC100,  1,   0,  20,  10,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 03: ATCK_STAND_LK */
	{   8,                    ZU,            A_NOM,              K_M,    2,   11,   16,     0, 0x1a,   8,  0x60, MARK_N2, SC200,  2,   0,  40,  11,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 04: ATCK_STAND_MK */
	{  14,                    ZU,            A_NOM,              K_L,    1,   12,   24,     0, 0x19,  10,  0x80, MARK_N3, SC300,  3,   0,  50,  12,  151,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 05: ATCK_STAND_HK */
	{   3,           ZU | NO_VEC,            A_NOM,             K_LS,    4,   10,   10,     0, 0x18,   6,  0x70, MARK_N1, SC100,  1,   0,  20,  13,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 06: ATCK_CROUCH_LP */
	{   6,                    ZU,            A_NOM,             K_LM,    2,   11,   16,     0, 0x18,   8,  0x80, MARK_N2, SC200,  2,   0,  40,  14,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 07: ATCK_CROUCH_MP */
	{  12,         BU_O | NO_VEC,            A_NOM,             K_LL,    1,   10,   24,     0, 0x00,  11,  0x80, MARK_N3, SC300,  3,   0,  50,  15,    0, NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 08: ATCK_CROUCH_HP */
	{   5,           ZU | NO_VEC,             A_LO,             K_LS,    4,   10,   10,     0, 0x18,   8,  0x70, MARK_N1, SC100,  1,   0,  20,  16,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 09: ATCK_CROUCH_LK */
	{   8,                    ZU,             A_LO,             K_LM,    2,   11,   16,     0, 0x18,   8,  0x40, MARK_N2, SC200,  2,   0,  40,  17,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0a: ATCK_CROUCH_MK */
	{  14,                    AS,             A_LO,             K_LL,    1,   12,   24,     8, 0x18,   8,  0x70, MARK_N3, SC300,  3,   0,  90,  18,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0b: ATCK_CROUCH_HK */
	{   4,           ZU | NO_VEC,             A_HI,              K_S,    1,   10,   10,     0, 0x00,   5,  0x70, MARK_N1, SC100,  1,   0,  20,  19,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0c: ATCK_JUMP_LP */
	{   6,           ZU | NO_VEC,             A_HI,              K_M,    1,   10,   16,     0, 0x00,   5,  0x80, MARK_N2, SC200,  2,   0,  40,  20,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0d: ATCK_JUMP_MP */
	{  12,           ZU | NO_VEC,             A_HI,              K_M,    1,   12,   32,     0, 0x15,  12,  0x80, MARK_N3, SC300,  3,   0,  60,  21,    0,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0e: ATCK_JUMP_HP */
	{   4,           ZU | NO_VEC,             A_HI,              K_S,    1,   10,   10,     0, 0x00,   4,  0x60, MARK_N1, SC100,  1,   0,  20,  22,  150,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0f: ATCK_JUMP_LK */
	{   7,           ZU | NO_VEC,             A_HI,              K_M,    1,   10,   16,     0, 0x1d,   0,  0x60, MARK_N2, SC200,  2,   0,  40,  23,  145,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 10: ATCK_JUMP_MK */
	{  13,           ZU | NO_VEC,             A_HI,              K_M,    1,   12,   32,     0, 0x15,  12,  0x80, MARK_N3, SC300,  3,   0,  60,  38,    0,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 11: ATCK_JUMP_HK */
	{   8,                    ZU,            A_NOM,              K_M,    2,   11,   16,     0, 0x00,  10,  0x80, MARK_N2, SC300,  2,   0,  40,   8,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 12: pl0b_at_shield_mp, Standing M punch with the shield in hand */
	{  16,                  BU_O,            A_NOM,              K_L,    1,   10,   24,     0, 0x1a,  12,  0x80, MARK_N3, SC200,  3,   0,  50,  29,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 13: pl0b_at_shield_hp, Standing HP with the shield in hand */
	{   5,           ZU | NO_VEC,            A_NOM,             K_LS,    4,   10,   10,     0, 0x18,   6,  0x70, MARK_N1, SC100,  1,   0,  20,  13,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 14: pl0b_at_shield_clp, Crouching LP with the shield in hand */
	{   8,                    ZU,            A_NOM,             K_LM,    2,   11,   16,     0, 0x18,   8,  0x80, MARK_N2, SC200,  2,   0,  40,  31,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 15: pl0b_at_shield_cmp, Crouching M punch with the shield in hand */
	{  14,         BU_O | NO_VEC,            A_NOM,             K_LL,    1,   10,   24,     0, 0x00,  11,  0x80, MARK_N3, SC300,  3,   0,  50,  32,    0, NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 16: pl0b_at_shield_chp, Crouching HP with the shield in hand */
	{   7,           ZU | NO_VEC,             A_HI,              K_M,    1,   10,   16,     0, 0x00,   5,  0x80, MARK_N2, SC200,  2,   0,  40,  47,   48,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 17: pl0b_at_shield_jmp, Jumping M punch with the shield in hand (never used: overwritten by ATCK_JUMP_MP) */
	{  13,           ZU | NO_VEC,             A_HI,              K_M,    1,   12,   32,     0, 0x16,  12,  0x80, MARK_N3, SC300,  3,   0,  60,  36,  148,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 18: pl0b_at_shield_jhp, Jumping HP with the shield in hand */
	{   8,                    ZU,            A_NOM,              K_M,    2,   11,   16,     0, 0x1a,   8,  0x60, MARK_N2, SC200,  2,   0,  40,  30,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 19: pl0b_at_stand_mk_2, Standing M kick's second kick, with the button pressed again */
	{   5,           ZU | NO_VEC,            A_NOM, K_S | K_BACK_MAX,    4,   10,   10,     0, 0x18,  11,  0x60, MARK_N1, SC100,  1,   0,  20,   7,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 1a */
	{   5,           ZU | NO_VEC,            A_NOM,              K_S,    4,   10,   10,     0, 0x18,  11,  0x70, MARK_N1, SC100,  1,   0,  20,  10,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 1b */
	{   5,           ZU | NO_VEC,            A_NOM,             K_LS,    4,   10,   10,     0, 0x18,  11,  0x70, MARK_N1, SC100,  1,   0,  20,  13,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 1c */
	{   5,           ZU | NO_VEC,             A_LO,             K_LS,    4,   10,   10,     0, 0x18,  11,  0x70, MARK_N1, SC100,  1,   0,  20,  16,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 1d */
	{  13,           ZU | NO_VEC,             A_HI,              K_M,    1,   12,   32,     0, 0x15,  12,  0x80, MARK_N3, SC300,  3,   0,  60,  39,    0,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 1e: pl0b_at_jdown_hk, Jumping HK with the stick down */
	{  13,           ZU | NO_VEC,             A_HI,              K_M,    1,   12,   32,     0, 0x15,  12,  0x80, MARK_N3, SC300,  3,   0,  60,  24,    0,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 1f: pl0b_at_jup_hk, Jumping HK with the stick up */
	{  16,                  BU_O,     A_NOM | 0x40,                0,    4,    4,   16,    15, 0x1c,   4,  0x80, MARK_N1, SC300, 76,   0,   0,   0,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 20: pl0b_at_throw_p, the punch throw, set on the opponent */
	{  16,                  BU_O,     A_NOM | 0x40,                0,    4,   16,   32,    15, 0x1f,   4,  0x80, MARK_N1, SC300, 76,   0,   0,   0,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 21: pl0b_at_throw_k, the kick throw, set on the opponent */
	{  16,                     2,     A_NOM | 0x40,                0,    4,    8,    0,    15, 0x10,   4,  0x80, MARK_N1, SC300,  1,   0,   0,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 22: pl0b_at_air_throw, the air throw, set on the opponent */
	{   0,            2 | NO_VEC,   A_HYPER | 0x40,                0,    0,    8,    0,    15, 0x00,  12,  0x80,       3, SC300,  1,   0,   0,   6,   17,       NO_WEIGHT | NO_KO,  1,     0, 16,    10,     0,   0, YK_GD030, { 0, 0 } },	/* 23: pl0b_at_justice_hits, Final Justice (hyper): the blows on the caught opponent */
	{   0,            2 | NO_VEC,   A_HYPER | 0x40,                0,    0,    8,    0,    15, 0x10,   6,  0x80,       3, SC300,  1,   0,   0,   6,   17,    NO_WEIGHT | NO_SCALE,  1,     0, 16,    10,     0,   0, YK_GD030, { 0, 0 } },	/* 24: pl0b_at_justice_toss, Final Justice: he lets go and the opponent goes up */
	{  35,            2 | NO_VEC,   A_HYPER | 0x40,                0,    0,    8,    0,    15, 0x10,   6,  0x80,       3, SC300,  1,   0,   0,   6,   17,    NO_WEIGHT | NO_SCALE,  1,     0, 16,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 25: pl0b_at_justice_slam, Final Justice: the opponent hits the ground */
	{ 0 },	/* 26 */
	{ 0 },	/* 27 */
	{  32,         BU_O | NO_VEC, A_SPECIAL | 0x80,              K_L,    0,   11,    0,    15, 0x1e,  15,  0x80,       3, SC300,  4,   0,  90,  54,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 28 */
	{  30,         BU_O | NO_VEC, A_SPECIAL | 0x80,              K_L,    0,   10,    0,    15, 0x1e,  15,  0x80,       3, SC300,  4,   0,  90,  55,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 29 */
	{  26,         BU_O | NO_VEC, A_SPECIAL | 0x80,              K_L,    0,   10,    0,    15, 0x1d,  13,  0x80,       3, SC300,  4,   0,  90,  56,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 2a */
	{  24,         BU_O | NO_VEC, A_SPECIAL | 0x80,              K_L,    0,   10,    0,    15, 0x1d,  12,  0x80,       3, SC300,  4,   0,  90,  56,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 2b */
	{  28,                  BU_O, A_SPECIAL | 0x80,              K_L,    0,   10,   16,     0, 0x1e,  15,  0x80,       3, SC300,  4,   0,  90, 162,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 2c */
	{  26,                  BU_O, A_SPECIAL | 0x80,              K_L,    0,   10,   16,     0, 0x1e,  15,  0x80,       3, SC300,  4,   1,  90, 164,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 2d */
	{  10,                  BU_O, A_SPECIAL | 0x80,              K_L,    0,   10,   16,     0, 0x1d,  13,  0x80,       3, SC300,  4,   1,  90, 166,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 2e */
	{   8,                  BU_O, A_SPECIAL | 0x80,              K_L,    0,   10,   16,     0, 0x1d,  12,  0x80,       3, SC300,  4,   1,  90, 166,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 2f */
	{  18,         BU_O | NO_VEC, A_SPECIAL | 0x80,              K_L,    0,   10,    0,    15, 0x1e,  12,  0x80,       3, SC300,  4,   0,  90,  54,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 30: pl0b_at_stars, Stars & Stripes */
	{  16,         BU_O | NO_VEC, A_SPECIAL | 0x80,              K_L,    0,   10,    0,    15, 0x1e,  12,  0x80,       3, SC300,  4,   0,  90,  55,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 31 */
	{  12,         BU_O | NO_VEC, A_SPECIAL | 0x80,              K_L,    0,   10,    0,    15, 0x1d,  11,  0x80,       3, SC300,  4,   0,  90,  56,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 32 */
	{  10,         BU_O | NO_VEC, A_SPECIAL | 0x80,              K_L,    0,   10,    0,    15, 0x1d,  10,  0x80,       3, SC300,  4,   0,  90,  56,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 33 */
	{  22,         BU_O | NO_VEC, A_SPECIAL | 0x80,              K_L,    0,   11,    0,    15, 0x1e,  15,  0x80,       3, SC300,  4,   0,  90,  54,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 34 */
	{  20,         BU_O | NO_VEC, A_SPECIAL | 0x80,              K_L,    0,   10,    0,    15, 0x1e,  15,  0x80,       3, SC300,  4,   0,  90,  55,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 35 */
	{  16,         BU_O | NO_VEC, A_SPECIAL | 0x80,              K_L,    0,   10,    0,    15, 0x1d,  13,  0x80,       3, SC300,  4,   0,  90,  56,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 36 */
	{  14,         BU_O | NO_VEC, A_SPECIAL | 0x80,              K_L,    0,   10,    0,    15, 0x1d,  12,  0x80,       3, SC300,  4,   0,  90,  56,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 37 */
	{  14,                  BU_O, A_SPECIAL | 0x80,              K_L,    0,   10,   16,     0, 0x1e,  12,  0x80,       3, SC300,  4,   0,  90, 162,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 38 */
	{  12,                  BU_O, A_SPECIAL | 0x80,              K_L,    0,   10,   16,     0, 0x1e,  12,  0x80,       3, SC300,  4,   0,  90, 164,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 39 */
	{   6,                  BU_O, A_SPECIAL | 0x80,              K_L,    0,   10,   16,     0, 0x1d,  11,  0x80,       3, SC300,  4,   0,  90, 166,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 3a */
	{   4,                  BU_O, A_SPECIAL | 0x80,              K_L,    0,   10,   16,     0, 0x1d,  10,  0x80,       3, SC300,  4,   0,  90, 166,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 3b */
	{  18,                  BU_O, A_SPECIAL | 0x80,              K_L,    0,   10,   16,     0, 0x1e,  15,  0x80,       3, SC300,  4,   0,  90, 162,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 3c */
	{  16,                  BU_O, A_SPECIAL | 0x80,              K_L,    0,   10,   16,     0, 0x1e,  15,  0x80,       3, SC300,  4,   1,  90, 164,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 3d */
	{  10,                  BU_O, A_SPECIAL | 0x80,              K_L,    0,   10,   16,     0, 0x1d,  13,  0x80,       3, SC300,  4,   1,  90, 166,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 3e */
	{   8,                  BU_O, A_SPECIAL | 0x80,              K_L,    0,   10,   16,     0, 0x1d,  12,  0x80,       3, SC300,  4,   1,  90, 166,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 3f */
	{   3, BU_O | NO_VEC | NO_ZU, A_SPECIAL | 0x80,              K_L,    0,    8,   15,     0, 0x1a,   8,  0x80,       3, SC300,  3,   0,  90,  57,    0,                       0,  1,     0,  0,     0,    30,   0,        6, { 0, 0 } },	/* 40 */
	{   7, BU_O | NO_VEC | NO_ZU, A_SPECIAL | 0x80,              K_L,    0,    8,   15,     0, 0x1a,   8,  0x80,       3, SC300,  3,   0,  90,  57,    0,                       0,  1,     0,  0,     0,    30,   0,        6, { 0, 0 } },	/* 41: pl0b_at_charge_h, Charging Star, heavy: the start (0x40 + level); the variable counter uses it too */
	{  18, BU_O | NO_VEC | NO_ZU, A_SPECIAL | 0x80,              K_S,    0,   12,   32,     0, 0x1e,  11,  0x80,       3, SC300,  4,   0,  90,  57,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 42 */
	{  22, BU_O | NO_VEC | NO_ZU, A_SPECIAL | 0x80,              K_S,    0,   12,   44,     0, 0x1e,  13,  0x80,       3, SC300,  4,   0,  90,  57,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 43: pl0b_at_charge_dash_h, Charging Star, heavy: the dash (0x42 + level); the variable counter uses it too */
	{  10,                    ZU, A_SPECIAL | 0x80,              K_L,    0,   10,   16,     0, 0x1a,  10,  0x80,       3, SC300,  3,   0,  50,  46,    0,                       0,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 44: pl0b_at_slash_l, Shield Slash, light (0x44 + level; 0x48 + level in the air); the variable counter uses it too */
	{  10,                    ZU, A_SPECIAL | 0x80,              K_L,    0,   10,   16,     0, 0x1a,  10,  0x80,       3, SC300,  3,   0,  50,   0,    0,                       0,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 45 */
	{  14,            ZU | NO_ZU, A_SPECIAL | 0x80,              K_L,    0,    8,   16,     0, 0x1a,  10,  0x80,       3, SC300,  3,   0,  50,  41,    0,                       0,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 46 */
	{  16,            ZU | NO_ZU, A_SPECIAL | 0x80,              K_L,    0,    8,   16,     0, 0x1a,  10,  0x80,       3, SC300,  3,   0,  50,  42,    0,                       0,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 47 */
	{   8,                    ZU, A_SPECIAL | 0x80,              K_L,    0,   10,   16,     0, 0x1a,   4,  0x80,       3, SC300,  3,   0,  50,  53,    0,                       0,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 48 */
	{   8,                    ZU, A_SPECIAL | 0x80,              K_L,    0,   10,   16,     0, 0x1a,   4,  0x80,       3, SC300,  3,   0,  50,  52,    0,                       0,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 49 */
	{  12,            ZU | NO_ZU, A_SPECIAL | 0x80,              K_L,    0,    8,   16,     0, 0x1a,   4,  0x80,       3, SC300,  3,   0,  50,  44,    0,                       0,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 4a */
	{  14,            ZU | NO_ZU, A_SPECIAL | 0x80,              K_L,    0,    8,   16,     0, 0x1a,   4,  0x80,       3, SC300,  3,   0,  50,  41,    0,                       0,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 4b */
	{   3,            2 | NO_VEC,   A_HYPER | 0x80,              K_S,    0,   10,   20,     0, 0x00,  11,  0x80,       3, SC300,  3,   0,  90,  61,    0, NO_WEIGHT | NO_COMBO | NO_KO,  1,     0, 16,    12,     0,   0,        3, { 0, 0 } },	/* 4c: pl0b_at_justice, Final Justice (hyper): the dash and the catch */
	{   3,                     5,   A_HYPER | 0x80,              K_L,    0,    2,    0,     0, 0x1a,   1,  0x80,       3, SC100,  3,   0,  90, 123,    0,                   NO_KO,  1,     0, 16,     0,     0,   0,        3, { 0, 0 } },	/* 4d */
	{   3,                     5,   A_HYPER | 0x80,              K_M,    0,    6,    0,     0, 0x1a,   1,  0x80,       3, SC100,  4,   0,  90, 123,    0,                   NO_KO,  1,     0, 16,     0,     0,   0,        3, { 0, 0 } },	/* 4e */
	{   6,                     2,   A_HYPER | 0x80,              K_L,    0,    2,    0,    60, 0x00,   9,  0x80,       3, SC300,  3,   0,  90, 123,    0,                   NO_KO,  1,     0, 16,     0,     0,   0,        3, { 0, 0 } },	/* 4f */
	{   5,           ZU | NO_VEC,   A_HYPER | 0x80,             K_LL,    0,    4,   16,     0, 0x00,   0,  0x80,       3, SC100,  3,   0,  90, 124,  125,                       0,  1,     0, 17,     0,     0,   0,        3, { 0, 0 } },	/* 50: pl0b_at_hstars, Hyper Stars & Stripes (hyper): the first hit; the later ones are 0x4f + n */
	{  15,         BU_O | NO_VEC,          A_HYPER,              K_L,    0,    5,    0,     0, 0x1d,  15,  0x80,       3, SC300,  4,   0,  90, 126,  125, NO_CHIP | NO_WEIGHT | NO_SCALE,  1,     0, 17,     0,     0,   0,        3, { 0, 0 } },	/* 51 */
	{   6, BU_O | NO_VEC | NO_ZU,   A_HYPER | 0x80,              K_L,    0,    8,   44,     0, 0x1a,   2,  0x80,       3, SC300,  3,   0,  90, 168,    0,                       0,  1,     0, 18,     0,     0,   0,        3, { 0, 0 } },	/* 52: pl0b_at_hcharge, Hyper Charging Star (hyper) */
	{   5, BU_O | NO_VEC | NO_ZU,   A_HYPER | 0x80,              K_L,    0,    8,   44,     0, 0x1a,   2,  0x80,       3, SC300,  3,   0,  90, 168,    0,                 NO_CHIP,  1,     0, 18,     0,     0,   0,        3, { 0, 0 } },	/* 53 */
	{   8,                     2,          A_HYPER,              K_L,    0,    8,   44,     0, 0x1a,  15,  0x80,       3, SC300,  4,   0,  90, 168,    0,                 NO_CHIP,  1,     0, 18,     0,     0,   0,        3, { 0, 0 } },	/* 54 */
	{  10,           10 | NO_VEC,        A_SPECIAL,              K_L,    0,   10,   10,     0, 0x1f,  10,  0x60,       3, SC200, 12,   0,  90, 143,    0, NO_CHIP | NO_WEIGHT | AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        5, { 0, 0 } },	/* 55: pl0b_at_t_kick, the jump-in kick when he is tagged in (t_kick_init) */
	{  10,           12 | NO_VEC,        A_SPECIAL,              K_L,    1,    4,   24,     0, 0x1c,  12,  0x80,       3, SC300, 12,  64,  90,  12,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0,        5, { 0, 0 } },	/* 56: pl0b_at_snapback, Snapback (sp_snapback) */
	{  13,         BU_O | NO_VEC,        A_SPECIAL,             K_LL,    1,   10,   24,     0, 0x00,  11,  0x80, MARK_N3, SC300,  3,   0,  40,  15,    0, NO_CHIP | NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 57: pl0b_at_upper_noshield, the launcher, crouching punch, with the shield away */
	{  15,         BU_O | NO_VEC,        A_SPECIAL,             K_LL,    1,   10,   24,     0, 0x00,  11,  0x80, MARK_N3, SC300,  3,   0,  40,  32,    0, NO_CHIP | NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 58: pl0b_at_upper, the launcher, crouching punch, with the shield in hand */
};
