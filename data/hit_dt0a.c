/* Rogue (character 0a): the attacks (HIT_DATA, 0x1c bytes).
   What each hit does. The character's module sets the attack's number (atck_no) with the move,
   and the row is read when the attack box meets a body. After a row is its number, in
   hexadecimal, and the program's name for it:
   ATCK_ for the normals every character has (player.h), pl0a_at_ for its own (pl0a_def.h).
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
     box, box2  boxes of hit_0a that the computer measures the attack's reach by
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

const HIT_DATA hit_dt_0a[] = {
	/* dmg                   type              kind      elem  piyo  stop  sand  sleep   vec  pow  range     mark  score        se  shc  jug  box  box2                     flag  13  guard  15  catch  abyss  cnt    answer        1a */
	{   5,           ZU | NO_VEC,            A_NOM,      K_S,    4,   10,   10,     0, 0x1a,   8,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,   7,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 00: ATCK_STAND_LP */
	{   8,                    ZU,            A_NOM,      K_M,    2,   10,   16,     0, 0x1a,  10,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40,   8,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 01: ATCK_STAND_MP */
	{  14,                  BU_O,            A_NOM,      K_L,    1,   10,   24,     0, 0x1a,  14,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  50,   9,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 02: ATCK_STAND_HP */
	{   5,           ZU | NO_VEC,            A_NOM,      K_S,    4,   10,   10,     0, 0x1a,   8,  0x60, MARK_N1, SC100, SD_KIK_S,   0,  20,  10,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 03: ATCK_STAND_LK */
	{   8,                    ZU,            A_NOM,      K_M,    2,   10,   16,     0, 0x1b,  10,  0x60, MARK_N2, SC200, SD_KIK_M,   0,  40,  11,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 04: ATCK_STAND_MK */
	{  14,         BU_O | NO_VEC,            A_NOM,     K_LL,    1,   10,   24,     0, 0x00,  12,  0x80, MARK_N3, SC300, SD_KIK_L,   0,  50,  12,    0, NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 05: ATCK_STAND_HK */
	{   5,           ZU | NO_VEC,            A_NOM,     K_LS,    4,   10,   10,     0, 0x1c,   8,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  13,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 06: ATCK_CROUCH_LP */
	{   8,           ZU | NO_VEC,     A_NOM | 0x80,     K_LM,    2,   10,   16,     0, 0x1a,   8,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40,  14,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 07: ATCK_CROUCH_MP */
	{  14,         BU_O | NO_VEC,            A_NOM,     K_LL,    1,   10,   24,     0, 0x1f,  11,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  50,  15,   53, NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 08: ATCK_CROUCH_HP */
	{   5,           ZU | NO_VEC,             A_LO,     K_LS,    4,   10,   10,     0, 0x1a,   8,  0x70, MARK_N1, SC100, SD_KIK_S,   0,  20,  16,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 09: ATCK_CROUCH_LK */
	{   8,                    ZU,            A_NOM,     K_LM,    2,   10,   16,     0, 0x1a,  10,  0x40, MARK_N2, SC200, SD_KIK_M,   0,  40,  17,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0a: ATCK_CROUCH_MK */
	{  14,                    AS,             A_LO,     K_LL,    1,   10,   24,     9, 0x1c,   8,  0x70, MARK_N3, SC300, SD_KIK_L,   0,  90,  18,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0b: ATCK_CROUCH_HK */
	{   5,           ZU | NO_VEC,             A_HI,      K_S,    1,   10,   10,     0, 0x1f,   5,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  19,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0c: ATCK_JUMP_LP */
	{   8,           ZU | NO_VEC,             A_HI,      K_M,    1,   10,   16,     0, 0x1f,   5,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40,  20,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0d: ATCK_JUMP_MP */
	{  14,           ZU | NO_VEC,             A_HI,      K_L,    1,   10,   24,     0, 0x11,  10,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  60,  21,  112,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0e: ATCK_JUMP_HP */
	{   5,           ZU | NO_VEC,             A_HI,      K_S,    1,   10,   10,     0, 0x1f,   5,  0x60, MARK_N1, SC100, SD_KIK_S,   0,  20,  22,  113,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0f: ATCK_JUMP_LK */
	{   8,           ZU | NO_VEC,             A_HI,      K_M,    1,   10,   16,     0, 0x1f,   3,  0x60, MARK_N2, SC200, SD_KIK_M,   0,  40,  23,  114,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 10: ATCK_JUMP_MK */
	{  14,           ZU | NO_VEC,             A_HI,      K_L,    1,   10,   24,     0, 0x17,  10,  0x80, MARK_N3, SC300, SD_KIK_L,   0,  60,  24,    0,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 11: ATCK_JUMP_HK */
	{ 0 },	/* 12 */
	{ 0 },	/* 13 */
	{  14,         BU_O | NO_VEC,            A_NOM,     K_LL,    0,   10,   24,     0, 0x00,  12,  0x80, MARK_N3, SC300, SD_KIK_L,   0,  90, 169,    0, NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 14 */
	{ 0 },	/* 15 */
	{ 0 },	/* 16 */
	{ 0 },	/* 17 */
	{  14,           ZU | NO_VEC,             A_HI,      K_L,    1,    6,   10,     0, 0x18,  10,  0x70, MARK_N3, SC100, SD_NOR_L,   0,  50, 158,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 18 */
	{ 0 },	/* 19 */
	{ 0 },	/* 1a */
	{ 0 },	/* 1b */
	{ 0 },	/* 1c */
	{ 0 },	/* 1d */
	{ 0 },	/* 1e */
	{   4,           ZU | NO_VEC,             A_HI,     K_LL,    1,   16,   10,     0, 0x18,  10,  0x70, MARK_N3, SC100, SD_NOR_L,   0,  50, 171,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 1f */
	{   8,                     2,        A_SPECIAL,     K_LM,    0,   12,   20,     0, 0x1a,  13,  0x60, MARK_N2, SC200, SD_NOR_M,   0,   0,   0,    0,                       0,  1,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 20 */
	{  16,                     2,     A_NOM | 0x40,     K_LM,    0,   12,   20,     0, 0x1c,   8,  0x60, MARK_N2, SC200, SD_NOR_M,   0,   0,  35,   36,                       0,  1,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 21 */
	{   2,                     2,     A_NOM | 0x40,      K_L,    0,   12,   24,     0, 0x11,  10,  0x80, MARK_N3,    24, SD_NOR_L,   0,   0,   0,    0,                       0,  1,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 22: pl0a_at_throw_p, Her punch throw, ground or air (nml_catch), set on the opponent */
	{  30,                     2,          A_HYPER,     K_LM,    0,   12,   20,     0, 0x06,   8,  0x60, MARK_N2, SC200, SD_NOR_M,   0,   0,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 23: pl0a_at_shungoku, Shun Goku Satsu (Akuma's power): the last blow, set on her and the opponent */
	{  18,                     2,          A_HYPER,     K_LM,    0,   12,   20,     0, 0x1a,  13,  0x60, MARK_N2, SC200, SD_NOR_M,   0,   0,  35,   36,                       0,  1,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 24: pl0a_at_sugah, Goodnight Sugah (hyper): the drop; the drain with super_flag uses it too */
	{  10,          BU_O | NO_ZU, A_SPECIAL | 0x40,        0,    0,   16,   12,    15, 0x1b,   7,  0x80,       3, SC300, SD_NOR_S,   0,   0,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 25 */
	{  40,                     2, A_SPECIAL | 0x40,        0,    0,    0,    0,    15, 0x1d,   0,  0x80, MARK_N1, SC300,       76,   0,   0,   0,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 26: pl0a_at_screw, Screw Piledriver (Zangief's power): the slam */
	{  15,   ZU | NO_VEC | NO_ZU,          A_HYPER, K_LM | 2,    0,    6,   20,     0, 0x18,   4,  0x60,       3, SC200, SD_NOR_M,   0,  90,  79,    0,                       0,  1,     0,  4,     0,     0,   0,        3, { 0, 0 } },	/* 27 */
	{   8,   ZU | NO_VEC | NO_ZU,          A_HYPER, K_LM | 2,    0,    6,   20,     0, 0x1e,  11,  0x60,       3, SC200, SD_NOR_M,   0,  90,  80,   81,                       0,  1,     0,  4,     0,     0,   0,        3, { 0, 0 } },	/* 28 */
	{  15,                  BU_O,          A_HYPER, K_LL | 2,    0,   10,   30,     0, 0x1b,  11,  0x60,       3, SC200, SD_NOR_M,   0,  90,  80,   81,                       0,  1,     0,  4,     0,     0,   0,        3, { 0, 0 } },	/* 29 */
	{   6,         BU_O | NO_VEC,          A_HYPER,        2,    0,   10,   30,     0, 0x1e,   8,  0x80,       3, SC300, SD_NOR_L,   0,  90,  79,    0,               NO_WEIGHT,  1,     0,  4,     0,     0,   0,        3, { 0, 0 } },	/* 2a */
	{ 0 },	/* 2b */
	{ 0 },	/* 2c */
	{ 0 },	/* 2d */
	{ 0 },	/* 2e */
	{ 0 },	/* 2f */
	{  10,           10 | NO_VEC,        A_SPECIAL,      K_L,    0,   10,   10,     0, 0x1f,  10,  0x60,       3, SC200,        8,   0,  90,  55,   56, NO_CHIP | NO_WEIGHT | AERIAL_FINISH,  1,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 30: pl0a_at_dive, the start of Dive Punch (sp_dive); Shoryu-Reppa (sp_reppa) and t_kick also start with it, and the animation's marks (hit_set) give the hits after it */
	{  12,   ZU | NO_VEC | NO_ZU,        A_SPECIAL,      K_L,    0,    4,   24,     0, 0x17,  10,  0x80,       3, SC300, SD_NOR_L,   0,  60,  60,    0,           AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 31 */
	{  12,   ZU | NO_VEC | NO_ZU,        A_SPECIAL,      K_L,    0,    4,   24,     0, 0x17,  10,  0x80,       3, SC300, SD_NOR_L,   0,  60,  60,    0,           AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 32 */
	{  12,   ZU | NO_VEC | NO_ZU,        A_SPECIAL,      K_L,    0,    4,   24,     0, 0x17,  10,  0x80,       3, SC300, SD_NOR_L,   0,  60,  60,    0,           AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 33 */
	{  12,   ZU | NO_VEC | NO_ZU,        A_SPECIAL,      K_L,    0,    4,   24,     0, 0x17,  10,  0x80,       3, SC300, SD_NOR_L,   0,  40,  58,    0,           AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 34 */
	{  12,   ZU | NO_VEC | NO_ZU,        A_SPECIAL,      K_L,    0,    4,   24,     0, 0x17,  10,  0x80,       3, SC300, SD_NOR_L,   0,  40,  58,    0,           AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 35 */
	{  12,   ZU | NO_VEC | NO_ZU,        A_SPECIAL,      K_L,    0,   16,   24,     0, 0x17,  10,  0x80,       3, SC300, SD_NOR_L,   0,  40,  58,    0,           AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 36 */
	{   4,   ZU | NO_VEC | NO_ZU,        A_SPECIAL,      K_M,    0,    4,   24,     0, 0x1a,   6,  0x80,       3, SC300, SD_NOR_L,   0,  90,  65,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 37 */
	{   4,   ZU | NO_VEC | NO_ZU,        A_SPECIAL,     K_LM,    0,    4,   24,     0, 0x1a,   6,  0x80,       3, SC300, SD_NOR_L,   0,  90,  65,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 38 */
	{  10, BU_O | NO_VEC | NO_ZU,        A_SPECIAL,      K_L,    0,   10,   24,     0, 0x1c,  13,  0x80,       3, SC300, SD_NOR_L,   0,  90,  67,   71, NO_WEIGHT | AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 39 */
	{   6,   ZU | NO_VEC | NO_ZU,        A_SPECIAL,     K_LM,    0,    4,   24,     0, 0x1a,   7,  0x80,       3, SC300, SD_NOR_L,   0,  90,  65,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 3a */
	{   6,   ZU | NO_VEC | NO_ZU,        A_SPECIAL,      K_M,    0,    4,   24,     0, 0x1a,   7,  0x80,       3, SC300, SD_NOR_L,   0,  90,  65,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 3b */
	{   6,   ZU | NO_VEC | NO_ZU,        A_SPECIAL,     K_LM,    0,    4,   24,     0, 0x1a,   7,  0x80,       3, SC300, SD_NOR_L,   0,  90,  65,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 3c */
	{  12, BU_O | NO_VEC | NO_ZU,        A_SPECIAL,      K_L,    0,   10,   24,     0, 0x1c,  14,  0x80,       3, SC300, SD_NOR_L,   0,  90,  67,   71, NO_WEIGHT | AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 3d */
	{   4,   ZU | NO_VEC | NO_ZU,        A_SPECIAL,      K_L,    0,    4,   24,     0, 0x1a,   8,  0x80,       3, SC300, SD_NOR_L,   0,  90,  65,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 3e */
	{   4,   ZU | NO_VEC | NO_ZU,        A_SPECIAL,     K_LM,    0,    4,   24,     0, 0x1a,   8,  0x80,       3, SC300, SD_NOR_L,   0,  90,  65,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 3f */
	{   4,   ZU | NO_VEC | NO_ZU,        A_SPECIAL,      K_M,    0,    4,   24,     0, 0x1a,   8,  0x80,       3, SC300, SD_NOR_L,   0,  90,  65,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 40 */
	{   4,   ZU | NO_VEC | NO_ZU,        A_SPECIAL,     K_LM,    0,    4,   24,     0, 0x1a,   8,  0x80,       3, SC300, SD_NOR_L,   0,  90,  65,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 41 */
	{  10, BU_O | NO_VEC | NO_ZU,        A_SPECIAL,      K_L,    0,   10,   24,     0, 0x1c,  15,  0x80,       3, SC300, SD_NOR_L,   0,  90,  67,   71, NO_WEIGHT | AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 42 */
	{  12,   ZU | NO_VEC | NO_ZU,        A_SPECIAL,      K_L,    0,    4,   24,     0, 0x17,  10,  0x80,       3, SC300, SD_NOR_L,   0,  50,  87,    0,           AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 43 */
	{  12,   ZU | NO_VEC | NO_ZU,        A_SPECIAL,      K_L,    0,    4,   24,     0, 0x17,  10,  0x80,       3, SC300, SD_NOR_L,   0,  50,  87,    0,           AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 44 */
	{  12,   ZU | NO_VEC | NO_ZU,        A_SPECIAL,      K_L,    0,    4,   24,     0, 0x17,  10,  0x80,       3, SC300, SD_NOR_L,   0,  50,  87,    0,           AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 45 */
	{  11,   ZU | NO_VEC | NO_ZU,        A_SPECIAL,      K_L,    0,    4,   24,     0, 0x17,  10,  0x80,       3, SC300, SD_NOR_L,   0,  50,  89,    0,           AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 46 */
	{  11,   ZU | NO_VEC | NO_ZU,        A_SPECIAL,      K_L,    0,    4,   24,     0, 0x17,  10,  0x80,       3, SC300, SD_NOR_L,   0,  50,  89,    0,           AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 47 */
	{  11,   ZU | NO_VEC | NO_ZU,        A_SPECIAL,      K_L,    0,    4,   24,     0, 0x17,  10,  0x80,       3, SC300, SD_NOR_L,   0,  50,  89,    0,           AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 48 */
	{  12,   ZU | NO_VEC | NO_ZU,        A_SPECIAL,      K_L,    0,    4,   24,     0, 0x17,  10,  0x80,       3, SC300, SD_NOR_L,   0,  40,  91,    0,           AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 49 */
	{  12,   ZU | NO_VEC | NO_ZU,        A_SPECIAL,      K_L,    0,    4,   24,     0, 0x17,  10,  0x80,       3, SC300, SD_NOR_L,   0,  40,  91,    0,           AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 4a */
	{  12,   ZU | NO_VEC | NO_ZU,        A_SPECIAL,      K_L,    0,    4,   24,     0, 0x17,  10,  0x80,       3, SC300, SD_NOR_L,   0,  40,  91,    0,           AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 4b */
	{  11,   ZU | NO_VEC | NO_ZU,        A_SPECIAL,      K_L,    0,    4,   24,     0, 0x17,  10,  0x80,       3, SC300, SD_NOR_L,   0,  50,  93,    0,           AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 4c */
	{  11,   ZU | NO_VEC | NO_ZU,        A_SPECIAL,      K_L,    0,    4,   24,     0, 0x17,  10,  0x80,       3, SC300, SD_NOR_L,   0,  50,  93,    0,           AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 4d */
	{  11,   ZU | NO_VEC | NO_ZU,        A_SPECIAL,      K_L,    0,    4,   24,     0, 0x17,  10,  0x80,       3, SC300, SD_NOR_L,   0,  50,  93,    0,           AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 4e */
	{  18,                  BU_O,        A_SPECIAL,      K_L,    0,   10,   30,     0, 0x1c,   6,  0x80,       3, SC300, SD_NOR_L,   0,  90,  80,   81,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        5, { 0, 0 } },	/* 4f */
	{  10,         BU_O | NO_VEC,        A_SPECIAL,      K_L,    0,   10,   30,     0, 0x1e,   8,  0x80,       3, SC300, SD_NOR_L,   0,  90,  80,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        5, { 0, 0 } },	/* 50 */
	{  12,         BU_O | NO_VEC,        A_SPECIAL,      K_L,    0,   10,   30,     0, 0x1d,  13,  0x80,       3, SC300, SD_NOR_L,   0,  90,  81,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        5, { 0, 0 } },	/* 51 */
	{   8,         BU_O | NO_VEC,        A_SPECIAL,  K_L | 2,    0,   10,   30,     0, 0x1e,   8,  0x80,       3, SC300, SD_NOR_L,   0,  90,  79,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        5, { 0, 0 } },	/* 52 */
	{   8,         BU_O | NO_VEC,        A_SPECIAL,  K_L | 2,    0,   10,   30,     0, 0x1f,   8,  0x80,       3, SC300, SD_NOR_L,   0,  90,  80,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        5, { 0, 0 } },	/* 53 */
	{  14,         BU_O | NO_VEC,        A_SPECIAL,  K_L | 2,    0,   10,   30,     0, 0x1b,  11,  0x80,       3, SC300, SD_NOR_L,   0,  90,  81,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        5, { 0, 0 } },	/* 54 */
	{   4, BU_O | NO_VEC | NO_ZU,        A_SPECIAL,      K_L,    0,    6,   30,     0, 0x1e,   8,  0x80,       3, SC300, SD_NOR_L,   0,  90, 103,  104,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        5, { 0, 0 } },	/* 55 */
	{   4, BU_O | NO_VEC | NO_ZU,        A_SPECIAL,      K_L,    0,    6,   30,     0, 0x1e,   7,  0x80,       3, SC300, SD_NOR_L,   0,  90, 104,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        5, { 0, 0 } },	/* 56 */
	{  12,             2 | NO_ZU,        A_SPECIAL,      K_L,    0,   12,   30,     0, 0x10,  11,  0x80,       3, SC300, SD_NOR_L,   0,  90, 108,    0, NO_WEIGHT | AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        5, { 0, 0 } },	/* 57 */
	{  12,   ZU | NO_VEC | NO_ZU,        A_SPECIAL,      K_L,    0,    4,   24,     0, 0x17,  10,  0x80,       3, SC300, SD_NOR_L,   0,  40,  95,    0,           AERIAL_FINISH,  1,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 58 */
	{  12,   ZU | NO_VEC | NO_ZU,        A_SPECIAL,      K_L,    0,    4,   24,     0, 0x17,  10,  0x80,       3, SC300, SD_NOR_L,   0,  40,  95,    0,           AERIAL_FINISH,  1,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 59 */
	{  12,   ZU | NO_VEC | NO_ZU,        A_SPECIAL,      K_L,    0,    4,   24,     0, 0x17,  10,  0x80,       3, SC300, SD_NOR_L,   0,  40,  95,    0,           AERIAL_FINISH,  1,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 5a */
	{   8,    2 | NO_VEC | NO_ZU,        A_SPECIAL,  K_L | 2,    0,    4,   20,     0, 0x00,   8,  0x80,       3, SC300, SD_NOR_L,   0,  90, 116,  122,               NO_WEIGHT,  1,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 5b */
	{   8,    2 | NO_VEC | NO_ZU,        A_SPECIAL,  K_L | 2,    0,    4,   20,     0, 0x00,   8,  0x80,       3, SC300, SD_NOR_L,   0,  90, 116,  122,               NO_WEIGHT,  1,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 5c */
	{   8,    2 | NO_VEC | NO_ZU,        A_SPECIAL,  K_L | 2,    0,    4,   20,     0, 0x00,   8,  0x80,       3, SC300, SD_NOR_L,   0,  90, 116,  122,               NO_WEIGHT,  1,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 5d */
	{   4,           ZU | NO_VEC,            A_NOM,      K_S,    0,   10,   10,     0, 0x1a,   8,  0x70, MARK_N1, SC100, SD_NOR_S,   0,   0,   7,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 5e */
	{   5,   ZU | NO_VEC | NO_ZU,        A_SPECIAL,      K_L,    0,    4,   24,     0, 0x17,  10,  0x80,       3, SC300, SD_NOR_L,   0,  60, 145,    0,           AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 5f */
	{   5,   ZU | NO_VEC | NO_ZU,        A_SPECIAL,      K_L,    0,    4,   24,     0, 0x17,  10,  0x80,       3, SC300, SD_NOR_L,   0,  60, 145,    0,           AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 60 */
	{   5,   ZU | NO_VEC | NO_ZU,        A_SPECIAL,      K_L,    0,    4,   24,     0, 0x17,  10,  0x80,       3, SC300, SD_NOR_L,   0,  60, 145,    0,           AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 61 */
	{   8,   ZU | NO_VEC | NO_ZU,        A_SPECIAL,     K_LL,    0,    4,   24,     0, 0x1a,   6,  0x80,      14, SC300,       15,   0,  90, 135,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 62: pl0a_at_barrage, Berserker Barrage (Wolverine's power, sp_barrage), also Berserker Barrage X (sp_barragex) */
	{   8,   ZU | NO_VEC | NO_ZU,          A_HYPER,     K_LL,    0,    4,   24,     0, 0x1a,   6,  0x80,      14, SC300,       15,   0,  90, 135,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 63 */
	{ 0 },	/* 64 */
	{   8,            ZU | NO_ZU,          A_HYPER,      K_S,    0,   12,   24,     0, 0x00,   0,  0x80,       3, SC300,        8,   0,  60, 139,    0,                   NO_KO,  1,     0,  0,     1,     0,   0,        3, { 0, 0 } },	/* 65 */
	{   2,                     5,          A_HYPER,      K_M,    0,    2,   24,     0, 0x00,   0,  0x80,       3, SC300,        8,   0,  60, 170,    0,                   NO_KO,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 66 */
	{   2,                     5,          A_HYPER,      K_L,    0,    2,   24,     0, 0x00,   0,  0x80,       3, SC300,        8,   0,  60, 170,    0,                   NO_KO,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 67 */
	{   2,                     5,          A_HYPER,     K_LS,    0,    2,   24,     0, 0x00,   0,  0x80,       3, SC300,        8,   0,  60, 170,    0,                   NO_KO,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 68 */
	{   2,                     5,          A_HYPER,     K_LM,    0,    2,   24,     0, 0x00,   0,  0x80,       3, SC300,        8,   0,  60, 170,    0,                   NO_KO,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 69 */
	{   2,                     5,          A_HYPER,     K_LL,    0,    2,   24,     0, 0x00,   0,  0x80,       3, SC300,        8,   0,  60, 170,    0,                   NO_KO,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 6a */
	{   1, BU_O | NO_VEC | NO_ZU,        A_SPECIAL,      K_L,    0,   10,   24,     0, 0x1c,  13,  0x80,       3, SC300, SD_NOR_L,   0,  90, 153,    0, NO_CHIP | NO_WEIGHT | NO_KO,  1,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 6b: pl0a_at_kiss, the taunt's kiss (shl0a_00 kind 0) */
	{  15,                  BU_O,          A_HYPER,     K_LL,    0,    8,   24,     0, 0x00,  12,  0x80, MARK_N3, SC300, SD_KIK_L,   0,  90, 152,    0, NO_WEIGHT | NO_KO | BLACK_COMBO,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 6c */
	{  16,            2 | NO_VEC,        A_SPECIAL,  K_L | 6,    0,   10,   20,     0, 0x1c,  10,  0x80,       3, SC300, SD_NOR_L,   0,  90, 155,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 6d */
	{  16,            2 | NO_VEC,             A_HI,      K_L,    0,   10,   24,     0, 0x11,  10,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  90, 162,    0,           AERIAL_FINISH,  1,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 6e */
	{   9,            2 | NO_VEC,        A_SPECIAL,      K_L,    0,   10,   10,     0, 0x1a,  11,  0x60,       3, SC200, SD_NOR_M,   0,  90,  55,   56,                       0,  1,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 6f */
	{   8,   ZU | NO_VEC | NO_ZU,          A_HYPER,      K_M,    0,    4,   24,     0, 0x1a,   8,  0x80,       3, SC300, SD_NOR_L,   0,  90,  65,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 70 */
	{   8,   ZU | NO_VEC | NO_ZU,          A_HYPER,     K_LM,    0,    4,   24,     0, 0x1a,   8,  0x80,       3, SC300, SD_NOR_L,   0,  90,  65,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 71 */
	{  12, BU_O | NO_VEC | NO_ZU,          A_HYPER,      K_L,    0,   10,   24,     0, 0x1c,  15,  0x80,       3, SC300, SD_NOR_L,   0,  90,  67,   71, NO_WEIGHT | AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 72 */
	{   6,                    ZU, A_SPECIAL | 0x80,      K_L,    0,   10,   16,     0, 0x18,  10,  0x80,       3, SC300,        8,   0,  50, 158,    0,                       0,  1,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 73 */
	{   2, BU_O | NO_VEC | NO_ZU,        A_SPECIAL,      K_L,    0,    6,   30,     0, 0x1d,   7,  0x80,       3, SC300, SD_NOR_L,   0,  90, 104,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        5, { 0, 0 } },	/* 74 */
	{   6, BU_O | NO_VEC | NO_ZU,        A_SPECIAL,      K_L,    0,    6,   30,     0, 0x1d,   7,  0x80,       3, SC300, SD_NOR_L,   0,  90,  87,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 75 */
	{   6, BU_O | NO_VEC | NO_ZU,        A_SPECIAL,      K_L,    0,    6,   30,     0, 0x1d,   7,  0x80,       3, SC300, SD_NOR_L,   0,  40,  89,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 76 */
	{  10,           12 | NO_VEC,        A_SPECIAL,      K_L,    0,    4,   24,     0, 0x1c,  12,  0x80,       3, SC300,        8,  64,  90, 170,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 77: pl0a_at_snapback, Snapback (sp_snapback) */
	{  14,         BU_O | NO_VEC,        A_SPECIAL,     K_LL,    0,   10,   24,     0, 0x00,  12,  0x80, MARK_N3, SC300, SD_KIK_L,   0,  40, 169,    0, NO_CHIP | NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 78: pl0a_at_upper, the launcher */
	{   8,   ZU | NO_VEC | NO_ZU,        A_SPECIAL,      K_L,    0,    4,   24,     0, 0x1a,   6,  0x80,      12, SC300,       15,   0,  90, 134,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 79 */
	{   8,   ZU | NO_VEC | NO_ZU,          A_HYPER,      K_L,    0,    4,   24,     0, 0x1a,   6,  0x80,      12, SC300,       15,   0,  90, 134,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 7a */
	{  14, BU_O | NO_VEC | NO_ZU,        A_SPECIAL,      K_L,    0,   10,   24,     0, 0x1c,  13,  0x80,      12, SC300,       15,   0,  90, 135,    0, NO_WEIGHT | AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 7b */
	{  14, 2 | NO_VEC | NO_ZU | FLY_SCREEN,   A_HYPER | 0x80,      K_L,    0,   10,   24,     0, 0x1c,  13,  0x80,      12, SC300,        3,   0,  90, 135,    0, NO_WEIGHT | AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 7c */
	{  14, BU_O | NO_VEC | NO_ZU,        A_SPECIAL,     K_LL,    0,   10,   24,     0, 0x1a,  13,  0x80,      14, SC300,       15,   0,  90, 134,    0, NO_WEIGHT | AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 7d */
};
