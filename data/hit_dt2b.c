/* Sabretooth (character 2b): the attacks (HIT_DATA, 0x1c bytes).
   What each hit does. The character's module sets the attack's number (atck_no) with the move,
   and the row is read when the attack box meets a body. After a row is its number, in
   hexadecimal, and the program's name for it:
   ATCK_ for the normals every character has (player.h), pl2b_at_ for its own (pl2b_def.h).
   The values' names are Capcom's own for these tables (hit_equ.h) and the program's (player.h).

     dmg     the damage
     type    the reaction: ZU pushed back, BU_O knocked off the feet, AS swept,
             or the number of another in pl_dmg.c's dm_tbl (here 2 dm_buttobi_ng, 5 dm_hold,
             10 dm_away, 11 dm_bound, 12 dm_snap, 21 dm_buttobi_ukemi_ng).
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
     box, box2  boxes of hit_2b that the computer measures the attack's reach by
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

const HIT_DATA hit_dt_2b[] = {
	/* dmg                 type          kind  elem  piyo  stop  sand  sleep   vec  pow  range     mark  score        se  shc  jug  box  box2                     flag  13  guard  15  catch  abyss  cnt    answer        1a */
	{   3,                  ZU,        A_NOM, K_LS,    4,    5,   10,     0, 0x1a,   1,  0x70, MARK_N1, SC100,       13,   0,  20,   7,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 00: ATCK_STAND_LP */
	{   7,                  ZU,        A_NOM, K_LM,    2,   11,   16,     0, 0x00,   2,  0x80, MARK_N2, SC200,       14,   0,  40,  11,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 01: ATCK_STAND_MP */
	{   9,                  ZU,        A_NOM,  K_M,    1,   12,   24,     0, 0x1a,   1,  0x80, MARK_N3, SC300,       15,   0,  50,  15,   88,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 02: ATCK_STAND_HP */
	{   4,                  ZU,        A_NOM, K_LS,    4,   10,   10,     0, 0x18,   4,  0x60, MARK_N1, SC100, SD_KIK_S,   0,  20,  25,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 03: ATCK_STAND_LK */
	{   7,                  ZU,        A_NOM,  K_M,    2,   11,   16,     0, 0x1a,   4,  0x60, MARK_N2, SC200, SD_KIK_M,   0,  40,  30,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 04: ATCK_STAND_MK */
	{  13,                  ZU,        A_NOM,  K_L,    1,   12,   24,     0, 0x1a,   8,  0x80, MARK_N3, SC300, SD_KIK_L,   0,  50,  31,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 05: ATCK_STAND_HK */
	{   4,                  ZU,        A_NOM, K_LS,    4,   10,   10,     0, 0x1c,   4,  0x70, MARK_N1, SC100,       13,   0,  20,  36,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 06: ATCK_CROUCH_LP */
	{   7,                  ZU,        A_NOM, K_LM,    2,   11,   16,     0, 0x13,   6,  0x80, MARK_N2, SC200,       14,   0,  40,  38,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 07: ATCK_CROUCH_MP */
	{  12,       BU_O | NO_VEC,        A_NOM, K_LL,    1,   12,   24,     0, 0x00,  11,  0x80, MARK_N3, SC300,       15,   0,  50,  42,    0, NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 08: ATCK_CROUCH_HP */
	{   4,                  ZU,         A_LO, K_LS,    4,   10,   10,     0, 0x1c,   4,  0x70, MARK_N1, SC100, SD_KIK_S,   0,  20,  45,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 09: ATCK_CROUCH_LK */
	{   7,                  ZU,         A_LO, K_LM,    2,   11,   16,     0, 0x1d,   6,  0x40, MARK_N2, SC200, SD_KIK_M,   0,  40,  48,   49,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0a: ATCK_CROUCH_MK */
	{  13,                  AS,         A_LO, K_LL,    1,   12,   24,     9, 0x1a,   8,  0x70, MARK_N3, SC300, SD_KIK_L,   0,  90,  52,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0b: ATCK_CROUCH_HK */
	{   4,         ZU | NO_VEC,         A_HI,  K_S,    1,   10,   10,     0, 0x00,   7,  0x70, MARK_N1, SC100,       13,   0,  20,  55,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0c: ATCK_JUMP_LP */
	{   7,         ZU | NO_VEC,         A_HI,  K_M,    1,   10,   16,     0, 0x00,   6,  0x80, MARK_N2, SC200,       14,   0,  40,  99,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0d: ATCK_JUMP_MP */
	{  13,         ZU | NO_VEC,         A_HI, K_LM,    1,   12,   32,     0, 0x14,   8,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  60,  58,    0,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0e: ATCK_JUMP_HP */
	{   4,         ZU | NO_VEC,         A_HI,  K_S,    1,   10,   10,     0, 0x00,   7,  0x60, MARK_N1, SC200, SD_KIK_S,   0,  20,  60,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0f: ATCK_JUMP_LK */
	{   6,         ZU | NO_VEC,         A_HI,  K_M,    1,   10,   16,     0, 0x00,   5,  0x60, MARK_N2, SC200, SD_KIK_M,   0,  40,  62,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 10: ATCK_JUMP_MK */
	{  13,         ZU | NO_VEC,         A_HI,  K_L,    1,   12,   32,     0, 0x15,   8,  0x60, MARK_N3, SC200, SD_KIK_L,   0,  60,  65,    0,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 11: ATCK_JUMP_HK */
	{  13,       BU_O | NO_VEC,    A_SPECIAL, K_LL,    0,   10,   24,     0, 0x00,  11,  0x80, MARK_N3, SC300,       15,   0,  40,  69,    0, NO_CHIP | NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 12: pl2b_at_upper, the launcher (pl2b_upper), crouching */
	{ 0 },	/* 13 */
	{ 0 },	/* 14 */
	{ 0 },	/* 15 */
	{ 0 },	/* 16 */
	{ 0 },	/* 17 */
	{ 0 },	/* 18 */
	{   2,         ZU | NO_VEC,        A_NOM,  K_S,    1,    5,   10,     0, 0x16,   4,  0x80, MARK_N1, SC300,       13,   0,  60,   9,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 19: pl2b_at_stand_lp_2, Second hit of the standing light punch */
	{   4,       BU_O | NO_VEC,        A_NOM,  K_L,    1,   11,   24,     0, 0x00,  11,  0x60, MARK_N3, SC200,       15,   3,  60,  19,   88, NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 1a: pl2b_at_stand_hp_2, Second hit of the standing heavy punch */
	{ 0 },	/* 1b */
	{ 0 },	/* 1c */
	{ 0 },	/* 1d */
	{ 0 },	/* 1e */
	{ 0 },	/* 1f */
	{   7,                   2, A_NOM | 0x40, K_LM,    4,   10,   20,     0, 0x04,  11,  0x60, MARK_N2, SC200, SD_NOR_M,   0,  40,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 20: pl2b_at_throw_2, Ground throw: the opponent tossed away after the slam */
	{  16,                   2, A_NOM | 0x40, K_LM,    4,   10,   20,     0, 0x1c,  10,  0x60, MARK_N2, SC200, SD_NOR_M,   0,  40,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 21: pl2b_at_air_throw, Air throw, slammed on landing */
	{   3,                   2, A_NOM | 0x40, K_LM,    4,   10,   20,     0, 0x04,  11,  0x60, MARK_N2, SC200, SD_NOR_M,   0,  40,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 22: pl2b_at_throw, Ground throw: the slam (catch_damage) */
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
	{  16,  2 | NO_VEC | NO_ZU,    A_SPECIAL, K_LL,    0,   12,   20,     0, 0x01,   8,  0x60,       3, SC200,       15,   0,  90,  72,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 30 */
	{  18,  2 | NO_VEC | NO_ZU,    A_SPECIAL, K_LL,    0,   15,   20,     0, 0x01,  10,  0x60,       3, SC200,       15,   0,  90,  72,    0,               NO_WEIGHT,  1,     0,  4,     0,     0,   0,        2, { 0, 0 } },	/* 31 */
	{   6, ZU | NO_VEC | NO_ZU,    A_SPECIAL, K_LM,    0,    6,   20,     0, 0x1c,   6,  0x60,       3, SC200, SD_NOR_M,   0,  90,  94,    0,               NO_WEIGHT,  1,     0,  7,     0,     0,   0,        2, { 0, 0 } },	/* 32: pl2b_at_birdie, Birdie (him and the thrown ball, shl2b_01 type 0) */
	{   5, ZU | NO_VEC | NO_ZU,    A_SPECIAL, K_LL,    0,    0,   30,     0, 0x1f,   8,  0x80,       3, SC300,       25,   0,  90,  83,    0, NO_WEIGHT | NO_SCALE | NO_COMBO,  1,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 33: pl2b_at_fang, Wild Fang leap */
	{  15,                  21,    A_SPECIAL, K_LL,    0,    8,   30,     0, 0x1a,  11,  0x80,       3, SC300,        8,   0,  90,   0,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 34: pl2b_at_fang_throw, Wild Fang's throw of the opponent it catches */
	{   8,           5 | NO_ZU,      A_HYPER, K_LL,    0,   10,   30,     0, 0x1e,   9,  0x80,       3, SC300,       15,   0,  90,  86,    0,       NO_WEIGHT | NO_KO,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 35: pl2b_at_clawx, Berserker Claw X rush, first hit (timer1 + 0x35 gives the next ones) */
	{   8,           5 | NO_ZU,      A_HYPER,  K_L,    0,    8,   30,     0, 0x1e,   8,  0x80,       3, SC300,       15,   0,  90,  87,    0,       NO_WEIGHT | NO_KO,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 36 */
	{   8,           5 | NO_ZU,      A_HYPER, K_LL,    0,   10,   30,     0, 0x1e,  10,  0x80,       3, SC300,       15,   0,  90,  86,    0,       NO_WEIGHT | NO_KO,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 37 */
	{  24,                   2,      A_HYPER,  K_L,    0,   30,   30,     0, 0x00,  10,  0x80,       3, SC300,       16,   0,  90,  89,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 38: pl2b_at_clawx_last, Berserker Claw X last blow (him and shl2b_02 when pl_sp_type 3) */
	{   4, ZU | NO_VEC | NO_ZU,      A_HYPER, K_LM,    0,    6,   20,     0, 0x1c,   6,  0x60,       3, SC200, SD_NOR_M,   0,  90,  94,    0,               NO_WEIGHT,  1,     0,  7,     0,     0,   0,        2, { 0, 0 } },	/* 39: pl2b_at_hbirdie, Hyper Birdie (him, and shl2b_01's balls: 0x39 or 0x3a at random) */
	{   4, ZU | NO_VEC | NO_ZU,      A_HYPER, K_LM,    0,    6,   20,     0, 0x1c,   6,  0x60,       3, SC200, SD_NOR_M,   0,  90,  94,    0,     NO_CHIP | NO_WEIGHT,  1,     0,  7,     0,     0,   0,        2, { 0, 0 } },	/* 3a */
	{   0,           5 | NO_ZU,      A_HYPER,  K_L,    0,   10,    0,     0, 0x00,   0,  0x80,       3, SC300, SD_NOR_L,   0,  90,  98,    0, NO_CHIP | NO_COMBO | NO_KO,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 3b: pl2b_at_weapon, Weapon X Rush, first hit (cg_flag1 + 0x3b gives the next ones) */
	{   4,           5 | NO_ZU,      A_HYPER,  K_L,    0,    1,    0,     0, 0x16,   8,  0x80,       3, SC300, SD_KIK_L,   0,  90,   7,    9,                   NO_KO,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 3c */
	{   4,           5 | NO_ZU,      A_HYPER, K_LL,    0,    1,    0,     0, 0x16,   8,  0x80,       3, SC300,       12,   0,  90,  11,   15,                   NO_KO,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 3d */
	{   4,           5 | NO_ZU,      A_HYPER,  K_L,    0,    1,    0,     0, 0x16,   8,  0x80,       3, SC300,       15,   0,  90,  19,   25,                   NO_KO,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 3e */
	{   4,           5 | NO_ZU,      A_HYPER, K_LL,    0,    1,    0,     0, 0x10,   7,  0x60,       3, SC200,       16,   5,  90,  30,   31,                   NO_KO,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 3f */
	{   4,          11 | NO_ZU,      A_HYPER,  K_S,    0,    1,    0,     0, 0x10,   8,  0x60,       3, SC200,       16,   0,  90,  36,   38,                   NO_KO,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 40 */
	{  15,                   2,      A_HYPER, K_LL,    0,   50,    0,     0, 0x00,  10,  0x60,       3, SC200,       16,   0,  90,  96,    0,    NO_WEIGHT | NO_SCALE,  1,     0,  4,     0,     0,   0,        3, { 0, 0 } },	/* 41: pl2b_at_weapon_last, Weapon X Rush's last blow (shl2b_02 type 0 set by weapon_drag) */
	{  10,         10 | NO_VEC,    A_SPECIAL,  K_L,    0,   10,   10,     0, 0x1f,  10,  0x60,       3, SC200,       12,   0,  90,  67,    0, NO_CHIP | NO_WEIGHT | AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 42: pl2b_at_t_kick, the partner's jump-in attack (t_kick_init) */
	{  10,         12 | NO_VEC,    A_SPECIAL,  K_L,    0,    4,   14,     0, 0x1c,  12,  0x80,       3, SC300,        8,  64,  90,  97,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 43: pl2b_at_snapback, Snapback (sp_snapback) */
	{  14,                BU_O,    A_SPECIAL,  K_L,    0,   12,   24,     0, 0x1a,  11,  0x80, MARK_N3, SC300,       15,   0,  90, 101,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 44: pl2b_at_crouch_hp, Crouching heavy punch as gamma assist (sp_crouch_hp) */
	{  14,                  ZU,    A_SPECIAL,  K_L,    0,   12,   24,     0, 0x1a,   8,  0x80, MARK_N3, SC300,       15,   0,  90, 100,    0,                       0,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 45: pl2b_at_crouch_hp_2, Its second hit (crouch_hp_move) */
};
