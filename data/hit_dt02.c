/* Guile (character 02): the attacks (HIT_DATA, 0x1c bytes).
   What each hit does. The character's module sets the attack's number (atck_no) with the move,
   and the row is read when the attack box meets a body. After a row is its number, in
   hexadecimal, and the program's name for it:
   ATCK_ for the normals every character has (player.h), pl02_at_ for its own (pl02_def.h).
   The values' names are Capcom's own for these tables (hit_equ.h) and the program's (player.h).

     dmg     the damage
     type    the reaction: ZU pushed back, BU_O knocked off the feet, AS swept,
             or the number of another in pl_dmg.c's dm_tbl (here 2 dm_buttobi_ng, 5 dm_hold,
             10 dm_away, 12 dm_snap, 21 dm_buttobi_ukemi_ng).
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
     box, box2  boxes of hit_02 that the computer measures the attack's reach by
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

const HIT_DATA hit_dt_02[] = {
	/* dmg                    type          kind  elem  piyo  stop  sand  sleep   vec  pow  range     mark  score        se  shc  jug  box  box2                     flag  13  guard  15  catch  abyss  cnt    answer        1a */
	{   4,                     ZU,        A_NOM,  K_S,    4,   10,   10,     0, 0x18,   4,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,   6,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 00: ATCK_STAND_LP */
	{   7,                     ZU,        A_NOM, K_LM,    2,   10,   16,     0, 0x1d,   6,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40,   7,    8,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 01: ATCK_STAND_MP */
	{  13,                     ZU,        A_NOM,  K_L,    1,   10,   24,     0, 0x1a,   4,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  50,   9,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 02: ATCK_STAND_HP */
	{   4,                     ZU,        A_NOM, K_LS,    4,   10,   10,     0, 0x18,   2,  0x60, MARK_N1, SC100, SD_KIK_S,   0,  20,  14,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 03: ATCK_STAND_LK */
	{   7,                     ZU,        A_NOM,  K_M,    2,   10,   16,     0, 0x1a,   3,  0x60, MARK_N2, SC200, SD_KIK_M,   0,  40,  15,   62,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 04: ATCK_STAND_MK */
	{  13,                     ZU,        A_NOM,  K_L,    1,   10,   24,     0, 0x1c,   4,  0x80, MARK_N3, SC300, SD_KIK_L,   0,  50,  18,   63,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 05: ATCK_STAND_HK */
	{   4,                     ZU,        A_NOM, K_LS,    4,   10,   10,     0, 0x18,   6,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  20,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 06: ATCK_CROUCH_LP */
	{   7,                     ZU,        A_NOM, K_LM,    2,   10,   16,     0, 0x18,   8,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40,  21,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 07: ATCK_CROUCH_MP */
	{  13,          BU_O | NO_VEC,        A_NOM, K_LL,    1,   10,   24,     0, 0x00,  11,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  50,  22,   23, NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 08: ATCK_CROUCH_HP */
	{   4,                     ZU,         A_LO, K_LS,    4,   10,   10,     0, 0x18,   8,  0x70, MARK_N1, SC100, SD_KIK_S,   0,  20,  24,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 09: ATCK_CROUCH_LK */
	{   7,                     ZU,         A_LO, K_LM,    2,   10,   16,     0, 0x18,   8,  0x40, MARK_N2, SC200, SD_KIK_M,   0,  40,  31,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0a: ATCK_CROUCH_MK */
	{   7,                     AS,         A_LO, K_LL,    1,   10,   24,     9, 0x18,   8,  0x70, MARK_N3, SC300, SD_KIK_L,   0,  90,  25,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0b: ATCK_CROUCH_HK */
	{   4,            ZU | NO_VEC,         A_HI,  K_S,    1,   10,   10,     0, 0x00,   6,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  26,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0c: ATCK_JUMP_LP */
	{   7,            ZU | NO_VEC,         A_HI,  K_M,    1,   10,   16,     0, 0x00,   4,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40,  27,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0d: ATCK_JUMP_MP */
	{  13,            ZU | NO_VEC,         A_HI,  K_M,    1,   10,   24,     0, 0x18,   8,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  60,  89,    0, NO_WEIGHT | AERIAL_FINISH,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0e: ATCK_JUMP_HP */
	{   4,            ZU | NO_VEC,         A_HI,  K_S,    1,   10,   10,     0, 0x00,   6,  0x60, MARK_N1, SC100, SD_KIK_S,   0,  20,  28,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0f: ATCK_JUMP_LK */
	{   7,            ZU | NO_VEC,         A_HI,  K_M,    1,   10,   16,     0, 0x00,   4,  0x60, MARK_N2, SC200, SD_KIK_M,   0,  40,  29,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 10: ATCK_JUMP_MK */
	{  13,            ZU | NO_VEC,         A_HI,  K_M,    1,   10,   24,     0, 0x18,   8,  0x80, MARK_N3, SC300, SD_KIK_L,   0,  60,  30,    0,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 11: ATCK_JUMP_HK */
	{  13,                     ZU,        A_NOM,  K_L,    1,   10,   24,     0, 0x1a,   8,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  50,  13,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 12: pl02_at_fwd_hp, Stand HP with the stick forward */
	{  13,                     ZU,        A_NOM,  K_M,    1,   10,   16,     0, 0x1a,   6,  0x60, MARK_N3, SC200, SD_KIK_L,   0,  50,  16,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 13: pl02_at_back_hk, Stand HK with the stick back */
	{  13,                     ZU,        A_NOM, K_LL,    4,   10,   16,     0, 0x1a,   8,  0x60, MARK_N1, SC200, SD_KIK_M,   0,  20,  19,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 14: pl02_at_back_lk, Stand LK with the stick back */
	{  13,                     ZU,        A_NOM, K_LL,    1,   10,   16,     0, 0x1a,   8,  0x60, MARK_N3, SC200, SD_KIK_L,   0,  50,  19,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 15: pl02_at_fwd_hk, Stand HK with the stick forward */
	{  13,          BU_O | NO_VEC,    A_SPECIAL, K_LL,    0,   10,   24,     0, 0x00,  11,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  40,  22,   23, NO_CHIP | NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 16: pl02_at_upper, the launcher (pl02_upper) */
	{ 0 },	/* 17 */
	{ 0 },	/* 18 */
	{   7,                     AS,         A_LO, K_LL,    1,   10,   24,     9, 0x18,   8,  0x70, MARK_N3, SC300, SD_KIK_L,   0,  90,  25,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 19: pl02_at_crouch_hk_2, Crouch HK, second hit */
	{ 0 },	/* 1a */
	{ 0 },	/* 1b */
	{ 0 },	/* 1c */
	{ 0 },	/* 1d */
	{ 0 },	/* 1e */
	{ 0 },	/* 1f */
	{  16,          BU_O | NO_VEC, A_NOM | 0x40,    0,    0,    4,   20,    15, 0x1c,   4,  0x80, MARK_N2, SC300,       76,   0,   0,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 20: pl02_at_throw_k, Kick throw */
	{  16,                      2, A_NOM | 0x40,    0,    0,   16,    0,    15, 0x07,  11,  0x80, MARK_N2, SC300,       76,   4,   0,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 21: pl02_at_throw_p, Punch throw */
	{   0,                      2, A_NOM | 0x40,    0,    0,    8,    0,    15, 0x10,   4,  0x80, MARK_N2, SC300,        8,   1,   0,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 22: pl02_at_air_throw_k, Kick air throw: the throw after landing */
	{  16,                      2, A_NOM | 0x40,    0,    0,    8,    0,    15, 0x16,  12,  0x80, MARK_N2, SC300,        8,   1,   0,   6,   17,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 23: pl02_at_air_throw_p, Punch air throw */
	{  16,             2 | NO_VEC, A_NOM | 0x40,    0,    0,    8,    0,    15, 0x00,  12,  0x80, MARK_N2, SC300, SD_NOR_L,   7,   0,   6,   17,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 24: pl02_at_air_throw_k_hit, Kick air throw: the hit on landing (cg_type < 0) */
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
	{  12,             ZU | NO_ZU,    A_SPECIAL,  K_L,    0,    4,   20,     0, 0x1c,   4,  0x80,       3, SC300,        8,   0,  50,  32,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 30: pl02_at_sonic, Sonic Boom: set on Guile as he throws it */
	{  12,             ZU | NO_ZU,    A_SPECIAL,  K_L,    0,    4,   20,     0, 0x1c,   4,  0x80,       3, SC300,        8,   0,  50,  32,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 31 */
	{  18,                   BU_O,    A_SPECIAL,  K_M,    0,   10,   20,     0, 0x1c,   9,  0x80,       3, SC300,       12,   4,  90,  90,   91,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        7, { 0, 0 } },	/* 32 */
	{  16,                   BU_O,    A_SPECIAL,  K_M,    0,   10,   20,     0, 0x1c,  11,  0x80,       3, SC300,       12,   2,  90,  92,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        7, { 0, 0 } },	/* 33 */
	{  10,          BU_O | NO_VEC,    A_SPECIAL,  K_M,    0,   10,   20,     0, 0x1c,  11,  0x80,       3, SC300,       12,   2,  90,  93,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        7, { 0, 0 } },	/* 34 */
	{   3,    ZU | NO_VEC | NO_ZU,      A_HYPER,  K_M,    0,    6,   20,     0, 0x1f,   2,  0x80,       3, SC300,       12,   0,  90, 102,  104,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 35: pl02_at_strike, Somersault Strike (hyper): the first two kicks */
	{   7,            21 | NO_VEC,      A_HYPER,  K_L,    0,    6,   20,     0, 0x1d,  13,  0x80,       3, SC300,       12,   0,  90, 102,  104,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 36: pl02_at_strike_end, Somersault Strike's last kick */
	{  10,            10 | NO_VEC,    A_SPECIAL,  K_L,    0,   10,   10,     0, 0x1f,  10,  0x60,       3, SC200,       12,   0,  90,  92,   93, NO_CHIP | AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        5, { 0, 0 } },	/* 37: pl02_at_t_kick, Tag-in jumping kick (t_kick) */
	{   4,           BU_O | NO_ZU,    A_SPECIAL,  K_L,    0,   10,   20,     0, 0x1e,  10,  0x80,       3, SC300,       12,   4,  30, 100,  120,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        5, { 0, 0 } },	/* 38 */
	{   4,           BU_O | NO_ZU,    A_SPECIAL,  K_L,    0,   10,   20,     0, 0x1e,  10,  0x80,       3, SC300,       12,   4,  30, 100,  120,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        5, { 0, 0 } },	/* 39 */
	{   5,             ZU | NO_ZU,      A_HYPER,  K_L,    0,    6,   20,     0, 0x1f,   0,  0x80,       3, SC300,       12,   4,  50, 105,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 3a: pl02_at_strike_crescent, Somersault Kick crescent from Somersault Strike (shl02_06 type 1) */
	{   4,    ZU | NO_VEC | NO_ZU,      A_HYPER,  K_L,    0,    1,   20,     0, 0x19,   6,  0x60,       3, SC200,        8,   0,  90,  84,    0,               NO_WEIGHT,  1,     0,  1,     0,     0,   5,        3, { 0, 0 } },	/* 3b: pl02_at_hurricane_3, Sonic Hurricane: every fourth of the boom's six-frame hits */
	{   4,    ZU | NO_VEC | NO_ZU,      A_HYPER,  K_L,    0,    1,   20,     0, 0x19,   6,  0x60,       3, SC200,        8,   0,  90,  84,    0,               NO_WEIGHT,  1,     0,  1,     0,     0,   5,        3, { 0, 0 } },	/* 3c: pl02_at_hurricane, Sonic Hurricane's start: on Guile, the boom's first hit; also shl02_04's repeated hit */
	{  15,                   BU_O,    A_SPECIAL,  K_M,    0,   10,   20,     0, 0x1c,  11,  0x80,       3, SC300,       12,   2,  90,  92,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        5, { 0, 0 } },	/* 3d: pl02_at_vc_smr, Somersault Kick as a variable counter */
	{   5,                      2,    A_SPECIAL,  K_M,    0,   10,   20,     0, 0x1c,  11,  0x80,       3, SC300,       12,   2,  90,  93,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        5, { 0, 0 } },	/* 3e */
	{   5,             ZU | NO_ZU,    A_SPECIAL,  K_L,    0,   10,   20,     0, 0x1e,  10,  0x80,       3, SC300,       12,   4,  90, 100,  120,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        5, { 0, 0 } },	/* 3f: pl02_at_vc_smr_crescent, Somersault Kick crescent as a variable counter (shl02_06) */
	{  10,            12 | NO_VEC,    A_SPECIAL,  K_S,    0,    4,   24,     0, 0x1c,  12,  0x80,       3, SC300,        8,  64,  90, 103,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 40: pl02_at_snapback, Snapback (sp_snapback) */
	{  20,     2 | NO_VEC | NO_ZU,      A_HYPER,  K_L,    0,    1,    0,     0, 0x1c,  10,  0x60,       3, SC200,        8,   0,  90,  84,    0,               NO_WEIGHT,  1,     0,  1,     0,     0,   5,        3, { 0, 0 } },	/* 41: pl02_at_hurricane_end, Sonic Hurricane's last hit */
	{   4,                     ZU,        A_NOM,  K_S,    4,   10,   10,     0, 0x18,   4,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,   6,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 42: pl02_at_far_lp, Far stand LP */
	{   7,                     ZU,        A_NOM, K_LM,    2,   10,   16,     0, 0x1d,   6,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40,   7,    8,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 43: pl02_at_far_mp, Stand MP (no close check; the MP slot of the 0x42-0x47 far block) */
	{  13,                     ZU,        A_NOM,  K_L,    1,   10,   24,     0, 0x1a,   4,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  50,   9,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 44: pl02_at_back_hp, Stand HP with the stick back */
	{   4,                     ZU,        A_NOM, K_LS,    4,   10,   10,     0, 0x18,   2,  0x60, MARK_N1, SC100, SD_KIK_S,   0,  20,  14,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 45: pl02_at_far_lk, Far stand LK */
	{   7,                     ZU,        A_NOM,  K_M,    2,   10,   16,     0, 0x1a,   3,  0x60, MARK_N2, SC200, SD_KIK_M,   0,  40,  15,   62,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 46: pl02_at_far_mk, Stand MK (no close check; the MK slot of the far block) */
	{  13,                     ZU,        A_NOM,  K_L,    1,   10,   24,     0, 0x1c,   4,  0x80, MARK_N3, SC300, SD_KIK_L,   0,  50,  18,   63,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 47: pl02_at_far_hk, Far stand HK */
	{   2,                      5,      A_HYPER, K_LL,    0,   10,   20,     0, 0x18,   0,  0x80,       3, SC300,       12,   0,  90, 108,    0,       NO_WEIGHT | NO_KO,  1,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 48: pl02_at_crossfire, Crossfire Assault (hyper) */
	{   2,                      5,      A_HYPER,  K_L,    0,    3,   20,     0, 0x18,   0,  0x80,       3, SC300,        8,   0,  90,  88,   52,       NO_WEIGHT | NO_KO,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 49 */
	{   2,                      5,      A_HYPER, K_LS,    0,    3,   20,     0, 0x18,   0,  0x80,       3, SC300,       12,   0,  90,  19,   48,       NO_WEIGHT | NO_KO,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 4a */
	{   2,                      5,      A_HYPER, K_LM,    0,   10,   20,     0, 0x18,   0,  0x80,       3, SC300,       12,   0,  90,  46,    0,       NO_WEIGHT | NO_KO,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 4b */
	{   2,                      5,      A_HYPER, K_LM,    0,    4,   20,     0, 0x00,   5,  0x80,       3, SC300,       12,   0,  90,  29,    0,       NO_WEIGHT | NO_KO,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 4c */
	{  10,         2 | FLY_SCREEN,      A_HYPER,  K_L,    0,   16,   20,     0, 0x18,  15,  0x80,       3, SC300,       12,   0,  90,  16,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 4d */
	{  25,         2 | FLY_SCREEN,      A_HYPER, K_LL,    0,   10,   20,     0, 0x1c,   9,  0x80,       3, SC300,       12,   4,  90,  90,   91,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        7, { 0, 0 } },	/* 4e: pl02_at_crossfire_crescent, Somersault Kick crescent from Crossfire Assault (shl02_06 type 2) */
	{   5, 2 | NO_ZU | FLY_SCREEN,      A_HYPER,  K_L,    0,   10,   20,     0, 0x1e,  10,  0x80,       3, SC300,       12,   4,  30, 100,  120,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        5, { 0, 0 } },	/* 4f */
	{   3,    ZU | NO_VEC | NO_ZU,      A_HYPER,  K_L,    0,    1,   20,     0, 0x19,   6,  0x60,       3, SC200,        8,   0,  90,  84,    0,     NO_CHIP | NO_WEIGHT,  1,     0,  1,     0,     0,   5,        3, { 0, 0 } },	/* 50: pl02_at_hurricane_2, Sonic Hurricane: the boom's other six-frame hits */
};
