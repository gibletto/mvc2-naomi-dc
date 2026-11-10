/* Cammy (character 24): the attacks (HIT_DATA, 0x1c bytes).
   What each hit does. The character's module sets the attack's number (atck_no) with the move,
   and the row is read when the attack box meets a body. After a row is its number, in
   hexadecimal, and the program's name for it:
   ATCK_ for the normals every character has (player.h), pl24_at_ for its own (pl24_def.h).
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
     box, box2  boxes of hit_24 that the computer measures the attack's reach by
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

const HIT_DATA hit_dt_24[] = {
	/* dmg                     type              kind      elem  piyo  stop  sand  sleep   vec  pow  range     mark  score        se  shc  jug  box  box2                     flag  13  guard  15  catch  abyss  cnt    answer        1a */
	{   4,                      ZU,            A_NOM,      K_S,    4,   10,   10,     0, 0x1a,   2,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,   7,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 00: ATCK_STAND_LP */
	{   8,                      ZU,            A_NOM,      K_M,    2,   10,   16,     0, 0x1c,   4,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40,   8,   17,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 01: ATCK_STAND_MP */
	{  14,                      ZU,            A_NOM,      K_L,    1,   10,   24,     0, 0x1a,   6,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  50,   9,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 02: ATCK_STAND_HP */
	{   4,                      ZU,            A_NOM,     K_LS,    4,   10,   10,     0, 0x1a,   1,  0x60, MARK_N1, SC100, SD_KIK_S,   0,  20,  10,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 03: ATCK_STAND_LK */
	{   8,                      ZU,            A_NOM,      K_M,    2,   10,   16,     0, 0x1a,   4,  0x60, MARK_N2, SC200, SD_KIK_M,   0,  40,  11,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 04: ATCK_STAND_MK */
	{  14,           BU_O | NO_VEC,            A_NOM,      K_L,    1,   10,   24,     0, 0x00,  11,  0x80, MARK_N3, SC300, SD_KIK_L,   0,  50,  12,    0, NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 05: ATCK_STAND_HK */
	{   3,             ZU | NO_VEC,            A_NOM,     K_LS,    4,   10,   10,     0, 0x1a,   2,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  13,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 06: ATCK_CROUCH_LP */
	{   6,                      ZU,            A_NOM,     K_LM,    2,   10,   16,     0, 0x1a,   4,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40,  14,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 07: ATCK_CROUCH_MP */
	{  12,           BU_O | NO_VEC,            A_NOM,      K_L,    1,   10,   24,     0, 0x00,  11,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  50,  15,   90, NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 08: ATCK_CROUCH_HP */
	{   3,             ZU | NO_VEC,             A_LO,     K_LS,    4,   10,   10,     0, 0x1a,   2,  0x70, MARK_N1, SC100, SD_KIK_S,   0,  20,  16,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 09: ATCK_CROUCH_LK */
	{   6,                      ZU,             A_LO,     K_LM,    2,   10,   16,     0, 0x1a,   4,  0x40, MARK_N2, SC200, SD_KIK_M,   0,  40,  54,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0a: ATCK_CROUCH_MK */
	{  12,              AS | NO_ZU,             A_LO,     K_LL,    1,   10,   24,     9, 0x1a,   8,  0x70, MARK_N3, SC300, SD_KIK_L,   0,  90,  55,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0b: ATCK_CROUCH_HK */
	{   3,             ZU | NO_VEC,             A_HI,      K_S,    1,   10,   10,     0, 0x00,   5,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  19,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0c: ATCK_JUMP_LP */
	{   6,             ZU | NO_VEC,             A_HI,      K_M,    1,   10,   16,     0, 0x00,   5,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40,  20,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0d: ATCK_JUMP_MP */
	{  12,             ZU | NO_VEC,             A_HI,      K_M,    1,   10,   24,     0, 0x17,   8,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  60,  21,    0,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0e: ATCK_JUMP_HP */
	{   3,             ZU | NO_VEC,             A_HI,      K_S,    1,   10,   10,     0, 0x00,   5,  0x60, MARK_N1, SC100, SD_KIK_S,   0,  20,  22,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0f: ATCK_JUMP_LK */
	{   6,             ZU | NO_VEC,             A_HI,      K_M,    1,   10,   16,     0, 0x00,   5,  0x60, MARK_N2, SC200, SD_KIK_M,   0,  40,  23,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 10: ATCK_JUMP_MK */
	{  12,             ZU | NO_VEC,             A_HI,      K_M,    1,   10,   24,     0, 0x18,   8,  0x80, MARK_N3, SC300, SD_KIK_L,   0,  60,  24,    0,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 11: ATCK_JUMP_HK */
	{   3,                      ZU,            A_NOM,      K_S,    4,   10,   10,     0, 0x1a,   2,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  60,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 12: pl24_at_stand_lp, Her standing LP; she never uses the common 0x00 */
	{   6,                      ZU,            A_NOM,      K_M,    2,   10,   16,     0, 0x1a,   4,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40,  62,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 13: pl24_at_stand_mp, Her standing MP (second press); she never uses the common 0x01 */
	{  12,                      ZU,            A_NOM,     K_LL,    1,   10,   24,     0, 0x1a,   6,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  50,  64,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 14: pl24_at_stand_hp, Her standing HP; she never uses the common 0x02 */
	{   3,                      ZU,            A_NOM,      K_S,    4,   10,   10,     0, 0x1a,   2,  0x60, MARK_N1, SC100, SD_KIK_S,   0,  20,  69,   71,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 15 */
	{   6,                      ZU,            A_NOM,      K_M,    2,   10,   16,     0, 0x1a,   4,  0x60, MARK_N2, SC200, SD_KIK_M,   0,  40,  59,   72,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 16: pl24_at_stand_mk, Her standing MK (second press of a kick) */
	{  12,             ZU | NO_VEC,            A_NOM,     K_LL,    1,   10,   24,     0, 0x1c,   6,  0x80, MARK_N3, SC300, SD_KIK_L,   0,  50,  46,   51,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 17: pl24_at_back_hk, Back + HK command normal (the code tests KEY_BACK) */
	{ 0 },	/* 18 */
	{ 0 },	/* 19 */
	{ 0 },	/* 1a */
	{ 0 },	/* 1b */
	{ 0 },	/* 1c */
	{ 0 },	/* 1d */
	{ 0 },	/* 1e */
	{ 0 },	/* 1f */
	{  16,                       2,     A_NOM | 0x40,        0,    0,    4,    0,    24, 0x1c,   4,  0x80, MARK_N1, SC300, SD_NOR_L,   0,   0,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 20: pl24_at_throw_k, K ground throw (catch_no 0) */
	{  16,                       2,     A_NOM | 0x40,        0,    0,   16,    0,    15, 0x1f,  10,  0x80, MARK_N1, SC300, SD_NOR_L,   0,   0,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 21: pl24_at_air_throw_k, K air throw (catch_no 1) */
	{  18,                       2,     A_NOM | 0x40,        0,    0,    8,    0,    15, 0x15,   8,  0x80, MARK_N1, SC300, SD_NOR_L,   0,   0,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 22: pl24_at_air_throw_p, P air throw (catch_no 2) */
	{  18,                       2, A_SPECIAL | 0x40,        0,    0,    4,    0,    15, 0x1c,   4,  0x80, MARK_N1, SC300, SD_NOR_L,   0,   0,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 23: pl24_at_hooligan_throw_h, Hooligan Combination throw on an airborne or low-held opponent (catch_no 0xc3) */
	{  18,                       2, A_SPECIAL | 0x40,        0,    0,    4,    0,    15, 0x1c,   4,  0x80, MARK_N1, SC300, SD_NOR_L,   0,   0,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 24: pl24_at_hooligan_throw_l, Hooligan Combination throw on a standing or crouching opponent (catch_no 0xc4) */
	{  16,                       2,     A_NOM | 0x40,        0,    0,    4,    0,    15, 0x1c,   4,  0x80, MARK_N1, SC300, SD_NOR_L,   0,   0,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 25: pl24_at_throw_p, P ground throw (catch_no 5) */
	{  34,                       2,   A_HYPER | 0x40,        0,    0,    4,    0,    15, 0x1c,   4,  0x80, MARK_N1, SC300, SD_NOR_L,   0,   0,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 26: pl24_at_killerbee_throw, Killer Bee Assault's final throw (set on the thrown opponent) */
	{ 0 },	/* 27 */
	{ 0 },	/* 28 */
	{ 0 },	/* 29 */
	{ 0 },	/* 2a */
	{ 0 },	/* 2b */
	{ 0 },	/* 2c */
	{ 0 },	/* 2d */
	{ 0 },	/* 2e */
	{ 0 },	/* 2f */
	{  18,           BU_O | NO_VEC,        A_SPECIAL,  K_S | 6,    0,   10,   20,     0, 0x1d,  11,  0x80,       3, SC300, SD_KIK_L,   0,  90,  44,   45,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        5, { 0, 0 } },	/* 30 */
	{ 0 },	/* 31 */
	{  20,           BU_O | NO_VEC,        A_SPECIAL,  K_L | 6,    0,   10,   20,     0, 0x1d,  13,  0x80,       3, SC300, SD_KIK_L,   0,  90,  44,   45,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        5, { 0, 0 } },	/* 32 */
	{  14,              2 | NO_VEC,        A_SPECIAL,  K_L | 6,    0,   10,   20,     0, 0x1c,  10,  0x80,       3, SC300, SD_KIK_L,   0,  90,  47,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 33 */
	{ 0 },	/* 34 */
	{  16,              2 | NO_VEC,        A_SPECIAL,  K_L | 6,    0,   10,   20,     0, 0x1c,  12,  0x80,       3, SC300, SD_KIK_L,   0,  90,  47,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 35 */
	{   7,                      ZU,        A_SPECIAL,  K_L | 6,    0,   10,   20,     0, 0x1e,   1,  0x80,       3, SC300, SD_NOR_L,   5,  30,  57,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 36 */
	{ 0 },	/* 37 */
	{   8,                      ZU,        A_SPECIAL,  K_L | 6,    0,   10,   20,     0, 0x1e,   1,  0x80,       3, SC300, SD_NOR_L,   5,  30,  57,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 38 */
	{   7,                    BU_O,        A_SPECIAL,  K_L | 6,    0,   10,   20,     0, 0x1e,   8,  0x80,       3, SC300, SD_NOR_L,   5,  30,  58,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 39 */
	{ 0 },	/* 3a */
	{   8,                    BU_O,        A_SPECIAL,  K_L | 6,    0,   10,   20,     0, 0x1e,   8,  0x80,       3, SC300, SD_NOR_L,   5,  30,  58,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 3b */
	{  10,             10 | NO_VEC,        A_SPECIAL,      K_L,    0,   10,   10,     0, 0x1f,  10,  0x60,       3, SC200,       12,   0,  90,  77,    0, NO_CHIP | NO_WEIGHT | AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 3c: pl24_at_t_kick, Partner's wall-leap kick (t_kick) */
	{  12,             AS | NO_VEC, A_LO | A_SPECIAL,      K_S,    0,   10,   20,     9, 0x00,   8,  0x80,       3, SC300, SD_KIK_L,   0,  90,  87,    0,                       0,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 3d */
	{ 0 },	/* 3e */
	{  14,             AS | NO_VEC, A_LO | A_SPECIAL,      K_S,    0,   10,   20,     9, 0x00,   8,  0x80,       3, SC300, SD_KIK_L,   0,  90,  87,    0,                       0,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 3f */
	{   4,             ZU | NO_VEC,          A_HYPER,  K_L | 6,    0,    4,   20,     0, 0x1a,  15,  0x80,       3, SC300, SD_KIK_L,   0,  90,  95,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 40: pl24_at_smasher_1, Spin Drive Smasher hyper: the spinning dash */
	{   6,              2 | NO_VEC,          A_HYPER,  K_L | 6,    0,    4,   20,     0, 0x1f,  11,  0x80,       3, SC300, SD_KIK_L,   0,  90,  96,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 41: pl24_at_smasher_2, Spin Drive Smasher hyper: the rising kick */
	{  18,                       2,          A_HYPER,  K_L | 6,    0,    4,   20,     0, 0x1c,   8,  0x80,       3, SC300, SD_KIK_L,   0,  90,  97,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 42 */
	{  14,              2 | NO_VEC,        A_SPECIAL,  K_L | 6,    0,   10,   20,     0, 0x1c,  10,  0x80,       3, SC300, SD_KIK_L,   0,  90,  47,    0, NO_WEIGHT | AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 43 */
	{ 0 },	/* 44 */
	{  16,              2 | NO_VEC,        A_SPECIAL,  K_L | 6,    0,   10,   20,     0, 0x1c,  12,  0x80,       3, SC300, SD_KIK_L,   0,  90,  47,    0, NO_WEIGHT | AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 45 */
	{  18,           BU_O | NO_VEC,        A_SPECIAL,  K_L | 6,    0,   10,   20,     0, 0x1d,  13,  0x80,       3, SC300, SD_KIK_L,   0,  90,  44,   45,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        5, { 0, 0 } },	/* 46: pl24_at_revenge, Cannon Revenge counter kick, lower level */
	{  14,           BU_O | NO_VEC,        A_SPECIAL,  K_L | 6,    0,   10,   20,     0, 0x1a,  13,  0x80,       3, SC300, SD_KIK_L,   0,  90,  98,  120, NO_WEIGHT | AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        5, { 0, 0 } },	/* 47 */
	{ 0 },	/* 48 */
	{  16,           BU_O | NO_VEC,        A_SPECIAL,  K_L | 6,    0,   10,   20,     0, 0x1a,  13,  0x80,       3, SC300, SD_KIK_L,   0,  90,  98,  120, NO_WEIGHT | AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        5, { 0, 0 } },	/* 49 */
	{   4,              2 | NO_VEC,          A_HYPER,      K_L,    0,    4,   20,     0, 0x1e,   8,  0x80,       3, SC300, SD_KIK_L,   0,  90, 102,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 4a */
	{   4,              2 | NO_VEC,          A_HYPER,      K_L,    0,    4,   20,     0, 0x1e,   8,  0x80,       3, SC300, SD_KIK_L,   0,  90, 104,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 4b */
	{   4,              2 | NO_VEC,          A_HYPER,      K_L,    0,    4,   20,     0, 0x1e,   8,  0x80,       3, SC300, SD_KIK_L,   0,  90, 105,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 4c */
	{   4,              2 | NO_VEC,          A_HYPER,      K_L,    0,    4,   20,     0, 0x1e,   8,  0x80,       3, SC300, SD_KIK_L,   0,  90, 106,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 4d */
	{   4,              2 | NO_VEC,          A_HYPER,      K_L,    0,    4,   20,     0, 0x1e,   8,  0x80,       3, SC300, SD_KIK_L,   0,  90, 107,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 4e: pl24_at_killerbee, Killer Bee Assault hyper: the first dash (also the level entry in the direction table) */
	{   4,              2 | NO_VEC,          A_HYPER,      K_L,    0,    4,   20,     0, 0x1e,   8,  0x80,       3, SC300, SD_KIK_L,   0,  90, 108,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 4f */
	{   4,              2 | NO_VEC,          A_HYPER,      K_L,    0,    4,   20,     0, 0x1e,   8,  0x80,       3, SC300, SD_KIK_L,   0,  90, 109,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 50 */
	{   4,              2 | NO_VEC,          A_HYPER,      K_L,    0,    4,   20,     0, 0x1e,   8,  0x80,       3, SC300, SD_KIK_L,   0,  90, 110,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 51 */
	{   4,              2 | NO_VEC,          A_HYPER,      K_L,    0,    4,   20,     0, 0x1e,   8,  0x80,       3, SC300, SD_KIK_L,   0,  90, 111,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 52 */
	{   4,              2 | NO_VEC,          A_HYPER,      K_L,    0,    4,   20,     0, 0x01,   8,  0x80,       3, SC300, SD_KIK_L,   0,  90, 102,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 53 */
	{   4,              2 | NO_VEC,          A_HYPER,      K_L,    0,    4,   20,     0, 0x01,   8,  0x80,       3, SC300, SD_KIK_L,   0,  90, 104,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 54 */
	{   4,              2 | NO_VEC,          A_HYPER,      K_L,    0,    4,   20,     0, 0x01,   8,  0x80,       3, SC300, SD_KIK_L,   0,  90, 105,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 55 */
	{   4,              2 | NO_VEC,          A_HYPER,      K_L,    0,    4,   20,     0, 0x01,   8,  0x80,       3, SC300, SD_KIK_L,   0,  90, 106,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 56 */
	{   4,              2 | NO_VEC,          A_HYPER,      K_L,    0,    4,   20,     0, 0x01,   8,  0x80,       3, SC300, SD_KIK_L,   0,  90, 107,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 57 */
	{   4,              2 | NO_VEC,          A_HYPER,      K_L,    0,    4,   20,     0, 0x01,   8,  0x80,       3, SC300, SD_KIK_L,   0,  90, 108,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 58 */
	{   4,              2 | NO_VEC,          A_HYPER,      K_L,    0,    4,   20,     0, 0x01,   8,  0x80,       3, SC300, SD_KIK_L,   0,  90, 109,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 59 */
	{   4,              2 | NO_VEC,          A_HYPER,      K_L,    0,    4,   20,     0, 0x01,   8,  0x80,       3, SC300, SD_KIK_L,   0,  90, 110,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 5a */
	{   4,              2 | NO_VEC,          A_HYPER,      K_L,    0,    4,   20,     0, 0x01,   8,  0x80,       3, SC300, SD_KIK_L,   0,  90, 111,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 5b */
	{   4,              2 | NO_VEC,          A_HYPER,      K_L,    0,    4,   20,     0, 0x00,  10,  0x80,       3, SC300, SD_KIK_L,   0,  90, 108,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 5c: pl24_at_killerbee_dive, Killer Bee Assault: a dive at the opponent from the wall (killerbee_dive_init) */
	{   4,              2 | NO_VEC,          A_HYPER,      K_L,    0,    0,   20,     0, 0x00,  10,  0x80,       3, SC300, SD_KIK_L,   0,  90, 108,    0,    NO_WEIGHT | NO_COMBO,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 5d: pl24_at_killerbee_last, Killer Bee Assault: the last hit, which catches (catch_no 0xc6) */
	{  10,             12 | NO_VEC,        A_SPECIAL,      K_L,    0,    4,   24,     0, 0x1c,  12,  0x80,       3, SC300,        8,  64,  90,   9,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 5e: pl24_at_snapback, Snapback (sp_snapback) */
	{  12,           BU_O | NO_VEC,        A_SPECIAL,      K_L,    0,   10,   24,     0, 0x00,  11,  0x80, MARK_N3, SC300, SD_KIK_L,   0,  40, 144,    0, NO_CHIP | NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 5f: pl24_at_upper, Launcher (pl24_upper, Char_Extra 3) */
	{  14,             ZU | NO_VEC,        A_SPECIAL,  K_L | 6,    0,   10,   20,     0, 0x1c,  10,  0x80,       3, SC300, SD_KIK_L,   0,  90,  47,    0, NO_WEIGHT | AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 60: pl24_at_hooligan_kick, Hooligan Combination: the kick down when K is pressed */
	{  12,             10 | NO_VEC,        A_SPECIAL,      K_L,    1,   10,   24,     0, 0x00,   9,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  90,  15,   90,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 61: pl24_at_revenge_spike, Cannon Revenge, higher level: the rising Cannon Spike */
	{   3,              2 | NO_VEC,          A_HYPER, K_LL | 6,    0,    2,   24,     0, 0x00,   8,  0x80,       3, SC300,       12,   0,  90, 124,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 62: pl24_at_shaft_1, Reverse Shaft Breaker hyper: start and first rising hits */
	{   3,           BU_O | NO_VEC,          A_HYPER, K_LL | 6,    0,    2,   24,     0, 0x00,   7,  0x80,       3, SC300,       12,   0,  90, 126,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 63: pl24_at_shaft_2, Reverse Shaft Breaker: the repeated rising hits */
	{  18, 2 | NO_VEC | FLY_SCREEN,          A_HYPER, K_LL | 6,    0,    2,   24,     0, 0x1c,  15,  0x80,       3, SC300,       12,   0,  90, 126,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 64: pl24_at_shaft_3, Reverse Shaft Breaker: the hit at the top */
};
