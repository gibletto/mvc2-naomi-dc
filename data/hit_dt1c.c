/* Megaman (character 1c): the attacks (HIT_DATA, 0x1c bytes).
   What each hit does. The character's module sets the attack's number (atck_no) with the move,
   and the row is read when the attack box meets a body. After a row is its number, in
   hexadecimal, and the program's name for it:
   ATCK_ for the normals every character has (player.h), pl1c_at_ for its own (pl1c_def.h).
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
     box, box2  boxes of hit_1c that the computer measures the attack's reach by
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

const HIT_DATA hit_dt_1c[] = {
	/* dmg                 type          kind  elem  piyo  stop  sand  sleep   vec  pow  range     mark  score        se  shc  jug  box  box2                     flag  13  guard  15  catch  abyss  cnt    answer        1a */
	{   3,                  ZU,        A_NOM, K_LS,    4,   10,   10,     0, 0x18,   4,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  15,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 00: ATCK_STAND_LP */
	{   7,          ZU | NO_ZU,        A_NOM,  K_M,    2,   11,   16,     0, 0x1d,   6,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40,  16,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 01: ATCK_STAND_MP */
	{  12,                  ZU,        A_NOM,  K_L,    1,   10,   24,     0, 0x1f,  11,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  50,   0,    0,                       0,  0,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 02: ATCK_STAND_HP */
	{   3,                  ZU,        A_NOM, K_LS,    4,   10,   10,     0, 0x18,   2,  0x60, MARK_N1, SC100, SD_KIK_S,   0,  20,  18,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 03: ATCK_STAND_LK */
	{   7,                  ZU,        A_NOM,  K_M,    2,   11,   16,     0, 0x1a,   3,  0x60, MARK_N2, SC200, SD_KIK_M,   0,  40,  24,   25,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 04: ATCK_STAND_MK */
	{  12,       BU_O | NO_VEC,        A_NOM, K_LL,    1,   10,   24,     0, 0x00,  11,  0x80, MARK_N3, SC300, SD_KIK_L,   0,  50,  28,  134, NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 05: ATCK_STAND_HK */
	{   4,                  ZU,        A_NOM, K_LS,    4,   10,   10,     0, 0x18,   6,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  30,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 06: ATCK_CROUCH_LP */
	{   7,                  ZU,        A_NOM,  K_M,    2,   11,   16,     0, 0x18,   8,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40,  32,   34,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 07: ATCK_CROUCH_MP */
	{  12,                  ZU,        A_NOM, K_LL,    1,   10,   24,     0, 0x18,  11,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  50,   0,    0,                       0,  0,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 08: ATCK_CROUCH_HP */
	{   4,                  ZU,         A_LO, K_LS,    4,   10,   10,     0, 0x18,   8,  0x70, MARK_N1, SC100, SD_KIK_S,   0,  20,  48,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 09: ATCK_CROUCH_LK */
	{   8,                  ZU,         A_LO, K_LM,    2,   11,   16,     0, 0x18,   8,  0x40, MARK_N2, SC200, SD_KIK_M,   0,  40,  68,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0a: ATCK_CROUCH_MK */
	{  13, AS | NO_VEC | NO_ZU,         A_LO, K_LL,    1,   12,   24,     0, 0x18,   8,  0x70, MARK_N3, SC300, SD_KIK_L,   0,  90,  68,    0,                       0,  0,     0,  0,     0,    18,   0, YK_GD060, { 0, 0 } },	/* 0b: ATCK_CROUCH_HK */
	{   4,         ZU | NO_VEC,         A_HI,  K_S,    1,   10,   10,     0, 0x00,   6,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  56,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0c: ATCK_JUMP_LP */
	{   6,         ZU | NO_VEC,         A_HI,  K_M,    1,   10,   16,     0, 0x00,   4,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40,  42,   40,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0d: ATCK_JUMP_MP */
	{  10,         ZU | NO_VEC,         A_HI,  K_M,    1,   10,   32,     0, 0x10,   8,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  60,   0,    0, NO_WEIGHT | AERIAL_FINISH,  0,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 0e: ATCK_JUMP_HP */
	{   4,         ZU | NO_VEC,         A_HI,  K_S,    1,   10,   10,     0, 0x00,   6,  0x60, MARK_N1, SC100, SD_KIK_S,   0,  20,  59,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0f: ATCK_JUMP_LK */
	{   6,         ZU | NO_VEC,         A_HI,  K_M,    1,   10,   16,     0, 0x1e,   4,  0x60, MARK_N2, SC200, SD_KIK_M,   0,  40,  81,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 10: ATCK_JUMP_MK */
	{  12,         ZU | NO_VEC,         A_HI,  K_M,    1,   12,   32,     0, 0x18,   8,  0x80, MARK_N3, SC300, SD_KIK_L,   0,  60,  65,    0,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 11: ATCK_JUMP_HK */
	{ 0 },	/* 12 */
	{ 0 },	/* 13 */
	{ 0 },	/* 14 */
	{ 0 },	/* 15 */
	{  13,       BU_O | NO_VEC,    A_SPECIAL, K_LL,    0,   10,   24,     0, 0x00,  11,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  40,  68,    0, NO_CHIP | NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 16: pl1c_at_upper, the launcher */
	{ 0 },	/* 17 */
	{ 0 },	/* 18 */
	{ 0 },	/* 19 */
	{ 0 },	/* 1a */
	{ 0 },	/* 1b */
	{ 0 },	/* 1c */
	{ 0 },	/* 1d */
	{ 0 },	/* 1e */
	{ 0 },	/* 1f */
	{  16,                   2, A_NOM | 0x40, K_LM,    4,   10,   20,     0, 0x09,  10,  0x60, MARK_N2, SC200, SD_NOR_M,   0,  40,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 20: pl1c_at_throw, Ground throw */
	{  16,                   2, A_NOM | 0x40, K_LM,    4,   10,   20,     0, 0x14,  10,  0x60, MARK_N2, SC200, SD_NOR_M,   0,  40,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 21: pl1c_at_air_throw, Air throw */
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
	{  14, ZU | NO_VEC | NO_ZU,        A_NOM, K_LM,    1,   10,   24,     0, 0x1a,   4,  0x60, MARK_N3, SC200, SD_NOR_M,   0,  50,  63,    0,     NO_CHIP | NO_WEIGHT,  0,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 30 */
	{   6, ZU | NO_VEC | NO_ZU,    A_SPECIAL, K_LL,    0,    6,   24,     0, 0x1b,   6,  0x60,       3, SC100, SD_NOR_L,   5,  90,  79,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 31: pl1c_at_buster, Charged Mega Buster, standing or crouching */
	{   6, ZU | NO_VEC | NO_ZU,    A_SPECIAL, K_LL,    0,    6,   24,     0, 0x1b,   6,  0x60,       3, SC100, SD_NOR_L,   5,  90,  79,    0,     NO_CHIP | NO_WEIGHT,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 32 */
	{  16,                BU_O,    A_SPECIAL, K_LL,    0,   12,   24,     0, 0x1e,   9,  0x80,       3, SC300,        8,   0, 100,  75,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        5, { 0, 0 } },	/* 33 */
	{  16,                BU_O,    A_SPECIAL, K_LL,    0,   12,   24,     0, 0x1e,   9,  0x80,       3, SC300,        8,   0, 100,  75,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        5, { 0, 0 } },	/* 34 */
	{ 0 },	/* 35 */
	{ 0 },	/* 36 */
	{  16,                BU_O,    A_SPECIAL, K_LL,    0,   12,   24,     0, 0x1e,   9,  0x80,       3, SC300,        8,   0, 100,  75,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        5, { 0, 0 } },	/* 37 */
	{  16,                BU_O,    A_SPECIAL, K_LL,    0,   12,   24,     0, 0x1e,   9,  0x80,       3, SC300,        8,   0, 100,  75,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        5, { 0, 0 } },	/* 38 */
	{  10, ZU | NO_VEC | NO_ZU,    A_SPECIAL,  K_M,    0,   10,   24,     0, 0x1a,   4,  0x60,       3, SC200,        8,   0,  64,  83,    0,     NO_CHIP | NO_WEIGHT,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 39: pl1c_at_rockball, Rock Ball: the kick and the bouncing ball (shl1c_02) */
	{   6,        BU_O | NO_ZU,    A_SPECIAL,  K_M,    0,    6,   24,     0, 0x00,   8,  0x60,       3, SC200,        8,   4,  50,  91,   89,     NO_CHIP | NO_WEIGHT,  1,     0,  0,     0,    20,   0, YK_GD060, { 0, 0 } },	/* 3a: pl1c_at_item, Tornado Hold and Leaf Shield put up, and the tornado (shl1c_03) */
	{ 0 },	/* 3b */
	{  20, ZU | NO_VEC | NO_ZU,        A_NOM,  K_M,    0,   10,   24,     0, 0x17,   8,  0x60, MARK_N3, SC200, SD_NOR_M,   0,  50,  63,    0, NO_CHIP | NO_WEIGHT | AERIAL_FINISH,  0,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 3c */
	{   6,  2 | NO_VEC | NO_ZU,    A_SPECIAL,  K_L,    0,    4,   24,     0, 0x1b,   6,  0x60,       3, SC100, SD_NOR_L,   5,  90,  79,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 3d: pl1c_at_buster_air, Charged Mega Buster in the air */
	{   6,  2 | NO_VEC | NO_ZU,    A_SPECIAL,  K_L,    0,    4,   24,     0, 0x1b,   6,  0x60,       3, SC100, SD_NOR_L,   5,  90,  79,    0,     NO_CHIP | NO_WEIGHT,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 3e */
	{   5, ZU | NO_VEC | NO_ZU,      A_HYPER,  K_M,    0,   10,   24,     0, 0x1a,   4,  0x60,       3, SC200, SD_NOR_L,   0,  50,  99,    0,               NO_WEIGHT,  1,     0, 50,     0,     0,   0,        3, { 0, 0 } },	/* 3f: pl1c_at_beat_2, Beat Plane: the last arm's hit (shl1c_08 kind 4) */
	{   1,  2 | NO_VEC | NO_ZU,      A_HYPER, K_LL,    0,   10,   24,     0, 0x1a,   4,  0x60,       3, SC200, SD_NOR_M,   0,  90, 101,    0,     NO_CHIP | NO_WEIGHT,  1,     0, 51,     0,     0,   4,        3, { 0, 0 } },	/* 40 */
	{   1,  2 | NO_VEC | NO_ZU,      A_HYPER,  K_L,    0,   10,   24,     0, 0x1a,   4,  0x60,       3, SC200, SD_NOR_M,   0,  90, 103,    0,     NO_CHIP | NO_WEIGHT,  1,     0, 51,     0,     0,   4,        3, { 0, 0 } },	/* 41 */
	{   1,  2 | NO_VEC | NO_ZU,      A_HYPER, K_LL,    0,   10,   24,     0, 0x1a,   4,  0x60,       3, SC200, SD_NOR_L,   0,  90, 105,    0,     NO_CHIP | NO_WEIGHT,  1,     0, 51,     0,     0,   4,        3, { 0, 0 } },	/* 42 */
	{   1,  2 | NO_VEC | NO_ZU,      A_HYPER,  K_L,    0,   10,   24,     0, 0x1a,   4,  0x60,       3, SC200, SD_NOR_L,   0,  90, 107,    0,     NO_CHIP | NO_WEIGHT,  1,     0, 51,     0,     0,   4,        3, { 0, 0 } },	/* 43: pl1c_at_hyper_rock_missile, Hyper Mega Man: the missiles (shl1c_06 kinds 3 and up) */
	{   4,  2 | NO_VEC | NO_ZU,      A_HYPER,  K_L,    0,   10,   24,     0, 0x1a,   4,  0x60,       5, SC200, SD_NOR_L,   0,  90, 109,    0,               NO_WEIGHT,  1,     0, 51,     0,     0,   6,        3, { 0, 0 } },	/* 44: pl1c_at_hyper_rock_1, Hyper Mega Man: the main fire, even frames */
	{   6,                  ZU,      A_HYPER, K_LS,    0,    1,   24,     0, 0x1a,   4,  0x60,       3, SC200, SD_NOR_L,   0,  90, 111,    0,               NO_WEIGHT,  1,     0, 49,     0,     0,   0,        3, { 0, 0 } },	/* 45: pl1c_at_drill, Rush Drill (hyper) */
	{   2,          2 | NO_VEC,      A_HYPER,  K_L,    0,   18,   50,     0, 0x1f,  11,  0x80,       3, SC300, SD_NOR_L,   0, 100, 118,  116, NO_CHIP | NO_WEIGHT | NO_KO,  1,     0, 51,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 46: pl1c_at_hyper_rock, Hyper Mega Man (hyper): the start */
	{ 0 },	/* 47 */
	{   2, ZU | NO_VEC | NO_ZU,    A_SPECIAL,  K_L,    0,    4,   24,     0, 0x1b,   2,  0x60,       3, SC200, SD_NOR_L,   4,  20,  95,    0,     NO_CHIP | NO_WEIGHT,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 48: pl1c_at_shield_throw, Leaf Shield thrown, ground or air, and its leaves (shl1c_04) */
	{   2,                  ZU,      A_HYPER, K_LM,    0,    1,   24,     0, 0x1a,   4,  0x60,       3, SC200, SD_NOR_L,   0,  90, 111,    0,     NO_CHIP | NO_WEIGHT,  1,     0, 49,     0,     0,   0,        3, { 0, 0 } },	/* 49 */
	{   3, ZU | NO_VEC | NO_ZU,      A_HYPER, K_LM,    0,   10,   24,     0, 0x1a,   4,  0x60,       3, SC200, SD_NOR_M,   0,  50,  97,    0,     NO_CHIP | NO_WEIGHT,  1,     0, 50,     0,     0,   0,        3, { 0, 0 } },	/* 4a */
	{   5, ZU | NO_VEC | NO_ZU,      A_HYPER, K_LS,    0,   10,   24,     0, 0x1a,   4,  0x60,       3, SC200, SD_NOR_L,   0,  50,  99,    0,     NO_CHIP | NO_WEIGHT,  1,     0, 50,     0,     0,   0,        3, { 0, 0 } },	/* 4b */
	{   5,  2 | NO_VEC | NO_ZU,      A_HYPER,  K_L,    0,   10,   24,     0, 0x1a,   4,  0x60,       5, SC200, SD_NOR_L,   0,  90, 109,    0,     NO_CHIP | NO_WEIGHT,  1,     0, 51,     0,     0,   6,        3, { 0, 0 } },	/* 4c: pl1c_at_hyper_rock_2, Hyper Mega Man: the main fire, odd frames */
	{   5,                  10,      A_HYPER, K_LL,    0,    1,   24,     0, 0x1e,  10,  0x60,       3, SC200, SD_NOR_L,   0,  90, 111,    0,               NO_WEIGHT,  1,     0, 49,     0,     0,   0,        3, { 0, 0 } },	/* 4d */
	{   4, ZU | NO_VEC | NO_ZU,      A_HYPER,  K_S,    0,   10,   24,     0, 0x1a,   4,  0x60,       3, SC200, SD_NOR_M,   0,  50,  97,    0,               NO_WEIGHT,  1,     0, 50,     0,     0,   0,        3, { 0, 0 } },	/* 4e: pl1c_at_beat, Beat Plane (hyper), Mega Man and the first arm (shl1c_08 kind 0) */
	{  10,         10 | NO_VEC,    A_SPECIAL,  K_L,    0,   10,   10,     0, 0x1f,  10,  0x60,       3, SC200,       12,   0,  90,  65,    0, NO_CHIP | NO_WEIGHT | AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        5, { 0, 0 } },	/* 4f: pl1c_at_t_kick, Tag-in kick that leaps off the wall */
	{  10,         12 | NO_VEC,    A_SPECIAL,  K_L,    0,    4,   24,     0, 0x1c,  12,  0x80,       3, SC300,       12,  64,  90,  28,  134,               NO_WEIGHT,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 50: pl1c_at_snapback, Snapback */
};
