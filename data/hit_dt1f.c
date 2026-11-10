/* B.B. Hood (character 1f): the attacks (HIT_DATA, 0x1c bytes).
   What each hit does. The character's module sets the attack's number (atck_no) with the move,
   and the row is read when the attack box meets a body. After a row is its number, in
   hexadecimal, and the program's name for it:
   ATCK_ for the normals every character has (player.h), pl1f_at_ for its own (pl1f_def.h).
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
     box, box2  boxes of hit_1f that the computer measures the attack's reach by
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

const HIT_DATA hit_dt_1f[] = {
	/* dmg                   type            kind      elem  piyo  stop  sand  sleep   vec  pow  range     mark  score        se  shc  jug  box  box2                     flag  13  guard  15  catch  abyss  cnt    answer        1a */
	{   4,                    ZU,          A_NOM,      K_S,    4,   10,   10,     0, 0x18,   4,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  11,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 00: ATCK_STAND_LP */
	{   7,                    ZU,          A_NOM,     K_LM,    2,   11,   16,     0, 0x00,   6,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40,  13,   96,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 01: ATCK_STAND_MP */
	{   2,                    ZU,          A_NOM,      K_S,    1,    2,   24,     0, 0x1a,   8,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  15,  52,    0,                       0,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 02: ATCK_STAND_HP */
	{   4,                    ZU,          A_NOM,      K_S,    4,   10,   10,     0, 0x18,   4,  0x60, MARK_N1, SC100, SD_KIK_S,   0,  20,  20,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 03: ATCK_STAND_LK */
	{   4, BU_O | NO_VEC | NO_ZU,          A_NOM,      K_L,    2,   10,   16,     0, 0x00,  11,  0x60, MARK_N2, SC200, SD_KIK_M,   0,  40,  22,    0, NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 04: ATCK_STAND_MK */
	{  14,         BU_O | NO_VEC,          A_NOM,  K_L | 2,    1,   12,   24,     0, 0x1e,  11,  0x80, MARK_N3, SC300,       21,   0,  50,  60,    0,                       0,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 05: ATCK_STAND_HK */
	{   4,                    ZU,          A_NOM,     K_LS,    4,   10,   10,     0, 0x1c,   4,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  26,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 06: ATCK_CROUCH_LP */
	{   7,                    ZU,          A_NOM,     K_LM,    2,   11,   16,     0, 0x13,   6,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40,  28,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 07: ATCK_CROUCH_MP */
	{   2,                    ZU,          A_NOM,     K_LS,    1,    2,   24,     0, 0x18,   6,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  15,  53,    0,                       0,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 08: ATCK_CROUCH_HP */
	{   4,                    ZU,           A_LO,     K_LS,    4,   10,   10,     0, 0x1c,   4,  0x70, MARK_N1, SC100, SD_KIK_S,   0,  20,  31,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 09: ATCK_CROUCH_LK */
	{   7,                    ZU,           A_LO,     K_LM,    2,   11,   16,     0, 0x1d,   6,  0x40, MARK_N2, SC200, SD_KIK_M,   0,  40,  33,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0a: ATCK_CROUCH_MK */
	{  13,                    AS,           A_LO,     K_LL,    1,   12,   24,     9, 0x1a,   8,  0x70, MARK_N3, SC300, SD_KIK_L,   0, 100,  35,    0,                       0,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 0b: ATCK_CROUCH_HK */
	{   4,           ZU | NO_VEC,           A_HI,      K_S,    1,   10,   10,     0, 0x00,   5,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  38,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0c: ATCK_JUMP_LP */
	{   7,           ZU | NO_VEC,           A_HI,     K_LM,    1,   10,   16,     0, 0x00,   5,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40,  40,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0d: ATCK_JUMP_MP */
	{   6,           ZU | NO_VEC,           A_HI,     K_LM,    1,    3,   32,     0, 0x16,   8,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  60,  54,    0,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 0e: ATCK_JUMP_HP */
	{   4,           ZU | NO_VEC,           A_HI,      K_S,    1,   10,   10,     0, 0x00,   5,  0x60, MARK_N1, SC200, SD_KIK_S,   0,  20,  46,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0f: ATCK_JUMP_LK */
	{   7,           ZU | NO_VEC,           A_HI,      K_M,    1,   10,   16,     0, 0x1f,   5,  0x60, MARK_N2, SC200, SD_KIK_M,   0,  40,  48,    0,               NO_WEIGHT,  0,     1,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 10: ATCK_JUMP_MK */
	{  14,         BU_O | NO_VEC,           A_HI,  K_S | 2,    1,    3,   32,     0, 0x1c,  11,  0x60, MARK_N3, SC200,       21,   0,  50,  60,    0,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 11: ATCK_JUMP_HK */
	{  14,           ZU | NO_VEC,          A_NOM,     K_LM,    2,   10,   16,     0, 0x18,   6,  0x60, MARK_N2, SC200, SD_NOR_M,   0,  40,  56,    0,                       0,  0,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 12: pl1f_at_fwd_hp, Forward or back + HP standing: a hop attack */
	{  12,           ZU | NO_VEC,          A_NOM,     K_LL,    1,   12,   16,     0, 0x18,  10,  0x60, MARK_N2, SC200, SD_KIK_M,   0,  40,  58,    0,                       0,  0,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 13 */
	{  12,         BU_O | NO_VEC,          A_NOM,  K_M | 2,    1,   12,   16,     0, 0x1d,  11,  0x60, MARK_N2, SC200,       21,   0,  50,  60,    0,                       0,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 14 */
	{  12,            2 | NO_VEC,          A_NOM,      K_M,    1,   10,   16,     0, 0x1a,  10,  0x60,      14, SC200,       15,   0,  40,  66,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 15: pl1f_at_dash_hp, Forward or back + HP during the dash (nml_atck_dash) */
	{ 0 },	/* 16 */
	{ 0 },	/* 17 */
	{ 0 },	/* 18 */
	{   5,            ZU | NO_ZU,          A_NOM,      K_S,    1,    2,   24,     0, 0x1a,   8,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  15,  52,    0,                       0,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 19: pl1f_at_stand_hp_2, Standing HP, second hit (cg_type 2) */
	{   5,            ZU | NO_ZU,          A_NOM,     K_LS,    1,    2,   24,     0, 0x00,  11,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  15,  54,    0,                       0,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 1a: pl1f_at_crouch_hp_2, Crouching HP, second hit (cg_type 2) */
	{ 0 },	/* 1b */
	{ 0 },	/* 1c */
	{ 0 },	/* 1d */
	{ 0 },	/* 1e */
	{ 0 },	/* 1f */
	{  16,                  BU_O,   A_NOM | 0x40,     K_LM,    4,   12,   20,     0, 0x04,  11,  0x60, MARK_N2, SC200, SD_NOR_M,   0,  40,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 20: pl1f_at_throw, Her only normal throw (punch, ground or air), set on the opponent */
	{  16,                     2,   A_NOM | 0x40,     K_LM,    4,   12,   20,     0, 0x09,  11,  0x60, MARK_N2, SC200, SD_NOR_M,   0,  40,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 21 */
	{ 0 },	/* 22 */
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
	{  14,   ZU | NO_VEC | NO_ZU,      A_SPECIAL,  K_M | 2,    0,    6,   20,     0, 0x1e,   8,  0x60,       3, SC200,       21,   0,  50,  61,    0,                       0,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 30: pl1f_at_missile, Smile & Missile, all three versions (sp_missile, sp_happy, sp_missile_j): her own record as she fires */
	{  14,    2 | NO_VEC | NO_ZU,      A_SPECIAL,  K_L | 2,    0,    6,   20,     0, 0x1e,  10,  0x60,       3, SC200,       21,   0,  50,  61,    0,                       0,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 31 */
	{   4,         BU_O | NO_VEC,      A_SPECIAL,  K_M | 2,    0,    6,   20,     0, 0x1d,   4,  0x60,       3, SC200,        8,   0,   5,  68,   69,                       0,  1,     0,  0,     0,     0,   2,        5, { 0, 0 } },	/* 32: pl1f_at_cheer, Cheer & Fire, punch (angled) version */
	{   4,         BU_O | NO_VEC,      A_SPECIAL,  K_L | 2,    0,    6,   30,     0, 0x1d,   4,  0x80,       3, SC300,        8,   0,   5,  68,   69,                       0,  1,     0,  0,     0,     0,   2,        5, { 0, 0 } },	/* 33 */
	{  16,                  BU_O,      A_SPECIAL,      K_L,    0,    8,   30,     0, 0x1d,   8,  0x80,       3, SC300,        8,   0,  50,  74,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 34: pl1f_at_shyness, Shyness & Strike */
	{  16,                  BU_O,      A_SPECIAL,     K_LL,    0,   10,   30,     0, 0x1e,  11,  0x80,       3, SC300,        8,   0,  50,  75,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 35 */
	{   2,            21 | NO_ZU,        A_HYPER,  K_L | 2,    0,    8,   30,     0, 0x1e,  11,  0x80,       3, SC300,       21,   0,  90,  62,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        8, { 0, 0 } },	/* 36 */
	{   2,            21 | NO_ZU,        A_HYPER,  K_L | 2,    0,   10,   30,     0, 0x1e,  11,  0x80,       3, SC300,       21,   0,  90,  62,    0,     NO_CHIP | NO_WEIGHT,  1,     0,  0,     0,     0,   0,        8, { 0, 0 } },	/* 37 */
	{   3,            21 | NO_ZU,        A_HYPER,  K_L | 2,    0,    8,   30,     0, 0x1e,  11,  0x80,       3, SC300,       21,   0,  90,  70,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        8, { 0, 0 } },	/* 38: pl1f_at_hunting, Cool Hunting hyper */
	{   4,            ZU | NO_ZU, A_HYPER | 0x80,      K_L,    0,    0,   30,     0, 0x1c,  10,  0x80,      10, SC300,       16,   0,  90,  71,    0,        NO_COMBO | NO_KO,  1,     0,  0,     1,    30,   0,        6, { 0, 0 } },	/* 39: pl1f_at_memory, Beautiful Memory hyper, the first hit that grabs */
	{   2,          BU_O | NO_ZU, A_HYPER | 0x40,      K_L,    0,   10,   30,     0, 0x1c,  10,  0x80,      10, SC300,       16,   0,  90,   0,    0,                   NO_KO,  1,     0,  0,     0,    30,   0,        3, { 0, 0 } },	/* 3a: pl1f_at_memory_hit_1, Beautiful Memory, hits of the first round on the held opponent */
	{   2,          BU_O | NO_ZU, A_HYPER | 0x40,      K_L,    0,   10,   30,     0, 0x1c,  10,  0x80,       3, SC300,       12,   0,  90,   0,    0,                   NO_KO,  1,     0,  0,     0,    30,   0,        3, { 0, 0 } },	/* 3b: pl1f_at_memory_hit_2, Beautiful Memory, hits of the second round */
	{  30,            2 | NO_VEC, A_HYPER | 0x40,        0,    0,   10,   30,     0, 0x1c,  11,  0x80,       3, SC300,       12,   0,  90,   0,    0,                       0,  1,     0,  0,     0,    30,   0,        3, { 0, 0 } },	/* 3c: pl1f_at_memory_last, Beautiful Memory, the last hit on landing (her and the opponent) */
	{   4, BU_O | NO_VEC | NO_ZU, A_HYPER | 0x40,      K_L,    0,   12,   30,     0, 0x16,   8,  0x80,       3, SC300,       12,   0,  90,  72,    0,                   NO_KO,  1,     0,  0,     0,    30,   0,        6, { 0, 0 } },	/* 3d: pl1f_at_apple, Apple for You hyper: the dash and the catch (her and the opponent) */
	{   4, BU_O | NO_VEC | NO_ZU, A_HYPER | 0x40,      K_L,    0,   10,   30,     0, 0x16,   8,  0x80,       3, SC300,       12,   0,  90,   0,    0,                   NO_KO,  1,     0,  0,     0,    30,   3,        3, { 0, 0 } },	/* 3e: pl1f_at_apple_hit, Apple for You, the hits on the held opponent (apple_hit_*) */
	{   3,   ZU | NO_VEC | NO_ZU, A_HYPER | 0x40,  K_L | 2,    0,    6,   20,     0, 0x1a,   8,  0x60,       3, SC200,       21,   5,  50,   0,    0,                       0,  1,     0,  0,     0,     0,  10,        3, { 0, 0 } },	/* 3f */
	{   4,            2 | NO_VEC,        A_HYPER, K_LL | 2,    0,   10,   30,     0, 0x06,   2,  0x60,       3, SC200,       21,   0,  90,  73,  161,                       0,  1,     0,  4,     0,     0,  10,        4, { 0, 0 } },	/* 40: pl1f_at_apple_scrap, Apple for You, the falling scraps of a broken dish (shl1f_03 type 1) */
	{  10,           12 | NO_VEC,      A_SPECIAL,      K_L,    0,    4,   24,     0, 0x1c,  12,  0x80,       9, SC300,       16,  64,  90,  66,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0,        7, { 0, 0 } },	/* 41: pl1f_at_snapback, Snapback (sp_snapback) */
	{  15, BU_O | NO_VEC | NO_ZU,      A_SPECIAL,      K_L,    0,   10,   24,     0, 0x00,  11,  0x60, MARK_N3, SC300, SD_KIK_L,   0,  40,  22,    0, NO_CHIP | NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 42: pl1f_at_upper, Launcher */
	{  10,           10 | NO_VEC,      A_SPECIAL,      K_L,    0,   10,   10,     0, 0x1f,  10,  0x60,       3, SC200,       12,   0,  90,  76,    0, NO_CHIP | NO_WEIGHT | AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        7, { 0, 0 } },	/* 43: pl1f_at_t_kick, the partner's jumping kick as she comes in */
	{   3,            21 | NO_ZU,        A_HYPER,  K_L | 2,    0,    8,   30,     0, 0x1e,  11,  0x80,       3, SC300,       21,   0,  90,  70,    0,     NO_CHIP | NO_WEIGHT,  1,     0,  0,     0,     0,   0,        8, { 0, 0 } },	/* 44 */
	{ 0 },	/* 45 */
	{ 0 },	/* 46 */
	{   4,            2 | NO_VEC, A_HYPER | 0x40, K_LL | 2,    0,   10,   30,     0, 0x1e,  11,  0x80,       3, SC300,       21,  16,  90,   0,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,  10,        3, { 0, 0 } },	/* 47: pl1f_at_apple_dish, Apple for You, each dish that hits the held opponent */
	{   4,         BU_O | NO_VEC,      A_SPECIAL,  K_M | 2,    0,    6,   20,     0, 0x1d,   4,  0x60,       3, SC200,        8,   0,  12,  77,   78,                       0,  1,     0,  7,     0,     0,   3,        4, { 0, 0 } },	/* 48: pl1f_at_cheer_k, Cheer & Fire, kick (horizontal, also in the air) version */
	{   4,         BU_O | NO_VEC,      A_SPECIAL,  K_L | 2,    0,    6,   30,     0, 0x1d,   4,  0x80,       3, SC300,        8,   0,  12,  77,   78,                       0,  1,     0,  0,     0,     0,   3,        4, { 0, 0 } },	/* 49 */
	{   4,            2 | NO_VEC, A_HYPER | 0x40, K_LL | 2,    0,   10,   30,     0, 0x1e,  11,  0x80,       3, SC300,       21,  16,  90,   0,    0,               NO_WEIGHT,  1,     0,  7,     0,     0,  10,        3, { 0, 0 } },	/* 4a: pl1f_at_apple_last, Apple for You, the last dish */
	{   4,         BU_O | NO_VEC,      A_SPECIAL,  K_M | 2,    0,    6,   20,     0, 0x1d,   8,  0x60,       3, SC200,        8,   0,  90,  68,   69,                       0,  1,     0,  7,     3,     0,   0,        4, { 0, 0 } },	/* 4b: pl1f_at_cheer_assist, Cheer & Fire's flame (shl1f_00 spark), with a hit only as a partner's attack (team_type 8) */
};
