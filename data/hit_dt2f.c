/* Silver Samurai (character 2f): the attacks (HIT_DATA, 0x1c bytes).
   What each hit does. The character's module sets the attack's number (atck_no) with the move,
   and the row is read when the attack box meets a body. After a row is its number, in
   hexadecimal, and the program's name for it:
   ATCK_ for the normals every character has (player.h), pl2f_at_ for its own (pl2f_def.h).
   The values' names are Capcom's own for these tables (hit_equ.h) and the program's (player.h).

     dmg     the damage
     type    the reaction: ZU pushed back, BU_O knocked off the feet, AS swept,
             or the number of another in pl_dmg.c's dm_tbl (here 2 dm_buttobi_ng, 9 dm_freeze,
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
     box, box2  boxes of hit_2f that the computer measures the attack's reach by
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

const HIT_DATA hit_dt_2f[] = {
	/* dmg                   type            kind      elem  piyo  stop  sand  sleep   vec  pow  range     mark  score        se  shc  jug  box  box2                      flag  13  guard  15  catch  abyss  cnt    answer        1a */
	{   4,                    ZU,          A_NOM,      K_S,    4,   10,   10,     0, 0x14,   0,  0x70,      14, SC100,       13,   0,  20,  30,   31,                        0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 00: ATCK_STAND_LP */
	{   5,            ZU | NO_ZU,          A_NOM,     K_LM,    2,   11,   16,     0, 0x12,   4,  0x80,      15, SC200,       14,   0,  40, 221,  222,                        0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 01: ATCK_STAND_MP */
	{  10,            ZU | NO_ZU,          A_NOM,      K_L,    1,   12,   24,     0, 0x00,   9,  0x80,      11, SC300,       15,   0,  50,  44,  162,                NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 02: ATCK_STAND_HP */
	{   4,                    ZU,          A_NOM,     K_LS,    4,   10,   10,     0, 0x18,   4,  0x60,      13, SC100,       13,   0,  20,  72,   66,                        0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 03: ATCK_STAND_LK */
	{   7,                    ZU,          A_NOM,      K_M,    2,   10,   16,     0, 0x18,   6,  0x60,      13, SC200,       14,   0,  40,  74,   68,                        0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 04: ATCK_STAND_MK */
	{  13,          BU_O | NO_ZU,          A_NOM,      K_L,    1,   12,   24,     0, 0x1c,   8,  0x80,      14, SC300,       15,   0,  50,  78,    0,                NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 05: ATCK_STAND_HK */
	{   4,                    ZU,           A_LO,     K_LS,    4,   10,   10,     0, 0x16,   1,  0x70,      14, SC100,       13,   0,  20, 164,   71,                        0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 06: ATCK_CROUCH_LP */
	{   7,                    ZU,          A_NOM,     K_LM,    2,   11,   16,     0, 0x1a,   4,  0x80,      14, SC200,       14,   0,  40, 193,   73,                        0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 07: ATCK_CROUCH_MP */
	{  13,                  BU_O,          A_NOM,     K_LL,    1,   10,   24,     0, 0x1f,  11,  0x80,      11, SC300,       15,   0,  50,  61,  172,  NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 08: ATCK_CROUCH_HP */
	{   4,                    ZU,          A_NOM,     K_LS,    4,   10,   10,     0, 0x18,   4,  0x70,      13, SC100,       13,   0,  20,  83,   76,                        0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 09: ATCK_CROUCH_LK */
	{   7,         BU_O | NO_VEC,          A_NOM,     K_LM,    2,   11,   16,     0, 0x00,   6,  0x40,      12, SC200,       14,   0,  40,  58,    0,                NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0a: ATCK_CROUCH_MK */
	{  13,                    AS,           A_LO,     K_LL,    1,   12,   24,     9, 0x18,   7,  0x70,      13, SC300,       15,   0,  90,  89,   77,                        0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0b: ATCK_CROUCH_HK */
	{   4,           ZU | NO_VEC,           A_HI,      K_S,    1,   10,   10,     0, 0x00,   4,  0x70,      14, SC100,       13,   0,  20,  63,    0,                NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0c: ATCK_JUMP_LP */
	{   7,           ZU | NO_VEC,           A_HI,     K_LM,    1,   10,   16,     0, 0x00,   2,  0x80,      13, SC200,       14,   0,  40,  65,   67,                NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0d: ATCK_JUMP_MP */
	{  13,           ZU | NO_VEC,           A_HI,     K_LM,    1,   12,   32,     0, 0x12,  11,  0x80,      15, SC300,       15,   0,  60,  69,  175,            AERIAL_FINISH,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0e: ATCK_JUMP_HP */
	{   4,           ZU | NO_VEC,           A_HI,      K_S,    1,   10,   10,     0, 0x00,   4,  0x60, MARK_N1, SC200, SD_KIK_S,   0,  20,  96,    0,                NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0f: ATCK_JUMP_LK */
	{   7,           ZU | NO_VEC,           A_HI,      K_M,    1,   10,   16,     0, 0x00,   2,  0x60,      12, SC200,       14,   0,  40, 100,    0,                NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 10: ATCK_JUMP_MK */
	{  13,           ZU | NO_VEC,           A_HI,      K_L,    1,   12,   32,     0, 0x11,  11,  0x60,      15, SC200,       15,   0,  60, 102,    0,            AERIAL_FINISH,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 11: ATCK_JUMP_HK */
	{ 0 },	/* 12 */
	{ 0 },	/* 13 */
	{ 0 },	/* 14 */
	{ 0 },	/* 15 */
	{ 0 },	/* 16 */
	{ 0 },	/* 17 */
	{ 0 },	/* 18 */
	{   4,                    ZU,          A_NOM,      K_S,    4,   10,   10,     0, 0x1c,   0,  0x70,      12, SC100,       13,   0,  20,  33,  222,                        0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 19 */
	{   3,           ZU | NO_VEC,          A_NOM,      K_M,    2,   11,   16,     0, 0x1e,   5,  0x60,      12, SC200,       14,   0,  40, 160,   38,                NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 1a: pl2f_at_stand_mp_2, Second hit of the standing medium punch */
	{   4,                     2,          A_NOM,      K_M,    1,   10,   24,     0, 0x13,   7,  0x80,      14, SC300,       15,   0,  90,  53,    0,                        0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 1b: pl2f_at_stand_hp_2, Standing heavy punch follow-up lunge (nml_move_stand_hp) */
	{   4,                    ZU,          A_NOM,     K_LS,    4,   12,   24,     0, 0x16,   1,  0x80,      14, SC300,       15,   0,  20, 164,   71,                        0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 1c: pl2f_at_crouch_lp_2, Crouching light punch repeated (nml_move_crouch_lp) */
	{ 0 },	/* 1d */
	{ 0 },	/* 1e */
	{ 0 },	/* 1f */
	{  16,            2 | NO_VEC,   A_NOM | 0x40,     K_LM,    0,   10,   20,     0, 0x04,   5,  0x60, MARK_N2, SC300, SD_NOR_M,   0,   0,   0,    0,                NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 20: pl2f_at_throw_k, Ground kick throw */
	{  16,                     2,   A_NOM | 0x40,     K_LM,    0,   10,   20,     0, 0x1c,   3,  0x60, MARK_N2, SC300, SD_NOR_M,   0,   0,   0,    0,                NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 21: pl2f_at_throw_p, Ground punch throw: a leap, slammed on landing */
	{  16,                     2,   A_NOM | 0x40,     K_LM,    0,   10,   20,     0, 0x04,   7,  0x60, MARK_N2, SC300, SD_NOR_M,   0,   0,   0,    0,                NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 22: pl2f_at_air_throw, Air kick throw */
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
	{  16,   ZU | NO_VEC | NO_ZU,      A_SPECIAL,  K_S | 3,    0,    6,    0,     0, 0x18,  10,  0x60,    0xff, SC300,       13,   0,  10,  20,    0, NO_WEIGHT | AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 30: pl2f_at_shuriken, Shuriken (sp_shuriken) */
	{  14,   ZU | NO_VEC | NO_ZU,      A_SPECIAL,  K_S | 1,    0,    6,    0,     0, 0x18,  10,  0x60,    0xff, SC300,       13,   0,  10,  20,    0, NO_CHIP | NO_WEIGHT | AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 31 */
	{   8,   ZU | NO_VEC | NO_ZU,      A_SPECIAL,  K_S | 2,    0,    6,    0,     0, 0x18,  10,  0x60,    0xff, SC300,       13,   0,  10,  20,    0, NO_WEIGHT | AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 32: pl2f_at_jshuriken, Shuriken in the air (sp_jshuriken) */
	{   2,   ZU | NO_VEC | NO_ZU,      A_SPECIAL,      K_S,    0,    6,    0,     0, 0x18,  10,  0x60,    0xff, SC300,       13,   0,  10,  20,    0, NO_SCALE | AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 33 */
	{   4, BU_O | NO_VEC | NO_ZU,        A_HYPER,     K_LM,    0,    2,    0,     0, 0x1e,  14,  0x60,    0xff, SC300,       13,   0,  10,  20,    0, NO_WEIGHT | AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 34: pl2f_at_hshuriken, Cho-Shuriken (hyper, sp_hshuriken) */
	{   4, BU_O | NO_VEC | NO_ZU,      A_SPECIAL,      K_L,    0,    6,    0,     0, 0x1e,  13,  0x80,      13, SC300,       15,   0,  90, 234,  235,                NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 35: pl2f_at_rapid, Hyakuretsutou hits (sp_rapid; also the running sp_dash and the team hyper sp_hrapid) */
	{   3, BU_O | NO_VEC | NO_ZU,        A_HYPER,        3,    0,    1,   20,     0, 0x04,   0,  0x80,       3,    20,       18,   0,  90, 131,  140,                NO_WEIGHT,  1,     0,  0,     0,     0,   6,        3, { 0, 0 } },	/* 36: pl2f_at_raimei, Raimeiken (hyper, sp_raimei) */
	{   3, BU_O | NO_VEC | NO_ZU,        A_HYPER,        3,    0,    1,   20,     0, 0x1f,   0,  0x80,       3,    20,       18,   0,  90, 131,  140,      NO_CHIP | NO_WEIGHT,  1,     0,  0,     0,     0,   6,        3, { 0, 0 } },	/* 37 */
	{   3, BU_O | NO_VEC | NO_ZU,        A_HYPER,     0x40,    0,    1,   20,     0, 0x1a,   0,  0x80,       3,    20,       18,   0,  90, 131,  140,      NO_CHIP | NO_WEIGHT,  1,     0,  0,     0,     0,   6,        3, { 0, 0 } },	/* 38 */
	{   1,                  BU_O,          A_NOM,        3,    0,    8,    0,     0, 0x00,   4,  0x80,      11, SC100,       13,   0,  90, 247,    0,                NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 39 */
	{  10,           12 | NO_VEC,      A_SPECIAL,      K_L,    0,    4,   24,     0, 0x1c,  12,  0x60,      13, SC200,       15,  64,  90,  85,    0,                NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 3a: pl2f_at_snapback, Snapback (sp_snapback) */
	{  10,           10 | NO_VEC,      A_SPECIAL,      K_L,    0,   10,   10,     0, 0x1f,  10,  0x60,      14, SC200,       16,   0,  90,  99,    0, NO_CHIP | NO_WEIGHT | AERIAL_FINISH,  1,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 3b: pl2f_at_t_kick, the partner's jump-in attack (pl2f_t_kick) */
	{  15, 2 | NO_VEC | NO_ZU | FLY_SCREEN,        A_HYPER,     0x40,    0,   12,    0,     0, 0x1a,  15,  0x60,      13, SC300,       16,   0,  90, 234,  235,      NO_CHIP | NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 3c: pl2f_at_hrapid_end, Hyper Hyakuretsutou's last dash (hrapid_move) */
	{  14,                  BU_O,      A_SPECIAL,     K_LL,    0,   10,   24,     0, 0x1f,  11,  0x80,      12, SC300,       15,   0,  40, 168,  169, NO_CHIP | NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 3d */
	{   8,   ZU | NO_VEC | NO_ZU,      A_SPECIAL,      K_S,    0,    6,    0,     0, 0x18,  10,  0x60,    0xff, SC300,       13,   0,  10,  20,    0, NO_WEIGHT | AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 3e */
	{ 0 },	/* 3f */
	{  40,    9 | NO_VEC | NO_ZU, A_LO | A_HYPER,     0x41,    0,   12,    0,     0, 0x00,   0,  0x20,    0xff, SC300,        0,   0,  90, 230,    0,                NO_WEIGHT,  1,     0,  0,    32,     0,   0,        3, { 0, 0 } },	/* 40: pl2f_at_hyouga, Hyougaken (hyper, ice; him and shl2f_04) */
	{   2, BU_O | NO_VEC | NO_ZU,        A_HYPER,     0x42,    0,    2,    0,     0, 0x00,  10,  0x20,    0xff, SC300,        0,   0,  90, 214,    0,                NO_WEIGHT,  1,     0,  0,     0,     0,   6,        3, { 0, 0 } },	/* 41: pl2f_at_homura, Homuraken (hyper, fire; him and shl2f_05) */
	{   6, BU_O | NO_VEC | NO_ZU,        A_HYPER, K_LM | 3,    0,    2,    0,     0, 0x1e,  14,  0x60,    0xff, SC300,       13,   0,  90,  20,    0, NO_WEIGHT | AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 42 */
	{   4, BU_O | NO_VEC | NO_ZU,        A_HYPER, K_LM | 1,    0,    2,    0,     0, 0x1e,  14,  0x60,    0xff, SC300,       13,   0,  90,  20,    0, NO_WEIGHT | AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 43 */
	{   3,    2 | NO_VEC | NO_ZU,        A_HYPER, K_LM | 2,    0,    2,    0,     0, 0x1e,  14,  0x60,    0xff, SC300,       13,   0,  90,  20,    0, NO_WEIGHT | AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 44 */
	{   4,                    ZU,      A_SPECIAL,      K_M,    0,    8,   20,     0, 0x1a,   8,  0x80,      12, SC300,       13,   0,  90, 234,  235,                NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 45 */
	{   4,                    ZU,      A_SPECIAL,  K_M | 3,    0,    8,   20,     0, 0x1a,   8,  0x80,      12, SC300,       13,   0,  90, 234,  235,                NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 46 */
	{   4,                    ZU,      A_SPECIAL,  K_L | 1,    0,   12,   20,     0, 0x18,   8,  0x80,      12, SC300,       13,   0,  90, 234,  235,                NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 47 */
	{   4,                    ZU,      A_SPECIAL,  K_S | 2,    0,    4,   20,     0, 0x1c,   8,  0x80,      12, SC300,       13,   0,  90, 234,  235,                NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 48 */
	{   4,                    ZU,      A_SPECIAL,     K_LM,    0,    8,   20,     0, 0x1a,   8,  0x80,      14, SC300,       13,   0,  90, 234,  235,                NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 49 */
	{   4,                    ZU,      A_SPECIAL, K_LM | 3,    0,    8,   20,     0, 0x1a,   8,  0x80,      14, SC300,       13,   0,  90, 234,  235,                NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 4a */
	{   4,                    ZU,      A_SPECIAL, K_LL | 1,    0,   12,   20,     0, 0x18,   8,  0x80,      14, SC300,       13,   0,  90, 234,  235,                NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 4b */
	{   4,                    ZU,      A_SPECIAL, K_LS | 2,    0,    4,   20,     0, 0x1c,   8,  0x80,      14, SC300,       13,   0,  90, 234,  235,                NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 4c */
	{   4,                    ZU,      A_SPECIAL,      K_M,    0,    8,   20,     0, 0x1a,   8,  0x80,      12, SC300,       13,   0,  90, 234,  235,                NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 4d */
	{   4,                    ZU,      A_SPECIAL,  K_M | 3,    0,    8,   20,     0, 0x1a,   8,  0x80,      12, SC300,       13,   0,  90, 234,  235,                NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 4e */
	{   4,                    ZU,      A_SPECIAL,  K_L | 1,    0,   12,   20,     0, 0x18,   8,  0x80,      12, SC300,       13,   0,  90, 234,  235,                NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 4f */
	{   4,                    ZU,      A_SPECIAL,  K_S | 2,    0,    4,   20,     0, 0x1c,   8,  0x80,      12, SC300,       13,   0,  90, 234,  235,                NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 50 */
	{   4,                    ZU,      A_SPECIAL,     K_LM,    0,    8,   20,     0, 0x1a,   8,  0x80,      14, SC300,       13,   0,  90, 234,  235,                NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 51 */
	{   4,                    ZU,      A_SPECIAL, K_LM | 3,    0,    8,   20,     0, 0x1a,   8,  0x80,      14, SC300,       13,   0,  90, 234,  235,                NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 52 */
	{   4,                    ZU,      A_SPECIAL, K_LL | 1,    0,   12,   20,     0, 0x18,   8,  0x80,      14, SC300,       13,   0,  90, 234,  235,                NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 53 */
	{   4,                    ZU,      A_SPECIAL, K_LS | 2,    0,    4,   20,     0, 0x1c,   8,  0x80,      14, SC300,       13,   0,  90, 234,  235,                NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 54 */
	{   4,                    ZU,        A_HYPER,      K_M,    0,    8,   20,     0, 0x1a,   8,  0x80,      12, SC300,       13,   0,  90, 234,  235,                NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 55 */
	{   4,                    ZU,        A_HYPER,  K_M | 3,    0,    8,   20,     0, 0x1a,   8,  0x80,      12, SC300,       13,   0,  90, 234,  235,                NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 56 */
	{   4,                    ZU,        A_HYPER,  K_L | 1,    0,   12,   20,     0, 0x18,   8,  0x80,      12, SC300,       13,   0,  90, 234,  235,                NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 57 */
	{   4,                    ZU,        A_HYPER,  K_S | 2,    0,    4,   20,     0, 0x1c,   8,  0x80,      12, SC300,       13,   0,  90, 234,  235,                NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 58 */
	{   4,                    ZU,        A_HYPER,     K_LM,    0,    8,   20,     0, 0x1a,   8,  0x80,      14, SC300,       13,   0,  90, 234,  235,                NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 59 */
	{   4,                    ZU,        A_HYPER, K_LM | 3,    0,    8,   20,     0, 0x1a,   8,  0x80,      14, SC300,       13,   0,  90, 234,  235,                NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 5a */
	{   4,                    ZU,        A_HYPER, K_LL | 1,    0,   12,   20,     0, 0x18,   8,  0x80,      14, SC300,       13,   0,  90, 234,  235,                NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 5b */
	{   4,                    ZU,        A_HYPER, K_LS | 2,    0,    4,   20,     0, 0x1c,   8,  0x80,      14, SC300,       13,   0,  90, 234,  235,                NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 5c */
	{  14,                  BU_O,      A_SPECIAL,     K_LL,    0,   10,   24,     0, 0x1f,  11,  0x80,      12, SC300,       15,   0,  40, 168,  169,  NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 5d */
};
