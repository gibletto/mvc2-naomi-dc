/* Marrow (character 16): the attacks (HIT_DATA, 0x1c bytes).
   What each hit does. The character's module sets the attack's number (atck_no) with the move,
   and the row is read when the attack box meets a body. After a row is its number, in
   hexadecimal, and the program's name for it:
   ATCK_ for the normals every character has (player.h), pl16_at_ for its own (pl16_def.h).
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
     box, box2  boxes of hit_16 that the computer measures the attack's reach by
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

const HIT_DATA hit_dt_16[] = {
	/* dmg                     type              kind  elem  piyo  stop  sand  sleep   vec  pow  range     mark  score        se  shc  jug  box  box2                     flag  13  guard  15  catch  abyss  cnt    answer        1a */
	{   3,                      ZU,            A_NOM,  K_S,    4,    8,   10,     0, 0x18,   4,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  64,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 00: ATCK_STAND_LP */
	{   6,                      ZU,            A_NOM,  K_M,    2,    9,   16,     0, 0x00,   6,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40,  65,   63,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 01: ATCK_STAND_MP */
	{  12,                    BU_O,            A_NOM,  K_L,    1,   10,   24,     0, 0x1a,  12,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  50,  67,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 02: ATCK_STAND_HP */
	{   3,                      ZU,            A_NOM, K_LS,    4,    8,   10,     0, 0x18,   4,  0x60, MARK_N1, SC100, SD_KIK_S,   0,  20,  68,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 03: ATCK_STAND_LK */
	{   6,                      ZU,            A_NOM,  K_M,    2,    9,   16,     0, 0x1a,   4,  0x60, MARK_N2, SC200, SD_KIK_M,   0,  40,  69,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 04: ATCK_STAND_MK */
	{   6,                      ZU,            A_NOM,  K_L,    1,    8,   24,     0, 0x1a,   8,  0x80, MARK_N3, SC300, SD_KIK_L,   0,  50,  70,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 05: ATCK_STAND_HK */
	{   3,                      ZU,            A_NOM, K_LS,    4,    8,   10,     0, 0x1c,   4,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  72,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 06: ATCK_CROUCH_LP */
	{   6,                      ZU,            A_NOM, K_LM,    2,    9,   16,     0, 0x13,   6,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40,  73,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 07: ATCK_CROUCH_MP */
	{  12,           BU_O | NO_VEC,            A_NOM, K_LL,    1,   12,   24,     0, 0x00,  11,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  50,  71,   74, NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 08: ATCK_CROUCH_HP */
	{   3,                      ZU,             A_LO, K_LS,    4,    8,   10,     0, 0x1c,   4,  0x70, MARK_N1, SC100, SD_KIK_S,   0,  20,  75,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 09: ATCK_CROUCH_LK */
	{   6,           BU_O | NO_VEC,            A_NOM, K_LM,    2,    9,   16,     0, 0x00,   5,  0x40, MARK_N2, SC200, SD_KIK_M,   0,  40,  76,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0a: ATCK_CROUCH_MK */
	{  12,                      AS,             A_LO, K_LL,    1,   10,   24,     9, 0x1a,   8,  0x70, MARK_N3, SC300, SD_KIK_L,   0, 100,  77,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0b: ATCK_CROUCH_HK */
	{   3,             ZU | NO_VEC,             A_HI,  K_S,    1,    8,   10,     0, 0x00,   6,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  78,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0c: ATCK_JUMP_LP */
	{   6,             ZU | NO_VEC,             A_HI, K_LM,    1,    9,   16,     0, 0x00,   5,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40,  79,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0d: ATCK_JUMP_MP */
	{  12,             ZU | NO_VEC,             A_HI, K_LM,    1,   10,   32,     0, 0x16,  11,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  60,  80,    0,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0e: ATCK_JUMP_HP */
	{   3,             ZU | NO_VEC,             A_HI,  K_S,    1,    8,   10,     0, 0x1d,   6,  0x60, MARK_N1, SC200, SD_KIK_S,   0,  20,  81,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0f: ATCK_JUMP_LK */
	{   6,             ZU | NO_VEC,             A_HI,  K_M,    1,    9,   16,     0, 0x00,   5,  0x60, MARK_N2, SC200, SD_KIK_M,   0,  40,  82,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 10: ATCK_JUMP_MK */
	{  12,             ZU | NO_VEC,             A_HI,  K_S,    1,   10,   32,     0, 0x15,  11,  0x60, MARK_N3, SC200, SD_KIK_L,   0,  60,  83,    0,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 11: ATCK_JUMP_HK */
	{  12,           BU_O | NO_VEC,        A_SPECIAL, K_LL,    0,   12,   24,     0, 0x00,  11,  0x80, MARK_N2,   156, SD_NOR_L,   0,  40,   8,   40, NO_CHIP | NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 12: pl16_at_upper, the launcher */
	{   6,           BU_O | NO_VEC,        A_SPECIAL, K_LM,    0,   12,   16,     0, 0x00,  11,  0x40, MARK_N2, SC200, SD_KIK_M,   0,  40,  42,    0, NO_CHIP | NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 13 */
	{ 0 },	/* 14 */
	{ 0 },	/* 15 */
	{ 0 },	/* 16 */
	{ 0 },	/* 17 */
	{ 0 },	/* 18 */
	{   6,             ZU | NO_VEC,             A_HI,  K_M,    1,   10,   32,     0, 0x16,   8,  0x80, MARK_N3, SC300, SD_KIK_L,   0,  60,  49,    0,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 19 */
	{ 0 },	/* 1a */
	{ 0 },	/* 1b */
	{ 0 },	/* 1c */
	{ 0 },	/* 1d */
	{ 0 },	/* 1e */
	{ 0 },	/* 1f */
	{  16,                    BU_O,     A_NOM | 0x40, K_LM,    0,   10,   20,     0, 0x1c,  11,  0x60, MARK_N2, SC200, SD_NOR_M,   0,  40,   0,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 20: pl16_at_throw_p, Ground punch throw's release */
	{  16,                       2,     A_NOM | 0x40, K_LM,    0,   10,   20,     0, 0x1c,  11,  0x60, MARK_N2, SC200, SD_NOR_M,   0,  40,   0,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 21: pl16_at_air_throw, Air throw's release */
	{   5,                    BU_O, A_SPECIAL | 0x40, K_LM,    0,   10,   20,     0, 0x1c,  10,  0x60, MARK_N2, SC200, SD_NOR_M,   0,  40,   0,    0,                   NO_KO,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 22: pl16_at_ride_1, Ride and Slash: the first two slashes on the caught opponent */
	{   5,                       2, A_SPECIAL | 0x40, K_LM,    0,   12,   20,     0, 0x1c,  10,  0x60, MARK_N2, SC200, SD_NOR_M,   0,  40,   0,    0,                   NO_KO,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 23: pl16_at_ride_2, Ride and Slash: the third slash */
	{   8,                    BU_O, A_SPECIAL | 0x40, K_LM,    0,   12,   20,     0, 0x1c,  10,  0x60, MARK_N2, SC200, SD_NOR_M,   0,  40,  35,   36,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 24: pl16_at_ride_3, Ride and Slash: jumping off, sending the opponent away */
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
	{   5,     ZU | NO_VEC | NO_ZU,        A_SPECIAL,  K_L,    0,    4,   20,     0, 0x1d,   5,  0x60,       3, SC200,        8,   0,  50, 102,    0,                       0,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 30: pl16_at_bone, Bone-merang: Marrow and the thrown bone (shl16_00) */
	{   4,   BU_O | NO_VEC | NO_ZU,        A_SPECIAL, K_LM,    0,    1,   20,     0, 0x02,   3,  0x60,       3, SC200,        8,   0,  50,  85,    0,     NO_CHIP | NO_WEIGHT,  1,     0,  4,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 31: pl16_at_spine, Towering Spine */
	{   4,   BU_O | NO_VEC | NO_ZU,        A_SPECIAL, K_LL,    0,    1,   20,     0, 0x02,   3,  0x60,       3, SC200,        8,   0,  50,  88,   89,     NO_CHIP | NO_WEIGHT,  1,     0,  7,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 32 */
	{   4,   BU_O | NO_VEC | NO_ZU,        A_SPECIAL, K_LL,    0,    1,   30,     0, 0x02,   3,  0x80,       3, SC300,        8,   0,  50,  88,   89,               NO_WEIGHT,  1,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 33 */
	{   4,   BU_O | NO_VEC | NO_ZU,        A_SPECIAL,  K_L,    0,    1,   30,     0, 0x02,   3,  0x80,       3, SC300,        8,   0,  50,  88,   89,               NO_WEIGHT,  1,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 34 */
	{   6,   BU_O | NO_VEC | NO_ZU,        A_SPECIAL, K_LL,    0,    8,   30,     0, 0x00,  12,  0x80,       3, SC300,        8,   0,  50,  88,   89,               NO_WEIGHT,  1,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 35 */
	{   5,             ZU | NO_VEC,        A_SPECIAL,  K_L,    0,    6,   30,     0, 0x1e,   8,  0x80,       3, SC300,        8,   0, 100,  90,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        5, { 0, 0 } },	/* 36 */
	{   5,             ZU | NO_VEC,        A_SPECIAL,  K_L,    0,    6,   30,     0, 0x1e,   8,  0x80,       3, SC300,        8,   0, 100,  90,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        5, { 0, 0 } },	/* 37 */
	{   5,              2 | NO_VEC,        A_SPECIAL,  K_L,    0,    6,   30,     0, 0x10,  10,  0x80,       3, SC300,        8,   0, 100,  90,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        5, { 0, 0 } },	/* 38 */
	{   6,             ZU | NO_VEC,        A_SPECIAL,  K_L,    0,    6,   30,     0, 0x1e,   8,  0x80,       3, SC300,        8,   0,  90,  90,    0,                       0,  1,     0,  0,     0,    30,   0,        5, { 0, 0 } },	/* 39 */
	{   6,             ZU | NO_VEC,        A_SPECIAL,  K_L,    0,    6,   30,     0, 0x1e,   8,  0x80,       3, SC300,        8,   0,  90,  90,    0,                       0,  1,     0,  0,     0,    30,   0,        5, { 0, 0 } },	/* 3a */
	{   6,              2 | NO_VEC,        A_SPECIAL,  K_L,    0,    6,   30,     0, 0x10,  10,  0x80,       3, SC300,        8,   0,  90,  90,    0,                       0,  1,     0,  0,     0,    30,   0,        5, { 0, 0 } },	/* 3b */
	{   3,             ZU | NO_VEC,        A_SPECIAL,  K_L,    0,    6,   30,     0, 0x1e,   8,  0x80,       3, SC300,        8,   0,  90,  90,    0,                       0,  1,     0,  0,     0,    30,   0,        5, { 0, 0 } },	/* 3c */
	{   3,              2 | NO_VEC,        A_SPECIAL,  K_L,    0,    6,   30,     0, 0x1c,  10,  0x80,       3, SC300,        8,   0,  90,  90,    0,                       0,  1,     0,  0,     0,    30,   0,        5, { 0, 0 } },	/* 3d */
	{   4,   BU_O | NO_VEC | NO_ZU,          A_HYPER,  K_L,    0,    4,   30,     0, 0x00,   2,  0x80,       3, SC300,        8,   0,  90, 103,    0,                       0,  1,     0,  0,     0,    30,   4,        3, { 0, 0 } },	/* 3e: pl16_at_burst, Bone Burst (hyper), ground and air, and the thorns (shl16_01) */
	{   4,           BU_O | NO_VEC,          A_HYPER,  K_L,    0,    4,   20,     0, 0x00,   2,  0x60,       3, SC200,        8,   0,  90, 103,    0,                 NO_CHIP,  1,     0,  0,     0,     0,   4,        3, { 0, 0 } },	/* 3f */
	{   4,   BU_O | NO_VEC | NO_ZU,          A_HYPER, K_LL,    0,    4,   30,     0, 0x00,   2,  0x60,       3, SC200,        8,   0,  90, 103,  161,                 NO_CHIP,  1,     0,  0,     0,     0,   4,        3, { 0, 0 } },	/* 40 */
	{   4,   BU_O | NO_VEC | NO_ZU,          A_HYPER, K_LM,    0,    4,   20,     0, 0x00,   2,  0x60,       3, SC200,        8,   0,  90, 103,   57,                 NO_CHIP,  1,     0,  0,     0,     0,   4,        3, { 0, 0 } },	/* 41 */
	{   4,   BU_O | NO_VEC | NO_ZU,          A_HYPER,  K_L,    0,    4,   20,     0, 0x00,   2,  0x60,       3, SC200,        8,   0,  90, 103,    0,                 NO_CHIP,  1,     0,  0,     0,     0,   4,        3, { 0, 0 } },	/* 42 */
	{   5,             ZU | NO_VEC,          A_HYPER, K_LL,    0,    3,   20,     0, 0x18,  10,  0x60,       3, SC200,        8,   0,  90,  92,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,  13,        3, { 0, 0 } },	/* 43: pl16_at_stinger, Stinger Bones (hyper) */
	{   5,             ZU | NO_VEC,          A_HYPER, K_LL,    0,    3,   20,     0, 0x18,  10,  0x60,       3, SC200,        8,   0,  90,  95,   97,               NO_WEIGHT,  1,     0,  0,     0,     0,  13,        3, { 0, 0 } },	/* 44 */
	{   5,             ZU | NO_VEC,          A_HYPER, K_LS,    0,    3,   20,     0, 0x00,   0,  0x60,       3, SC200,        8,   0,  90,  94,   96,               NO_WEIGHT,  1,     0,  0,     0,     0,  13,        3, { 0, 0 } },	/* 45 */
	{  22, 2 | NO_VEC | FLY_SCREEN,          A_HYPER, K_LL,    0,   18,   20,     0, 0x1a,  15,  0x60,       3, SC200,        8,   0,  90, 100,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,  13,        3, { 0, 0 } },	/* 46 */
	{   0,     ZU | NO_VEC | NO_ZU,        A_SPECIAL, K_LL,    0,    0,   20,     0, 0x1e,  11,  0x60,       3, SC200,        8,   0,  90,  91,    0,                NO_COMBO,  1,     0,  7,     0,     0,   0,        5, { 0, 0 } },	/* 47: pl16_at_ride, Ride and Slash: the leap that catches, ground and air */
	{  10,             10 | NO_VEC,        A_SPECIAL,  K_L,    0,   10,   10,     0, 0x1f,  10,  0x60,       3, SC200,       12,   0,  90, 101,    0, NO_CHIP | NO_WEIGHT | AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 48: pl16_at_t_kick, Tag-in kick that leaps off the wall */
	{  10,             12 | NO_VEC,        A_SPECIAL,  K_L,    1,    4,   24,     0, 0x1c,  12,  0x80,       3, SC300,        8,  64, 100,  67,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 49: pl16_at_snapback, Snapback (sp_snapback, pl_snap_flash) */
	{  10,            BU_O | NO_ZU,        A_SPECIAL,  K_L,    0,    4,   20,     0, 0x1a,   8,  0x60,       3, SC300, SD_NOR_L,   0,  90,  23,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 4a */
};
