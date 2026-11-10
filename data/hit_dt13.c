/* Ruby Heart (character 13): the attacks (HIT_DATA, 0x1c bytes).
   What each hit does. The character's module sets the attack's number (atck_no) with the move,
   and the row is read when the attack box meets a body. After a row is its number, in
   hexadecimal, and the program's name for it:
   ATCK_ for the normals every character has (player.h), pl13_at_ for its own (pl13_def.h).
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
     box, box2  boxes of hit_13 that the computer measures the attack's reach by
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

const HIT_DATA hit_dt_13[] = {
	/* dmg                   type              kind     elem  piyo  stop  sand  sleep   vec  pow  range     mark  score        se  shc  jug  box  box2                     flag  13  guard  15  catch  abyss  cnt    answer        1a */
	{   4,                    ZU,            A_NOM,     K_S,    4,   10,   10,     0, 0x19,   4,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  32,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 00: ATCK_STAND_LP */
	{   7,                    ZU,            A_NOM,     K_M,    2,   11,   16,     0, 0x1f,   6,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40,  33,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 01: ATCK_STAND_MP */
	{  13,                    ZU,            A_NOM,     K_L,    1,   12,   24,     0, 0x1a,   8,  0x80,      13, SC300,       15,   0,  50,  34,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 02: ATCK_STAND_HP */
	{   4,                    ZU,            A_NOM,    K_LS,    4,   10,   10,     0, 0x1c,   4,  0x60, MARK_N1, SC100, SD_KIK_S,   0,  20,  35,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 03: ATCK_STAND_LK */
	{   4,            ZU | NO_ZU,            A_NOM,     K_M,    2,   10,   16,     0, 0x1a,   4,  0x60, MARK_N2, SC200, SD_KIK_M,   0,  20,  36,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 04: ATCK_STAND_MK */
	{  16,                    ZU,            A_NOM,     K_L,    1,   12,   24,     0, 0x00,  11,  0x80, MARK_N3, SC300, SD_KIK_L,   0,  50,  37,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 05: ATCK_STAND_HK */
	{   4,                    ZU,            A_NOM,    K_LS,    4,   10,   10,     0, 0x19,   4,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  38,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 06: ATCK_CROUCH_LP */
	{   7,                  BU_O,            A_NOM,    K_LM,    2,   11,   16,     0, 0x1e,   4,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40,  39,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 07: ATCK_CROUCH_MP */
	{  13,                  BU_O,            A_NOM,    K_LL,    1,   10,   24,     0, 0x00,  11,  0x80,      11, SC300,       15,   0,  50,  40,    0, NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 08: ATCK_CROUCH_HP */
	{   4,                    ZU,             A_LO,    K_LS,    4,   10,   10,     0, 0x19,   4,  0x70, MARK_N1, SC100, SD_KIK_S,   0,  20,  41,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 09: ATCK_CROUCH_LK */
	{   7,                    ZU,             A_LO,    K_LM,    2,   11,   16,     0, 0x1c,   6,  0x40, MARK_N2, SC200, SD_KIK_M,   0,  40,  42,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0a: ATCK_CROUCH_MK */
	{  13,                    AS,             A_LO,    K_LL,    1,   12,   24,     9, 0x1c,   8,  0x70, MARK_N3, SC300, SD_KIK_L,   0,  90,  43,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0b: ATCK_CROUCH_HK */
	{   4,           ZU | NO_VEC,             A_HI,     K_S,    1,   10,   10,     0, 0x00,   6,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  44,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0c: ATCK_JUMP_LP */
	{   7,           ZU | NO_VEC,             A_HI,    K_LM,    1,   10,   16,     0, 0x00,   5,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40,  45,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0d: ATCK_JUMP_MP */
	{  13,           ZU | NO_VEC,             A_HI,     K_L,    1,   12,   32,     0, 0x15,   8,  0x80,      15, SC300,       15,   0,  60,  46,    0,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0e: ATCK_JUMP_HP */
	{   4,           ZU | NO_VEC,             A_HI,     K_S,    1,   10,   10,     0, 0x00,   6,  0x60, MARK_N1, SC200, SD_KIK_S,   0,  20,  47,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0f: ATCK_JUMP_LK */
	{   7,           ZU | NO_VEC,             A_HI,     K_M,    1,   10,   16,     0, 0x00,   5,  0x60, MARK_N2, SC200, SD_KIK_M,   0,  40,  48,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 10: ATCK_JUMP_MK */
	{  13,           ZU | NO_VEC,             A_HI,     K_L,    1,   12,   32,     0, 0x15,   8,  0x60, MARK_N3, SC200, SD_KIK_L,   0,  60,  49,    0,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 11: ATCK_JUMP_HK */
	{ 0 },	/* 12 */
	{  10,           10 | NO_VEC,        A_SPECIAL,     K_L,    0,   10,   10,     0, 0x1f,  10,  0x60,       3, SC200,       12,   0,  90,  91,    0, NO_CHIP | NO_WEIGHT | AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 13: pl13_at_t_kick, the jump-in kick when she is tagged in (t_kick_init) */
	{  10,           12 | NO_VEC,        A_SPECIAL,     K_L,    0,    4,   24,     0, 0x1c,  12,  0x80,      13, SC300,       16,  64,  90,  89,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 14: pl13_at_snapback, Snapback (snapback_init) */
	{  13,         BU_O | NO_VEC,        A_SPECIAL,     K_L,    0,   10,   16,     0, 0x00,  11,  0x60, MARK_N2, SC200, SD_NOR_M,   0,  40,  90,    0, NO_CHIP | NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 15: pl13_at_upper, the launcher (pl13_upper) */
	{ 0 },	/* 16 */
	{ 0 },	/* 17 */
	{ 0 },	/* 18 */
	{   4,                    ZU,            A_NOM,     K_M,    2,   10,   16,     0, 0x1a,   4,  0x60, MARK_N2, SC200, SD_KIK_M,   0,  20, 101,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 19: pl13_at_stand_mk_2, Standing MK's second hit (stand_mk_move) */
	{ 0 },	/* 1a */
	{ 0 },	/* 1b */
	{ 0 },	/* 1c */
	{ 0 },	/* 1d */
	{ 0 },	/* 1e */
	{ 0 },	/* 1f */
	{  16,                     2,     A_NOM | 0x40,    K_LM,    0,   10,    0,     0, 0x1a,  11,  0x60, MARK_N2, SC200, SD_NOR_M,   0,  60,   0,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 20: pl13_at_throw_p, Ground throw (nml_p_catch) */
	{  16,                  BU_O,     A_NOM | 0x40,    K_LM,    0,   10,   20,     0, 0x13,  10,  0x60, MARK_N2, SC200, SD_NOR_M,   0,  60,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 21: pl13_at_air_throw, Air throw (jump_p_catch_throw) */
	{  20,                     2, A_SPECIAL | 0x40,     K_L,    0,    0,   28,     0, 0x1c,   8,  0x60, MARK_N2, SC200,        8,   0,  60,  48,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 22 */
	{   5,                    10,          A_HYPER,    K_LL,    0,   12,   20,     0, 0x00,   8,  0x60, MARK_N2, SC200,        8,   0,  60,   0,    0,                NO_SCALE,  0,     0, 10,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 23: pl13_at_magie_catch, Tour De Magie: the opponent caught by kind 14 (ef_magie_catch_hold) */
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
	{   5,            10 | NO_ZU,          A_HYPER,    K_LL,    0,   10,   30,     0, 0x00,  12,  0x80,       3, SC300,        8,   0,  90,  85,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 30: pl13_at_magie, Tour De Magie (hyper): kind 14, which drops and catches (ef_magie_init) */
	{  10,                  BU_O,        A_SPECIAL,     K_L,    0,   12,   20,     0, 0x1c,   8,  0x60, MARK_N2, SC200,        8,   0,  50,  81,    0,                       0,  1,     0,  0,     0,     0,   0,        5, { 0, 0 } },	/* 31: pl13_at_throw, the carrying-throw assist and its kind 4 (throw_init, ef_throw_init); rafale_throw_init sets it too */
	{   0,                    ZU,        A_SPECIAL,     K_L,    0,    8,    0,     0, 0x00,   8,  0x60, MARK_N2, SC200,        8,   0,   0,  82,    0,                       0,  1,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 32: pl13_at_power, the power-up assist's kind 5, which powers up a player it hits (ef_power_init) */
	{   3,         BU_O | NO_VEC,        A_SPECIAL,     K_L,    0,    4,   12,     0, 0x1e,   4,  0x60, MARK_N2, SC200,        8,   0,  60,  92,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 33: pl13_at_schwarz, Schwarzaile flight: start and each turn (schwarz_init, schwarz_turn) */
	{   3,         BU_O | NO_VEC,        A_SPECIAL,     K_L,    0,    4,   12,     0, 0x1e,   4,  0x60, MARK_N2, SC200,        8,   0,  60,  93,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 34: pl13_at_schwarz_2, Schwarzaile flight's later hits (schwarz_fly, hit_cnt 0) */
	{   4,         BU_O | NO_VEC,        A_SPECIAL,     K_L,    0,    4,   12,     0, 0x1e,   8,  0x60, MARK_N2, SC200,        8,   0,  60,  94,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 35: pl13_at_schwarz_3, Schwarzaile flight's later hits (schwarz_fly, hit_cnt 1) */
	{ 0 },	/* 36 */
	{ 0 },	/* 37 */
	{   5,            2 | NO_VEC,          A_HYPER,     K_L,    0,    4,   30,     0, 0x1e,   4,  0x60, MARK_N2, SC200,        8,   0,  90,  97,    0,     NO_CHIP | NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 38: pl13_at_flammeile, Flammeile (hyper) air flight: start and each turn (flammeile_init, flammeile_turn) */
	{   5,            2 | NO_VEC,          A_HYPER,     K_L,    0,    4,   30,     0, 0x1f,   4,  0x60, MARK_N2, SC200,        8,   0,  90,  98,    0,     NO_CHIP | NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 39: pl13_at_flammeile_2, Flammeile's later hits (flammeile_fly, air_hit_cnt 0) */
	{   5,            2 | NO_VEC,          A_HYPER,     K_L,    0,    4,   30,     0, 0x1f,   9,  0x60, MARK_N2, SC200,        8,   0,  90,  99,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 3a: pl13_at_flammeile_3, Flammeile's later hits (flammeile_fly, air_hit_cnt 1) */
	{ 0 },	/* 3b */
	{ 0 },	/* 3c */
	{  10,          BU_O | NO_ZU,        A_SPECIAL,     K_L,    0,   12,   20,     0, 0x1c,  11,  0x60, MARK_N2, SC200,        8,   0,  90,  80,    0,                       0,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 3d: pl13_at_rafale, Rafale Cannon's hand (kind 6), and its capture (ef_rafale_init, ef_rafale_catch_hold) */
	{  10,   ZU | NO_VEC | NO_ZU,        A_SPECIAL,     K_L,    0,    4,    0,     0, 0x00,   1,  0x60, MARK_N1, SC200,        8,   0,  50,  87,    0,                       0,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 3e: pl13_at_fantome, Fantome: Ruby and the ghost (fantome_init, ef_fantome_shot_init); mille_init sets it too */
	{   4, BU_O | NO_VEC | NO_ZU,        A_SPECIAL, K_L | 6,    0,   12,   24,     0, 0x1f,   9,  0x60, MARK_N2, SC200,        8,   0,  50,  86,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   3,        4, { 0, 0 } },	/* 3f: pl13_at_sublimation, Sublimation's strike (sublimation_init, ef_sublimation_init); power_init sets it too */
	{   5, BU_O | NO_VEC | NO_ZU,          A_HYPER, K_L | 2,    0,    4,   20,     0, 0x1e,   6,  0x60, MARK_N2, SC200,        8,   0,  90,  88,    0,                       0,  1,     0,  0,     0,     0,   6,        3, { 0, 0 } },	/* 40: pl13_at_mille, Mille Fantomes (hyper): the three-hit shots from kind 22 (kind 23, ef_mille_shot_init) */
	{   4,          BU_O | NO_ZU,          A_HYPER,     K_L,    0,    8,   50,     0, 0x19,   6,  0x80,      13, SC300,       16,   0,  90,  83,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 41: pl13_at_paretonnre, Pare-tonn're (hyper): start and its charge's hits (paretonnre_init, ef_paretonnre_atck_init); magie_init too */
	{  10,          BU_O | NO_ZU,          A_HYPER,     K_L,    0,   10,   50,     0, 0x1e,  11,  0x80,      13, SC300,       16,   0,  90,  83,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 42: pl13_at_paretonnre_last, Pare-tonn're's last hit, with a quake (ef_paretonnre_atck_hit) */
	{   2, BU_O | NO_VEC | NO_ZU,          A_HYPER,     K_L,    0,    2,   30,     0, 0x1d,   2,  0x60,       3, SC200,        8,   0,  50,  84,  161,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 43: pl13_at_paretonnre_blast_2, Pare-tonn're: the blast on the opponent, second part (ef_paretonnre_blast_init, sub_no set) */
	{   2, BU_O | NO_VEC | NO_ZU,          A_HYPER,     K_L,    0,    2,   30,     0, 0x1e,   1,  0x60,       3, SC200,        8,   0,  50,  84,  161,     NO_CHIP | NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 44: pl13_at_paretonnre_blast, Pare-tonn're: the blast on the opponent (ef_paretonnre_blast_init, sub_no 0) */
	{   5, BU_O | NO_VEC | NO_ZU,          A_HYPER, K_L | 2,    0,    4,   20,     0, 0x1e,   6,  0x60, MARK_N2, SC200,        8,   0,  90,  88,    0,                 NO_CHIP,  1,     0,  0,     0,     0,   6,        3, { 0, 0 } },	/* 45: pl13_at_mille_2, Mille Fantomes shot's middle hit (ef_mille_shot_fly, routine_3 6) */
	{ 0 },	/* 46 */
	{   5,         BU_O | NO_VEC,          A_HYPER,     K_L,    0,    8,   10,     0, 0x1a,   1,  0x60, MARK_N2, SC200,        8,   0,  90,  97,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 47: pl13_at_flammeile_team, Flammeile as her part of a team hyper: its nine hits (sp_flammeile_team) */
	{   5,         BU_O | NO_VEC,        A_SPECIAL,     K_M,    0,    6,   18,     0, 0x00,  11,  0x60, MARK_N2, SC200,        8,   0,  60,  94,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 48: pl13_at_schwarz_vc, Schwarzaile as a variable counter (schwarz_init, team_type 3) */
	{   4,          BU_O | NO_ZU,        A_SPECIAL, K_L | 6,    0,   12,   24,     0, 0x1f,   9,  0x60, MARK_N2, SC200,        8,   0,  60,  86,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   3,        4, { 0, 0 } },	/* 49 */
};
