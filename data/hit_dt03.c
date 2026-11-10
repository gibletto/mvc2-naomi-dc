/* Morrigan (character 03): the attacks (HIT_DATA, 0x1c bytes).
   What each hit does. The character's module sets the attack's number (atck_no) with the move,
   and the row is read when the attack box meets a body. After a row is its number, in
   hexadecimal, and the program's name for it:
   ATCK_ for the normals every character has (player.h), pl03_at_ for its own (pl03_def.h).
   The values' names are Capcom's own for these tables (hit_equ.h) and the program's (player.h).

     dmg     the damage
     type    the reaction: ZU pushed back, BU_O knocked off the feet, AS swept,
             or the number of another in pl_dmg.c's dm_tbl (here 2 dm_buttobi_ng, 5 dm_hold,
             10 dm_away, 12 dm_snap).
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
     box, box2  boxes of hit_03 that the computer measures the attack's reach by
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

const HIT_DATA hit_dt_03[] = {
	/* dmg                     type              kind              elem  piyo  stop  sand  sleep   vec  pow  range     mark  score        se  shc  jug  box  box2                     flag  13  guard  15  catch  abyss  cnt    answer        1a */
	{   3,             ZU | NO_VEC,            A_NOM,              K_S,    4,   10,   10,     0, 0x18,   4,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  32,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 00: ATCK_STAND_LP */
	{   6,             ZU | NO_VEC,            A_NOM,             K_LM,    2,   10,   16,     0, 0x1e,   6,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40,  33,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 01: ATCK_STAND_MP */
	{  12,             ZU | NO_VEC,            A_NOM,              K_L,    1,   11,   24,     0, 0x1a,   8,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  50,  34,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 02: ATCK_STAND_HP */
	{   3,             ZU | NO_VEC,            A_NOM,              K_S,    4,   10,   10,     0, 0x18,   4,  0x60, MARK_N1, SC100, SD_KIK_S,   0,  20,  35,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 03: ATCK_STAND_LK */
	{   6,             ZU | NO_VEC,            A_NOM,             K_LM,    2,   10,   16,     0, 0x1a,   6,  0x60, MARK_N2, SC200, SD_KIK_M,   0,  40,  65,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 04: ATCK_STAND_MK */
	{  12,             ZU | NO_VEC,            A_NOM,              K_L,    1,   11,   24,     0, 0x1c,   8,  0x80, MARK_N3, SC300, SD_KIK_L,   0,  50,  37,   11,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 05: ATCK_STAND_HK */
	{   3,             ZU | NO_VEC,            A_NOM,             K_LS,    4,   10,   10,     0, 0x1c,   4,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  10,  38,   20,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 06: ATCK_CROUCH_LP */
	{   6,             ZU | NO_VEC,            A_NOM,             K_LM,    2,   10,   16,     0, 0x1d,   6,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40,  39,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 07: ATCK_CROUCH_MP */
	{  12,           BU_O | NO_VEC,            A_NOM,              K_L,    1,   10,   24,     0, 0x00,  11,  0x80, MARK_N3, SC300, SD_KIK_L,   0,  50, 101,    0, NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 08: ATCK_CROUCH_HP */
	{   3,             ZU | NO_VEC,             A_LO,             K_LS,    4,   10,   10,     0, 0x1c,   4,  0x70, MARK_N1, SC100, SD_KIK_S,   0,  20,  41,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 09: ATCK_CROUCH_LK */
	{   6,             ZU | NO_VEC,             A_LO,             K_LM,    2,   10,   16,     0, 0x1d,   6,  0x40, MARK_N2, SC200, SD_KIK_M,   0,  40,  42,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0a: ATCK_CROUCH_MK */
	{  12,                      AS,             A_LO,             K_LL,    1,   10,   24,     4, 0x1a,   6,  0x70, MARK_N3, SC300, SD_KIK_L,   0,  90,  43,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0b: ATCK_CROUCH_HK */
	{   3,             ZU | NO_VEC,             A_HI,              K_S,    1,   10,   10,     0, 0x1e,   3,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  44,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0c: ATCK_JUMP_LP */
	{   6,             ZU | NO_VEC,             A_HI,             K_LS,    1,   10,   16,     6, 0x1e,   5,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40,  45,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0d: ATCK_JUMP_MP */
	{  12,             ZU | NO_VEC,             A_HI,             K_LM,    1,   12,   32,     0, 0x16,  10,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  60,  46,    0,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0e: ATCK_JUMP_HP */
	{   3,             ZU | NO_VEC,             A_HI,              K_S,    1,   10,   10,     0, 0x00,   3,  0x60, MARK_N1, SC100, SD_KIK_S,   0,  20,  47,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0f: ATCK_JUMP_LK */
	{   6,             ZU | NO_VEC,             A_HI,              K_M,    1,   10,   16,     0, 0x1d,   5,  0x60, MARK_N2, SC200, SD_KIK_M,   0,  40,  48,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 10: ATCK_JUMP_MK */
	{  12,             ZU | NO_VEC,             A_HI,              K_M,    1,   12,   32,     0, 0x16,  11,  0x80, MARK_N3, SC300, SD_KIK_L,   0,  60,  49,    0,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 11: ATCK_JUMP_HK */
	{  12,             ZU | NO_VEC,             A_HI,              K_M,    1,   12,   32,     0, 0x10,   8,  0x80, MARK_N3, SC300, SD_KIK_L,   0,  60,  84,    0,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 12: pl03_at_jdown_hk, Jump HK with the stick down (stops the jump) */
	{   4,     ZU | NO_VEC | NO_ZU,            A_NOM,              K_S,    1,    6,   24,     0, 0x10,   4,  0x80, MARK_N1, SC300, SD_KIK_L,   4,  20, 108,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 13: pl03_at_fwd_hk, Stand HK with the stick forward */
	{   5,     ZU | NO_VEC | NO_ZU,            A_NOM,              K_S,    1,    6,   24,     0, 0x10,   4,  0x80, MARK_N2, SC300, SD_KIK_L,   4,  20, 109,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 14 */
	{   7,     ZU | NO_VEC | NO_ZU,            A_NOM,              K_S,    1,    6,   24,     0, 0x10,   4,  0x80, MARK_N2, SC300, SD_KIK_L,   4,  20, 110,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 15 */
	{   7,             ZU | NO_VEC,            A_NOM,              K_L,    1,    8,   30,     0, 0x10,   8,  0x80, MARK_N3, SC300, SD_KIK_L,   4,  20, 111,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 16 */
	{   3,             ZU | NO_VEC, A_HI | A_SPECIAL,              K_M,    1,   11,   24,     0, 0x02,   5,  0x80, MARK_N3, SC300, SD_KIK_M,   0,  60, 117,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 17: pl03_at_shell, Shell Kick in the air (sp_shell) */
	{   3,             ZU | NO_VEC, A_HI | A_SPECIAL,              K_M,    1,   11,   24,     0, 0x02,   5,  0x80, MARK_N3, SC300, SD_KIK_M,   0,  60, 117,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 18 */
	{   3,             ZU | NO_VEC, A_HI | A_SPECIAL,              K_L,    1,   11,   24,     0, 0x16,  12,  0x80, MARK_N3, SC300, SD_KIK_M,   0,  60, 117,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 19 */
	{   3,             ZU | NO_VEC,            A_NOM, K_S | K_BACK_MAX,    0,   10,   10,     0, 0x18,   4,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  10,  32,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 1a */
	{   3,             ZU | NO_VEC,            A_NOM,             K_LS,    1,   10,   10,     0, 0x1c,   4,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  10,  38,   20,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 1b: pl03_at_crouch_lp_2, Crouch LP, second hit */
	{  12,             ZU | NO_VEC,            A_NOM,             K_LL,    1,   11,   24,     0, 0x00,   8,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  50,  40,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 1c: pl03_at_dback_hp, Crouch HP with the stick back */
	{ 0 },	/* 1d */
	{ 0 },	/* 1e */
	{ 0 },	/* 1f */
	{  16,                       2,     A_NOM | 0x40,             K_LM,    4,   12,   16,     0, 0x11,  10,  0x60, MARK_N2, SC200, SD_NOR_M,   0,  40, 243,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 20: pl03_at_throw_p, Punch throw */
	{  24,                       2, A_SPECIAL | 0x40,             K_LM,    4,   12,   20,     0, 0x1a,   8,  0x60, MARK_N2, SC200, SD_NOR_M,   0,  40, 245,    0,                       0,  1,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 21: pl03_at_vector, Vector Drain's throw (vector_catch_leave) */
	{  16,                       2,     A_NOM | 0x40,             K_LM,    4,   12,   20,     0, 0x1c,  10,  0x60, MARK_N2, SC200, SD_NOR_M,   0,  40, 244,    0,                       0,  0,     0, 10,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 22: pl03_at_air_throw_p, Punch air throw */
	{  16,                       2,     A_NOM | 0x40,             K_LM,    4,   12,    8,     0, 0x1a,   1,  0x60, MARK_N2, SC200, SD_NOR_M,   0,  40, 243,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 23: pl03_at_throw_k, Kick throw's grip (pls03_01) */
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
	{  16,           BU_O | NO_VEC,        A_SPECIAL,              K_L,    0,    8,   24,     0, 0x1e,  12,  0x60,       3, SC200,        8,   0,  90,  61,   63,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 30 */
	{  12,           BU_O | NO_VEC,        A_SPECIAL,              K_M,    0,    4,    8,     0, 0x1d,  12,  0x60,       3, SC200,        8,   6,  90,  61,   63,               NO_WEIGHT,  1,     0,  0,     0,    30,   0,        6, { 0, 0 } },	/* 31: pl03_at_blade_h, Shadow Blade, heavy (48+level); also its variable counter */
	{   4,           BU_O | NO_VEC,        A_SPECIAL,              K_M,    0,    4,    8,     0, 0x1d,  10,  0x60,       3, SC200,        8,   2,  90,  61,   63,               NO_WEIGHT,  1,     0,  0,     0,    30,   0,        6, { 0, 0 } },	/* 32 */
	{   4,           BU_O | NO_VEC,        A_SPECIAL,              K_L,    0,    6,   10,     0, 0x1e,  13,  0x60,       3, SC200,        8,   2,  90,  61,   63,               NO_WEIGHT,  1,     0,  0,     0,    30,   0,        6, { 0, 0 } },	/* 33 */
	{  10,           BU_O | NO_VEC,        A_SPECIAL,              K_L,    0,    6,   24,     0, 0x1e,  13,  0x60,       3, SC200,        8,   0,  90,  61,   63,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 34 */
	{  12,           BU_O | NO_VEC,        A_SPECIAL,              K_L,    0,    6,   24,     0, 0x1e,  13,  0x60,       3, SC200,        8,   0,  90,  61,   63,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 35 */
	{  12,           BU_O | NO_VEC,        A_SPECIAL,              K_L,    1,   10,   24,     0, 0x00,  11,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  40, 101,    0, NO_CHIP | NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 36: pl03_at_upper, the launcher (pl03_upper) */
	{  15,     ZU | NO_VEC | NO_ZU,        A_SPECIAL,              K_L,    0,   10,   24,     0, 0x1d,   8,  0x80,       3, SC300,        8,   0,  50,  69,   96,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 37: pl03_at_soul, Soul Fist on the ground (Morrigan and shl03_00) */
	{  14,     ZU | NO_VEC | NO_ZU,        A_SPECIAL,              K_L,    0,    8,   24,     0, 0x16,   8,  0x80,       3, SC300,        8,   0,  50,  67,   96, NO_WEIGHT | AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        5, { 0, 0 } },	/* 38: pl03_at_jsoul, Soul Fist in the air (Morrigan and shl03_00) */
	{  10,             10 | NO_VEC,        A_SPECIAL,              K_L,    0,   10,   10,     0, 0x1f,  10,  0x60,       3, SC200,       12,   0,  90,  98,    0, NO_CHIP | NO_WEIGHT | AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        5, { 0, 0 } },	/* 39: pl03_at_t_kick, Tag-in jumping kick (t_kick_init) */
	{  10,             12 | NO_VEC,        A_SPECIAL,              K_L,    1,    4,   24,     0, 0x1c,  12,  0x80,       3, SC300,        8,  64,  90,  40,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0,        5, { 0, 0 } },	/* 3a: pl03_at_snapback, Snapback (sp_snapback) */
	{   5,             ZU | NO_VEC,   A_HYPER | 0x80,              K_M,    0,   10,   90,     0, 0x1c,   3,  0x80,       3, SC300,       12,   0,  90,  73,    0, NO_WEIGHT | NO_COMBO | NO_KO,  1,     0, 48,    12,    16,   0,        3, { 0, 0 } },	/* 3b: pl03_at_illusion, Darkness Illusion's dash, ground and air */
	{   3,                       5,   A_HYPER | 0x80,              K_S,    0,    4,    0,     0, 0x1a,   1,  0x80,       3, SC100, SD_NOR_L,   0,  90,   0,    0,                   NO_KO,  1,     0, 48,     0,     0,   0,        3, { 0, 0 } },	/* 3c */
	{   2,                       5,   A_HYPER | 0x80,             K_LS,    0,    4,    0,    60, 0x00,   9,  0x80,       3, SC300,        8,   0,  90,   0,    0,                   NO_KO,  1,     0, 48,     0,     0,   0,        3, { 0, 0 } },	/* 3d */
	{   4,              2 | NO_VEC,          A_HYPER,              K_L,    0,   10,   30,     0, 0x00,  11,  0x80,       3, SC300, SD_KIK_L,   0,  90,   0,    0,       NO_WEIGHT | NO_KO,  1,     0, 48,     0,     0,   0,        3, { 0, 0 } },	/* 3e */
	{  11, 2 | NO_VEC | FLY_SCREEN,          A_HYPER,              K_L,    0,   12,   20,    30, 0x10,  11,  0x60,       3, SC200,        8,   0,  90,   0,    0,    NO_WEIGHT | NO_SCALE,  1,     0, 48,     0,     0,   0,        3, { 0, 0 } },	/* 3f: pl03_at_illusion_air, Darkness Illusion's hits in the air (cg_type 63) */
	{   3,                       5,   A_HYPER | 0x80,              K_S,    0,    4,    0,     0, 0x1a,   1,  0x80,       3, SC100, SD_NOR_L,   0,  90,   0,    0,                   NO_KO,  1,     0, 48,     0,     0,   0,        3, { 0, 0 } },	/* 40 */
	{   2,                       5,   A_HYPER | 0x80,             K_LS,    0,    4,    0,    60, 0x00,   9,  0x80,       3, SC300,        8,   0,  90,   0,    0,                   NO_KO,  1,     0, 48,     0,     0,   0,        3, { 0, 0 } },	/* 41 */
	{   4,              2 | NO_VEC,          A_HYPER,              K_L,    0,   10,   30,     0, 0x00,  10,  0x80,       3, SC300, SD_KIK_L,   0,  90,   0,    0,       NO_WEIGHT | NO_KO,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 42 */
	{   2,                       5,          A_HYPER,              K_L,    0,    2,   20,     0, 0x00,   6,  0x60,       3, SC200, SD_NOR_M,   0,  90,   0,    0,       NO_WEIGHT | NO_KO,  1,     0, 48,     0,     0,   0,        3, { 0, 0 } },	/* 43 */
	{  11,          2 | FLY_SCREEN,          A_HYPER,              K_L,    0,   10,   20,    30, 0x10,  15,  0x60,       3, SC200,        8,   0,  90,   0,    0,    NO_WEIGHT | NO_SCALE,  1,     0, 48,     0,     0,   0,        3, { 0, 0 } },	/* 44: pl03_at_jillusion_end, Darkness Illusion in the air: the last hit */
	{   4,      2 | NO_VEC | NO_ZU,          A_HYPER,              K_L,    0,    8,   24,    15, 0x1a,   6,  0x80,       4, SC300,       18,   0,  90,  79,   80,               NO_WEIGHT,  1,     0, 44,     0,     0,   5,        3, { 0, 0 } },	/* 45: pl03_at_shower, Finishing Shower (sp_shower, on Morrigan) and its shots' heads and tails */
	{   4,      2 | NO_VEC | NO_ZU,          A_HYPER,             K_LL,    0,    8,   24,    15, 0x1a,   6,  0x80,       4, SC300,       18,   0,  90,  80,   79,     NO_CHIP | NO_WEIGHT,  1,     0, 44,     0,     0,   5,        3, { 0, 0 } },	/* 46: pl03_at_shower_hit, a Finishing Shower head's hit on its animation's mark */
	{   2,      2 | NO_VEC | NO_ZU,          A_HYPER,              K_L,    0,    8,   24,    15, 0x1a,   6,  0x80,       4, SC300,       17,   0,  90,  87,   88,     NO_CHIP | NO_WEIGHT,  1,     0, 44,     0,     0,   4,        3, { 0, 0 } },	/* 47: pl03_at_shower_next, Finishing Shower shots put out by an effect (pls03_00): head, tail and burst */
	{   6,               2 | NO_ZU,          A_HYPER,              K_L,    0,   10,   24,    15, 0x1c,  11,  0x80,       3, SC300,        8,   0,  90,  89,   90,               NO_WEIGHT,  1,     0, 45,     0,     0,   0,        3, { 0, 0 } },	/* 48: pl03_at_silhouette, Silhouette Blade (hyper, sp_silhouette) */
	{   6,               2 | NO_ZU,          A_HYPER,              K_L,    0,   10,   24,    15, 0x1c,  11,  0x80,       3, SC300,        8,   0,  90,  90,   91,               NO_WEIGHT,  1,     0, 45,     0,     0,   0,        3, { 0, 0 } },	/* 49 */
	{   8,            BU_O | NO_ZU,          A_HYPER,              K_L,    0,   10,   24,    15, 0x1c,  11,  0x80,       3, SC300,        8,   0,  90,  91,   92,               NO_WEIGHT,  1,     0, 45,     0,     0,   0,        3, { 0, 0 } },	/* 4a */
	{   8,            BU_O | NO_ZU,          A_HYPER,              K_L,    0,   10,   24,    15, 0x1c,  11,  0x80,       3, SC300,        8,   0,  90,  92,   93,               NO_WEIGHT,  1,     0, 45,     0,     0,   0,        3, { 0, 0 } },	/* 4b */
	{  18,            BU_O | NO_ZU,          A_HYPER,              K_L,    0,   10,   24,    15, 0x1c,  15,  0x80,       3, SC300,        8,   0,  90,  93,   94,               NO_WEIGHT,  1,     0, 45,     0,     0,   0,        3, { 0, 0 } },	/* 4c */
};
