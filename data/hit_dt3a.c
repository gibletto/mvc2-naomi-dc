/* Servbot (character 3a): the attacks (HIT_DATA, 0x1c bytes).
   What each hit does. The character's module sets the attack's number (atck_no) with the move,
   and the row is read when the attack box meets a body. After a row is its number, in
   hexadecimal, and the program's name for it:
   ATCK_ for the normals every character has (player.h), pl3a_at_ for its own (pl3a_def.h).
   The values' names are Capcom's own for these tables (hit_equ.h) and the program's (player.h).

     dmg     the damage
     type    the reaction: ZU pushed back, BU_O knocked off the feet, AS swept,
             or the number of another in pl_dmg.c's dm_tbl (here 2 dm_buttobi_ng, 10 dm_away,
             12 dm_snap, 21 dm_buttobi_ukemi_ng).
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
     box, box2  boxes of hit_3a that the computer measures the attack's reach by
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

const HIT_DATA hit_dt_3a[] = {
	/* dmg                     type          kind     elem  piyo  stop  sand  sleep   vec  pow  range     mark  score        se  shc  jug  box  box2                     flag  13  guard  15  catch  abyss  cnt    answer        1a */
	{   1,                      ZU,        A_NOM,     K_S,    4,   10,   10,     0, 0x18,   4,  0x70,      13, SC100,       13,   0,  20,  80,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 00: ATCK_STAND_LP */
	{   2,                      ZU,        A_NOM,     K_M,    2,   11,   16,     0, 0x00,   6,  0x80,      12, SC200,       14,   0,  40,  93,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 01: ATCK_STAND_MP */
	{   3,           BU_O | NO_VEC,        A_NOM,     K_L,    1,   12,   24,     0, 0x00,   9,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  50,  81,  100, NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 02: ATCK_STAND_HP */
	{   1,                      ZU,        A_NOM,     K_S,    4,   10,   10,     0, 0x18,   4,  0x60,      14, SC100,       13,   0,  20,  92,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 03: ATCK_STAND_LK */
	{   2,                      ZU,        A_NOM,     K_M,    2,   10,   16,     0, 0x1a,   4,  0x60,      13, SC200,       14,   0,  40,  94,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 04: ATCK_STAND_MK */
	{   3,           BU_O | NO_VEC,        A_NOM,     K_L,    1,   12,   24,     0, 0x00,   9,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  50,  81,  100, NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 05: ATCK_STAND_HK */
	{   1,                      ZU,        A_NOM,    K_LS,    4,   10,   10,     0, 0x1c,   4,  0x70,      13, SC100,       13,   0,  20,  89,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 06: ATCK_CROUCH_LP */
	{   2,                      ZU,        A_NOM,    K_LM,    2,   11,   16,     0, 0x13,   6,  0x80,      14, SC200,       14,   0,  40,  96,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 07: ATCK_CROUCH_MP */
	{   3,                      AS,         A_LO,    K_LL,    1,   12,   24,     9, 0x1a,   8,  0x70, MARK_N3, SC300, SD_KIK_L,   0,  90,  84,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 08: ATCK_CROUCH_HP */
	{   1,                      ZU,         A_LO,    K_LS,    4,   10,   10,     0, 0x1c,   4,  0x70,      12, SC100,       13,   0,  20,  95,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 09: ATCK_CROUCH_LK */
	{   2,                      ZU,         A_LO,    K_LM,    2,   11,   16,     0, 0x1d,   6,  0x40,      13, SC200,       14,   0,  40,  97,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0a: ATCK_CROUCH_MK */
	{   3,                      AS,         A_LO,    K_LL,    1,   12,   24,     9, 0x1a,   8,  0x70, MARK_N3, SC300, SD_KIK_L,   0,  90,  84,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0b: ATCK_CROUCH_HK */
	{   1,             ZU | NO_VEC,         A_HI,     K_S,    1,   10,   10,     0, 0x00,   5,  0x70,      14, SC100,       13,   0,  20,  82,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0c: ATCK_JUMP_LP */
	{   2,             ZU | NO_VEC,         A_HI,    K_LM,    1,   10,   16,     0, 0x00,   8,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40,  83,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0d: ATCK_JUMP_MP */
	{   3,             ZU | NO_VEC,         A_HI,     K_L,    1,   12,   32,     0, 0x18,   7,  0x80,      13, SC300, SD_NOR_L,   0,  60,  98,    0, NO_WEIGHT | AERIAL_FINISH,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0e: ATCK_JUMP_HP */
	{   1,             ZU | NO_VEC,         A_HI,     K_S,    1,   10,   10,     0, 0x00,   5,  0x60, MARK_N1, SC200, SD_KIK_S,   0,  20,  85,   90,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0f: ATCK_JUMP_LK */
	{   2,             ZU | NO_VEC,         A_HI,     K_M,    1,   10,   16,     0, 0x00,   5,  0x60, MARK_N2, SC200, SD_KIK_M,   0,  40,  86,   91,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 10: ATCK_JUMP_MK */
	{   3,             ZU | NO_VEC,         A_HI,     K_L,    1,   12,   32,     0, 0x12,   9,  0x60,      14, SC200, SD_KIK_L,   0,  60,  88,    0, NO_WEIGHT | AERIAL_FINISH,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 11: ATCK_JUMP_HK */
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
	{   3,                       2, A_NOM | 0x40,       0,    0,    4,    0,    40, 0x0a,   0,  0x80, MARK_N1, SC300,       76,   0,   0,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 21: pl3a_at_throw, Ground and air throw's last throw (catch_throw) */
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
	{   2,      2 | NO_VEC | NO_ZU,    A_SPECIAL,     K_S,    0,    6,   20,     0, 0x1e,   4,  0x60, MARK_N1, SC200, SD_NOR_S,   0,  50,  64,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 30: pl3a_at_fire, Kobun Fire (fire_init); also the tackle (tackle_init) and shl3a_04's pieces */
	{   2,      2 | NO_VEC | NO_ZU,    A_SPECIAL,     K_M,    0,    6,   20,     0, 0x1e,   4,  0x60, MARK_N2, SC200, SD_NOR_M,   0,  50,  64,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 31 */
	{   2,      2 | NO_VEC | NO_ZU,    A_SPECIAL,     K_L,    0,    6,   20,     0, 0x1e,   4,  0x60, MARK_N3, SC200, SD_NOR_L,   0,  50,  64,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 32 */
	{   2,      2 | NO_VEC | NO_ZU,    A_SPECIAL,    0x40,    0,    6,   20,     0, 0x1e,   4,  0x60,       3, SC200,        8,   0,  50,  64,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 33 */
	{   2,      2 | NO_VEC | NO_ZU,    A_SPECIAL,    K_LS,    0,    6,   20,     0, 0x1e,   4,  0x60, MARK_N1, SC200, SD_NOR_S,   0,  50,  64,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 34 */
	{   2,      2 | NO_VEC | NO_ZU,    A_SPECIAL,    K_LM,    0,    6,   20,     0, 0x1e,   4,  0x60, MARK_N2, SC200, SD_NOR_M,   0,  50,  64,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 35 */
	{   2,      2 | NO_VEC | NO_ZU,    A_SPECIAL,    K_LL,    0,    6,   20,     0, 0x1e,   4,  0x60, MARK_N3, SC200, SD_NOR_L,   0,  50,  64,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 36 */
	{   2,      2 | NO_VEC | NO_ZU,    A_SPECIAL,    0xc0,    0,    6,   20,     0, 0x1e,   4,  0x60,       3, SC200,        8,   0,  50,  64,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 37 */
	{   2,      2 | NO_VEC | NO_ZU,    A_SPECIAL,     K_S,    0,    6,   20,     0, 0x1e,   4,  0x60, MARK_N1, SC200, SD_NOR_S,   0,  50,  64,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 38 */
	{   2,      2 | NO_VEC | NO_ZU,    A_SPECIAL,     K_M,    0,    6,   20,     0, 0x1e,   4,  0x60, MARK_N2, SC200, SD_NOR_M,   0,  50,  64,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 39 */
	{   2,      2 | NO_VEC | NO_ZU,    A_SPECIAL,     K_L,    0,    6,   20,     0, 0x1e,   4,  0x60, MARK_N3, SC200, SD_NOR_L,   0,  50,  64,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 3a */
	{   2,      2 | NO_VEC | NO_ZU,    A_SPECIAL,    K_LS,    0,    6,   20,     0, 0x1e,   4,  0x60, MARK_N1, SC200, SD_NOR_S,   0,  50,  64,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 3b */
	{   2,      2 | NO_VEC | NO_ZU,    A_SPECIAL,    K_LM,    0,    6,   20,     0, 0x1e,   4,  0x60, MARK_N2, SC200, SD_NOR_M,   0,  50,  64,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 3c */
	{   2,      2 | NO_VEC | NO_ZU,    A_SPECIAL,    K_LL,    0,    6,   20,     0, 0x1e,   4,  0x60, MARK_N3, SC200, SD_NOR_L,   0,  50,  64,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 3d */
	{   2,      2 | NO_VEC | NO_ZU,    A_SPECIAL,    0xc0,    0,    6,   20,     0, 0x1e,   4,  0x60,       3, SC200,        8,   0,  50,  64,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 3e */
	{   1,      2 | NO_VEC | NO_ZU,      A_HYPER,     K_L,    0,   12,   60,   120, 0x00,  11,  0x60,       3, SC200,        8,   0,  90, 106,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 3f: pl3a_at_lunch, Lunch Rush, Servbot himself (lunch_init) */
	{   1,     ZU | NO_VEC | NO_ZU,      A_HYPER,     K_S,    0,   10,   20,     0, 0x1a,   8,  0x60,       8, SC200, SD_NOR_S,   0,  90,  64,    0,     NO_CHIP | NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 40 */
	{   1,     ZU | NO_VEC | NO_ZU,      A_HYPER,     K_M,    0,   10,   20,     0, 0x1a,   8,  0x60,       8, SC200, SD_NOR_M,   0,  90,  64,    0,     NO_CHIP | NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 41 */
	{   1,     ZU | NO_VEC | NO_ZU,      A_HYPER,     K_L,    0,   10,   20,     0, 0x1a,   8,  0x60,       8, SC200, SD_NOR_L,   0,  90,  64,    0,     NO_CHIP | NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 42 */
	{   1,     ZU | NO_VEC | NO_ZU,      A_HYPER,    0x40,    0,   10,   20,     0, 0x1a,   8,  0x60,       8, SC200,        8,   0,  90,  64,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 43 */
	{   1,     ZU | NO_VEC | NO_ZU,      A_HYPER,    K_LS,    0,   10,   20,     0, 0x1a,   8,  0x60,       8, SC200, SD_NOR_S,   0,  90,  64,    0,     NO_CHIP | NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 44 */
	{   1,     ZU | NO_VEC | NO_ZU,      A_HYPER,    K_LM,    0,   10,   20,     0, 0x1a,   8,  0x60,       8, SC200, SD_NOR_M,   0,  90,  64,    0,     NO_CHIP | NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 45 */
	{   1,     ZU | NO_VEC | NO_ZU,      A_HYPER,    K_LL,    0,   10,   20,     0, 0x1a,   8,  0x60,       8, SC200, SD_NOR_L,   0,  90,  64,    0,     NO_CHIP | NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 46 */
	{   1,     ZU | NO_VEC | NO_ZU,      A_HYPER,    0xc0,    0,   10,   20,     0, 0x1a,   8,  0x60,       8, SC200,        8,   0,  90,  64,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 47 */
	{   1,     ZU | NO_VEC | NO_ZU,      A_HYPER,     K_S,    0,   10,   20,     0, 0x1a,   8,  0x60,       8, SC200, SD_NOR_S,   0,  90,  64,    0,     NO_CHIP | NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 48 */
	{   1,     ZU | NO_VEC | NO_ZU,      A_HYPER,     K_M,    0,   10,   20,     0, 0x1a,   8,  0x60,       8, SC200, SD_NOR_M,   0,  90,  64,    0,     NO_CHIP | NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 49 */
	{   1,     ZU | NO_VEC | NO_ZU,      A_HYPER,     K_L,    0,   10,   20,     0, 0x1a,   8,  0x60,       8, SC200, SD_NOR_L,   0,  90,  64,    0,     NO_CHIP | NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 4a */
	{   1,     ZU | NO_VEC | NO_ZU,      A_HYPER,    K_LS,    0,   10,   20,     0, 0x1a,   8,  0x60,       8, SC200, SD_NOR_S,   0,  90,  64,    0,     NO_CHIP | NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 4b */
	{   1,     ZU | NO_VEC | NO_ZU,      A_HYPER,    K_LM,    0,   10,   20,     0, 0x1a,   8,  0x60,       8, SC200, SD_NOR_M,   0,  90,  64,    0,     NO_CHIP | NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 4c */
	{   1,     ZU | NO_VEC | NO_ZU,      A_HYPER,    K_LL,    0,   10,   20,     0, 0x1a,   8,  0x60,       8, SC200, SD_NOR_L,   0,  90,  64,    0,     NO_CHIP | NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 4d */
	{   4,     ZU | NO_VEC | NO_ZU,      A_HYPER,    0xc0,    0,   10,   20,     0, 0x1a,   8,  0x60,       8, SC200,        8,   0,  90,  64,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 4e */
	{   1, 10 | NO_ZU | FLY_SCREEN,      A_HYPER,     K_L,    0,   10,   10,     0, 0x1e,  13,  0x70,       8, SC300,        8,   0,  90,  64,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 4f: pl3a_at_lunch_first, Lunch Rush's first piece (shl3a_01 mode 1, type 0) */
	{   2,            BU_O | NO_ZU,    A_SPECIAL,    K_LL,    0,    6,   10,     0, 0x1c,   8,  0x70,      13, SC300,       14,   0,  20, 103,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 50: pl3a_at_engo_k_h, Engo Kougeki kick, heavy: the servbot flying in from the far edge (mode 3, up to 3 hits) */
	{   1,     ZU | NO_VEC | NO_ZU,    A_SPECIAL,     K_L,    0,   10,    0,     0, 0x1a,   1,  0x80, MARK_N1, SC100,        0,   0,  50, 104,    0,                       0,  0,     0,  0,     1,     0,   0,        4, { 0, 0 } },	/* 51: pl3a_at_engo_h, Engo Kougeki punch, heavy: the servbot running in that grabs (mode 7) */
	{   1,             10 | NO_VEC,      A_HYPER,    0x40,    0,   16,   20,    30, 0x1f,   8,  0x60,       3, SC200,        8,   0,  90, 105,    0,               NO_WEIGHT,  1,     0, 49,     0,     0,   0,        3, { 0, 0 } },	/* 52: pl3a_at_king, King Kobun (king_init) */
	{   2,            BU_O | NO_ZU,    A_SPECIAL,    K_LL,    0,    6,   10,     0, 0x1c,   8,  0x70,      13, SC300,       14,   0,  20, 103,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 53: pl3a_at_engo_k_l, Engo Kougeki kick, light: the servbot falling from above him (mode 4) */
	{   1,               2 | NO_ZU,      A_HYPER,    K_LL,    0,    6,   10,     0, 0x18,   5,  0x70,      13, SC300,       14,   0,  90, 103,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 54: pl3a_at_lunch_burst_fly, the gamma type's Lunch Rush: the burst's pieces that fly in from the screen's edge (mode 6) */
	{   1,               2 | NO_ZU,      A_HYPER,    K_LL,    0,    6,   10,     0, 0x1e,   8,  0x60,       8, SC200,        8,   0,  90,  64,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 55: pl3a_at_lunch_burst, the gamma type's Lunch Rush (sp_lunch_burst) and the burst's pieces that bounce in (mode 2) */
	{   4,  2 | NO_ZU | FLY_SCREEN,      A_HYPER,    K_LL,    0,   10,   10,     0, 0x19,   8,  0x60,       3, SC200,        8,   0,  90,  64,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 56: pl3a_at_lunch_drop, the beta type's Lunch Rush (sp_lunch_drop) and its piece (mode 18) */
	{ 0 },	/* 57 */
	{ 0 },	/* 58 */
	{ 0 },	/* 59 */
	{ 0 },	/* 5a */
	{ 0 },	/* 5b */
	{ 0 },	/* 5c */
	{ 0 },	/* 5d */
	{ 0 },	/* 5e */
	{ 0 },	/* 5f */
	{ 0 },	/* 60 */
	{ 0 },	/* 61 */
	{ 0 },	/* 62 */
	{ 0 },	/* 63 */
	{ 0 },	/* 64 */
	{ 0 },	/* 65 */
	{ 0 },	/* 66 */
	{ 0 },	/* 67 */
	{ 0 },	/* 68 */
	{ 0 },	/* 69 */
	{ 0 },	/* 6a */
	{ 0 },	/* 6b */
	{ 0 },	/* 6c */
	{   3,              21 | NO_ZU,      A_HYPER,    0x40,    0,   16,   20,    20, 0x1f,   7,  0x60,       3, SC200,        8,   0,  90,  65,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   2,        3, { 0, 0 } },	/* 6d: pl3a_at_rking_last, Rushing King Kobun's last hit (rking_run) */
	{   2,     ZU | NO_VEC | NO_ZU,    A_SPECIAL,    K_LS,    0,    2,   24,     0, 0x1a,   6,  0x80,       3, SC300, SD_NOR_L,   0,  90, 107,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 6e */
	{   2,     ZU | NO_VEC | NO_ZU,    A_SPECIAL,     K_S,    0,    2,   24,     0, 0x1a,   6,  0x80,       3, SC300, SD_NOR_L,   0,  90, 107,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 6f */
	{   2,   BU_O | NO_VEC | NO_ZU,    A_SPECIAL,     K_L,    0,    2,   24,     0, 0x1e,  13,  0x80,       3, SC300, SD_NOR_L,   0,  90, 107,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 70 */
	{   2,     ZU | NO_VEC | NO_ZU,    A_SPECIAL,     K_S,    0,    2,   24,     0, 0x1a,   6,  0x80,       3, SC300, SD_NOR_L,   0,  90, 107,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 71 */
	{   2,     ZU | NO_VEC | NO_ZU,    A_SPECIAL,    K_LS,    0,    2,   24,     0, 0x1a,   6,  0x80,       3, SC300, SD_NOR_L,   0,  90, 107,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 72 */
	{   2,     ZU | NO_VEC | NO_ZU,    A_SPECIAL,     K_S,    0,    2,   24,     0, 0x1a,   6,  0x80,       3, SC300, SD_NOR_L,   0,  90, 107,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 73 */
	{   2,     ZU | NO_VEC | NO_ZU,    A_SPECIAL,    K_LS,    0,    2,   24,     0, 0x1a,  10,  0x80,       3, SC300, SD_NOR_L,   0,  90, 107,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 74 */
	{   2,   BU_O | NO_VEC | NO_ZU,    A_SPECIAL,     K_L,    0,    2,   24,     0, 0x1e,  13,  0x80,       3, SC300, SD_NOR_L,   0,  90, 107,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 75 */
	{   3,           BU_O | NO_VEC,    A_SPECIAL,     K_L,    0,   12,   24,     0, 0x00,   9,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  40,  81,  100, NO_CHIP | NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 76: pl3a_at_upper, Launcher (pl3a_upper) */
	{   1,             10 | NO_VEC,    A_SPECIAL,     K_L,    0,   10,   10,     0, 0x1f,  10,  0x60,       3, SC200,       12,   0,  90, 101,    0, NO_CHIP | NO_WEIGHT | AERIAL_FINISH,  1,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 77: pl3a_at_t_kick, Partner jump-in attack (pl3a_t_kick) */
	{   3,             12 | NO_VEC,    A_SPECIAL,     K_L,    0,    4,   24,     0, 0x1c,  12,  0x80,       3, SC300,        8,  64,  90,  84,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 78: pl3a_at_snapback, Snapback (sp_snapback, crouching) */
	{   1,     ZU | NO_VEC | NO_ZU,        A_NOM,     K_S,    0,    6,    0,     0, 0x00,   0,  0x60, MARK_N1, SC200, SD_NOR_S,   0,  20,  23,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 79 */
	{   3,           BU_O | NO_VEC,        A_NOM,     K_L,    0,   12,   24,     0, 0x00,   8,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  50,  34,    0, NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 7a */
	{   1,     ZU | NO_VEC | NO_ZU,    A_SPECIAL,     K_S,    0,    6,   20,     0, 0x1a,   8,  0x60,       3, SC200, SD_NOR_S,   0,  50,  23,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 7b: pl3a_at_plane, Paper Planes: Servbot and the planes (shl3a_02) */
	{   3,              ZU | NO_ZU,      A_HYPER,     K_L,    0,    2,   20,     0, 0x1a,   4,  0x60,       3, SC200, SD_NOR_L,   0,  90,  79,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   2,        3, { 0, 0 } },	/* 7c: pl3a_at_king_punch, King Kobun's punch (king_punch) and Rushing King Kobun's hits (rking_init, rking_run) */
	{   3,                      21,      A_HYPER,    0x40,    0,   16,   20,    20, 0x11,  10,  0x60,       3, SC200,        8,   0,  90,  65,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   2,        3, { 0, 0 } },	/* 7d: pl3a_at_king_dash, King Kobun's dash (king_dash, dash_ck) */
	{   2,             ZU | NO_VEC,    A_SPECIAL, K_L | 2,    0,    4,   20,     0, 0x18,   8,  0x80,       3, SC300, SD_NOR_L,   0,  20,  69,   70, NO_WEIGHT | AERIAL_FINISH,  1,     0,  0,     0,     0,   3, YK_GD060, { 0, 0 } },	/* 7e */
	{   2,             ZU | NO_VEC,    A_SPECIAL, K_L | 2,    0,    4,   20,     0, 0x18,   8,  0x80,       3, SC300, SD_NOR_L,   0,  20,  77,   78, NO_WEIGHT | AERIAL_FINISH,  1,     0,  0,     0,     0,   3, YK_GD060, { 0, 0 } },	/* 7f */
};
