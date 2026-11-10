/* Iron Man (character 33): the attacks (HIT_DATA, 0x1c bytes).
   What each hit does. The character's module sets the attack's number (atck_no) with the move,
   and the row is read when the attack box meets a body. After a row is its number, in
   hexadecimal, and the program's name for it:
   ATCK_ for the normals every character has (player.h), pl33_at_ for its own (pl33_def.h).
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
     box, box2  boxes of hit_33 that the computer measures the attack's reach by
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

const HIT_DATA hit_dt_33[] = {
	/* dmg                type              kind     elem  piyo  stop  sand  sleep   vec  pow  range     mark  score  se  shc  jug  box  box2                     flag  13  guard  15  catch  abyss  cnt    answer        1a */
	{   4,        ZU | NO_VEC,            A_NOM,     K_S,    4,   10,   10,     0, 0x18,   9,  0x70, MARK_N1, SC100,  1,   0,  20,  12,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 00: ATCK_STAND_LP */
	{   7,                 ZU,            A_NOM,     K_M,    2,   11,   16,     0, 0x1a,   9,  0x80, MARK_N2, SC200,  2,   0,  40,  14,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 01: ATCK_STAND_MP */
	{  13,       BU_O | NO_ZU,            A_NOM,     K_L,    1,   10,   24,     0, 0x1a,  12,  0x80, MARK_N3, SC300,  4,   0,  50,  62,   66,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 02: ATCK_STAND_HP */
	{   4,        ZU | NO_VEC,            A_NOM,     K_S,    4,   10,   10,     0, 0x19,   8,  0x60, MARK_N1, SC100,  1,   0,  20,  22,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 03: ATCK_STAND_LK */
	{   7,                 ZU,            A_NOM,     K_M,    2,   11,   16,     0, 0x1a,  10,  0x60, MARK_N2, SC200,  2,   0,  40,  69,   70,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 04: ATCK_STAND_MK */
	{  12,      BU_O | NO_VEC,            A_NOM,     K_L,    1,   10,   24,     0, 0x00,  11,  0x80, MARK_N3, SC300,  3,   0,  50,  31,    0, NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 05: ATCK_STAND_HK */
	{   3,        ZU | NO_VEC,            A_NOM,    K_LS,    4,   10,   10,     0, 0x18,  11,  0x70, MARK_N1, SC100,  1,   0,  20,  32,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 06: ATCK_CROUCH_LP */
	{   7,      BU_O | NO_VEC,            A_NOM,    K_LM,    2,   10,   16,     0, 0x00,   6,  0x80, MARK_N2, SC200,  2,   0,  40,  36,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 07: ATCK_CROUCH_MP */
	{  13,         ZU | NO_ZU,            A_NOM,    K_LL,    1,   10,   24,     0, 0x1c,   7,  0x80, MARK_N3, SC300,  3,   0,  50,  71,    0,                       0,  0,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 08: ATCK_CROUCH_HP */
	{   4,        ZU | NO_VEC,             A_LO,    K_LS,    4,   10,   10,     0, 0x18,   8,  0x70, MARK_N1, SC100,  1,   0,  20,  38,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 09: ATCK_CROUCH_LK */
	{   7,                 AS,             A_LO,    K_LM,    2,   11,   16,     8, 0x1d,   5,  0x40, MARK_N2, SC200,  2,   0, 100,  67,   68,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0a: ATCK_CROUCH_MK */
	{   9,               BU_O,            A_NOM,    K_LL,    1,   11,   24,     0, 0x1a,  10,  0x70, MARK_N3, SC300,  3,   0,  32,  41,   42,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0b: ATCK_CROUCH_HK */
	{   4,        ZU | NO_VEC,             A_HI,     K_S,    1,   10,   10,     0, 0x1f,   3,  0x70, MARK_N1, SC100,  1,   0,  20,  45,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0c: ATCK_JUMP_LP */
	{   7,        ZU | NO_VEC,             A_HI,     K_M,    1,   10,   16,     0, 0x1f,   3,  0x80, MARK_N2, SC200,  2,   0,  40,  73,   74,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0d: ATCK_JUMP_MP */
	{  13,        ZU | NO_VEC,            A_NOM,     K_M,    1,   12,   32,     0, 0x17,   4,  0x80, MARK_N3, SC300,  3,   0,  60,  47,    0,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0e: ATCK_JUMP_HP */
	{   4,        ZU | NO_VEC,             A_HI,     K_S,    1,   10,   10,     0, 0x00,   8,  0x60, MARK_N1, SC100,  1,   0,  20,  49,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0f: ATCK_JUMP_LK */
	{   7,        ZU | NO_VEC,             A_HI,     K_M,    1,   10,   16,     0, 0x1a,   4,  0x60, MARK_N2, SC100,  1,   0,  40,  51,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 10: ATCK_JUMP_MK */
	{  13,        ZU | NO_VEC,             A_HI,     K_M,    1,   12,   32,     0, 0x14,   4,  0x80, MARK_N3, SC300,  3,   0,  60,  53,    0,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 11: ATCK_JUMP_HK */
	{  13,        ZU | NO_VEC,            A_NOM,     K_M,    1,   12,   24,     0, 0x1e,   3,  0x80, MARK_N3, SC300,  3,   0,  60,  55,  129,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 12: pl33_at_jup_hp, Air heavy punch with up */
	{  13,        ZU | NO_VEC,            A_NOM,     K_M,    1,   12,   32,     0, 0x14,   4,  0x80, MARK_N3, SC300,  3,   0,  60,  57,  130,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 13: pl33_at_jdown_hp, Air heavy punch with down */
	{  13,      BU_O | NO_VEC,             A_HI,     K_M,    1,   12,   24,     0, 0x00,   8,  0x80, MARK_N3, SC300,  3,   0,  60,  59,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 14: pl33_at_jup_hk, Air heavy kick with up */
	{  12,        ZU | NO_VEC,             A_HI,     K_M,    1,   12,   16,     0, 0x1e,   4,  0x60, MARK_N2, SC200,  2,   0,  40,  27,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 15: pl33_at_jdown_hk, Air heavy kick with down, high enough: the dive */
	{ 0 },	/* 16 */
	{ 0 },	/* 17 */
	{ 0 },	/* 18 */
	{ 0 },	/* 19 */
	{ 0 },	/* 1a */
	{   8,               BU_O,            A_NOM,    K_LL,    1,   11,   24,    30, 0x1a,  10,  0x70, MARK_N3, SC300,  3,   0,  20,  42,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 1b */
	{  12,      BU_O | NO_VEC,        A_SPECIAL,     K_L,    0,   10,   24,     0, 0x00,  11,  0x80, MARK_N3, SC300,  3,   0,  40,  31,    0, NO_CHIP | NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 1c: pl33_at_upper, the launcher (pl33_upper) */
	{ 0 },	/* 1d */
	{ 0 },	/* 1e */
	{ 0 },	/* 1f */
	{  16,                  2,     A_NOM | 0x40,       0,    4,   16,   20,    15, 0x1a,  14,  0x80, MARK_N1, SC300, 76,   0,   0,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 20: pl33_at_throw_k, Ground kick throw */
	{  16,                  2,     A_NOM | 0x40,       0,    4,   16,   20,    15, 0x15,  15,  0x80, MARK_N1, SC300, 76,   0,   0,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 21: pl33_at_air_throw, Air punch throw */
	{  16,                  2,     A_NOM | 0x40,       0,    4,   16,   20,    15, 0x19,  15,  0x80, MARK_N1, SC300, 76,   0,   0,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 22: pl33_at_throw_p, Ground punch throw */
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
	{   4,         ZU | NO_ZU,        A_SPECIAL,     K_S,    0,   10,   16,    15, 0x1e,  10,  0x80,       4, SC300, 17,   0,  14,  87,   88,               NO_WEIGHT,  1,     0,  0,     0,    25,   2,        8, { 0, 0 } },	/* 30 */
	{   3,         ZU | NO_ZU,        A_SPECIAL,    K_LS,    0,   10,   16,    15, 0x1e,  10,  0x80,       4, SC300, 17,   0,   4, 133,  135,               NO_WEIGHT,  1,     0,  0,     0,    18,   2,        8, { 0, 0 } },	/* 31 */
	{   9,         ZU | NO_ZU, A_SPECIAL | 0x80, K_L | 2,    0,   10,   16,     0, 0x1d,   8,  0x80,       3, SC300,  3,   0,  30, 105,    0,                       0,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 32: pl33_at_bomb, Smart Bomb, ground and air (him and the bomb, shl33_02) */
	{  12,          2 | NO_ZU, A_SPECIAL | 0x80,     K_S,    0,   16,   16,    15, 0x03,   8,  0x80,       3, SC300, 17,   0, 100,  82,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   3,        2, { 0, 0 } },	/* 33: pl33_at_repulsor, Repulsor Blast (sp_repulsor) */
	{   2,                  2,          A_HYPER,    K_LL,    0,    1,   16,     0, 0x19,  15,  0x80,       4, SC300, 18,   0,  90,  81,   82,     NO_CHIP | NO_WEIGHT,  1,     0, 13,     0,     0,   6,        3, { 0, 0 } },	/* 34: pl33_at_proton_beam, Proton Cannon's beams (shl33_04) */
	{   6,                 ZU,        A_SPECIAL,     K_S,    0,   10,   16,    15, 0x1e,  10,  0x80,       4, SC300, 17,   0,   0,  87,   88,               NO_WEIGHT,  1,     0,  0,     0,     0,   2,        8, { 0, 0 } },	/* 35 */
	{   4,                 ZU,        A_SPECIAL,     K_S,    0,   10,   16,    15, 0x1e,  10,  0x80,       4, SC300, 17,   0,   0,  87,   88,               NO_WEIGHT,  1,     0,  0,     0,     0,   2,        8, { 0, 0 } },	/* 36 */
	{   4, 2 | NO_VEC | NO_ZU,          A_HYPER,     K_S,    0,   10,   16,     0, 0x1e,  11,  0x80,       4, SC300, 18,   0,  80,  97,    0,               NO_WEIGHT,  1,     0, 13,     0,     0,   6,        3, { 0, 0 } },	/* 37 */
	{   3, 2 | NO_VEC | NO_ZU,          A_HYPER,    K_LS,    0,   10,   16,     0, 0x1e,  11,  0x80,       4, SC300, 18,   0,  80,  97,    0,     NO_CHIP | NO_WEIGHT,  1,     0, 13,     0,     0,   6,        3, { 0, 0 } },	/* 38 */
	{   4, 2 | NO_VEC | NO_ZU,          A_HYPER,     K_M,    0,   10,   16,     0, 0x1e,  11,  0x80,       4, SC300, 18,   0,  80,  97,    0,               NO_WEIGHT,  1,     0, 13,     0,     0,   6,        3, { 0, 0 } },	/* 39 */
	{   3, 2 | NO_VEC | NO_ZU,          A_HYPER,    K_LM,    0,   10,   16,     0, 0x1e,  11,  0x80,       4, SC300, 18,   0,  80,  97,    0,     NO_CHIP | NO_WEIGHT,  1,     0, 13,     0,     0,   6,        3, { 0, 0 } },	/* 3a */
	{  12,          2 | NO_ZU, A_SPECIAL | 0x80,     K_S,    0,   10,   16,    15, 0x03,   8,  0x80,       3, SC300, 17,   0, 100,  83,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   3,        2, { 0, 0 } },	/* 3b */
	{  12,          2 | NO_ZU, A_SPECIAL | 0x80,     K_S,    0,   10,   16,    15, 0x03,   8,  0x80,       3, SC300, 17,   0, 100,  81,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   3,        2, { 0, 0 } },	/* 3c */
	{   1,      BU_O | NO_VEC,          A_HYPER,     K_S,    0,    0,   24,     0, 0x1c,   5,  0x80,       3, SC300,  1,   0, 100,  98,    0,       NO_WEIGHT | NO_KO,  1,     0, 13,     0,     0,   0,        3, { 0, 0 } },	/* 3d: pl33_at_proton, Proton Cannon (hyper, sp_proton): him and the cannon that drops (shl33_03) */
	{ 0 },	/* 3e */
	{  10,        10 | NO_VEC,        A_SPECIAL,     K_L,    0,   10,   10,     0, 0x1f,  10,  0x60,       3, SC200, 12,   0,  90, 117,    0, NO_CHIP | NO_WEIGHT | AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 3f: pl33_at_t_kick, the partner's jump-in attack (t_kick), a dash through the air */
	{ 0 },	/* 40 */
	{ 0 },	/* 41 */
	{   4,                 ZU,        A_SPECIAL,     K_S,    0,   10,   16,    15, 0x1e,  10,  0x80,       4, SC300, 17,   0,   0,  87,   88,               NO_WEIGHT,  1,     0,  0,     0,     0,   2,        8, { 0, 0 } },	/* 42 */
	{ 0 },	/* 43 */
	{ 0 },	/* 44 */
	{ 0 },	/* 45 */
	{ 0 },	/* 46 */
	{  10,        12 | NO_VEC,        A_SPECIAL,     K_L,    0,    4,   24,     0, 0x1c,  12,  0x80,       3, SC300, 18,  64, 100,  62,   66,               NO_WEIGHT,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 47: pl33_at_snapback, Snapback (sp_snapback) */
};
