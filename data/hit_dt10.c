/* Tron Bonne (character 10): the attacks (HIT_DATA, 0x1c bytes).
   What each hit does. The character's module sets the attack's number (atck_no) with the move,
   and the row is read when the attack box meets a body. After a row is its number, in
   hexadecimal, and the program's name for it:
   ATCK_ for the normals every character has (player.h), pl10_at_ for its own (pl10_def.h).
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
     box, box2  boxes of hit_10 that the computer measures the attack's reach by
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

const HIT_DATA hit_dt_10[] = {
	/* dmg                 type          kind      elem  piyo  stop  sand  sleep   vec  pow  range     mark  score        se  shc  jug  box  box2                     flag  13  guard  15  catch  abyss  cnt    answer        1a */
	{   5,                  ZU,        A_NOM,      K_S,    4,   12,   10,     0, 0x18,   4,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  15,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 00: ATCK_STAND_LP */
	{   3,                  ZU,        A_NOM,     K_LM,    2,    8,   16,     0, 0x18,   6,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40,  16,   17,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 01: ATCK_STAND_MP */
	{  14,                BU_O,        A_NOM,      K_L,    1,   18,   24,     0, 0x1c,  10,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  30,  28,  155,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 02: ATCK_STAND_HP */
	{   4,                  ZU,        A_NOM,      K_S,    4,   12,   28,     0, 0x1e,   4,  0x60, MARK_N1, SC100, SD_KIK_S,   0,  90,  18,   52,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 03: ATCK_STAND_LK */
	{   4,                  ZU,        A_NOM,  K_M | 2,    2,   10,   24,     0, 0x1e,   4,  0x60, MARK_N2, SC200, SD_KIK_M,   0,  30,  24,   25,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 04: ATCK_STAND_MK */
	{   8,                BU_O,        A_NOM,      K_M,    1,   12,   50,     0, 0x1e,   9,  0x60, MARK_N3, SC300, SD_KIK_L,   0,  50,  58,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 05: ATCK_STAND_HK */
	{   5,                  ZU,        A_NOM,     K_LS,    4,   12,   10,     0, 0x1c,   4,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  30,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 06: ATCK_CROUCH_LP */
	{   5,                  ZU,        A_NOM,     K_LM,    2,    8,   16,     0, 0x13,   6,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40,  79,    0,                       0,  0,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 07: ATCK_CROUCH_MP */
	{  14,       BU_O | NO_VEC,        A_NOM,     K_LL,    1,   14,   24,     0, 0x00,  11,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  50,  32,   34, NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 08: ATCK_CROUCH_HP */
	{   5,                  ZU,         A_LO,     K_LS,    4,   12,   10,     0, 0x1c,   4,  0x70, MARK_N1, SC100, SD_KIK_S,   0,  20,  48,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 09: ATCK_CROUCH_LK */
	{   8,       BU_O | NO_VEC,        A_NOM,     K_LM,    2,   16,   16,     0, 0x1f,   8,  0x40, MARK_N2, SC200, SD_KIK_M,   0,  40,  68,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0a: ATCK_CROUCH_MK */
	{  14,                  AS,         A_LO,     K_LL,    1,   18,   24,     9, 0x1a,   8,  0x70, MARK_N3, SC300, SD_KIK_L,   0, 100,  32,  140,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0b: ATCK_CROUCH_HK */
	{   5,         ZU | NO_VEC,         A_HI,      K_S,    1,   12,   10,     0, 0x00,   5,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  56,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0c: ATCK_JUMP_LP */
	{   8,         ZU | NO_VEC,         A_HI, K_LM | 3,    1,   16,   16,     0, 0x00,   2,  0x80, MARK_N2, SC200,       22,   0,  40,  40,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0d: ATCK_JUMP_MP */
	{  14,         ZU | NO_VEC,         A_HI,     K_LL,    1,   12,   32,     0, 0x18,   8,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  60, 134,  137,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0e: ATCK_JUMP_HP */
	{   5,         ZU | NO_VEC,         A_HI,      K_S,    1,   12,   10,     0, 0x00,   5,  0x60, MARK_N1, SC200, SD_KIK_S,   0,  20,  59,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0f: ATCK_JUMP_LK */
	{   5,         ZU | NO_VEC,         A_HI,      K_M,    1,   10,   16,     0, 0x00,   2,  0x60, MARK_N2, SC200, SD_KIK_M,   0,  40,  81,  123,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 10: ATCK_JUMP_MK */
	{  14,         ZU | NO_VEC,         A_HI,      K_L,    1,   18,   32,     0, 0x15,   8,  0x60, MARK_N3, SC200, SD_KIK_L,   0,  60,  65,    0,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 11: ATCK_JUMP_HK */
	{ 0 },	/* 12 */
	{ 0 },	/* 13 */
	{ 0 },	/* 14 */
	{ 0 },	/* 15 */
	{  14,       BU_O | NO_VEC,    A_SPECIAL,     K_LL,    0,   18,   24,     0, 0x00,  11,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  40,   8,   40, NO_CHIP | NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 16: pl10_at_upper, the launcher (upper_init, crouching, Char_Extra 3) */
	{ 0 },	/* 17 */
	{ 0 },	/* 18 */
	{   4,                  AS,        A_NOM,      K_S,    0,   12,   28,     0, 0x1e,   4,  0x60, MARK_N1, SC100, SD_KIK_S,   0,  20,  18,   52,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 19: pl10_at_stand_lk_2, Standing LK's second hit (stand_kick_move, nml_no 0) */
	{ 0 },	/* 1a */
	{ 0 },	/* 1b */
	{ 0 },	/* 1c */
	{ 0 },	/* 1d */
	{ 0 },	/* 1e */
	{ 0 },	/* 1f */
	{  16,                   2, A_NOM | 0x40,     K_LM,    4,   10,   20,     0, 0x1c,  10,  0x60, MARK_N2, SC200, SD_NOR_M,   0,  40,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 20: pl10_at_throw_p, Ground throw (nml_p_catch) */
	{  16,                   2, A_NOM | 0x40,     K_LM,    4,   10,   20,     0, 0x1c,  10,  0x60, MARK_N2, SC200, SD_NOR_M,   0,  40,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 21: pl10_at_air_throw, Air throw (jump_p_throw) */
	{  14,                   2, A_NOM | 0x40,     K_LM,    4,    0,   20,     0, 0x1c,  10,  0x60, MARK_N2, SC200, SD_NOR_M,   0,  40,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 22: pl10_at_assist_throw, the assist's throw (throw_throw, pl10_sp_throw) */
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
	{  10, ZU | NO_VEC | NO_ZU,        A_NOM,     K_LM,    1,   10,   24,     0, 0x1a,   4,  0x60, MARK_N3, SC200, SD_NOR_M,   0,  50,  63,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 30 */
	{  15, ZU | NO_VEC | NO_ZU,    A_SPECIAL,      K_M,    0,   10,   10,     0, 0x1c,  11,  0x60,       3, SC200,        8,   0,  90,  83,   85,                       0,  1,     0,  0,     0,     0,   0,        5, { 0, 0 } },	/* 31: pl10_at_kobun, Kobun Launch (kobun_init) */
	{  10,         10 | NO_VEC,    A_SPECIAL,      K_L,    0,   10,   10,     0, 0x1f,  10,  0x60,       3, SC200,       12,   0,  90,  65,    0, NO_CHIP | NO_WEIGHT | AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        5, { 0, 0 } },	/* 32: pl10_at_t_kick, the jump-in attack when she is tagged in */
	{   1,          ZU | NO_ZU,      A_HYPER,      K_M,    0,   10,   30,     0, 0x18,   9,  0x80,       3, SC300,        8,   0, 100,  87,    0,     NO_CHIP | NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 33 */
	{   1,          ZU | NO_ZU,      A_HYPER,     K_LM,    0,   10,   30,     0, 0x18,   9,  0x80,       3, SC300,        8,   0, 100,  87,    0,     NO_CHIP | NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 34 */
	{   1,          ZU | NO_ZU,      A_HYPER,      K_M,    0,   10,   30,     0, 0x18,   9,  0x80,       3, SC300,        8,   0, 100,  87,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 35 */
	{   1,          ZU | NO_ZU,      A_HYPER,     K_LM,    0,   10,   30,     0, 0x18,   9,  0x80,       3, SC300,        8,   0, 100,  87,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 36 */
	{   3,         ZU | NO_VEC,    A_SPECIAL,     K_LS,    0,    4,   30,     0, 0x1a,   0,  0x80,       3, SC300,        8,   0, 100, 104,  108,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 37: pl10_at_strike, Bonne Strike's rolling hits on the ground; mashing speeds them up (strike_init, strike_renda) */
	{   3,         ZU | NO_VEC,    A_SPECIAL,      K_S,    0,    4,   30,     0, 0x1e,   4,  0x80,       3, SC300,        8,   0, 100, 115,  119,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 38: pl10_at_strike_air, Bonne Strike's rolling hits in the air (strike_air_init, strike_renda) */
	{   5,       BU_O | NO_VEC,      A_HYPER,      K_L,    0,   16,   60,    30, 0x1c,  10,  0x60,       3, SC300,        8,   0,  90,  95,   97, NO_CHIP | NO_WEIGHT | NO_KO,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 39: pl10_at_king, King Kobun (hyper): start, and the big Servbot's first hit (king_init, pls10_03_king_wait) */
	{   7,          ZU | NO_ZU,      A_HYPER,      K_M,    0,   10,   24,     0, 0x1a,   4,  0x60,       3, SC200, SD_NOR_L,   0,  90,  99,    0,               NO_WEIGHT,  1,     0, 49,     0,     0,   0,        3, { 0, 0 } },	/* 3a: pl10_at_king_2, King Kobun: the grown Servbot's run and later hits (pls10_03_king_grow, pls10_03_king_rush) */
	{  20,                  ZU,    A_SPECIAL,  K_M | 2,    0,   10,   16,     0, 0x1a,   4,  0x60, MARK_N2, SC200, SD_KIK_M,   0,  30,  25,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 3b: pl10_at_kick, the kick assist (pl10_sp_kick) */
	{  15,                  ZU,    A_SPECIAL,     K_LM,    0,   11,   16,     0, 0x13,   6,  0x80,       3, SC200, SD_NOR_M,   0,  40,  79,    0,                       0,  0,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 3c: pl10_at_fire, Gustaff's fire assist (pl10_sp_fire) */
	{   2,         21 | NO_VEC,      A_HYPER,      K_L,    0,   16,   60,    30, 0x1f,  13,  0x60,       3, SC300,        8,   0,  90,  77,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 3d: pl10_at_lunch, Lunch Rush (hyper): the short beam (lunch_init) */
	{  10,           2 | NO_ZU,      A_HYPER,     K_LM,    0,   10,   30,     0, 0x1e,   9,  0x80,       3, SC300,        8,   0, 100,  87,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 3e */
	{  10, ZU | NO_VEC | NO_ZU,    A_SPECIAL,      K_L,    0,    0,    0,     0, 0x1a,   1,  0x80,       3, SC100,        0,   0,  90,  93,    0,                       0,  0,     0,  0,     1,     0,   0,        4, { 0, 0 } },	/* 3f: pl10_at_beacon, Beacon Bomb: Tron and the claw that catches (beacon_init, shl10_04_catch_init) */
	{  10,                BU_O,      A_HYPER, K_LL | 2,    0,   10,   30,     0, 0x1b,  11,  0x60,       3, SC200,        8,   0,  90,  93,    0,     NO_CHIP | NO_WEIGHT,  1,     0,  4,     0,     0,   0,        5, { 0, 0 } },	/* 40 */
	{   6,       BU_O | NO_VEC,    A_SPECIAL,     K_LL,    0,   12,   30,     0, 0x1a,  12,  0x80,       3, SC300,        8,   0, 100, 104,  108,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 41: pl10_at_strike_last, Bonne Strike's last hit on the ground (strike_rush, timer0 2) */
	{   6,       BU_O | NO_VEC,    A_SPECIAL,     K_LM,    0,    7,   30,     0, 0x1e,  12,  0x80,       3, SC300,        8,   0, 100, 115,  119,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 42: pl10_at_strike_air_last, Bonne Strike's last hit in the air (strike_air_rush) */
	{  10,         12 | NO_VEC,    A_SPECIAL,      K_L,    0,    4,   24,     0, 0x1c,  12,  0x80,       3, SC300,        8,  64,  90,  28,  155,               NO_WEIGHT,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 43: pl10_at_snapback, Snapback */
};
