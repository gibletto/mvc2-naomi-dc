/* Wolverine with bone claws (character 39): the attacks (HIT_DATA, 0x1c bytes).
   What each hit does. The character's module sets the attack's number (atck_no) with the move,
   and the row is read when the attack box meets a body. After a row is its number, in
   hexadecimal, and the program's name for it:
   ATCK_ for the normals every character has (player.h), pl39_at_ for its own (pl39_def.h).
   The values' names are Capcom's own for these tables (hit_equ.h) and the program's (player.h).

     dmg     the damage
     type    the reaction: ZU pushed back, BU_O knocked off the feet, AS swept,
             or the number of another in pl_dmg.c's dm_tbl (here 2 dm_buttobi_ng, 5 dm_hold,
             10 dm_away, 11 dm_bound, 12 dm_snap).
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
     box, box2  boxes of hit_39 that the computer measures the attack's reach by
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

const HIT_DATA hit_dt_39[] = {
	/* dmg                   type              kind              elem  piyo  stop  sand  sleep   vec  pow  range     mark  score  se  shc  jug  box  box2                     flag  13  guard  15  catch  abyss  cnt    answer        1a */
	{   4,           ZU | NO_VEC,            A_NOM,              K_S,    4,   10,   11,     0, 0x18,   8,  0x70, MARK_N1, SC100, 13,   0,  20,  31,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 00: ATCK_STAND_LP */
	{   6,           ZU | NO_VEC,            A_NOM,             K_LM,    2,    8,   18,     0, 0x1a,   9,  0x80, MARK_N2, SC200, 14,   0,  40,  16,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 01: ATCK_STAND_MP */
	{  15,                  BU_O,            A_NOM,              K_L,    1,   12,   24,     0, 0x1a,  12,  0x80, MARK_N3, SC300, 15,   0,  50,  70,  112,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 02: ATCK_STAND_HP */
	{   4,           ZU | NO_VEC,            A_NOM,             K_LS,    4,   10,   11,     0, 0x19,   6,  0x60, MARK_N1, SC100,  1,   0,  20,  26,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 03: ATCK_STAND_LK */
	{   6,           ZU | NO_VEC,            A_NOM,             K_LM,    2,    8,   18,     0, 0x1a,   7,  0x60, MARK_N2, SC200,  2,   0,  40,  27,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 04: ATCK_STAND_MK */
	{  14, BU_O | NO_VEC | NO_ZU,            A_NOM,              K_L,    1,   12,   24,     0, 0x1f,  11,  0x80, MARK_N3, SC300,  3,   0,  50,  39,  146, NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 05: ATCK_STAND_HK */
	{   4,           ZU | NO_VEC,            A_NOM,             K_LS,    4,   10,   11,     0, 0x18,   8,  0x70, MARK_N1, SC100, 13,   0,  20,  41,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 06: ATCK_CROUCH_LP */
	{   6,                  BU_O,            A_NOM,             K_LM,    2,   11,   18,     0, 0x01,   4,  0x80, MARK_N2, SC200, 15,   0,  40,  43,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 07: ATCK_CROUCH_MP */
	{  15,                    ZU,            A_NOM,             0xc0,    1,   12,   24,     0, 0x1c,  10,  0x80, MARK_N3, SC300, 15,   0,  50,  46,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 08: ATCK_CROUCH_HP */
	{   4,           ZU | NO_VEC,             A_LO,             K_LS,    4,   10,   11,     0, 0x18,   8,  0x70, MARK_N1, SC100,  1,   0,  20,  52,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 09: ATCK_CROUCH_LK */
	{   7,                    ZU,             A_LO,             K_LM,    2,   11,   18,     0, 0x1d,   7,  0x40, MARK_N2, SC200,  2,   0,  50,  54,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0a: ATCK_CROUCH_MK */
	{  15,                  BU_O,            A_NOM,             K_LL,    1,   12,   24,     0, 0x1c,  10,  0x70, MARK_N3, SC300,  3,   0,  50,  60,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0b: ATCK_CROUCH_HK */
	{   3,           ZU | NO_VEC,             A_HI,              K_S,    1,   10,   10,     0, 0x00,   2,  0x70, MARK_N1, SC100, 13,   0,  20,  62,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0c: ATCK_JUMP_LP */
	{   6,           ZU | NO_VEC,             A_HI,              K_M,    1,   10,   16,     0, 0x00,   8,  0x80, MARK_N2, SC200, 14,   0,  40,  65,  145,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0d: ATCK_JUMP_MP */
	{  12,           ZU | NO_VEC,             A_HI,              K_M,    1,   12,   32,     0, 0x16,   8,  0x80, MARK_N3, SC300, 15,   0,  60,  72,  147, NO_WEIGHT | AERIAL_FINISH,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0e: ATCK_JUMP_HP */
	{   4,           ZU | NO_VEC,             A_HI,              K_S,    1,   10,   10,     0, 0x1f,   2,  0x60, MARK_N1, SC100,  1,   0,  20,  74,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0f: ATCK_JUMP_LK */
	{   6,           ZU | NO_VEC,             A_HI,              K_M,    1,   10,   16,     0, 0x00,   5,  0x60, MARK_N2, SC200,  2,   0,  40,  76,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 10: ATCK_JUMP_MK */
	{  12,           ZU | NO_VEC,             A_HI,              K_M,    1,   12,   32,     0, 0x17,   9,  0x80, MARK_N3, SC300,  3,   0,  90,  78,   79, NO_WEIGHT | AERIAL_FINISH,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 11: ATCK_JUMP_HK */
	{  14,                    AS,             A_LO,              K_L,    1,   12,   24,     8, 0x14,   3,  0x80, MARK_N3, SC300, 15,   0,  90,  49,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 12 */
	{  13,           ZU | NO_VEC,             A_HI,             K_LL,    1,   12,   24,     0, 0x14,  10,  0x80, MARK_N2, SC300,  3,   0,  60, 176,    0,                       0,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 13 */
	{  14, BU_O | NO_VEC | NO_ZU,        A_SPECIAL,              K_L,    0,   12,   24,     0, 0x1f,  11,  0x80, MARK_N3, SC300,  3,   0,  40,  39,  146, NO_CHIP | NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 14: pl39_at_upper, Launcher (pl39_upper) */
	{ 0 },	/* 15 */
	{ 0 },	/* 16 */
	{ 0 },	/* 17 */
	{ 0 },	/* 18 */
	{   3,           ZU | NO_VEC,            A_NOM, K_S | K_BACK_MAX,    4,   10,   10,     0, 0x18,  15,  0x70, MARK_N1, SC100, 13,   0,  20,  31,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 19 */
	{   3,           ZU | NO_VEC,            A_NOM, K_S | K_BACK_MAX,    4,   10,   10,     0, 0x18,  15,  0x70, MARK_N1, SC100, 13,   0,  20,  41,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 1a */
	{   5,                    ZU,            A_NOM,              K_M,    2,   11,   16,     0, 0x1a,   4,  0x60, MARK_N2, SC200,  2,   3,  40,  35,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 1b: pl39_at_stand_mk_2, Standing MK's follow-up hit, button held (stand_kick_move) */
	{   4,                    ZU,            A_NOM,              K_M,    2,   11,   16,     0, 0x1a,   7,  0x80, MARK_N2, SC200, 14,   3,  40,  17,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 1c: pl39_at_stand_mp_2, Standing MP's chained second hit (cg_type 1) */
	{   4,                    ZU,             A_HI,              K_S,    1,   10,   16,     0, 0x00,   2,  0x80, MARK_N2, SC200, 14,   2,  40, 145,   65,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 1d: pl39_at_sjump_mp_2, Super jump MP's chained second hit */
	{ 0 },	/* 1e */
	{ 0 },	/* 1f */
	{   6,            2 | NO_VEC,     A_NOM | 0x40,              K_L,    4,   16,    0,    15, 0x06,   8,  0xc0, MARK_N2, SC300, 13,   4,   0,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 20: pl39_at_throw_p, Punch throw's toss (nml_p_throw) */
	{   3,                  BU_O,     A_NOM | 0x40,                0,    0,    3,   20,     0, 0x10,   2,  0x80, MARK_N2, SC300, 13,   1,   0,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 21: pl39_at_throw_p_hold, Punch throw's hold (nml_p_hold_hit) */
	{   6,    2 | NO_VEC | NO_ZU,     A_NOM | 0x40,              K_L,    4,   16,    0,    15, 0x1a,   6,  0xc0, MARK_N2, SC300, 13,   4,   0,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 22: pl39_at_throw_k, Kick throw: jumping up with the opponent (nml_k_jump) */
	{   2,                  BU_O,     A_NOM | 0x40,                0,    4,   16,   30,     0, 0x00,   2,  0x80, MARK_N2, SC300, 13,   1,   0,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 23: pl39_at_throw_k_slam, Kick throw: slamming him down (nml_k_slam) */
	{   2,                  BU_O,     A_NOM | 0x40,                0,    0,    3,   30,     0, 0x00,   2,  0x80, MARK_N2, SC300, 13,   1,   0,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 24: pl39_at_throw_k_hold, Kick throw's hold (nml_k_hold_hit) */
	{   6,            2 | NO_VEC,     A_NOM | 0x40,              K_L,    4,   16,    0,    15, 0x14,   2,  0xc0, MARK_N2, SC300, 13,   4,   0,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 25: pl39_at_air_throw, Air throw: leaping up with the opponent (jump_p_leap_init) */
	{   2,                  BU_O,     A_NOM | 0x40,                0,    0,    3,   30,     0, 0x00,   2,  0x80, MARK_N2, SC300, 13,   1,   0,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 26: pl39_at_air_throw_hold, Air throw's hold (jump_p_hold_hit) */
	{ 0 },	/* 27 */
	{ 0 },	/* 28 */
	{ 0 },	/* 29 */
	{ 0 },	/* 2a */
	{ 0 },	/* 2b */
	{ 0 },	/* 2c */
	{ 0 },	/* 2d */
	{ 0 },	/* 2e */
	{ 0 },	/* 2f */
	{   0,   ZU | NO_VEC | NO_ZU,          A_HYPER,              K_S,    0,   10,    0,     0, 0x00,   0,  0x70,       3, SC100,  1,   0,  90, 109,    0,                 NO_CHIP,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 30: pl39_at_weapon_x, Weapon X (sp_weapon_x) */
	{   5,             5 | NO_ZU,          A_HYPER,              K_L,    0,    1,    0,     0, 0x18,   4,  0x70,       3, SC100, 13,   0,  90, 134,    0,                   NO_KO,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 31 */
	{   4,             5 | NO_ZU,          A_HYPER,             K_LL,    0,    1,    0,     0, 0x1a,   7,  0x80,       3, SC200, 14,   0,  90,  16,   17,                   NO_KO,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 32 */
	{   3,             5 | NO_ZU,          A_HYPER,              K_L,    0,    1,    0,     0, 0x19,   4,  0x60,       3, SC100,  1,   0,  90,  26,    0,                   NO_KO,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 33 */
	{   2,             5 | NO_ZU,          A_HYPER,             K_LL,    0,    1,    0,     0, 0x18,   4,  0x70,       3, SC100, 13,   0,  90,  62,    0,                   NO_KO,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 34 */
	{   4,             5 | NO_ZU,          A_HYPER,              K_L,    0,    1,    0,     0, 0x1a,   4,  0x60,       3, SC100,  1,   0,  90,  74,    0,                   NO_KO,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 35 */
	{   3,             5 | NO_ZU,          A_HYPER,             K_LL,    0,    1,    0,     0, 0x18,   7,  0x80,       3, SC200, 14,   0,  70,  65,    0,                   NO_KO,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 36 */
	{   4,             5 | NO_ZU,          A_HYPER,              K_L,    0,    1,    0,     0, 0x00,   7,  0x80,       3, SC200, 14,   0,  90, 148,    0,                   NO_KO,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 37 */
	{   3,             5 | NO_ZU,          A_HYPER,             K_LL,    0,    1,    0,     0, 0x00,   7,  0x80,       3, SC200,  2,   0,  90, 149,    0,                   NO_KO,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 38 */
	{   6,            11 | NO_ZU,          A_HYPER,              K_L,    0,    4,    0,     0, 0x10,   7,  0x80,       3, SC300, 15,   0,  90,  72,    0,       NO_WEIGHT | NO_KO,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 39 */
	{  22,                     2,          A_HYPER,              K_L,    0,   50,    0,     0, 0x00,   8,  0x70,    0xff, SC300, 16,  10,  90, 150,    0,    NO_WEIGHT | NO_SCALE,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 3a */
	{   8, BU_O | NO_VEC | NO_ZU, A_SPECIAL | 0x80,              K_M,    0,   10,   16,     0, 0x1a,   4,  0x70,       9, SC300, 15,  10,  90, 128,    0,     NO_CHIP | NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 3b: pl39_at_berserker, Berserker Barrage (sp_berserker) */
	{   3, BU_O | NO_VEC | NO_ZU, A_SPECIAL | 0x80,              K_S,    0,   10,   16,     0, 0x1a,   8,  0x70,      10, SC300, 15,   2,  90, 128,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 3c */
	{   8, BU_O | NO_VEC | NO_ZU, A_SPECIAL | 0x80,             K_LL,    0,   10,   16,     0, 0x1a,   4,  0x70,       9, SC300, 15,  10,  90, 128,    0,     NO_CHIP | NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 3d */
	{   3, BU_O | NO_VEC | NO_ZU, A_SPECIAL | 0x80,              K_L,    0,   10,   16,     0, 0x1a,   8,  0x70,      10, SC300, 15,   2,  90, 128,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 3e */
	{   8, BU_O | NO_VEC | NO_ZU, A_SPECIAL | 0x80,              K_M,    0,   10,   16,     0, 0x1c,   4,  0x70,       9, SC300, 15,  10,  90, 128,    0,     NO_CHIP | NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 3f */
	{   3, BU_O | NO_VEC | NO_ZU, A_SPECIAL | 0x80,              K_S,    0,   10,   16,     0, 0x1c,   8,  0x70,      10, SC300, 15,   2,  90, 128,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 40 */
	{   3,   ZU | NO_VEC | NO_ZU,   A_HYPER | 0x80,             K_LL,    0,   10,    0,     0, 0x1c,   0,  0x70,       9, SC300, 15,  10,  90, 128,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   4,        3, { 0, 0 } },	/* 41: pl39_at_berserker_x, Berserker Barrage X (sp_berserker_x) */
	{   3,   ZU | NO_VEC | NO_ZU,          A_HYPER,              K_L,    0,   10,    0,     0, 0x1c,   0,  0x70,      10, SC300, 15,   2,  90, 128,    0,     NO_CHIP | NO_WEIGHT,  1,     0,  0,     0,     0,   4,        3, { 0, 0 } },	/* 42 */
	{  16, 2 | NO_VEC | NO_ZU | FLY_SCREEN,   A_HYPER | 0x80,              K_L,    0,   10,    0,     0, 0x1b,  15,  0x70,      11, SC300,  3,   2,  90, 144,    0,    NO_WEIGHT | NO_SCALE,  1,     0,  0,     0,     0,   4,        3, { 0, 0 } },	/* 43 */
	{   4,    2 | NO_VEC | NO_ZU,          A_HYPER,              K_L,    0,    6,   10,     0, 0x00,   0,  0x60,       3, SC200, 15,   0,  90, 163,    0,                       0,  1,     0,  0,     0,     0,  13,        3, { 0, 0 } },	/* 44: pl39_at_fatal, Fatal Claw (sp_fatal) */
	{   4,    2 | NO_VEC | NO_ZU,          A_HYPER,              K_L,    0,    6,   10,     0, 0x00,   0,  0x60,       3, SC200, 15,   0,  90, 168,    0,                       0,  1,     0,  0,     0,     0,  13,        3, { 0, 0 } },	/* 45 */
	{   8,    2 | NO_VEC | NO_ZU,          A_HYPER,              K_L,    0,    6,   10,     0, 0x08,   4,  0x60,       3, SC200, 15,   0,  90, 166,    0,                       0,  1,     0,  0,     0,     0,  13,        3, { 0, 0 } },	/* 46 */
	{   8,    2 | NO_VEC | NO_ZU,          A_HYPER,              K_L,    0,    6,   10,     0, 0x03,   8,  0x60,       3, SC200, 15,   0,  90, 167,    0,                       0,  1,     0,  0,     0,     0,  13,        3, { 0, 0 } },	/* 47 */
	{   6,    2 | NO_VEC | NO_ZU,          A_HYPER,              K_L,    0,    6,   10,     0, 0x08,   0,  0x60,       3, SC200, 15,   0,  90, 164,    0,                       0,  1,     0,  0,     0,     0,  13,        3, { 0, 0 } },	/* 48 */
	{   6,    2 | NO_VEC | NO_ZU,          A_HYPER,              K_L,    0,    6,   10,     0, 0x04,   6,  0x60,       3, SC200, 15,   0,  90, 165,    0,                       0,  1,     0,  0,     0,     0,  13,        3, { 0, 0 } },	/* 49 */
	{  10,           10 | NO_VEC,        A_SPECIAL,              K_L,    0,   10,   10,     0, 0x1f,  10,  0x60,       3, SC200,  4,   0,  90, 157,    0, NO_CHIP | AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 4a: pl39_at_t_kick, Partner jump-in attack */
	{   2,         BU_O | NO_VEC, A_SPECIAL | 0x80,              K_S,    0,    6,   20,     0, 0x00,   6,  0x80,       3, SC300, 15,  10,  90,  94,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        7, { 0, 0 } },	/* 4b: pl39_at_tornado_last, Tornado Claw's last hit (tornado_up, tornado_keep) */
	{  12,         BU_O | NO_VEC, A_SPECIAL | 0x80,              K_S,    0,    6,   20,     0, 0x00,   6,  0x80,       3, SC300, 15,  10,  90,  83,   85,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        7, { 0, 0 } },	/* 4c */
	{   3,         BU_O | NO_VEC, A_SPECIAL | 0x80,              K_S,    0,    6,   20,     0, 0x1f,   8,  0x80,       3, SC300, 15,   1,  90,  86,   88,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        7, { 0, 0 } },	/* 4d */
	{  16,         BU_O | NO_VEC, A_SPECIAL | 0x80,              K_S,    0,    6,   20,     0, 0x00,   6,  0x80,       3, SC300, 15,  10,  90,  83,   85,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        7, { 0, 0 } },	/* 4e */
	{   3,         BU_O | NO_VEC, A_SPECIAL | 0x80,              K_S,    0,    6,   20,     0, 0x1e,   8,  0x80,       3, SC300, 15,   1,  90,  86,   88,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        7, { 0, 0 } },	/* 4f */
	{  18,         BU_O | NO_VEC, A_SPECIAL | 0x80,              K_S,    0,    6,   20,     0, 0x00,   6,  0x80,       3, SC300, 15,  10,  90,  83,   85,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        7, { 0, 0 } },	/* 50 */
	{   3,         BU_O | NO_VEC, A_SPECIAL | 0x80,              K_S,    0,    6,   20,     0, 0x1d,   8,  0x80,       3, SC300, 15,   1,  90,  86,   88,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        7, { 0, 0 } },	/* 51 */
	{  16,         BU_O | NO_VEC, A_SPECIAL | 0x80,              K_M,    0,   10,   16,     0, 0x1e,   8,  0x80,       3, SC300, 15,   0,  90,  97,    0, NO_WEIGHT | AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 52 */
	{  16,         BU_O | NO_VEC, A_SPECIAL | 0x80,              K_M,    0,   10,   16,     0, 0x1e,   8,  0x80,       3, SC300, 15,   0,  90,  99,    0, NO_WEIGHT | AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 53 */
	{  16,         BU_O | NO_VEC, A_SPECIAL | 0x80,              K_M,    0,   10,   16,     0, 0x1e,   8,  0x80,       3, SC300, 15,   0,  90, 101,    0, NO_WEIGHT | AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 54 */
	{  16,         BU_O | NO_VEC, A_SPECIAL | 0x80,              K_M,    0,   10,   16,     0, 0x1e,   8,  0x80,       3, SC300, 15,   0,  90, 103,    0, NO_WEIGHT | AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 55 */
	{  16,         BU_O | NO_VEC, A_SPECIAL | 0x80,              K_M,    0,   10,   16,     0, 0x1e,   8,  0x80,       3, SC300, 15,   0,  90, 105,    0, NO_WEIGHT | AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 56 */
	{  16,         BU_O | NO_VEC, A_SPECIAL | 0x80,              K_M,    0,   10,   16,     0, 0x1e,   8,  0x80,       3, SC300, 15,   0,  90, 103,    0, NO_WEIGHT | AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 57 */
	{  16,         BU_O | NO_VEC, A_SPECIAL | 0x80,              K_M,    0,   10,   16,     0, 0x1e,   8,  0x80,       3, SC300, 15,   0,  90, 101,    0, NO_WEIGHT | AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 58 */
	{  16,         BU_O | NO_VEC, A_SPECIAL | 0x80,              K_M,    0,   10,   16,     0, 0x1e,   8,  0x80,       3, SC300, 15,   0,  90,  99,    0, NO_WEIGHT | AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 59 */
	{  18,          BU_O | NO_ZU,        A_SPECIAL,              K_L,    0,   11,   24,     0, 0x1a,  12,  0x80,      10, SC300, 15,   0,  90, 169,  170,                       0,  0,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 5a */
	{  18,          BU_O | NO_ZU,        A_SPECIAL,              K_L,    0,   11,   24,     0, 0x1a,  12,  0x80,      10, SC300, 15,   0,  90, 169,  170,                       0,  0,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 5b */
	{  18,          BU_O | NO_ZU,        A_SPECIAL,              K_L,    0,   11,   24,     0, 0x1a,  12,  0x80,      10, SC300, 15,   0,  90, 169,  170,                       0,  0,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 5c */
	{  14,          BU_O | NO_ZU,        A_SPECIAL,              K_L,    1,   10,   24,     0, 0x1a,  12,  0x80,      10, SC300, 15,   0,  50,  70,  112,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 5d */
	{  14,          BU_O | NO_ZU,        A_SPECIAL,              K_L,    1,   10,   24,     0, 0x1a,  12,  0x80,      10, SC300, 15,   0,  50,  70,  112,                       0,  0,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 5e */
	{  14,          BU_O | NO_ZU,        A_SPECIAL,              K_L,    1,   10,   24,     0, 0x1a,  12,  0x80,      10, SC300, 15,   0,  50,  70,  112,                       0,  0,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 5f */
	{   6,         BU_O | NO_VEC,          A_HYPER,              K_L,    1,   10,   24,     0, 0x1a,  12,  0x80,      10, SC300, 15,   0,  90, 171,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 60: pl39_at_charge, Berserker Charge (sp_charge) */
	{   7, BU_O | NO_VEC | NO_ZU, A_SPECIAL | 0x80,              K_L,    0,    8,   10,     0, 0x1c,  12,  0x80,      10, SC300, 15,   0,  50, 128,    0,                       0,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 61 */
	{   5,    2 | NO_VEC | NO_ZU,   A_HYPER | 0x80,              K_L,    0,    8,   10,     0, 0x1c,  12,  0x80,      10, SC300, 15,   0,  50, 128,    0,                       0,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 62 */
	{  10,           12 | NO_VEC,        A_SPECIAL,              K_L,    0,   12,   24,     0, 0x1c,  12,  0x80,       3, SC300, 16,  64,  90,  46,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 63: pl39_at_snapback, Snapback (sp_snapback) */
	{  20,                  BU_O,        A_SPECIAL,              K_L,    1,   11,   24,     0, 0x1a,  12,  0x80,       3, SC300, 15,   0,  50,  70,  112,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 64: pl39_at_assist_hp, Standing HP as the alpha assist (sp_stand_hp) */
	{  20, BU_O | NO_VEC | NO_ZU,            A_NOM,              K_L,    1,   10,   24,     0, 0x1f,  11,  0x80, MARK_N3, SC300,  3,   0,  50,  39,  146,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 65: pl39_at_assist_hk, Standing HK as the beta assist (sp_stand_hk) */
	{  20,                    AS, A_LO | A_SPECIAL,              K_L,    1,   11,   24,     8, 0x14,   3,  0x80,       3, SC300, 15,   0, 100,  49,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 66: pl39_at_slide, Sliding Claw, the gamma assist (sp_slide) */
};
