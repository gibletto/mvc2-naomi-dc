/* Thanos (character 36): the attacks (HIT_DATA, 0x1c bytes).
   What each hit does. The character's module sets the attack's number (atck_no) with the move,
   and the row is read when the attack box meets a body. After a row is its number, in
   hexadecimal, and the program's name for it:
   ATCK_ for the normals every character has (player.h), pl36_at_ for its own (pl36_def.h).
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
     box, box2  boxes of hit_36 that the computer measures the attack's reach by
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

const HIT_DATA hit_dt_36[] = {
	/* dmg                   type            kind     elem  piyo  stop  sand  sleep   vec  pow  range     mark  score        se  shc  jug  box  box2                     flag  13  guard  15  catch  abyss  cnt    answer        1a */
	{   4,           ZU | NO_VEC,          A_NOM,     K_S,    4,   10,    8,     0, 0x18,   4,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  40,   81,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 00: ATCK_STAND_LP */
	{   5,           ZU | NO_VEC,          A_NOM,     K_M,    2,    6,    8,     0, 0x18,   4,  0x70, MARK_N2, SC100, SD_NOR_M,   0,  40,  34,  171,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 01: ATCK_STAND_MP */
	{  13,                  BU_O,          A_NOM, K_L | 3,    1,   15,   16,     0, 0x1e,   9,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  50,  24,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 02: ATCK_STAND_HP */
	{   4,           ZU | NO_VEC,          A_NOM,     K_S,    4,   10,    8,     0, 0x1c,   4,  0x60, MARK_N1, SC100, SD_KIK_S,   0,  20,  48,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 03: ATCK_STAND_LK */
	{   7,                    ZU,          A_NOM,     K_M,    2,   12,   12,     0, 0x1c,   7,  0x80, MARK_N2, SC200, SD_KIK_M,   0,  40, 174,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 04: ATCK_STAND_MK */
	{  13,                    ZU,          A_NOM,     K_M,    1,   15,   16,     0, 0x1a,  10,  0x90, MARK_N3, SC300, SD_KIK_L,   0,  50, 154,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 05: ATCK_STAND_HK */
	{   4,           ZU | NO_VEC,          A_NOM,    K_LS,    4,   10,    8,     0, 0x18,   4,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20, 161,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 06: ATCK_CROUCH_LP */
	{   7,         BU_O | NO_VEC,          A_NOM,    K_LM,    2,   12,   12,     0, 0x00,   8,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40,  57,   59,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 07: ATCK_CROUCH_MP */
	{  13,         BU_O | NO_VEC,          A_NOM,    K_LL,    1,   15,   24,     0, 0x00,   9,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  50, 162,    0, NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 08: ATCK_CROUCH_HP */
	{   4,           ZU | NO_VEC,           A_LO,    K_LS,    4,   10,    8,     0, 0x18,   4,  0x70, MARK_N1, SC100, SD_KIK_S,   0,  20,  67,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 09: ATCK_CROUCH_LK */
	{   7,                    AS,           A_LO,    K_LM,    2,   12,   12,     8, 0x18,   7,  0x80, MARK_N2, SC200, SD_KIK_M,   0,  90,  67,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0a: ATCK_CROUCH_MK */
	{  13,                  BU_O,          A_NOM,    K_LL,    1,   15,   16,     0, 0x1c,  10,  0x70, MARK_N3, SC300, SD_KIK_L,   0,  50, 163,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0b: ATCK_CROUCH_HK */
	{   4,           ZU | NO_VEC,           A_HI,     K_S,    1,   10,    8,     0, 0x00,   7,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  70,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0c: ATCK_JUMP_LP */
	{   7,           ZU | NO_VEC,           A_HI,     K_M,    1,   10,   12,     0, 0x1f,   8,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40,  70,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0d: ATCK_JUMP_MP */
	{  13,           ZU | NO_VEC,           A_HI, K_L | 3,    1,   12,   16,     0, 0x17,  13,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  60,  88,   90, NO_WEIGHT | AERIAL_FINISH,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0e: ATCK_JUMP_HP */
	{   4,           ZU | NO_VEC,           A_HI,     K_S,    1,   10,    8,     0, 0x00,   5,  0x60, MARK_N1, SC100, SD_KIK_S,   0,  20,  75,  173,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0f: ATCK_JUMP_LK */
	{   7,           ZU | NO_VEC,           A_HI,     K_M,    1,   10,   12,     0, 0x00,   5,  0x60, MARK_N2, SC200, SD_KIK_M,   0,  40,  80,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 10: ATCK_JUMP_MK */
	{  13,           ZU | NO_VEC,           A_HI,     K_L,    1,   12,   16,     0, 0x16,  10,  0x80, MARK_N3, SC300, SD_KIK_L,   0,  60,  75,  173, NO_WEIGHT | AERIAL_FINISH,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 11: ATCK_JUMP_HK */
	{  12,           ZU | NO_VEC,           A_HI,     K_L,    1,   14,   16,     0, 0x14,  10,  0x80, MARK_N3, SC300, SD_KIK_L,   0,  60,  76,    0, NO_WEIGHT | AERIAL_FINISH,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 12: pl36_at_jdown_hk, Jumping HK with down, high enough: the two-hit stomp (jump_kick_stomp) */
	{  13,                    ZU,          A_NOM,     K_L,    1,   14,   16,     0, 0x00,  11,  0x80, MARK_N3, SC200, SD_KIK_L,   0,  40,  98,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 13: pl36_at_back_hk, Back + standing HK */
	{ 0 },	/* 14 */
	{ 0 },	/* 15 */
	{ 0 },	/* 16 */
	{ 0 },	/* 17 */
	{ 0 },	/* 18 */
	{   2,           ZU | NO_VEC,          A_NOM,     K_M,    2,   12,    8,     0, 0x18,   4,  0x70, MARK_N2, SC100, SD_NOR_M,   0,  20,  40,   81,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 19: pl36_at_stand_mp_2, Second hit of standing MP (two_hits[1]; nml_move_2hit) */
	{   8,            ZU | NO_ZU,          A_NOM,     K_L,    1,   14,   16,     0, 0x19,  10,  0x80, MARK_N3, SC300, SD_KIK_L,   0,  50, 255,    0,                       0,  0,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 1a */
	{ 0 },	/* 1b */
	{ 0 },	/* 1c */
	{ 0 },	/* 1d */
	{ 0 },	/* 1e */
	{ 0 },	/* 1f */
	{  16,                     2,   A_NOM | 0x40,       0,    0,   16,    0,    15, 0x1c,   4,  0x80, MARK_N2, SC300, SD_NOR_L,   0,   0,   0,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 20: pl36_at_throw_p, Punch throw */
	{  16,                  BU_O,   A_NOM | 0x40,       0,    0,   16,   20,    15, 0x1e,  11,  0x80, MARK_N2, SC300, SD_NOR_L,   0,   0,   0,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 21: pl36_at_throw_k, Kick throw */
	{   6,                    21, A_HYPER | 0x40,       0,    0,   16,    0,    15, 0x1e,   8,  0x80, MARK_N2, SC300, SD_NOR_L,   0,   0,   0,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 22: pl36_at_soul_release, Soul Gem's hand lets go of the drained opponent (shl36_04_hold) */
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
	{  16, BU_O | NO_VEC | NO_ZU,      A_SPECIAL,     K_L,    0,   10,    0,    15, 0x1e,  12,  0x80,       3, SC100, SD_NOR_L,   0,  90,  93,  105, NO_WEIGHT | AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 30 */
	{  18, BU_O | NO_VEC | NO_ZU,      A_SPECIAL,     K_L,    0,   10,    0,    15, 0x1e,  12,  0x80,       3, SC100, SD_NOR_L,   0,  90,  93,  105, NO_WEIGHT | AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 31 */
	{  20, BU_O | NO_VEC | NO_ZU,      A_SPECIAL,     K_L,    0,   10,    0,    15, 0x1e,  12,  0x80,       3, SC100, SD_NOR_L,   0,  90,  93,  105, NO_WEIGHT | AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 32 */
	{  15,   ZU | NO_VEC | NO_ZU,      A_SPECIAL,     K_L,    0,   10,    0,     0, 0x1a,   1,  0x80,       3, SC100, SD_NOR_M,   0,  50, 145,    0,               NO_WEIGHT,  1,     0,  0,     1,     0,   0,        4, { 0, 0 } },	/* 33: pl36_at_sphere, Death Sphere: Thanos and the bubble (shl36_03) */
	{ 0 },	/* 34 */
	{ 0 },	/* 35 */
	{ 0 },	/* 36 */
	{ 0 },	/* 37 */
	{ 0 },	/* 38 */
	{   4,   10 | NO_VEC | NO_ZU,        A_HYPER,     K_L,    0,    4,   15,    15, 0x1a,   9,  0x70,       3, SC100, SD_NOR_L,   0,  90, 135,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   6,        3, { 0, 0 } },	/* 39 */
	{   4,   10 | NO_VEC | NO_ZU,        A_HYPER,     K_L,    0,    4,   15,    15, 0x06,   9,  0x70,       3, SC100, SD_NOR_L,   0,  90, 125,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   6,        3, { 0, 0 } },	/* 3a */
	{   4,   10 | NO_VEC | NO_ZU,        A_HYPER,    K_LM,    0,    6,   10,    15, 0x1e,   8,  0x70,       3, SC100, SD_NOR_L,   0,  90, 135,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   6,        3, { 0, 0 } },	/* 3b */
	{   4,   10 | NO_VEC | NO_ZU,        A_HYPER,    K_LM,    0,    6,   10,    15, 0x02,   8,  0x70,       3, SC100, SD_NOR_L,   0,  90, 124,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   6,        3, { 0, 0 } },	/* 3c */
	{   4, BU_O | NO_VEC | NO_ZU,        A_HYPER,     K_L,    0,   10,   30,    15, 0x00,  14,  0x70,       3, SC100, SD_NOR_L,   0,  90, 133,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   6,        3, { 0, 0 } },	/* 3d: pl36_at_reality, Gauntlet Reality: Thanos and the blasts' burst (shl36_01) */
	{   2,   21 | NO_VEC | NO_ZU,        A_HYPER,    K_LL,    0,    8,   20,    30, 0x03,   1,  0x70,       3, SC100, SD_NOR_L,   0,  90, 121,  123,               NO_WEIGHT,  1,     0,  0,     0,     0,   8,        3, { 0, 0 } },	/* 3e: pl36_at_space, Gauntlet Space (space_init) */
	{   1,   21 | NO_VEC | NO_ZU,        A_HYPER,    K_LM,    0,    8,   20,    30, 0x03,   1,  0x70,       3, SC100, SD_NOR_L,   0,  90, 121,    0,     NO_CHIP | NO_WEIGHT,  1,     0,  0,     0,     0,   8,        3, { 0, 0 } },	/* 3f */
	{   2,   21 | NO_VEC | NO_ZU,        A_HYPER,    K_LM,    0,    8,   20,    48, 0x12,   0,  0x70,       3, SC100, SD_NOR_L,   0,  90, 121,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   8,        3, { 0, 0 } },	/* 40 */
	{   1,   21 | NO_VEC | NO_ZU,        A_HYPER,    K_LM,    0,    8,   20,    48, 0x12,   0,  0x70,       3, SC100, SD_NOR_L,   0,  90, 121,    0,     NO_CHIP | NO_WEIGHT,  1,     0,  0,     0,     0,   8,        3, { 0, 0 } },	/* 41 */
	{   9, BU_O | NO_VEC | NO_ZU,        A_HYPER,       2,    0,   10,    8,     0, 0x1c,  12,  0x70,       3, SC100, SD_NOR_L,   0,  90, 122,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,  20,        3, { 0, 0 } },	/* 42: pl36_at_power, Power Gem: Thanos and each hit of the ball (shl36_00) */
	{   0,   ZU | NO_VEC | NO_ZU,        A_HYPER,     K_L,    0,   10,    8,     0, 0x18,   8,  0x70,       3, SC100, SD_NOR_S,   0,  90, 251,    0,                 NO_CHIP,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 43 */
	{   9,   ZU | NO_VEC | NO_ZU,        A_HYPER,       3,    0,   10,    8,     0, 0x00,   0,  0x70,       3, SC100, SD_NOR_S,   0,  90, 167,    0,               NO_WEIGHT,  1,     0,  0,     1,     0,   0,        3, { 0, 0 } },	/* 44: pl36_at_soul, Soul Gem: Thanos and the grabbing hand (shl36_04) */
	{  10,           12 | NO_VEC,      A_SPECIAL,     K_L,    0,    4,   24,     0, 0x1c,  12,  0x80,       3, SC300,       12,  64,  90, 168,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 45: pl36_at_snapback, Snapback */
	{  10,           10 | NO_VEC,      A_SPECIAL,     K_L,    0,   10,   10,     0, 0x1f,  10,  0x60,       3, SC200,       12,   0,  90,  75,  173, NO_CHIP | NO_WEIGHT | AERIAL_FINISH,  1,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 46: pl36_at_t_kick, Partner jump-in attack */
	{  14,         BU_O | NO_VEC,      A_SPECIAL,    K_LM,    0,   10,   12,     0, 0x00,  11,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40, 169,  170, NO_CHIP | NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 47: pl36_at_upper, Launcher (pl36_upper) */
	{   8,         BU_O | NO_VEC,      A_SPECIAL,    K_LM,    0,   10,   12,     0, 0x00,  11,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  90, 169,  170,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 48: pl36_at_upper_assist, sp_upper, the uppercut assist (crouching, Char_Extra 5) */
};
