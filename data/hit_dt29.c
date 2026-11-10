/* Juggernaut (character 29): the attacks (HIT_DATA, 0x1c bytes).
   What each hit does. The character's module sets the attack's number (atck_no) with the move,
   and the row is read when the attack box meets a body. After a row is its number, in
   hexadecimal, and the program's name for it:
   ATCK_ for the normals every character has (player.h), pl29_at_ for its own (pl29_def.h).
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
     box, box2  boxes of hit_29 that the computer measures the attack's reach by
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

const HIT_DATA hit_dt_29[] = {
	/* dmg                    type              kind  elem  piyo  stop  sand  sleep   vec  pow  range     mark  score        se  shc  jug  box  box2                     flag  13  guard  15  catch  abyss  cnt    answer        1a */
	{   7,            ZU | NO_VEC,            A_NOM,  K_S,    4,   11,   10,     0, 0x18,  10,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  19,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 00: ATCK_STAND_LP */
	{  10,                     ZU,            A_NOM,  K_M,    2,   14,   16,     0, 0x1a,  10,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40,  28,  187,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 01: ATCK_STAND_MP */
	{  17,            ZU | NO_VEC,            A_NOM,  K_L,    1,   17,   24,     0, 0x00,  11,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  60, 148,   39,                       0,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 02: ATCK_STAND_HP */
	{   7,            ZU | NO_VEC,            A_NOM,  K_S,    4,   11,   10,     0, 0x18,  10,  0x60, MARK_N1, SC100, SD_KIK_S,   0,  20,  43,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 03: ATCK_STAND_LK */
	{  10,                     ZU,            A_NOM,  K_M,    2,   14,   16,     0, 0x18,  10,  0x60, MARK_N2, SC200, SD_KIK_M,   0,  40, 174,   57,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 04: ATCK_STAND_MK */
	{  17,                     ZU,            A_NOM,  K_L,    1,   17,   24,     0, 0x1c,  12,  0x80, MARK_N3, SC300, SD_KIK_L,   0,  60,  50,  213,                       0,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 05: ATCK_STAND_HK */
	{   7,            ZU | NO_VEC,            A_NOM, K_LS,    4,   11,   10,     0, 0x18,  10,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  64,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 06: ATCK_CROUCH_LP */
	{   7,                     ZU,            A_NOM,  K_M,    2,   14,   16,     0, 0x1c,   4,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  20,  71,  150,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 07: ATCK_CROUCH_MP */
	{  17,          BU_O | NO_VEC,            A_NOM, K_LL,    1,   10,   24,     0, 0x00,  11,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  60, 181,   78, NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 08: ATCK_CROUCH_HP */
	{   7,            ZU | NO_VEC,             A_LO, K_LS,    4,   11,   10,     0, 0x18,  10,  0x70, MARK_N1, SC100, SD_KIK_S,   0,  20,  89,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 09: ATCK_CROUCH_LK */
	{  10,                     AS,             A_LO, K_LM,    2,   14,   16,     9, 0x18,   8,  0x40, MARK_N2, SC200, SD_KIK_M,   0, 100,  85,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0a: ATCK_CROUCH_MK */
	{  17,                     ZU,            A_NOM, K_LL,    1,   17,   24,     0, 0x1c,  12,  0x70, MARK_N3, SC300, SD_KIK_L,   0,  60,  96,  212,                       0,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 0b: ATCK_CROUCH_HK */
	{   5,            ZU | NO_VEC,             A_HI,  K_S,    1,   11,   10,     0, 0x1e,   4,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  98,  215,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0c: ATCK_JUMP_LP */
	{   8,            ZU | NO_VEC,             A_HI,  K_M,    1,   14,   16,     0, 0x1a,   5,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40, 100,  193,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0d: ATCK_JUMP_MP */
	{  15,            ZU | NO_VEC,             A_HI,  K_L,    1,   17,   24,     0, 0x18,   5,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  60, 104,  185,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 0e: ATCK_JUMP_HP */
	{   5,            ZU | NO_VEC,             A_HI,  K_S,    1,   11,   10,     0, 0x1e,   6,  0x60, MARK_N1, SC100, SD_KIK_S,   0,  20, 106,  216,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0f: ATCK_JUMP_LK */
	{   8,            ZU | NO_VEC,             A_HI,  K_M,    1,   14,   16,     0, 0x19,   5,  0x60, MARK_N2, SC200, SD_KIK_M,   0,  40, 110,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 10: ATCK_JUMP_MK */
	{  15,            ZU | NO_VEC,             A_HI,  K_L,    1,   17,   24,     0, 0x18,   5,  0x80, MARK_N3, SC300, SD_KIK_L,   0,  60, 108,  211,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 11: ATCK_JUMP_HK */
	{  20,                   BU_O,            A_NOM,  K_L,   12,   17,   24,     0, 0x1f,  10,  0x80, MARK_N3, SC300,        8,  10,  60, 116,    0,                       0,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 12: pl29_at_fwd_hp, Forward + HP command normal (Char_Extra 3) */
	{ 0 },	/* 13 */
	{ 0 },	/* 14 */
	{ 0 },	/* 15 */
	{ 0 },	/* 16 */
	{ 0 },	/* 17 */
	{ 0 },	/* 18 */
	{   3,                     ZU,            A_NOM, K_LM,    2,   10,   16,     0, 0x1c,  10,  0x80, MARK_N2, SC200, SD_NOR_M,   3,  20, 150,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 19: pl29_at_crouch_mp_2, Second hit of the crouching MP (shakes the screen) */
	{ 0 },	/* 1a */
	{ 0 },	/* 1b */
	{ 0 },	/* 1c */
	{ 0 },	/* 1d */
	{ 0 },	/* 1e */
	{ 0 },	/* 1f */
	{  16,                      2,     A_NOM | 0x40,    0,    4,   16,    0,    15, 0x1e,   2,  0x80, MARK_N1, SC300,       76,   0,   0,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 20: pl29_at_throw_k, K throw */
	{   3,                      2,     A_NOM | 0x40,    0,    4,   16,    0,    15, 0x1c,   8,  0x80, MARK_N1, SC300,       76,   1,   0,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 21: pl29_at_throw_p_hit, P throw: the hit while holding */
	{  10,                      2,     A_NOM | 0x40,    0,    4,   16,    0,    15, 0x1b,   8,  0x80, MARK_N1, SC300,       76,   2,   0,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 22: pl29_at_throw_p, P throw: the release */
	{  16,                      2,     A_NOM | 0x40,    0,    0,   16,    0,    15, 0x14,   4,  0x80, MARK_N1, SC300, SD_NOR_S,   0,   0,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 23: pl29_at_air_throw, P air throw */
	{   5,                      2,     A_NOM | 0x40,    0,    4,   16,    0,    15, 0x1c,   8,  0x80,       3, SC300,       76,   1,   0,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 24 */
	{  16,                      2,     A_NOM | 0x40,    0,    4,   16,    0,    15, 0x1c,   8,  0x80,       3, SC300,       76,   0,   0,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 25 */
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
	{  20, 2 | NO_ZU | FLY_SCREEN, A_SPECIAL | 0x80,  K_L,    0,   10,   20,    15, 0x1a,  15,  0x80,       3, SC300,        8,   0,  90, 210,  130,                       0,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 30 */
	{  24, 2 | NO_ZU | FLY_SCREEN, A_SPECIAL | 0x80,  K_L,    0,   10,   20,    15, 0x1a,  15,  0x80,       3, SC300,        8,   0,  90, 210,  130,                       0,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 31 */
	{  28, 2 | NO_ZU | FLY_SCREEN, A_SPECIAL | 0x80,  K_L,    0,   10,   20,    15, 0x1a,  15,  0x80,       3, SC300,        8,   0,  90, 210,  130,                       0,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 32 */
	{  10,                     ZU,   A_HYPER | 0x80, K_LL,    0,    6,    0,    10, 0x1c,   0,  0x80,       3, SC300,        8,   0,  90, 137,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,  20,        6, { 0, 0 } },	/* 33: pl29_at_headcrush, Juggernaut Headcrush hyper: the repeating charge */
	{  28,     2 | NO_VEC | NO_ZU,   A_HYPER | 0x80,  K_L,    0,   10,    0,    10, 0x01,  12,  0x80,       3, SC300,        8,   0,  90, 147,    0,                NO_SCALE,  1,     0,  0,     0,     0,  20,        6, { 0, 0 } },	/* 34: pl29_at_headcrush_last, Juggernaut Headcrush: the finishing hit */
	{   4,             ZU | NO_ZU, A_SPECIAL | 0x80,  K_L,    0,    8,   16,     0, 0x18,   8,  0x80,       3, SC300,        8,   1,  18, 199,  196,                       0,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 35 */
	{   4,             ZU | NO_ZU, A_SPECIAL | 0x80,  K_L,    0,    8,   16,     0, 0x18,   8,  0x80,       3, SC300,        8,   1,  10, 199,  196,                       0,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 36 */
	{   4,             ZU | NO_ZU, A_SPECIAL | 0x80,  K_L,    0,    8,   16,     0, 0x18,  10,  0x80,       3, SC300,        8,   1,  10, 199,  196,                       0,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 37 */
	{   8,                     ZU, A_SPECIAL | 0x80,  K_L,    0,   10,   16,     0, 0x18,  10,  0x80,       3, SC300,        8,   1,  90, 161,  123,                       0,  1,     0,  0,     0,     0,   0,        7, { 0, 0 } },	/* 38: pl29_at_quake, Earthquake Punch */
	{  10,            ZU | NO_VEC,            A_NOM,  K_S,    2,   10,   10,     0, 0x18,  10,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  19,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 39 */
	{  10,            ZU | NO_VEC,            A_NOM,  K_S,    2,   10,   10,     0, 0x18,  10,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  19,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 3a */
	{  18,             2 | NO_VEC, A_HI | A_SPECIAL,  K_L,    0,   10,   20,    15, 0x1a,   7,  0x80,       3, SC300,        8,   0,  90, 160,  211,                       0,  1,     0,  0,     0,     0,   0,        7, { 0, 0 } },	/* 3b */
	{  20,             2 | NO_VEC, A_HI | A_SPECIAL,  K_L,    0,   10,   20,    15, 0x1a,   7,  0x80,       3, SC300,        8,   0,  90, 160,  211,                       0,  1,     0,  0,     0,     0,   0,        7, { 0, 0 } },	/* 3c */
	{  22,             2 | NO_VEC, A_HI | A_SPECIAL,  K_L,    0,   10,   20,    15, 0x1a,   7,  0x80,       3, SC300,        8,   0,  90, 160,  211,                       0,  1,     0,  0,     0,     0,   0,        7, { 0, 0 } },	/* 3d */
	{  10,            10 | NO_VEC,        A_SPECIAL,  K_L,    0,   10,   10,     0, 0x1f,  10,  0x60,       3, SC200,        8,   0,  90, 219,    0, NO_CHIP | NO_WEIGHT | AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        7, { 0, 0 } },	/* 3e: pl29_at_t_kick, Partner's jump-in attack (pl29_t_kick) */
	{  20, 2 | NO_ZU | FLY_SCREEN, A_SPECIAL | 0x80,  K_L,    0,   10,   20,    15, 0x1a,  15,  0x80,       3, SC300,        8,   0,  90, 210,  130,                       0,  1,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 3f: pl29_at_punch_vc, Juggernaut Punch as a variable counter (team_type 3) */
	{  10,            12 | NO_VEC,        A_SPECIAL,  K_L,    0,    4,   24,     0, 0x1c,  12,  0x80,       3, SC300,        8,  64,  90,  28,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 40: pl29_at_snapback, Snapback (sp_snapback) */
	{  18,          BU_O | NO_VEC,        A_SPECIAL, K_LL,    0,   10,   24,     0, 0x00,  11,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  60, 181,   78, NO_CHIP | NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 41: pl29_at_upper, Launcher (pl29_upper) */
};
