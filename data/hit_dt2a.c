/* Storm (character 2a): the attacks (HIT_DATA, 0x1c bytes).
   What each hit does. The character's module sets the attack's number (atck_no) with the move,
   and the row is read when the attack box meets a body. After a row is its number, in
   hexadecimal, and the program's name for it:
   ATCK_ for the normals every character has (player.h), pl2a_at_ for its own (pl2a_def.h).
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
     box, box2  boxes of hit_2a that the computer measures the attack's reach by
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

const HIT_DATA hit_dt_2a[] = {
	/* dmg                   type          kind      elem  piyo  stop  sand  sleep   vec  pow  range     mark  score        se  shc  jug  box  box2                     flag  13  guard  15  catch  abyss  cnt    answer        1a */
	{   4,           ZU | NO_VEC,        A_NOM,  K_S | 3,    4,    8,   10,     0, 0x18,   2,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,   7,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 00: ATCK_STAND_LP */
	{   7,                    ZU,        A_NOM,  K_M | 3,    2,    8,   16,     0, 0x18,   4,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40,   8,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 01: ATCK_STAND_MP */
	{  13,                  BU_O,        A_NOM,  K_L | 3,    1,   12,   24,     0, 0x1a,  13,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  50,   9,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 02: ATCK_STAND_HP */
	{   4,           ZU | NO_VEC,        A_NOM,      K_S,    4,    8,   10,     0, 0x18,   3,  0x60, MARK_N1, SC100, SD_KIK_S,   0,  20,  10,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 03: ATCK_STAND_LK */
	{   7,           ZU | NO_VEC,        A_NOM,      K_M,    2,    8,   16,     0, 0x18,   5,  0x60, MARK_N2, SC200, SD_KIK_M,   0,  40,  11,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 04: ATCK_STAND_MK */
	{  13,         BU_O | NO_VEC,        A_NOM,     K_LL,    1,   12,   24,     0, 0x00,  11,  0x80, MARK_N3, SC300, SD_KIK_L,   0,  50,  12,   38, NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 05: ATCK_STAND_HK */
	{   4,           ZU | NO_VEC,        A_NOM, K_LS | 3,    4,   10,   10,     0, 0x1c,   5,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  13,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 06: ATCK_CROUCH_LP */
	{   7,                    ZU,        A_NOM, K_LM | 3,    2,   10,   16,     0, 0x18,   6,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40,  14,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 07: ATCK_CROUCH_MP */
	{  13,         BU_O | NO_VEC,        A_NOM, K_LL | 3,    1,    8,   24,     0, 0x00,  11,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  50,  15,    0, NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 08: ATCK_CROUCH_HP */
	{   4,           ZU | NO_VEC,         A_LO,     K_LS,    4,    8,   10,     0, 0x18,   6,  0x70, MARK_N1, SC100, SD_KIK_S,   0,  20,  16,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 09: ATCK_CROUCH_LK */
	{   7,                    AS,         A_LO,     K_LM,    2,   10,   16,     9, 0x18,   8,  0x40, MARK_N2, SC200, SD_KIK_M,   0,  90,  17,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0a: ATCK_CROUCH_MK */
	{  13,         BU_O | NO_VEC,        A_NOM,     K_LL,    1,    8,   24,     0, 0x1f,  11,  0x70, MARK_N3, SC300, SD_KIK_L,   0,  50,  18,   53, NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0b: ATCK_CROUCH_HK */
	{   4,           ZU | NO_VEC,         A_HI,  K_S | 3,    1,    8,   10,     0, 0x00,   5,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  19,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0c: ATCK_JUMP_LP */
	{   7,           ZU | NO_VEC,         A_HI,  K_M | 3,    1,   10,   16,     0, 0x00,   4,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40, 113,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0d: ATCK_JUMP_MP */
	{  13,           ZU | NO_VEC,         A_HI,  K_L | 3,    1,   11,   24,     0, 0x14,  10,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  60,  21,    0,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0e: ATCK_JUMP_HP */
	{   3,           ZU | NO_VEC,         A_HI,      K_S,    1,    8,   10,     0, 0x00,   5,  0x60, MARK_N1, SC100, SD_KIK_S,   0,  20,  22,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0f: ATCK_JUMP_LK */
	{   7,           ZU | NO_VEC,         A_HI,      K_M,    1,    8,   16,     0, 0x00,   4,  0x60, MARK_N2, SC200, SD_KIK_M,   0,  40,  23,   59,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 10: ATCK_JUMP_MK */
	{  13,           ZU | NO_VEC,         A_HI,      K_L,    1,   12,   24,     0, 0x18,  12,  0x80, MARK_N3, SC300, SD_KIK_L,   0,  60,  24,   60,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 11: ATCK_JUMP_HK */
	{ 0 },	/* 12 */
	{ 0 },	/* 13 */
	{  12,         BU_O | NO_VEC,        A_NOM,     K_LL,    0,   10,   24,     0, 0x00,  11,  0x80, MARK_N3, SC300, SD_KIK_L,   0,  90,  12,    0, NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 14 */
	{ 0 },	/* 15 */
	{ 0 },	/* 16 */
	{ 0 },	/* 17 */
	{  12,   ZU | NO_VEC | NO_ZU,    A_SPECIAL,  K_M | 3,    0,   10,   16,     0, 0x1e,   0,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  50,  25,    0,                       0,  0,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 18: pl2a_at_jsphere, Lightning Sphere in the air: the bolts shl2a_04 throws */
	{   8,                    ZU, A_NOM | 0x80,      K_L,    1,   10,   24,     0, 0x1f,  10,  0x80, MARK_N3, SC300, SD_KIK_L,   0,  50,  71,   72,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 19 */
	{   8,         BU_O | NO_VEC, A_NOM | 0x80,  K_L | 3,    1,   10,   36,     0, 0x00,  11,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  50,  91,  180, NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 1a */
	{   6,           ZU | NO_VEC, A_NOM | 0x80,      K_S,    4,   10,   10,     0, 0x18,  12,  0x60, MARK_N1, SC100, SD_KIK_S,   0,  20,  52,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 1b */
	{   6,           ZU | NO_VEC,  A_LO | 0x80,     K_LS,    4,   10,   10,     0, 0x18,  12,  0x70, MARK_N1, SC100, SD_KIK_S,   0,  20,  93,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 1c */
	{ 0 },	/* 1d */
	{ 0 },	/* 1e */
	{ 0 },	/* 1f */
	{  16,                  BU_O, A_NOM | 0x40,        0,    0,   16,   20,    15, 0x1e,  11,  0x80, MARK_N1, SC300,       76,   0,   0,   0,    0,                       1,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 20: pl2a_at_throw_k, K throw (player and opponent) */
	{   9,                  BU_O, A_NOM | 0x40,        3,    0,   16,   10,    15, 0x1d,  11,  0x80, MARK_N1, SC300,       76,   0,   0,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 21: pl2a_at_throw_p, P throw, ground or air: the final throw */
	{   1,                     2, A_NOM | 0x40,        3,    0,    8,    0,    15, 0x10,  10,  0x80, MARK_N1, SC300, SD_NOR_S,   0,   0,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 22: pl2a_at_throw_p_hit, P throw: each hit while holding */
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
	{   5,   ZU | NO_VEC | NO_ZU,    A_SPECIAL,      K_L,    0,   10,   16,     0, 0x1c,   5,  0x80,       3, SC200, SD_NOR_M,   0,  40,  26,    0,           AERIAL_FINISH,  1,     0,  0,     0,     0,   4,        4, { 0, 0 } },	/* 30: pl2a_at_typhoon, Typhoon, ground or air */
	{   6,   ZU | NO_VEC | NO_ZU,    A_SPECIAL,      K_L,    0,   10,   16,     0, 0x1c,   5,  0x80,       3, SC200, SD_NOR_M,   0,  40,  26,    0,           AERIAL_FINISH,  1,     0,  0,     0,     0,   4,        4, { 0, 0 } },	/* 31 */
	{   7,   ZU | NO_VEC | NO_ZU,    A_SPECIAL,      K_L,    0,   10,   16,     0, 0x1c,   5,  0x80,       3, SC200, SD_NOR_M,   0,  40,  26,    0,           AERIAL_FINISH,  1,     0,  0,     0,     0,   4,        4, { 0, 0 } },	/* 32 */
	{ 0 },	/* 33 */
	{ 0 },	/* 34 */
	{ 0 },	/* 35 */
	{ 0 },	/* 36 */
	{ 0 },	/* 37 */
	{ 0 },	/* 38 */
	{ 0 },	/* 39 */
	{ 0 },	/* 3a */
	{ 0 },	/* 3b */
	{ 0 },	/* 3c */
	{ 0 },	/* 3d */
	{ 0 },	/* 3e */
	{ 0 },	/* 3f */
	{ 0 },	/* 40 */
	{ 0 },	/* 41 */
	{ 0 },	/* 42 */
	{ 0 },	/* 43 */
	{ 0 },	/* 44 */
	{ 0 },	/* 45 */
	{ 0 },	/* 46 */
	{ 0 },	/* 47 */
	{ 0 },	/* 48 */
	{ 0 },	/* 49 */
	{ 0 },	/* 4a */
	{ 0 },	/* 4b */
	{ 0 },	/* 4c */
	{ 0 },	/* 4d */
	{ 0 },	/* 4e */
	{ 0 },	/* 4f */
	{ 0 },	/* 50 */
	{ 0 },	/* 51 */
	{ 0 },	/* 52 */
	{ 0 },	/* 53 */
	{   7, BU_O | NO_VEC | NO_ZU,    A_SPECIAL,  K_L | 3,    0,   16,   10,    15, 0x00,  12,  0x80,       3, SC300,        8,   0,  90,  34,    0,                       0,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 54 */
	{   7, BU_O | NO_VEC | NO_ZU,    A_SPECIAL,  K_L | 3,    0,   16,   30,    15, 0x1f,   8,  0x80,       3, SC300,        8,   0,  90,  34,    0,                       0,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 55 */
	{   7, BU_O | NO_VEC | NO_ZU,    A_SPECIAL,  K_L | 3,    0,   16,   30,    15, 0x1f,   8,  0x80,       3, SC300,        8,   0,  90,  34,    0,                       0,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 56 */
	{   6,   ZU | NO_VEC | NO_ZU,    A_SPECIAL,      K_L,    0,   10,   16,     0, 0x1c,   5,  0x80,       3, SC200, SD_NOR_M,   0,  40,  26,    0,                       0,  1,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 57 */
	{   6,   ZU | NO_VEC | NO_ZU,    A_SPECIAL,      K_L,    0,   10,   16,     0, 0x1c,   5,  0x80,       3, SC200, SD_NOR_M,   0,  40,  26,    0,                       0,  1,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 58 */
	{   6,   ZU | NO_VEC | NO_ZU,    A_SPECIAL,      K_L,    0,   10,   16,     0, 0x1c,   5,  0x80,       3, SC200, SD_NOR_M,   0,  40,  26,    0,                       0,  1,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 59 */
	{   3,             2 | NO_ZU,      A_HYPER,  K_L | 1,    0,    4,   10,     0, 0x14,   2,  0x70,       3, SC100, SD_NOR_S,   0,  90,  36,    0,                       0,  1,     0,  0,     0,     0,   4,        3, { 0, 0 } },	/* 5a: pl2a_at_hail, Hail Storm hyper */
	{   5, BU_O | NO_VEC | NO_ZU,    A_SPECIAL,      K_L,    0,   10,   16,     0, 0x1f,  11,  0x80,       3, SC200, SD_NOR_M,   0,  40,  37,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   4, YK_GD060, { 0, 0 } },	/* 5b: pl2a_at_dtyphoon, Double Typhoon (player and shl2a_01) */
	{  10,           10 | NO_VEC,    A_SPECIAL,      K_L,    0,   10,   10,     0, 0x1f,  10,  0x60,       3, SC200,        8,   0,  90,  78,    0, NO_CHIP | NO_WEIGHT | AERIAL_FINISH,  1,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 5c: pl2a_at_t_kick, Partner's wall-leap kick (t_kick) */
	{   5,             2 | NO_ZU,      A_HYPER,  K_L | 3,    0,    4,   10,    16, 0x00,   2,  0x70,       3, SC100, SD_NOR_S,   0,  90,  27,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   6,        3, { 0, 0 } },	/* 5d: pl2a_at_lstorm, Lightning Storm hyper */
	{   5,             2 | NO_ZU,      A_HYPER,  K_L | 3,    0,    4,   10,    16, 0x00,   2,  0x70,       3, SC100, SD_NOR_S,   0,  90,  27,    0,     NO_CHIP | NO_WEIGHT,  1,     0,  0,     0,     0,   6,        3, { 0, 0 } },	/* 5e */
	{   4,             2 | NO_ZU,      A_HYPER,  K_L | 1,    0,    4,   10,     0, 0x14,   2,  0x70,       3, SC100, SD_NOR_S,   0,  90,  36,    0,                 NO_CHIP,  1,     0,  0,     0,     0,   4,        3, { 0, 0 } },	/* 5f */
	{  10,           12 | NO_VEC,    A_SPECIAL,      K_L,    0,    4,   24,     0, 0x1c,  12,  0x80,       3, SC300,        8,  64,  90,  98,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 60: pl2a_at_snapback, Snapback (sp_snapback) */
	{  13,         BU_O | NO_VEC,    A_SPECIAL,     K_LL,    0,   12,   24,     0, 0x00,  11,  0x80, MARK_N3, SC300, SD_KIK_L,   0,  40,  99,    0, NO_CHIP | NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 61: pl2a_at_upper, Launcher (pl2a_upper) */
};
