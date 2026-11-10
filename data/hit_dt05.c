/* Strider Hiryu (character 05): the attacks (HIT_DATA, 0x1c bytes).
   What each hit does. The character's module sets the attack's number (atck_no) with the move,
   and the row is read when the attack box meets a body. After a row is its number, in
   hexadecimal, and the program's name for it:
   ATCK_ for the normals every character has (player.h), pl05_at_ for its own (pl05_def.h).
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
     box, box2  boxes of hit_05 that the computer measures the attack's reach by
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

const HIT_DATA hit_dt_05[] = {
	/* dmg                   type              kind     elem  piyo  stop  sand  sleep   vec  pow  range     mark  score        se  shc  jug  box  box2                     flag  13  guard  15  catch  abyss  cnt    answer        1a */
	{   3,                    ZU,            A_NOM,     K_S,    4,   10,   10,     0, 0x1a,   4,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  16,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 00: ATCK_STAND_LP */
	{   6,                    ZU,            A_NOM,     K_M,    2,   10,   16,     0, 0x1a,   6,  0x80,      13, SC200,       14,   0,  40,  17,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 01: ATCK_STAND_MP */
	{  12,                  BU_O,            A_NOM,    K_LL,    1,   10,   24,     0, 0x1d,   9,  0x80,      12, SC300,       15,   0,  50,  18,   76,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 02: ATCK_STAND_HP */
	{   3,                    ZU,            A_NOM,     K_S,    4,   10,   10,     0, 0x1a,   5,  0x60, MARK_N1, SC100, SD_KIK_S,   0,  20,  19,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 03: ATCK_STAND_LK */
	{   6,                    ZU,            A_NOM,     K_S,    2,   10,   16,     0, 0x1a,   7,  0x60,      14, SC200,       14,   0,  40,  20,  144,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 04: ATCK_STAND_MK */
	{  12,                  BU_O,            A_NOM,    K_LL,    1,   10,   24,     0, 0x1d,   9,  0x80,      14, SC300,       15,   4,  50,  21,   11,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 05: ATCK_STAND_HK */
	{   3,           ZU | NO_VEC,            A_NOM,    K_LS,    4,   10,   10,     0, 0x1a,   4,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  22,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 06: ATCK_CROUCH_LP */
	{   6,                    ZU,            A_NOM,    K_LM,    2,   10,   16,     0, 0x1a,   6,  0x80,      13, SC200,       15,   0,  40,  23,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 07: ATCK_CROUCH_MP */
	{  12,         BU_O | NO_VEC,            A_NOM,     K_L,    1,   10,   24,     0, 0x00,  11,  0x80,      11, SC300,       15,   0,  50,  24,   36, NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 08: ATCK_CROUCH_HP */
	{   3,           ZU | NO_VEC,             A_LO,    K_LS,    4,   10,   10,     0, 0x18,   5,  0x70, MARK_N1, SC100, SD_KIK_S,   0,  20,  25,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 09: ATCK_CROUCH_LK */
	{   6,                    ZU,             A_LO,    K_LM,    2,   10,   16,     0, 0x1a,   7,  0x40,      14, SC200,       14,   0,  40,  26,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0a: ATCK_CROUCH_MK */
	{  12,                    AS,             A_LO,    K_LL,    1,   10,   24,     4, 0x18,   8,  0x70,      13, SC300,       15,   0,  90,  27,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0b: ATCK_CROUCH_HK */
	{   3,           ZU | NO_VEC,             A_HI,     K_S,    1,   10,   10,     0, 0x00,   5,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  28,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0c: ATCK_JUMP_LP */
	{   6,           ZU | NO_VEC,             A_HI,     K_M,    1,   10,   16,     0, 0x00,   5,  0x80,      13, SC200,       14,   0,  40,  29,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0d: ATCK_JUMP_MP */
	{  12,           ZU | NO_VEC,             A_HI,     K_M,    1,   10,   32,     0, 0x16,   8,  0x80,      14, SC300,       15,   0,  60,  30,  143,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0e: ATCK_JUMP_HP */
	{   3,           ZU | NO_VEC,             A_HI,     K_S,    1,   10,   10,     0, 0x00,   5,  0x60, MARK_N1, SC100, SD_KIK_S,   0,  20,  31,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0f: ATCK_JUMP_LK */
	{   6,           ZU | NO_VEC,             A_HI,     K_M,    1,   10,   16,     0, 0x00,   6,  0x60,      14, SC200,       14,   0,  40,  32,  142,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 10: ATCK_JUMP_MK */
	{  12,           ZU | NO_VEC,             A_HI,     K_M,    1,   10,   32,     0, 0x16,   8,  0x80,      14, SC300,       15,   0,  60,  33,   34,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 11: ATCK_JUMP_HK */
	{  12,            AS | NO_ZU,             A_LO,    K_LL,    1,   10,   24,     9, 0x18,   8,  0x70, MARK_N3, SC300, SD_KIK_L,   0,  90, 108,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 12: pl05_at_dfwd_hk, Crouch HK with the stick forward */
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
	{  16,                     2,     A_NOM | 0x40,       0,    4,    4,    0,    15, 0x1a,   3,  0x80, MARK_N1, SC300, SD_NOR_M,   0,   0,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 20: pl05_at_throw_k, Kick throw */
	{  16,                     2,     A_NOM | 0x40,       0,    4,   16,    0,    15, 0x1a,   7,  0x80, MARK_N1, SC300, SD_NOR_M,   0,   0,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 21: pl05_at_air_throw_p, Punch air throw */
	{   0,                     2,   A_HYPER | 0x40,       0,    4,   16,    0,    15, 0x00,   0,  0x80, MARK_N1, SC300, SD_NOR_M,   0,   0,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 22: pl05_at_ragnarok_throw, Ragnarok's throw when the rush connects (ragnarok_up) */
	{  16,                     2,     A_NOM | 0x40,       0,    4,   16,    0,    15, 0x04,   7,  0x80, MARK_N1, SC300, SD_NOR_M,   0,   0,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 23: pl05_at_throw_p, Punch throw */
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
	{  14,            2 | NO_VEC,        A_SPECIAL,     K_M,    0,   10,   10,     0, 0x1a,   8,  0x60,      16, SC100,       16,   5,  90,  21,   11,                       0,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 30 */
	{ 0 },	/* 31 */
	{  14,            2 | NO_VEC,        A_SPECIAL,     K_M,    0,   10,   10,     0, 0x1a,   8,  0x60,      16, SC100,       16,   5,  90,  21,   11,                       0,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 32 */
	{  10,           10 | NO_VEC,        A_SPECIAL,     K_L,    0,   10,   10,     0, 0x1f,  10,  0x60,       3, SC200,       12,   0,  90,  35,    0, NO_CHIP | NO_WEIGHT | AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 33: pl05_at_t_kick, Tag-in kick off the wall (t_kick) */
	{  14,    2 | NO_VEC | NO_ZU,        A_SPECIAL,     K_M,    0,   10,   10,     0, 0x1a,   8,  0x60,      16, SC100,       15,   5,  50,  21,   11,                       0,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 34: pl05_at_murakumo_2, Ame no Murakumo when pl_sp_type is not 0 (types 1, 2, 18-21; type 0 uses 0x30/0x32) */
	{   2,            ZU | NO_ZU,          A_HYPER,     K_S,    0,   14,   20,     0, 0x18,   2,  0x80,       3, SC300,        8,   5,  20,  87,    0,                       0,  1,     0, 42,     0,     0,   0,        3, { 0, 0 } },	/* 35: pl05_at_ouroboros, Ouroboros (hyper) */
	{  18,         BU_O | NO_VEC,        A_SPECIAL,     K_L,    0,   10,   10,     0, 0x1a,   8,  0x60,      13, SC100,       16,   0,  90,  89,    0,                       0,  1,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 36 */
	{ 0 },	/* 37 */
	{  18,         BU_O | NO_VEC,        A_SPECIAL,     K_L,    0,   10,   10,     0, 0x1a,   8,  0x60,      13, SC100,       16,   0,  90,  89,    0,                       0,  1,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 38 */
	{  16,         BU_O | NO_VEC,        A_SPECIAL,     K_L,    0,   10,   10,     0, 0x1a,   8,  0x60,      13, SC100,       16,   0,  90,  90,    0,                       0,  1,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 39 */
	{ 0 },	/* 3a */
	{  16,         BU_O | NO_VEC,        A_SPECIAL,     K_L,    0,   10,   10,     0, 0x1a,   8,  0x60,      13, SC100,       16,   0,  90,  90,    0,                       0,  1,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 3b */
	{  18,         BU_O | NO_VEC,        A_SPECIAL,     K_L,    0,   10,   10,     0, 0x1a,   8,  0x60,      13, SC100,       15,   0,  90,  91,    0,                       0,  1,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 3c */
	{ 0 },	/* 3d */
	{  18,         BU_O | NO_VEC,        A_SPECIAL,     K_L,    0,   10,   10,     0, 0x1a,   8,  0x60,      13, SC100,       15,   0,  90,  91,    0,                       0,  1,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 3e */
	{  12,         BU_O | NO_VEC,        A_SPECIAL,     K_L,    0,   10,   10,     0, 0x1a,   8,  0x60,      13, SC100,       15,   0,  50, 104,    0,                       0,  1,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 3f: pl05_at_kabe_p, Punch from the wall cling (stick_punch) */
	{  12,                  BU_O, A_SPECIAL | 0x80,    K_LL,    0,    8,   10,     0, 0x1c,   6,  0x80,       3, SC300,       12,   5,  90, 100,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        5, { 0, 0 } },	/* 40 */
	{ 0 },	/* 41 */
	{  12,                  BU_O, A_SPECIAL | 0x80,    K_LL,    0,    8,   10,     0, 0x1a,   6,  0x80,       3, SC300,       12,   5,  90, 102,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        5, { 0, 0 } },	/* 42 */
	{   8,         BU_O | NO_VEC,        A_SPECIAL,     K_S,    0,   10,   20,     0, 0x1a,   3,  0x80,       3, SC300, SD_KIK_L,   0,  50, 115,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 43: pl05_at_kabe_slide_k, Kick sliding down the wall (stick_slide) */
	{  16, BU_O | NO_VEC | NO_ZU, A_SPECIAL | 0x80,     K_S,    0,   10,   24,     0, 0x1e,   9,  0x80,       3, SC300,        8,   0,  50, 110,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 44: pl05_at_formation_a_l, Formation A, light: the animal that runs in (shl05_02 type 0) */
	{   6,    2 | NO_VEC | NO_ZU,   A_HYPER | 0x80,     K_S,    0,   10,   20,     0, 0x1e,   9,  0x80,       3, SC300,        8,   0,  90, 110,    0,               NO_WEIGHT,  0,     0, 43,     0,     0,   0,        3, { 0, 0 } },	/* 45: pl05_at_legion, Legion (hyper) and its running animals (shl05_02 type 1) */
	{   2,            ZU | NO_ZU,          A_HYPER,     K_S,    0,   10,   20,     0, 0x18,   1,  0x80,       3, SC300, SD_NOR_L,   5,  10, 117,    0,     NO_CHIP | NO_WEIGHT,  1,     0, 42,     0,     0,   0,        3, { 0, 0 } },	/* 46: pl05_at_ouroboros_shot, the shot the Ouroboros options fire (shl05_01) */
	{   0,            ZU | NO_ZU,          A_HYPER,     K_S,    0,    0,   20,     0, 0x18,   1,  0x80,       3, SC300, SD_NOR_S,   5,  90, 118,    0, NO_WEIGHT | NO_COMBO | NO_KO,  1,     0, 41,     3,     0,   0,        3, { 0, 0 } },	/* 47: pl05_at_ragnarok, Ragnarok (hyper): the rush */
	{  14, BU_O | NO_VEC | NO_ZU, A_SPECIAL | 0x80, K_L | 2,    0,    1,   20,     0, 0x1d,   8,  0x80,       3, SC300, SD_NOR_L,   5,  50,  87,    0,                       0,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 48: pl05_at_formation_b, Formation B and its charging panther (shl05_02 type 4); sp_formation_b_shot sets it too */
	{   4,    5 | NO_VEC | NO_ZU,          A_HYPER,     K_L,    0,    1,   20,     0, 0x10,   1,  0x80,      14, SC300,       16,   5,  90, 119,    0,       NO_WEIGHT | NO_KO,  1,     0, 41,     0,     0,   0,        3, { 0, 0 } },	/* 49 */
	{   8,   10 | NO_VEC | NO_ZU,          A_HYPER,     K_L,    0,    1,   20,     0, 0x00,   8,  0x80,       3, SC300,       12,   5,  90, 124,    0,               NO_WEIGHT,  1,     0, 41,     0,     0,   0,        3, { 0, 0 } },	/* 4a: pl05_at_ragnarok_end, Ragnarok's copies' last, aimed slash (shl05_04) */
	{  17,            2 | NO_VEC, A_SPECIAL | 0x80,     K_L,    0,    8,   10,     0, 0x1e,   9,  0x80,       3, SC300, SD_KIK_L,   5,  90, 124,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        5, { 0, 0 } },	/* 4b: pl05_at_vajra, Vajra: leap, warp above the far side, dive */
	{   7,            2 | NO_VEC, A_SPECIAL | 0x80,     K_L,    0,    6,   10,     0, 0x1e,  11,  0x80,      12, SC300,       15,   5,  90, 128,    0,               NO_WEIGHT,  1,     0,  0,     0,    32,   0,        6, { 0, 0 } },	/* 4c */
	{ 0 },	/* 4d */
	{   7,         BU_O | NO_VEC, A_SPECIAL | 0x80,     K_L,    0,    6,   10,     0, 0x1e,   5,  0x80,      13, SC300,       15,   5,  90, 132,    0,               NO_WEIGHT,  1,     0,  0,     0,    32,   0,        6, { 0, 0 } },	/* 4e */
	{   7,         BU_O | NO_VEC, A_SPECIAL | 0x80,     K_L,    0,    6,   10,     0, 0x1c,   9,  0x80,      14, SC300,       15,   5,  90, 134,    0,               NO_WEIGHT,  1,     0,  0,     0,    32,   0,        6, { 0, 0 } },	/* 4f */
	{ 0 },	/* 50 */
	{   7,         BU_O | NO_VEC, A_SPECIAL | 0x80,     K_L,    0,    6,   10,     0, 0x1c,   9,  0x80,      13, SC300,       15,   5,  90, 138,    0,               NO_WEIGHT,  1,     0,  0,     0,    32,   0,        6, { 0, 0 } },	/* 51 */
	{  12,          BU_O | NO_ZU, A_SPECIAL | 0x80, K_L | 2,    0,   10,   20,     0, 0x1e,   7,  0x80,       3, SC300,        8,   5,  90, 140,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 52: pl05_at_formation_c, Formation C, on Strider and the bird's bomb (shl05_03 type 2) */
	{   6,    5 | NO_VEC | NO_ZU,   A_HYPER | 0x80,     K_L,    0,    1,   20,     0, 0x10,   1,  0x80,      16, SC300,       16,   5,  90, 119,    0,       NO_WEIGHT | NO_KO,  1,     0, 41,     0,     0,   0,        3, { 0, 0 } },	/* 53 */
	{ 0 },	/* 54 */
	{  20,          BU_O | NO_ZU, A_SPECIAL | 0x80, K_L | 2,    0,   10,   20,     0, 0x00,  10,  0x80,       3, SC300, SD_NOR_L,   5,  90, 145,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 55: pl05_at_formation_c_blast, the Formation C bomb's burst on landing */
	{   5,    2 | NO_VEC | NO_ZU,   A_HYPER | 0x80,     K_S,    0,   10,   15,     0, 0x18,   8,  0x80,       3, SC300, SD_NOR_L,   0,  90, 146,    0,               NO_WEIGHT,  0,     0, 43,     0,     0,   0,        3, { 0, 0 } },	/* 56: pl05_at_legion_2, Legion's high animals (shl05_02 type 2) */
	{   2,            ZU | NO_ZU,          A_HYPER,     K_S,    0,   14,   20,     0, 0x18,   2,  0x80,       3, SC300, SD_NOR_L,   5,  20,  87,    0,                 NO_CHIP,  1,     0, 42,     0,     0,   0,        3, { 0, 0 } },	/* 57 */
	{  10,           12 | NO_VEC,        A_SPECIAL,     K_L,    0,    4,   24,     0, 0x1c,  12,  0x80,      14, SC300,       16,  64,  90,  11,   21,               NO_WEIGHT,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 58: pl05_at_snapback, Snapback */
	{  13,         BU_O | NO_VEC,        A_SPECIAL,     K_L,    0,   10,   24,     0, 0x00,  11,  0x80,      11, SC300,       15,   0,  40, 153,  154, NO_CHIP | NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 59: pl05_at_upper, the launcher (pl05_upper) */
	{  14,            2 | NO_VEC,        A_SPECIAL,     K_M,    0,   10,   10,     0, 0x1a,   8,  0x60,      16, SC100,       16,   5,  90,  21,   11,                       0,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 5a: pl05_at_vc_murakumo, Ame no Murakumo as a variable counter */
	{  14,            2 | NO_VEC, A_SPECIAL | 0x80,     K_L,    0,    8,   10,     0, 0x1e,   9,  0x80,       3, SC300, SD_KIK_L,   5,  90, 124,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        5, { 0, 0 } },	/* 5b: pl05_at_vc_vajra, Vajra as a variable counter */
	{  16, BU_O | NO_VEC | NO_ZU, A_SPECIAL | 0x80,     K_S,    0,   10,   24,     0, 0x1e,   9,  0x80,       3, SC300,        8,   0,  50, 110,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 5c: pl05_at_vc_formation_a, Formation A as a variable counter, on Strider and its animal (shl05_02 type 0) */
	{  16, BU_O | NO_VEC | NO_ZU, A_SPECIAL | 0x80,     K_S,    0,   10,   24,     0, 0x1e,   9,  0x80,       3, SC300,        8,   0,  50, 146,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 5d: pl05_at_formation_a_h, Formation A, heavy: the robot (shl05_02 type 6) */
};
