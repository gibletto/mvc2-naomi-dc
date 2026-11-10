/* Spiral (character 31): the attacks (HIT_DATA, 0x1c bytes).
   What each hit does. The character's module sets the attack's number (atck_no) with the move,
   and the row is read when the attack box meets a body. After a row is its number, in
   hexadecimal, and the program's name for it:
   ATCK_ for the normals every character has (player.h), pl31_at_ for its own (pl31_def.h).
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
     box, box2  boxes of hit_31 that the computer measures the attack's reach by
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

const HIT_DATA hit_dt_31[] = {
	/* dmg                 type          kind  elem  piyo  stop  sand  sleep   vec  pow  range     mark  score        se  shc  jug  box  box2                     flag  13  guard  15  catch  abyss  cnt    answer        1a */
	{   4,                  ZU,        A_NOM,  K_S,    4,   10,   10,     0, 0x18,   4,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  23,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 00: ATCK_STAND_LP */
	{   7,                  ZU,        A_NOM,  K_M,    2,   11,   16,     0, 0x1a,   6,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40,  24,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 01: ATCK_STAND_MP */
	{   3,                  ZU,        A_NOM, K_LM,    1,    8,   24,     0, 0x1e,   1,  0x80, MARK_N3, SC300, SD_NOR_L,   2,  50,  25,   26,                       0,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 02: ATCK_STAND_HP */
	{   4,                  ZU,        A_NOM, K_LS,    4,   10,   10,     0, 0x18,   4,  0x60, MARK_N1, SC100, SD_KIK_S,   0,  20,  37,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 03: ATCK_STAND_LK */
	{   7,                  ZU,        A_NOM, K_LM,    2,   10,   16,     0, 0x1a,   4,  0x60, MARK_N2, SC200, SD_KIK_M,   0,  40,  38,   39,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 04: ATCK_STAND_MK */
	{  13,                  ZU,        A_NOM,  K_L,    1,   12,   24,     0, 0x1a,   8,  0x80, MARK_N3, SC300, SD_KIK_L,   0,  50,  41,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 05: ATCK_STAND_HK */
	{   4,                  ZU,        A_NOM, K_LS,    4,   10,   10,     0, 0x1c,   4,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  48,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 06: ATCK_CROUCH_LP */
	{   7,                  ZU,        A_NOM, K_LM,    2,   11,   16,     0, 0x13,   6,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40,  49,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 07: ATCK_CROUCH_MP */
	{  13,       BU_O | NO_VEC,        A_NOM, K_LL,    1,   10,   24,     0, 0x1c,  10,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  50,  50,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 08: ATCK_CROUCH_HP */
	{   4,                  ZU,         A_LO, K_LS,    4,   10,   10,     0, 0x1c,   4,  0x70, MARK_N1, SC100, SD_KIK_S,   0,  20,  51,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 09: ATCK_CROUCH_LK */
	{   7,                  ZU,         A_LO, K_LM,    2,   11,   16,     0, 0x1d,   6,  0x40, MARK_N2, SC200, SD_KIK_M,   0,  40,  52,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0a: ATCK_CROUCH_MK */
	{  12,       BU_O | NO_VEC,        A_NOM, K_LL,    1,   12,   24,     0, 0x1f,  11,  0x70, MARK_N3, SC300, SD_KIK_L,   0,  50,  53,    0, NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0b: ATCK_CROUCH_HK */
	{   4,         ZU | NO_VEC,         A_HI,  K_S,    1,   10,   10,     0, 0x00,   7,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  63,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0c: ATCK_JUMP_LP */
	{   7,         ZU | NO_VEC,         A_HI,  K_M,    1,   10,   16,     0, 0x00,   6,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40,  67,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0d: ATCK_JUMP_MP */
	{  13,         ZU | NO_VEC,         A_HI, K_LL,    1,   12,   32,     0, 0x16,   8,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  60,  64,    0,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0e: ATCK_JUMP_HP */
	{   4,         ZU | NO_VEC,         A_HI,  K_S,    1,   10,   10,     0, 0x00,   7,  0x60, MARK_N1, SC200, SD_KIK_S,   0,  20,  68,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0f: ATCK_JUMP_LK */
	{   7,         ZU | NO_VEC,         A_HI,  K_M,    1,   10,   16,     0, 0x00,   6,  0x60, MARK_N2, SC200, SD_KIK_M,   0,  40,  71,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 10: ATCK_JUMP_MK */
	{  13,         ZU | NO_VEC,         A_HI,  K_L,    1,   12,   32,     0, 0x15,   8,  0x60, MARK_N3, SC200, SD_KIK_L,   0,  60,  70,    0,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 11: ATCK_JUMP_HK */
	{   7,         ZU | NO_VEC,         A_HI,  K_M,    1,   10,   16,     0, 0x00,   6,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40,  66,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 12: pl31_at_jdown_mp, Air medium punch with down */
	{  13,         ZU | NO_VEC,         A_HI, K_LL,    1,   12,   32,     0, 0x16,   8,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  60,  65,    0,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 13: pl31_at_jdown_hp, Air heavy punch with down */
	{   7,         ZU | NO_VEC,         A_HI,  K_M,    1,   10,   16,     0, 0x00,   6,  0x60, MARK_N2, SC200, SD_KIK_M,   0,  40,  69,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 14: pl31_at_jup_mk, Air medium kick with up */
	{ 0 },	/* 15 */
	{ 0 },	/* 16 */
	{ 0 },	/* 17 */
	{ 0 },	/* 18 */
	{  12,       BU_O | NO_VEC,    A_SPECIAL, K_LL,    0,   12,   24,     0, 0x1f,  11,  0x70, MARK_N3, SC300, SD_KIK_L,   0,  40,  53,    0, NO_CHIP | NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 19: pl31_at_upper, the launcher (pl31_upper), crouching */
	{   3,                  ZU,        A_NOM,  K_L,    1,    8,   24,     0, 0x1e,   1,  0x80, MARK_N3, SC300, SD_NOR_L,   2,  90,  25,   26,                       0,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 1a: pl31_at_stand_hp_2, Second hit of the standing heavy punch (also the gamma assist, sp_stand_hp) */
	{ 0 },	/* 1b */
	{ 0 },	/* 1c */
	{ 0 },	/* 1d */
	{ 0 },	/* 1e */
	{ 0 },	/* 1f */
	{   5,                BU_O, A_NOM | 0x40, K_LM,    0,   10,   20,     0, 0x1c,  14,  0x60, MARK_N2, SC200, SD_NOR_M,   0,  40,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 20: pl31_at_throw_p, Ground punch throw's hits (nml_p_move) */
	{  16,                   2, A_NOM | 0x40, K_LM,    0,   10,   40,     0, 0x19,  11,  0x60, MARK_N2, SC200, SD_NOR_M,   0,  40,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 21 */
	{  16,                BU_O, A_NOM | 0x40, K_LM,    0,   10,   20,     0, 0x00,  10,  0x60, MARK_N2, SC200, SD_NOR_M,   0,  40,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 22 */
	{  16,                   2, A_NOM | 0x40, K_LM,    0,   10,   20,     0, 0x10,  10,  0x60, MARK_N2, SC200, SD_NOR_M,   0,  40,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 23 */
	{  16,                   2, A_NOM | 0x40, K_LM,    0,    8,   20,     0, 0x04,  10,  0x60, MARK_N2, SC200, SD_NOR_M,   0,  40,  35,   36,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 24: pl31_at_air_throw_k, Air kick throw: slammed on landing */
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
	{  10,         10 | NO_VEC,    A_SPECIAL,  K_L,    0,   10,   10,     0, 0x1f,  10,  0x60,       3, SC200,       12,   0,  90,  87,    0, NO_CHIP | NO_WEIGHT | AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 30: pl31_at_t_kick, the partner's jump-in attack (pl31_t_kick) */
	{  10,         12 | NO_VEC,    A_SPECIAL,  K_L,    1,    4,   24,     0, 0x1c,  12,  0x80,       3, SC300,        8,  64, 100,  50,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 31: pl31_at_snapback, Snapback (sp_snapback) */
	{   4, ZU | NO_VEC | NO_ZU,    A_SPECIAL, K_LS,    0,    6,   20,     0, 0x00,   1,  0x60,       3, SC200, SD_NOR_M,   0,  50,  72,    0,                       0,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 32: pl31_at_tobashi, Tsurugi Tobashi: the swords fired (tobashi_init, the assist's swords_init, shl31_00) */
	{   4,       BU_O | NO_VEC,      A_HYPER,  K_L,    0,   10,   20,     0, 0x1f,  10,  0x60,       3, SC200, SD_NOR_L,   0,  90,  77,    0,                       0,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 33: pl31_at_metamor, Metamorphose's speed-up start (metamor_maxspeed_init) */
	{   6,           5 | NO_ZU,      A_HYPER,  K_M,    0,    4,    4,     0, 0x00,   0,  0x60,       3, SC200, SD_NOR_L,   0,  90,  84,    0,                   NO_KO,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 34: pl31_at_metamor_1, Metamorphose's hyper throw, first hit (metamor_throw, cg_type 1) */
	{   6,           5 | NO_ZU,      A_HYPER, K_LS,    0,    4,    4,     0, 0x00,   0,  0x60,       3, SC200, SD_NOR_L,   0,  90,  86,    0,                   NO_KO,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 35: pl31_at_metamor_2, Its second hit (cg_type 2) */
	{  30,      2 | FLY_SCREEN,      A_HYPER,  K_L,    0,   12,   20,     0, 0x1c,  14,  0x60,       3, SC200, SD_NOR_L,   0,  90,  84,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 36: pl31_at_metamor_3, Its third hit (cg_type 3) */
	{ 0 },	/* 37 */
	{   3, ZU | NO_VEC | NO_ZU,      A_HYPER,  K_M,    0,    4,   14,     0, 0x14,   0,  0x60,       3, SC200, SD_NOR_L,   0,  90,  84,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 38: pl31_at_hmetamor_1, Hyper Metamorphose (team hyper): the dash and first hit */
	{   3, ZU | NO_VEC | NO_ZU,      A_HYPER, K_LS,    0,    4,   14,     0, 0x14,   0,  0x60,       3, SC200, SD_NOR_L,   0,  90,  86,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 39: pl31_at_hmetamor_2, Its second hit */
	{  30,      2 | FLY_SCREEN,      A_HYPER,  K_L,    0,   12,   20,     0, 0x1c,  14,  0x60,       3, SC200, SD_NOR_L,   0,  90,  84,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 3a: pl31_at_hmetamor_3, Its third hit */
	{   4, ZU | NO_VEC | NO_ZU,      A_HYPER, K_LL,    0,   10,   30,     0, 0x1e,   2,  0x60,       3, SC200, SD_NOR_L,   0,  90,  72,    0,                       0,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 3b: pl31_at_stampede, Stampede Sword: her and the odd-numbered swords (shl31_01) */
	{   4, ZU | NO_VEC | NO_ZU,      A_HYPER, K_LL,    0,   10,   30,     0, 0x1e,   2,  0x60,       3, SC200, SD_NOR_L,   0,  90,  72,    0,                 NO_CHIP,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 3c: pl31_at_stampede_2, Stampede Sword's even-numbered swords (shl31_01) */
};
