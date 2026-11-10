/* Captain Commando (character 38): the attacks (HIT_DATA, 0x1c bytes).
   What each hit does. The character's module sets the attack's number (atck_no) with the move,
   and the row is read when the attack box meets a body. After a row is its number, in
   hexadecimal, and the program's name for it:
   ATCK_ for the normals every character has (player.h), pl38_at_ for its own (pl38_def.h).
   The values' names are Capcom's own for these tables (hit_equ.h) and the program's (player.h).

     dmg     the damage
     type    the reaction: ZU pushed back, BU_O knocked off the feet, AS swept,
             or the number of another in pl_dmg.c's dm_tbl (here 2 dm_buttobi_ng, 10 dm_away,
             12 dm_snap, 13 dm_zuri, 14 dm_nop).
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
     box, box2  boxes of hit_38 that the computer measures the attack's reach by
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

const HIT_DATA hit_dt_38[] = {
	/* dmg                     type          kind      elem  piyo  stop  sand  sleep   vec  pow  range     mark  score        se  shc  jug  box  box2                     flag  13  guard  15  catch  abyss  cnt    answer        1a */
	{   4,                      ZU,        A_NOM,      K_S,    4,   10,   10,     0, 0x18,   4,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,   8,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 00: ATCK_STAND_LP */
	{   7,                      ZU,        A_NOM,     K_LM,    2,   11,   16,     0, 0x1e,   6,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40,  10,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 01: ATCK_STAND_MP */
	{  13,                    BU_O,        A_NOM,  K_L | 2,    1,   10,   24,     0, 0x1a,   9,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  50,  12,   42,                       0,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 02: ATCK_STAND_HP */
	{   4,                      ZU,        A_NOM,     K_LS,    4,   10,   10,     0, 0x18,   4,  0x60, MARK_N1, SC100, SD_KIK_S,   0,  20,  14,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 03: ATCK_STAND_LK */
	{   7,             ZU | NO_VEC,        A_NOM,     K_LM,    2,   11,   16,     0, 0x1a,   9,  0x60, MARK_N2, SC200, SD_KIK_M,   0,  40,  16,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 04: ATCK_STAND_MK */
	{  13,            BU_O | NO_ZU,        A_NOM,     K_LL,    0,    4,   24,     0, 0x00,   3,  0x80, MARK_N3, SC300, SD_KIK_L,   0,  30,  20,   92,                       0,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 05: ATCK_STAND_HK */
	{   4,                      ZU,        A_NOM,     K_LS,    4,   10,   10,     0, 0x1c,   4,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  22,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 06: ATCK_CROUCH_LP */
	{   6,           BU_O | NO_VEC,        A_NOM,     K_LM,    2,   10,   16,     0, 0x1f,  10,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40,  26,   24, NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 07: ATCK_CROUCH_MP */
	{  13,                      ZU,        A_NOM, K_LL | 3,    1,   12,   24,     0, 0x1a,   6,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  50,  28,    0,                       0,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 08: ATCK_CROUCH_HP */
	{   4,                      ZU,         A_LO,     K_LS,    4,   10,   10,     0, 0x1c,   4,  0x70, MARK_N1, SC100, SD_KIK_S,   0,  20,  32,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 09: ATCK_CROUCH_LK */
	{   7,                      ZU,         A_LO,     K_LM,    2,   11,   16,     0, 0x1d,   6,  0x40, MARK_N2, SC200, SD_KIK_M,   0,  40,  34,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0a: ATCK_CROUCH_MK */
	{  13,                      AS,         A_LO,     K_LL,    1,   12,   24,     3, 0x1a,   8,  0x70, MARK_N3, SC300, SD_KIK_L,   0,  90,  36,    0,                       0,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 0b: ATCK_CROUCH_HK */
	{   4,             ZU | NO_VEC,         A_HI,      K_S,    1,   10,   10,     0, 0x00,   5,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  40,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0c: ATCK_JUMP_LP */
	{   7,             ZU | NO_VEC,         A_HI,     K_LM,    1,   10,   16,     0, 0x00,   6,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40,  45,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0d: ATCK_JUMP_MP */
	{  13,             ZU | NO_VEC,         A_HI,  K_M | 3,    1,   12,   32,     0, 0x16,   8,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  60, 158,   47,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 0e: ATCK_JUMP_HP */
	{   4,             ZU | NO_VEC,         A_HI,      K_S,    1,   10,   10,     0, 0x00,   5,  0x60, MARK_N1, SC100, SD_KIK_S,   0,  20,  49,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0f: ATCK_JUMP_LK */
	{   7,             ZU | NO_VEC,         A_HI,      K_M,    1,   10,   16,     0, 0x00,   6,  0x60, MARK_N2, SC200, SD_KIK_M,   0,  40,  51,  141,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 10: ATCK_JUMP_MK */
	{  13,             ZU | NO_VEC,         A_HI,      K_L,    1,   12,   32,     0, 0x16,   8,  0x80, MARK_N3, SC300, SD_KIK_L,   0,  60,  53,   43,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 11: ATCK_JUMP_HK */
	{ 0 },	/* 12 */
	{ 0 },	/* 13 */
	{ 0 },	/* 14 */
	{ 0 },	/* 15 */
	{ 0 },	/* 16 */
	{ 0 },	/* 17 */
	{ 0 },	/* 18 */
	{  12,   BU_O | NO_VEC | NO_ZU,        A_NOM,      K_L,    1,   10,   24,     0, 0x00,  11,  0x60, MARK_N3, SC300, SD_KIK_L,   0,  50,  20,   92, NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 19: pl38_at_stand_hk, Standing HK has its own record instead of 5 */
	{ 0 },	/* 1a */
	{ 0 },	/* 1b */
	{ 0 },	/* 1c */
	{ 0 },	/* 1d */
	{ 0 },	/* 1e */
	{ 0 },	/* 1f */
	{   4,                       2, A_NOM | 0x40,     K_LM,    4,   12,   20,     0, 0x1f,  11,  0x60, MARK_N2, SC200, SD_NOR_M,   0,  40,   0,    0, NO_WEIGHT | NO_COMBO | NO_KO,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 20: pl38_at_throw_p, Ground punch throw's grab (catch_no 0, 4, 5) */
	{  16,                       2, A_NOM | 0x40,     K_LM,    4,   12,   20,     0, 0x0c,  11,  0x60, MARK_N2, SC300, SD_NOR_M,   0,  40,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 21: pl38_at_air_throw, Air throw (jump_p_catch) */
	{   0,                    BU_O, A_NOM | 0x40,     K_LM,    4,   12,   20,     0, 0x00,   2,  0x60, MARK_N2, SC300,       22,   0,  40,   0,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 22 */
	{  16,                    BU_O, A_NOM | 0x40,  K_L | 3,    4,   10,   30,     0, 0x1e,   2,  0x80, MARK_N3, SC300, SD_KIK_L,   0,  30,   0,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 23: pl38_at_throw_k, Kick throw's mashed hold and last throw (catch_no 2) */
	{  15,                    BU_O, A_NOM | 0x40,  K_L | 3,    4,   10,   20,     0, 0x1c,   8,  0x60,       3, SC300, SD_KIK_M,   4,  40,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 24 */
	{  12,            BU_O | NO_ZU,        A_NOM,      K_M,    2,   10,   20,     0, 0x1f,  15,  0x80, MARK_N2, SC300, SD_NOR_M,   0,  40,  24,   26, NO_WEIGHT | NO_COMBO | NO_KO,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 25 */
	{   4,            BU_O | NO_ZU,        A_NOM,      K_M,    2,   10,   10,     0, 0x1f,   4,  0x80, MARK_N2, SC300, SD_NOR_M,   4,  60,  24,   26,               NO_WEIGHT,  1,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 26: pl38_at_throw_prop, the helper's hits in the punch throw of catch_no 4 (shl38_00 type 16) */
	{ 0 },	/* 27 */
	{ 0 },	/* 28 */
	{ 0 },	/* 29 */
	{ 0 },	/* 2a */
	{ 0 },	/* 2b */
	{ 0 },	/* 2c */
	{ 0 },	/* 2d */
	{ 0 },	/* 2e */
	{  45,      2 | NO_VEC | NO_ZU,      A_HYPER,      K_L,    0,    4,   30,     0, 0x1b,  13,  0x60,       3, SC300,       18,   0,  50,  65,   67,                 NO_CHIP,  1,     0, 11,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 2f */
	{  16,   BU_O | NO_VEC | NO_ZU,    A_SPECIAL,  K_L | 2,    0,    8,   20,     0, 0x1b,   8,  0x60,       3, SC300,        8,   0,  50, 109,   88,                       0,  1,     0,  0,     0,     0,   3,        8, { 0, 0 } },	/* 30: pl38_at_fire_l, Captain Fire on the ground, light */
	{  22,            BU_O | NO_ZU,    A_SPECIAL,  K_L | 3,    0,   12,   20,     0, 0x1e,  13,  0x60,       3, SC300,        8,   0,  90, 121,   55,                       0,  1,     0,  0,     0,     0,   5,        4, { 0, 0 } },	/* 31: pl38_at_corridor_l, Captain Corridor, light */
	{  10,             10 | NO_VEC,    A_SPECIAL,      K_L,    0,   10,   10,     0, 0x1f,  10,  0x60,       3, SC200,       12,   0,  90, 103,    0, NO_CHIP | NO_WEIGHT | AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        7, { 0, 0 } },	/* 32: pl38_at_t_kick, Partner jump-in attack */
	{  14,   BU_O | NO_VEC | NO_ZU,    A_SPECIAL,        2,    0,    6,   20,     0, 0x1c,   6,  0x60,       3, SC300,        8,   0,  50,  88,  109,                       0,  1,     0,  0,     0,     0,   3,        8, { 0, 0 } },	/* 33: pl38_at_jfire_l, Captain Fire in the air, light */
	{   2,             ZU | NO_VEC,      A_HYPER,      K_L,    0,    4,   30,     0, 0x1d,   8,  0x60,       6, SC300,       17,   0,  70,  63,   67,                       0,  1,     0, 11,     0,     0,  20,        8, { 0, 0 } },	/* 34: pl38_at_sword, Captain Sword: Commando and shl38_02 */
	{   2,             ZU | NO_VEC,      A_HYPER,      K_L,    0,    4,   30,     0, 0x1c,   2,  0x60,       6, SC300,       17,   0,  70,  65,   67,                       0,  1,     0, 11,     0,     0,  20,        8, { 0, 0 } },	/* 35 */
	{   2,             ZU | NO_VEC,      A_HYPER,      K_L,    0,    4,   30,     0, 0x16,  15,  0x60,       6, SC300,       17,   0,  70,  65,   67,                 NO_CHIP,  1,     0, 11,     0,     0,  20,        8, { 0, 0 } },	/* 36 */
	{   2,             ZU | NO_VEC,      A_HYPER,      K_L,    0,    4,   30,     0, 0x15,  15,  0x60,       6, SC300,       18,   0,  70,  65,   67,                       0,  1,     0, 11,     0,     0,  20,        8, { 0, 0 } },	/* 37 */
	{   2,             ZU | NO_VEC,      A_HYPER,      K_L,    0,    4,   30,     0, 0x19,  15,  0x60,       6, SC300,       18,   0,  70,  65,   67,                       0,  1,     0, 11,     0,     0,  20,        8, { 0, 0 } },	/* 38 */
	{   2,             ZU | NO_VEC,      A_HYPER,      K_L,    0,    4,   30,     0, 0x19,  15,  0x60,       6, SC300,       18,   0,  70,  65,   67,                 NO_CHIP,  1,     0, 11,     0,     0,  20,        8, { 0, 0 } },	/* 39 */
	{   2,             ZU | NO_VEC,      A_HYPER,      K_L,    0,    4,   30,     0, 0x19,  15,  0x60,       6, SC300,       18,   0,  70,  65,   67,                       0,  1,     0, 11,     0,     0,  20,        8, { 0, 0 } },	/* 3a */
	{   2,             ZU | NO_VEC,      A_HYPER,      K_L,    0,    4,   30,     0, 0x00,   1,  0x60,       6, SC300,       18,   0,  70,  65,   67,                       0,  1,     0, 11,     0,     0,  20,        8, { 0, 0 } },	/* 3b */
	{  36, 2 | NO_VEC | FLY_SCREEN,      A_HYPER,      K_L,    0,    4,   30,     0, 0x1c,  15,  0x60,       6, SC300,       18,   0,  70,  65,   67,                 NO_CHIP,  1,     0, 11,     0,     0,  20,        8, { 0, 0 } },	/* 3c */
	{   2,      2 | NO_VEC | NO_ZU,      A_HYPER,     K_LL,    0,    7,   20,     0, 0x00,  11,  0x60,       3, SC300,        8,   0,  90,  69,   72,       NO_WEIGHT | NO_KO,  1,     0, 12,     0,     0,   0,        5, { 0, 0 } },	/* 3d: pl38_at_storm, Captain Storm's rush, up to five hits */
	{   6,     10 | NO_VEC | NO_ZU,      A_HYPER,     K_LL,    0,   10,   20,     0, 0x00,   1,  0x60,       3, SC200, SD_NOR_L,   0,  70,  79,    0,       NO_WEIGHT | NO_KO,  1,     0, 12,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 3e: pl38_at_storm_helper_1, Captain Storm: helper type 1 flying across the opponent (shl38_01 type 1) */
	{   6,     10 | NO_VEC | NO_ZU,      A_HYPER,     K_LL,    0,   10,   20,     0, 0x00,   1,  0x60,       3, SC200, SD_KIK_L,   0,  70,  78,   80,       NO_WEIGHT | NO_KO,  1,     0, 12,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 3f: pl38_at_storm_helper, Captain Storm: helper types 0 and 2 flying across the opponent */
	{  20,               2 | NO_ZU,      A_HYPER,        3,    0,   12,   20,    40, 0x01,  15,  0x60,       3, SC300,        8,   0,  90, 121,   55,                       0,  1,     0, 12,     0,     0,   5, YK_GD030, { 0, 0 } },	/* 40: pl38_at_storm_meet, Captain Storm when partner_get finds a partner of the same character (storm_meet) */
	{  12,              13 | NO_ZU,    A_SPECIAL,     K_LL,    0,    4,   20,    20, 0x1f,  12,  0x60,       3, SC200, SD_NOR_M,   4,  50,  81,    0,                NO_COMBO,  1,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 41 */
	{   5,            BU_O | NO_ZU,    A_SPECIAL,      K_L,    0,    8,   24,     0, 0x1a,   6,  0x70,      11, SC300,       15,   4,  20,  86,    0,                       0,  0,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 42: pl38_at_strike_l, Commando Strike, strike_level 0: Commando and the helper (shl38_00 type 5) */
	{   6,               2 | NO_ZU,    A_SPECIAL,     K_LL,    0,   10,   10,     0, 0x1c,   8,  0x70,       3, SC300, SD_NOR_L,   6,  20,  87,  124,                       0,  0,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 43: pl38_at_strike_h, Commando Strike, strike_level 1: Commando and the helper (shl38_00 type 6) */
	{  22,               2 | NO_ZU,    A_SPECIAL,        2,    0,    4,   12,     3, 0x1b,  10,  0x70,       3, SC300,        8,   0,  20,  82,    0,                       0,  0,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 44: pl38_at_strike_x, Commando Strike, strike_level 2 (the HP+LK command): Commando, the helper and its shot (shl38_00 types 7 and 9) */
	{  12,            BU_O | NO_ZU,    A_SPECIAL,        2,    0,    4,   10,     0, 0x1c,   4,  0x60,       3, SC200,        8,   4,  50,   0,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 45: pl38_at_throw_p_toss2, Ground throw catch_no 5: tossing the opponent (catch5_toss) */
	{  12,            BU_O | NO_ZU,    A_SPECIAL,     K_LL,    0,   16,   10,     0, 0x1c,   8,  0x60,       3, SC200, SD_NOR_L,   0,  50,   0,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 46: pl38_at_throw_p_toss, Ground throw catch_no 0: throwing the opponent down (catch0_toss) */
	{  10,             14 | NO_VEC,    A_SPECIAL,      K_L,    0,   16,   10,    30, 0x02,   9,  0x60,       3, SC200,       12,   0,  90, 103,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 47 */
	{   2,            BU_O | NO_ZU,    A_SPECIAL,  K_L | 3,    0,    4,   10,     0, 0x1e,  12,  0x60,       3, SC300, SD_NOR_M,   0,  50,   0,   97,                       0,  1,     0,  0,     0,     0,   5,        4, { 0, 0 } },	/* 48 */
	{  16,   BU_O | NO_VEC | NO_ZU,    A_SPECIAL,  K_L | 2,    0,    8,   20,     0, 0x1b,   8,  0x60,       3, SC300,        8,   0,  50,  88,  109,                       0,  1,     0,  0,     0,     0,   3,        8, { 0, 0 } },	/* 49 */
	{  16,      2 | NO_VEC | NO_ZU,    A_SPECIAL,  K_L | 2,    0,    8,   20,     0, 0x1b,   8,  0x60,       3, SC300,        8,   0,  50,  88,  109,                       0,  1,     0,  0,     0,     0,   3,        8, { 0, 0 } },	/* 4a: pl38_at_fire_h, Captain Fire on the ground, heavy */
	{  12,   BU_O | NO_VEC | NO_ZU,    A_SPECIAL,        2,    0,    8,   20,     0, 0x1c,   6,  0x60,       3, SC300,        8,   0,  50,  88,  109,                       0,  1,     0,  0,     0,     0,   3,        8, { 0, 0 } },	/* 4b */
	{  12,   BU_O | NO_VEC | NO_ZU,    A_SPECIAL,        2,    0,    8,   20,     0, 0x1c,   6,  0x60,       3, SC300,        8,   0,  50,  88,  109,                       0,  1,     0,  0,     0,     0,   3,        8, { 0, 0 } },	/* 4c: pl38_at_jfire_h, Captain Fire in the air, heavy */
	{  22,            BU_O | NO_ZU,    A_SPECIAL,  K_L | 3,    0,   12,   20,     0, 0x1e,  13,  0x60,       3, SC300,        8,   0,  90,  56,    0,                       0,  1,     0,  0,     0,     0,   5,        4, { 0, 0 } },	/* 4d */
	{  22,            BU_O | NO_ZU,    A_SPECIAL,  K_L | 3,    0,   12,   20,     0, 0x1e,  13,  0x60,       3, SC300,        8,   0,  90,  56,    0,                       0,  1,     0,  0,     0,     0,   5,        4, { 0, 0 } },	/* 4e: pl38_at_corridor_h, Captain Corridor, heavy */
	{   4,            BU_O | NO_ZU,    A_SPECIAL,  K_L | 3,    0,    4,   20,     0, 0x1d,  12,  0x60,       3, SC200,        8,   0,  50,   0,   97,                       0,  1,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 4f */
	{   6,            BU_O | NO_ZU,    A_SPECIAL,  K_L | 3,    0,    4,   20,     0, 0x1d,  12,  0x60,       3, SC200, SD_NOR_M,   0,  50,   0,   97,                       0,  1,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 50 */
	{   6,   BU_O | NO_VEC | NO_ZU,    A_SPECIAL,      K_L,    0,    6,   32,     0, 0x1c,   6,  0x60,       3, SC300, SD_KIK_L,   8,  90, 134,    0,       NO_WEIGHT | NO_KO,  1,     0,  0,     0,     0,   0,        5, { 0, 0 } },	/* 51 */
	{   6,   BU_O | NO_VEC | NO_ZU,    A_SPECIAL,      K_L,    0,    6,   32,     0, 0x1d,   6,  0x60,       3, SC300, SD_KIK_L,   8,  90, 134,    0,       NO_WEIGHT | NO_KO,  1,     0,  0,     0,     0,   0,        5, { 0, 0 } },	/* 52 */
	{   6,   BU_O | NO_VEC | NO_ZU,    A_SPECIAL,      K_L,    0,    6,   32,     0, 0x1e,   6,  0x60,       3, SC300, SD_KIK_L,   8,  90, 134,    0,       NO_WEIGHT | NO_KO,  1,     0,  0,     0,     0,   0,        5, { 0, 0 } },	/* 53 */
	{  16,              2 | NO_VEC,    A_SPECIAL,     0x42,    0,   12,   20,     0, 0x19,  14,  0x60,       3, SC300,       12,   8,  90, 137,  154,                       0,  1,     0,  0,     0,     0,   0,        5, { 0, 0 } },	/* 54 */
	{  16,              2 | NO_VEC,    A_SPECIAL,     0x42,    0,   12,   20,     0, 0x19,  14,  0x60,       3, SC300,       12,   8,  90, 137,    0,                       0,  1,     0,  0,     0,     0,   0,        5, { 0, 0 } },	/* 55 */
	{  16,              2 | NO_VEC,    A_SPECIAL,     0x42,    0,   12,   20,     0, 0x19,  14,  0x60,       3, SC300,       12,   8,  90, 137,  154,                       0,  1,     0,  0,     0,     0,   0,        5, { 0, 0 } },	/* 56 */
	{  10,             12 | NO_VEC,    A_SPECIAL,  K_L | 2,    0,    4,   24,     0, 0x1c,  12,  0x80,       3, SC300,        8,  64,  90,  12,   42,               NO_WEIGHT,  1,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 57: pl38_at_snapback, Snapback */
	{  15,   BU_O | NO_VEC | NO_ZU,    A_SPECIAL,      K_L,    0,   10,   24,     0, 0x1f,  10,  0x60, MARK_N3, SC300, SD_NOR_L,   0,  40, 160,  161, NO_CHIP | NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 58: pl38_at_upper, Launcher (pl38_upper) */
};
