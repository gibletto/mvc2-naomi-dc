/* Psylocke (character 08): the attacks (HIT_DATA, 0x1c bytes).
   What each hit does. The character's module sets the attack's number (atck_no) with the move,
   and the row is read when the attack box meets a body. After a row is its number, in
   hexadecimal, and the program's name for it:
   ATCK_ for the normals every character has (player.h), pl08_at_ for its own (pl08_def.h).
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
     box, box2  boxes of hit_08 that the computer measures the attack's reach by
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

const HIT_DATA hit_dt_08[] = {
	/* dmg            type              kind  elem  piyo  stop  sand  sleep   vec  pow  range     mark  score        se  shc  jug  box  box2                     flag  13  guard  15  catch  abyss  cnt    answer        1a */
	{   3,    ZU | NO_VEC,            A_NOM,  K_S,    4,   10,   10,     0, 0x18,   8,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  13,  193,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 00: ATCK_STAND_LP */
	{   6,             ZU,            A_NOM, K_LM,    2,   10,   16,     0, 0x1a,   8,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40,   7,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 01: ATCK_STAND_MP */
	{  16,           BU_O,            A_NOM,  K_L,    1,   10,   24,    10, 0x1b,  12,  0x80, MARK_N3, SC300,       15,   0,  50,  16,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 02: ATCK_STAND_HP */
	{   3,    ZU | NO_VEC,            A_NOM,  K_S,    4,   10,   10,     0, 0x19,   8,  0x60, MARK_N1, SC100, SD_KIK_S,   0,  20,  20,   21,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 03: ATCK_STAND_LK */
	{   6,             ZU,            A_NOM, K_LM,    2,   10,   16,     0, 0x1a,   5,  0x60, MARK_N2, SC200, SD_KIK_M,   0,  40,  26,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 04: ATCK_STAND_MK */
	{  12,             ZU,            A_NOM,  K_L,    1,   10,   24,     0, 0x19,  10,  0x80, MARK_N3, SC300, SD_KIK_L,   0,  50,  33,   34,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 05: ATCK_STAND_HK */
	{   3,    ZU | NO_VEC,            A_NOM, K_LS,    4,   10,   10,     0, 0x18,   4,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  38,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 06: ATCK_CROUCH_LP */
	{   6,             ZU,            A_NOM, K_LM,    2,   10,   16,     0, 0x1d,   6,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40,  40,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 07: ATCK_CROUCH_MP */
	{  12,  BU_O | NO_VEC,            A_NOM, K_LL,    1,   10,   24,     0, 0x00,  11,  0x80, MARK_N3, SC300,       15,   0,  50,  45,   44, NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 08: ATCK_CROUCH_HP */
	{   3,             ZU,             A_LO, K_LS,    4,   10,   10,     0, 0x18,   4,  0x70, MARK_N1, SC100, SD_KIK_S,   0,  20,  49,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 09: ATCK_CROUCH_LK */
	{   6,  BU_O | NO_VEC,            A_NOM, K_LM,    2,   10,   16,     0, 0x1f,   6,  0x40, MARK_N2, SC200, SD_KIK_M,   0,  40,  51,   52,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0a: ATCK_CROUCH_MK */
	{  12,             AS,             A_LO, K_LL,    1,   10,   24,     8, 0x18,  11,  0x70, MARK_N3, SC300, SD_KIK_L,   0, 100,  56,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0b: ATCK_CROUCH_HK */
	{   3,    ZU | NO_VEC,             A_HI,  K_S,    1,   10,   10,     0, 0x00,   5,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  58,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0c: ATCK_JUMP_LP */
	{   6,    ZU | NO_VEC,             A_HI,  K_M,    1,   10,   16,     0, 0x00,   2,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40,  60,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0d: ATCK_JUMP_MP */
	{  12,    ZU | NO_VEC,             A_HI, K_LM,    1,   10,   24,     0, 0x13,  10,  0x80, MARK_N3, SC300,       15,   0,  60,  62,    0,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0e: ATCK_JUMP_HP */
	{   3,    ZU | NO_VEC,             A_HI,  K_S,    1,   10,   10,     0, 0x00,   5,  0x60, MARK_N1, SC100, SD_KIK_S,   0,  20,  64,  194,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0f: ATCK_JUMP_LK */
	{   6,    ZU | NO_VEC,             A_HI,  K_M,    1,   10,   16,     0, 0x00,   2,  0x60, MARK_N2, SC200, SD_KIK_M,   0,  40,  66,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 10: ATCK_JUMP_MK */
	{  12,    ZU | NO_VEC,             A_HI, K_LM,    1,   10,   24,     0, 0x14,   8,  0x80, MARK_N3, SC300, SD_KIK_L,   0,  60,  68,   25,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 11: ATCK_JUMP_HK */
	{   6,  BU_O | NO_VEC,             A_HI,  K_L,    1,   10,   24,     0, 0x1e,   8,  0x80, MARK_N3, SC300, SD_KIK_L,   0,  40, 146,   72,               NO_WEIGHT,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 12: pl08_at_jup_hk, Jumping HK with the stick up */
	{  12,             ZU,            A_NOM,  K_L,    1,   10,   24,     0, 0x19,   9,  0x80, MARK_N3, SC300, SD_KIK_L,   0,  50,  33,   34,                       0,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 13: pl08_at_back_hk, Standing HK with the stick back: a flip */
	{  12,             ZU,            A_NOM,  K_L,    1,   10,   16,     0, 0x19,   5,  0x80, MARK_N3, SC300, SD_KIK_L,   0,  50, 155,    0,                       0,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 14: pl08_at_fwd_hk, Standing HK with the stick forward: a hop */
	{   6,  BU_O | NO_VEC,             A_HI,  K_L,    1,   10,   24,     0, 0x1e,  10,  0x80, MARK_N3, SC300, SD_KIK_L,   0,  40, 146,   72,               NO_WEIGHT,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 15: pl08_at_jump_hk_2, Jumping HK's second hit */
	{ 0 },	/* 16 */
	{ 0 },	/* 17 */
	{ 0 },	/* 18 */
	{   3,    ZU | NO_VEC,            A_NOM,  K_S,    2,   10,   10,     0, 0x18,  12,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  13,  193,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 19 */
	{   3,    ZU | NO_VEC,             A_LO,  K_S,    1,   10,   10,     0, 0x18,  12,  0x70, MARK_N1, SC100, SD_KIK_S,   0,  20,  49,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 1a */
	{   3,    ZU | NO_VEC,            A_NOM,  K_S,    2,   10,   10,     0, 0x18,  12,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  38,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 1b */
	{ 0 },	/* 1c */
	{ 0 },	/* 1d */
	{ 0 },	/* 1e */
	{ 0 },	/* 1f */
	{  16,           BU_O,     A_NOM | 0x40,    0,    0,   16,    0,    15, 0x1c,  12,  0x80, MARK_N1, SC300,       76,   0,   0,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 20: pl08_at_throw_p, the punch throw (nml_p_catch), also the air throw (jump_k_catch), set on the opponent */
	{  16,           BU_O,     A_NOM | 0x40,    0,    0,   16,    0,    15, 0x02,  10,  0x80, MARK_N1, SC300,       76,   0,   0,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 21: pl08_at_throw_k, the kick throw (nml_k_catch), set on the opponent */
	{ 0 },	/* 22: pl08_at_throw_3, the throw of catch3 (on target_w); nothing in s_pl08 sets catch_no 3 */
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
	{   4,      2 | NO_ZU,   A_HYPER | 0x80,  K_L,    0,    1,   16,    20, 0x00,   3,  0x80,       3, SC300, SD_NOR_M,   0,  90, 125,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 30: pl08_at_butterfly, Kocho-Gakure (hyper): her start and the even blades (shl08_01) */
	{   1,             ZU,        A_SPECIAL,  K_L,    2,   10,   16,     0, 0x1a,   1,  0x80, MARK_N1, SC100, SD_NOR_L,   4,  10,  30,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 31: pl08_at_chouhatsu, the taunt, which has a hit */
	{   5,              2,   A_HYPER | 0x80,  K_L,    0,    2,   16,    15, 0x1f,   1,  0x80,       3, SC300,       15,   0,  90, 100,    0,                       0,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 32 */
	{   5,              2,   A_HYPER | 0x80,  K_L,    0,    2,   16,    15, 0x1f,   3,  0x80,       3, SC300,       15,   0,  90, 112,    0,                       0,  1,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 33 */
	{   5,              2,   A_HYPER | 0x80,  K_L,    0,    2,   16,    15, 0x00,   1,  0x80,       3, SC300,       15,   0,  90, 134,    0,                       0,  1,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 34 */
	{   5,              2,   A_HYPER | 0x80,  K_L,    0,    1,   16,    15, 0x02,   3,  0x80,       3, SC300,       15,   0,  90, 136,    0,                       0,  1,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 35 */
	{   5,      2 | NO_ZU,   A_HYPER | 0x80,  K_L,    0,    2,   16,    15, 0x02,   1,  0x80,       3, SC300,       15,   0,  90, 138,    0,                       0,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 36: pl08_at_thrust, Psy-Thrust (hyper): the start, before the dashes */
	{   5,              2,   A_HYPER | 0x80,  K_L,    0,    2,   16,    15, 0x06,   1,  0x80,       3, SC300,       15,   0,  90, 140,  243,                       0,  1,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 37 */
	{   5,              2,   A_HYPER | 0x80,  K_L,    0,    2,   16,    15, 0x10,   1,  0x80,       3, SC300,       15,   0,  90, 142,    0,                       0,  1,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 38 */
	{   5,              2,   A_HYPER | 0x80,  K_L,    0,    2,   16,    15, 0x1a,   1,  0x80,       3, SC300,       15,   0,  90, 144,    0,                       0,  1,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 39 */
	{  14,     ZU | NO_ZU, A_SPECIAL | 0x80,  K_M,    0,   10,   24,     0, 0x1a,  11,  0x80,       3, SC300,       18,   0,  50,  74,    0,                       0,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 3a */
	{  14,     ZU | NO_ZU, A_SPECIAL | 0x80,  K_M,    0,   10,   24,     0, 0x1d,  11,  0x80,       3, SC300,       18,   0,  50,  75,    0,                       0,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 3b */
	{  14,     ZU | NO_ZU, A_SPECIAL | 0x80,  K_M,    0,   10,   24,     0, 0x15,  11,  0x80,       3, SC300,       18,   0,  50, 195,    0,                       0,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 3c */
	{  14,     ZU | NO_ZU, A_SPECIAL | 0x80,  K_L,    0,   10,   24,     0, 0x1a,  10,  0x80,       3, SC300,       18,   0,  50, 197,    0,                       0,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 3d: pl08_at_blast_h, Psy-Blast: heavy from the ground (0x3b + level), and light in the air */
	{  14,     ZU | NO_ZU, A_SPECIAL | 0x80,  K_L,    0,   10,   24,     0, 0x1d,  10,  0x80,       3, SC300,       18,   0,  50,  75,    0,                       0,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 3e */
	{  14,     ZU | NO_ZU, A_SPECIAL | 0x80,  K_L,    0,   10,   24,     0, 0x15,  10,  0x80,       3, SC300,       18,   0,  50,  76,    0,                       0,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 3f: pl08_at_jblast, Psy-Blast in the air, M and H */
	{   7,              2, A_SPECIAL | 0x80,  K_S,    0,    4,   20,    10, 0x1f,   2,  0x80,       3, SC300,       15,   0,  90, 160,  163,                       0,  1,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 40 */
	{   7,              2, A_SPECIAL | 0x80,  K_S,    0,    4,   20,    10, 0x1f,   1,  0x80,       3, SC300,       15,   0,  90, 160,  163,                       0,  1,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 41 */
	{   7,              2, A_SPECIAL | 0x80,  K_S,    0,    4,   20,    10, 0x1f,   1,  0x80,       3, SC300,       15,   0,  90,  77,   51,                       0,  1,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 42 */
	{   7,              2, A_SPECIAL | 0x80,  K_S,    0,    4,   20,    10, 0x1f,   1,  0x80,       3, SC300,       15,   0,  90,  77,   51,                       0,  1,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 43 */
	{   7,              2, A_SPECIAL | 0x80,  K_S,    0,    4,   20,    10, 0x00,   1,  0x80,       3, SC300,       15,   0,  90,  77,   81,                       0,  1,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 44 */
	{   7,              2, A_SPECIAL | 0x80,  K_S,    0,    4,   20,    10, 0x00,   1,  0x80,       3, SC300,       15,   0,  90,  81,  121,                       0,  1,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 45 */
	{  18,              2, A_SPECIAL | 0x80,  K_L,    0,   16,   16,    32, 0x1d,  10,  0x80,       3, SC300,       15,   0,  90, 103,    0,                       0,  1,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 46 */
	{  18,              2, A_SPECIAL | 0x80,  K_L,    0,   16,   16,    32, 0x1e,  10,  0x80,       3, SC300,       15,   0,  90, 101,    0,                       0,  1,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 47 */
	{  18,              2, A_SPECIAL | 0x80,  K_L,    0,   16,   16,    32, 0x00,  10,  0x80,       3, SC300,       15,   0,  90,  97,    0,                       0,  1,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 48 */
	{  18,              2, A_SPECIAL | 0x80,  K_L,    0,   16,   16,    15, 0x02,  10,  0x80,       3, SC300,       15,   0,  90, 118,    0,                       0,  1,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 49 */
	{  18,              2, A_SPECIAL | 0x80,  K_L,    0,   16,   16,    32, 0x03,  10,  0x80,       3, SC300,       15,   0,  90, 115,    0,                       0,  1,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 4a */
	{  18,              2, A_SPECIAL | 0x80,  K_L,    0,   16,   16,    32, 0x06,  10,  0x80,       3, SC300,       15,   0,  90, 113,    0,                       0,  1,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 4b */
	{  18,              2, A_SPECIAL | 0x80,  K_L,    0,   16,   16,    32, 0x10,  10,  0x80,       3, SC300,       15,   0,  90, 109,    0,                       0,  1,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 4c */
	{  18,              2, A_SPECIAL | 0x80,  K_L,    0,   16,   16,    32, 0x1a,  10,  0x80,       3, SC300,       15,   0,  90, 107,    0,                       0,  1,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 4d */
	{   2,              2,   A_HYPER | 0x80,  K_L,    0,    3,   20,    10, 0x00,   0,  0x80,       3, SC300,       15,   0,  90, 171,  172,               NO_WEIGHT,  1,     0,  0,     0,     0,   4,        2, { 0, 0 } },	/* 4e: pl08_at_maelstrom, Psy-Maelstrom (hyper): the repeated hits */
	{  10, 2 | FLY_SCREEN,   A_HYPER | 0x80,  K_L,    0,    3,   20,    20, 0x16,  15,  0x80,       3, SC300,        8,   0,  90, 172,  172,    NO_WEIGHT | NO_SCALE,  1,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 4f: pl08_at_maelstrom_end, Psy-Maelstrom: the last hit */
	{  10,     2 | NO_VEC,            A_NOM,  K_L,    0,   10,   16,     0, 0x1f,   9,  0x80, MARK_N2, SC300, SD_KIK_L,   0,  40, 146,   48,                       0,  1,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 50: pl08_at_leap, the leaping slash */
	{  10,    10 | NO_VEC,        A_SPECIAL,  K_L,    0,   10,   10,     0, 0x1f,  10,  0x60,       3, SC200,       12,   0,  90, 210,    0, NO_CHIP | NO_WEIGHT | AERIAL_FINISH,  1,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 51: pl08_at_t_kick, the jump-in kick when she is tagged in (t_kick) */
	{  10,    12 | NO_VEC,        A_SPECIAL,  K_L,    1,    4,   24,     0, 0x1c,  12,  0x80,       3, SC300,       12,  64,  90, 155,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 52: pl08_at_snapback, Snapback */
	{  14,  BU_O | NO_VEC,        A_SPECIAL,  K_L,    0,   10,   16,     0, 0x00,  11,  0x80, MARK_N2, SC200,       15,   0,  40, 211,   45, NO_CHIP | NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 53: pl08_at_upper, the launcher, a crouching punch */
	{   9,  BU_O | NO_VEC,        A_SPECIAL,  K_L,    0,   10,   16,     0, 0x1f,   8,  0x40, MARK_N2, SC200,       12,   0,  40, 212,   52, NO_CHIP | NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 54 */
	{  18,     ZU | NO_ZU, A_SPECIAL | 0x80,  K_L,    0,   10,   24,     0, 0x1d,  11,  0x80,       3, SC300,       18,  64,  90,  75,    0,                       0,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 55: pl08_at_blast_vc, Psy-Blast as a variable counter (team_type 3) */
	{   7,              2, A_SPECIAL | 0x80,  K_S,    0,    4,   20,    10, 0x00,   1,  0x80,       3, SC300,       15,  64,  90,  77,   81,                       0,  1,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 56: pl08_at_spin_vc, Psy-Blade as a variable counter: the first part */
	{   7,              2, A_SPECIAL | 0x80,  K_S,    0,    4,   20,    10, 0x00,   1,  0x80,       3, SC300,       15,  64,  90,  81,  121,                       0,  1,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 57: pl08_at_spin_vc_2, Psy-Blade as a variable counter: the follow-up */
	{   4,      2 | NO_ZU,          A_HYPER,  K_L,    0,    1,   16,    20, 0x00,   3,  0x80,       3, SC300, SD_NOR_M,   0,  90, 125,    0,     NO_CHIP | NO_WEIGHT,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 58: pl08_at_butterfly_2, Kocho-Gakure: the odd blades (shl08_01 type & 1) */
};
