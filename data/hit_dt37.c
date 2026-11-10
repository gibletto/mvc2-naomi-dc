/* Jin (character 37): the attacks (HIT_DATA, 0x1c bytes).
   What each hit does. The character's module sets the attack's number (atck_no) with the move,
   and the row is read when the attack box meets a body. After a row is its number, in
   hexadecimal, and the program's name for it:
   ATCK_ for the normals every character has (player.h), pl37_at_ for its own (pl37_def.h).
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
     box, box2  boxes of hit_37 that the computer measures the attack's reach by
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

const HIT_DATA hit_dt_37[] = {
	/* dmg                   type              kind      elem  piyo  stop  sand  sleep   vec  pow  range     mark  score        se  shc  jug  box  box2                     flag  13  guard  15  catch  abyss  cnt    answer        1a */
	{   2,                    ZU,            A_NOM,      K_S,    4,   12,   10,     0, 0x18,   4,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  19,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 00: ATCK_STAND_LP */
	{   9,         BU_O | NO_VEC,     A_NOM | 0x80, K_LM | 2,    2,   10,   16,     0, 0x00,  11,  0x80, MARK_N2, SC200, SD_NOR_L,   0,  40,  21,    0, NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 01: ATCK_STAND_MP */
	{  16,         BU_O | NO_VEC,     A_NOM | 0x80,  K_L | 2,    1,   14,   40,     0, 0x1a,  15,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  60,  23,    0,                       0,  0,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 02: ATCK_STAND_HP */
	{   6,                    ZU,            A_NOM,     K_LL,    4,   12,   10,     0, 0x1a,   8,  0x60, MARK_N1, SC100, SD_KIK_L,   0,  20,  25,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 03: ATCK_STAND_LK */
	{   6,            2 | NO_VEC,     A_NOM | 0x80,  K_M | 2,    1,    8,   16,     0, 0x1d,   8,  0x60, MARK_N2, SC200, SD_KIK_L,   0,  40,  30,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 04: ATCK_STAND_MK */
	{  16,            2 | NO_VEC,     A_NOM | 0x80,  K_L | 2,    1,   12,   24,     0, 0x1a,  11,  0x80, MARK_N3, SC300, SD_KIK_L,   0,  60,  23,    0,                       0,  0,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 05: ATCK_STAND_HK */
	{   6,                    ZU,            A_NOM,     K_LS,    4,   12,   10,     0, 0x1c,   4,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  40,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 06: ATCK_CROUCH_LP */
	{   9,                    ZU,     A_NOM | 0x80, K_LM | 2,    2,   12,   16,     0, 0x1d,   6,  0x80, MARK_N2, SC200, SD_NOR_L,   0,  40,  42,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 07: ATCK_CROUCH_MP */
	{  16,                  BU_O,     A_NOM | 0x80, K_LL | 2,    1,   10,   40,     0, 0x1a,  14,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  60,  44,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 08: ATCK_CROUCH_HP */
	{   6,                    ZU,             A_LO,     K_LL,    4,   12,   10,     0, 0x1a,   8,  0x70, MARK_N1, SC100, SD_KIK_L,   0,  20,  46,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 09: ATCK_CROUCH_LK */
	{   6,                  BU_O,     A_NOM | 0x80,  K_M | 2,    1,    8,   16,     0, 0x00,   0,  0x40, MARK_N2, SC200, SD_KIK_L,   0,  40,  49,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0a: ATCK_CROUCH_MK */
	{  16,            AS | NO_ZU,      A_LO | 0x80, K_LL | 2,    1,   12,   24,     9, 0x1a,   8,  0x70, MARK_N3, SC300, SD_KIK_L,   0, 100,  56,    0,                       0,  0,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 0b: ATCK_CROUCH_HK */
	{   4,           ZU | NO_VEC,             A_HI,      K_S,    1,   12,   10,     0, 0x00,   5,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  58,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0c: ATCK_JUMP_LP */
	{   7,           ZU | NO_VEC,      A_HI | 0x80, K_LM | 2,    1,   12,   16,     6, 0x00,   0,  0x80, MARK_N2, SC200, SD_NOR_L,   0,  40,  60,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0d: ATCK_JUMP_MP */
	{  15,           ZU | NO_VEC,      A_HI | 0x80, K_LM | 2,    1,   14,   32,     0, 0x14,   8,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  60,  62,    0,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0e: ATCK_JUMP_HP */
	{   4,           ZU | NO_VEC,             A_HI,      K_S,    1,   12,   10,     0, 0x00,   5,  0x60, MARK_N1, SC100, SD_KIK_S,   0,  20,  64,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0f: ATCK_JUMP_LK */
	{   7,           ZU | NO_VEC,      A_HI | 0x80,  K_M | 2,    1,   12,   16,     0, 0x00,   0,  0x60, MARK_N2, SC200, SD_KIK_L,   0,  40,  66,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 10: ATCK_JUMP_MK */
	{   8,           ZU | NO_VEC,      A_HI | 0x80,  K_M | 2,    1,    8,   32,     0, 0x00,   5,  0x80, MARK_N3, SC300, SD_KIK_L,   0,  60,  22,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 11: ATCK_JUMP_HK */
	{   5,           ZU | NO_VEC,     A_NOM | 0x80,  K_L | 2,    1,   12,   24,     0, 0x16,  12,  0x80, MARK_N3, SC300, SD_KIK_L,   0,  60,  70,    0, NO_WEIGHT | AERIAL_FINISH,  0,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 12: pl37_at_jdown_hk, Jumping HK with down, high enough: the dive, three hits (jump_hk_dive) */
	{ 0 },	/* 13 */
	{ 0 },	/* 14 */
	{   9,         BU_O | NO_VEC,        A_SPECIAL, K_LM | 2,    0,   10,   16,     0, 0x00,  11,  0x80, MARK_N2, SC200, SD_NOR_L,   0,  40,  21,    0, NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 15: pl37_at_upper, Launcher, the standing punch */
	{ 0 },	/* 16: pl37_at_upper_k, Launcher, the crouching kick */
	{ 0 },	/* 17: pl37_at_upper_k_2, Crouching kick launcher's later hit (cg_type bit 8) */
	{ 0 },	/* 18 */
	{   3,                     2,     A_NOM | 0x80,  K_M | 2,    1,   10,   16,     0, 0x1a,   6,  0x60, MARK_N2, SC200, SD_KIK_M,   0,  40,  34,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 19: pl37_at_stand_mk_2, Standing MK's later hit, with the flame (cg_type bit 8) */
	{   3,                    ZU,     A_NOM | 0x80,  K_L | 2,    1,   10,   16,     0, 0x1c,  10,  0x40, MARK_N2, SC200, SD_KIK_M,   0,  40,  53,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 1a: pl37_at_crouch_mk_2, Crouching MK's later hit, with the flame */
	{   6,           ZU | NO_VEC,      A_HI | 0x80,  K_M | 2,    1,   10,   32,     0, 0x16,   8,  0x80, MARK_N3, SC300, SD_KIK_L,   0,  60,  68,    0,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 1b: pl37_at_jump_hk_2, Jumping HK's later hit, with the flame */
	{ 0 },	/* 1c */
	{ 0 },	/* 1d */
	{ 0 },	/* 1e */
	{ 0 },	/* 1f */
	{   4,                     2,     A_NOM | 0x40,        0,    4,    8,    0,    28, 0x1a,   8,  0x80, MARK_N1, SC300, SD_NOR_S,   2,   0,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 20: pl37_at_throw_k, Kick throw, mashed hits and the throw (nml_k_mash, catch_no 0) */
	{  16,                     2,     A_NOM | 0x40,        0,    4,   16,    0,    15, 0x1c,   4,  0x80, MARK_N1, SC300,       76,   0,   0,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 21: pl37_at_throw_p, Punch throw, run to the screen's edge (nml_p_run) */
	{  24,                     2, A_SPECIAL | 0x40,        0,    4,   16,    0,    15, 0x1c,   4,  0x80, MARK_N1, SC300,       76,   0,   0,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 22: pl37_at_crush_throw, Saotome Crash's grab slam (crush_c_throw, catch_no 2) */
	{  16,                     2,     A_NOM | 0x40,        0,    4,   16,    0,     4, 0x1e,   4,  0x80, MARK_N1, SC300,       76,   0,   0,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 23: pl37_at_air_throw, Air throw, run to the edge (jump_p_run) */
	{  50,                     2,   A_HYPER | 0x40,        0,    0,   16,    0,    15, 0x00,  12,  0x80, MARK_N1, SC300,       76,   0,   0,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 24: pl37_at_cyclone_throw, Saotome Cyclone's throw after the spin (cyclone_hold) */
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
	{   6,             2 | NO_ZU,        A_SPECIAL,      K_L,    0,    4,   24,     0, 0x00,   2,  0x70,       3,    20, SD_NOR_L,   4,  90,  74,   75,               NO_WEIGHT,  1,     0,  0,     0,     0,   3,        6, { 0, 0 } },	/* 30 */
	{   7,             2 | NO_ZU,        A_SPECIAL,      K_L,    0,    4,   24,     0, 0x00,   2,  0x70,       3,    20, SD_NOR_L,   4,  90,  74,   75,               NO_WEIGHT,  1,     0,  0,     0,     0,   3,        6, { 0, 0 } },	/* 31 */
	{   8,             2 | NO_ZU,        A_SPECIAL,      K_L,    0,    4,   24,     0, 0x00,   2,  0x70,       3,    20, SD_NOR_L,   4,  90,  74,   75,               NO_WEIGHT,  1,     0,  0,     0,     0,   3,        6, { 0, 0 } },	/* 32 */
	{   6, BU_O | NO_VEC | NO_ZU,        A_SPECIAL,      K_L,    0,   10,   24,     0, 0x1b,  13,  0x70,       3,    20, SD_NOR_L,   3,  90, 102,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   3,        4, { 0, 0 } },	/* 33 */
	{   6, BU_O | NO_VEC | NO_ZU,        A_SPECIAL,      K_L,    0,   10,   24,     0, 0x1b,  13,  0x70,       3,    20, SD_NOR_L,   3,  90, 102,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   3,        4, { 0, 0 } },	/* 34 */
	{   6, BU_O | NO_VEC | NO_ZU,        A_SPECIAL,      K_L,    0,   10,   24,     0, 0x1b,  13,  0x70,       3,    20, SD_NOR_L,   3,  90, 102,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   3,        4, { 0, 0 } },	/* 35 */
	{   2,    2 | NO_VEC | NO_ZU,          A_HYPER,      K_L,    0,    4,   24,    10, 0x00,   2,  0x70,       3,    20, SD_NOR_L,   0, 100,  76,    0,                   NO_KO,  1,     0, 39,     0,     0,   0,        3, { 0, 0 } },	/* 36: pl37_at_vulcan, Blodia Vulcan: Jin and the sparks (shl37_01 type 3, sub_type 0) */
	{   4,            ZU | NO_ZU,          A_HYPER,      K_L,    0,    6,   24,    30, 0x18,  12,  0x70,       3,    20, SD_NOR_L,   0,  90,  78,   23,                       0,  1,     0, 38,     0,     0,  20,        3, { 0, 0 } },	/* 37: pl37_at_blodia_beam, Blodia Punch's beam, each hit as it runs out (shl37_02) */
	{  30,        2 | FLY_SCREEN,          A_HYPER,      K_L,    0,    6,   24,    30, 0x1c,  12,  0x70,       3,    20, SD_NOR_L,   0,  90,  78,    0,                 NO_CHIP,  1,     0, 38,     0,     0,  20,        3, { 0, 0 } },	/* 38: pl37_at_blodia_beam_end, Blodia Punch's beam at the far end */
	{   2,          BU_O | NO_ZU,          A_HYPER,      K_L,    0,    0,    0,     0, 0x04,   0,  0x70,       3,    20, SD_NOR_L,   0,  90, 108,  125, NO_SCALE | NO_COMBO | NO_KO,  1,     0, 40,     2,     0,   0,        3, { 0, 0 } },	/* 39: pl37_at_cyclone, Saotome Cyclone's spin, every 16 frames */
	{  10,           10 | NO_VEC,        A_SPECIAL,      K_L,    0,   10,   10,     0, 0x1f,  10,  0x60,       3, SC200,       12,   0,  90, 109,    0, NO_CHIP | NO_WEIGHT | AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 3a: pl37_at_t_kick, Partner jump-in attack */
	{   2,                    ZU,          A_HYPER,      K_L,    0,   10,   24,     0, 0x1c,  12,  0x70,       3,    20, SD_NOR_L,   0,  20,  80,    0,                   NO_KO,  1,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 3b: pl37_at_shine, Saotome Shine (sp_shine, at low vitality) */
	{   5, BU_O | NO_VEC | NO_ZU,        A_SPECIAL,      K_L,    0,    4,   24,     0, 0x1b,  13,  0x70,       3,    20, SD_NOR_L,   3,  90, 102,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   3,        4, { 0, 0 } },	/* 3c */
	{   5, BU_O | NO_VEC | NO_ZU,        A_SPECIAL,      K_L,    0,    4,   24,     0, 0x1b,  13,  0x70,       3,    20, SD_NOR_L,   3,  90, 102,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   3,        4, { 0, 0 } },	/* 3d */
	{   5, BU_O | NO_VEC | NO_ZU,        A_SPECIAL,      K_L,    0,    4,   24,     0, 0x1c,  13,  0x70,       3,    20, SD_NOR_L,   3,  90, 102,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   3,        4, { 0, 0 } },	/* 3e */
	{   5, BU_O | NO_VEC | NO_ZU,        A_SPECIAL,      K_L,    0,    4,   24,     0, 0x1d,  13,  0x70,       3,    20, SD_NOR_L,   3,  90, 102,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   3,        4, { 0, 0 } },	/* 3f */
	{   5, BU_O | NO_VEC | NO_ZU,        A_SPECIAL,      K_L,    0,    4,   24,     0, 0x1e,  13,  0x70,       3,    20, SD_NOR_L,   3,  90, 102,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   3,        4, { 0, 0 } },	/* 40 */
	{   5, BU_O | NO_VEC | NO_ZU,        A_SPECIAL,      K_L,    0,    4,   24,     0, 0x00,  13,  0x70,       3,    20, SD_NOR_L,   3,  90, 102,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   3,        4, { 0, 0 } },	/* 41 */
	{   5, BU_O | NO_VEC | NO_ZU,        A_SPECIAL,      K_L,    0,    4,   24,     0, 0x02,  13,  0x70,       3,    20, SD_NOR_L,   3,  90, 102,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   3,        4, { 0, 0 } },	/* 42 */
	{   5, BU_O | NO_VEC | NO_ZU,        A_SPECIAL,      K_L,    0,    4,   24,     0, 0x03,  13,  0x70,       3,    20, SD_NOR_L,   3,  90, 102,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   3,        4, { 0, 0 } },	/* 43 */
	{   5, BU_O | NO_VEC | NO_ZU,        A_SPECIAL,      K_L,    0,    4,   24,     0, 0x04,  13,  0x70,       3,    20, SD_NOR_L,   3,  90, 102,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   3,        4, { 0, 0 } },	/* 44 */
	{   5, BU_O | NO_VEC | NO_ZU,        A_SPECIAL,      K_L,    0,    4,   24,     0, 0x04,  13,  0x70,       3,    20, SD_NOR_L,   3,  90, 102,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   3,        4, { 0, 0 } },	/* 45 */
	{   6,                    ZU,          A_HYPER,  K_L | 2,    0,   10,   30,     0, 0x08,   4,  0x80,       3, SC300, SD_NOR_L,   0,  90,  23,    0,                   NO_KO,  1,     0, 38,     0,     0,   0,        3, { 0, 0 } },	/* 46: pl37_at_blodia, Blodia Punch, Jin himself (blodia_init) */
	{ 0 },	/* 47 */
	{   5,    2 | NO_VEC | NO_ZU,          A_HYPER,      K_L,    0,    4,   10,    10, 0x06,   1,  0x70,       3,    20, SD_NOR_L,   0, 100,  70,    0,                       0,  1,     0, 39,     0,     0,   6,        3, { 0, 0 } },	/* 48: pl37_at_vulcan_flame_1, Blodia Vulcan's flame, flame_no 0 (shl37_01 type 7) */
	{   5,    2 | NO_VEC | NO_ZU,          A_HYPER,      K_L,    0,    4,   10,    10, 0x1a,   1,  0x70,       3,    20, SD_NOR_L,   0, 100,  70,    0,                       0,  1,     0, 39,     0,     0,   6,        3, { 0, 0 } },	/* 49: pl37_at_vulcan_flame_2, Blodia Vulcan's flame, flame_no 1 */
	{   2,    2 | NO_VEC | NO_ZU,          A_HYPER,      K_L,    0,    4,   24,    10, 0x00,   1,  0x70,       3,    20, SD_NOR_L,   0, 100,  76,    0,         NO_CHIP | NO_KO,  1,     0, 39,     0,     0,   6,        3, { 0, 0 } },	/* 4a: pl37_at_vulcan_spark_2, Blodia Vulcan's spark with a nonzero sub_type (shl37_01 type 3; vulcan_spark only sets 0) */
	{  30,                     2,          A_HYPER,      K_L,    0,    6,   24,    30, 0x1c,  12,  0x70,       3,    20, SD_NOR_L,   0,  90,  78,    0,                 NO_CHIP,  1,     0, 38,     0,     0,  20,        3, { 0, 0 } },	/* 4b: pl37_at_blodia_beam_team, the beam's end hit in a team hyper (team_type 4 or 5) */
	{   8,                  BU_O,        A_SPECIAL,  K_L | 2,    0,   10,   24,     0, 0x1f,   8,  0x70,       3,    20, SD_NOR_L,   0,  20,  80,    0,                       0,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 4c: pl37_at_chouhatsu, the taunt that charges him, each step (sp_chouhatsu) */
	{   0,           ZU | NO_VEC,        A_SPECIAL,      K_L,    0,    0,   20,    15, 0x00,   0,  0x70,       3,    20,        0,   0,  90, 113,    0, NO_SCALE | NO_COMBO | NO_KO,  1,     0, 40,     2,     0,   0,        2, { 0, 0 } },	/* 4d: pl37_at_crush, Saotome Crash, the dash grab (crush_init) */
	{   6,             2 | NO_ZU,        A_SPECIAL,      K_L,    0,    4,   24,     0, 0x00,   2,  0x70,       3,    20, SD_NOR_L,   8,  90,  74,   75,     NO_CHIP | NO_WEIGHT,  1,     0,  0,     0,     0,   3,        6, { 0, 0 } },	/* 4e */
	{   7,             2 | NO_ZU,        A_SPECIAL,      K_L,    0,    4,   24,     0, 0x00,   2,  0x70,       3,    20, SD_NOR_L,   8,  90,  74,   75,     NO_CHIP | NO_WEIGHT,  1,     0,  0,     0,     0,   3,        6, { 0, 0 } },	/* 4f */
	{   8,             2 | NO_ZU,        A_SPECIAL,      K_L,    0,    4,   24,     0, 0x00,   2,  0x70,       3,    20, SD_NOR_L,   8,  90,  74,   75,     NO_CHIP | NO_WEIGHT,  1,     0,  0,     0,     0,   3,        6, { 0, 0 } },	/* 50 */
	{  10,           12 | NO_VEC,        A_SPECIAL,      K_L,    0,    4,   24,     0, 0x1c,  12,  0x80,       3, SC300,        8,  64, 100,  23,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 51: pl37_at_snapback, Snapback (snapback_init, pl_snap_flash) */
};
