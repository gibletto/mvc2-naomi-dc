/* Doctor Doom (character 0f): the attacks (HIT_DATA, 0x1c bytes).
   What each hit does. The character's module sets the attack's number (atck_no) with the move,
   and the row is read when the attack box meets a body. After a row is its number, in
   hexadecimal, and the program's name for it:
   ATCK_ for the normals every character has (player.h), pl0f_at_ for its own (pl0f_def.h).
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
     box, box2  boxes of hit_0f that the computer measures the attack's reach by
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

const HIT_DATA hit_dt_0f[] = {
	/* dmg                 type          kind      elem  piyo  stop  sand  sleep   vec  pow  range     mark  score        se  shc  jug  box  box2                     flag  13  guard  15  catch  abyss  cnt    answer        1a */
	{   4,         ZU | NO_VEC,        A_NOM,      K_S,    4,   12,    8,     0, 0x19,   8,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  15,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 00: ATCK_STAND_LP */
	{   7,         ZU | NO_VEC,        A_NOM,      K_M,    2,   12,   12,     0, 0x19,   8,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40,  20,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 01: ATCK_STAND_MP */
	{  13,       BU_O | NO_VEC,        A_NOM,  K_L | 3,    1,   12,   16,     0, 0x1b,  12,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  50,  25,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 02: ATCK_STAND_HP */
	{   4,         ZU | NO_VEC,        A_NOM,     K_LS,    4,   12,    8,     0, 0x19,   4,  0x60, MARK_N1, SC100, SD_KIK_S,   0,  20,  30,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 03: ATCK_STAND_LK */
	{   7,       BU_O | NO_VEC,        A_NOM,      K_M,    2,    8,   12,     0, 0x1f,  11,  0x60, MARK_N2, SC200, SD_KIK_M,   0,  40,  31,    0, NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 04: ATCK_STAND_MK */
	{   7,         ZU | NO_VEC,        A_NOM,      K_L,    1,   12,   16,     0, 0x19,   0,  0x80, MARK_N3, SC300, SD_KIK_L,   0,  50,  33,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 05: ATCK_STAND_HK */
	{   4,         ZU | NO_VEC,        A_NOM,     K_LS,    4,   12,    8,     0, 0x19,   8,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  39,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 06: ATCK_CROUCH_LP */
	{   7,                  ZU,        A_NOM,     K_LM,    2,   12,   12,     0, 0x19,   8,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40,  41,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 07: ATCK_CROUCH_MP */
	{  13,       BU_O | NO_VEC,        A_NOM, K_LL | 3,    1,   12,   16,     0, 0x00,  11,  0x80, MARK_N3, SC200, SD_NOR_L,   0,  50,  43,    0, NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 08: ATCK_CROUCH_HP */
	{   4,         ZU | NO_VEC,         A_LO,     K_LS,    4,   12,    8,     0, 0x18,   7,  0x70, MARK_N1, SC100, SD_KIK_S,   0,  20,  45,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 09: ATCK_CROUCH_LK */
	{   7,                  ZU,         A_LO,     K_LM,    2,   12,   12,     0, 0x18,   7,  0x40, MARK_N2, SC200, SD_KIK_M,   0,  40,  47,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0a: ATCK_CROUCH_MK */
	{  13,                  AS,         A_LO,     K_LL,    1,   12,   16,     5, 0x18,   9,  0x70, MARK_N3, SC300, SD_KIK_L,   0, 100,  49,    0,                       0,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 0b: ATCK_CROUCH_HK */
	{   4,         ZU | NO_VEC,         A_HI,      K_S,    1,   12,    8,     0, 0x00,   5,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  51,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0c: ATCK_JUMP_LP */
	{   7,         ZU | NO_VEC,         A_HI, K_LM | 3,    1,   12,   12,     0, 0x00,   4,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40,  53,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0d: ATCK_JUMP_MP */
	{   7, ZU | NO_VEC | NO_ZU,         A_HI,      K_L,    1,   12,   16,     0, 0x15,   6,  0x80, MARK_N3, SC300,       17,   0,  10,  93,    0,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0,        8, { 0, 0 } },	/* 0e: ATCK_JUMP_HP */
	{   4,         ZU | NO_VEC,         A_HI,      K_S,    1,   12,    8,     0, 0x00,   5,  0x60, MARK_N1, SC200, SD_KIK_S,   0,  20,  56,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0f: ATCK_JUMP_LK */
	{   7,         ZU | NO_VEC,         A_HI,      K_M,    1,   12,   12,     0, 0x1c,   5,  0x60, MARK_N2, SC200, SD_KIK_M,   0,  40,  58,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 10: ATCK_JUMP_MK */
	{  13,  2 | NO_VEC | NO_ZU,         A_HI,     K_LM,    1,    4,   12,     0, 0x18,  12,  0x60, MARK_N3, SC200, SD_KIK_L,   0,  60,  81,    0, NO_WEIGHT | AERIAL_FINISH,  0,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 11: ATCK_JUMP_HK */
	{  13,  2 | NO_VEC | NO_ZU,         A_HI,     K_LM,    1,    4,   12,     0, 0x1a,  12,  0x60, MARK_N3, SC200, SD_KIK_L,   0,  60,  88,    0, NO_WEIGHT | AERIAL_FINISH,  0,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 12: pl0f_at_air_hk, Jumping HK without stick-down (Char_Extra 3); the down+HK dive keeps ATCK_JUMP_HK */
	{ 0 },	/* 13 */
	{ 0 },	/* 14 */
	{ 0 },	/* 15 */
	{ 0 },	/* 16 */
	{ 0 },	/* 17 */
	{ 0 },	/* 18 */
	{   7,                  ZU,        A_NOM,     K_LL,    1,   12,   16,     0, 0x19,  10,  0x80, MARK_N3, SC300, SD_KIK_L,   0,  50,  38,    0,                       0,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 19: pl0f_at_stand_hk_2, Standing HK's second hit */
	{  13,       BU_O | NO_VEC,    A_SPECIAL, K_LL | 3,    0,   12,   16,     0, 0x00,  11,  0x80, MARK_N3, SC200, SD_NOR_L,   0,  40,  43,    0, NO_CHIP | NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 1a: pl0f_at_upper_p, Launcher, punch version (pl0f_upper, Char_Extra 6) */
	{   7,       BU_O | NO_VEC,    A_SPECIAL,      K_M,    0,    8,   12,     0, 0x1f,  11,  0x60, MARK_N2, SC200, SD_KIK_M,   0,  40,  31,    0, NO_CHIP | NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 1b: pl0f_at_upper_k, Launcher, kick version (pl0f_upper, atck_pk set, Char_Extra 7) */
	{ 0 },	/* 1c */
	{ 0 },	/* 1d */
	{ 0 },	/* 1e */
	{ 0 },	/* 1f */
	{  16,                   2, A_NOM | 0x40,     K_LM,    0,   16,   20,    15, 0x1c,   5,  0x60, MARK_N1, SC200, SD_NOR_M,   0,  40,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 20: pl0f_at_throw_p, Ground punch throw (nml_p_throw) */
	{  16,                BU_O, A_NOM | 0x40,     K_LM,    0,   16,   20,    15, 0x00,   7,  0x60, MARK_N1, SC200, SD_NOR_M,   0,  40,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 21: pl0f_at_throw_k, Ground kick throw (nml_k_catch) */
	{  16,                   2, A_NOM | 0x40,     K_LM,    0,   16,   20,    15, 0x1c,   5,  0x60, MARK_N1, SC200, SD_NOR_M,   0,  40,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 22: pl0f_at_air_throw_p, Air punch throw (jump_p_throw) */
	{  16,                   2, A_NOM | 0x40,     K_LM,    0,    8,   20,    15, 0x10,   1,  0x60, MARK_N2, SC200, SD_NOR_M,   0,  40,   0,    0,                       0,  0,     0, 10,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 23: pl0f_at_air_throw_k, Air kick throw (jump_k_catch) */
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
	{   6,        BU_O | NO_ZU,    A_SPECIAL,      K_L,    0,   10,   16,     0, 0x1e,   3,  0x80,       3, SC200,       17,   0,  30,  91,    0,                       0,  1,     0,  0,     0,     0,   2,        2, { 0, 0 } },	/* 30: pl0f_at_photon, Photon Shot's shots (shl0f_02 type 0) */
	{   6,        BU_O | NO_ZU,    A_SPECIAL,      K_L,    0,   10,   16,     0, 0x1e,   3,  0x80,       3, SC200,       17,   0,  30,  91,    0,                       0,  1,     0,  0,     0,     0,   2,        2, { 0, 0 } },	/* 31 */
	{  14,        BU_O | NO_ZU,    A_SPECIAL,      K_L,    0,   10,   16,     5, 0x1a,  13,  0x80,       3, SC300,       18,   0,  90,  84,    0,                       0,  1,     0,  0,     0,     0,   5,        8, { 0, 0 } },	/* 32 */
	{  14,        BU_O | NO_ZU,    A_SPECIAL,      K_L,    0,   10,   16,     5, 0x1a,  13,  0x80,       3, SC300,       18,   0,  90,  84,    0,                       0,  1,     0,  0,     0,     0,   5,        8, { 0, 0 } },	/* 33 */
	{  14,           2 | NO_ZU,    A_SPECIAL,      K_L,    0,   10,   16,     5, 0x1a,   8,  0x80,       3, SC200,       18,   0,  90,  84,    0,                       0,  1,     0,  0,     0,     0,   5,        8, { 0, 0 } },	/* 34 */
	{  14,           2 | NO_ZU,    A_SPECIAL,      K_L,    0,   10,   16,     5, 0x1a,   8,  0x80,       3, SC200,       18,   0,  90,  84,    0,                       0,  1,     0,  0,     0,     0,   5,        8, { 0, 0 } },	/* 35 */
	{   3,        BU_O | NO_ZU,    A_SPECIAL,     K_LL,    0,   10,   30,    10, 0x1f,  10,  0x80,       3, SC300,        8,   0,  15,  95,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        8, { 0, 0 } },	/* 36: pl0f_at_shield, Molecular Shield's rocks (shl0f_05) */
	{   3,        BU_O | NO_ZU,    A_SPECIAL,     K_LL,    0,   10,   30,    10, 0x1f,  10,  0x80,       3, SC300,        8,   0,  15,  95,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        8, { 0, 0 } },	/* 37 */
	{   5,          21 | NO_ZU,      A_HYPER,      K_L,    0,   10,   16,    10, 0x1e,   3,  0x80,       3, SC300,       17,   0,  90,  91,    0,                       0,  1,     0,  0,     0,     0,   4,        3, { 0, 0 } },	/* 38: pl0f_at_array, Photon Array (hyper): its shots (shl0f_02 type 1), also set on Doom when it starts in the air */
	{  10,         10 | NO_VEC,    A_SPECIAL,      K_L,    0,   10,   10,     0, 0x1f,  10,  0x60,       3, SC200,       12,   0,  90,  56,    0, NO_CHIP | NO_WEIGHT | AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 39: pl0f_at_t_kick, the partner's jump-in attack (pl0f_t_kick) */
	{   4,          ZU | NO_ZU,      A_HYPER,  K_L | 3,    0,    4,   20,     0, 0x1e,   0,  0x80,       3, SC300,        8,   0,  90,  76,   78,               NO_WEIGHT,  1,     0,  0,     0,     0,   6,        3, { 0, 0 } },	/* 3a: pl0f_at_cage, Electric Cage (hyper): the flying cage and its hits while it holds (cage_init, shl0f_00) */
	{  10,        BU_O | NO_ZU,      A_HYPER,  K_L | 3,    0,    4,   20,     0, 0x1e,  14,  0x80,       3, SC300,        8,   0,  90,  76,   78,               NO_WEIGHT,  1,     0,  0,     0,     0,   6,        3, { 0, 0 } },	/* 3b: pl0f_at_cage_last, Electric Cage's last hit (shl0f_00_hold, timer out) */
	{   4,        BU_O | NO_ZU,      A_HYPER,      K_L,    0,    4,   20,     0, 0x00,   6,  0x80,       3, SC300,        8,   0,  90,  79,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   8,        3, { 0, 0 } },	/* 3c: pl0f_at_flame, Sphere Flame (hyper): a hit every 8 frames (shl0f_01) */
	{  10,        BU_O | NO_ZU,      A_HYPER,      K_L,    0,    4,   20,     0, 0x1e,  10,  0x80,       3, SC300,        8,   0,  90,  79,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   8,        3, { 0, 0 } },	/* 3d: pl0f_at_flame_last, Sphere Flame's last hit (shl0f_01_hit, timer0 8) */
	{  10,         12 | NO_VEC,    A_SPECIAL,      K_L,    1,    4,   24,     0, 0x1c,  12,  0x80,       3, SC300,        8,   0, 100,  25,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 3e: pl0f_at_snapback, Snapback (sp_snapback) */
	{   6,          ZU | NO_ZU,    A_SPECIAL,      K_L,    0,    4,   16,     0, 0x18,   8,  0x80,       3, SC200,       17,   0,  90,  97,    0,                       0,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 3f: pl0f_at_photon_gnd, Set on Doom as Photon Shot starts on the ground (photon_gnd_init); the shots use 0x30 */
	{   4,        BU_O | NO_ZU,      A_HYPER,      K_L,    0,   10,   20,     0, 0x00,  12,  0x80,       3, SC300,        8,   0,  90,  74,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 40: pl0f_at_flame_start, Set on Doom as Sphere Flame starts (flame_init) */
	{   8,          ZU | NO_ZU,      A_HYPER,      K_L,    0,    4,   16,     0, 0x18,   8,  0x80,       3, SC200,       17,  64,  90,  97,    0,                       0,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 41: pl0f_at_array_gnd, Set on Doom as Photon Array starts on the ground (array_gnd_init); the shots use 0x38 */
};
