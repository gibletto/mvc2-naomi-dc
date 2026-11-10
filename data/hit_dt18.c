/* Abyss (first form) (character 18): the attacks (HIT_DATA, 0x1c bytes).
   What each hit does. The character's module sets the attack's number (atck_no) with the move,
   and the row is read when the attack box meets a body. After a row is its number, in
   hexadecimal, and the program's name for it:
   ATCK_ for the normals every character has (player.h), pl18_at_ for its own (pl18_def.h).
   The values' names are Capcom's own for these tables (hit_equ.h) and the program's (player.h).

     dmg     the damage
     type    the reaction: ZU pushed back, BU_O knocked off the feet, AS swept,
             or the number of another in pl_dmg.c's dm_tbl (here 2 dm_buttobi_ng).
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
     box, box2  boxes of hit_18 that the computer measures the attack's reach by
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

const HIT_DATA hit_dt_18[] = {
	/* dmg                 type       kind      elem  piyo  stop  sand  sleep   vec  pow  range     mark  score        se  shc  jug  box  box2                     flag  13  guard  15  catch  abyss  cnt    answer        1a */
	{   6,                  ZU,     A_NOM,      K_S,    4,   12,   10,     0, 0x18,   4,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  15,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 00: ATCK_STAND_LP */
	{   4,                  ZU,     A_NOM,     K_LM,    2,    8,   16,     0, 0x18,   6,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40,  16,   17,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 01: ATCK_STAND_MP */
	{  10,                BU_O,     A_NOM,      K_L,    1,   18,   24,     0, 0x1c,  10,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  30,  28,  155,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 02: ATCK_STAND_HP */
	{   4,                  ZU,     A_NOM,      K_S,    4,   12,   28,     0, 0x1e,   4,  0x60, MARK_N1, SC100, SD_KIK_S,   0,  20,  18,   52,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 03: ATCK_STAND_LK */
	{  10,                  ZU,     A_NOM,  K_M | 2,    2,   10,   24,     0, 0x1e,   4,  0x60, MARK_N2, SC200, SD_KIK_M,   0,  30,  24,   25,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 04: ATCK_STAND_MK */
	{   8,                BU_O,     A_NOM,      K_M,    1,   12,   50,     0, 0x1e,   9,  0x60, MARK_N3, SC300, SD_KIK_L,   0,  50,  58,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 05: ATCK_STAND_HK */
	{   6,                  ZU,     A_NOM,     K_LS,    4,   12,   10,     0, 0x1c,   4,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  30,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 06: ATCK_CROUCH_LP */
	{   8,                  ZU,     A_NOM,     K_LM,    2,    8,   16,     0, 0x13,   6,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40,  79,    0,                       0,  0,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 07: ATCK_CROUCH_MP */
	{  18,       BU_O | NO_VEC,     A_NOM,     K_LL,    1,   14,   24,     0, 0x00,  11,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  50,  32,   34, NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 08: ATCK_CROUCH_HP */
	{   6,                  ZU,      A_LO,     K_LS,    4,   12,   10,     0, 0x1c,   4,  0x70, MARK_N1, SC100, SD_KIK_S,   0,  20,  48,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 09: ATCK_CROUCH_LK */
	{  12,       BU_O | NO_VEC,     A_NOM,     K_LM,    2,   16,   16,     0, 0x1f,   8,  0x40, MARK_N2, SC200, SD_KIK_M,   0,  40,  68,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0a: ATCK_CROUCH_MK */
	{  18,                  AS,      A_LO,     K_LL,    1,   18,   24,     9, 0x1a,   8,  0x70, MARK_N3, SC300, SD_KIK_L,   0, 100,  32,  140,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0b: ATCK_CROUCH_HK */
	{   6,         ZU | NO_VEC,      A_HI,      K_S,    1,   12,   10,     0, 0x00,   5,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  56,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0c: ATCK_JUMP_LP */
	{  12,         ZU | NO_VEC,      A_HI, K_LM | 3,    1,   16,   16,     0, 0x00,   2,  0x80, MARK_N2, SC200,       22,   0,  40,  40,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0d: ATCK_JUMP_MP */
	{  18,         ZU | NO_VEC,      A_HI,     K_LL,    1,   12,   32,     0, 0x18,   8,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  60, 134,  137,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0e: ATCK_JUMP_HP */
	{   6,         ZU | NO_VEC,      A_HI,      K_S,    1,   12,   10,     0, 0x00,   5,  0x60, MARK_N1, SC200, SD_KIK_S,   0,  20,  59,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0f: ATCK_JUMP_LK */
	{   7,         ZU | NO_VEC,      A_HI,      K_M,    1,   10,   16,     0, 0x00,   2,  0x60, MARK_N2, SC200, SD_KIK_M,   0,  40,  81,  123,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 10: ATCK_JUMP_MK */
	{  18,         ZU | NO_VEC,      A_HI,      K_L,    1,   18,   32,     0, 0x15,   8,  0x60, MARK_N3, SC200, SD_KIK_L,   0,  60,  65,    0,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 11: ATCK_JUMP_HK */
	{ 0 },	/* 12 */
	{ 0 },	/* 13 */
	{ 0 },	/* 14 */
	{ 0 },	/* 15 */
	{ 0 },	/* 16 */
	{ 0 },	/* 17 */
	{ 0 },	/* 18 */
	{ 0 },	/* 19 */
	{ 0 },	/* 1a */
	{ 0 },	/* 1b */
	{ 0 },	/* 1c */
	{ 0 },	/* 1d */
	{ 0 },	/* 1e */
	{ 0 },	/* 1f */
	{ 0 },	/* 20 */
	{ 0 },	/* 21 */
	{ 0 },	/* 22 */
	{ 0 },	/* 23 */
	{ 0 },	/* 24 */
	{ 0 },	/* 25 */
	{ 0 },	/* 26 */
	{ 0 },	/* 27 */
	{ 0 },	/* 28 */
	{ 0 },	/* 29 */
	{ 0 },	/* 2a */
	{ 0 },	/* 2b */
	{ 0 },	/* 2c */
	{ 0 },	/* 2d */
	{ 0 },	/* 2e */
	{ 0 },	/* 2f */
	{   6, ZU | NO_VEC | NO_ZU, A_SPECIAL,     K_LM,    0,   10,   10,     0, 0x1a,   4,  0x60,       3, SC200, SD_NOR_M,   0,  50,  63,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        5, { 0, 0 } },	/* 30: pl18_at_t_kick, Tag-in kick that leaps off the wall (tk_init) */
	{  10, ZU | NO_VEC | NO_ZU, A_SPECIAL,      K_M,    0,   10,   10,     0, 0x1c,  11,  0x60,       3, SC200,        8,   0,  90,  83,   85,                       0,  1,     0,  0,     0,     0,   0,        5, { 0, 0 } },	/* 31: pl18_at_thunder, Dark bolts dropped on the opponent (hyper), and each spark (shl18_00) */
	{  10,          ZU | NO_ZU, A_SPECIAL,  K_L | 3,    0,   10,   10,     0, 0x1f,   8,  0x60,       3, SC200,       22,   0,  90,  68,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 32: pl18_at_ball, Three homing dark balls */
	{  18,          2 | NO_VEC, A_SPECIAL,      K_L,    0,   10,   30,     0, 0x1c,   9,  0x80,       3, SC300,        8,   0, 100,  16,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 33: pl18_at_sweep, the long sweeping blow */
	{   4,                  ZU, A_SPECIAL,      K_L,    0,    5,   10,     0, 0x18,   8,  0x80,       3, SC300,        8,   0, 100,  24,    0,                       0,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 34: pl18_at_rush_1, the charging rush: odd hits */
	{   4,          ZU | NO_ZU,   A_HYPER,      K_M,    0,   10,   30,     0, 0x18,   9,  0x80,       5, SC300,       18,   0, 100,  87,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 35: pl18_at_pillar, Row of dark pillars (hyper), Abyss and the pillars (shl18_03) */
	{   4,          ZU | NO_ZU,   A_HYPER,      K_M,    0,   10,   30,     0, 0x18,   9,  0x80,       5, SC300,       18,   0, 100,  87,    0,     NO_CHIP | NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 36 */
	{   5,          ZU | NO_ZU, A_SPECIAL,      K_L,    0,   10,   30,     0, 0x18,   6,  0x80,       3, SC300,        8,   0, 100,  80,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 37 */
	{   6,          ZU | NO_ZU, A_SPECIAL,      K_L,    0,   10,   30,     0, 0x18,   7,  0x80,       3, SC300,        8,   0, 100,  82,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 38 */
	{   7,          ZU | NO_ZU, A_SPECIAL,      K_L,    0,   10,   30,     0, 0x18,   8,  0x80,       3, SC300,        8,   0, 100,  84,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 39 */
	{   4,          2 | NO_VEC, A_SPECIAL,      K_L,    0,    5,   10,     0, 0x1e,  10,  0x80,       3, SC300,        8,   0, 100,  24,    0,                       0,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 3a: pl18_at_rush_end, the charging rush: hits in its last steps (timer0 under 6) */
	{   8,          2 | NO_VEC, A_SPECIAL,      K_L,    0,   10,   30,     0, 0x1c,   9,  0x80,       3, SC300,        8,   0, 100,  23,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 3b: pl18_at_punch, the dark fist thrown out */
	{  10,  2 | NO_VEC | NO_ZU, A_SPECIAL,      K_L,    0,    5,   10,     0, 0x1e,  10,  0x80,       3, SC300,        8,   0, 100,  94,    0,                       0,  1,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 3c */
	{   4,                  ZU, A_SPECIAL,      K_L,    0,    5,   10,     0, 0x18,   8,  0x80,       3, SC300,        8,   0, 100,  24,    0,                 NO_CHIP,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 3d: pl18_at_rush_2, the charging rush: even hits */
};
