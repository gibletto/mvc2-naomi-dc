/* Colossus (character 32): the attacks (HIT_DATA, 0x1c bytes).
   What each hit does. The character's module sets the attack's number (atck_no) with the move,
   and the row is read when the attack box meets a body. After a row is its number, in
   hexadecimal, and the program's name for it:
   ATCK_ for the normals every character has (player.h), pl32_at_ for its own (pl32_def.h).
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
     box, box2  boxes of hit_32 that the computer measures the attack's reach by
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

const HIT_DATA hit_dt_32[] = {
	/* dmg                   type          kind  elem  piyo  stop  sand  sleep   vec  pow  range     mark  score        se  shc  jug  box  box2                     flag  13  guard  15  catch  abyss  cnt    answer        1a */
	{   6,           ZU | NO_VEC,        A_NOM,  K_S,    4,   11,    8,     0, 0x19,   8,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  13,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 00: ATCK_STAND_LP */
	{   9,           ZU | NO_VEC,        A_NOM, K_LM,    2,   14,   12,     0, 0x19,   9,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40,  18,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 01: ATCK_STAND_MP */
	{  16,                  BU_O,        A_NOM,  K_L,    1,   17,   16,     0, 0x1a,  12,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  60,  27,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 02: ATCK_STAND_HP */
	{   6,           ZU | NO_VEC,        A_NOM, K_LS,    4,   11,    8,     0, 0x19,   4,  0x60, MARK_N1, SC100, SD_KIK_S,   0,  20,  31,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 03: ATCK_STAND_LK */
	{   9,           ZU | NO_VEC,        A_NOM, K_LM,    2,   14,   12,     0, 0x19,   8,  0x60, MARK_N2, SC200, SD_KIK_M,   0,  40,  37,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 04: ATCK_STAND_MK */
	{  16,         BU_O | NO_VEC,        A_NOM,  K_L,    1,   17,   16,     0, 0x1f,  11,  0x80, MARK_N3, SC300, SD_KIK_L,   0,  60,  42,    0, NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 05: ATCK_STAND_HK */
	{   6,           ZU | NO_VEC,        A_NOM, K_LS,    4,   11,    8,     0, 0x19,   8,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  43,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 06: ATCK_CROUCH_LP */
	{   9,                    ZU,        A_NOM, K_LM,    2,   14,   12,     0, 0x19,   8,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40,  48,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 07: ATCK_CROUCH_MP */
	{  19,                    ZU,        A_NOM, K_LL,    1,   17,   16,     0, 0x19,  10,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  60,  52,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 08: ATCK_CROUCH_HP */
	{   6,           ZU | NO_VEC,         A_LO, K_LS,    4,   11,    8,     0, 0x18,   7,  0x70, MARK_N1, SC100, SD_KIK_S,   0,  20,  53,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 09: ATCK_CROUCH_LK */
	{   9,                    ZU,         A_LO, K_LM,    2,   14,   12,     0, 0x18,   7,  0x40, MARK_N2, SC200, SD_KIK_M,   0,  40,  55,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0a: ATCK_CROUCH_MK */
	{  16,                    AS,         A_LO, K_LL,    1,   17,   16,     5, 0x18,   9,  0x70, MARK_N3, SC300, SD_KIK_L,   0,  90,  57,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0b: ATCK_CROUCH_HK */
	{   6,           ZU | NO_VEC,         A_HI,  K_S,    1,   11,    8,     0, 0x00,   4,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  64,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0c: ATCK_JUMP_LP */
	{   9,           ZU | NO_VEC,         A_HI, K_LM,    1,   14,   12,     0, 0x00,   2,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40,  66,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0d: ATCK_JUMP_MP */
	{  16,           ZU | NO_VEC,         A_HI,  K_L,    1,   17,   16,     0, 0x14,  10,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  60,  68,   69,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0e: ATCK_JUMP_HP */
	{   6,           ZU | NO_VEC,         A_HI,  K_S,    1,   11,    8,     0, 0x00,   4,  0x60, MARK_N1, SC200, SD_KIK_S,   0,  20,  74,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0f: ATCK_JUMP_LK */
	{   9,           ZU | NO_VEC,         A_HI,  K_M,    1,   14,   12,     0, 0x00,   2,  0x60, MARK_N2, SC200, SD_KIK_M,   0,  40,  76,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 10: ATCK_JUMP_MK */
	{  16,           ZU | NO_VEC,         A_HI,  K_L,    1,   17,   12,     0, 0x18,  12,  0x60, MARK_N3, SC200, SD_KIK_L,   0,  60,  78,    0,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 11: ATCK_JUMP_HK */
	{  16,           ZU | NO_VEC,         A_HI,  K_L,    1,   17,   16,     0, 0x18,  10,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  60,  72,    0,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 12: pl32_at_jfwd_hp, Air heavy punch with forward */
	{ 0 },	/* 13 */
	{ 0 },	/* 14 */
	{ 0 },	/* 15 */
	{ 0 },	/* 16 */
	{ 0 },	/* 17 */
	{ 0 },	/* 18 */
	{  15,         BU_O | NO_VEC,    A_SPECIAL,  K_L,    0,   12,   16,     0, 0x1f,  11,  0x80, MARK_N3, SC300, SD_KIK_L,   0,  60,  42,    0, NO_CHIP | NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 19: pl32_at_upper, the launcher (pl32_upper) */
	{ 0 },	/* 1a */
	{ 0 },	/* 1b */
	{ 0 },	/* 1c */
	{ 0 },	/* 1d */
	{ 0 },	/* 1e */
	{ 0 },	/* 1f */
	{  24,        2 | FLY_SCREEN, A_NOM | 0x40,    0,    4,   16,    0,    15, 0x1a,  12,  0x80, MARK_N1, SC300,       76,   0,   0,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 20: pl32_at_swing_throw, Giant Swing's grab: thrown down at the top (rush_top, ground and air) */
	{  16,                     2, A_NOM | 0x40,    0,    4,   16,   24,     0, 0x1c,   4,  0x80, MARK_N1, SC300,       76,   0,   0,   0,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 21: pl32_at_throw_k, Ground kick throw, slammed on landing */
	{  16,                  BU_O, A_NOM | 0x40,    0,    4,   16,   30,     4, 0x06,  10,  0x80, MARK_N1, SC300,       76,   0,   0,   0,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 22: pl32_at_throw_p, Ground punch throw */
	{  16,                     2, A_NOM | 0x40,    0,    4,   16,   30,     0, 0x1c,   4,  0x80, MARK_N1, SC300,       76,   0,   0,   0,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 23: pl32_at_air_throw, Air punch throw, slammed on landing */
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
	{  25, BU_O | NO_VEC | NO_ZU,    A_SPECIAL,  K_L,    0,   21,   16,     0, 0x1c,  14,  0x80,       3, SC200, SD_NOR_L,   0,  90,  81,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 30 */
	{  25, BU_O | NO_VEC | NO_ZU,    A_SPECIAL,  K_L,    0,   21,   16,     0, 0x1e,  15,  0x80,       3, SC200, SD_NOR_L,   0,  90,  81,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 31 */
	{   0,            ZU | NO_ZU,    A_SPECIAL,  K_M,    0,    0,   16,     5, 0x08,   0,  0x80,       3, SC300, SD_NOR_S,   0,  90,  87,    0,                NO_COMBO,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 32: pl32_at_swing, Giant Swing (sp_swing): the rush that grabs */
	{  10,            21 | NO_ZU,      A_HYPER, K_LL,    0,   21,   40,    40, 0x12,  13,  0x80,       3, SC300, SD_NOR_L,   0,  90,  91,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 33: pl32_at_dive, Super Dive (hyper): the start and rise */
	{  40,            21 | NO_ZU,      A_HYPER,  K_L,    0,   21,   48,     5, 0x1a,   8,  0x80,       3, SC200, SD_NOR_L,   0,  90,  93,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 34: pl32_at_dive_2, Super Dive: the dive down */
	{  10,           10 | NO_VEC,    A_SPECIAL,  K_L,    0,   10,   10,     0, 0x1f,  10,  0x60,       3, SC200,       12,   0,  90,  27,    0, NO_CHIP | NO_WEIGHT | AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 35: pl32_at_t_kick, the partner's jump-in attack (t_kick) */
	{  10,           12 | NO_VEC,    A_SPECIAL,  K_L,    0,   21,   24,     0, 0x1c,  12,  0x80,       3, SC300,        8,  64, 100,   0,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 36: pl32_at_snapback, Snapback (sp_snapback) */
};
