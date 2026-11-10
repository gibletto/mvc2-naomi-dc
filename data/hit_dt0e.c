/* Venom (character 0e): the attacks (HIT_DATA, 0x1c bytes).
   What each hit does. The character's module sets the attack's number (atck_no) with the move,
   and the row is read when the attack box meets a body. After a row is its number, in
   hexadecimal, and the program's name for it:
   ATCK_ for the normals every character has (player.h), pl0e_at_ for its own (pl0e_def.h).
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
     box, box2  boxes of hit_0e that the computer measures the attack's reach by
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

const HIT_DATA hit_dt_0e[] = {
	/* dmg                     type          kind               elem  piyo  stop  sand  sleep   vec  pow  range     mark  score  se  shc  jug  box  box2                     flag  13  guard  15  catch  abyss  cnt    answer        1a */
	{   5,                      ZU,        A_NOM,               K_S,    4,   10,   10,     0, 0x18,   4,  0x70, MARK_N1, SC100,  1,   0,  20,  10,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 00: ATCK_STAND_LP */
	{   8,           BU_O | NO_VEC,        A_NOM,              K_LM,    2,   11,   16,     0, 0x1f,  11,  0x80, MARK_N2, SC200,  2,   0,  40,  16,   13, NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 01: ATCK_STAND_MP */
	{  15,                    BU_O,        A_NOM,               K_L,    1,   12,   24,     0, 0x1a,  12,  0x80, MARK_N3, SC300,  3,   0,  50,  18,   20,                       0,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 02: ATCK_STAND_HP */
	{   5,                      ZU,        A_NOM,              K_LS,    4,   10,   10,     0, 0x18,   2,  0x60, MARK_N1, SC100,  1,   0,  20,  23,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 03: ATCK_STAND_LK */
	{   4,                      ZU,        A_NOM,               K_M,    2,    8,   16,     0, 0x1a,   3,  0x60, MARK_N2, SC200,  2,   0,  40,  26,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 04: ATCK_STAND_MK */
	{   7,                      ZU,        A_NOM,               K_S,    1,    4,   24,     0, 0x1c,  10,  0x80, MARK_N3, SC300,  2,   0,  30,  34,   35,                       0,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 05: ATCK_STAND_HK */
	{   5,                      ZU,        A_NOM,              K_LS,    4,   10,   10,     0, 0x18,   6,  0x70, MARK_N1, SC100,  1,   0,  20,  37,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 06: ATCK_CROUCH_LP */
	{   8,                      ZU,        A_NOM,              K_LM,    2,   11,   16,     0, 0x18,   8,  0x80, MARK_N2, SC200,  2,   0,  40,  39,  209,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 07: ATCK_CROUCH_MP */
	{  14,                    BU_O,        A_NOM,              K_LL,    1,   12,   24,     0, 0x1a,  12,  0x80, MARK_N3, SC300,  3,   0,  50,  46,   48,                       0,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 08: ATCK_CROUCH_HP */
	{   5,                      ZU,         A_LO,              K_LS,    4,   10,   10,     0, 0x18,   8,  0x70, MARK_N1, SC100,  1,   0,  20,  50,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 09: ATCK_CROUCH_LK */
	{   8,                      ZU,        A_NOM,              K_LM,    2,   11,   16,     0, 0x18,   8,  0x40, MARK_N2, SC200,  2,   0,  40,  52,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0a: ATCK_CROUCH_MK */
	{  14,                      AS,         A_LO,              K_LL,    1,   12,   24,     9, 0x18,   8,  0x70, MARK_N3, SC300,  3,   0,  90,  54,    0,                       0,  0,     0,  0,     0,    60,   0,        2, { 0, 0 } },	/* 0b: ATCK_CROUCH_HK */
	{   4,             ZU | NO_VEC,         A_HI,               K_S,    1,   10,   10,     0, 0x00,   3,  0x70, MARK_N1, SC100,  1,   0,  20,  56,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0c: ATCK_JUMP_LP */
	{   7,             ZU | NO_VEC,         A_HI,               K_M,    1,   10,   16,     0, 0x00,   4,  0x80, MARK_N2, SC200,  2,   0,  40,  58,   59,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0d: ATCK_JUMP_MP */
	{   7,             ZU | NO_VEC,         A_HI,               K_M,    1,    5,   32,     0, 0x17,   8,  0x80, MARK_N3, SC300,  2,   0,  30,  64,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 0e: ATCK_JUMP_HP */
	{   4,             ZU | NO_VEC,         A_HI,               K_S,    1,   10,   10,     0, 0x00,   4,  0x60, MARK_N1, SC100,  1,   0,  20,  66,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0f: ATCK_JUMP_LK */
	{   7,             ZU | NO_VEC,         A_HI,               K_M,    1,   10,   16,     0, 0x00,   2,  0x60, MARK_N2, SC200,  2,   0,  40,  68,   69,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 10: ATCK_JUMP_MK */
	{  13,             ZU | NO_VEC,         A_HI,               K_M,    1,    3,   32,     0, 0x1d,   8,  0x80, MARK_N3, SC300,  3,   0,  30,  71,   74, NO_WEIGHT | AERIAL_FINISH,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 11: ATCK_JUMP_HK */
	{   8,           BU_O | NO_VEC,    A_SPECIAL,              K_LL,    0,   11,   16,     0, 0x1f,  11,  0x80, MARK_N3, SC300,  2,   0,  40,  16,   13, NO_CHIP | NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 12: pl0e_at_upper, the launcher (pl0e_upper, Char_Extra 4) */
	{   5,                      ZU,        A_NOM,  K_S | K_BACK_MAX,    4,   10,   10,     0, 0x18,  10,  0x70, MARK_N1, SC100,  1,   0,  20,  10,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 13 */
	{   5,                      ZU,        A_NOM, K_LS | K_BACK_MAX,    4,   10,   10,     0, 0x18,   2,  0x60, MARK_N1, SC100,  1,   0,  20,  23,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 14 */
	{ 0 },	/* 15 */
	{ 0 },	/* 16 */
	{ 0 },	/* 17 */
	{ 0 },	/* 18 */
	{   4,                      ZU,        A_NOM,              K_LM,    2,   11,   16,     0, 0x1a,   3,  0x60, MARK_N2, SC200,  2,   2,  40,  26,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 19: pl0e_at_stand_mk_2, Standing MK's second hit (stand_kick_move, nml_no 1) */
	{   7,                      ZU,        A_NOM,               K_L,    1,   10,   24,     0, 0x1c,  10,  0x80, MARK_N3, SC300,  3,   4,  30,  34,   35,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 1a: pl0e_at_stand_hk_2, Standing HK's second hit (stand_kick_move, nml_no 2) */
	{   7,             ZU | NO_VEC,         A_HI,               K_M,    1,    8,   32,     0, 0x14,  10,  0x80, MARK_N3, SC300,  3,   4,  30,  64,    0,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 1b */
	{   7,             ZU | NO_VEC,         A_HI,               K_M,    1,    3,   32,     0, 0x18,   6,  0x80, MARK_N3, SC300,  3,   0,  30,  74,   76, NO_WEIGHT | AERIAL_FINISH,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 1c */
	{   7,             ZU | NO_VEC,         A_HI,               K_M,    1,    8,   32,     0, 0x1e,  10,  0x80, MARK_N3, SC300,  3,   0,  30,  74,   76, NO_WEIGHT | AERIAL_FINISH,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 1d */
	{ 0 },	/* 1e */
	{ 0 },	/* 1f */
	{  10,              2 | NO_VEC, A_NOM | 0x40,               K_L,    4,    4,   20,     0, 0x19,  15,  0x60,       3, SC300,  3,   0,  90,   0,    0,                NO_SCALE,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 20: pl0e_at_throw_p, Ground punch throw (nml_p_move, set on the held opponent) */
	{  16,              2 | NO_VEC, A_NOM | 0x40,               K_L,    4,    4,   20,     0, 0x06,  15,  0x60,       3, SC300,  3,   0,  90,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 21: pl0e_at_throw_k, Ground kick throw: lets the opponent go up with a quake (nml_k_move) */
	{  16,              2 | NO_VEC, A_NOM | 0x40,               K_L,    4,    4,   20,     0, 0x1a,  15,  0x60,       3, SC300,  3,   0,  90,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 22: pl0e_at_air_throw, Air throw: lands, then throws them up (jump_p_move) */
	{  16, 2 | NO_VEC | FLY_SCREEN,    A_SPECIAL,               K_L,    4,    4,   20,     0, 0x07,  15,  0x60,       3, SC300,  3,   0,  90,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 23: pl0e_at_webthrow_end, Web Throw's last hit, the release (webthrow_release) */
	{ 0 },	/* 24 */
	{  10,             10 | NO_VEC,    A_SPECIAL,               K_L,    0,   10,   10,     0, 0x1f,  10,  0x60,       3, SC200,  4,   0,  90, 168,    0, NO_CHIP | NO_WEIGHT | AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 25: pl0e_at_t_kick, T_kick, the jump-in attack (t_kick_init) */
	{  10,           BU_O | NO_VEC,    A_SPECIAL,              K_LM,    1,   10,   16,     0, 0x1f,  11,  0x80, MARK_N2, SC200,  2,   0,  60,  16,   13,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 26: pl0e_at_assist, the attack on the spot of the gamma assist and counter (Char_Special 16) */
	{ 0 },	/* 27 */
	{  18,   BU_O | NO_VEC | NO_ZU,    A_SPECIAL,               K_L,    0,    6,   10,     0, 0x1b,  12,  0x60,       3, SC300,  4,   0,  90,  84,   92,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 28 */
	{   3,     ZU | NO_VEC | NO_ZU,    A_SPECIAL,               K_L,    0,    6,   10,     0, 0x1a,  10,  0x60,       3, SC300,  4,   0,  90,  88,   92,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 29 */
	{   3,     ZU | NO_VEC | NO_ZU,    A_SPECIAL,               K_L,    0,    6,   10,     0, 0x17,   8,  0x60,       3, SC300,  4,   0,  90,  92,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 2a */
	{   3,     ZU | NO_VEC | NO_ZU,    A_SPECIAL,               K_L,    0,    6,   10,     0, 0x14,   6,  0x60,       3, SC300,  4,   0,  90,  99,   92,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 2b */
	{   3,   BU_O | NO_VEC | NO_ZU,    A_SPECIAL,               K_L,    0,    6,   10,     0, 0x1b,  15,  0x60,       3, SC300,  4,   0,  90,  80,   92,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 2c */
	{  16,   BU_O | NO_VEC | NO_ZU,    A_SPECIAL,               K_L,    0,    6,   10,     0, 0x1a,   8,  0x60,       3, SC300,  4,   0,  90,  84,   92,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 2d */
	{   2,     ZU | NO_VEC | NO_ZU,    A_SPECIAL,               K_L,    0,    6,   10,     0, 0x18,   6,  0x60,       3, SC300,  4,   0,  90,  88,   92,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 2e */
	{   2,     ZU | NO_VEC | NO_ZU,    A_SPECIAL,               K_L,    0,    6,   10,     0, 0x17,   6,  0x60,       3, SC300,  4,   0,  90,  92,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 2f */
	{   2,     ZU | NO_VEC | NO_ZU,    A_SPECIAL,               K_L,    0,    6,   10,     0, 0x15,   6,  0x60,       3, SC300,  4,   0,  90,  99,   92,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 30 */
	{   2,   BU_O | NO_VEC | NO_ZU,    A_SPECIAL,               K_L,    0,    6,   10,     0, 0x1c,  15,  0x60,       3, SC300,  4,   0,  90,  80,   92,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 31 */
	{   2,     ZU | NO_VEC | NO_ZU,    A_SPECIAL,              K_LS,    0,    0,    0,     0, 0x1a,   2,  0x80,       3, SC300, 19,   0,  50, 130,  135,                 NO_CHIP,  1,     0,  0,    12,     0,   0,        4, { 0, 0 } },	/* 32 */
	{   2,              2 | NO_VEC,    A_SPECIAL,              K_LS,    0,    0,    0,     0, 0x1a,   2,  0x80,       3, SC300, 19,   0,  50, 125,  146,                 NO_CHIP,  1,     0,  0,    12,     0,   0,        4, { 0, 0 } },	/* 33 */
	{   2,              2 | NO_VEC,    A_SPECIAL,              K_LS,    0,    0,    0,     0, 0x00,   2,  0x80,       3, SC300, 19,   0,  50, 150,  156,                 NO_CHIP,  1,     0,  0,    12,     0,   0,        4, { 0, 0 } },	/* 34 */
	{   6,              2 | NO_VEC,    A_SPECIAL,               K_L,    0,    8,   10,     0, 0x1c,   6,  0x60,       3, SC300,  3,   0,   0,   0,    0,                   NO_KO,  1,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 35: pl0e_at_webthrow, Web Throw's slam on the caught opponent (webthrow_throw) */
	{   3,              2 | NO_VEC,    A_SPECIAL,               K_L,    0,    4,   10,     0, 0x1c,   6,  0x60,       3, SC300,  3,   0,   0,   0,    0,                   NO_KO,  1,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 36: pl0e_at_webthrow_renda, Web Throw's slam after button mashing (webthrow_throw, timer0 set) */
	{   0,     ZU | NO_VEC | NO_ZU,      A_HYPER,               K_S,    0,    0,    0,    60, 0x00,   2,  0x80,       3, SC300, 19,   0,  90, 100,    0,         NO_CHIP | NO_KO,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 37: pl0e_at_web, Venom Web (hyper): the start and the web that catches (web_init, shl0e_01) */
	{   5,      5 | NO_VEC | NO_ZU,      A_HYPER,               K_L,    0,    6,   48,     0, 0x1c,   6,  0x60,       3, SC100,  3,   0,  90, 105,  107,       NO_WEIGHT | NO_KO,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 38: pl0e_at_web_dive, Venom Web's dive onto the webbed opponent (web_dive) */
	{   4,      5 | NO_VEC | NO_ZU,      A_HYPER,              K_LL,    0,    6,   48,     0, 0x1c,   6,  0x60,       3, SC100,  4,   0,  90, 109,  111,       NO_WEIGHT | NO_KO,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 39 */
	{   4,      2 | NO_VEC | NO_ZU,      A_HYPER,               K_L,    0,    6,   48,     0, 0x00,   7,  0x60,       3, SC100,  4,   0,  90, 113,  114,       NO_WEIGHT | NO_KO,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 3a */
	{   8,              2 | NO_VEC,      A_HYPER,               K_L,    0,    8,   10,     0, 0x16,   3,  0x60,       3, SC100,  4,   0,  90, 107,   92,                       0,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 3b */
	{  14, 2 | NO_VEC | FLY_SCREEN,      A_HYPER,               K_L,    0,   10,   10,     0, 0x1a,  15,  0x60,       3, SC100,  4,   0,  90,  99,   80,                       0,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 3c */
	{   2, 2 | NO_VEC | NO_ZU | FLY_SCREEN,      A_HYPER,               K_L,    0,    6,   20,     0, 0x1c,   8,  0x60,       3, SC300,  3,   0,  90, 180,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 3d */
	{   6, 2 | NO_VEC | NO_ZU | FLY_SCREEN,      A_HYPER,               K_L,    0,    4,   20,     0, 0x1f,  10,  0x60,       3, SC300,  2,   0,  90, 170,  172,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 3e: pl0e_at_bite, Death Bite (hyper): each hit of the chain (shl0e_02) */
	{   5, 2 | NO_VEC | NO_ZU | FLY_SCREEN,      A_HYPER,               K_L,    0,    6,   20,     0, 0x18,  10,  0x60,       3, SC300,  3,   0,  90, 174,  176,     NO_CHIP | NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 3f */
	{   6, 2 | NO_VEC | NO_ZU | FLY_SCREEN,      A_HYPER,               K_L,    0,    3,   20,     0, 0x19,  10,  0x60,       3, SC300,  4,   0,  90, 178,  226,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 40 */
	{   6,   BU_O | NO_VEC | NO_ZU,    A_SPECIAL,               K_L,    0,    6,   20,     0, 0x00,  10,  0x60,       3, SC300,  4,   0,  50, 212,  214,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 41: pl0e_at_rush, Venom Rush: Venom and the symbiote hit along the ground (rush_init, shl0e_00) */
	{   6,   BU_O | NO_VEC | NO_ZU,    A_SPECIAL,               K_L,    0,    6,   20,     0, 0x00,   5,  0x60,       3, SC300,  4,   0,  50, 216,  218,     NO_CHIP | NO_WEIGHT,  1,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 42 */
	{  12,   BU_O | NO_VEC | NO_ZU,    A_SPECIAL,               K_L,    0,    6,   20,     0, 0x00,  11,  0x60,       3, SC300,  4,   0,  50, 222,  224,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 43 */
	{   2,      2 | NO_VEC | NO_ZU,      A_HYPER,               K_L,    0,    6,   20,     0, 0x1c,   8,  0x60,       3, SC300,  3,   0,  90, 180,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 44 */
	{   6,      2 | NO_VEC | NO_ZU,      A_HYPER,               K_L,    0,    6,   20,     0, 0x1f,  10,  0x60,       3, SC300,  2,   0,  90, 170,  172,               NO_WEIGHT,  1,     0,  0,     0,     0,   5,        3, { 0, 0 } },	/* 45 */
	{   5,      2 | NO_VEC | NO_ZU,      A_HYPER,               K_L,    0,    6,   20,     0, 0x18,  10,  0x60,       3, SC300,  3,   0,  90, 174,  176,     NO_CHIP | NO_WEIGHT,  1,     0,  0,     0,     0,   5,        3, { 0, 0 } },	/* 46 */
	{   6,      2 | NO_VEC | NO_ZU,      A_HYPER,               K_L,    0,    6,   20,     0, 0x19,  10,  0x60,       3, SC300,  4,   0,  90, 178,  226,               NO_WEIGHT,  1,     0,  0,     0,     0,   5,        3, { 0, 0 } },	/* 47 */
	{  10,             12 | NO_VEC,    A_SPECIAL,               K_L,    0,    4,   24,     0, 0x1c,  12,  0x80,       3, SC300,  8,  64,  90,  64,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 48: pl0e_at_snapback, Snapback (sp_snapback) */
};
