/* Omega Red (character 30): the attacks (HIT_DATA, 0x1c bytes).
   What each hit does. The character's module sets the attack's number (atck_no) with the move,
   and the row is read when the attack box meets a body. After a row is its number, in
   hexadecimal, and the program's name for it:
   ATCK_ for the normals every character has (player.h), pl30_at_ for its own (pl30_def.h).
   The values' names are Capcom's own for these tables (hit_equ.h) and the program's (player.h).

     dmg     the damage
     type    the reaction: ZU pushed back, BU_O knocked off the feet, AS swept,
             or the number of another in pl_dmg.c's dm_tbl (here 2 dm_buttobi_ng, 6 dm_hold,
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
     box, box2  boxes of hit_30 that the computer measures the attack's reach by
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

const HIT_DATA hit_dt_30[] = {
	/* dmg                 type              kind  elem  piyo  stop  sand  sleep   vec  pow  range     mark  score        se  shc  jug  box  box2                     flag  13  guard  15  catch  abyss  cnt    answer        1a */
	{   4,                  ZU,            A_NOM,  K_S,    4,   10,   10,     0, 0x18,   4,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  15,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 00: ATCK_STAND_LP */
	{   7,       BU_O | NO_VEC,            A_NOM, K_LM,    2,   10,   16,     0, 0x1f,  11,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40, 162,   16, NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 01: ATCK_STAND_MP */
	{  13,                  ZU,            A_NOM,  K_L,    1,   10,   24,     0, 0x1a,   4,  0x80, MARK_N3, SC300,       18,   0,  50, 163,  112,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 02: ATCK_STAND_HP */
	{   4,                  ZU,            A_NOM, K_LS,    4,   10,   10,     0, 0x18,   2,  0x60, MARK_N1, SC100, SD_KIK_S,   0,  20,  18,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 03: ATCK_STAND_LK */
	{   7,                  ZU,            A_NOM,  K_M,    2,   10,   16,     0, 0x1a,   3,  0x60, MARK_N2, SC200, SD_KIK_M,   0,  40,  19,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 04: ATCK_STAND_MK */
	{  14,                  ZU,            A_NOM,  K_L,    1,   10,   24,     0, 0x1c,   4,  0x80, MARK_N3, SC300, SD_KIK_L,   0,  50,  20,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 05: ATCK_STAND_HK */
	{   4,                  ZU,            A_NOM, K_LS,    4,   10,   10,     0, 0x18,   6,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  65,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 06: ATCK_CROUCH_LP */
	{   7,                  ZU,            A_NOM, K_LM,    2,   10,   16,     0, 0x18,   8,  0x80, MARK_N2, SC200,       18,   0,  40,  66,  138,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 07: ATCK_CROUCH_MP */
	{   5,                  ZU,            A_NOM, K_LL,    1,    1,   24,     0, 0x1f,  11,  0x80, MARK_N3, SC300,       18,   0,  50,  67,  143,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 08: ATCK_CROUCH_HP */
	{   4,                  ZU,             A_LO, K_LS,    4,   10,   10,     0, 0x18,   8,  0x70, MARK_N1, SC100, SD_KIK_S,   0,  20,  68,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 09: ATCK_CROUCH_LK */
	{   7,                  ZU,             A_LO, K_LM,    2,   10,   16,     0, 0x18,   8,  0x40, MARK_N2, SC200, SD_KIK_M,   0,  40,  69,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0a: ATCK_CROUCH_MK */
	{  13,                  ZU,            A_NOM, K_LL,    1,   10,   24,     9, 0x18,   8,  0x70, MARK_N3, SC300,       18,   0,  50,  70,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0b: ATCK_CROUCH_HK */
	{   3,         ZU | NO_VEC,             A_HI,  K_S,    1,   10,   10,     0, 0x00,   5,  0x70, MARK_N1, SC100,       18,   0,  20,  71,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0c: ATCK_JUMP_LP */
	{   7,         ZU | NO_VEC,             A_HI,  K_M,    1,   10,   16,     0, 0x00,   5,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40, 160,   72,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0d: ATCK_JUMP_MP */
	{   5,         ZU | NO_VEC,             A_HI,  K_M,    1,    5,   24,     0, 0x17,   8,  0x80, MARK_N3, SC300,       18,   4,  60,  73,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0e: ATCK_JUMP_HP */
	{   4,         ZU | NO_VEC,             A_HI,  K_S,    1,   10,   10,     0, 0x00,   5,  0x60, MARK_N1, SC100, SD_KIK_S,   0,  20,  74,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0f: ATCK_JUMP_LK */
	{   7,         ZU | NO_VEC,             A_HI,  K_M,    1,   10,   16,     0, 0x00,   3,  0x60, MARK_N2, SC200, SD_KIK_M,   0,  40,  75,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 10: ATCK_JUMP_MK */
	{  13,         ZU | NO_VEC,             A_HI,  K_M,    1,   10,   24,     0, 0x16,   8,  0x80, MARK_N3, SC300, SD_KIK_L,   0,  60,  77,    0, NO_WEIGHT | AERIAL_FINISH,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 11: ATCK_JUMP_HK */
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
	{  12,       BU_O | NO_VEC, A_SPECIAL | 0x40,  K_L,    4,   20,   30,     0, 0x00,   7,  0xc0, MARK_N1, SC300,       79,   0,   0,   0,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 20 */
	{  12,       BU_O | NO_VEC, A_SPECIAL | 0x40,  K_L,    4,   20,   30,     0, 0x04,   7,  0xc0, MARK_N1, SC300,       79,   0,   0,   0,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 21 */
	{  12,       BU_O | NO_VEC, A_SPECIAL | 0x40,  K_L,    4,   20,   30,     0, 0x07,   7,  0xc0, MARK_N1, SC300,       79,   0,   0,   0,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 22 */
	{  12,       BU_O | NO_VEC, A_SPECIAL | 0x40,  K_L,    4,   20,   30,     0, 0x0c,   7,  0xc0, MARK_N1, SC300,       79,   0,   0,   0,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 23 */
	{  12,       BU_O | NO_VEC, A_SPECIAL | 0x40,  K_L,    4,   20,   30,     0, 0x10,   7,  0xc0, MARK_N1, SC300,       79,   0,   0,   0,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 24 */
	{  12,       BU_O | NO_VEC, A_SPECIAL | 0x40,  K_L,    4,   20,   30,     0, 0x1f,   7,  0xc0, MARK_N1, SC300,       79,   0,   0,   0,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 25 */
	{  16,       BU_O | NO_VEC, A_SPECIAL | 0x40,  K_L,    4,   20,   30,     0, 0x0c,   7,  0xc0, MARK_N1, SC300,       79,   0,   0,   0,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 26 */
	{  16,       BU_O | NO_VEC,     A_NOM | 0x40,  K_L,    4,   20,   20,     0, 0x04,   7,  0xc0, MARK_N1, SC300,       79,   0,   0,   0,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 27 */
	{  16,       BU_O | NO_VEC,     A_NOM | 0x40,  K_L,    4,   20,   20,     0, 0x1c,  11,  0xc0, MARK_N1, SC300,       79,   0,   0,   0,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 28 */
	{  16,       BU_O | NO_VEC,     A_NOM | 0x40,  K_L,    4,   20,   20,     0, 0x1c,   7,  0xc0, MARK_N1, SC300,       79,   0,   0,   0,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 29 */
	{  12,          2 | NO_VEC,   A_HYPER | 0x40,  K_L,    0,   20,   30,    30, 0x10,   7,  0xc0, MARK_N1, SC300,       79,   0,   0,   0,    0,    NO_WEIGHT | NO_SCALE,  0,     0, 51,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 2a: pl30_at_coil_slam, Carbonadium Coil: the caught opponent pulled in and slammed (shl30_01_chase_2) */
	{ 0 },	/* 2b */
	{ 0 },	/* 2c */
	{ 0 },	/* 2d */
	{ 0 },	/* 2e */
	{ 0 },	/* 2f */
	{  10,         10 | NO_VEC,        A_SPECIAL,  K_L,    0,   10,   10,     0, 0x1f,  10,  0x60,       3, SC200,       12,   0,  90,  78,    0, NO_CHIP | NO_WEIGHT | AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        7, { 0, 0 } },	/* 30: pl30_at_t_kick, the partner's jump-in attack (t_kick) */
	{   5,          2 | NO_VEC,          A_HYPER,  K_L,    0,    4,   10,     0, 0x1f,   4,  0x60,       3, SC200,       18,   0,  90,  27,   29,                       0,  1,     0, 27,     0,     0,   6,        3, { 0, 0 } },	/* 31 */
	{   5,          2 | NO_VEC,          A_HYPER,  K_L,    0,    4,   10,     0, 0x1f,   4,  0x60,       3, SC200,       18,   0,  90,  31,   32,                 NO_CHIP,  1,     0, 27,     0,     0,   6,        3, { 0, 0 } },	/* 32 */
	{   4,         ZU | NO_VEC,        A_SPECIAL, K_LS,    0,   12,    0,     0, 0x00,   0,  0x70, MARK_N2, SC300,       18, 136,  90,  64,   88,                       0,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 33 */
	{   4,         ZU | NO_VEC,        A_SPECIAL, K_LS,    0,   12,    0,     0, 0x00,   0,  0x70, MARK_N2, SC300,       18, 136,  90,  64,   92,                       0,  1,     0,  0,     0,     0,   0,        5, { 0, 0 } },	/* 34 */
	{   4,         ZU | NO_VEC,        A_SPECIAL, K_LS,    0,   12,    0,     0, 0x00,   0,  0x70, MARK_N2, SC300,       18, 136,  90,  64,   96,                       0,  1,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 35 */
	{   4,         ZU | NO_VEC,        A_SPECIAL, K_LS,    0,   12,    0,     0, 0x00,   0,  0x70, MARK_N2, SC300,       18, 136,  90,  64,  106,                       0,  1,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 36 */
	{   4,         ZU | NO_VEC,        A_SPECIAL, K_LS,    0,   12,    0,     0, 0x00,   0,  0x70, MARK_N2, SC300,       18, 136,  90,  64,  108,                       0,  1,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 37 */
	{  10,       BU_O | NO_VEC, A_SPECIAL | 0x40,  K_L,    0,   20,   40,     0, 0x00,   7,  0xc0, MARK_N2,    16,       79,   0,   0,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 38: pl30_at_coil_throw, Carbonadium Coil hold: the opponent hurt and thrown (shl30_01_catch_hold) */
	{  16,       BU_O | NO_VEC,        A_SPECIAL,  K_L,    0,   10,   10,     0, 0x1a,  15,  0x60,       3, SC200, SD_KIK_M,   0,  90,  79,  165,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        7, { 0, 0 } },	/* 39 */
	{  16,       BU_O | NO_VEC,        A_SPECIAL,  K_L,    0,   10,   10,     0, 0x1c,  14,  0x60,       3, SC200, SD_KIK_M,   0,  90,  82,  167,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 3a */
	{  16,       BU_O | NO_VEC,        A_SPECIAL,  K_L,    0,   10,   10,     0, 0x1f,  14,  0x60,       3, SC200, SD_KIK_M,   0,  90, 159,   85,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 3b */
	{   6,                  ZU,            A_NOM, K_LL,    0,   10,   24,     9, 0x18,   8,  0x70, MARK_N3, SC300,       18,   0,  90, 112,  163,                       0,  0,     0,  0,     0,     0,   0,        7, { 0, 0 } },	/* 3c: pl30_at_burrow, Burrowing Coil (gamma assist; him and shl30_02 type 1) */
	{   0,          6 | NO_VEC,          A_HYPER,  K_L,    0,   10,   10,     0, 0x1c,   4,  0x60,       3, SC200,       18,   0,  90, 169,    0,                   NO_KO,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 3d: pl30_at_jdestroyer, Omega Destroyer in the air: the dash (jdestroyer_dash_init) */
	{   5,          6 | NO_VEC,          A_HYPER,  K_S,    0,    4,   10,     0, 0x1c,   4,  0x60,       3, SC200, SD_NOR_M,   0,  90,   0,    0,                   NO_KO,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 3e */
	{   5,          6 | NO_VEC,          A_HYPER,  K_L,    0,    4,   10,     0, 0x1c,   4,  0x60,       3, SC200, SD_NOR_M,   0,  90,   0,    0,                   NO_KO,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 3f */
	{   5,          6 | NO_VEC,          A_HYPER,  K_S,    0,    6,   10,     0, 0x1c,   4,  0x60,       3, SC200, SD_NOR_M,   0,  90,   0,    0,                   NO_KO,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 40 */
	{   5,          6 | NO_VEC,          A_HYPER,  K_L,    0,    6,   10,     0, 0x1c,   4,  0x60,       3, SC200, SD_NOR_M,   0,  90,   0,    0,                   NO_KO,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 41 */
	{  10,         12 | NO_VEC,        A_SPECIAL,  K_L,    0,    4,   24,     0, 0x1c,  12,  0x80,       3, SC300,       18,  64,  90, 112,  163,               NO_WEIGHT,  0,     0,  0,     0,     0,   0,        7, { 0, 0 } },	/* 42: pl30_at_snapback, Snapback */
	{  14, ZU | NO_VEC | NO_ZU,        A_SPECIAL,  K_L,    0,   10,   10,     0, 0x1c,   4,  0x60,       3, SC200,       18,  64,  90, 112,  163,                       0,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 43: pl30_at_counter_a, Alpha type's Variable Counter */
	{  14,                  ZU,            A_NOM, K_LL,    0,    1,   24,     0, 0x1f,  11,  0x80, MARK_N3, SC300,       18,  64,  90,  67,  143,                       0,  0,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 44: pl30_at_counter_g, Gamma type's Variable Counter */
	{  14,       BU_O | NO_VEC,        A_SPECIAL, K_LM,    0,   10,   16,     0, 0x1f,  11,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40, 162,   16, NO_CHIP | NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 45: pl30_at_upper, the launcher (pl30_upper) */
	{  16,       BU_O | NO_VEC,        A_SPECIAL,  K_L,    0,   10,   10,     0, 0x1c,  14,  0x60,       3, SC200, SD_KIK_M,  64,  90,  82,  167,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 46 */
};
