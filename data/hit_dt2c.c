/* Magneto (character 2c): the attacks (HIT_DATA, 0x1c bytes).
   What each hit does. The character's module sets the attack's number (atck_no) with the move,
   and the row is read when the attack box meets a body. After a row is its number, in
   hexadecimal, and the program's name for it:
   ATCK_ for the normals every character has (player.h), pl2c_at_ for its own (pl2c_def.h).
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
     box, box2  boxes of hit_2c that the computer measures the attack's reach by
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

const HIT_DATA hit_dt_2c[] = {
	/* dmg                   type              kind  elem  piyo  stop  sand  sleep   vec  pow  range     mark  score        se  shc  jug  box  box2                     flag  13  guard  15  catch  abyss  cnt    answer        1a */
	{   4,           ZU | NO_VEC,            A_NOM,  K_S,    4,   10,   10,     0, 0x18,   7,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  17,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 00: ATCK_STAND_LP */
	{   7,         BU_O | NO_VEC,            A_NOM,  K_M,    2,   10,   20,     0, 0x1f,  10,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40,  94,   21, NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 01: ATCK_STAND_MP */
	{  13,                  BU_O,            A_NOM,  K_L,    1,   10,   30,     0, 0x1a,  15,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  50,  26,  115,                       0,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 02: ATCK_STAND_HP */
	{   4,           ZU | NO_VEC,            A_NOM,  K_S,    4,   10,   10,     0, 0x18,   7,  0x60, MARK_N1, SC100, SD_KIK_S,   0,  20,  35,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 03: ATCK_STAND_LK */
	{   7,                    ZU,            A_NOM, K_LM,    2,   10,   20,     0, 0x1a,   9,  0x60, MARK_N2, SC200, SD_KIK_M,   0,  40,  35,   36,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 04: ATCK_STAND_MK */
	{   9,                  BU_O,            A_NOM,  K_L,    1,   10,   24,     0, 0x00,   5,  0x80, MARK_N3, SC300, SD_KIK_L,   0,  50,  40,   44,                       0,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 05: ATCK_STAND_HK */
	{   4,           ZU | NO_VEC,            A_NOM, K_LS,    4,   10,   10,     0, 0x1c,   8,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  45,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 06: ATCK_CROUCH_LP */
	{   7, BU_O | NO_VEC | NO_ZU,            A_NOM, K_LM,    2,   10,   20,     0, 0x1d,   8,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40,  95,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 07: ATCK_CROUCH_MP */
	{  13,         BU_O | NO_VEC,            A_NOM, K_LL,    1,   10,   24,     0, 0x00,  11,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  50,  52,    0, NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 08: ATCK_CROUCH_HP */
	{   4,           ZU | NO_VEC,             A_LO, K_LS,    4,   10,   10,     0, 0x1c,  10,  0x70, MARK_N1, SC100, SD_KIK_S,   0,  20,  56,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 09: ATCK_CROUCH_LK */
	{   7,                    ZU,             A_LO, K_LM,    2,   10,   20,     0, 0x18,  11,  0x40, MARK_N2, SC200, SD_KIK_M,   0,  40,  98,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0a: ATCK_CROUCH_MK */
	{  13,            AS | NO_ZU,             A_LO, K_LL,    1,   10,   30,     9, 0x02,   8,  0x70, MARK_N3, SC300, SD_KIK_L,   0,  90,  60,    0,                       0,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 0b: ATCK_CROUCH_HK */
	{   4,           ZU | NO_VEC,             A_HI,  K_S,    1,   10,   14,     0, 0x00,   5,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20, 108,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0c: ATCK_JUMP_LP */
	{   7,           ZU | NO_VEC,             A_HI, K_LM,    1,   10,   20,     6, 0x00,   5,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40,  64,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0d: ATCK_JUMP_MP */
	{  13,           ZU | NO_VEC,             A_HI, K_LM,    1,   10,   30,     0, 0x14,   7,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  60,  66,    0,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 0e: ATCK_JUMP_HP */
	{   4,           ZU | NO_VEC,             A_HI,  K_S,    1,   10,   14,     0, 0x00,   5,  0x60, MARK_N1, SC100, SD_KIK_S,   0,  20,  70,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0f: ATCK_JUMP_LK */
	{   7,           ZU | NO_VEC,             A_HI,  K_M,    1,   10,   20,     0, 0x00,   5,  0x60, MARK_N2, SC200, SD_KIK_M,   0,  40,  72,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 10: ATCK_JUMP_MK */
	{  13,           ZU | NO_VEC,             A_HI,  K_M,    1,   10,   30,     0, 0x16,   8,  0x80, MARK_N3, SC300, SD_KIK_L,   0,  60,  74,    0,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 11: ATCK_JUMP_HK */
	{   4,                    ZU,             A_HI,  K_S,    1,   10,   14,     0, 0x1a,   2,  0x60, MARK_N1, SC100, SD_KIK_S,   0,  20,  77,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 12: pl2c_at_jdown_lk, Air light kick with down: the dive */
	{ 0 },	/* 13 */
	{ 0 },	/* 14 */
	{ 0 },	/* 15 */
	{ 0 },	/* 16 */
	{ 0 },	/* 17 */
	{ 0 },	/* 18 */
	{   4,                  BU_O,            A_NOM,  K_L,    1,   10,   24,     0, 0x1e,  11,  0x80, MARK_N3, SC300, SD_KIK_L,   2,  50,  44,    0,             BLACK_COMBO,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 19: pl2c_at_stand_hk_2, Second hit of the standing heavy kick */
	{ 0 },	/* 1a */
	{ 0 },	/* 1b */
	{ 0 },	/* 1c */
	{ 0 },	/* 1d */
	{ 0 },	/* 1e */
	{ 0 },	/* 1f */
	{   8,             2 | NO_ZU,     A_NOM | 0x40,    4,    0,   16,    0,    15, 0x18,   7,  0x80,       3, SC300, SD_NOR_S,   0,   0,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 20 */
	{   8,             2 | NO_ZU,     A_NOM | 0x40,    4,    0,   16,    0,    15, 0x19,   7,  0x80,       3, SC300, SD_NOR_S,   0,   0,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 21 */
	{  16,             2 | NO_ZU,     A_NOM | 0x40,    0,    4,   16,    0,    15, 0x19,  15,  0x80,       3, SC300, SD_NOR_S,   0,   0,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 22 */
	{  16,             2 | NO_ZU,     A_NOM | 0x40,    0,    4,   16,    0,    15, 0x19,  15,  0x80,       3, SC300, SD_NOR_S,   0,   0,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 23 */
	{  16,             2 | NO_ZU,     A_NOM | 0x40,    0,    8,   16,    0,    15, 0x10,  15,  0x80,       3, SC300, SD_NOR_S,   0,   0,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 24 */
	{   8,          BU_O | NO_ZU, A_SPECIAL | 0x40,    0,    4,   16,   12,    15, 0x1b,   7,  0x80,       3, SC300, SD_NOR_S,   0,   0,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 25: pl2c_at_grav_free, Hyper Grav magnet lets the held opponent go (shl2c_03_release) */
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
	{  14,          BU_O | NO_ZU, A_SPECIAL | 0x80,  K_L,    0,   10,   30,     0, 0x1b,   7,  0x80,       4, SC300,       18,   0,  50,  87,   92,                       0,  1,     0,  0,     0,     0,   3,        8, { 0, 0 } },	/* 30: pl2c_at_disrupt, EM Disruptor, light (him and the spark, shl2c_00 type 2); +2 for the heavy one */
	{  16,          BU_O | NO_ZU, A_SPECIAL | 0x80,  K_L,    0,   10,   30,     0, 0x1b,   7,  0x80,       4, SC300,       18,   0,  50,  87,   92,                       0,  1,     0,  0,     0,     0,   3,        8, { 0, 0 } },	/* 31 */
	{  18,          BU_O | NO_ZU, A_SPECIAL | 0x80,  K_L,    0,   10,   30,     0, 0x1b,   7,  0x80,       4, SC300,       18,   0,  50,  87,   92,                       0,  1,     0,  0,     0,     0,   3,        8, { 0, 0 } },	/* 32 */
	{   4,   ZU | NO_VEC | NO_ZU, A_SPECIAL | 0x80,    0,    0,    0,    0,     0, 0x12,   1,  0x80,       3, SC300, SD_NOR_L,   0,  50,  99,    0,                       0,  1,     0,  0,     1,     0,   0,        4, { 0, 0 } },	/* 33: pl2c_at_grav, Hyper Grav on the ground (him and the magnets, shl2c_03) */
	{   7, 2 | NO_VEC | NO_ZU | FLY_SCREEN,   A_HYPER | 0x80,  K_L,    0,    8,    0,    15, 0x19,  11,  0x80,       3, SC300, SD_NOR_S,   0,  90, 103,  104,               NO_WEIGHT,  1,     0,  0,     0,     0,  16,        4, { 0, 0 } },	/* 34: pl2c_at_shock, Magnetic Shockwave (hyper; him and shl2c_02's columns) */
	{   3,    2 | NO_VEC | NO_ZU,   A_HYPER | 0x80,  K_L,    0,   10,    0,    15, 0x1e,   2,  0x80,       3, SC300,        8,   0,  10, 100,    0,                       0,  1,     0,  0,     0,     0,   6,        4, { 0, 0 } },	/* 35: pl2c_at_tempest, Magnetic Tempest (hyper; him and the shl2c_01 sparks, 1 frame in 4) */
	{  14,            ZU | NO_ZU,        A_SPECIAL,  K_L,    0,   10,   24,     0, 0x16,   8,  0x80,       4, SC300,       17,   0,  50,  93,    0,                       0,  1,     0,  0,     0,     0,   3,        4, { 0, 0 } },	/* 36: pl2c_at_jblast, Magnetic Blast in the air, light (him and the shl2c_00 type 0 bolt); +2 for the heavy one */
	{  14,            ZU | NO_ZU,        A_SPECIAL,  K_L,    0,   10,   24,     0, 0x16,   8,  0x80,       4, SC300,       17,   0,  50,  93,    0,                       0,  1,     0,  0,     0,     0,   3,        4, { 0, 0 } },	/* 37 */
	{  14,            ZU | NO_ZU,        A_SPECIAL,  K_L,    0,   10,   24,     0, 0x16,   8,  0x80,       4, SC300,       17,   0,  50,  93,    0,                       0,  1,     0,  0,     0,     0,   3,        4, { 0, 0 } },	/* 38 */
	{   6,    2 | NO_VEC | NO_ZU,          A_HYPER,  K_M,    0,   10,    0,    15, 0x1e,   2,  0x80,       3, SC300,        8,   0,  10, 100,    0,                 NO_CHIP,  1,     0,  0,     0,     0,   3,        4, { 0, 0 } },	/* 39: pl2c_at_tempest_2, Magnetic Tempest's other spark hit (shl2c_01, 3 frames in 4) */
	{  20,    2 | NO_VEC | NO_ZU, A_SPECIAL | 0x80,  K_L,    0,    8,    0,    15, 0x1c,  13,  0x80,       3, SC300, SD_NOR_S,   0, 100, 119,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        7, { 0, 0 } },	/* 3a: pl2c_at_field, Magnetic Forcefield's wave after it catches (shl2c_02 types 3 and 4, from field_catch) */
	{   4,   ZU | NO_VEC | NO_ZU, A_SPECIAL | 0x80,    0,    0,    0,    0,     0, 0x12,   1,  0x80,       3, SC300, SD_NOR_L,   0,  50,  99,    0,                       0,  1,     0,  0,     1,     0,   0,        4, { 0, 0 } },	/* 3b: pl2c_at_jgrav, Hyper Grav's magnets in the air (shl2c_03) */
	{  16,                     2,        A_SPECIAL,    0,    0,   10,    0,     0, 0x1a,  12,  0x80,       3, SC300, SD_NOR_L,   0,  90, 124,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        7, { 0, 0 } },	/* 3c */
	{  10,           10 | NO_VEC,        A_SPECIAL,  K_L,    0,   10,   10,     0, 0x1f,  10,  0x60,       3, SC200,       12,   0,  90, 126,    0, NO_CHIP | NO_WEIGHT | AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        7, { 0, 0 } },	/* 3d: pl2c_at_t_kick, the partner's jump-in attack (t_kick) */
	{   9,    2 | NO_VEC | NO_ZU,   A_HYPER | 0x80,  K_L,    0,    8,    0,    15, 0x00,   7,  0x80,       3, SC300, SD_NOR_S,   0,  90, 103,  104,               NO_WEIGHT,  1,     0,  0,     0,     0,  16,        4, { 0, 0 } },	/* 3e */
	{  10,           12 | NO_VEC,        A_SPECIAL,  K_L,    0,    4,   24,     0, 0x1c,  12,  0x80,       3, SC300,        8,  64,  90,  26,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 3f: pl2c_at_snapback, Snapback (sp_snapback) */
	{  15,         BU_O | NO_VEC,        A_SPECIAL,  K_L,    0,   10,   16,     0, 0x1f,  12,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  90,  27,   28,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 40: pl2c_at_crouch_hp, Crouching heavy punch as gamma assist (sp_crouch_hp) */
	{  14,         BU_O | NO_VEC,        A_SPECIAL, K_LL,    0,   10,   24,     0, 0x00,  11,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  40,  52,    0, NO_CHIP | NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 41: pl2c_at_upper, the launcher (pl2c_upper) */
};
