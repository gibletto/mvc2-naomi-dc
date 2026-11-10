/* Ken (character 27): the attacks (HIT_DATA, 0x1c bytes).
   What each hit does. The character's module sets the attack's number (atck_no) with the move,
   and the row is read when the attack box meets a body. After a row is its number, in
   hexadecimal, and the program's name for it:
   ATCK_ for the normals every character has (player.h), pl27_at_ for its own (pl27_def.h).
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
     box, box2  boxes of hit_27 that the computer measures the attack's reach by
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

const HIT_DATA hit_dt_27[] = {
	/* dmg                   type          kind      elem  piyo  stop  sand  sleep   vec  pow  range     mark  score        se  shc  jug  box  box2                     flag  13  guard  15  catch  abyss  cnt    answer        1a */
	{   4,                    ZU,        A_NOM,      K_S,    4,   10,   10,     0, 0x18,   4,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  32,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 00: ATCK_STAND_LP */
	{   5,                    ZU,        A_NOM,     K_LM,    2,   11,   16,     0, 0x1e,   6,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40,  33,   64,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 01: ATCK_STAND_MP */
	{  13,                    ZU,        A_NOM,      K_L,    1,   12,   24,     0, 0x1a,   8,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  50,  34,    0,                       0,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 02: ATCK_STAND_HP */
	{   4,                    ZU,        A_NOM,      K_S,    4,   10,   10,     0, 0x18,   4,  0x60, MARK_N1, SC100, SD_KIK_S,   0,  20,  35,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 03: ATCK_STAND_LK */
	{   7,           ZU | NO_VEC,        A_NOM,     K_LM,    2,   10,   20,     0, 0x1a,   9,  0x60, MARK_N2, SC200, SD_KIK_M,   4,  20,  36,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 04: ATCK_STAND_MK */
	{  13,                  BU_O,        A_NOM,      K_L,    1,   10,   30,     0, 0x1a,  11,  0x80, MARK_N3, SC300, SD_KIK_L,   0,  30,  37,    0,                       0,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 05: ATCK_STAND_HK */
	{   4,                    ZU,        A_NOM,     K_LS,    4,   10,   10,     0, 0x1c,   4,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  38,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 06: ATCK_CROUCH_LP */
	{   7,                    ZU,        A_NOM,     K_LM,    2,   11,   16,     0, 0x13,   6,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40,  39,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 07: ATCK_CROUCH_MP */
	{  13,         BU_O | NO_VEC,        A_NOM,     K_LL,    1,   10,   24,     0, 0x00,  11,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  50,  20,   40, NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 08: ATCK_CROUCH_HP */
	{   4,                    ZU,         A_LO,     K_LS,    4,   10,   10,     0, 0x1c,   4,  0x70, MARK_N1, SC100, SD_KIK_S,   0,  20,  41,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 09: ATCK_CROUCH_LK */
	{   7,                    ZU,         A_LO,     K_LM,    2,   10,   20,     0, 0x1d,   6,  0x40, MARK_N2, SC200, SD_KIK_M,   0,  40,  42,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0a: ATCK_CROUCH_MK */
	{  13,            AS | NO_ZU,         A_LO,     K_LL,    1,   10,   30,     9, 0x1a,   8,  0x70, MARK_N3, SC300, SD_KIK_L,   0, 100,  43,    0,                       0,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 0b: ATCK_CROUCH_HK */
	{   4,           ZU | NO_VEC,         A_HI,      K_S,    1,   10,   14,     0, 0x00,   5,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  44,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0c: ATCK_JUMP_LP */
	{   7,           ZU | NO_VEC,         A_HI,     K_LM,    1,   10,   20,     6, 0x00,   2,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40,  45,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0d: ATCK_JUMP_MP */
	{  13,           ZU | NO_VEC,         A_HI,     K_LL,    1,   10,   30,     0, 0x16,   8,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  60,  46,    0,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 0e: ATCK_JUMP_HP */
	{   4,           ZU | NO_VEC,         A_HI,      K_S,    1,   10,   14,     0, 0x00,   5,  0x60, MARK_N1, SC100, SD_KIK_S,   0,  40,  47,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0f: ATCK_JUMP_LK */
	{   7,           ZU | NO_VEC,         A_HI,      K_M,    1,   10,   20,     0, 0x00,   2,  0x60, MARK_N2, SC200, SD_KIK_M,   0,  40,  48,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 10: ATCK_JUMP_MK */
	{  13,           ZU | NO_VEC,         A_HI,      K_L,    1,   10,   30,     0, 0x16,   8,  0x80, MARK_N3, SC300, SD_KIK_L,   0,  60,  49,    0,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 11: ATCK_JUMP_HK */
	{   8,           ZU | NO_VEC,         A_HI,      K_M,    1,   10,   20,     0, 0x18,   6,  0x60, MARK_N2, SC200, SD_KIK_M,   2,  40,  50,    0,                       0,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 12: pl27_at_fwd_hk_1, Forward + HK command normal, first hit */
	{   5,           ZU | NO_VEC,         A_HI,      K_M,    1,   10,   20,     0, 0x18,  10,  0x60, MARK_N2, SC200, SD_KIK_M,   2,  50,  51,    0,                       0,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 13: pl27_at_fwd_hk_2, Forward + HK command normal, second hit */
	{   4,           ZU | NO_VEC,         A_HI,      K_M,    1,   10,   20,     0, 0x00,   5,  0x60, MARK_N1, SC200, SD_KIK_S,   0,  20,  54,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 14: pl27_at_jump_lk2, His own jumping LK, used every time */
	{   7,           ZU | NO_VEC,         A_HI,      K_M,    1,   10,   20,     0, 0x00,   5,  0x60, MARK_N2, SC200, SD_KIK_M,   0,  40,  55,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 15: pl27_at_jump_mk2, His own jumping MK (holding up gives the common 0x10) */
	{  13,           ZU | NO_VEC,         A_HI,      K_M,    1,   10,   20,     0, 0x16,   8,  0x60, MARK_N3, SC200, SD_KIK_L,   0,  60,  77,    0,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 16: pl27_at_jump_hk2, His own jumping HK (holding up gives the common 0x11) */
	{  13,         BU_O | NO_VEC,    A_SPECIAL,     K_LL,    0,   10,   24,     0, 0x00,  11,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  40,  20,   40, NO_CHIP | NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 17: pl27_at_upper, Launcher (pl27_upper, crouching, Char_Extra 8) */
	{ 0 },	/* 18 */
	{   3,           ZU | NO_VEC,        A_NOM,      K_M,    2,   10,   20,     0, 0x1a,   9,  0x60, MARK_N2, SC200, SD_KIK_M,   4,  40,  63,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 19: pl27_at_stand_lk_2, Second hit of the standing LK and MK */
	{ 0 },	/* 1a */
	{ 0 },	/* 1b */
	{ 0 },	/* 1c */
	{ 0 },	/* 1d */
	{ 0 },	/* 1e */
	{ 0 },	/* 1f */
	{  16,                  BU_O, A_NOM | 0x40,     K_LM,    4,   12,   20,     0, 0x04,   8,  0x60, MARK_N2, SC200, SD_NOR_M,   0,  40,  35,   36,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 20: pl27_at_throw, Throw: the P ground throw and the K air throw both use it */
	{   4,                     2, A_NOM | 0x40,     K_LM,    1,   12,   20,     0, 0x06,   8,  0x60, MARK_N2, SC200, SD_NOR_M,   1,  40,  35,   36,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 21: pl27_at_throw_k_hit, K throw: each hit of the rolls */
	{   8,                     2, A_NOM | 0x40,     K_LM,    2,   12,   20,     0, 0x1a,  12,  0x60, MARK_N2, SC200, SD_NOR_M,   2,  40,  35,   36,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 22: pl27_at_throw_k, K throw: letting go after the rolls */
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
	{  12,   ZU | NO_VEC | NO_ZU,    A_SPECIAL,      K_L,    0,    4,   20,     0, 0x18,   6,  0x60,       3, SC200, SD_NOR_M,   0,  50,  56,    0,                       0,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 30 */
	{  12,   ZU | NO_VEC | NO_ZU,    A_SPECIAL,      K_L,    0,    4,   20,     0, 0x18,  10,  0x60,       3, SC200, SD_NOR_M,   0,  50,  56,    0,                       0,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 31 */
	{ 0 },	/* 32 */
	{  20,                  BU_O,    A_SPECIAL,        2,    0,   10,   30,     0, 0x1f,   8,  0x80,       3, SC300, SD_NOR_L,   0,  90,  58,   59,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 33 */
	{  20,                  BU_O,    A_SPECIAL,        2,    0,   10,   30,     0, 0x1f,   8,  0x80,       3, SC300, SD_NOR_L,   0,  90,  58,   59,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 34 */
	{  20,         BU_O | NO_VEC,    A_SPECIAL,        2,    0,   10,   30,     0, 0x1e,   8,  0x80,       3, SC300, SD_NOR_L,   6, 100,  68,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 35 */
	{ 0 },	/* 36 */
	{  14,         BU_O | NO_VEC,    A_SPECIAL,        2,    0,   10,   30,     0, 0x1e,   8,  0x80,       3, SC300, SD_NOR_L,   8, 100,  58,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 37 */
	{   6,         BU_O | NO_VEC,    A_SPECIAL,        2,    0,   10,   30,     0, 0x1e,  12,  0x80,       3, SC300, SD_NOR_L,   6, 100,  58,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 38 */
	{   6,                  BU_O,    A_SPECIAL,        2,    0,   10,   30,     0, 0x1e,   9,  0x80,       3, SC300, SD_NOR_L,   6, 100,  59,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 39 */
	{   3,    2 | NO_VEC | NO_ZU,    A_SPECIAL,      K_S,    0,    6,   30,     0, 0x1d,   3,  0x80,       3, SC300, SD_KIK_L,   8,  90,  60,   61,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 3a */
	{   4,    2 | NO_VEC | NO_ZU,    A_SPECIAL,      K_S,    0,    6,   30,     0, 0x1e,   5,  0x80,       3, SC300, SD_KIK_L,   4,  90,  60,   61,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 3b */
	{ 0 },	/* 3c */
	{  15,   ZU | NO_VEC | NO_ZU,    A_SPECIAL,      K_M,    0,   10,   30,     0, 0x1c,   8,  0x80,       3, SC300, SD_KIK_L,   6,  90,  60,   61,                       0,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 3d */
	{  17,   ZU | NO_VEC | NO_ZU,    A_SPECIAL,      K_M,    0,   10,   30,     0, 0x1c,   8,  0x80,       3, SC300, SD_KIK_L,   6,  90,  60,   61,                       0,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 3e */
	{ 0 },	/* 3f */
	{  12,   ZU | NO_VEC | NO_ZU,    A_SPECIAL,     K_LL,    0,    4,   20,     0, 0x18,   6,  0x60,       3, SC200, SD_NOR_M,   5,  50, 101,    0,           AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 40 */
	{  12,   ZU | NO_VEC | NO_ZU,    A_SPECIAL,     K_LL,    0,    4,   20,     0, 0x18,   4,  0x60,       3, SC200, SD_NOR_M,   5,  50, 101,    0,           AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 41 */
	{ 0 },	/* 42 */
	{   4,                    ZU,        A_NOM,      K_S,    1,   10,   10,     0, 0x18,   4,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  32,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 43 */
	{   4,                    ZU,        A_NOM,      K_S,    1,   10,   10,     0, 0x18,   4,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  32,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 44 */
	{   4,                    ZU,        A_NOM,      K_S,    1,   10,   10,     0, 0x18,   4,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  32,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 45 */
	{   4,                    ZU,        A_NOM,      K_S,    1,   10,   10,     0, 0x18,   4,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  32,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 46 */
	{   4,                    ZU,        A_NOM,      K_S,    1,   10,   10,     0, 0x18,   4,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  32,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 47 */
	{   4,                    ZU,        A_NOM,      K_S,    1,   10,   10,     0, 0x18,   4,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  32,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 48 */
	{  10,   ZU | NO_VEC | NO_ZU,      A_HYPER, K_LM | 2,    0,    6,   20,     0, 0x18,   4,  0x60,       3, SC200, SD_NOR_M,   0,  90, 100,   99,                       0,  1,     0,  4,     0,     0,   0,        3, { 0, 0 } },	/* 49: pl27_at_reppa, Shouryuu Reppa hyper */
	{   3,   ZU | NO_VEC | NO_ZU,      A_HYPER, K_LM | 2,    0,    6,   20,     0, 0x1e,  11,  0x60,       3, SC200, SD_NOR_M,   0,  90, 100,   99,                       0,  1,     0,  4,     0,     0,   0,        3, { 0, 0 } },	/* 4a */
	{   8, BU_O | NO_VEC | NO_ZU,      A_HYPER, K_LL | 2,    0,   10,   30,     0, 0x1b,  11,  0x60,       3, SC200, SD_NOR_M,   0,  90, 100,   99,                       0,  1,     0,  4,     0,     0,   0,        3, { 0, 0 } },	/* 4b */
	{   4,          BU_O | NO_ZU,      A_HYPER,        2,    0,    2,   20,     0, 0x00,   8,  0x60,       3, SC200, SD_NOR_M,   0,  90,  80,    0,                       0,  1,     0,  5,     0,     0,   8,        3, { 0, 0 } },	/* 4c: pl27_at_shinryuu, Shinryuuken hyper, start (later hits come from the animation's cg_flag1) */
	{   3,          BU_O | NO_ZU,      A_HYPER,        2,    0,    2,   20,     0, 0x00,   6,  0x60,       3, SC200, SD_NOR_M,   0,  40,  80,    0,                       0,  1,     0,  5,     0,     0,   8,        3, { 0, 0 } },	/* 4d */
	{  20,        2 | FLY_SCREEN,      A_HYPER,        2,    0,    2,   20,     0, 0x1c,  15,  0x60,       3, SC200, SD_NOR_M,   0,  90,  80,    0,                       0,  1,     0,  5,     0,     0,   8,        3, { 0, 0 } },	/* 4e: pl27_at_shinryuu_top, Shinryuuken hit high in the air, the finish */
	{   4,         BU_O | NO_VEC,      A_HYPER,        2,    0,   10,   30,     0, 0x1e,   8,  0x80,       3, SC300, SD_NOR_L,  16,  90, 100,   99,               NO_WEIGHT,  1,     0,  4,     0,     0,   0,        3, { 0, 0 } },	/* 4f */
	{  14,         BU_O | NO_VEC,    A_SPECIAL,        2,    0,   10,   30,     0, 0x1e,  13,  0x80,       3, SC300, SD_NOR_L,  16, 100,  68,   59,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 50: pl27_at_jshouryuu, Shouryuuken in the air */
	{  10,           10 | NO_VEC,    A_SPECIAL,      K_L,    0,   10,   10,     0, 0x1f,  10,  0x60,       3, SC200,       12,   0,  90,  81,    0, NO_CHIP | NO_WEIGHT | AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        5, { 0, 0 } },	/* 51: pl27_at_t_kick, Partner's wall-leap kick (t_kick) */
	{   3,   ZU | NO_VEC | NO_ZU,      A_HYPER,  K_M | 2,    0,    4,   20,     0, 0x1a,   6,  0x60,       3, SC200, SD_KIK_M,   0,  90,  84,    0,                       0,  1,     0,  6,     0,     0,   0,        3, { 0, 0 } },	/* 52 */
	{ 0 },	/* 53 */
	{ 0 },	/* 54 */
	{   4,    2 | NO_VEC | NO_ZU,      A_HYPER,      K_M,    0,    4,   20,     0, 0x1a,   6,  0x60,       6, SC200,       18,   0,  90, 106,  108,                       0,  1,     0,  3,     0,     0,   0,        3, { 0, 0 } },	/* 55 */
	{   3,    2 | NO_VEC | NO_ZU,      A_HYPER,      K_M,    0,    4,   20,     0, 0x1a,   6,  0x60,       6, SC200,       18,   0,  90, 106,  108,                 NO_CHIP,  1,     0,  3,     0,     0,   0,        3, { 0, 0 } },	/* 56 */
	{   8,            2 | NO_VEC,    A_SPECIAL,      K_M,    0,   10,   20,     0, 0x1a,   9,  0x60,       3, SC200, SD_KIK_M,   4,  90,  63,    0,                       0,  0,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 57: pl27_at_tatsumaki_last, Tatsumaki's last turn, only after the spin has hit */
	{   5,    2 | NO_VEC | NO_ZU,      A_HYPER,  K_M | 2,    0,    6,   30,     0, 0x1e,   7,  0x80,       3, SC300, SD_KIK_L,   8,  90,  60,   61,               NO_WEIGHT,  1,     0,  6,     0,     0,   0,        3, { 0, 0 } },	/* 58: pl27_at_shippu, Shippuu Jinraikyaku hyper */
	{   6,    2 | NO_VEC | NO_ZU,      A_HYPER,  K_M | 2,    0,    6,   30,     0, 0x1d,   5,  0x80,       3, SC300, SD_KIK_L,   8,  90,  60,   61,               NO_WEIGHT,  1,     0,  6,     0,     0,   0,        3, { 0, 0 } },	/* 59 */
	{  14,            2 | NO_VEC,      A_HYPER,  K_M | 2,    0,   10,   20,     0, 0x1a,   9,  0x60,       3, SC200, SD_KIK_M,   4,  90,  63,    0,                       0,  0,     0,  6,     0,     0,   0,        3, { 0, 0 } },	/* 5a */
	{  10,           12 | NO_VEC,    A_SPECIAL,      K_L,    0,    4,   24,     0, 0x1c,  12,  0x80,       3, SC300,        8,  64,  90,  50,   51,               NO_WEIGHT,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 5b: pl27_at_snapback, Snapback */
	{  16,    2 | NO_VEC | NO_ZU,      A_HYPER,  K_L | 2,    0,    6,   30,     0, 0x1e,  12,  0x80,       3, SC200, SD_NOR_M,   0,  90, 100,   99,               NO_WEIGHT,  1,     0,  4,     0,     0,   0,        3, { 0, 0 } },	/* 5c */
	{   6,    2 | NO_VEC | NO_ZU,      A_HYPER,  K_M | 2,    0,    6,   30,     0, 0x1f,   0,  0x80,       3, SC300, SD_KIK_L,   8,  90,  60,   61,               NO_WEIGHT,  1,     0,  6,     0,     0,   0,        3, { 0, 0 } },	/* 5d */
};
