/* Jill (character 11): the attacks (HIT_DATA, 0x1c bytes).
   What each hit does. The character's module sets the attack's number (atck_no) with the move,
   and the row is read when the attack box meets a body. After a row is its number, in
   hexadecimal, and the program's name for it:
   ATCK_ for the normals every character has (player.h), pl11_at_ for its own (pl11_def.h).
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
     box, box2  boxes of hit_11 that the computer measures the attack's reach by
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

const HIT_DATA hit_dt_11[] = {
	/* dmg                   type              kind      elem  piyo  stop  sand  sleep   vec  pow  range     mark  score        se  shc  jug  box  box2                     flag  13  guard  15  catch  abyss  cnt    answer        1a */
	{   4,                    ZU,            A_NOM,      K_S,    4,   10,   10,     0, 0x18,   4,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  32,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 00: ATCK_STAND_LP */
	{   7,                    ZU,            A_NOM,      K_M,    2,   11,   16,     0, 0x18,   6,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40,  33,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 01: ATCK_STAND_MP */
	{  13,                    ZU,            A_NOM,      K_L,    1,   12,   24,     0, 0x1a,   8,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  50,  34,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 02: ATCK_STAND_HP */
	{   4,                    ZU,            A_NOM,      K_S,    4,   10,   10,     0, 0x18,   4,  0x60, MARK_N1, SC100, SD_KIK_S,   0,  20,  35,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 03: ATCK_STAND_LK */
	{   7,                    ZU,            A_NOM,      K_M,    2,   10,   16,     0, 0x18,   6,  0x60, MARK_N2, SC200, SD_KIK_M,   0,  30,  36,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 04: ATCK_STAND_MK */
	{  13,                    ZU,            A_NOM,      K_L,    1,   12,   24,     0, 0x1a,   8,  0x80, MARK_N3, SC300, SD_KIK_L,   0,  30,  37,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 05: ATCK_STAND_HK */
	{   4,                    ZU,            A_NOM,     K_LS,    4,   10,   10,     0, 0x1c,   4,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  38,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 06: ATCK_CROUCH_LP */
	{   7,                    ZU,            A_NOM,     K_LM,    2,   11,   16,     0, 0x1d,   6,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40,  39,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 07: ATCK_CROUCH_MP */
	{  13,         BU_O | NO_VEC,            A_NOM,     K_LL,    1,   10,   24,     0, 0x00,  11,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  50,  41,   40, NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 08: ATCK_CROUCH_HP */
	{   4,                    ZU,             A_LO,     K_LS,    4,   10,   10,     0, 0x1c,   4,  0x70, MARK_N1, SC100, SD_KIK_S,   0,  20,  42,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 09: ATCK_CROUCH_LK */
	{   7,                    ZU,             A_LO,     K_LM,    2,   11,   16,     0, 0x1c,   6,  0x40, MARK_N2, SC200, SD_KIK_M,   0,  40,  43,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0a: ATCK_CROUCH_MK */
	{  13,                    AS,             A_LO,     K_LL,    1,   12,   24,     9, 0x1a,   8,  0x70, MARK_N3, SC300, SD_KIK_L,   0,  90,  44,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0b: ATCK_CROUCH_HK */
	{   4,           ZU | NO_VEC,             A_HI,      K_S,    1,   10,   10,     0, 0x00,   5,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  45,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0c: ATCK_JUMP_LP */
	{   7,           ZU | NO_VEC,             A_HI,      K_M,    1,   10,   16,     0, 0x00,   3,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40,  46,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0d: ATCK_JUMP_MP */
	{  13,           ZU | NO_VEC,             A_HI,      K_L,    1,   12,   32,     0, 0x16,   8,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  60,  47,   48,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0e: ATCK_JUMP_HP */
	{   4,           ZU | NO_VEC,             A_HI,      K_S,    1,   10,   10,     0, 0x00,   5,  0x60, MARK_N1, SC200, SD_KIK_S,   0,  20,  49,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0f: ATCK_JUMP_LK */
	{   7,           ZU | NO_VEC,             A_HI,      K_M,    1,   10,   16,     0, 0x00,   3,  0x60, MARK_N2, SC200, SD_KIK_M,   0,  40,  50,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 10: ATCK_JUMP_MK */
	{  13,           ZU | NO_VEC,             A_HI,      K_L,    1,   12,   32,     0, 0x15,   8,  0x60, MARK_N3, SC200, SD_KIK_L,   0,  60,  51,    0,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 11: ATCK_JUMP_HK */
	{ 0 },	/* 12 */
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
	{  16,                  BU_O,     A_NOM | 0x40,     K_LM,    4,   10,   20,     0, 0x1c,   9,  0x60, MARK_N2, SC200,       14,   0,  40,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 20 */
	{  16,                  BU_O,     A_NOM | 0x40,     K_LM,    4,   10,   20,     0, 0x1c,   9,  0x60, MARK_N2, SC200,       14,   0,  40,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 21 */
	{   2,                  BU_O, A_SPECIAL | 0x40,      K_L,    0,   10,   20,     0, 0x1c,   8,  0x60, MARK_N2, SC200, SD_NOR_M,   0,  40,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 22: pl11_at_counter_hit, Each hit on the opponent held by sp_catch (the counter's throw) */
	{  10,                  BU_O, A_SPECIAL | 0x40,      K_L,    4,   12,   20,     0, 0x1c,   8,  0x60, MARK_N2, SC200, SD_NOR_M,   0,  40,   0,    0,                NO_SCALE,  0,     0, 10,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 23: pl11_at_counter_release, the counter's throw lets the opponent go (sp_catch_release) */
	{   8,                     2, A_SPECIAL | 0x40,      K_L,    4,   12,   20,     0, 0x1c,   8,  0x60, MARK_N2, SC200, SD_NOR_M,   0,  40,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 24: pl11_at_counter_end, the counter's throw's last shot (sp_catch_end) */
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
	{  14, BU_O | NO_VEC | NO_ZU,        A_SPECIAL,      K_L,    0,    6,   20,     0, 0x1c,   9,  0x60,       3, SC200,        8,   0,  50,  53,    0,                       0,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 30: pl11_at_cerberus, Cerberus Escape: Jill and the dog (cerberus_init, shl11_00 type 0) */
	{  14,   ZU | NO_VEC | NO_ZU,        A_SPECIAL,      K_L,    0,    6,   20,     0, 0x1c,   9,  0x60,       3, SC200,        8,   0,  50,  55,    0,                       0,  1,     0,  0,     0,     0,   0,        5, { 0, 0 } },	/* 31: pl11_at_crow, Crow Escape: Jill and the crows (crow_init, shl11_00 type 1) */
	{   2,   ZU | NO_VEC | NO_ZU,        A_SPECIAL,      K_L,    0,    6,   20,     0, 0x19,   9,  0x60,       3, SC200,        8,   0,  90,  57,    0,                       0,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 32: pl11_at_zombie, Zombie Escape: the thing that catches and carries (zombie_init, shl11_01 type 0) */
	{  10,           10 | NO_VEC,        A_SPECIAL,      K_L,    0,   10,   10,     0, 0x1f,  10,  0x60,       3, SC200,       12,   0,  90,  49,    0, NO_CHIP | NO_WEIGHT | AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 33: pl11_at_t_kick, the jump-in attack (pl11_t_kick) */
	{  10,           12 | NO_VEC,        A_SPECIAL,      K_L,    0,    4,   24,     0, 0x1c,  12,  0x80,       3, SC300,       12,  64,  90,  37,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 34: pl11_at_snapback, Snapback (sp_snapback) */
	{  14,         BU_O | NO_VEC,        A_SPECIAL,     K_LL,    0,   10,   24,     0, 0x00,  11,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  40, 128,  129, NO_CHIP | NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 35: pl11_at_upper, the launcher (pl11_upper, crouching) */
	{  16,    2 | NO_VEC | NO_ZU,        A_SPECIAL,  K_L | 2,    0,    8,   30,     0, 0x1d,   8,  0x80,       3, SC300,       21,   0,  90,  60,    0,                       0,  1,     0,  0,     0,     0,   0,        5, { 0, 0 } },	/* 36: pl11_at_grenade, Grenade Launcher's shot (grenade_init, shl11_02_a_shot) */
	{   2,    2 | NO_VEC | NO_ZU,        A_SPECIAL,  K_L | 2,    0,    8,   30,     0, 0x1d,  10,  0x80,       3, SC300,       21,   0,  90,  62,    0,                       0,  1,     0,  0,     0,     0,   0,        5, { 0, 0 } },	/* 37: pl11_at_grenade_blast, Grenade Launcher's shot at the end of its flight (shl11_02_a_fly) */
	{   8, BU_O | NO_VEC | NO_ZU,        A_SPECIAL,      K_S,    0,    8,   30,     0, 0x1a,   2,  0x80,       3, SC300,        8,   0,  90,  52,    0,                       0,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 38 */
	{  10,    2 | NO_VEC | NO_ZU,        A_SPECIAL,      K_L,    0,   10,   30,     0, 0x1a,  11,  0x80,       3, SC300,        8,   0,  90,  52,    0,                       0,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 39 */
	{   8, BU_O | NO_VEC | NO_ZU,        A_SPECIAL,     K_LS,    0,    8,   30,     0, 0x1a,   2,  0x80,       3, SC300,        8,   0,  90,  76,    0,                       0,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 3a: pl11_at_stars_air, Charging Stars in the air: start and first rush hits (stars_j_init, stars_j_rush) */
	{  10,          BU_O | NO_ZU,        A_SPECIAL,      K_L,    0,   10,   30,     0, 0x1a,  11,  0x80,       3, SC300,        8,   0,  90,  76,    0,                       0,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 3b: pl11_at_stars_air_end, Air Charging Stars' finishing hit as the rush ends (stars_j_rush) */
	{   4, BU_O | NO_VEC | NO_ZU,          A_HYPER,      K_S,    0,    8,   30,     0, 0x1a,   2,  0x80,       3, SC300,        8,   0,  90,  52,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 3c: pl11_at_hstars, Hyper Charging Stars on the ground: its rush hits (hstars_start, hstars_rush) */
	{  20, BU_O | NO_VEC | NO_ZU,          A_HYPER,      K_L,    0,   10,   30,     0, 0x1a,  11,  0x80,       3, SC300,        8,   0,  90,  52,    0,                       0,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 3d: pl11_at_hstars_end, Ground Hyper Charging Stars' finishing hit (hstars_rush) */
	{   4, BU_O | NO_VEC | NO_ZU,          A_HYPER,     K_LS,    0,    8,   30,     0, 0x1a,   2,  0x80,       3, SC300,        8,   0,  90,  76,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 3e: pl11_at_hstars_air, Hyper Charging Stars in the air: its rush hits (hstars_j_start, hstars_j_rush) */
	{  20, BU_O | NO_VEC | NO_ZU,          A_HYPER,      K_L,    0,   10,   30,     0, 0x1a,  11,  0x60,       3, SC200,        8,   0,  90,  76,    0,                       0,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 3f: pl11_at_hstars_air_end, Air Hyper Charging Stars' finishing hit (hstars_j_rush) */
	{   5,   ZU | NO_VEC | NO_ZU,          A_HYPER, K_LM | 2,    0,   10,   30,     0, 0x1a,   8,  0x60,       3, SC200,       21,   0,  90,  63,    0,                       0,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 40: pl11_at_tyrant_shot, Tyrant Escape's follow-up command: Jill and the shot (tyrant_com_ck, shl11_02 type 2) */
	{   3,   ZU | NO_VEC | NO_ZU,          A_HYPER, K_LM | 2,    0,   10,   30,     0, 0x1a,   8,  0x60,       3, SC200,       21,   0,  90,  62,    0,                 NO_CHIP,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 41 */
	{   1,    2 | NO_VEC | NO_ZU,          A_HYPER,     K_LL,    0,   10,   20,     8, 0x00,   6,  0x60,       3, SC200,        8,   0,  90,  65,    0,       NO_WEIGHT | NO_KO,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 42: pl11_at_tyrant, Tyrant Escape (hyper): start and the reaching hand (tyrant_init, shl11_03_b_wait) */
	{   4,    2 | NO_VEC | NO_ZU,   A_HYPER | 0x40,     K_LL,    0,   10,   20,     8, 0x01,  15,  0x60,       3, SC200,        8,   0,  90,   0,    0,       NO_WEIGHT | NO_KO,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 43: pl11_at_tyrant_hit, Tyrant Escape: the hits while the hand holds (shl11_03_b_near_hit, _far_hit and _last_hit) */
	{  36,    2 | NO_VEC | NO_ZU,   A_HYPER | 0x40,     K_LL,    0,   10,   20,     8, 0x01,  15,  0x60,       3, SC200,        8,   0,  90,   0,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 44: pl11_at_tyrant_hit_last, Tyrant Escape: the last hit of the hold (shl11_03_b_last_hit) */
	{  15,    2 | NO_VEC | NO_ZU,          A_HYPER, K_LL | 2,    0,    6,   20,     0, 0x1e,  11,  0x60,       3, SC200,       21,   0,  90,  67,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 45: pl11_at_tyrant_finish, Tyrant Escape's follow-up attack (shl11_03_b_shot_wait, after tyrant_throw_flag) */
	{  15, BU_O | NO_VEC | NO_ZU,        A_SPECIAL, K_LL | 2,    0,   10,   30,     0, 0x1b,  11,  0x60,       3, SC200,        8,   0,  90,  68,    0,                       0,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 46: pl11_at_zombie_h, Zombie Escape, the stronger version (zombie_init by atck_level, shl11_01 type 2) */
	{ 0 },	/* 47 */
	{  10,           ZU | NO_VEC,        A_SPECIAL,      K_L,    0,    0,   10,     0, 0x19,   6,  0x60,       3, SC200,        8,   0,  90,  70,    0,    NO_WEIGHT | NO_COMBO,  1,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 48: pl11_at_counter, the counter's hit after taking a blow (return_flash) */
	{  10,    2 | NO_VEC | NO_ZU,          A_HYPER, K_LL | 2,    0,   10,   30,     0, 0x1b,  11,  0x60,       3, SC200,       21,   0,  90,  63,    0,                       0,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 49 */
	{  10,    2 | NO_VEC | NO_ZU,          A_HYPER, K_LL | 2,    0,   10,   30,     0, 0x1b,  11,  0x60,       3, SC200,       21,   0,  90,  62,    0,                       0,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 4a */
	{   1,         BU_O | NO_VEC,          A_HYPER,     K_LL,    0,    1,   60,     0, 0x1d,  12,  0x60,       3, SC200,        8,   0,  90,  71,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 4b: pl11_at_rocket, Rocket Launcher (hyper) (rocket_init) */
	{   3, BU_O | NO_VEC | NO_ZU,        A_SPECIAL,      K_S,    0,    8,   30,     0, 0x19,   4,  0x80,       3, SC300,        8,   0,  90,  52,    0,                       0,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 4c */
	{   3, BU_O | NO_VEC | NO_ZU,        A_SPECIAL,     K_LS,    0,    8,   30,     0, 0x19,   4,  0x80,       3, SC300,        8,   0,  90,  76,    0,                       0,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 4d: pl11_at_stars_air_2, Air Charging Stars' later rush hits (stars_j_rush, stars_hit_cnt > 1) */
	{   2,   ZU | NO_VEC | NO_ZU,        A_SPECIAL,      K_L,    0,    6,   20,     0, 0x19,   9,  0x60,       3, SC200,        8,   0,  90,  72,    0,                       0,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 4e: pl11_at_zombie_fall, Zombie Escape's piece landing from its fall (shl11_01_a_fall_bound) */
	{  14,   ZU | NO_VEC | NO_ZU,        A_SPECIAL,      K_L,    0,    6,   20,     0, 0x1c,   9,  0x60,       3, SC200,        8,   0,  50,  55,    0,                       0,  1,     0,  0,     0,     0,   0,        5, { 0, 0 } },	/* 4f: pl11_at_crow_vc, Crow Escape as a variable counter (team_type 3) */
	{   8, BU_O | NO_VEC | NO_ZU,        A_SPECIAL,      K_S,    0,    8,   30,     0, 0x1a,   2,  0x80,       3, SC300,        8,   0,  90,  52,    0,                       0,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 50 */
	{  10,    2 | NO_VEC | NO_ZU,        A_SPECIAL,      K_L,    0,   10,   30,     0, 0x1a,  11,  0x80,       3, SC300,        8,   0,  90,  52,    0,                       0,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 51 */
	{   3, BU_O | NO_VEC | NO_ZU,        A_SPECIAL,      K_S,    0,    8,   30,     0, 0x1a,   2,  0x80,       3, SC300,        8,   0,  90,  52,    0,                       0,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 52 */
	{  15,    2 | NO_VEC | NO_ZU,        A_SPECIAL,  K_L | 2,    0,    8,   30,     0, 0x1d,   8,  0x80,       3, SC300,       21,   0,  90,  60,    0,                       0,  1,     0,  0,     0,     0,   0,        5, { 0, 0 } },	/* 53: pl11_at_grenade_vc, Grenade Launcher's shot as a variable counter (team_type 3) */
	{   2,    2 | NO_VEC | NO_ZU,        A_SPECIAL,  K_L | 2,    0,    8,   30,     0, 0x1d,  10,  0x80,       3, SC300,       21,   0,  90,  62,    0,                       0,  1,     0,  0,     0,     0,   0,        5, { 0, 0 } },	/* 54: pl11_at_grenade_blast_vc, Grenade Launcher's end-of-flight hit as a variable counter (team_type 3) */
};
