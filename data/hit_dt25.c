/* Dhalsim (character 25): the attacks (HIT_DATA, 0x1c bytes).
   What each hit does. The character's module sets the attack's number (atck_no) with the move,
   and the row is read when the attack box meets a body. After a row is its number, in
   hexadecimal, and the program's name for it:
   ATCK_ for the normals every character has (player.h), pl25_at_ for its own (pl25_def.h).
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
     box, box2  boxes of hit_25 that the computer measures the attack's reach by
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

const HIT_DATA hit_dt_25[] = {
	/* dmg                type            kind     elem  piyo  stop  sand  sleep   vec  pow  range     mark  score        se  shc  jug  box  box2                     flag  13  guard  15  catch  abyss  cnt    answer        1a */
	{   3,                 ZU,          A_NOM,    K_LS,    4,   10,   10,     0, 0x18,   4,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,   7,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 00: ATCK_STAND_LP */
	{   6,      BU_O | NO_VEC,          A_NOM,    K_LM,    2,   10,   16,     0, 0x00,  11,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40, 156,   10, NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 01: ATCK_STAND_MP */
	{  12,                 ZU,          A_NOM,    K_LL,    1,   10,   24,     0, 0x1a,   8,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  50,  12,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 02: ATCK_STAND_HP */
	{   3,                 ZU,          A_NOM,    K_LS,    4,   10,   10,     0, 0x18,   2,  0x60, MARK_N1, SC100, SD_KIK_S,   0,  20,  14,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 03: ATCK_STAND_LK */
	{   6,                 ZU,          A_NOM,     K_M,    2,   10,   16,     0, 0x1b,   6,  0x60, MARK_N2, SC200, SD_KIK_M,   0,  40,  16,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 04: ATCK_STAND_MK */
	{  12,               BU_O,          A_NOM,     K_L,    1,   10,   24,     0, 0x1b,  10,  0x80, MARK_N3, SC300, SD_KIK_L,   0,  50,  18,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 05: ATCK_STAND_HK */
	{   3,        ZU | NO_VEC,          A_NOM,    K_LS,    4,   10,   10,     0, 0x1a,   4,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  20,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 06: ATCK_CROUCH_LP */
	{   6,                 ZU,          A_NOM,    K_LM,    2,   10,   16,     0, 0x1a,   6,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40,  20,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 07: ATCK_CROUCH_MP */
	{  12,                 ZU,          A_NOM,    K_LL,    1,   10,   24,     0, 0x1a,   8,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  50,  20,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 08: ATCK_CROUCH_HP */
	{   3,                 ZU,           A_LO,    K_LS,    4,   10,   10,     0, 0x1a,   2,  0x70, MARK_N1, SC100, SD_KIK_S,   0,  20,  22,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 09: ATCK_CROUCH_LK */
	{   6,                 ZU,           A_LO,    K_LM,    2,   10,   16,     0, 0x1a,   3,  0x40, MARK_N2, SC200, SD_KIK_M,   0,  40,  89,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0a: ATCK_CROUCH_MK */
	{  12,         AS | NO_ZU,           A_LO,    K_LL,    1,   10,   24,     9, 0x1a,   4,  0x70, MARK_N3, SC300, SD_KIK_L,   0,  90,  91,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0b: ATCK_CROUCH_HK */
	{   3,        ZU | NO_VEC,           A_HI,     K_S,    1,   10,   10,     0, 0x00,   5,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  24,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0c: ATCK_JUMP_LP */
	{   6,        ZU | NO_VEC,           A_HI,     K_M,    1,   10,   16,     0, 0x00,   5,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40,  26,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0d: ATCK_JUMP_MP */
	{  12,        ZU | NO_VEC,           A_HI,     K_M,    1,   10,   24,     0, 0x18,   8,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  60,  28,    0,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0e: ATCK_JUMP_HP */
	{   3,        ZU | NO_VEC,           A_HI,     K_S,    1,   10,   10,     0, 0x00,   5,  0x60, MARK_N1, SC100, SD_KIK_S,   0,  20,  31,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0f: ATCK_JUMP_LK */
	{   6,        ZU | NO_VEC,           A_HI,     K_M,    1,   10,   16,     0, 0x00,   5,  0x60, MARK_N2, SC200, SD_KIK_M,   0,  40, 116,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 10: ATCK_JUMP_MK */
	{  12,        ZU | NO_VEC,           A_HI,     K_M,    1,   10,   24,     0, 0x19,   8,  0x80, MARK_N3, SC300, SD_KIK_L,   0,  60,  34,    0,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 11: ATCK_JUMP_HK */
	{  12,        ZU | NO_VEC,           A_HI,     K_L,    1,   10,   24,     0, 0x18,   8,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  60,  29,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 12: pl25_at_jdown_hp, Down + HP in the air */
	{   3,        ZU | NO_VEC,           A_HI,     K_S,    4,   10,   16,     0, 0x1d,   2,  0x60, MARK_N1, SC200, SD_KIK_S,   0,  20,  35,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 13: pl25_at_jdown_lk, Down + LK in the air (drill) */
	{   6,        ZU | NO_VEC,           A_HI,     K_M,    2,   10,   16,     0, 0x1d,   2,  0x60, MARK_N2, SC200, SD_KIK_M,   0,  40,  36,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 14: pl25_at_jdown_mk, Down + MK in the air (drill) */
	{  12,        ZU | NO_VEC,           A_HI,     K_M,    1,   10,   16,     0, 0x1d,   2,  0x60, MARK_N3, SC200, SD_KIK_L,   0,  60,  37,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 15: pl25_at_jdown_hk, Down + HK in the air (drill) */
	{  12,      BU_O | NO_VEC,      A_SPECIAL,    K_LM,    0,   10,   16,     0, 0x00,  11,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40,  10,    0, NO_CHIP | NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 16: pl25_at_upper, Launcher (pl25_upper, Char_Extra 16) */
	{ 0 },	/* 17 */
	{ 0 },	/* 18 */
	{ 0 },	/* 19 */
	{ 0 },	/* 1a */
	{ 0 },	/* 1b */
	{ 0 },	/* 1c */
	{ 0 },	/* 1d */
	{ 0 },	/* 1e */
	{ 0 },	/* 1f */
	{  10,                  2,   A_NOM | 0x40,       0,    0,    4,    0,    15, 0x1e,   0,  0x80, MARK_N2, SC300,       76,   1,   0,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 20: pl25_at_com_throw, Command throw (half circle + HP): the final throw */
	{  16,                  2,   A_NOM | 0x40,       0,    0,   16,    0,    15, 0x06,   4,  0x80, MARK_N2, SC300,       76,   0,   0,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 21: pl25_at_throw_p, P ground throw */
	{  50,                  2, A_HYPER | 0x40,       0,    0,    8,    0,    15, 0x04,   4,  0x80,       3, SC300,        8,   0,  90,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 22: pl25_at_strike_release, Yoga Strike hyper: lifts and lets go of the caught opponent */
	{   8,                  2,   A_NOM | 0x40,       0,    0,    8,    0,    15, 0x18,  15,  0x80, MARK_N2, SC300, SD_NOR_L,   0,   0,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 23: pl25_at_air_throw, P air throw: the release */
	{   8,                  2,   A_NOM | 0x40,       0,    0,    8,    0,    15, 0x1a,   8,  0x80, MARK_N2, SC300, SD_NOR_M,   0,   0,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 24: pl25_at_throw_k, K ground throw: the release */
	{   8,                  2,   A_NOM | 0x40,       0,    0,    8,    0,    15, 0x1a,   8,  0x80, MARK_N2, SC300, SD_NOR_M,   0,   0,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 25 */
	{   3,                  2,   A_NOM | 0x40,       0,    0,    8,    0,    15, 0x1a,   8,  0x80, MARK_N2, SC300, SD_NOR_M,   0,   0,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 26: pl25_at_com_throw_hit, Command throw: each hit while held (can be mashed out of) */
	{  10,                  2, A_HYPER | 0x40,       0,    0,    8,    0,    15, 0x1a,   8,  0x80,       3, SC300,        8,   0,  90,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 27: pl25_at_strike_slam, Yoga Strike hyper: the slam */
	{ 0 },	/* 28 */
	{ 0 },	/* 29 */
	{ 0 },	/* 2a */
	{ 0 },	/* 2b */
	{ 0 },	/* 2c */
	{ 0 },	/* 2d */
	{ 0 },	/* 2e */
	{ 0 },	/* 2f */
	{   3,                 ZU,          A_NOM,    K_LS,    4,   10,   10,     0, 0x18,   4,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,   8,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 30: pl25_at_far_lp, Far standing LP */
	{   6,                 ZU,          A_NOM,    K_LM,    2,   10,   16,     0, 0x1a,   6,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40,   9,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 31: pl25_at_far_mp, Far standing MP */
	{  12,                 ZU,          A_NOM,    K_LL,    1,   10,   24,     0, 0x1a,   8,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  50,  11,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 32: pl25_at_far_hp, Far standing HP */
	{   3,                 ZU,          A_NOM,    K_LS,    4,   10,   10,     0, 0x18,   2,  0x60, MARK_N1, SC100, SD_KIK_S,   0,  20,  59,   13,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 33: pl25_at_far_lk, Far standing LK */
	{   6,                 ZU,          A_NOM,     K_M,    2,   10,   16,     0, 0x1b,   6,  0x60, MARK_N2, SC200, SD_KIK_M,   0,  40,  15,   61,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 34: pl25_at_far_mk, Far standing MK */
	{  13,               BU_O,          A_NOM,     K_L,    1,   10,   24,     0, 0x1b,  10,  0x80, MARK_N3, SC300, SD_KIK_L,   0,  50,  17,   64,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 35: pl25_at_far_hk, Far standing HK */
	{   3,                 ZU,          A_NOM,    K_LS,    4,   10,   10,     0, 0x1a,   4,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  19,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 36: pl25_at_far_crouch_lp, Far crouching LP */
	{   6,                 ZU,          A_NOM,    K_LM,    2,   10,   16,     0, 0x1a,   6,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40,  72,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 37: pl25_at_far_crouch_mp, Far crouching MP */
	{  13,                 ZU,          A_NOM,    K_LL,    1,   10,   24,     0, 0x1a,   8,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  50,  75,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 38: pl25_at_far_crouch_hp, Far crouching HP */
	{   3,                 ZU,           A_LO,    K_LS,    4,   10,   10,     0, 0x1a,   2,  0x70, MARK_N1, SC100, SD_KIK_S,   0,  20,  21,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 39: pl25_at_far_crouch_lk, Far crouching LK */
	{   6,                 ZU,           A_LO,    K_LM,    2,   10,   16,     0, 0x1a,   3,  0x40, MARK_N2, SC200, SD_KIK_M,   0,  40,  81,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 3a: pl25_at_far_crouch_mk, Far crouching MK */
	{  13,                 AS,           A_LO,    K_LL,    1,   10,   24,     9, 0x1a,   4,  0x70, MARK_N3, SC300, SD_KIK_L,   0,  90,  84,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 3b: pl25_at_far_crouch_hk, Far crouching HK */
	{   3,        ZU | NO_VEC,           A_HI,    K_LS,    1,   10,   10,     0, 0x00,   2,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  23,   93,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 3c: pl25_at_far_jump_lp, Far jumping LP */
	{   6,        ZU | NO_VEC,           A_HI,    K_LM,    1,   10,   16,     0, 0x00,   2,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40,  25,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 3d: pl25_at_far_jump_mp, Far jumping MP */
	{  13,        ZU | NO_VEC,           A_HI,    K_LL,    1,   10,   24,     0, 0x16,  10,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  60,  27,   98,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 3e: pl25_at_far_jump_hp, Far jumping HP */
	{   3,                 ZU,           A_HI,    K_LS,    1,   10,   10,     0, 0x00,   2,  0x70, MARK_N1, SC100, SD_KIK_S,   0,  20, 105,   30,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 3f: pl25_at_far_jump_lk, Far jumping LK */
	{   6,                 ZU,           A_HI,    K_LM,    1,   10,   16,     0, 0x00,   3,  0x40, MARK_N2, SC200, SD_KIK_M,   0,  40,  32,  108,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 40: pl25_at_far_jump_mk, Far jumping MK */
	{  13,                 ZU,           A_HI,    K_LM,    1,   10,   24,     8, 0x1a,   4,  0x70, MARK_N3, SC300, SD_KIK_L,   0, 100,  33,  111,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 41: pl25_at_far_jump_hk, Far jumping HK */
	{  14,         ZU | NO_ZU,      A_SPECIAL, K_S | 2,    0,    4,   20,     0, 0x1b,   6,  0x80,       3, SC300,        8,   0,  50,  38,    0, NO_WEIGHT | AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 42: pl25_at_fire, Yoga Fire fireball (shl25_00); the player also sets it for both Yoga Infernos */
	{ 0 },	/* 43 */
	{  14,         ZU | NO_ZU,      A_SPECIAL, K_L | 2,    0,    4,   20,     0, 0x1b,   6,  0x80,       3, SC300, SD_NOR_L,   0,  50,  38,    0, NO_WEIGHT | AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 44 */
	{  18, 2 | NO_VEC | NO_ZU,      A_SPECIAL, K_L | 2,    0,    4,   20,     0, 0x00,   8,  0x80,       3, SC300,        8,   2,  90, 136,   40,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 45: pl25_at_frei_l, Yoga Flame, light (player and shl25_01) */
	{  18,          2 | NO_ZU,      A_SPECIAL, K_L | 2,    0,    4,   20,     0, 0x00,   8,  0x80,       3, SC300, SD_NOR_L,   2,  90, 144,   42,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 46: pl25_at_brast_l, Yoga Blast, light (player and shl25_03) */
	{   3,         21 | NO_ZU,        A_HYPER,       2,    0,    8,   20,     8, 0x00,   2,  0x80,       3, SC300,        8,   0,  90,  45,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   5,        3, { 0, 0 } },	/* 47: pl25_at_inferno_1, Yoga Inferno flame (shl25_04, flame_no 0) */
	{  15,          2 | NO_ZU,        A_HYPER,       2,    0,   10,   20,     0, 0x1e,   8,  0x80,       3, SC300,        8,   0,  90,  45,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   5, YK_GD030, { 0, 0 } },	/* 48 */
	{  10,        10 | NO_VEC,      A_SPECIAL,     K_L,    0,   10,   10,     0, 0x1f,  10,  0x60,       3, SC200,       12,   0,  90, 149,    0, NO_CHIP | AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        5, { 0, 0 } },	/* 49: pl25_at_t_kick, Partner's wall-leap kick (t_kick) */
	{   8,                  2,   A_NOM | 0x40,       0,    0,    8,    0,    15, 0x1a,   8,  0x80, MARK_N1, SC300, SD_NOR_M,   0,   0,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 4a: pl25_at_throw_k_hit, K ground throw: the hit while held */
	{   8,                  2,   A_NOM | 0x40,       0,    0,    8,    0,    15, 0x1a,   8,  0x80, MARK_N1, SC300, SD_NOR_M,   0,   0,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 4b: pl25_at_air_throw_hit, P air throw: the hit while held */
	{   8,                  2,   A_NOM | 0x40,       0,    0,    8,    0,    15, 0x1a,   8,  0x80, MARK_N1, SC300, SD_NOR_M,   0,   0,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 4c */
	{   4,         21 | NO_ZU,        A_HYPER,       2,    0,    8,   20,     8, 0x00,   0,  0x80,       3, SC300, SD_NOR_L,   0,  90,  45,    0,     NO_CHIP | NO_WEIGHT,  1,     0,  0,     0,     0,   5,        3, { 0, 0 } },	/* 4d: pl25_at_inferno_2, Yoga Inferno flame, the other attack number (shl25_04, flame_no set) */
	{  10,        12 | NO_VEC,      A_SPECIAL,     K_S,    0,    4,   24,     0, 0x1c,  12,  0x80,       3, SC300,        8,  64,  90,   0,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0,        5, { 0, 0 } },	/* 4e: pl25_at_snapback, Snapback */
	{   7, 2 | NO_VEC | NO_ZU,      A_SPECIAL, K_L | 2,    0,    4,   20,     0, 0x00,   8,  0x80,       3, SC300, SD_NOR_L,   2,  90, 136,   40,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        7, { 0, 0 } },	/* 4f: pl25_at_frei_h, Yoga Flame, higher level (player and shl25_01) */
	{   7,          2 | NO_ZU,      A_SPECIAL, K_L | 2,    0,    4,   20,     0, 0x00,   8,  0x80,       3, SC300, SD_NOR_L,   2,  90, 144,   42,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        7, { 0, 0 } },	/* 50: pl25_at_brast_h, Yoga Blast, higher level (player and shl25_03) */
};
