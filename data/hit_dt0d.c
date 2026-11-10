/* Hulk (character 0d): the attacks (HIT_DATA, 0x1c bytes).
   What each hit does. The character's module sets the attack's number (atck_no) with the move,
   and the row is read when the attack box meets a body. After a row is its number, in
   hexadecimal, and the program's name for it:
   ATCK_ for the normals every character has (player.h), pl0d_at_ for its own (pl0d_def.h).
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
     box, box2  boxes of hit_0d that the computer measures the attack's reach by
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

const HIT_DATA hit_dt_0d[] = {
	/* dmg                      type              kind     elem  piyo  stop  sand  sleep   vec  pow  range     mark  score  se  shc  jug  box  box2                     flag  13  guard  15  catch  abyss  cnt    answer        1a */
	{   6,              ZU | NO_VEC,     A_NOM | 0x80,     K_M,    4,   16,   16,     0, 0x18,   8,  0x70, MARK_N1, SC100,  2,   0,  30,  40,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 00: ATCK_STAND_LP */
	{  10,                     BU_O,     A_NOM | 0x80,     K_L,    2,   14,   24,     0, 0x19,  10,  0x80, MARK_N2, SC200,  3,   0,  50,  41,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 01: ATCK_STAND_MP */
	{  16,             BU_O | NO_ZU,     A_NOM | 0x80,    0x40,    1,   16,   24,     0, 0x1c,  12,  0x80, MARK_N3, SC300,  4,   0,  90,  42,  183,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 02: ATCK_STAND_HP */
	{   6,              ZU | NO_VEC,     A_NOM | 0x80,     K_S,    4,    8,   10,     0, 0x18,   6,  0x60, MARK_N1, SC100,  2,   0,  30,  43,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 03: ATCK_STAND_LK */
	{  10,                     BU_O,     A_NOM | 0x80,     K_L,    2,   14,   24,     0, 0x19,  12,  0x60, MARK_N2, SC200,  3,   0,  50,  44,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 04: ATCK_STAND_MK */
	{  16,            BU_O | NO_VEC,     A_NOM | 0x80,    0x40,    1,   10,   28,     0, 0x1d,  12,  0x80, MARK_N3, SC300,  4,   1,  90,  78,   72,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 05: ATCK_STAND_HK */
	{   6,              ZU | NO_VEC,     A_NOM | 0x80,    K_LM,    4,   16,   10,     0, 0x1c,   6,  0x70, MARK_N1, SC100,  2,   0,  30,  46,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 06: ATCK_CROUCH_LP */
	{  10,                       ZU,     A_NOM | 0x80,    K_LL,    2,   16,   16,     0, 0x18,   8,  0x80, MARK_N2, SC200,  3,   0,  50,  48,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 07: ATCK_CROUCH_MP */
	{   8,            BU_O | NO_VEC,     A_NOM | 0x80,    K_LL,    1,    6,   24,     0, 0x01,   8,  0x80, MARK_N3, SC300,  3,   0,  90,  65,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 08: ATCK_CROUCH_HP */
	{   6,              ZU | NO_VEC,      A_LO | 0x80,    K_LS,    4,    8,   10,     0, 0x18,   6,  0x70, MARK_N1, SC100,  2,   0,  30,  50,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 09: ATCK_CROUCH_LK */
	{  10,                       ZU,     A_NOM | 0x80,    K_LL,    2,   16,   16,     0, 0x18,   8,  0x40, MARK_N2, SC200,  3,   0,  50,  51,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0a: ATCK_CROUCH_MK */
	{  16,                     BU_O,     A_NOM | 0x80,    0xc0,    1,   14,   24,     0, 0x1b,  10,  0x70, MARK_N3, SC300,  4,   0,  90,  52,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0b: ATCK_CROUCH_HK */
	{   6,              ZU | NO_VEC,      A_HI | 0x80,     K_S,    1,   10,   10,     0, 0x1d,   5,  0x70, MARK_N1, SC100,  2,   0,  30,  53,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0c: ATCK_JUMP_LP */
	{  10,              ZU | NO_VEC,      A_HI | 0x80,     K_M,    1,   14,   16,     0, 0x17,  10,  0x80, MARK_N2, SC200,  3,   0,  50,  54,    0,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0d: ATCK_JUMP_MP */
	{  16,              ZU | NO_VEC,      A_HI | 0x80,     K_M,    1,   10,   32,     0, 0x18,  10,  0x80, MARK_N3, SC300,  4,   0,  90,  55,    0,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0e: ATCK_JUMP_HP */
	{   6,              ZU | NO_VEC,      A_HI | 0x80,     K_S,    1,   10,   10,     0, 0x1e,   5,  0x60, MARK_N1, SC100,  2,   0,  30,  56,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0f: ATCK_JUMP_LK */
	{  10,              ZU | NO_VEC,      A_HI | 0x80,     K_M,    1,   10,   16,     0, 0x1f,   5,  0x60, MARK_N2, SC200,  3,   0,  50,  57,  202,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 10: ATCK_JUMP_MK */
	{  16,              ZU | NO_VEC,      A_HI | 0x80,     K_M,    1,   12,   32,     0, 0x17,  10,  0x80, MARK_N3, SC300,  4,   0,  90,  64,    0,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 11: ATCK_JUMP_HK */
	{ 0 },	/* 12 */
	{ 0 },	/* 13 */
	{ 0 },	/* 14 */
	{ 0 },	/* 15 */
	{ 0 },	/* 16 */
	{ 0 },	/* 17 */
	{ 0 },	/* 18 */
	{  16,            BU_O | NO_VEC,     A_NOM | 0x80,     K_L,    1,   10,   24,     0, 0x1f,   9,  0x80, MARK_N3, SC300,  4,   0,  90,  45,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 19 */
	{   8,            BU_O | NO_VEC,     A_NOM | 0x80,     K_L,    1,   10,   24,     0, 0x00,  11,  0x80, MARK_N3, SC300,  4,   0,  90,  49,    0, NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 1a: pl0d_at_crouch_hp_2, Crouching HP's second hit */
	{   8,            BU_O | NO_VEC,        A_SPECIAL,    K_LL,    0,    6,   24,     0, 0x01,   8,  0x80, MARK_N3, SC300,  3,   0,  90,  65,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 1b: pl0d_at_upper_p, the launcher, crouching punch: first hit */
	{   8,            BU_O | NO_VEC,        A_SPECIAL,     K_L,    0,   10,   24,     0, 0x00,  11,  0x80, MARK_N3, SC300,  4,   0,  90,  49,    0, NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 1c: pl0d_at_upper_p_2, the launcher, crouching punch: second hit */
	{  16,            BU_O | NO_VEC,        A_SPECIAL,    0x40,    0,   10,   28,     0, 0x1f,  11,  0x80, MARK_N3, SC300,  4,   0,  90,  78,   72, NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 1d: pl0d_at_upper_k, the launcher, standing kick */
	{  16,            BU_O | NO_VEC,        A_SPECIAL,     K_L,    0,   10,   24,     0, 0x1f,   9,  0x80, MARK_N3, SC300,  4,   0,  90,  45,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 1e */
	{ 0 },	/* 1f */
	{  16,                     BU_O,     A_NOM | 0x40,       0,    4,   16,   24,     4, 0x1c,   5,  0x80, MARK_N1, SC300, 76,   0,   0,   0,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 20: pl0d_at_throw_p, the punch throw, set on him and the opponent */
	{  16,                        2,     A_NOM | 0x40,       0,    4,    8,   20,    15, 0x10,  10,  0x80, MARK_N1, SC300,  1,   0,   0,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 21: pl0d_at_air_throw, the air throw, set on him and the opponent */
	{  22,           2 | FLY_SCREEN,        A_SPECIAL,       0,    4,   16,    0,    15, 0x1c,   8,  0x80, MARK_N1, SC300, 76,   0,   0,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 22 */
	{  30,           2 | FLY_SCREEN,        A_SPECIAL,       0,    4,   16,    0,    15, 0x1c,  10,  0x80, MARK_N1, SC300, 76,   0,   0,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 23 */
	{  30,           2 | FLY_SCREEN,        A_SPECIAL,       0,    4,   16,    0,    15, 0x1c,  11,  0x80, MARK_N1, SC300, 76,   0,   0,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 24 */
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
	{  21,    BU_O | NO_VEC | NO_ZU, A_SPECIAL | 0x80,     K_L,    0,   15,   20,     0, 0x1f,  10,  0x80,       3, SC300,  4,  10, 100,  35,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 30 */
	{  21,    BU_O | NO_VEC | NO_ZU, A_SPECIAL | 0x80,     K_L,    0,   15,   20,     0, 0x01,  11,  0x80,       3, SC300,  4,  10, 100,  35,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 31 */
	{  21,       2 | NO_VEC | NO_ZU, A_SPECIAL | 0x80,     K_L,    0,   15,   20,     0, 0x01,  11,  0x80,       3, SC300,  4,  10, 100,  35,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 32 */
	{  10,                     BU_O, A_SPECIAL | 0x80,     K_L,    0,   15,   20,     8, 0x1e,  11,  0x80,       3, SC300,  4,   4, 100,  39,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 33 */
	{  10,                     BU_O, A_SPECIAL | 0x80,     K_L,    0,   15,   20,     8, 0x1e,  11,  0x80,       3, SC300,  4,   4, 100,  39,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 34 */
	{  10,                     BU_O, A_SPECIAL | 0x80,     K_L,    0,   15,   20,     8, 0x1e,  11,  0x80,       3, SC300,  4,   4, 100,  39,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 35 */
	{  10,                     BU_O, A_SPECIAL | 0x80,     K_L,    0,   15,   20,     8, 0x01,  11,  0x80,       3, SC300,  4,   4, 100,  58,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 36 */
	{  10,                     BU_O, A_SPECIAL | 0x80,     K_L,    0,   15,   20,     8, 0x01,  11,  0x80,       3, SC300,  4,   4, 100,  58,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 37 */
	{  10,                     BU_O, A_SPECIAL | 0x80,     K_L,    0,   15,   20,     8, 0x01,  11,  0x80,       3, SC300,  4,   4, 100,  58,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 38 */
	{  10,                     BU_O, A_SPECIAL | 0x80,     K_L,    0,   15,   20,     8, 0x01,  11,  0x80,       3, SC300,  4,   4, 100,  60,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 39 */
	{  10,                     BU_O, A_SPECIAL | 0x80,     K_L,    0,   15,   20,     8, 0x01,  11,  0x80,       3, SC300,  4,   4, 100,  60,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 3a */
	{  10,                     BU_O, A_SPECIAL | 0x80,     K_L,    0,   15,   20,     8, 0x01,  11,  0x80,       3, SC300,  4,   4, 100,  60,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 3b */
	{  10,              10 | NO_VEC,        A_SPECIAL,     K_L,    0,   10,   10,     0, 0x1f,  10,  0x60,       3, SC200, 12,   0,  90,  99,    0, NO_CHIP | NO_WEIGHT | AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 3c: pl0d_at_t_kick, the jump-in kick when he is tagged in (t_kick) */
	{   8, 21 | NO_VEC | FLY_SCREEN,   A_HYPER | 0x80,     K_L,    0,   10,   20,    50, 0x1e,  11,  0x80,       3, SC300,  3,   0, 100,  79,    0,     NO_CHIP | NO_WEIGHT,  1,     0, 19,     0,     0,   0,        3, { 0, 0 } },	/* 3d: pl0d_at_crush, Gamma Crush (hyper): the start */
	{  10, 21 | NO_VEC | FLY_SCREEN,   A_HYPER | 0x80,     K_L,    0,   10,   20,    50, 0x18,   0,  0x80,       3, SC300,  3,   0, 100,  62,    0,     NO_CHIP | NO_WEIGHT,  1,     0, 19,     0,     0,   0,        6, { 0, 0 } },	/* 3e: pl0d_at_crush_rise, Gamma Crush: the rise, after its timer */
	{  40,               2 | NO_VEC,   A_HYPER | 0x80, K_L | 2,    0,    8,   20,    15, 0x1c,   8,  0x80,       3, SC300,  4,   0, 100,  72,    0,                NO_SCALE,  1,     0, 19,     0,     0,   0,        6, { 0, 0 } },	/* 3f */
	{  40,               2 | NO_VEC,   A_HYPER | 0x80, K_L | 2,    0,    8,   20,    15, 0x1c,   8,  0x80,       3, SC300,  4,   0, 100,  74,    0,                NO_SCALE,  1,     0, 19,     0,     0,   0,        6, { 0, 0 } },	/* 40 */
	{  40,               2 | NO_VEC,   A_HYPER | 0x80, K_L | 2,    0,    8,   20,    15, 0x04,   8,  0x80,       3, SC300,  4,   0, 100,  75,    0,                NO_SCALE,  1,     0, 19,     0,     0,   0,        6, { 0, 0 } },	/* 41 */
	{   0,                     BU_O,        A_SPECIAL,     K_S,    0,    0,   20,    15, 0x00,   0,  0x80,       3, SC300,  3,   0,  90,  70,    0, NO_CHIP | NO_SCALE | NO_COMBO,  1,     0,  0,     2,     0,   0, YK_GD060, { 0, 0 } },	/* 42: pl0d_at_tornado, Gamma Tornado: the grabbing spin (the throw after it is 0x22 + level) */
	{  20,            BU_O | NO_VEC, A_SPECIAL | 0x80,     K_L,    0,   15,   20,    15, 0x1f,  11,  0x80,       3, SC300,  4,  10, 100,  39,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 43 */
	{  20,            BU_O | NO_VEC, A_SPECIAL | 0x80,     K_L,    0,   15,   20,    15, 0x1f,  12,  0x80,       3, SC300,  4,  10, 100,  39,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 44 */
	{  20,            BU_O | NO_VEC, A_SPECIAL | 0x80,     K_L,    0,   15,   20,    15, 0x1f,  12,  0x80,       3, SC300,  4,  10, 100,  39,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 45 */
	{  10,                        2, A_SPECIAL | 0x80,     K_L,    0,   15,   20,    15, 0x1e,  10,  0x80,       3, SC300,  4,   6, 100,  35,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 46 */
	{  10,                        2, A_SPECIAL | 0x80,     K_L,    0,   15,   20,    15, 0x1e,  10,  0x80,       3, SC300,  4,   6, 100,  35,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 47 */
	{  10,                        2, A_SPECIAL | 0x80,     K_L,    0,   15,   20,    15, 0x1e,  10,  0x80,       3, SC300,  4,   6, 100,  35,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 48 */
	{  10,                        2, A_SPECIAL | 0x80,     K_L,    0,   15,   20,    15, 0x1e,  10,  0x80,       3, SC300,  4,   6, 100,  60,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 49 */
	{  10,                        2, A_SPECIAL | 0x80,     K_L,    0,   15,   20,    15, 0x1e,  10,  0x80,       3, SC300,  4,   6, 100,  60,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 4a */
	{  10,                        2, A_SPECIAL | 0x80,     K_L,    0,   15,   20,    15, 0x1e,  10,  0x80,       3, SC300,  4,   6, 100,  60,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 4b */
	{  10,                        2, A_SPECIAL | 0x80,     K_L,    0,   15,   20,    15, 0x1e,  10,  0x80,       3, SC300,  4,   6, 100,  39,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 4c */
	{  10,                        2, A_SPECIAL | 0x80,     K_L,    0,   15,   20,    15, 0x1e,  10,  0x80,       3, SC300,  4,   6, 100,  39,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 4d */
	{  10,                        2, A_SPECIAL | 0x80,     K_L,    0,   15,   20,    15, 0x1e,  10,  0x80,       3, SC300,  4,   6, 100,  39,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 4e */
	{   6,   2 | NO_ZU | FLY_SCREEN,   A_HYPER | 0x80,    0x40,    0,    6,   20,    15, 0x19,  11,  0x80,       3, SC300,  4,   0, 100,  76,    0,               NO_WEIGHT,  1,     0, 20,     0,     0,   0,        3, { 0, 0 } },	/* 4f: pl0d_at_wave, Gamma Wave (hyper): his move and the wave's rock columns (shl0d_01) */
	{ 0 },	/* 50 */
	{   8,      ZU | NO_VEC | NO_ZU, A_SPECIAL | 0x80,    K_LL,    0,    6,   20,    15, 0x1a,  10,  0x80,       3, SC300,  4,   6, 100,  76,    0,               NO_WEIGHT,  1,     0, 20,     0,     0,   0,        8, { 0, 0 } },	/* 51: pl0d_at_slam, Gamma Slam: his move and the wave's rock columns (shl0d_01) */
	{   6,    BU_O | NO_VEC | NO_ZU,   A_HYPER | 0x80,    K_LL,    0,    6,   34,    15, 0x1e,   7,  0x80,       3, SC300,  4,   0, 100,  96,   97,               NO_WEIGHT,  1,     0, 15,     0,     0,  10,        3, { 0, 0 } },	/* 52: pl0d_at_quake_rock, Gamma Quake: the rocks (shl0d_02) */
	{ 0 },	/* 53 */
	{  10,               2 | NO_VEC,   A_HYPER | 0x80,    K_LL,    0,    6,   32,    15, 0x00,   8,  0x80,       3, SC300,  3,   0, 100, 100,    0,               NO_WEIGHT,  1,     0, 15,     0,     0,   0,        3, { 0, 0 } },	/* 54: pl0d_at_quake, Gamma Quake (hyper): his move */
	{  10,    BU_O | NO_VEC | NO_ZU,   A_HYPER | 0x80,    K_LL,    0,    6,   34,    15, 0x01,   7,  0x80,       3, SC300,  4,   0, 100,  96,   97,     NO_CHIP | NO_WEIGHT,  1,     0, 15,     0,     0,  10,        3, { 0, 0 } },	/* 55 */
	{   7,                        2,   A_HYPER | 0x80,    0x40,    0,    6,   20,    15, 0x19,  11,  0x80,       3, SC300,  4,   0, 100,  76,    0,               NO_WEIGHT,  1,     0, 20,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 56 */
	{   5,              ZU | NO_VEC,   A_HYPER | 0x80, K_L | 2,    0,    8,   20,    15, 0x10,   0,  0x80,       3, SC300,  4,   0, 100,  72,    0,                       0,  1,     0, 19,     0,     0,   0,        3, { 0, 0 } },	/* 57: pl0d_at_crush_fall, Gamma Crush: the hit that rides on him as he comes down (shl0d_00) */
	{  10,              12 | NO_VEC,        A_SPECIAL,     K_L,    0,    4,   24,     0, 0x1c,  12,  0x80,       3, SC300,  8,  64, 100,  42,   84,               NO_WEIGHT,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 58: pl0d_at_snapback, Snapback */
};
