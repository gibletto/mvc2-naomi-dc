/* Sakura (character 22): the attacks (HIT_DATA, 0x1c bytes).
   What each hit does. The character's module sets the attack's number (atck_no) with the move,
   and the row is read when the attack box meets a body. After a row is its number, in
   hexadecimal, and the program's name for it:
   ATCK_ for the normals every character has (player.h), pl22_at_ for its own (pl22_def.h).
   The values' names are Capcom's own for these tables (hit_equ.h) and the program's (player.h).

     dmg     the damage
     type    the reaction: ZU pushed back, BU_O knocked off the feet, AS swept,
             or the number of another in pl_dmg.c's dm_tbl (here 2 dm_buttobi_ng, 5 dm_hold,
             6 dm_hold, 10 dm_away, 12 dm_snap, 16 dm_dizzy).
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
     box, box2  boxes of hit_22 that the computer measures the attack's reach by
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

const HIT_DATA hit_dt_22[] = {
	/* dmg                     type          kind  elem  piyo  stop  sand  sleep   vec  pow  range     mark  score        se  shc  jug  box  box2                     flag  13  guard  15  catch  abyss  cnt    answer        1a */
	{   4,                      ZU,        A_NOM,  K_S,    4,   10,   10,     0, 0x18,   4,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  15,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 00: ATCK_STAND_LP */
	{   7,                      ZU,        A_NOM,  K_M,    2,   10,   16,     0, 0x1d,   6,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40,  16,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 01: ATCK_STAND_MP */
	{  13,                      ZU,        A_NOM,  K_L,    1,   10,   24,     0, 0x1f,  10,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  50,  17,   18,                       0,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 02: ATCK_STAND_HP */
	{   4,                      ZU,        A_NOM, K_LS,    4,   10,   10,     0, 0x18,   2,  0x60, MARK_N1, SC100, SD_KIK_S,   0,  20,  19,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 03: ATCK_STAND_LK */
	{   7,           BU_O | NO_VEC,        A_NOM,  K_M,    2,   10,   16,     0, 0x1f,  11,  0x60, MARK_N2, SC200, SD_KIK_M,   0,  40,  20,   54, NO_WEIGHT | BLACK_COMBO,  0,    16,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 04: ATCK_STAND_MK */
	{  13,                      ZU,        A_NOM,  K_L,    1,   10,   24,     0, 0x1a,  11,  0x80, MARK_N3, SC300, SD_KIK_L,   0,  50,  21,  100,                       0,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 05: ATCK_STAND_HK */
	{   4,                      ZU,        A_NOM, K_LS,    4,   10,   10,     0, 0x18,   6,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  22,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 06: ATCK_CROUCH_LP */
	{   7,                      ZU,        A_NOM, K_LM,    2,   10,   16,     0, 0x18,   8,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40,  23,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 07: ATCK_CROUCH_MP */
	{  13,           BU_O | NO_VEC,        A_NOM, K_LL,    1,   10,   24,     0, 0x1f,  11,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  50,  25,    0, NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 08: ATCK_CROUCH_HP */
	{   4,                      ZU,         A_LO, K_LS,    4,   10,   10,     0, 0x18,   8,  0x70, MARK_N1, SC100, SD_KIK_S,   0,  20,  26,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 09: ATCK_CROUCH_LK */
	{   7,                      ZU,         A_LO, K_LM,    2,   10,   16,     0, 0x18,   8,  0x40, MARK_N2, SC200, SD_KIK_M,   0,  40,  27,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0a: ATCK_CROUCH_MK */
	{  13,              AS | NO_ZU,         A_LO, K_LL,    1,   10,   24,     9, 0x18,   8,  0x70, MARK_N3, SC300, SD_KIK_L,   0,  90,  28,    0,                       0,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 0b: ATCK_CROUCH_HK */
	{   4,             ZU | NO_VEC,         A_HI,  K_S,    1,   10,   10,     0, 0x00,   6,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  29,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0c: ATCK_JUMP_LP */
	{   7,             ZU | NO_VEC,         A_HI,  K_M,    1,   10,   16,     0, 0x00,   4,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40,  30,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0d: ATCK_JUMP_MP */
	{  13,             ZU | NO_VEC,         A_HI,  K_M,    1,   10,   24,     0, 0x17,   8,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  60,  31,    0, NO_WEIGHT | AERIAL_FINISH,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 0e: ATCK_JUMP_HP */
	{   4,             ZU | NO_VEC,         A_HI,  K_S,    1,   10,   10,     0, 0x00,   6,  0x60, MARK_N1, SC100, SD_KIK_S,   0,  20,  32,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0f: ATCK_JUMP_LK */
	{   7,             ZU | NO_VEC,         A_HI,  K_M,    1,   10,   16,     0, 0x00,   4,  0x60, MARK_N2, SC200, SD_KIK_M,   0,  40,  33,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 10: ATCK_JUMP_MK */
	{  13,             ZU | NO_VEC,         A_HI,  K_M,    1,   10,   24,     0, 0x17,   8,  0x80, MARK_N3, SC300, SD_KIK_L,   0,  60,  34,    0,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 11: ATCK_JUMP_HK */
	{   7,                      ZU,         A_HI,  K_M,    2,   10,   16,     0, 0x1a,   8,  0x80, MARK_N2, SC300, SD_KIK_M,   0,  40,  35,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 12: pl22_at_fwd_hk, Forward + HK standing */
	{   6,                      ZU,         A_HI,  K_M,    2,   10,   16,     0, 0x1a,   6,  0x60, MARK_N2, SC200, SD_KIK_M,   0,  40,  36,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 13 */
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
	{   2,                       2, A_NOM | 0x40,    0,    4,    4,    0,    24, 0x1c,   4,  0x80, MARK_N1, SC300,       76,   0,   0,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 20: pl22_at_throw_p_hit, Punch throw, the hits on the held opponent */
	{   8,                       2, A_NOM | 0x40,    0,    4,    4,    0,    24, 0x00,   0,  0x80, MARK_N1, SC300,       76,   0,   0,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 21: pl22_at_throw_p, Punch throw, the final throw */
	{   7,                       2, A_NOM | 0x40,    0,    4,    4,    0,    24, 0x1c,   4,  0x80, MARK_N1, SC300,       76,   0,   0,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 22: pl22_at_throw_k_1, Kick throw, first record set on the opponent */
	{  16,                       2, A_NOM | 0x40,    0,    4,    4,    0,    24, 0x1c,   4,  0x80, MARK_N1, SC300,       76,   0,   0,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 23: pl22_at_throw_k, Kick throw, the throw itself (catch_type 1) */
	{  70,                       2,      A_HYPER, K_LM,    4,   12,   20,     0, 0x06,   8,  0x60, MARK_N2, SC200, SD_NOR_M,   0,  40,  35,   36,                NO_SCALE,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 24: pl22_at_shungoku, Shun Goku Satsu: the opponent's record when the flurry ends */
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
	{   3,     ZU | NO_VEC | NO_ZU,    A_SPECIAL,  K_M,    0,    4,   30,     0, 0x1d,   2,  0x80,       3, SC300, SD_NOR_L,   0,  90,  40,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 30 */
	{   3,   BU_O | NO_VEC | NO_ZU,    A_SPECIAL,  K_M,    0,    4,   30,     0, 0x1d,   2,  0x80,       3, SC300, SD_NOR_L,   0,  90,  41,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 31 */
	{   3,   BU_O | NO_VEC | NO_ZU,    A_SPECIAL,  K_M,    0,    4,   30,     0, 0x1d,   2,  0x80,       3, SC300, SD_NOR_L,   0,  90,  42,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 32 */
	{   7,   BU_O | NO_VEC | NO_ZU,    A_SPECIAL,  K_L,    0,    6,   30,     0, 0x1e,  12,  0x80,       3, SC300, SD_NOR_L,   0,  90,  42,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 33 */
	{ 0 },	/* 34 */
	{ 0 },	/* 35 */
	{ 0 },	/* 36 */
	{ 0 },	/* 37 */
	{   4,     ZU | NO_VEC | NO_ZU,    A_SPECIAL,  K_M,    0,    4,   30,     0, 0x1d,   2,  0x80,       3, SC300, SD_NOR_L,   0,  90,  40,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 38 */
	{   4,   BU_O | NO_VEC | NO_ZU,    A_SPECIAL,  K_M,    0,    4,   30,     0, 0x1d,   2,  0x80,       3, SC300, SD_NOR_L,   0,  90,  41,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 39 */
	{   4,   BU_O | NO_VEC | NO_ZU,    A_SPECIAL,  K_M,    0,    4,   30,     0, 0x1d,   2,  0x80,       3, SC300, SD_NOR_L,   0,  90,  42,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 3a */
	{   7,   BU_O | NO_VEC | NO_ZU,    A_SPECIAL,  K_L,    0,    4,   30,     0, 0x1d,  14,  0x80,       3, SC300, SD_NOR_L,   0,  90,  42,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 3b */
	{   1,           BU_O | NO_VEC,    A_SPECIAL,  K_L,    0,    4,   30,     0, 0x1c,  12,  0x80,       3, SC300, SD_NOR_L,   0,  90,  43,    0, NO_CHIP | NO_WEIGHT | NO_KO,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 3c: pl22_at_chouhatsu, the taunt, which has a hit (sp_chouhatsu) */
	{  10,             10 | NO_VEC,    A_SPECIAL,  K_L,    0,   10,   10,     0, 0x1f,  10,  0x60,       3, SC200,       12,   0,  90,  33,    0, NO_CHIP | NO_WEIGHT | AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        7, { 0, 0 } },	/* 3d: pl22_at_t_kick, the partner's jumping kick as she comes in */
	{   1,             ZU | NO_VEC,    A_SPECIAL,  K_S,    0,    8,   30,     0, 0x00,   0,  0x80,       3, SC300, SD_NOR_L,   0,  90, 113,    0,                   NO_KO,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 3e */
	{ 0 },	/* 3f */
	{   1,             ZU | NO_VEC,    A_SPECIAL,  K_S,    0,    8,   30,     0, 0x00,   0,  0x80,       3, SC300, SD_NOR_L,   0,  90, 113,    0,                   NO_KO,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 40 */
	{  10,              2 | NO_VEC,        A_NOM, K_LL,    0,   10,   24,     0, 0x1c,  12,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  50,  22,   23,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 41 */
	{  13,             ZU | NO_VEC,        A_NOM, K_LL,    0,   10,   24,     0, 0x1b,  14,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  50,  22,   23,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 42 */
	{  13,      2 | NO_VEC | NO_ZU,        A_NOM, K_LL,    0,   10,   24,     0, 0x1e,   8,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  50,  45,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 43: pl22_at_win_ball, a win pose's falling, bouncing effect (shl22_00 types 2 and 3) */
	{  14,   BU_O | NO_VEC | NO_ZU,    A_SPECIAL,  K_M,    0,   10,   24,     0, 0x1c,   8,  0x80,       3, SC300, SD_NOR_L,   0,  50,  46,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 44 */
	{ 0 },	/* 45 */
	{  14,   BU_O | NO_VEC | NO_ZU,    A_SPECIAL,  K_M,    0,   10,   24,     0, 0x1c,  12,  0x80,       3, SC300, SD_NOR_L,   0,  50,  46,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 46 */
	{   4,               6 | NO_ZU,      A_HYPER,  K_M,    0,    2,   30,     0, 0x00,   0,  0x80,       3, SC300, SD_NOR_L,   0,  90, 112,    0,                   NO_KO,  1,     0, 40,     0,     0,   0,        6, { 0, 0 } },	/* 47: pl22_at_midare, Midare Zakura hyper, first record */
	{   4,               5 | NO_ZU,      A_HYPER,  K_L,    0,    2,   30,     0, 0x00,   0,  0x80,       3, SC300, SD_NOR_L,   0,  90,  58,    0,                   NO_KO,  1,     0, 40,     0,     0,   0,        6, { 0, 0 } },	/* 48 */
	{   4,               5 | NO_ZU,      A_HYPER, K_LL,    0,    2,   30,     0, 0x00,   0,  0x80,       3, SC300, SD_NOR_L,   0,  90,  58,    0,                   NO_KO,  1,     0, 40,     0,     0,   0,        6, { 0, 0 } },	/* 49 */
	{  22,              16 | NO_ZU,      A_HYPER,  K_S,    0,    2,   30,     0, 0x00,   0,  0x80,       3, SC300, SD_NOR_L,   0,  90,  40,    0,                   NO_KO,  1,     0, 40,     0,     0,   0,        6, { 0, 0 } },	/* 4a */
	{  15,            BU_O | NO_ZU,    A_SPECIAL,  K_S,    0,   10,   30,     0, 0x1e,   7,  0x80,       3, SC300, SD_KIK_L,   0, 100,  11,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        7, { 0, 0 } },	/* 4b */
	{  15,            BU_O | NO_ZU,    A_SPECIAL,  K_S,    0,   10,   30,     0, 0x1e,   8,  0x80,       3, SC300, SD_KIK_L,   0, 100,  11,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        7, { 0, 0 } },	/* 4c: pl22_at_shunpu_assist, Shunpukyaku as a partner's attack (team_type 8) */
	{  10,            BU_O | NO_ZU,    A_SPECIAL,  K_S,    0,   10,   30,     0, 0x1a,   5,  0x80,       3, SC300, SD_KIK_L,   0,  40,  11,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        7, { 0, 0 } },	/* 4d */
	{  14,     ZU | NO_VEC | NO_ZU,    A_SPECIAL,  K_M,    0,    8,   24,     0, 0x18,   8,  0x80,       3, SC300, SD_NOR_L,   0,  50,   8,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 4e */
	{ 0 },	/* 4f */
	{  14,     ZU | NO_VEC | NO_ZU,    A_SPECIAL,  K_M,    0,    8,   24,     0, 0x18,   8,  0x80,       3, SC300, SD_NOR_L,   0,  50,   8,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 50 */
	{   5,   BU_O | NO_VEC | NO_ZU,      A_HYPER,  K_M,    0,   10,   24,     0, 0x1d,   4,  0x80,       3, SC300, SD_NOR_L,   0,  50,   9,    0,               NO_WEIGHT,  1,     0, 38,     0,     0,   5,        4, { 0, 0 } },	/* 51: pl22_at_shinkuu, Shinkuu Hadouken hyper (sp_shinkuu): her record and the sparks (shl22_00 type 5) */
	{   8,               5 | NO_ZU,      A_HYPER, K_LL,    0,    2,   24,     0, 0x08,   0,  0x80,       3, SC300, SD_NOR_L,   0,  50,  10,    0,                   NO_KO,  1,     0, 39,     0,     0,   0,        7, { 0, 0 } },	/* 52 */
	{  25,          2 | FLY_SCREEN,      A_HYPER,  K_L,    0,   10,   24,     0, 0x1c,  10,  0x80,       3, SC300, SD_NOR_L,   0,  50,  10,    0,               NO_WEIGHT,  1,     0, 39,     0,     0,   0,        7, { 0, 0 } },	/* 53 */
	{   2,             ZU | NO_VEC,      A_HYPER,  K_M,    0,    4,   30,     0, 0x1d,   8,  0x80,       3, SC300, SD_NOR_L,   0,  90,  40,    0,                   NO_KO,  1,     0, 40,     0,     0,   0,        6, { 0, 0 } },	/* 54 */
	{   4,      2 | NO_VEC | NO_ZU,      A_HYPER,  K_L,    0,    4,   20,     0, 0x1a,   6,  0x60,       6, SC200,       18,   0,  90,  76,    0,                       0,  1,     0, 38,     0,     0,   0,        8, { 0, 0 } },	/* 55: pl22_at_dshinkuu, Dark-form Shinkuu Hadouken hyper (sp_dshinkuu): her record and every third petal hit (shl22_01) */
	{   4,      2 | NO_VEC | NO_ZU,      A_HYPER,  K_L,    0,    4,   20,     0, 0x1a,   6,  0x60,       6, SC200,       18,   0,  90,  76,    0,                 NO_CHIP,  1,     0, 38,     0,     0,   0,        8, { 0, 0 } },	/* 56: pl22_at_dshinkuu_2, Dark-form Shinkuu Hadouken, the other petal hits */
	{   2,           BU_O | NO_VEC,      A_HYPER,  K_M,    0,    4,   30,     0, 0x1d,   8,  0x80,       3, SC300, SD_NOR_L,   0,  90,  41,    0,                   NO_KO,  1,     0, 40,     0,     0,   0,        6, { 0, 0 } },	/* 57 */
	{   2,           BU_O | NO_VEC,      A_HYPER,  K_M,    0,    4,   30,     0, 0x1d,   8,  0x80,       3, SC300, SD_NOR_L,   0,  90,  42,    0,                   NO_KO,  1,     0, 40,     0,     0,   0,        6, { 0, 0 } },	/* 58 */
	{   8,              2 | NO_VEC,      A_HYPER,  K_L,    0,    4,   30,     0, 0x1d,  12,  0x80,       3, SC300, SD_NOR_L,   0,  90,  42,    0,               NO_WEIGHT,  1,     0, 40,     0,     0,   0,        6, { 0, 0 } },	/* 59 */
	{  15,     ZU | NO_VEC | NO_ZU,    A_SPECIAL,  K_L,    0,   10,   30,     0, 0x1c,   8,  0x80,       3, SC300, SD_KIK_L,   0, 100,  68,    0,                       1,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 5a */
	{   8,                      ZU,    A_SPECIAL,  K_S,    0,   10,   30,     0, 0x1e,  10,  0x80,       3, SC300, SD_KIK_L,   0, 100,  68,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 5b */
	{  15,     ZU | NO_VEC | NO_ZU,    A_SPECIAL,  K_L,    0,   10,   30,     0, 0x1c,   8,  0x80,       3, SC300, SD_KIK_L,   0,  40,  68,    0,                       0,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 5c */
	{   8,                      ZU,    A_SPECIAL,  K_S,    0,   10,   30,     0, 0x18,   4,  0x80,       3, SC300, SD_KIK_L,   0,  90,  70,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 5d: pl22_at_jshunpu_land, Air Shunpukyaku, the hit once she reaches the floor */
	{   3,           BU_O | NO_VEC,    A_SPECIAL,  K_M,    0,    4,   30,     0, 0x1e,   4,  0x80,       3, SC300, SD_NOR_L,   0,  90,  80,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 5e */
	{   3,           BU_O | NO_VEC,    A_SPECIAL,  K_M,    0,    4,   30,     0, 0x1e,   4,  0x80,       3, SC300, SD_NOR_L,   0,  90,  82,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 5f */
	{  10,           BU_O | NO_VEC,    A_SPECIAL,  K_L,    0,    6,   30,     0, 0x1e,  12,  0x80,       3, SC300, SD_NOR_L,   0,  90,  84,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 60 */
	{   3,           BU_O | NO_VEC,      A_HYPER,  K_L,    0,    4,   30,     0, 0x00,   5,  0x80,       3, SC300, SD_KIK_L,   0,  90, 107,    0,       NO_WEIGHT | NO_KO,  1,     0, 39,     0,     0,   0,        3, { 0, 0 } },	/* 61 */
	{   5,           BU_O | NO_VEC,      A_HYPER,  K_L,    0,    4,   30,     0, 0x1c,   8,  0x80,       3, SC300, SD_KIK_L,   0,  90, 108,    0,       NO_WEIGHT | NO_KO,  1,     0, 39,     0,     0,   0,        3, { 0, 0 } },	/* 62 */
	{   2,           BU_O | NO_VEC,      A_HYPER,  K_L,    0,    4,   30,     0, 0x18,   0,  0x80,       3, SC300, SD_NOR_L,   0,  90,  58,    0,       NO_WEIGHT | NO_KO,  1,     0, 39,     0,     0,   0,        3, { 0, 0 } },	/* 63 */
	{  40, 2 | NO_VEC | FLY_SCREEN,      A_HYPER,  K_L,    0,   10,   30,     0, 0x12,   8,  0x80,       3, SC300, SD_NOR_L,   0,  90,  58,    0,               NO_WEIGHT,  1,     0, 39,     0,     0,   0,        3, { 0, 0 } },	/* 64 */
	{   4,           BU_O | NO_VEC,      A_HYPER,  K_L,    0,    4,   30,     0, 0x02,   8,  0x80,       3, SC300, SD_KIK_L,   0,  90, 109,    0,       NO_WEIGHT | NO_KO,  1,     0, 39,     0,     0,   0,        3, { 0, 0 } },	/* 65: pl22_at_haru, Haru Ichiban hyper, first record */
	{   4,           BU_O | NO_VEC,      A_HYPER,  K_L,    0,    4,   30,     0, 0x02,   4,  0x80,       3, SC300, SD_KIK_L,   0,  90,  58,    0,       NO_WEIGHT | NO_KO,  1,     0, 39,     0,     0,   0,        3, { 0, 0 } },	/* 66 */
	{   7,   BU_O | NO_VEC | NO_ZU,      A_HYPER,  K_M,    0,   10,   24,     0, 0x1d,  13,  0x80,       3, SC300, SD_NOR_L,   0,  50,   9,   23,               NO_WEIGHT,  1,     0, 38,     0,     0,   0,        4, { 0, 0 } },	/* 67: pl22_at_shinkuu_last, Shinkuu Hadouken, the last round of sparks (sub_type 1) */
	{  25,            BU_O | NO_ZU,    A_SPECIAL,  K_S,    0,   10,   30,     0, 0x1e,   7,  0x80,       3, SC300, SD_KIK_L,   0,  90,  68,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 68 */
	{  15,            BU_O | NO_ZU,    A_SPECIAL,  K_S,    0,   10,   30,     0, 0x1e,   8,  0x80,       3, SC300, SD_KIK_L,   0,  90,  68,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 69 */
	{  20,            BU_O | NO_ZU,    A_SPECIAL,  K_S,    0,   10,   30,     0, 0x1e,   9,  0x80,       3, SC300, SD_KIK_L,   0,  90,  68,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 6a */
	{  10,             12 | NO_VEC,    A_SPECIAL,  K_L,    0,    4,   24,     0, 0x1c,  12,  0x80,       3, SC300,       12,  64,  90,  35,   36,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 6b: pl22_at_snapback, Snapback (sp_snapback) */
	{  13,           BU_O | NO_VEC,    A_SPECIAL, K_LL,    0,   10,   24,     0, 0x1f,  11,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  40,  24,    0, NO_CHIP | NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 6c: pl22_at_upper_p, Crouching HP launcher */
	{  12,     ZU | NO_VEC | NO_ZU,    A_SPECIAL,  K_L,    0,   10,   24,     0, 0x18,   6,  0x80,       3, SC300, SD_NOR_L,   0,  50, 115,    0,                       0,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 6d */
	{  12,     ZU | NO_VEC | NO_ZU,    A_SPECIAL,  K_L,    0,   10,   24,     0, 0x18,  10,  0x80,       3, SC300, SD_NOR_L,   0,  50, 115,    0,                       0,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 6e */
	{  12,     ZU | NO_VEC | NO_ZU,    A_SPECIAL,  K_L,    0,   10,   24,     0, 0x18,   6,  0x80,       3, SC300, SD_NOR_L,   0,  50, 117,    0,           AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 6f */
	{  12,     ZU | NO_VEC | NO_ZU,    A_SPECIAL,  K_L,    0,   10,   24,     0, 0x18,  10,  0x80,       3, SC300, SD_NOR_L,   0,  50, 117,    0,           AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 70 */
	{   7,           BU_O | NO_VEC,    A_SPECIAL,  K_M,    0,   10,   16,     0, 0x1f,  11,  0x60, MARK_N2, SC200, SD_KIK_M,   0,  40,  20,   54, NO_CHIP | NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 71: pl22_at_upper_k, the kick launcher (standing) */
};
