/* Iceman (character 09): the attacks (HIT_DATA, 0x1c bytes).
   What each hit does. The character's module sets the attack's number (atck_no) with the move,
   and the row is read when the attack box meets a body. After a row is its number, in
   hexadecimal, and the program's name for it:
   ATCK_ for the normals every character has (player.h), pl09_at_ for its own (pl09_def.h).
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
     box, box2  boxes of hit_09 that the computer measures the attack's reach by
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

const HIT_DATA hit_dt_09[] = {
	/* dmg                   type              kind  elem  piyo  stop  sand  sleep   vec  pow  range     mark  score        se  shc  jug  box  box2                     flag  13  guard  15  catch  abyss  cnt    answer        1a */
	{   4,                    ZU,            A_NOM,  K_S,    4,   10,   10,     0, 0x18,   2,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  16,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 00: ATCK_STAND_LP */
	{   7,                    ZU,            A_NOM, K_LM,    2,   11,   16,     0, 0x00,   4,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40,  18,   63,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 01: ATCK_STAND_MP */
	{  15,                    ZU,            A_NOM,  K_L,    1,   12,   24,     0, 0x1c,   6,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  50,  71,   72,                       0,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 02: ATCK_STAND_HP */
	{   4,                    ZU,            A_NOM,  K_S,    4,   10,   10,     0, 0x18,   2,  0x60, MARK_N1, SC100, SD_KIK_S,   0,  20,  21,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 03: ATCK_STAND_LK */
	{   7,                    ZU,            A_NOM,  K_M,    2,   10,   16,     0, 0x1c,   4,  0x60, MARK_N2, SC200, SD_KIK_M,   0,  40,  23,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 04: ATCK_STAND_MK */
	{  13,                    ZU,            A_NOM,  K_L,    1,   12,   24,     0, 0x1e,  10,  0x80, MARK_N3, SC300, SD_KIK_L,   0,  50,  25,    0,                       0,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 05: ATCK_STAND_HK */
	{   4,                    ZU,            A_NOM, K_LS,    4,   10,   10,     0, 0x18,   2,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  27,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 06: ATCK_CROUCH_LP */
	{   7,                    ZU,            A_NOM, K_LM,    2,   11,   16,     0, 0x1c,   4,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40,  29,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 07: ATCK_CROUCH_MP */
	{  13,                    AS,             A_LO, K_LL,    1,   10,   24,     0, 0x1c,   6,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  90,  31,   40,                       0,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 08: ATCK_CROUCH_HP */
	{   4,                    ZU,             A_LO, K_LS,    4,   10,   10,     0, 0x18,   1,  0x70, MARK_N1, SC100, SD_KIK_S,   0,  20,  33,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 09: ATCK_CROUCH_LK */
	{   7,                    ZU,             A_LO, K_LM,    2,   11,   16,     0, 0x1a,   4,  0x40, MARK_N2, SC200, SD_KIK_M,   0,  40,  35,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0a: ATCK_CROUCH_MK */
	{  13,         BU_O | NO_VEC,            A_NOM, K_LL,    1,   12,   24,     9, 0x1f,  11,  0x70, MARK_N3, SC300, SD_KIK_L,   0,  50,  37,    0, NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 0b: ATCK_CROUCH_HK */
	{   4,           ZU | NO_VEC,             A_HI,  K_S,    1,   10,   10,     0, 0x00,   5,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  44,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0c: ATCK_JUMP_LP */
	{   7,           ZU | NO_VEC,             A_HI, K_LM,    1,   10,   16,     0, 0x00,   5,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40,  41,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0d: ATCK_JUMP_MP */
	{  13,           ZU | NO_VEC,             A_HI, K_LM,    1,   12,   32,     0, 0x16,   8,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  60,  43,    0,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 0e: ATCK_JUMP_HP */
	{   4,           ZU | NO_VEC,             A_HI,  K_S,    1,   10,   10,     0, 0x00,   5,  0x60, MARK_N1, SC200, SD_KIK_S,   0,  20,  45,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0f: ATCK_JUMP_LK */
	{   7,           ZU | NO_VEC,             A_HI,  K_M,    1,   10,   16,     0, 0x00,   5,  0x60, MARK_N2, SC200, SD_KIK_M,   0,  40,  47,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 10: ATCK_JUMP_MK */
	{  13,           ZU | NO_VEC,             A_HI,  K_S,    1,   12,   32,     0, 0x15,   8,  0x60, MARK_N3, SC200, SD_KIK_L,   0,  60,  49,    0,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 11: ATCK_JUMP_HK */
	{  13,           ZU | NO_VEC,             A_HI,  K_S,    1,   12,   32,     0, 0x15,   8,  0x60, MARK_N3, SC200, SD_KIK_L,   0,  60,  55,    0,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 12: pl09_at_jdown_hk, Jumping HK with the stick down */
	{ 0 },	/* 13 */
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
	{   8,                     2,     A_NOM | 0x40, K_LM,    4,   10,   20,     0, 0x19,   2,  0x60, MARK_N2, SC200, SD_NOR_M,   0,  40,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 20: pl09_at_throw_p, the punch throw, set on the opponent */
	{  16,                     2,     A_NOM | 0x40, K_LM,    4,   10,   20,     0, 0x04,  11,  0x60, MARK_N2, SC200, SD_NOR_M,   0,  40,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 21: pl09_at_throw_k, the kick throw, set on the opponent */
	{  16,                     2,     A_NOM | 0x40, K_LM,    4,   10,   20,     0, 0x0c,  11,  0x60, MARK_N2, SC200, SD_NOR_M,   0,  40,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 22: pl09_at_air_throw, the air throw, set on the opponent */
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
	{  20, BU_O | NO_VEC | NO_ZU, A_SPECIAL | 0x80, K_LL,    0,   14,   20,     0, 0x1e,   6,  0x80,       3, SC300,        8,  10,  50,  70,    0,                       0,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 30: pl09_at_avalanche_l, Ice Avalanche from the ground, light (0x30 + level, its shot shl09_02 too); Arctic Attack's start sets it as well */
	{  20, BU_O | NO_VEC | NO_ZU, A_SPECIAL | 0x80, K_LL,    0,   14,   20,     0, 0x1e,   6,  0x80,       3, SC300,        8,  10,  50,  70,    0,                       0,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 31 */
	{  20, BU_O | NO_VEC | NO_ZU, A_SPECIAL | 0x80, K_LL,    0,   14,   20,     0, 0x1e,   6,  0x80,       3, SC300,        8,  10,  50,  70,    0,                       0,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 32 */
	{  20, BU_O | NO_VEC | NO_ZU, A_SPECIAL | 0x80, K_LL,    0,   14,   20,     0, 0x1e,   6,  0x80,       3, SC300,        8,  10,  50,  70,    0,                       0,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 33 */
	{   3, BU_O | NO_VEC | NO_ZU, A_SPECIAL | 0x80,    1,    0,    3,   20,     0, 0x1c,   8,  0x80,       4, SC300,       18,   1,   5,  64,    0,                       0,  1,     0,  0,     0,     0,   2,        8, { 0, 0 } },	/* 34 */
	{   3, BU_O | NO_VEC | NO_ZU, A_SPECIAL | 0x80,    1,    0,    3,   20,     0, 0x1c,   8,  0x80,       4, SC300,       18,   1,   5,  64,    0,                       0,  1,     0,  0,     0,     0,   2,        8, { 0, 0 } },	/* 35 */
	{   3, BU_O | NO_VEC | NO_ZU, A_SPECIAL | 0x80,    1,    0,    3,   20,     0, 0x1e,   6,  0x80,       4, SC300,       18,   1,   5,  64,    0,                       0,  1,     0,  0,     0,     0,   2,        8, { 0, 0 } },	/* 36 */
	{   3, BU_O | NO_VEC | NO_ZU, A_SPECIAL | 0x80,    1,    0,    3,   20,     0, 0x1e,   6,  0x80,       4, SC300,       18,   1,   5,  64,    0,                       0,  1,     0,  0,     0,     0,   2,        8, { 0, 0 } },	/* 37 */
	{   4,    2 | NO_VEC | NO_ZU,   A_HYPER | 0x80,  K_S,    0,    3,   16,     0, 0x1c,   4,  0x80,       3,    20, SD_NOR_S,   0,  90,  66,    0,                       0,  1,     0,  0,     0,     0,   4,        3, { 0, 0 } },	/* 38 */
	{   3,    2 | NO_VEC | NO_ZU,          A_HYPER,  K_S,    0,    3,   16,     0, 0x1c,   4,  0x80,       3,    20, SD_NOR_S,   0,  90,  66,    0,                 NO_CHIP,  1,     0,  0,     0,     0,   4,        3, { 0, 0 } },	/* 39 */
	{ 0 },	/* 3a */
	{  10,           10 | NO_VEC,        A_SPECIAL,  K_L,    0,   10,   10,     0, 0x1f,  10,  0x60,       3, SC200,       12,   0,  90,  51,    0, NO_CHIP | NO_WEIGHT | AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        5, { 0, 0 } },	/* 3b: pl09_at_t_kick, the jump-in kick when he is tagged in (pl09_t_kick) */
	{  10,           12 | NO_VEC,        A_SPECIAL,  K_L,    0,    4,   24,     0, 0x1c,  12,  0x80,       3, SC300,        8,  64,  90,  71,   72,               NO_WEIGHT,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 3c: pl09_at_snapback, Snapback (also shl09_00 kind 5) */
	{  15,                    ZU,            A_NOM,  K_L,    1,   12,   24,     0, 0x1c,   6,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  50,  71,   72,                       0,  0,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 3d */
	{  13,                  BU_O,            A_NOM,  K_L,    1,   12,   24,     0, 0x1e,  10,  0x80, MARK_N3, SC300, SD_KIK_L,   0,  50,  25,    0,                       0,  0,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 3e */
	{   4,           ZU | NO_VEC,             A_HI,  K_S,    4,   10,   10,     0, 0x00,   5,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  39,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 3f */
	{  13,                    AS,             A_LO, K_LL,    1,   10,   24,     0, 0x1c,   6,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  90,  31,    0,                       0,  0,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 40 */
	{  10,            ZU | NO_ZU,            A_NOM, K_LM,    1,    8,   24,     0, 0x14,   8,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  60,  58,    0,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 41: pl09_at_jdown_hp, Jumping HP with the stick down */
	{  13,         BU_O | NO_VEC,        A_SPECIAL, K_LL,    0,   12,   24,     9, 0x1f,  11,  0x70, MARK_N3, SC300, SD_KIK_L,   0,  40,  37,    0, NO_CHIP | NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 42: pl09_at_upper, the launcher, crouching */
	{   6,           ZU | NO_VEC,     A_NOM | 0x80,  K_S,    8,   10,   10,     0, 0x1c,   5,  0x80, MARK_N1, SC300, SD_NOR_S,   0,  90,  68,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 43: pl09_at_fist_lp, LP, standing or crouching, with the Ice Fist on (shl09_04) */
	{  11,           ZU | NO_VEC,     A_NOM | 0x80,  K_M,    8,   11,   16,     0, 0x1c,   5,  0x90, MARK_N2, SC300, SD_NOR_M,   0,  90,  68,   68,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 44: pl09_at_fist_mp, M punch with the Ice Fist on */
	{  18,           ZU | NO_VEC,     A_NOM | 0x80,  K_L,    8,   12,   24,     0, 0x1c,   5,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  90,  68,    0,                       0,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 45: pl09_at_fist_hp, HP with the Ice Fist on */
	{   6,           ZU | NO_VEC,      A_HI | 0x80,  K_S,    8,   10,   10,     0, 0x1c,   5,  0x80, MARK_N1, SC300, SD_NOR_S,   0,  90,  68,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 46: pl09_at_fist_jlp, Jumping LP with the Ice Fist on (shl09_00 gives j.LP attack 63) */
	{  11,           ZU | NO_VEC,      A_HI | 0x80,  K_M,    8,   10,   16,     0, 0x1c,   5,  0x90, MARK_N2, SC300, SD_NOR_M,   0,  90,  68,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 47: pl09_at_fist_jmp, Jumping M punch with the Ice Fist on */
	{  18,           ZU | NO_VEC,      A_HI | 0x80,  K_L,    8,   12,   32,     0, 0x1c,   5,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  90,  68,    0,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 48: pl09_at_fist_jhp, Jumping HP with the Ice Fist on */
};
