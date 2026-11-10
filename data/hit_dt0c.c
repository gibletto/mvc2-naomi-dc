/* Spiderman (character 0c): the attacks (HIT_DATA, 0x1c bytes).
   What each hit does. The character's module sets the attack's number (atck_no) with the move,
   and the row is read when the attack box meets a body. After a row is its number, in
   hexadecimal, and the program's name for it:
   ATCK_ for the normals every character has (player.h), pl0c_at_ for its own (pl0c_def.h).
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
     box, box2  boxes of hit_0c that the computer measures the attack's reach by
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

const HIT_DATA hit_dt_0c[] = {
	/* dmg                 type              kind               elem  piyo  stop  sand  sleep   vec  pow  range     mark  score  se  shc  jug  box  box2                     flag  13  guard  15  catch  abyss  cnt    answer        1a */
	{   3,         ZU | NO_VEC,            A_NOM,               K_S,    4,   10,   10,     0, 0x18,   8,  0x70, MARK_N1, SC100,  1,   0,  20,  15,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 00: ATCK_STAND_LP */
	{   6,       BU_O | NO_VEC,            A_NOM,               K_M,    2,   11,   16,     0, 0x00,  10,  0x80, MARK_N2, SC200,  2,   0,  40,  19,   23, NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 01: ATCK_STAND_MP */
	{  12,                BU_O,            A_NOM,               K_L,    1,   12,   24,     0, 0x1a,   9,  0x80, MARK_N3, SC300,  3,   0,  50,  31,   36,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 02: ATCK_STAND_HP */
	{   3,         ZU | NO_VEC,            A_NOM,              K_LS,    4,   10,   10,     0, 0x19,   9,  0x60, MARK_N1, SC100,  1,   0,  20,  37,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 03: ATCK_STAND_LK */
	{   6,                  ZU,            A_NOM,               K_M,    2,   11,   16,     0, 0x1a,   9,  0x60, MARK_N2, SC200,  2,   0,  40,  41,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 04: ATCK_STAND_MK */
	{  12,         ZU | NO_VEC,            A_NOM,               K_L,    1,   12,   24,     0, 0x00,  11,  0x80, MARK_N3, SC300,  3,   0,  50,  45,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 05: ATCK_STAND_HK */
	{   3,         ZU | NO_VEC,            A_NOM,              K_LS,    4,   10,   10,     0, 0x18,   8,  0x70, MARK_N1, SC100,  1,   0,  20,  46,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 06: ATCK_CROUCH_LP */
	{   6,                  ZU,            A_NOM,              K_LM,    2,   11,   16,     0, 0x18,   9,  0x80, MARK_N2, SC200,  2,   0,  40,  49,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 07: ATCK_CROUCH_MP */
	{  12,                  ZU,            A_NOM,              K_LL,    1,   12,   24,     0, 0x18,  15,  0x80, MARK_N3, SC300,  3,   0,  50,  52,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 08: ATCK_CROUCH_HP */
	{   3,         ZU | NO_VEC,             A_LO,              K_LS,    4,   10,   10,     0, 0x18,   8,  0x70, MARK_N1, SC100,  1,   0,  20,  53,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 09: ATCK_CROUCH_LK */
	{   6,       BU_O | NO_VEC,            A_NOM,              K_LM,    2,   10,   16,     0, 0x00,   5,  0x40, MARK_N2, SC200,  2,   0,  40,  54,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0a: ATCK_CROUCH_MK */
	{  12,                  AS,             A_LO,              K_LL,    1,   12,   24,     8, 0x18,  10,  0x70, MARK_N3, SC300,  3,   0,  90,  57,    0,                       0,  0,     0,  0,     0,    60,   0, YK_GD060, { 0, 0 } },	/* 0b: ATCK_CROUCH_HK */
	{   3,         ZU | NO_VEC,             A_HI,               K_S,    1,   10,   10,     0, 0x14,   2,  0x70, MARK_N1, SC100,  1,   0,  20,  59,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0c: ATCK_JUMP_LP */
	{   6,         ZU | NO_VEC,             A_HI,               K_S,    1,   10,   16,     0, 0x00,   8,  0x80, MARK_N2, SC200,  2,   0,  40,  60,   61,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0d: ATCK_JUMP_MP */
	{  12,         ZU | NO_VEC,             A_HI,               K_M,    1,   12,   32,     0, 0x10,   3,  0x80, MARK_N3, SC300,  3,   0,  60,  64,  226, NO_WEIGHT | AERIAL_FINISH,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0e: ATCK_JUMP_HP */
	{   3,         ZU | NO_VEC,             A_HI,               K_S,    1,   10,   10,     0, 0x00,   1,  0x60, MARK_N1, SC100,  1,   0,  20,  65,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0f: ATCK_JUMP_LK */
	{   6,         ZU | NO_VEC,             A_HI,               K_S,    1,   10,   16,     0, 0x00,   6,  0x60, MARK_N2, SC200,  2,   0,  40,  67,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 10: ATCK_JUMP_MK */
	{  12,         ZU | NO_VEC,             A_HI,               K_M,    1,   12,   32,     0, 0x17,  10,  0x80, MARK_N3, SC300,  3,   0,  60,  68,  227, NO_WEIGHT | AERIAL_FINISH,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 11: ATCK_JUMP_HK */
	{ 0 },	/* 12 */
	{ 0 },	/* 13 */
	{ 0 },	/* 14 */
	{ 0 },	/* 15 */
	{ 0 },	/* 16 */
	{ 0 },	/* 17 */
	{ 0 },	/* 18 */
	{   3,         ZU | NO_VEC,            A_NOM,  K_S | K_BACK_MAX,    4,   10,   10,     0, 0x18,   8,  0x70, MARK_N1, SC100,  1,   0,  20,  15,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 19 */
	{   3,         ZU | NO_VEC,            A_NOM, K_LS | K_BACK_MAX,    4,   10,   10,     0, 0x18,   8,  0x70, MARK_N1, SC100,  1,   0,  20,  46,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 1a */
	{ 0 },	/* 1b */
	{ 0 },	/* 1c */
	{ 0 },	/* 1d */
	{ 0 },	/* 1e */
	{ 0 },	/* 1f */
	{  16,                   2,     A_NOM | 0x40,                 0,    4,   16,   20,     0, 0x06,  11,  0x80, MARK_N1, SC300, 76,   0,   0,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 20: pl0c_at_throw_p, the punch throw, set on the opponent */
	{  16,                   2,     A_NOM | 0x40,                 0,    4,   16,   20,     0, 0x14,  10,  0x80, MARK_N1, SC300, 76,   0,   0,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 21: pl0c_at_throw_k, the kick throw, set on the opponent */
	{  16,                   2,     A_NOM | 0x40,                 0,    4,    8,   20,    10, 0x14,   4,  0x80, MARK_N1, SC300,  1,   0,   0,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 22: pl0c_at_air_throw, the air throw, set on the opponent */
	{  22,      2 | FLY_SCREEN, A_SPECIAL | 0x40,                 0,    0,   10,   20,    15, 0x07,  15,  0x80, MARK_N1, SC300,  1,   0,   0,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 23: pl0c_at_webthrow, Web Throw: the opponent swung over, set on the opponent */
	{  44,          2 | NO_VEC,   A_HYPER | 0x40,                 0,    0,   10,   20,    15, 0x12,  12,  0x80, MARK_N1, SC300,  1,   0,   0,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 24: pl0c_at_hwebthrow_2, Ultimate Web Throw (hyper): the throw, set on the opponent */
	{ 0 },	/* 25 */
	{ 0 },	/* 26 */
	{ 0 },	/* 27 */
	{ 0 },	/* 28 */
	{ 0 },	/* 29 */
	{   6,       BU_O | NO_VEC,        A_SPECIAL,              K_LM,    0,   10,   16,     0, 0x00,  10,  0x40, MARK_N2, SC200,  2,   0,  40,  54,    0, NO_CHIP | NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 2a */
	{   6,       BU_O | NO_VEC,        A_SPECIAL,               K_M,    0,   10,   16,     0, 0x00,  10,  0x80, MARK_N2, SC200,  2,   0,  40,  19,   23, NO_CHIP | NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 2b */
	{  10,         10 | NO_VEC,        A_SPECIAL,               K_L,    0,   10,   10,     0, 0x1f,  10,  0x60,       3, SC200,  4,   0,  90, 104,  106, NO_CHIP | NO_WEIGHT | AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 2c: pl0c_at_t_kick, the jump-in kick when he is tagged in (t_kick) */
	{ 0 },	/* 2d */
	{ 0 },	/* 2e */
	{ 0 },	/* 2f */
	{  12,          2 | NO_VEC, A_SPECIAL | 0x80,              K_LM,    0,   10,   30,     0, 0x00,   9,  0x80,       3, SC300,  3,   0,  90,  73,   78,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        5, { 0, 0 } },	/* 30 */
	{  16,          2 | NO_VEC, A_SPECIAL | 0x80,              K_LM,    0,   10,   30,     0, 0x1f,  11,  0x80,       3, SC300,  3,   0,  90,  73,   78,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        5, { 0, 0 } },	/* 31 */
	{  14,          2 | NO_VEC, A_SPECIAL | 0x80,              K_LM,    0,   10,   30,     0, 0x1f,  10,  0x80,       3, SC300,  3,   0,  90,  73,   78,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        5, { 0, 0 } },	/* 32 */
	{  10,          2 | NO_VEC, A_SPECIAL | 0x80,               K_L,    0,   10,   20,     0, 0x11,  12,  0x80,       3, SC300,  4,   0,  90,  81,   83,                       0,  1,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 33 */
	{  10,          2 | NO_VEC, A_SPECIAL | 0x80,               K_L,    0,   10,   20,     0, 0x11,  12,  0x80,       3, SC300,  4,   0,  90,  81,   83,                       0,  1,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 34 */
	{  10,          2 | NO_VEC, A_SPECIAL | 0x80,               K_L,    0,   10,   20,     0, 0x11,  12,  0x80,       3, SC300,  4,   0,  90,  81,   83,                       0,  1,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 35 */
	{  10,          ZU | NO_ZU, A_SPECIAL | 0x80,              K_LS,    0,    0,   20,    60, 0x00,   2,  0x80,       3, SC300, 19,   0,  50, 205,    0,                       0,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 36 */
	{  12,          ZU | NO_ZU, A_SPECIAL | 0x80,              K_LS,    0,    0,   20,    60, 0x00,   2,  0x80,       3, SC300, 19,   0,  50, 205,    0,                       0,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 37 */
	{  11,          ZU | NO_ZU, A_SPECIAL | 0x80,              K_LS,    0,    0,   20,    60, 0x00,   2,  0x80,       3, SC300, 19,   0,  50, 205,    0,                       0,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 38 */
	{  10,          ZU | NO_ZU, A_SPECIAL | 0x80,              K_LS,    0,    0,   20,    60, 0x00,   2,  0x80,       3, SC300, 19,   0,  50, 177,    0,                       0,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 39 */
	{  14,          ZU | NO_ZU, A_SPECIAL | 0x80,              K_LS,    0,    0,   20,    60, 0x00,   2,  0x80,       3, SC300, 19,   0,  50, 177,    0,                       0,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 3a */
	{  12,          ZU | NO_ZU, A_SPECIAL | 0x80,              K_LS,    0,    0,   20,    60, 0x00,   2,  0x80,       3, SC300, 19,   0,  50, 177,    0,                       0,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 3b */
	{  22,  2 | NO_VEC | NO_ZU, A_SPECIAL | 0x80,              K_LL,    0,   10,   30,     2, 0x1c,   7,  0x80,       3, SC300,  4,   0,  90,  86,  212,                       0,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 3c */
	{  22,  2 | NO_VEC | NO_ZU, A_SPECIAL | 0x80,              K_LL,    0,   10,   30,     2, 0x1b,  10,  0x80,       3, SC300,  4,   0,  90,  86,  212,                       0,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 3d */
	{  22,  2 | NO_VEC | NO_ZU, A_SPECIAL | 0x80,              K_LL,    0,   10,   30,     2, 0x1b,   8,  0x80,       3, SC300,  4,   0,  90,  86,  212,                       0,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 3e */
	{  20,  2 | NO_VEC | NO_ZU, A_SPECIAL | 0x80,              K_LL,    0,   10,   30,    16, 0x1c,   7,  0x80,       3, SC300,  4,   0,  90,  89,  213,                       0,  1,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 3f */
	{  20,  2 | NO_VEC | NO_ZU, A_SPECIAL | 0x80,              K_LL,    0,   10,   30,    16, 0x1b,  10,  0x80,       3, SC300,  4,   0,  90,  89,  213,                       0,  1,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 40 */
	{  20,  2 | NO_VEC | NO_ZU, A_SPECIAL | 0x80,              K_LL,    0,   10,   30,    16, 0x1b,   8,  0x80,       3, SC300,  4,   0,  90,  89,  213,                       0,  1,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 41 */
	{   5, ZU | NO_VEC | NO_ZU, A_SPECIAL | 0x80,              K_LS,    0,    0,    0,    60, 0x00,   2,  0x80,       3, SC300, 19,   0,  50, 131,  135,                 NO_CHIP,  1,     0,  0,    12,     0,   0,        4, { 0, 0 } },	/* 42 */
	{   5, ZU | NO_VEC | NO_ZU, A_SPECIAL | 0x80,              K_LS,    0,    0,    0,    60, 0x00,   2,  0x80,       3, SC300, 19,   0,  50, 147,  151,                 NO_CHIP,  1,     0,  0,    12,     0,   0,        4, { 0, 0 } },	/* 43 */
	{   5, ZU | NO_VEC | NO_ZU, A_SPECIAL | 0x80,              K_LS,    0,    0,    0,    60, 0x00,   2,  0x80,       3, SC300, 19,   0,  50, 164,  168,                 NO_CHIP,  1,     0,  0,    12,     0,   0,        4, { 0, 0 } },	/* 44 */
	{  10,  2 | NO_VEC | NO_ZU,   A_HYPER | 0x80,               K_M,    0,    6,   60,     0, 0x00,  11,  0x80,       3, SC300,  3,   0,  90, 103,  104,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 45: pl0c_at_maximum, Maximum Spider (hyper): the start */
	{  10,  2 | NO_VEC | NO_ZU,   A_HYPER | 0x80,               K_M,    0,    6,   60,     0, 0x00,   6,  0x80,       3, SC300,  3,   0,  90, 103,  104,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 46 */
	{  10,  2 | NO_VEC | NO_ZU,   A_HYPER | 0x80,               K_M,    0,    6,   60,     0, 0x00,  10,  0x80,       3, SC300,  3,   0,  90, 103,  104,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 47 */
	{ 0 },	/* 48 */
	{  10,  2 | NO_VEC | NO_ZU,   A_HYPER | 0x80,               K_M,    0,    6,   60,     0, 0x00,  11,  0x80,       3, SC300,  3,   0,  90, 103,  104,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 49 */
	{  10,  2 | NO_VEC | NO_ZU,   A_HYPER | 0x80,               K_M,    0,    6,   60,     0, 0x00,   6,  0x80,       3, SC300,  3,   0,  90, 103,  104,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 4a */
	{  10,  2 | NO_VEC | NO_ZU,   A_HYPER | 0x80,               K_M,    0,    6,   60,     0, 0x00,  10,  0x80,       3, SC300,  3,   0,  90, 103,  104,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 4b */
	{ 0 },	/* 4c */
	{   9,  2 | NO_VEC | NO_ZU,   A_HYPER | 0x80,               K_M,    0,    4,   60,     0, 0x02,   5,  0x80,       3, SC300,  3,   0,  90, 101,  106,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 4d */
	{   9,  2 | NO_VEC | NO_ZU,   A_HYPER | 0x80,               K_M,    0,    4,   60,     0, 0x01,   6,  0x80,       3, SC300,  3,   0,  90, 102,  106,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 4e */
	{   9,  2 | NO_VEC | NO_ZU,   A_HYPER | 0x80,               K_M,    0,    4,   60,     0, 0x00,   7,  0x80,       3, SC300,  3,   0,  90, 103,  106,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 4f */
	{   9,  2 | NO_VEC | NO_ZU,   A_HYPER | 0x80,               K_M,    0,    4,   60,     0, 0x01,   8,  0x80,       3, SC300,  3,   0,  90, 104,  106,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 50 */
	{   9,  2 | NO_VEC | NO_ZU,   A_HYPER | 0x80,               K_M,    0,    4,   60,     0, 0x01,  10,  0x80,       3, SC300,  3,   0,  90, 105,  106,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 51 */
	{   9,  2 | NO_VEC | NO_ZU,   A_HYPER | 0x80,               K_M,    0,    4,   60,     0, 0x01,   8,  0x80,       3, SC300,  3,   0,  90, 107,  106,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 52 */
	{   9,  2 | NO_VEC | NO_ZU,   A_HYPER | 0x80,               K_M,    0,    4,   60,     0, 0x00,   7,  0x80,       3, SC300,  3,   0,  90, 108,  106,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 53 */
	{   9,  2 | NO_VEC | NO_ZU,   A_HYPER | 0x80,               K_M,    0,    4,   60,     0, 0x01,   6,  0x80,       3, SC300,  3,   0,  90, 109,  106,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 54 */
	{   9,  2 | NO_VEC | NO_ZU,   A_HYPER | 0x80,               K_M,    0,    4,   60,     0, 0x1e,   5,  0x80,       3, SC300,  3,   0,  90, 110,  106,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 55 */
	{   9,  2 | NO_VEC | NO_ZU,   A_HYPER | 0x80,               K_M,    0,    4,   60,     0, 0x1f,   6,  0x80,       3, SC300,  3,   0,  70, 111,  106,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 56 */
	{   9,  2 | NO_VEC | NO_ZU,   A_HYPER | 0x80,               K_M,    0,    4,   60,     0, 0x00,   7,  0x80,       3, SC300,  3,   0,  90, 112,  106,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 57 */
	{   9,  2 | NO_VEC | NO_ZU,   A_HYPER | 0x80,               K_M,    0,    4,   60,     0, 0x1f,   8,  0x80,       3, SC300,  3,   0,  90, 113,  106,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 58 */
	{   9,  2 | NO_VEC | NO_ZU,   A_HYPER | 0x80,               K_M,    0,    4,   60,     0, 0x1e,  10,  0x80,       3, SC300,  3,   0,  90, 114,  106,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 59 */
	{   9,  2 | NO_VEC | NO_ZU,   A_HYPER | 0x80,               K_M,    0,    4,   60,     0, 0x1f,   8,  0x80,       3, SC300,  3,   0,  90, 115,  106,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 5a */
	{   9,  2 | NO_VEC | NO_ZU,   A_HYPER | 0x80,               K_M,    0,    4,   60,     0, 0x00,   6,  0x80,       3, SC300,  3,   0,  90, 116,  106,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 5b */
	{   9,  2 | NO_VEC | NO_ZU,   A_HYPER | 0x80,               K_M,    0,    4,   60,     0, 0x1f,   5,  0x80,       3, SC300,  3,   0,  90, 117,  106,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 5c */
	{  16,      2 | FLY_SCREEN,   A_HYPER | 0x80,               K_M,    0,   16,   60,    20, 0x1a,  13,  0x80,       3, SC300,  4,   0,  90, 105,  106,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 5d */
	{   8, ZU | NO_VEC | NO_ZU,          A_HYPER,              K_LL,    0,    3,   20,    16, 0x1a,   1,  0x80,       3, SC300,  3,   0,  90, 234,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 5e: pl0c_at_crawler, Crawler Assault (hyper): the start, before the animation's hits */
	{   5, ZU | NO_VEC | NO_ZU,          A_HYPER,               K_L,    0,    3,   20,     0, 0x1a,   1,  0x80,       3, SC200,  2,   0,  90, 234,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 5f */
	{   5, ZU | NO_VEC | NO_ZU,          A_HYPER,              K_LL,    0,    3,   20,     0, 0x1a,   1,  0x80,       3, SC300,  3,   0,  90, 234,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 60 */
	{   5, ZU | NO_VEC | NO_ZU,          A_HYPER,              K_LM,    0,    3,   20,     0, 0x1a,   1,  0x60,       3, SC300,  1,   0,  90,  23,  229,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 61 */
	{  16,          2 | NO_VEC,          A_HYPER,              K_LL,    0,   16,   30,     0, 0x1a,  15,  0x80,       3, SC300,  4,   0,  90, 104,  106,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 62 */
	{   5, ZU | NO_VEC | NO_ZU,          A_HYPER,               K_M,    0,   12,    0,     0, 0x1a,   1,  0x80,       3, SC300, 19,   0,  90, 175,  249, NO_CHIP | NO_WEIGHT | NO_KO,  0,     0,  0,     2,     0,   0,        3, { 0, 0 } },	/* 63: pl0c_at_hwebthrow, Ultimate Web Throw (hyper): the web's grab (sp_hwebthrow and shl0c_02) */
	{   9,       BU_O | NO_VEC, A_SPECIAL | 0x80,              K_LM,    0,   10,   32,     0, 0x00,  12,  0x80,       3, SC300,  3,   0,  90,  73,   78,               NO_WEIGHT,  1,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 64 */
	{   9,       BU_O | NO_VEC, A_SPECIAL | 0x80,              K_LM,    0,   10,   32,     0, 0x00,  12,  0x80,       3, SC300,  3,   0,  90,  73,   78,               NO_WEIGHT,  1,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 65 */
	{   9,       BU_O | NO_VEC, A_SPECIAL | 0x80,              K_LM,    0,   10,   32,     0, 0x00,  12,  0x80,       3, SC300,  3,   0,  90,  73,   78,               NO_WEIGHT,  1,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 66 */
	{  10,         12 | NO_VEC,        A_SPECIAL,               K_L,    0,    4,   24,     0, 0x1c,  12,  0x80,       3, SC300,  8,  64,  90,  31,   36,               NO_WEIGHT,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 67: pl0c_at_snapback, Snapback (sp_snapback) */
};
