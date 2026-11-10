/* Sentinel (character 34): the attacks (HIT_DATA, 0x1c bytes).
   What each hit does. The character's module sets the attack's number (atck_no) with the move,
   and the row is read when the attack box meets a body. After a row is its number, in
   hexadecimal, and the program's name for it:
   ATCK_ for the normals every character has (player.h), pl34_at_ for its own (pl34_def.h).
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
     box, box2  boxes of hit_34 that the computer measures the attack's reach by
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

const HIT_DATA hit_dt_34[] = {
	/* dmg                   type              kind  elem  piyo  stop  sand  sleep   vec  pow  range     mark  score        se  shc  jug  box  box2                     flag  13  guard  15  catch  abyss  cnt    answer        1a */
	{   6,           ZU | NO_VEC,     A_NOM | 0x80, K_LS,    4,   10,   10,     0, 0x14,   4,  0x70, MARK_N2, SC100,        1,   0,  20,  60,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 00: ATCK_STAND_LP */
	{   6,           ZU | NO_VEC,     A_NOM | 0x80, K_LM,    2,   11,   16,     0, 0x1c,   6,  0x80, MARK_N3, SC200,        2,   0,  40,  54,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 01: ATCK_STAND_MP */
	{  10, BU_O | NO_VEC | NO_ZU,     A_NOM | 0x80,  K_L,    1,   12,   24,     0, 0x1a,   6,  0x80, MARK_N3, SC300,        3,   0,  50,  90,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 02: ATCK_STAND_HP */
	{   5,           ZU | NO_VEC,     A_NOM | 0x80, K_LS,    4,   10,   10,     0, 0x1a,   4,  0x60, MARK_N2, SC100, SD_KIK_S,   0,  20,  56,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 03: ATCK_STAND_LK */
	{   5,           ZU | NO_VEC,     A_NOM | 0x80, K_LM,    2,   10,   16,     0, 0x1e,   5,  0x60, MARK_N3, SC200, SD_KIK_M,   0,  40,  58,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 04: ATCK_STAND_MK */
	{  18,         BU_O | NO_VEC,     A_NOM | 0x80, K_LL,    1,   12,   24,     0, 0x00,  11,  0x80, MARK_N3, SC300, SD_KIK_L,   0,  60,  13,   21, NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 05: ATCK_STAND_HK */
	{   6,           ZU | NO_VEC,     A_NOM | 0x80, K_LS,    4,   10,   10,     0, 0x16,   4,  0x70, MARK_N2, SC100,        1,   0,  20,  61,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 06: ATCK_CROUCH_LP */
	{   6,           ZU | NO_VEC,     A_NOM | 0x80, K_LM,    2,   11,   16,     0, 0x1c,   6,  0x80, MARK_N3, SC200,        2,   0,  40,  63,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 07: ATCK_CROUCH_MP */
	{  10, BU_O | NO_VEC | NO_ZU,     A_NOM | 0x80, K_LL,    1,   10,   24,     0, 0x1a,   6,  0x80, MARK_N3, SC300,        3,   0,  50,  90,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 08: ATCK_CROUCH_HP */
	{   6,           ZU | NO_VEC,      A_LO | 0x80, K_LS,    4,   10,   10,     0, 0x18,   4,  0x70, MARK_N2, SC100, SD_KIK_S,   0,  20,  66,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 09: ATCK_CROUCH_LK */
	{   6,           ZU | NO_VEC,      A_LO | 0x80, K_LM,    2,   11,   16,     0, 0x18,   5,  0x40, MARK_N3, SC200, SD_KIK_M,   0,  40,  68,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0a: ATCK_CROUCH_MK */
	{   6,           ZU | NO_VEC,      A_LO | 0x80, K_LL,    1,   12,   24,     9, 0x18,   6,  0x70, MARK_N3, SC300, SD_KIK_L,   0,  50,  68,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0b: ATCK_CROUCH_HK */
	{   5,           ZU | NO_VEC,      A_HI | 0x80,  K_S,    1,   10,   10,     0, 0x00,   7,  0x70, MARK_N2, SC100,        1,   0,  20,  72,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0c: ATCK_JUMP_LP */
	{   8,           ZU | NO_VEC,      A_HI | 0x80,  K_M,    1,   10,   16,     0, 0x00,   6,  0x80, MARK_N3, SC200,        2,   0,  40,  34,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0d: ATCK_JUMP_MP */
	{  18,           ZU | NO_VEC,      A_HI | 0x80,  K_L,    1,   12,   32,     0, 0x16,  15,  0x80, MARK_N3, SC300,        3,   0,  60,  39,    0,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0e: ATCK_JUMP_HP */
	{   5,           ZU | NO_VEC,      A_HI | 0x80,  K_S,    1,   10,   10,     0, 0x00,   7,  0x60, MARK_N2, SC200, SD_KIK_S,   0,  20,  41,   42,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0f: ATCK_JUMP_LK */
	{   8,           ZU | NO_VEC,      A_HI | 0x80,  K_M,    1,   10,   16,     0, 0x00,   6,  0x60, MARK_N3, SC200, SD_KIK_M,   0,  40,  45,   46,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 10: ATCK_JUMP_MK */
	{  15,           ZU | NO_VEC,      A_HI | 0x80,  K_L,    1,   12,   32,     0, 0x15,  15,  0x60, MARK_N3, SC200, SD_KIK_L,   0,  60,  49,   50,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 11: ATCK_JUMP_HK */
	{   3,           ZU | NO_VEC,      A_LO | 0x80, K_LM,    2,   11,   16,     0, 0x18,   5,  0x40, MARK_N3, SC200, SD_KIK_M,   0,  40,  68,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 12 */
	{   3,           ZU | NO_VEC,     A_NOM | 0x80, K_LM,    2,   11,   16,     0, 0x1c,   6,  0x80, MARK_N3, SC200,        2,   0,  40,  54,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 13: pl34_at_stand_mp_2, Second hit of the standing medium punch */
	{   3,           ZU | NO_VEC,     A_NOM | 0x80, K_LM,    2,   11,   16,     0, 0x1c,   6,  0x80, MARK_N3, SC200,        2,   0,  40,  63,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 14: pl34_at_crouch_mp_2, Second hit of the crouching medium punch */
	{   6,           ZU | NO_VEC,      A_LO | 0x80, K_LL,    1,   10,   16,     0, 0x18,   6,  0x60, MARK_N3, SC200,        3,   0,  50,  70,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 15 */
	{   6,           AS | NO_VEC,      A_LO | 0x80, K_LL,    1,   10,   16,     0, 0x18,   6,  0x60, MARK_N3, SC200,        3,   0,  50,  70,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 16 */
	{ 0 },	/* 17 */
	{ 0 },	/* 18 */
	{ 0 },	/* 19 */
	{ 0 },	/* 1a */
	{ 0 },	/* 1b */
	{ 0 },	/* 1c */
	{ 0 },	/* 1d */
	{ 0 },	/* 1e */
	{ 0 },	/* 1f */
	{  16,                     2,     A_NOM | 0x40, K_LM,    4,   10,   20,     0, 0x1a,   4,  0x60, MARK_N2, SC200,        2,   0,  40,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 20: pl34_at_throw, the throws (ground and air): the grabbing fist slams the opponent into the wall (shl34_00_catch_fly) */
	{   8,            ZU | NO_ZU, A_SPECIAL | 0x80,  K_L,    0,   10,   20,     0, 0x1a,   4,  0x60,       3, SC200,        3,   0,  50,  86,    0,                       0,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 21: pl34_at_force, Sentinel Force (sp_force): him and the drones (shl34_01 types 0 and 1) */
	{   4,            ZU | NO_ZU, A_SPECIAL | 0x80,  K_M,    0,   10,   20,     0, 0x14,   4,  0x60,       3, SC200,       21,   0,   5,  88,    0,                       0,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 22: pl34_at_force_shot, the shot a drone drops (shl34_02) */
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
	{  25,         BU_O | NO_VEC, A_SPECIAL | 0x80,  K_L,    0,   10,   30,     0, 0x1e,   8,  0x80,       3, SC300,        4,   0,  90, 106,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 2e: pl34_at_rocket2, Strongest Rocket Punch's fist on the ground (shl34_00 case 2); plasma_init (Plasma Storm) also sets it */
	{  25,         BU_O | NO_VEC, A_SPECIAL | 0x80,  K_L,    0,   10,   30,     0, 0x1e,   8,  0x80,       3, SC300,        4,   0,  90, 106,    0, NO_WEIGHT | AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 2f: pl34_at_jrocket2, Strongest Rocket Punch's fist in the air (shl34_00 case 5) */
	{  25,         BU_O | NO_VEC, A_SPECIAL | 0x80,  K_L,    0,   10,   30,     0, 0x1e,   8,  0x80,       3, SC300,        4,   0,  90,  77,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 30: pl34_at_rocket, Rocket Punch: him, and the ground heavy fist (also the Variable Counter's light fist) */
	{  25,         BU_O | NO_VEC, A_SPECIAL | 0x80,  K_L,    0,   10,   30,     0, 0x1e,   8,  0x80,       3, SC300,        4,   0,  90,  78,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 31: pl34_at_rocket_l, Rocket Punch's ground light fist */
	{  25,         BU_O | NO_VEC, A_SPECIAL | 0x80,  K_L,    0,   10,   30,     0, 0x1e,   8,  0x80,       3, SC300,        4,   0,  90,  77,    0, NO_WEIGHT | AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 32: pl34_at_jrocket, Rocket Punch's air heavy fist */
	{  25,         BU_O | NO_VEC, A_SPECIAL | 0x80,  K_L,    0,   10,   30,     0, 0x1e,   8,  0x80,       3, SC300,        4,   0,  90,  78,    0, NO_WEIGHT | AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        5, { 0, 0 } },	/* 33: pl34_at_jrocket_l, Rocket Punch's air light fist */
	{  10,           12 | NO_VEC,        A_SPECIAL,  K_L,    0,    4,   24,     0, 0x1c,  12,  0x80,       3, SC300,        4,  64,  90,  90,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0,        5, { 0, 0 } },	/* 34: pl34_at_snapback, Snapback (sp_snapback), a kick that leaves the heavy kick's effect (also shl34_03 type 2) */
	{  17,         BU_O | NO_VEC, A_SPECIAL | 0x80, K_LL,    1,   12,   24,     0, 0x00,  11,  0x80, MARK_N3, SC300, SD_KIK_L,   0,  60,  13,   21, NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 35: pl34_at_upper, the kick launcher (pl34_upper, atck_pk 1); also sp_stand_hk's standing heavy kick as Variable Counter (team_type 3) */
	{  10,           10 | NO_VEC,        A_SPECIAL,  K_L,    0,   10,   10,     0, 0x1f,  10,  0x60,       3, SC200,        4,   0,  90,  97,    0, NO_CHIP | NO_WEIGHT | AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        5, { 0, 0 } },	/* 36: pl34_at_t_kick, the partner's jump-in attack (t_kick) */
	{   6,            ZU | NO_ZU,   A_HYPER | 0x80,  K_L,    0,   10,   30,     0, 0x1e,   0,  0x80,       3, SC300,       17,   0,  90,  91,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   8,        3, { 0, 0 } },	/* 37: pl34_at_plasma, Plasma Storm's blow in front of him (shl34_04, set by plasma_wait) */
	{  10,          BU_O | NO_ZU,          A_HYPER,  K_L,    0,   10,   30,     0, 0x1e,  15,  0x80,       3, SC300,       18,   0,  90,  91,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   8,        3, { 0, 0 } },	/* 38 */
	{   8,             2 | NO_ZU,          A_HYPER,  K_L,    0,   10,   30,     0, 0x1e,   4,  0x80,       3, SC300,        4,   0,  90,  86,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 39: pl34_at_hforce, Hyper Sentinel Force (sp_hforce): him and the drones (shl34_01 type 2) */
	{   4, BU_O | NO_VEC | NO_ZU,          A_HYPER,  K_L,    0,   10,   30,     0, 0x1c,   3,  0x80,       3, SC300,        3,   0,  90, 105,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   8,        3, { 0, 0 } },	/* 3a: pl34_at_hdrive, Hard Drive's rush hits (sp_hdrive) */
	{  13,             2 | NO_ZU,          A_HYPER,  K_L,    0,   10,   30,     0, 0x19,  13,  0x80,       3, SC300,        4,   0,  90, 105,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 3b: pl34_at_hdrive_end, Hard Drive's last blow at the end of the rush */
};
