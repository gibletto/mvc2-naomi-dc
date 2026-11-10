/* Zangief (character 01): the attacks (HIT_DATA, 0x1c bytes).
   What each hit does. The character's module sets the attack's number (atck_no) with the move,
   and the row is read when the attack box meets a body. After a row is its number, in
   hexadecimal, and the program's name for it:
   ATCK_ for the normals every character has (player.h), pl01_at_ for its own (pl01_def.h).
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
     box, box2  boxes of hit_01 that the computer measures the attack's reach by
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

const HIT_DATA hit_dt_01[] = {
	/* dmg                type              kind     elem  piyo  stop  sand  sleep   vec  pow  range     mark  score        se  shc  jug  box  box2                     flag  13  guard  15  catch  abyss  cnt    answer        1a */
	{   6,                 ZU,            A_NOM,     K_S,    4,   10,   10,     0, 0x1a,   4,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  16,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 00: ATCK_STAND_LP */
	{  10,                 ZU,            A_NOM,     K_M,    2,   11,   16,     0, 0x1a,   6,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40,  17,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 01: ATCK_STAND_MP */
	{  18,                 ZU,            A_NOM,     K_L,    1,   12,   24,     0, 0x1a,  10,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  60,  18,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 02: ATCK_STAND_HP */
	{   6,                 ZU,            A_NOM,    K_LS,    4,   10,   10,     0, 0x18,   4,  0x60, MARK_N1, SC100, SD_KIK_S,   0,  20,  22,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 03: ATCK_STAND_LK */
	{  10,                 ZU,            A_NOM,    K_LM,    2,   11,   16,     0, 0x1a,   6,  0x60, MARK_N2, SC200, SD_KIK_M,   0,  40,  23,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 04: ATCK_STAND_MK */
	{   9,      BU_O | NO_VEC,            A_NOM,    K_LL,    1,    8,   24,     0, 0x1f,   6,  0x80, MARK_N3, SC300, SD_KIK_L,   4,  60,  91,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 05: ATCK_STAND_HK */
	{   6,        ZU | NO_VEC,            A_NOM,    K_LS,    4,   10,   10,     0, 0x18,   4,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  19,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 06: ATCK_CROUCH_LP */
	{  10,      BU_O | NO_VEC,            A_NOM,    K_LM,    2,   10,   16,     0, 0x00,  11,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40,  53,   20, NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 07: ATCK_CROUCH_MP */
	{  18,                 ZU,            A_NOM,    K_LL,    1,   12,   24,     0, 0x1a,  10,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  60,  21,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 08: ATCK_CROUCH_HP */
	{   6,        ZU | NO_VEC,             A_LO,    K_LS,    4,   10,   10,     0, 0x18,   4,  0x70, MARK_N1, SC100, SD_KIK_S,   0,  20,  25,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 09: ATCK_CROUCH_LK */
	{  10,                 ZU,             A_LO,    K_LM,    2,   11,   16,     0, 0x18,   6,  0x40, MARK_N2, SC200, SD_KIK_M,   0,  40,  26,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0a: ATCK_CROUCH_MK */
	{  18,                 AS,             A_LO,    K_LL,    1,   12,   24,     9, 0x18,   8,  0x70, MARK_N3, SC300, SD_KIK_L,   0,  90,  27,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0b: ATCK_CROUCH_HK */
	{   6,        ZU | NO_VEC,             A_HI,     K_S,    1,   10,   10,     0, 0x00,   3,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  28,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0c: ATCK_JUMP_LP */
	{  10,        ZU | NO_VEC,             A_HI,     K_M,    1,   10,   16,     0, 0x00,   4,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40,  29,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0d: ATCK_JUMP_MP */
	{  16,        ZU | NO_VEC,             A_HI,     K_M,    1,   12,   32,     0, 0x14,  12,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  60,  30,    0,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0e: ATCK_JUMP_HP */
	{   6,        ZU | NO_VEC,             A_HI,     K_S,    1,   10,   10,     0, 0x00,   4,  0x60, MARK_N1, SC100, SD_KIK_S,   0,  20,  34,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0f: ATCK_JUMP_LK */
	{  10,        ZU | NO_VEC,             A_HI,     K_M,    1,   10,   16,     0, 0x00,   4,  0x60, MARK_N2, SC200, SD_KIK_M,   0,  40,  35,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 10: ATCK_JUMP_MK */
	{  18,        ZU | NO_VEC,             A_HI,     K_M,    1,   12,   32,     0, 0x18,  10,  0x80, MARK_N3, SC300, SD_KIK_L,   0,  60,  36,    0,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 11: ATCK_JUMP_HK */
	{  16,        ZU | NO_VEC,             A_HI,     K_M,    1,   12,   32,     0, 0x18,  12,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  60,  31,    0,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 12: pl01_at_jfwd_hp, Jump HP with the stick forward */
	{  10,        ZU | NO_VEC,             A_HI,    K_LM,    1,   10,   24,     0, 0x18,   8,  0x80, MARK_N2, SC300, SD_NOR_M,   0,  40,  33,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 13: pl01_at_jup_mp, Jump MP with the stick up (ends on landing) */
	{  18,        ZU | NO_VEC,             A_HI,     K_M,    1,   12,   32,     0, 0x16,   9,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  60,  32,    0,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 14: pl01_at_jdown_hp, Jump HP with the stick down (ends on landing) */
	{  10,         2 | NO_VEC,            A_NOM,    K_LM,    1,   10,   16,    10, 0x14,   4,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  60,  49,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 15: pl01_at_jdown_lp, Jump LP with the stick down: the dive to the floor */
	{  10,        ZU | NO_VEC,             A_HI,    K_LM,    1,   10,   16,     0, 0x1d,   2,  0x60, MARK_N2, SC200, SD_KIK_M,   0,  40,  37,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 16: pl01_at_jdown_lk, Jump LK with the stick down (ends on landing) */
	{  10,                  2,            A_NOM,    K_LM,    2,   11,   16,    16, 0x16,   6,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  60,  10,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 17: pl01_at_back_lp, Stand LP with the stick back (step, screen shake, turns round) */
	{  10,      BU_O | NO_VEC,        A_SPECIAL,    K_LM,    2,   10,   16,     0, 0x00,  11,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40,  53,   20, NO_CHIP | NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 18: pl01_at_upper, the launcher (pl01_upper) */
	{   9,                 ZU,            A_NOM,     K_L,    1,   11,   24,     0, 0x1f,   8,  0x80, MARK_N3, SC300, SD_KIK_L,   4,  60,  24,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 19: pl01_at_stand_hk_2, Stand HK, second hit on the animation's mark */
	{ 0 },	/* 1a */
	{ 0 },	/* 1b */
	{ 0 },	/* 1c */
	{ 0 },	/* 1d */
	{ 0 },	/* 1e */
	{ 0 },	/* 1f */
	{  16,                  2,     A_NOM | 0x40,       0,    4,    4,    0,    15, 0x1c,   4,  0x80, MARK_N1, SC300,       76,   0,   0,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 20: pl01_at_throw_p, Punch throw; also the punch that ends the running grab (dash_end) */
	{  16,                  2,     A_NOM | 0x40,       0,    4,   16,    0,    15, 0x0d,  15,  0x80, MARK_N1, SC300,       76,   0,   0,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 21: pl01_at_air_throw, Air throw (punch) */
	{  16,      BU_O | NO_VEC,     A_NOM | 0x40,       0,    4,    8,   30,     0, 0x00,  11,  0x80, MARK_N1, SC300, SD_NOR_S,   0,   0,   0,    0, NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 22: pl01_at_com_throw, the command throw (catch_com_ck sets catch_no 9) */
	{   6,                  2,     A_NOM | 0x40,       0,    1,    4,    0,    15, 0x1a,   4,  0x80, MARK_N1, SC300,       76,   2,   0,   0,    0,                 NO_CHIP,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 23: pl01_at_throw_k, Kick throw: the hold's mashable hits and the throw */
	{   8,                  2,     A_NOM | 0x40,       0,    1,    4,    0,    15, 0x1a,   4,  0x80, MARK_N1, SC300,       76,   2,   0,   0,    0,                 NO_CHIP,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 24: pl01_at_crouch_throw_k, Crouching kick throw (hold and throw) */
	{  30,                  2, A_SPECIAL | 0x40,       0,    0,    4,    0,    15, 0x1d,   0,  0x80, MARK_N1, SC300,       76,   0,   0,   0,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 25: pl01_at_jscrew, Spinning Pile Driver in the air (screw_air) */
	{  34,                  2, A_SPECIAL | 0x40,       0,    0,    4,    0,    15, 0x1d,   0,  0x80, MARK_N1, SC300,       76,   0,   0,   0,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 26 */
	{   0,               BU_O,     A_NOM | 0x40,       0,    0,    4,    0,    15, 0x1d,   0,  0x80, MARK_N1, SC300,       76,   0,   0,   0,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 27: pl01_at_dash_throw, Running grab's release when its timer runs out */
	{  10,       BU_O | NO_ZU,          A_HYPER,     K_M,    0,   10,   20,     0, 0x00,   1,  0x80,       3, SC300,        8,   0,  90, 112,    0,               NO_WEIGHT,  1,     0, 34,     0,     0,   0,        3, { 0, 0 } },	/* 28: pl01_at_blizzard, Siberian Blizzard (Mecha Zangief's rising lariat hyper) */
	{   4,       BU_O | NO_ZU,          A_HYPER,     K_L,    0,   10,   20,     0, 0x00,   0,  0x80,       3, SC300,        8,   0,  90, 112,    0,               NO_WEIGHT,  1,     0, 34,     0,     0,   0,        3, { 0, 0 } },	/* 29 */
	{  10,       BU_O | NO_ZU,          A_HYPER,     K_L,    0,   10,   20,     0, 0x1c,  12,  0x80,       3, SC300,        8,   0,  90, 112,    0,               NO_WEIGHT,  1,     0, 34,     0,     0,   0,        3, { 0, 0 } },	/* 2a */
	{   3, 2 | NO_VEC | NO_ZU,        A_SPECIAL, K_L | 6,    0,    6,   20,     0, 0x1c,   5,  0x80,       3, SC300, SD_NOR_L,   2,  90, 110,  111,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 2b: pl01_at_vodka, Vodka Fire (Zangief and the breath shot shl01_01) */
	{ 0 },	/* 2c */
	{   4,         2 | NO_VEC,        A_SPECIAL,    K_LL,    0,   11,   20,     0, 0x1c,  10,  0x80,       3, SC300, SD_NOR_L,   0,  90,  83,   40,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 2d */
	{ 0 },	/* 2e */
	{ 0 },	/* 2f */
	{  16,        ZU | NO_VEC,        A_SPECIAL,     K_M,    0,   12,   20,     0, 0x1e,   8,  0x80,       3, SC300,        8,   0,  90,  38,   90,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        7, { 0, 0 } },	/* 30: pl01_at_banish, Banishing Flat */
	{  16,        ZU | NO_VEC,        A_SPECIAL,     K_M,    0,   12,   20,     0, 0x1e,   8,  0x80,       3, SC300,        8,   0,  90,  38,   90,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        7, { 0, 0 } },	/* 31 */
	{   4,      BU_O | NO_VEC,        A_SPECIAL,     K_S,    0,   10,   20,     0, 0x1f,   1,  0x80,       3, SC300, SD_NOR_L,   0,  90,  83,   40,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 32: pl01_at_lariat, Double Lariat on the ground */
	{  14,      BU_O | NO_VEC,        A_SPECIAL,     K_S,    0,   10,   20,     0, 0x1c,  10,  0x80,       3, SC300, SD_NOR_L,   0,  90,  83,   40, NO_WEIGHT | AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 33: pl01_at_qlariat, Quick Double Lariat on the ground */
	{  12,      BU_O | NO_VEC,        A_SPECIAL,     K_S,    0,   11,   20,     0, 0x00,   8,  0x80,       3, SC300, SD_NOR_L,   0,  90,  45,   46,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 34: pl01_at_jlariat, Double Lariat in the air */
	{  10,      BU_O | NO_VEC,        A_SPECIAL,     K_S,    0,   10,   20,     0, 0x00,   9,  0x80,       3, SC300, SD_NOR_L,   0,  90,  45,   46,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 35: pl01_at_jqlariat, Quick Double Lariat in the air */
	{  10,        10 | NO_VEC,        A_SPECIAL,     K_L,    0,   10,   10,     0, 0x1f,  10,  0x60,       3, SC200,        8,   0,  90,  92,   93, NO_CHIP | NO_WEIGHT | AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        5, { 0, 0 } },	/* 36: pl01_at_t_kick, Tag-in jumping kick (pl01_t_kick) */
	{   5,         2 | NO_VEC,          A_HYPER,     K_L,    0,    2,   20,     0, 0x1c,   4,  0x80,       3, SC300, SD_NOR_L,   0,  90,  83,   40,               NO_WEIGHT,  1,     0, 41,     0,     0,   0,        3, { 0, 0 } },	/* 37: pl01_at_team_1, His part in a team hyper (sp_team): first hit of the charge */
	{  18,         2 | NO_VEC,          A_HYPER,     K_L,    0,    2,   20,     0, 0x1b,  11,  0x80,       3, SC300, SD_NOR_L,   0,  90,  40,   41,               NO_WEIGHT,  1,     0, 41,     0,     0,   0,        3, { 0, 0 } },	/* 38: pl01_at_team_2, the team hyper, second hit of the charge */
	{ 0 },	/* 39 */
	{  10,        12 | NO_VEC,        A_SPECIAL,     K_L,    1,    4,   24,     0, 0x1c,  12,  0x80,       3, SC300,        8,  64,  90,  18,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0,        5, { 0, 0 } },	/* 3a: pl01_at_snapback, Snapback (sp_snapback) */
	{  22,                  2, A_SPECIAL | 0x40,       0,    0,   16,    0,    15, 0x0d,  15,  0x80, MARK_N1, SC300,       76,   0,   0,  64,   68,                       0,  1,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 3b */
	{  22,                  2, A_SPECIAL | 0x40,       0,    0,   16,    0,    15, 0x0d,  15,  0x80, MARK_N1, SC300,       76,   0,   0,  64,   68,                       0,  1,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 3c: pl01_at_russian_h, Aerial Russian Slam, heavy (59+level); also its variable counter */
	{  24,                  2, A_SPECIAL | 0x40,       0,    0,    4,    0,    15, 0x1d,   0,  0x80, MARK_N1, SC300,       76,   0,   0,   0,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 3d */
	{  28,                  2, A_SPECIAL | 0x40,       0,    0,    4,    0,    15, 0x1d,   0,  0x80, MARK_N1, SC300,       76,   0,   0,   0,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 3e */
	{   8,                  2, A_SPECIAL | 0x40,       0,    0,    4,    0,    15, 0x1a,   1,  0x80, MARK_N1, SC300,       76,   0,   0,  96,    0,           NO_WEIGHT | 1,  1,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 3f */
	{   8,                  2, A_SPECIAL | 0x40,       0,    0,    4,    0,    15, 0x1a,   1,  0x80, MARK_N1, SC300,       76,   0,   0,  96,    0,           NO_WEIGHT | 1,  1,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 40: pl01_at_powerbomb_h, Flying Powerbomb's grab hit, heavy (63+level); also its variable counter */
	{  16,                  2, A_SPECIAL | 0x40,       0,    0,    4,    0,    15, 0x1a,   1,  0x80, MARK_N1, SC300,       76,   0,   0,  96,    0,           NO_WEIGHT | 1,  1,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 41 */
	{  20,                  2, A_SPECIAL | 0x40,       0,    0,    4,    0,    15, 0x1a,   1,  0x80, MARK_N1, SC300,       76,   0,   0,  96,    0,           NO_WEIGHT | 1,  1,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 42: pl01_at_powerbomb_slam_h, Flying Powerbomb's slam after the standing grab, heavy (65+level); also VC */
	{  20,                  2, A_SPECIAL | 0x40,       0,    0,    4,    0,    15, 0x1a,   1,  0x80, MARK_N1, SC300,       76,   0,   0,  96,    0,                       0,  1,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 43 */
	{  24,                  2, A_SPECIAL | 0x40,       0,    0,    4,    0,    15, 0x1a,   1,  0x80, MARK_N1, SC300,       76,   0,   0,  96,    0,                       0,  1,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 44: pl01_at_powerbomb_run_h, Flying Powerbomb's slam after the grab at the end of the run, heavy (67+level); also VC */
	{   8,                  2,   A_HYPER | 0x40,       0,    0,    4,    0,    15, 0x1c,   4,  0x80, MARK_N1, SC300,       76,   0,   0, 102,    0,        NO_COMBO | NO_KO,  1,     0, 35,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 45: pl01_at_fab_1, Final Atomic Buster, first slam (fab_g_lift) */
	{   8,                  2,   A_HYPER | 0x40,       0,    0,    4,    0,    15, 0x1c,   4,  0x80, MARK_N1, SC300,       76,   0,   0,   0,    0,        NO_COMBO | NO_KO,  1,     0, 35,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 46: pl01_at_fab_2, Final Atomic Buster, second slam (fab_g_lift2) */
	{   0,         2 | NO_VEC,   A_HYPER | 0x40,       0,    0,    8,    0,    15, 0x00,  12,  0x80, MARK_N1, SC300, SD_NOR_S,   0,   0,   0,    0, NO_WEIGHT | NO_SCALE | NO_COMBO | NO_KO,  1,     0, 35,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 47: pl01_at_fab_3, Final Atomic Buster, the throw up (fab_g_up) */
	{   9,                  2,   A_HYPER | 0x40,       0,    0,    4,    0,    15, 0x1c,   4,  0x80, MARK_N1, SC300,       76,   0,   0,   0,    0,        NO_COMBO | NO_KO,  1,     0, 35,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 48 */
	{  10,                  2,   A_HYPER | 0x40,       0,    0,    4,    0,    15, 0x1c,   4,  0x80, MARK_N1, SC300,       76,   0,   0,   0,    0,        NO_COMBO | NO_KO,  1,     0, 35,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 49 */
	{  11,                  2,   A_HYPER | 0x40,       0,    0,    4,    0,    15, 0x1c,   4,  0x80, MARK_N1, SC300,       76,   0,   0,   0,    0,        NO_COMBO | NO_KO,  1,     0, 35,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 4a */
	{  12,                  2,   A_HYPER | 0x40,       0,    0,    4,    0,    15, 0x1c,   4,  0x80, MARK_N1, SC300,       76,   0,   0,   0,    0,        NO_COMBO | NO_KO,  1,     0, 35,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 4b */
	{  20,                  2,   A_HYPER | 0x40,       0,    0,    4,    0,    15, 0x1e,   5,  0x80, MARK_N1, SC300,       76,   0,   0,   0,    0,    NO_WEIGHT | NO_COMBO,  1,     0, 35,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 4c */
	{  24,                  2,   A_HYPER | 0x40,       0,    0,    4,    0,    15, 0x1e,   5,  0x80, MARK_N1, SC300,       76,   0,   0,   0,    0,    NO_WEIGHT | NO_COMBO,  1,     0, 35,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 4d */
	{  26,                  2,   A_HYPER | 0x40,       0,    0,    4,    0,    15, 0x1e,   5,  0x80, MARK_N1, SC300,       76,   0,   0,   0,    0,    NO_WEIGHT | NO_COMBO,  1,     0, 35,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 4e */
	{  30,                  2,   A_HYPER | 0x40,       2,    0,    4,    0,    15, 0x1e,   5,  0x80, MARK_N1, SC300,       76,   0,   0,   0,    0,    NO_WEIGHT | NO_COMBO,  1,     0, 35,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 4f */
	{  10,         2 | NO_VEC,   A_HYPER | 0x40,       0,    0,    8,    0,    15, 0x00,  13,  0x80, MARK_N1, SC300, SD_NOR_S,   0,   0,   0,    0, NO_WEIGHT | NO_COMBO | NO_KO,  1,     0, 55,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 50: pl01_at_uab_1, Ultra Final Atomic Buster: slam on landing (uab_g_land) */
	{  16,         2 | NO_VEC,   A_HYPER | 0x40,       0,    0,    8,    0,    15, 0x00,  13,  0x80, MARK_N1, SC300, SD_NOR_S,   0,   0,   0,    0, NO_WEIGHT | NO_COMBO | NO_KO,  1,     0, 55,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 51: pl01_at_uab_2, Ultra Final Atomic Buster: lift (uab_g_lift) */
	{  16,         2 | NO_VEC,   A_HYPER | 0x40,       0,    0,    8,    0,    15, 0x00,  13,  0x80, MARK_N1, SC300, SD_NOR_S,   0,   0,   0,    0, NO_WEIGHT | NO_COMBO | NO_KO,  1,     0, 55,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 52: pl01_at_uab_3, Ultra Final Atomic Buster: second landing (uab_g_land2) */
	{   0,         2 | NO_VEC,   A_HYPER | 0x40,       0,    0,    8,    0,    15, 0x00,  13,  0x80, MARK_N1, SC300, SD_NOR_S,   0,   0,   0,    0, NO_WEIGHT | NO_SCALE | NO_COMBO | NO_KO,  1,     0, 55,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 53: pl01_at_uab_4, Ultra Final Atomic Buster: the throw up (uab_g_up) */
	{  22,                  2,   A_HYPER | 0x40,       0,    0,    4,    0,    15, 0x1e,   5,  0x80, MARK_N1, SC300,       76,   0,   0,   0,    0,    NO_WEIGHT | NO_COMBO,  1,     0, 55,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 54 */
	{  24,                  2,   A_HYPER | 0x40,       0,    0,    4,    0,    15, 0x1e,   5,  0x80, MARK_N1, SC300,       76,   0,   0,   0,    0,    NO_WEIGHT | NO_COMBO,  1,     0, 55,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 55 */
	{  31,                  2,   A_HYPER | 0x40,       0,    0,    4,    0,    15, 0x1e,   5,  0x80, MARK_N1, SC300,       76,   0,   0,   0,    0,    NO_WEIGHT | NO_COMBO,  1,     0, 55,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 56 */
	{  38,                  2,   A_HYPER | 0x40,       2,    0,    4,    0,    15, 0x1e,   5,  0x80, MARK_N1, SC300,       76,   0,   0,   0,    0,    NO_WEIGHT | NO_COMBO,  1,     0, 55,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 57 */
};
