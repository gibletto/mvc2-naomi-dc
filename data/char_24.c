/* Cammy (character 24): the animations. PLAYER_WORK's char_adrs points here.

   spr_prm_24      a sprite's sizes and turn (SPR_PRM): a frame's zoom number picks a row, 0 for none.
   char_tbl_24     the groups: pl_char_set( pl_w, group, no ) takes the group's table from here and
                   starts its animation no. The character's module (s_pl24.c) says which group
                   and number each move uses.
   pl24_GROUP_tbl  a group's animations. GROUP is the group's name in player.h (stand for
                   Char_Stand), or grpNN for a group with no name there.
   pl24_GROUP_NN   an animation, named by the first group and number that has it: its frames in
                   order. The comment before it lists its places in the groups and, for each,
                   the routines that start it by that number, with their special move;
                   (2 + atck_level) is a routine that counts on from 2 by the attack's level.
                   A frame with END_OF_DATA is the last, and the word after it is the frame the
                   animation goes on from (CG_NEXT; pl24_GROUP_NN_loop where that is no
                   animation's first frame). An effect's animation has short frames (EF_FRAME)
                   and may end with no word.
   pl24_unused_NN  frames that no table or animation reaches.

   A player's frame (CG_FRAME, the 0x14 bytes of CHAR_DATA), with the names Capcom's tables used:
     prm, type  a number and a code for the character's module (a speed, an attack, a voice)
     ctr        the game frames it is shown for
     end        END_OF_DATA on the last frame
     spr        the sprite, -1 for none
     prio       added to the draw priority (a signed byte)
     shadow     the shadow's offset (the low five bits) and size, 0xff none
     atck       ATK while an attack is on its way or can hit, ATE in its frames after that;
                CG_ATCK_NG lets nothing cancel it
     flag1      a number for the module: most often the attack, a row of hit_dt_24
     se         a voice (1 to 63), or with CSE a sound effect; 0 none
     zoom       the row of spr_prm_24, 0 none
     unk        a byte the program is not seen to read
     flag2      a second code for the character's effects
     rival      where a held opponent is put (the low byte), and a code for the module (the high)
     box        the frame's row of frame_box_24 */
#include "player.h"
#include "char_tbl.h"

extern const Sint32 pl24_stand_00[], pl24_stand_02[], pl24_stand_03[], pl24_jump_00[],
	pl24_jump_00_loop[], pl24_jump_01[], pl24_jump_01_loop[], pl24_jump_02[], pl24_jump_02_loop[],
	pl24_jump_03[], pl24_jump_06[], pl24_jump_09[], pl24_jump_07[], pl24_jump_10[], pl24_unused_00[],
	pl24_jump_08[], pl24_jump_11[], pl24_unused_01[], pl24_dash_00[], pl24_dash_00_loop[],
	pl24_dash_01[], pl24_crouch_00[], pl24_crouch_01[], pl24_crouch_02[], pl24_turn_00[],
	pl24_turn_01[], pl24_guard_00[], pl24_guard_01[], pl24_guard_02[], pl24_guard_03[],
	pl24_guard_04[], pl24_guard_05[], pl24_guard_06[], pl24_guard_07[], pl24_air_guard_00[],
	pl24_air_guard_01[], pl24_air_guard_02[], pl24_stand_p_00[], pl24_stand_p_03[],
	pl24_stand_p_01[], pl24_stand_p_04[], pl24_stand_p_02[], pl24_stand_p_05[], pl24_stand_k_00[],
	pl24_stand_k_03[], pl24_stand_k_01[], pl24_stand_k_04[], pl24_dash_02[], pl24_dash_03[],
	pl24_crouch_p_00[], pl24_crouch_p_01[], pl24_crouch_p_02[], pl24_crouch_k_00[],
	pl24_crouch_k_01[], pl24_crouch_k_02[], pl24_jump_p_00[], pl24_jump_p_01[], pl24_jump_p_02[],
	pl24_jump_k_00[], pl24_jump_k_01[], pl24_jump_k_02[], pl24_get_up_00[], pl24_get_up_01[],
	pl24_get_up_02[], pl24_get_up_03[], pl24_appear_00[], pl24_win_00[], pl24_win_00_loop[],
	pl24_win_01[], pl24_win_01_loop[], pl24_win_02[], pl24_win_02_loop[], pl24_win_03[],
	pl24_win_03_loop[], pl24_win_04[], pl24_win_04_loop[], pl24_win_05[], pl24_win_05_loop[],
	pl24_win_06[], pl24_win_06_loop[], pl24_damage_00[], pl24_damage_03[], pl24_damage_01[],
	pl24_damage_04[], pl24_damage_02[], pl24_damage_05[], pl24_damage_06[], pl24_damage_07[],
	pl24_damage_08[], pl24_damage_10[], pl24_damage_11[], pl24_damage_12[], pl24_damage_13[],
	pl24_damage_13_loop[], pl24_damage_09[], pl24_damage_09_loop[], pl24_damage_16[],
	pl24_damage_17[], pl24_damage_18[], pl24_damage_19[], pl24_damage_20[], pl24_damage_21[],
	pl24_damage_22[], pl24_damage_23[], pl24_damage_24[], pl24_damage_25[], pl24_damage_26[],
	pl24_damage_27[], pl24_damage_30[], pl24_damage_32[], pl24_damage_33[], pl24_damage_28[],
	pl24_damage_29[], pl24_damage_31[], pl24_dizzy_00[], pl24_catch_00[], pl24_catch_01[],
	pl24_catch_02[], pl24_catch_03[], pl24_catch_04[], pl24_catch_04_loop[], pl24_catch_05[],
	pl24_catch_06[], pl24_catch_06_loop[], pl24_effect_00[], pl24_effect_01[], pl24_effect_02[],
	pl24_unused_02[], pl24_effect_03[], pl24_effect_03_loop[], pl24_effect_04[], pl24_special_00[],
	pl24_special_00_loop[], pl24_special_01[], pl24_special_01_loop[], pl24_special_23[],
	pl24_special_23_loop[], pl24_special_03[], pl24_special_03_loop[], pl24_special_04[],
	pl24_special_04_loop[], pl24_special_05[], pl24_special_18[], pl24_special_24[],
	pl24_special_24_loop[], pl24_special_25[], pl24_special_25_loop[], pl24_special_27[],
	pl24_super_00[], pl24_super_01[], pl24_super_02[], pl24_super_03[], pl24_super_03_loop[],
	pl24_super_04[], pl24_super_04_loop[], pl24_super_05[], pl24_super_05_loop[], pl24_super_06[],
	pl24_super_06_loop[], pl24_super_07[], pl24_super_07_loop[], pl24_super_08[],
	pl24_super_08_loop[], pl24_super_09[], pl24_super_09_loop[], pl24_super_10[],
	pl24_super_10_loop[], pl24_super_11[], pl24_super_11_loop[], pl24_super_12[],
	pl24_super_12_loop[], pl24_super_13[], pl24_super_13_loop[], pl24_super_14[], pl24_super_15[],
	pl24_super_15_loop[], pl24_super_16[], pl24_super_16_loop[], pl24_super_17[], pl24_special_19[],
	pl24_special_19_loop[], pl24_special_20[], pl24_special_06[], pl24_special_06_loop[],
	pl24_special_21[], pl24_special_21_loop[], pl24_special_07[], pl24_special_22[],
	pl24_special_22_loop[], pl24_special_08[], pl24_special_09[], pl24_special_11[],
	pl24_special_11_loop[], pl24_special_12[], pl24_special_12_loop[], pl24_special_13[],
	pl24_special_14[], pl24_special_32[], pl24_special_15[], pl24_special_15_loop[],
	pl24_special_16[], pl24_special_16_loop[], pl24_extra_00[], pl24_extra_01[], pl24_special_28[],
	pl24_special_29[], pl24_special_29_loop[], pl24_special_30[], pl24_special_31[], pl24_extra_02[],
	pl24_extra_03[], pl24_grp27_00[], pl24_grp27_00_loop[], pl24_grp27_01[], pl24_grp27_01_loop[],
	pl24_caught_00[], pl24_caught_01[], pl24_caught_02[], pl24_caught_03[], pl24_caught_04[],
	pl24_caught_05[], pl24_caught_06[], pl24_caught_07[], pl24_caught_08[], pl24_caught_09[],
	pl24_caught_10[], pl24_caught_11[], pl24_caught_12[], pl24_caught_13[], pl24_caught_14[],
	pl24_caught_15[], pl24_caught_16[], pl24_caught_17[], pl24_caught_18[], pl24_caught_19[],
	pl24_caught_20[], pl24_caught_21[];
extern const Sint32 *const pl24_stand_tbl[], *const pl24_jump_tbl[], *const pl24_dash_tbl[],
	*const pl24_crouch_tbl[], *const pl24_turn_tbl[], *const pl24_guard_tbl[],
	*const pl24_air_guard_tbl[], *const pl24_stand_p_tbl[], *const pl24_stand_k_tbl[],
	*const pl24_crouch_p_tbl[], *const pl24_crouch_k_tbl[], *const pl24_jump_p_tbl[],
	*const pl24_jump_k_tbl[], *const pl24_damage_tbl[], *const pl24_dizzy_tbl[],
	*const pl24_catch_tbl[], *const pl24_caught_tbl[], *const pl24_get_up_tbl[],
	*const pl24_appear_tbl[], *const pl24_win_tbl[], *const pl24_extra_tbl[],
	*const pl24_special_tbl[], *const pl24_super_tbl[], *const pl24_effect_tbl[],
	*const pl24_grp25_tbl[], *const pl24_grp27_tbl[];

/* the sprites' sizes and turns, by a frame's zoom number */
const SPR_PRM spr_prm_24[16] = {
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 1000, 1000, -16384, 18, -44, 0, 0, 0 },
	{ 1000, 1000, 0, 0, 0, 0, 0, 0 },
	{ 1000, 1000, 0, 0, 0, 0, 0, 0 },
	{ 1000, 1000, 0, 0, 0, 0, 0, 0 },
	{ 1000, 1000, 0, 0, 0, 0, 0, 0 },
	{ 1000, 1000, 0, 0, 0, 0, 0, 0 },
	{ 1000, 1000, 0, 0, 0, 0, 0, 0 },
	{ 1000, 1000, 0, 0, 0, 0, 0, 0 },
	{ 1000, 1000, 0, 0, 0, 0, 0, 0 },
	{ 1000, 1000, 0, 0, 0, 0, 0, 0 },
	{ 1000, 1000, 0, 0, 0, 0, 0, 0 },
	{ 1000, 1000, 0, 0, 0, 0, 0, 0 },
	{ 1000, 1000, 0, 0, 0, 0, 0, 0 },
	{ 1000, 1000, 0, 0, 0, 0, 0, 0 },
	{ 1000, 1000, 0, 0, 0, 0, 0, 0 },
};

/* the groups */
const Sint32 *const *const char_tbl_24[] = {
	pl24_stand_tbl, pl24_jump_tbl, pl24_dash_tbl, pl24_crouch_tbl, pl24_turn_tbl, pl24_guard_tbl,
	pl24_air_guard_tbl, pl24_stand_p_tbl, pl24_stand_k_tbl, pl24_crouch_p_tbl, pl24_crouch_k_tbl, pl24_jump_p_tbl,
	pl24_jump_k_tbl, pl24_damage_tbl, pl24_dizzy_tbl, pl24_catch_tbl, pl24_caught_tbl, pl24_get_up_tbl,
	pl24_appear_tbl, pl24_win_tbl, pl24_extra_tbl, pl24_special_tbl, pl24_super_tbl, pl24_effect_tbl,
	pl24_effect_tbl, pl24_grp25_tbl, pl24_stand_tbl, pl24_grp27_tbl, pl24_stand_tbl, pl24_stand_tbl,
	pl24_stand_tbl, pl24_grp27_tbl, 0,
};

const Sint32 *const pl24_stand_tbl[] = {
	pl24_stand_00, pl24_stand_00, pl24_stand_02, pl24_stand_03, pl24_stand_00, pl24_stand_00,
	pl24_stand_02, pl24_stand_03, 0,
};

const Sint32 *const pl24_jump_tbl[] = {
	pl24_jump_00, pl24_jump_01, pl24_jump_02, pl24_jump_03, pl24_jump_03, pl24_jump_03,
	pl24_jump_06, pl24_jump_07, pl24_jump_08, pl24_jump_09, pl24_jump_10, pl24_jump_11,
	0,
};

const Sint32 *const pl24_dash_tbl[] = {
	pl24_dash_00, pl24_dash_01, pl24_dash_02, pl24_dash_03, 0,
};

const Sint32 *const pl24_crouch_tbl[] = {
	pl24_crouch_00, pl24_crouch_01, pl24_crouch_02, 0,
};

const Sint32 *const pl24_turn_tbl[] = {
	pl24_turn_00, pl24_turn_01, 0,
};

const Sint32 *const pl24_guard_tbl[] = {
	pl24_guard_00, pl24_guard_01, pl24_guard_02, pl24_guard_03, pl24_guard_04, pl24_guard_05,
	pl24_guard_06, pl24_guard_07, 0,
};

const Sint32 *const pl24_air_guard_tbl[] = {
	pl24_air_guard_00, pl24_air_guard_01, pl24_air_guard_02, 0,
};

const Sint32 *const pl24_stand_p_tbl[] = {
	pl24_stand_p_00, pl24_stand_p_01, pl24_stand_p_02, pl24_stand_p_03, pl24_stand_p_04, pl24_stand_p_05,
	0,
};

const Sint32 *const pl24_stand_k_tbl[] = {
	pl24_stand_k_00, pl24_stand_k_01, pl24_dash_02, pl24_stand_k_03, pl24_stand_k_04, pl24_dash_03,
	0,
};

const Sint32 *const pl24_crouch_p_tbl[] = {
	pl24_crouch_p_00, pl24_crouch_p_01, pl24_crouch_p_02, 0,
};

const Sint32 *const pl24_crouch_k_tbl[] = {
	pl24_crouch_k_00, pl24_crouch_k_01, pl24_crouch_k_02, 0,
};

const Sint32 *const pl24_jump_p_tbl[] = {
	pl24_jump_p_00, pl24_jump_p_01, pl24_jump_p_02, pl24_jump_p_00, pl24_jump_p_01, pl24_jump_p_02,
	0,
};

const Sint32 *const pl24_jump_k_tbl[] = {
	pl24_jump_k_00, pl24_jump_k_01, pl24_jump_k_02, pl24_jump_k_00, pl24_jump_k_01, pl24_jump_k_02,
	0,
};

const Sint32 *const pl24_damage_tbl[] = {
	pl24_damage_00, pl24_damage_01, pl24_damage_02, pl24_damage_03, pl24_damage_04, pl24_damage_05,
	pl24_damage_06, pl24_damage_07, pl24_damage_08, pl24_damage_09, pl24_damage_10, pl24_damage_11,
	pl24_damage_12, pl24_damage_13, pl24_damage_13, pl24_damage_13, pl24_damage_16, pl24_damage_17,
	pl24_damage_18, pl24_damage_19, pl24_damage_20, pl24_damage_21, pl24_damage_22, pl24_damage_23,
	pl24_damage_24, pl24_damage_25, pl24_damage_26, pl24_damage_27, pl24_damage_28, pl24_damage_29,
	pl24_damage_30, pl24_damage_31, pl24_damage_32, pl24_damage_33, pl24_damage_13, 0,
};

const Sint32 *const pl24_dizzy_tbl[] = {
	pl24_dizzy_00, 0,
};

const Sint32 *const pl24_catch_tbl[] = {
	pl24_catch_00, pl24_catch_01, pl24_catch_02, pl24_catch_03, pl24_catch_04, pl24_catch_05,
	pl24_catch_06, 0,
};

const Sint32 *const pl24_caught_tbl[] = {
	pl24_caught_00, pl24_caught_01, pl24_caught_02, pl24_caught_03, pl24_caught_04, pl24_caught_05,
	pl24_caught_06, pl24_caught_07, pl24_caught_08, pl24_caught_09, pl24_caught_10, pl24_caught_11,
	pl24_caught_12, pl24_caught_13, pl24_caught_14, pl24_caught_15, pl24_caught_16, pl24_caught_17,
	pl24_caught_18, pl24_caught_19, pl24_caught_20, pl24_caught_21, 0,
};

const Sint32 *const pl24_get_up_tbl[] = {
	pl24_get_up_00, pl24_get_up_01, pl24_get_up_02, pl24_get_up_03, 0,
};

const Sint32 *const pl24_appear_tbl[] = {
	pl24_appear_00, 0,
};

const Sint32 *const pl24_win_tbl[] = {
	pl24_win_00, pl24_win_01, pl24_win_02, pl24_win_03, pl24_win_04, pl24_win_05,
	pl24_win_06, 0,
};

const Sint32 *const pl24_extra_tbl[] = {
	pl24_extra_00, pl24_extra_01, pl24_extra_02, pl24_extra_03, 0,
};

const Sint32 *const pl24_special_tbl[] = {
	pl24_special_00, pl24_special_01, pl24_special_01, pl24_special_03, pl24_special_04, pl24_special_05,
	pl24_special_06, pl24_special_07, pl24_special_08, pl24_special_09, pl24_special_09, pl24_special_11,
	pl24_special_12, pl24_special_13, pl24_special_14, pl24_special_15, pl24_special_16, pl24_special_04,
	pl24_special_18, pl24_special_19, pl24_special_20, pl24_special_21, pl24_special_22, pl24_special_23,
	pl24_special_24, pl24_special_25, pl24_special_25, pl24_special_27, pl24_special_28, pl24_special_29,
	pl24_special_30, pl24_special_31, pl24_special_32, 0,
};

const Sint32 *const pl24_super_tbl[] = {
	pl24_super_00, pl24_super_01, pl24_super_02, pl24_super_03, pl24_super_04, pl24_super_05,
	pl24_super_06, pl24_super_07, pl24_super_08, pl24_super_09, pl24_super_10, pl24_super_11,
	pl24_super_12, pl24_super_13, pl24_super_14, pl24_super_15, pl24_super_16, pl24_super_17,
	0,
};

const Sint32 *const pl24_effect_tbl[] = {
	pl24_effect_00, pl24_effect_01, pl24_effect_02, pl24_effect_03, pl24_effect_04, 0,
};

const Sint32 *const pl24_grp25_tbl[] = {
	pl24_stand_00, 0,
};

const Sint32 *const pl24_grp27_tbl[] = {
	pl24_grp27_00, pl24_grp27_01, 0,
};

	/*         prm  type  ctr          end    spr  prio  shadow  atck  flag1          se  zoom  unk  flag2   rival  box */

/* Char_Stand 0: win_e_set, act_stand, appear_move, act_change_gattai_move, ...
   Char_Stand 1
   Char_Stand 4
   Char_Stand 5
   group 25 0
   Char_Fly 0 */
const Sint32 pl24_stand_00[] = {
	CG_FRAME(    0,    0,   8,           0,     9, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,    10, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,    11, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,    12, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,    13, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,    14, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,    15, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,    16, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl24_stand_00 ),
};

/* Char_Stand 2: act_walk (2 + no)
   Char_Stand 6
   Char_Fly 2
   Char_Fly 6
   group 28 2
   group 28 6 */
const Sint32 pl24_stand_02[] = {
	CG_FRAME(    0,    0,   4,           0,    17, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    18, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    19, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    20, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    21, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    22, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    23, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    24, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    25, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    26, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    27, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    28, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl24_stand_02 ),
};

/* Char_Stand 3
   Char_Stand 7
   Char_Fly 3
   Char_Fly 7
   group 28 3
   group 28 7 */
const Sint32 pl24_stand_03[] = {
	CG_FRAME(    0,    0,   4,           0,    29, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    30, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    31, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    32, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    33, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    34, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    35, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    36, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    37, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    38, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    39, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    40, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl24_stand_03 ),
};

/* Char_Jump 0 */
const Sint32 pl24_jump_00[] = {
	CG_FRAME(    0,    1,   4,           0,    73, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 106 ),
	CG_FRAME(    0,    0,   3,           0,    54, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 106 ),
	CG_FRAME(    0,    0,   3,           0,    55, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 106 ),
	CG_FRAME(    0,    0,   4,           0,    56, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 106 ),
	CG_FRAME(    0,    0,   4,           0,    57, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 106 ),
	CG_FRAME(    0,    0,  14,           0,    58, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 106 ),
	CG_FRAME(    0,    0,   4,           0,    59, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 106 ),
	CG_FRAME(    0,    0,   4,           0,    60, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 106 ),
};

const Sint32 pl24_jump_00_loop[] = {
	CG_FRAME(    0,    0,   2,           0,    61, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 106 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,    62, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 106 ),
	CG_NEXT( pl24_jump_00_loop ),
};

/* Char_Jump 1: act_wall_jump, act_change_gattai_init, act_change_zero_init, act_change_assist_init, ... */
const Sint32 pl24_jump_01[] = {
	CG_FRAME(    0,    1,   4,           0,    73, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 106 ),
	CG_FRAME(    0,    0,   4,           0,    69, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 106 ),
	CG_FRAME(    0,    0,   4,           0,    68, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 106 ),
	CG_FRAME(    0,    0,   4,           0,    67, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 106 ),
	CG_FRAME(    0,    0,   4,           0,    66, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 106 ),
	CG_FRAME(    0,    0,   4,           0,    65, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 106 ),
};

const Sint32 pl24_jump_01_loop[] = {
	CG_FRAME(    0,    0,   4,           0,    63, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 106 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    71, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 106 ),
	CG_NEXT( pl24_jump_01_loop ),
};

/* Char_Jump 2 */
const Sint32 pl24_jump_02[] = {
	CG_FRAME(    0,    1,   4,           0,    73, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 106 ),
	CG_FRAME(    0,    0,   4,           0,    64, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 106 ),
	CG_FRAME(    0,    0,   4,           0,    65, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 106 ),
	CG_FRAME(    0,    0,   4,           0,    66, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 106 ),
	CG_FRAME(    0,    0,   4,           0,    67, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 106 ),
	CG_FRAME(    0,    0,   4,           0,    68, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 106 ),
};

const Sint32 pl24_jump_02_loop[] = {
	CG_FRAME(    0,    0,   4,           0,    70, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 106 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    72, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 106 ),
	CG_NEXT( pl24_jump_02_loop ),
};

/* Char_Jump 3: drill_fall (Cannon Drill)
   Char_Jump 4
   Char_Jump 5: hooligan_miss (Hooligan Combination), killerbee_hop (Killer Bee Assault (hyper)), hooligan_l_catch_hop */
const Sint32 pl24_jump_03[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,    73, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl24_jump_03 ),
};

/* Char_Jump 6 */
const Sint32 pl24_jump_06[] = {
	CG_FRAME(    0,    1,   4,           0,    73, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,    54, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,    55, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,    54, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,    55, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,    54, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,    55, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    56, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    57, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    58, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    59, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    60, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Jump 9: act_fall, act_air_guard_hold, act_wall_jump */
const Sint32 pl24_jump_09[] = {
	CG_FRAME(    0,    0,   2,           0,    61, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,    62, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl24_jump_09 ),
};

/* Char_Jump 7 */
const Sint32 pl24_jump_07[] = {
	CG_FRAME(    0,    1,   4,           0,    73, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    69, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    68, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    67, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    66, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    65, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Jump 10: change_in */
const Sint32 pl24_jump_10[] = {
	CG_FRAME(    0,    0,   2,           0,    61, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,    62, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl24_jump_10 ),
};

const Sint32 pl24_unused_00[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,   4,           0,    63, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    71, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl24_jump_10 ),
};

/* Char_Jump 8 */
const Sint32 pl24_jump_08[] = {
	CG_FRAME(    0,    1,   4,           0,    73, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    64, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    65, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    66, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    67, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    68, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Jump 11: act_change_out_recover */
const Sint32 pl24_jump_11[] = {
	CG_FRAME(    0,    0,   2,           0,    61, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,    62, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl24_jump_11 ),
};

const Sint32 pl24_unused_01[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,   4,           0,    70, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    72, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl24_jump_11 ),
};

/* Char_Dash 0: act_dash */
const Sint32 pl24_dash_00[] = {
	CG_FRAME(    0,    1,   2,           0,    73, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  85 ),
};

const Sint32 pl24_dash_00_loop[] = {
	CG_FRAME(    0,    0,   3,           0,    41, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  85 ),
	CG_FRAME(    0,    0,   3,           0,    42, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  85 ),
	CG_FRAME(    0,    0,   3,           0,    43, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  85 ),
	CG_FRAME(    0,    0,   3,           0,    44, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  85 ),
	CG_FRAME(    0,    0,   3,           0,    45, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  85 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,    46, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  85 ),
	CG_NEXT( pl24_dash_00_loop ),
};

/* Char_Dash 1: act_back_dash */
const Sint32 pl24_dash_01[] = {
	CG_FRAME(    0,    0,   6,           0,    47, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  86 ),
	CG_FRAME(    0,    0,   6,           0,    48, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  86 ),
	CG_FRAME(    0,    1,   3,           0,    49, 0x80,   0x00,    0,  0x00, 0x34 | CSE,    0,   0,     0,      0,  86 ),
	CG_FRAME(    0,    1,   3,           0,    50, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  86 ),
	CG_FRAME(    0,    1,   3,           0,    51, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  86 ),
	CG_FRAME(    0,    0,   8,           0,    52, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  86 ),
	CG_FRAME(    0,    0,   4,           0,    53, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  86 ),
	CG_FRAME(    0,    1,   4, END_OF_DATA,    73, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  86 ),
	CG_NEXT( pl24_dash_01 ),
};

/* Char_Crouch 0: act_crouch, act_stand_up */
const Sint32 pl24_crouch_00[] = {
	CG_FRAME(    0,    2,   4,           0,    73, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    1,   4,           0,    74, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    75, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl24_crouch_00 ),
};

/* Char_Crouch 1: act_crouching, act_escape */
const Sint32 pl24_crouch_01[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,    76, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl24_crouch_01 ),
};

/* Char_Crouch 2: act_stand_up, act_crouch */
const Sint32 pl24_crouch_02[] = {
	CG_FRAME(    0,    2,   4,           0,    75, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   4,           0,    74, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    73, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl24_crouch_02 ),
};

/* Char_Turn 0: act_turn */
const Sint32 pl24_turn_00[] = {
	CG_FRAME(    0,    0,   4,           0,    77, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    78, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    79, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl24_turn_00 ),
};

/* Char_Turn 1: act_crouch_turn */
const Sint32 pl24_turn_01[] = {
	CG_FRAME(    0,    0,   4,           0,    80, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,    81, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    82, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl24_turn_01 ),
};

/* Char_Guard 0 */
const Sint32 pl24_guard_00[] = {
	CG_FRAME(    0,    0,   4,           0,    83, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

/* Char_Guard 1: act_guard_hold */
const Sint32 pl24_guard_01[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,    84, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl24_guard_01 ),
};

/* Char_Guard 2 */
const Sint32 pl24_guard_02[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,    83, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl24_guard_02 ),
};

/* Char_Guard 3 */
const Sint32 pl24_guard_03[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,    85, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl24_guard_03 ),
};

/* Char_Guard 4 */
const Sint32 pl24_guard_04[] = {
	CG_FRAME(    0,    0,   4,           0,    86, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
};

/* Char_Guard 5: act_guard_hold */
const Sint32 pl24_guard_05[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,    87, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl24_guard_05 ),
};

/* Char_Guard 6 */
const Sint32 pl24_guard_06[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,    86, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl24_guard_06 ),
};

/* Char_Guard 7 */
const Sint32 pl24_guard_07[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,    88, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl24_guard_07 ),
};

/* Char_Air_Guard 0: act_air_guard */
const Sint32 pl24_air_guard_00[] = {
	CG_FRAME(    0,    0,   4,           0,    89, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    90, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Air_Guard 1: act_air_guard_start */
const Sint32 pl24_air_guard_01[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,    91, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl24_air_guard_01 ),
};

/* Char_Air_Guard 2: guard_char_set */
const Sint32 pl24_air_guard_02[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,    91, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl24_air_guard_02 ),
};

/* Char_Stand_P 0 (light) */
const Sint32 pl24_stand_p_00[] = {
	CG_FRAME(    0,    0,   2,           0,   157, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME( 0x80,    0,   1,           0,   158, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   2,           0,   159, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   156, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl24_stand_p_00 ),
};

/* Char_Stand_P 3 */
const Sint32 pl24_stand_p_03[] = {
	CG_FRAME(    0,    0,   2,           0,   412, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   1,           0,   413, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME( 0x80,    0,   2,           0,   414, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,   2,           0,   415, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,   2,           0,   413, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   412, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl24_stand_p_03 ),
};

/* Char_Stand_P 1 (medium) */
const Sint32 pl24_stand_p_01[] = {
	CG_FRAME(    0,    0,   4,           0,   160, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME( 0x80,    0,   2,           0,   161, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_FRAME( 0x80,    0,   1,           0,   162, 0x80,   0x00,  ATK,  0x00,       0x05,    0,   0,     0,      0,  44 ),
	CG_FRAME(    0,    0,   1,           0,   163, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  44 ),
	CG_FRAME(    0,    0,   1,           0,   164, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   6,           0,   165, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   4,           0,   166, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   167, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl24_stand_p_01 ),
};

/* Char_Stand_P 4 */
const Sint32 pl24_stand_p_04[] = {
	CG_FRAME(    0,    0,   3,           0,   425, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   416, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME( 0x80,    0,   3,           0,   417, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,   8,           0,   418, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   2,           0,   416, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,   425, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl24_stand_p_04 ),
};

/* Char_Stand_P 2 (heavy) */
const Sint32 pl24_stand_p_02[] = {
	CG_FRAME(    0,    0,   1,           0,   168, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   169, 0x80,   0x4c,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   1,           0,   170, 0x80,   0x4a,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME( 0x80,    0,   1,           0,   171, 0x80,   0x4a,  ATK,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   1,           0,   172, 0x80,   0x4a,  ATK,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   1,           0,   171, 0x80,   0x4a,  ATK,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   1,           0,   172, 0x80,   0x4a,  ATE,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   1,           0,   171, 0x80,   0x4a,  ATE,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   1,           0,   172, 0x80,   0x4a,  ATE,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   1,           0,   171, 0x80,   0x4a,  ATE,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   1,           0,   172, 0x80,   0x4a,  ATE,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   1,           0,   171, 0x80,   0x4a,  ATE,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   1,           0,   172, 0x80,   0x4a,  ATE,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   4,           0,   173, 0x80,   0x4c,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   174, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl24_stand_p_02 ),
};

/* Char_Stand_P 5 */
const Sint32 pl24_stand_p_05[] = {
	CG_FRAME(    0,    0,   4,           0,   419, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   1,           0,   420, 0x80,   0x69,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   421, 0x80,   0x69,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME( 0x80,    0,   2,           0,   422, 0x80,   0x69,  ATK,  0x00,       0x04,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   1,           0,   423, 0x80,   0x69,  ATK,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   1,           0,   424, 0x80,   0x69,  ATE,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   1,           0,   423, 0x80,   0x69,  ATE,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   1,           0,   424, 0x80,   0x69,  ATE,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   1,           0,   423, 0x80,   0x69,  ATE,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   1,           0,   424, 0x80,   0x69,  ATE,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   1,           0,   423, 0x80,   0x69,  ATE,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   6,           0,   424, 0x80,   0x69,  ATE,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   6,           0,   421, 0x80,   0x69,  ATE,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   425, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_NEXT( pl24_stand_p_05 ),
};

/* Char_Stand_K 0 (light) */
const Sint32 pl24_stand_k_00[] = {
	CG_FRAME(    0,    0,   1,           0,   160, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   175, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME( 0x80,    0,   2,           0,   176, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   4,           0,   177, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  15 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   160, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl24_stand_k_00 ),
};

/* Char_Stand_K 3 */
const Sint32 pl24_stand_k_03[] = {
	CG_FRAME(    0,    0,   1,           0,   426, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   179, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME( 0x80,    0,   1,           0,   180, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,   4,           0,   181, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   178, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl24_stand_k_03 ),
};

/* Char_Stand_K 1 (medium) */
const Sint32 pl24_stand_k_01[] = {
	CG_FRAME(    0,    0,   1,           0,   178, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   179, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME( 0x80,    0,   1,           0,   180, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  16 ),
	CG_FRAME(    0,    0,   6,           0,   181, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  16 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   178, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl24_stand_k_01 ),
};

/* Char_Stand_K 4 */
const Sint32 pl24_stand_k_04[] = {
	CG_FRAME(    0,    0,   4,           0,   427, 0x80,   0x0c,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   428, 0x80,   0x08,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME( 0x80,    0,   2,           0,   429, 0x80,   0x04,  ATK,  0x00,       0x04,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,   8,           0,   430, 0x80,   0x04,  ATE,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,   4,           0,   431, 0x80,   0x08,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   427, 0x80,   0x0c,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl24_stand_k_04 ),
};

/* Char_Dash 2
   Char_Stand_K 2 (heavy) */
const Sint32 pl24_dash_02[] = {
	CG_FRAME(    0,    0,   2,           0,    73, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   1,           0,   182, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   1,           0,   183, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   184, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   1,           0,   185, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME( 0x80,    0,   3,           0,   186, 0x80,   0x00,  ATK,  0x00,       0x05,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,   1,           0,   187, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,   1,           0,   189, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    0,   1,           0,   187, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    0,   1,           0,   189, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    0,   1,           0,   187, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    0,   1,           0,   189, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    0,   1,           0,   187, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    0,   6,           0,   189, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    0,   3,           0,   185, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   184, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   188, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,    76, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   1,           0,    74, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,    73, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl24_dash_02 ),
};

/* Char_Dash 3
   Char_Stand_K 5 */
const Sint32 pl24_dash_03[] = {
	CG_FRAME(    0,    0,   1,           0,    73, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   2,           0,   432, 0x80,   0x0c,  ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   4,           0,   433, 0x80,   0x0c,  ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   2,           0,   434, 0x80,   0x44,  ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME( 0x80,    0,   1,           0,   435, 0x80,   0x44,  ATK,  0x00,       0x03,    0,   0,     0,      0,  65 ),
	CG_FRAME( 0x81,    0,   6,           0,   436, 0x80,   0x44,  ATK,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_FRAME( 0x81,    0,   6,           0,   436, 0x80,   0x44,  ATE,  0x00,          0,    0,   0,     0,      0, 107 ),
	CG_FRAME(    0,    0,   8,           0,   437, 0x80,   0x06,  ATE,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   4,           0,    75, 0x80,   0x0c,  ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   2,           0,    76, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   2,           0,    74, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,    73, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl24_dash_03 ),
};

/* Char_Crouch_P 0 (light) */
const Sint32 pl24_crouch_p_00[] = {
	CG_FRAME(    0,    0,   3,           0,   190, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   191, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,   5,           0,   192, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   193, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl24_crouch_p_00 ),
};

/* Char_Crouch_P 1 (medium) */
const Sint32 pl24_crouch_p_01[] = {
	CG_FRAME(    0,    0,   2,           0,   193, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   3,           0,   194, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME( 0x80,    0,   2,           0,   195, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  22 ),
	CG_FRAME(    0,    0,   2,           0,   196, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  22 ),
	CG_FRAME(    0,    0,   8,           0,   196, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  23 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   194, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl24_crouch_p_01 ),
};

/* Char_Crouch_P 2 (heavy) */
const Sint32 pl24_crouch_p_02[] = {
	CG_FRAME(    0,    0,   6,           0,   197, 0x80,   0x49,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   1,           0,   198, 0x80,   0x49,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   1,           0,   199, 0x80,   0x49,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME( 0x80,    0,   2,           0,   200, 0x80,   0x49,  ATK,  0x00,       0x05,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    0,   2,           0,   201, 0x80,   0x49,  ATK,  0x00,          0,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    0,   1,           0,   202, 0x80,   0x49,  ATE,  0x00,          0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,   1,           0,   201, 0x80,   0x49,  ATE,  0x00,          0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,  10,           0,   202, 0x80,   0x49,  ATE,  0x00,          0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,   6,           0,   203, 0x80,   0x49,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   198, 0x80,   0x49,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl24_crouch_p_02 ),
};

/* Char_Crouch_K 0 (light) */
const Sint32 pl24_crouch_k_00[] = {
	CG_FRAME(    0,    0,   3,           0,   204, 0x80,   0x0d,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME( 0x80,    0,   2,           0,   205, 0x80,   0x49,  ATK,  0x00,          0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,   6,           0,   206, 0x80,   0x49,  ATE,  0x00,          0,    0,   0,     0,      0,  27 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   204, 0x80,   0x0d,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl24_crouch_k_00 ),
};

/* Char_Crouch_K 1 (medium) */
const Sint32 pl24_crouch_k_01[] = {
	CG_FRAME(    0,    0,   3,           0,    75, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   207, 0x80,   0x0a,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME( 0x80,    0,   2,           0,   208, 0x80,   0x61,  ATK,  0x00,       0x04,    0,   0,     0,      0,  28 ),
	CG_FRAME(    0,    0,   2,           0,   209, 0x80,   0x61,  ATK,  0x00,          0,    0,   0,     0,      0,  28 ),
	CG_FRAME(    0,    0,   8,           0,   209, 0x80,   0x61,  ATE,  0x00,          0,    0,   0,     0,      0,  29 ),
	CG_FRAME(    0,    0,   3,           0,   210, 0x80,   0x06,  ATE,  0x00,          0,    0,   0,     0,      0,  29 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    75, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl24_crouch_k_01 ),
};

/* Char_Crouch_K 2 (heavy) */
const Sint32 pl24_crouch_k_02[] = {
	CG_FRAME(    0,    0,   1,           0,    75, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   5,           0,   211, 0x80,   0x0c,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   212, 0x80,   0x0c,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   6,           0,   213, 0x80,   0x41,  ATK,  0x00,          0,    0,   0,     0,      0,  30 ),
	CG_FRAME(    0,    0,   6,           0,   213, 0x80,   0x41,  ATE,  0x00,          0,    0,   0,     0,      0,  31 ),
	CG_FRAME(    0,    0,   8,           0,   214, 0x80,   0x41,  ATE,  0x00,          0,    0,   0,     0,      0,  31 ),
	CG_FRAME(    0,    0,  10,           0,   215, 0x80,   0x06,  ATE,  0x00,          0,    0,   0,     0,      0,  31 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,    75, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl24_crouch_k_02 ),
};

/* Char_Jump_P 0 (light)
   Char_Jump_P 3 */
const Sint32 pl24_jump_p_00[] = {
	CG_FRAME(    0,    0,   3,           0,   216, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,  10,           0,   217, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  32 ),
	CG_FRAME(    0,    0,   6,           0,   217, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  33 ),
	CG_FRAME(    0,    0,   2,           0,    58, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  33 ),
	CG_FRAME(    0,    0,   1,           0,    59, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,    60, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl24_jump_p_00 ),
};

/* Char_Jump_P 1 (medium)
   Char_Jump_P 4 */
const Sint32 pl24_jump_p_01[] = {
	CG_FRAME(    0,    0,   3,           0,   218, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME( 0x80,    0,   2,           0,   219, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   4,           0,   217, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,   2,           0,   217, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  35 ),
	CG_FRAME(    0,    0,   2,           0,    58, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  35 ),
	CG_FRAME(    0,    0,   1,           0,    59, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,    60, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl24_jump_p_01 ),
};

/* Char_Jump_P 2 (heavy)
   Char_Jump_P 5 */
const Sint32 pl24_jump_p_02[] = {
	CG_FRAME(    0,    0,   6,           0,   220, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2,           0,   221, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   6,           0,   222, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,  10,           0,   222, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  37 ),
	CG_FRAME(    0,    0,   4,           0,    58, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2,           0,    59, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,    60, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl24_jump_p_02 ),
};

/* Char_Jump_K 0 (light)
   Char_Jump_K 3 */
const Sint32 pl24_jump_k_00[] = {
	CG_FRAME(    0,    0,   2,           0,    59, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   1,           0,   223, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,  10,           0,   224, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,   4,           0,   224, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  39 ),
	CG_FRAME(    0,    0,   2,           0,    58, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  39 ),
	CG_FRAME(    0,    0,   1,           0,    59, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,    60, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl24_jump_k_00 ),
};

/* Char_Jump_K 1 (medium)
   Char_Jump_K 4 */
const Sint32 pl24_jump_k_01[] = {
	CG_FRAME(    0,    0,   2,           0,   225, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2,           0,   226, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2,           0,   227, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   6,           0,   228, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  40 ),
	CG_FRAME(    0,    0,   8,           0,   228, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,   4,           0,   225, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,   2,           0,    58, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   1,           0,    59, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,    60, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl24_jump_k_01 ),
};

/* Char_Jump_K 2 (heavy)
   Char_Jump_K 5 */
const Sint32 pl24_jump_k_02[] = {
	CG_FRAME(    0,    0,   2,           0,   225, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   6,           0,   229, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   6,           0,   230, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,   8,           0,   230, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,   4,           0,   225, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,   2,           0,    58, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2,           0,    59, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,    60, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl24_jump_k_02 ),
};

/* Char_Get_Up 0: dm_dizzy_wait */
const Sint32 pl24_get_up_00[] = {
	CG_FRAME(    0,    0,   3,           0,   125, 0x80,   0x88,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   126, 0x80,   0x8c,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   127, 0x80,   0x55,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   128, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   129, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   130, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   131, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   132, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   133, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,    73, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl24_get_up_00 ),
};

/* Char_Get_Up 1 */
const Sint32 pl24_get_up_01[] = {
	CG_FRAME(    0,    0,   2,           0,   134, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   135, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   136, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   137, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,    76, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,    75, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,    74, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    73, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl24_get_up_01 ),
};

/* Char_Get_Up 2 */
const Sint32 pl24_get_up_02[] = {
	CG_FRAME(    0,    0,   3,           0,   134, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   135, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   136, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   137, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   134, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   135, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   136, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   137, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   134, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   135, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   136, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   137, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,    76, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,    75, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,    74, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,    73, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl24_get_up_02 ),
};

/* Char_Get_Up 3 */
const Sint32 pl24_get_up_03[] = {
	CG_FRAME(    0,    0,   5,           0,   137, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   136, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   135, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   134, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,    76, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,    75, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,    74, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    73, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl24_get_up_03 ),
};

/* Char_Appear 0: appear_init */
const Sint32 pl24_appear_00[] = {
	CG_FRAME(    0,    0,  64,           0,   114, 0x80,   0x29,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   115, 0x80,   0x29,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   116, 0x80,   0x29,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  36,           0,   117, 0x80,   0x29,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   118, 0x80,   0x29,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,   -1,   6,           0,   119, 0x80,   0x00,    0,  0x00,       0x0a,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,  64,           0,   120, 0x80,   0x00,    0,  0x00,       0x1e,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   121, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl24_appear_00 ),
};

/* Char_Win 0: win_ko_init, win_time_init */
const Sint32 pl24_win_00[] = {
	CG_FRAME(    0,    0,   1,           0,    92, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   7,           0,    92, 0x80,   0x00,    0,  0x00,       0x10,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8,           0,    93, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8,           0,    94, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8,           0,    95, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  16,           0,    96, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  16,           0,    97, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,    98, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl24_win_00_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,    99, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl24_win_00_loop ),
};

/* Char_Win 1: win_ko_init */
const Sint32 pl24_win_01[] = {
	CG_FRAME(    0,    0,   6,           0,   100, 0x80,   0x33,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   101, 0x80,   0x33,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  10,           0,   102, 0x80,   0x33,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   103, 0x80,   0x33,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   104, 0x80,   0x33,    0,  0x00,       0x0f,    0,   0,     0,      0,   0 ),
};

const Sint32 pl24_win_01_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   104, 0x80,   0x33,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl24_win_01_loop ),
};

/* Char_Win 2: win_ko_init */
const Sint32 pl24_win_02[] = {
	CG_FRAME(    0,    0,   4,           0,   144, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   145, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   146, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  20,           0,   147, 0x80,   0x00,    0,  0x00,       0x10,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8,           0,   148, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   149, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  32,           0,   150, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl24_win_02_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   151, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl24_win_02_loop ),
};

/* Char_Win 3: win_ko_init */
const Sint32 pl24_win_03[] = {
	CG_FRAME(    0,    0,   4,           0,   144, 0x80,   0x33,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   145, 0x80,   0x33,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   152, 0x80,   0x3a,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  20,           0,   153, 0x80,   0x3a,    0,  0x00,       0x10,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   154, 0x80,   0x3a,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl24_win_03_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   155, 0x80,   0x3a,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl24_win_03_loop ),
};

/* Char_Win 4: win_time_init */
const Sint32 pl24_win_04[] = {
	CG_FRAME(    0,    0,   6,           0,   105, 0x80,   0x33,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  10,           0,   106, 0x80,   0x33,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   107, 0x80,   0x33,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   108, 0x80,   0x33,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl24_win_04_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   109, 0x80,   0x33,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl24_win_04_loop ),
};

/* Char_Win 5: draw_time */
const Sint32 pl24_win_05[] = {
	CG_FRAME(    0,    0,   4,           0,   144, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   145, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl24_win_05_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   110, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl24_win_05_loop ),
};

/* Char_Win 6: lose_time_ko */
const Sint32 pl24_win_06[] = {
	CG_FRAME(    0,    0,   4,           0,   410, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   411, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   410, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   409, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   377, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   378, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
};

const Sint32 pl24_win_06_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   379, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl24_win_06_loop ),
};

/* Char_Damage 0: dm_freeze, dm_freeze_air, caught_dm */
const Sint32 pl24_damage_00[] = {
	CG_FRAME(    0,    0,   2,           0,   343, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   347, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   343, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl24_damage_00 ),
};

/* Char_Damage 3 */
const Sint32 pl24_damage_03[] = {
	CG_FRAME(    0,    0,   2,           0,   350, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   354, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   350, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl24_damage_03 ),
};

/* Char_Damage 1 */
const Sint32 pl24_damage_01[] = {
	CG_FRAME(    0,    0,   4,           0,   344, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   348, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   343, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl24_damage_01 ),
};

/* Char_Damage 4 */
const Sint32 pl24_damage_04[] = {
	CG_FRAME(    0,    0,   2,           0,   351, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  10,           0,   355, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   350, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl24_damage_04 ),
};

/* Char_Damage 2 */
const Sint32 pl24_damage_02[] = {
	CG_FRAME(    0,    0,   4,           0,   345, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  12,           0,   349, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   346, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl24_damage_02 ),
};

/* Char_Damage 5 */
const Sint32 pl24_damage_05[] = {
	CG_FRAME(    0,    0,   4,           0,   352, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  12,           0,   356, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   353, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl24_damage_05 ),
};

/* Char_Damage 6 */
const Sint32 pl24_damage_06[] = {
	CG_FRAME(    0,    0,   2,           0,   357, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   6,           0,   358, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   357, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl24_damage_06 ),
};

/* Char_Damage 7 */
const Sint32 pl24_damage_07[] = {
	CG_FRAME(    0,    0,   4,           0,   359, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   8,           0,   360, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   357, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl24_damage_07 ),
};

/* Char_Damage 8 */
const Sint32 pl24_damage_08[] = {
	CG_FRAME( 0x80,    0,   4,           0,   361, 0x80,   0x53,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME( 0x82,    0,  12,           0,   362, 0x80,   0x53,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   357, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl24_damage_08 ),
};

/* Char_Damage 10: dm_ko_blow */
const Sint32 pl24_damage_10[] = {
	CG_FRAME(    0,    0,   1,           0,   363, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   8,           0,   366, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   363, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl24_damage_10 ),
};

/* Char_Damage 11 */
const Sint32 pl24_damage_11[] = {
	CG_FRAME(    0,    0,   1,           0,   364, 0x80,   0x36,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   7,           0,   367, 0x80,   0x36,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   363, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl24_damage_11 ),
};

/* Char_Damage 12 */
const Sint32 pl24_damage_12[] = {
	CG_FRAME(    0,    0,   1,           0,   365, 0x80,   0x35,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   6,           0,   368, 0x80,   0x35,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   364, 0x80,   0x34,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   363, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl24_damage_12 ),
};

/* Char_Damage 13: dm_snap_fly, dm_air
   Char_Damage 14
   Char_Damage 15
   Char_Damage 34: dm_snap_fly, dm_snap_down */
const Sint32 pl24_damage_13[] = {
	CG_FRAME(    0,    0,   2,           0,   372, 0x80,   0x4a,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   373, 0x80,   0x49,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
};

const Sint32 pl24_damage_13_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   374, 0x80,   0x49,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl24_damage_13_loop ),
};

/* Char_Damage 9: dm_sweep */
const Sint32 pl24_damage_09[] = {
	CG_FRAME(    0,    0,   4,           0,   375, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  12,           0,   376, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   377, 0x80,   0x52,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   392, 0x80,   0x4a,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   3,           0,   393, 0x80,   0x09,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
};

const Sint32 pl24_damage_09_loop[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   394, 0x80,   0xa9,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl24_damage_09_loop ),
};

/* Char_Damage 16: dm_blow_char_set (16 + no), dm_spark, caught_shock_init */
const Sint32 pl24_damage_16[] = {
	CG_FRAME(    0,    0,   2,           0,   380, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   381, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl24_damage_16 ),
};

/* Char_Damage 17: dm_07_1 */
const Sint32 pl24_damage_17[] = {
	CG_FRAME(    0,    0,   2,           0,   382, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   383, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl24_damage_17 ),
};

/* Char_Damage 18 */
const Sint32 pl24_damage_18[] = {
	CG_FRAME(    0,    0,   2,           0,   384, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   385, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl24_damage_18 ),
};

/* Char_Damage 19 */
const Sint32 pl24_damage_19[] = {
	CG_FRAME(    0,    0,   2,           0,   386, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   387, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl24_damage_19 ),
};

/* Char_Damage 20 */
const Sint32 pl24_damage_20[] = {
	CG_FRAME(    0,    0,   2,           0,   388, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   389, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl24_damage_20 ),
};

/* Char_Damage 21: dm_fall_char_set (21 + no) */
const Sint32 pl24_damage_21[] = {
	CG_FRAME(    0,    0,   2,           0,   380, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   381, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 22 */
const Sint32 pl24_damage_22[] = {
	CG_FRAME(    0,    0,   2,           0,   382, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   383, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 23 */
const Sint32 pl24_damage_23[] = {
	CG_FRAME(    0,    0,   2,           0,   384, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   385, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 24 */
const Sint32 pl24_damage_24[] = {
	CG_FRAME(    0,    0,   2,           0,   386, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   387, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 25 */
const Sint32 pl24_damage_25[] = {
	CG_FRAME(    0,    0,   2,           0,   388, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   389, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl24_damage_25 ),
};

/* Char_Damage 26: dm_buttobi_fly, dm_07_1, dm_away_fly, dm_bound_hop, ... */
const Sint32 pl24_damage_26[] = {
	CG_FRAME(    0,    0,   2,           0,   390, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   391, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   392, 0x80,   0x0b,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   393, 0x80,   0x29,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   395, 0x80,   0x0b,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   394, 0x80,   0xa9,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl24_damage_26 ),
};

/* Char_Damage 27 */
const Sint32 pl24_damage_27[] = {
	CG_FRAME(    0,    0,   3,           0,   390, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   137, 0x80,   0x4f,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   136, 0x80,   0x4f,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   135, 0x80,   0x4f,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   134, 0x80,   0x4f,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    75, 0x80,   0x4e,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    76, 0x80,   0x4e,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    74, 0x80,   0x4e,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,    73, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl24_damage_27 ),
};

/* Char_Damage 30: act_air_recover, act_change_out_init, caught_blow_fly, caught_blow_fix_fly, ... */
const Sint32 pl24_damage_30[] = {
	CG_FRAME(    0,    0,   4,           0,    66, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    67, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    68, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    70, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    61, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    62, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl24_damage_30 ),
};

/* Char_Damage 32: act_escape */
const Sint32 pl24_damage_32[] = {
	CG_FRAME(    0,    0,   4,           0,    66, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    67, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    68, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    70, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl24_damage_32 ),
};

/* Char_Damage 33: act_escape */
const Sint32 pl24_damage_33[] = {
	CG_FRAME(    0,    0,   3,           0,   390, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   137, 0x80,   0x4f,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   136, 0x80,   0x4f,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   135, 0x80,   0x4f,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   134, 0x80,   0x4f,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    75, 0x80,   0x4f,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    76, 0x80,   0x4e,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    74, 0x80,   0x4e,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,    73, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl24_damage_33 ),
};

/* Char_Damage 28: dm_away_init, dm_bound_fly, dm_piyo, dm_away */
const Sint32 pl24_damage_28[] = {
	CG_FRAME(    0,    0,   4,           0,   397, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   398, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   399, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   400, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   401, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   402, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl24_damage_28 ),
};

/* Char_Damage 29: dm_away_fly, dm_bound_hop, dm_bound, caught_spin_fly, ... */
const Sint32 pl24_damage_29[] = {
	CG_FRAME(    0,    0,   4,           0,   403, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   404, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   405, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   406, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   407, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   408, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl24_damage_29 ),
};

/* Char_Damage 31: act_down, dm_dizzy_down */
const Sint32 pl24_damage_31[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   396, 0x80,   0x89,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl24_damage_31 ),
};

/* Char_Dizzy 0: act_dizzy */
const Sint32 pl24_dizzy_00[] = {
	CG_FRAME(    0,    0,  12,           0,   369, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  12,           0,   370, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  12,           0,   371, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  12, END_OF_DATA,   370, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl24_dizzy_00 ),
};

/* Char_Catch 0: nml_k_set */
const Sint32 pl24_catch_00[] = {
	CG_FRAME(    0,    0,  10,           0,   231, 0x80,   0x00,  ATK,  0x01,          0,    0,   0,     0, 0x0001,   0 ),
	CG_FRAME(    0,    0,   2,           0,   232, 0x80,   0x00,  ATK,  0x01,          0,    0,   0,     0, 0x0002,   0 ),
	CG_FRAME(    0,    1,   4,           0,   233, 0x80,   0x00,  ATK,  0x02,          0,    0,   0,     0, 0x001f,   0 ),
	CG_FRAME(    0,    1,   8,           0,   234, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x001c,   0 ),
	CG_FRAME(    0,    1,   8,           0,   234, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x001d,   0 ),
	CG_FRAME(    0,    0,   1,           0,   234, 0x80,   0x00,  ATK,  0x00,       0x05,    0,   0,     0, 0x001e,   0 ),
	CG_FRAME(    0,    1,   1,           0,   235, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,    75, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,    74, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,    73, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl24_catch_00 ),
};

/* Char_Catch 1: jump_k_set */
const Sint32 pl24_catch_01[] = {
	CG_FRAME(    0,    0,   6,           0,   314, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0003,   0 ),
	CG_FRAME(    0,    0,   1,           0,   315, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0004,   0 ),
	CG_FRAME(    0,    0,   6,           0,   316, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0005,   0 ),
	CG_FRAME(    0,    1,   6,           0,   317, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0006,   0 ),
	CG_FRAME(    0,    1,   1,           0,   235, 0x80,   0x00,  ATK,  0x00,       0x05,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,    74, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,    75, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,    74, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,    73, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl24_catch_01 ),
};

/* Char_Catch 2: jump_p_set */
const Sint32 pl24_catch_02[] = {
	CG_FRAME(    0,    0,   2,           0,   319, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0007,   0 ),
	CG_FRAME(    0,    0,   8,           0,   320, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0008,   0 ),
	CG_FRAME(    0,    0,   4,           0,   321, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0009,   0 ),
	CG_FRAME(    0,    1,  10,           0,   321, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0009,   0 ),
	CG_FRAME(    0,    0,   6,           0,    68, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,    67, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,    66, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,    65, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,    63, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl24_catch_02 ),
};

/* Char_Catch 3: hooligan_h_set */
const Sint32 pl24_catch_03[] = {
	CG_FRAME( 0x80,    0,   1,           0,   442, 0x80,   0x00,  ATK,  0x01,          0,    0,   0,     0, 0x000a,   0 ),
	CG_FRAME(    0,    0,   5,           0,   442, 0x80,   0x00,  ATK,  0x00,       0x04,    0,   0,     0, 0x000a,   0 ),
	CG_FRAME(    0,    0,   6,           0,   443, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x000b,   0 ),
	CG_FRAME(    0,    0,   6,           0,   444, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x000c,   0 ),
	CG_FRAME(    0,    0,   6,           0,   231, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0003,   0 ),
	CG_FRAME(    0,    0,   4,           0,   232, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0004,   0 ),
	CG_FRAME(    0,    1,   4,           0,   233, 0x80,   0x00,  ATK,  0x02,          0,    0,   0,     0, 0x0005,   0 ),
	CG_FRAME(    0,    1,   8,           0,   234, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x001c,   0 ),
	CG_FRAME(    0,    1,   8,           0,   234, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x001d,   0 ),
	CG_FRAME(    0,    0,   1,           0,   234, 0x80,   0x00,  ATK,  0x00,       0x05,    0,   0,     0, 0x001e,   0 ),
	CG_FRAME(    0,    1,   1,           0,   235, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,    74, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,    75, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,    74, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,    73, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl24_catch_03 ),
};

/* Char_Catch 4: hooligan_l_set */
const Sint32 pl24_catch_04[] = {
	CG_FRAME(    0,    0,   6,           0,   445, 0x80,   0x00,  ATK,  0x00,       0x04,    0,   0,     0, 0x000d,   0 ),
	CG_FRAME(    0,    0,   6,           0,   446, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x000e,   0 ),
	CG_FRAME(    0,    0,   6,           0,   447, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x000f,   0 ),
	CG_FRAME(    0,    0,   6,           0,   448, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0010,   0 ),
	CG_FRAME(    0,    0,   6,           0,   445, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x000d,   0 ),
	CG_FRAME(    0,    1,   4,           0,   449, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0011,   0 ),
	CG_FRAME(    0,    0,   6,           0,   450, 0x80,   0x00,  ATK,  0x00,       0x05,    0,   0,     0, 0x0012,   0 ),
	CG_FRAME(    0,    1,   3,           0,    65, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,    66, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,    67, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,    68, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,    70, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl24_catch_04_loop[] = {
	CG_FRAME(    0,    0,   3,           0,    61, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,    62, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl24_catch_04_loop ),
};

/* Char_Catch 5: nml_p_set */
const Sint32 pl24_catch_05[] = {
	CG_FRAME(    0,    0,   3,           0,   236, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0013,   0 ),
	CG_FRAME(    0,    0,   8,           0,   237, 0x80,   0x00,  ATK,  0x01,          0,    0,   0,     0, 0x0014,   0 ),
	CG_FRAME(    0,    0,   6,           0,   238, 0x80,   0x00,  ATK,  0x00,       0x04,    0,   0,     0, 0x0015,   0 ),
	CG_FRAME(    0,    0,   3,           0,   239, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0016,   0 ),
	CG_FRAME(    0,    0,   2,           0,   240, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0017,   0 ),
	CG_FRAME(    0,    0,   3,           0,   241, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0018,   0 ),
	CG_FRAME(    0,    0,   3,           0,   242, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0019,   0 ),
	CG_FRAME(    0,    0,   3,           0,   243, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x001a,   0 ),
	CG_FRAME(    0,    0,   3,           0,   244, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x001b,   0 ),
	CG_FRAME(    0,    1,   2,           0,   244, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  10,           0,   243, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   245, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   246, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   247, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   248, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   249, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   250, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   251, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   252, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   253, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   254, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   255, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   256, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl24_catch_05 ),
};

/* Char_Catch 6: killerbee_dive (Killer Bee Assault (hyper)), killerbee_catch_set */
const Sint32 pl24_catch_06[] = {
	CG_FRAME(    0,    0,   6,           0,   445, 0x80,   0x00,  ATK,  0x00,       0x04,    0,   0,     0, 0x000d,   0 ),
	CG_FRAME(    0,    0,   6,           0,   446, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x000e,   0 ),
	CG_FRAME(    0,    0,   6,           0,   447, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x000f,   0 ),
	CG_FRAME(    0,    0,   6,           0,   448, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0010,   0 ),
	CG_FRAME(    0,    0,   6,           0,   445, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x000d,   0 ),
	CG_FRAME(    0,    1,   4,           0,   449, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0011,   0 ),
	CG_FRAME(    0,    0,   6,           0,   450, 0x80,   0x00,  ATK,  0x00,       0x03,    0,   0,     0, 0x0012,   0 ),
	CG_FRAME(    0,    1,   3,           0,    65, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,    66, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,    67, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,    68, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,    70, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl24_catch_06_loop[] = {
	CG_FRAME(    0,    0,   3,           0,    61, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,    62, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl24_catch_06_loop ),
};

/* Char_Effect 0
   Char_Change 0: act_change_tag_init */
const Sint32 pl24_effect_00[] = {
	CG_FRAME(    0,    0,   2,           0,   144, 0x30,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   146, 0x30,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   148, 0x30,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   149, 0x30,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   150, 0x30,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   151, 0x30,   0x00,  ATK,  0x00,       0x0a,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,  10, END_OF_DATA,   151, 0x30,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl24_effect_00 ),
};

/* Char_Effect 1
   Char_Change 1: act_change_vcounter_init */
const Sint32 pl24_effect_01[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   337, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl24_effect_01 ),
};

/* Char_Effect 2
   Char_Change 2: act_change_out_bound */
const Sint32 pl24_effect_02[] = {
	CG_FRAME(    0,    0,   4,           0,    64, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    65, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    66, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    67, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    68, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,    61, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,    62, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl24_jump_11 ),
};

const Sint32 pl24_unused_02[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,   4,           0,    70, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    72, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	0x000038c8,
};

/* Char_Effect 3
   Char_Change 3: act_stand, act_change_call */
const Sint32 pl24_effect_03[] = {
	CG_FRAME(    0,    0,   1,           0,   338, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   7,           0,   338, 0x80,   0x00,    0,  0x00,       0x0a,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   339, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   340, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   341, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   4,           0,   342, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl24_effect_03_loop[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA,   342, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl24_effect_03_loop ),
};

/* Char_Effect 4
   Char_Change 4: act_change_assist_end */
const Sint32 pl24_effect_04[] = {
	CG_FRAME(    0,    0,   2,           0,   338, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   338, 0x80,   0x00,    0,  0x00,       0x0b,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   339, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   340, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   341, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  12, END_OF_DATA,   342, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl24_effect_04 ),
};

/* Char_Special 0 */
const Sint32 pl24_special_00[] = {
	CG_FRAME(    0,    0,   1,           0,   266, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   267, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   268, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   269, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   270, 0x80,   0x00,  ATK,  0x00, 0x4b | CSE,    0,   0,     0,      0,  47 ),
	CG_FRAME(    0,    1,   4,           0,   271, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
};

const Sint32 pl24_special_00_loop[] = {
	CG_FRAME(    0,    0,   1,           0,   272, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   1,           0,   273, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   1,           0,   274, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   1,           0,   275, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   1,           0,   276, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   1,           0,   277, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   1,           0,   278, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   1,           0,   279, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   280, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_NEXT( pl24_special_00_loop ),
};

/* Char_Special 1
   Char_Special 2 */
const Sint32 pl24_special_01[] = {
	CG_FRAME(    0,    0,   1,           0,   266, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   267, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   268, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   269, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   270, 0x80,   0x00,  ATK,  0x00, 0x4b | CSE,    0,   0,     0,      0,  47 ),
	CG_FRAME(    0,    1,   4,           0,   271, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
};

const Sint32 pl24_special_01_loop[] = {
	CG_FRAME(    0,    0,   1,           0,   272, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   1,           0,   273, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   1,           0,   274, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   1,           0,   275, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   1,           0,   276, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   1,           0,   277, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   1,           0,   278, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   1,           0,   279, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   280, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_NEXT( pl24_special_01_loop ),
};

/* Char_Special 23: revenge_counter (Cannon Revenge) */
const Sint32 pl24_special_23[] = {
	CG_FRAME(    0,    0,   1,           0,   266, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   267, 0x80,   0x00,  ATK,  0x00,       0x19,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   268, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   269, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   270, 0x80,   0x00,  ATK,  0x00, 0x4b | CSE,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    1,   4,           0,   271, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  92 ),
};

const Sint32 pl24_special_23_loop[] = {
	CG_FRAME(    0,    0,   1,           0,   272, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  92 ),
	CG_FRAME(    0,    0,   1,           0,   273, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  92 ),
	CG_FRAME(    0,    0,   1,           0,   274, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  92 ),
	CG_FRAME(    0,    0,   1,           0,   275, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  92 ),
	CG_FRAME(    0,    0,   1,           0,   276, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  92 ),
	CG_FRAME(    0,    0,   1,           0,   277, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  92 ),
	CG_FRAME(    0,    0,   1,           0,   278, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  92 ),
	CG_FRAME(    0,    0,   1,           0,   279, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  92 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   280, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  92 ),
	CG_NEXT( pl24_special_23_loop ),
};

/* Char_Special 3: spike_up */
const Sint32 pl24_special_03[] = {
	CG_FRAME(    0,    0,   4,           0,   281, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 105 ),
	CG_FRAME(    0,    0,   4,           0,   282, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 105 ),
	CG_FRAME(    0,    0,   4,           0,   283, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 105 ),
	CG_FRAME(    0,    0,   4,           0,   284, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 105 ),
};

const Sint32 pl24_special_03_loop[] = {
	CG_FRAME(    0,    0,   4,           0,    61, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 105 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    62, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 105 ),
	CG_NEXT( pl24_special_03_loop ),
};

/* Char_Special 4: spike_hit
   Char_Special 17: smasher_air (Spin Drive Smasher (hyper)) */
const Sint32 pl24_special_04[] = {
	CG_FRAME(    0,    0,   8,           0,   281, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 105 ),
	CG_FRAME(    0,    0,   8,           0,   282, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 105 ),
	CG_FRAME(    0,    0,   6,           0,   283, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 105 ),
	CG_FRAME(    0,    0,   6,           0,   284, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 105 ),
};

const Sint32 pl24_special_04_loop[] = {
	CG_FRAME(    0,    0,   4,           0,    61, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 105 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    62, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 105 ),
	CG_NEXT( pl24_special_04_loop ),
};

/* Char_Special 5: spike_fall */
const Sint32 pl24_special_05[] = {
	CG_FRAME(    0,    2,   2,           0,    73, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    1,   2,           0,    74, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   8,           0,    75, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    76, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl24_special_05 ),
};

/* Char_Special 18: smasher_fall (Spin Drive Smasher (hyper)) */
const Sint32 pl24_special_18[] = {
	CG_FRAME(    0,    0,   2,           0,    73, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,   2,           0,    74, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,  10,           0,    75, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    76, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl24_special_18 ),
};

/* Char_Special 24 */
const Sint32 pl24_special_24[] = {
	CG_FRAME(    0,    1,   8,           0,   454, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  94 ),
	CG_FRAME(    0,    1,   4,           0,   455, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  94 ),
	CG_FRAME(    0,    1,   3,           0,   456, 0x80,   0x00,  ATK,  0x00,       0x03,    0,   0,     0,      0,  94 ),
	CG_FRAME(    0,    1,   3,           0,   456, 0x80,   0x00,  ATK,  0x00, 0x4b | CSE,    0,   0,     0,      0,  94 ),
};

const Sint32 pl24_special_24_loop[] = {
	CG_FRAME(    0,    0,   1,           0,   457, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   1,           0,   458, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   1,           0,   459, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   1,           0,   460, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   1,           0,   461, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   1,           0,   462, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   1,           0,   463, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   1,           0,   464, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   465, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_NEXT( pl24_special_24_loop ),
};

/* Char_Special 25
   Char_Special 26 */
const Sint32 pl24_special_25[] = {
	CG_FRAME(    0,    1,  10,           0,   454, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  94 ),
	CG_FRAME(    0,    1,   4,           0,   455, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  94 ),
	CG_FRAME(    0,    1,   3,           0,   456, 0x80,   0x00,  ATK,  0x00,       0x03,    0,   0,     0,      0,  94 ),
	CG_FRAME(    0,    1,   3,           0,   456, 0x80,   0x00,  ATK,  0x00, 0x4b | CSE,    0,   0,     0,      0,  94 ),
};

const Sint32 pl24_special_25_loop[] = {
	CG_FRAME(    0,    0,   1,           0,   457, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   1,           0,   458, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   1,           0,   459, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   1,           0,   460, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   1,           0,   461, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   1,           0,   462, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   1,           0,   463, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   1,           0,   464, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   465, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_NEXT( pl24_special_25_loop ),
};

/* Char_Special 27: strike_fall (Cannon Strike) */
const Sint32 pl24_special_27[] = {
	CG_FRAME(    0,    2,   2,           0,    73, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    1,   2,           0,    74, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,    75, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,    76, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl24_special_27 ),
};

/* Char_Super 0: killerbee_floor_ck (Killer Bee Assault (hyper)) */
const Sint32 pl24_super_00[] = {
	CG_FRAME(    0,    0,   2,           0,   322, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   323, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl24_super_00 ),
};

/* Char_Super 1: killerbee_ceil_ck (Killer Bee Assault (hyper)) */
const Sint32 pl24_super_01[] = {
	CG_FRAME(    0,    0,   2,           0,   324, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   325, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl24_super_01 ),
};

/* Char_Super 2: killerbee_jump (Killer Bee Assault (hyper)), killerbee_left_ck (Killer Bee Assault (hyper)), killerbee_right_ck (Killer Bee Assault (hyper)) */
const Sint32 pl24_super_02[] = {
	CG_FRAME(    0,    0,   2,           0,   326, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   327, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl24_super_02 ),
};

/* Char_Super 3 */
const Sint32 pl24_super_03[] = {
	CG_FRAME( 0x80,    0,   1,           0,   328, 0x80,   0x00,  ATK,  0x00, 0x4b | CSE,    0,   0,     0,      0,  96 ),
};

const Sint32 pl24_super_03_loop[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA,   328, 0x80,   0x00,  ATK,  0x00,       0x05,    0,   0,     0,      0,  96 ),
	CG_NEXT( pl24_super_03_loop ),
};

/* Char_Super 4 */
const Sint32 pl24_super_04[] = {
	CG_FRAME( 0x80,    0,   1,           0,   329, 0x80,   0x00,  ATK,  0x00, 0x4b | CSE,    0,   0,     0,      0,  97 ),
};

const Sint32 pl24_super_04_loop[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA,   329, 0x80,   0x00,  ATK,  0x00,       0x05,    0,   0,     0,      0,  97 ),
	CG_NEXT( pl24_super_04_loop ),
};

/* Char_Super 5 */
const Sint32 pl24_super_05[] = {
	CG_FRAME( 0x80,    0,   1,           0,   330, 0x80,   0x00,  ATK,  0x00, 0x4b | CSE,    0,   0,     0,      0,  98 ),
};

const Sint32 pl24_super_05_loop[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA,   330, 0x80,   0x00,  ATK,  0x00,       0x05,    0,   0,     0,      0,  98 ),
	CG_NEXT( pl24_super_05_loop ),
};

/* Char_Super 6 */
const Sint32 pl24_super_06[] = {
	CG_FRAME( 0x80,    0,   1,           0,   331, 0x80,   0x00,  ATK,  0x00, 0x4b | CSE,    0,   0,     0,      0,  99 ),
};

const Sint32 pl24_super_06_loop[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA,   331, 0x80,   0x00,  ATK,  0x00,       0x05,    0,   0,     0,      0,  99 ),
	CG_NEXT( pl24_super_06_loop ),
};

/* Char_Super 7 */
const Sint32 pl24_super_07[] = {
	CG_FRAME( 0x80,    0,   1,           0,   332, 0x80,   0x00,  ATK,  0x00, 0x4b | CSE,    0,   0,     0,      0, 100 ),
};

const Sint32 pl24_super_07_loop[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA,   332, 0x80,   0x00,  ATK,  0x00,       0x05,    0,   0,     0,      0, 100 ),
	CG_NEXT( pl24_super_07_loop ),
};

/* Char_Super 8 */
const Sint32 pl24_super_08[] = {
	CG_FRAME( 0x80,    0,   1,           0,   333, 0x80,   0x00,  ATK,  0x00, 0x4b | CSE,    0,   0,     0,      0, 101 ),
};

const Sint32 pl24_super_08_loop[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA,   333, 0x80,   0x00,  ATK,  0x00,       0x05,    0,   0,     0,      0, 101 ),
	CG_NEXT( pl24_super_08_loop ),
};

/* Char_Super 9 */
const Sint32 pl24_super_09[] = {
	CG_FRAME( 0x80,    0,   1,           0,   334, 0x80,   0x00,  ATK,  0x00, 0x4b | CSE,    0,   0,     0,      0, 102 ),
};

const Sint32 pl24_super_09_loop[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA,   334, 0x80,   0x00,  ATK,  0x00,       0x05,    0,   0,     0,      0, 102 ),
	CG_NEXT( pl24_super_09_loop ),
};

/* Char_Super 10 */
const Sint32 pl24_super_10[] = {
	CG_FRAME( 0x80,    0,   1,           0,   335, 0x80,   0x00,  ATK,  0x00, 0x4b | CSE,    0,   0,     0,      0, 103 ),
};

const Sint32 pl24_super_10_loop[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA,   335, 0x80,   0x00,  ATK,  0x00,       0x05,    0,   0,     0,      0, 103 ),
	CG_NEXT( pl24_super_10_loop ),
};

/* Char_Super 11 */
const Sint32 pl24_super_11[] = {
	CG_FRAME( 0x80,    0,   1,           0,   336, 0x80,   0x00,  ATK,  0x00, 0x4b | CSE,    0,   0,     0,      0, 104 ),
};

const Sint32 pl24_super_11_loop[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA,   336, 0x80,   0x00,  ATK,  0x00,       0x05,    0,   0,     0,      0, 104 ),
	CG_NEXT( pl24_super_11_loop ),
};

/* Char_Super 12: killerbee_init (Killer Bee Assault (hyper)) */
const Sint32 pl24_super_12[] = {
	CG_FRAME(    0,    1,   8,           0,   287, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
};

const Sint32 pl24_super_12_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   137, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   136, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   135, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   134, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl24_super_12_loop ),
};

/* Char_Super 13: killerbee_fall (Killer Bee Assault (hyper)) */
const Sint32 pl24_super_13[] = {
	CG_FRAME(    0,    0,   4,           0,    66, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    67, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    68, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl24_super_13_loop[] = {
	CG_FRAME(    0,    0,   2,           0,    61, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,    62, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl24_super_13_loop ),
};

/* Char_Super 14: killerbee_miss (Killer Bee Assault (hyper)) */
const Sint32 pl24_super_14[] = {
	CG_FRAME(    0,    2,   2,           0,    73, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    1,   2,           0,    74, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   8,           0,    75, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    76, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl24_super_14 ),
};

/* Char_Super 15: shaft_init (Reverse Shaft Breaker (hyper)) */
const Sint32 pl24_super_15[] = {
	CG_FRAME(    0,    0,   8,           0,   287, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,   1,           0,   287, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   1,           0,   182, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   1,           0,   433, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   1,           0,   434, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   1,           0,   126, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   1,           0,   127, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   1,           0,   128, 0x80,   0x00,  ATK,  0x01,       0x04,    0,   0,     0,      0, 111 ),
	CG_FRAME(    0,    0,   1,           0,   129, 0x80,   0x00,  ATK,  0x01, 0x4b | CSE,    0,   0,     0,      0, 111 ),
	CG_FRAME(    0,    0,   1,           0,   130, 0x80,   0x00,  ATK,  0x01,          0,    0,   0,     0,      0, 111 ),
};

const Sint32 pl24_super_15_loop[] = {
	CG_FRAME(    0,    1,   1,           0, 32770, 0x80,   0x00,  ATK,  0x01,          0,    1,   0,     0,      0, 112 ),
	CG_FRAME(    0,    1,   1,           0, 32773, 0x80,   0x00,  ATK,  0x00,          0,    1,   0,     0,      0, 112 ),
	CG_FRAME(    0,    1,   1,           0, 32771, 0x80,   0x00,  ATK,  0x01,          0,    1,   0,     0,      0, 112 ),
	CG_FRAME(    0,    1,   1,           0, 32772, 0x80,   0x00,  ATK,  0x00,          0,    1,   0,     0,      0, 112 ),
	CG_FRAME(    0,    1,   1, END_OF_DATA, 32775, 0x80,   0x00,  ATK,  0x01,          0,    1,   0,     0,      0, 112 ),
	CG_NEXT( pl24_super_15_loop ),
};

/* Char_Super 16: shaft_up (Reverse Shaft Breaker (hyper)) */
const Sint32 pl24_super_16[] = {
	CG_FRAME(    0,    0,   2,           0, 32770, 0x80,   0x00,  ATK,  0x00,          0,    1,   0,     0,      0, 112 ),
	CG_FRAME(    0,    0,   2,           0, 32773, 0x80,   0x00,  ATK,  0x00,          0,    1,   0,     0,      0, 112 ),
	CG_FRAME(    0,    0,   6,           0,   253, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 106 ),
	CG_FRAME(    0,    0,   2,           0,   254, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 106 ),
	CG_FRAME(    0,    0,   4,           0,    58, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0, 106 ),
	CG_FRAME(    0,    0,   3,           0,    59, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0, 106 ),
	CG_FRAME(    0,    0,   3,           0,    60, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0, 106 ),
};

const Sint32 pl24_super_16_loop[] = {
	CG_FRAME(    0,    0,   2,           0,    61, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0, 106 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,    62, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0, 106 ),
	CG_NEXT( pl24_super_16_loop ),
};

/* Char_Super 17: shaft_fall (Reverse Shaft Breaker (hyper)) */
const Sint32 pl24_super_17[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA,    73, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl24_super_17 ),
};

/* Char_Special 19 */
const Sint32 pl24_special_19[] = {
	CG_FRAME(    0,    0,  32,           0,   139, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl24_special_19_loop[] = {
	CG_FRAME(    0,    1,   6,           0,    41, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  87 ),
	CG_FRAME(    0,    0,   6,           0,    42, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  87 ),
	CG_FRAME(    0,    0,   6,           0,    43, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  87 ),
	CG_FRAME(    0,    0,   6,           0,    44, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  87 ),
	CG_FRAME(    0,    0,   6,           0,    45, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  87 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,    46, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  87 ),
	CG_NEXT( pl24_special_19_loop ),
};

/* Char_Special 20 */
const Sint32 pl24_special_20[] = {
	CG_FRAME(    0,    0,  32, END_OF_DATA,   266, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl24_special_20 ),
};

/* Char_Special 6: arrow_init (Spiral Arrow) */
const Sint32 pl24_special_06[] = {
	CG_FRAME(    0,    0,   4,           0,    73, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0, 32768, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0, 32769, 0x80,   0x48,  ATK,  0x00, 0x4b | CSE,    0,   0,     0,      0,   5 ),
};

const Sint32 pl24_special_06_loop[] = {
	CG_FRAME(    0,    1,   2,           0, 32770, 0x80,   0x68,  ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   1,           0, 32773, 0x80,   0x68,  ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   2,           0, 32771, 0x80,   0x68,  ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   2,           0, 32772, 0x80,   0x68,  ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA, 32775, 0x80,   0x68,  ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_NEXT( pl24_special_06_loop ),
};

/* Char_Special 21: drill_init (Cannon Drill) */
const Sint32 pl24_special_21[] = {
	CG_FRAME(    0,    0,   4,           0,   182, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   6,           0, 32769, 0x80,   0x48,  ATK,  0x00, 0x4b | CSE,    0,   0,     0,      0,   3 ),
};

const Sint32 pl24_special_21_loop[] = {
	CG_FRAME(    0,    1,   2,           0, 32770, 0x80,   0x68,  ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   1,           0, 32773, 0x80,   0x68,  ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   2,           0, 32771, 0x80,   0x68,  ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   2,           0, 32772, 0x80,   0x68,  ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA, 32775, 0x80,   0x68,  ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_NEXT( pl24_special_21_loop ),
};

/* Char_Special 7: arrow_move (Spiral Arrow) */
const Sint32 pl24_special_07[] = {
	CG_FRAME(    0,    0,  10,           0, 32776, 0x80,   0x68,  ATE,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   8,           0,    76, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    75, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl24_special_07 ),
};

/* Char_Special 22: drill_move (Cannon Drill) */
const Sint32 pl24_special_22[] = {
	CG_FRAME(    0,    0,   4,           0,    69, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,    68, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,    67, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,    66, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,    65, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
};

const Sint32 pl24_special_22_loop[] = {
	CG_FRAME(    0,    0,   4,           0,    63, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    71, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl24_special_22_loop ),
};

/* Char_Special 8 */
const Sint32 pl24_special_08[] = {
	CG_FRAME(    0,    0,   2,           0,   286, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   287, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,   4,           0,   288, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,   2,           0,   289, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    1,   2,           0,   290, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,   0,           0,   291, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,   0,           0,   292, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,   0,           0,   293, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,   2,           0,   294, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,   2,           0,   295, 0x80,   0x69,  ATK,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_FRAME( 0x80,    0,   1,           0,   296, 0x80,   0x69,  ATK,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,   2,           0,   297, 0x80,   0x69,  ATK,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME( 0x80,    1,   2,           0,   298, 0x80,   0x69,  ATK,  0x00,       0x05,    0,   0,     0,      0,  83 ),
	CG_FRAME( 0x80,    0,   2,           0,   299, 0x80,   0x69,  ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME( 0x80,    0,   2,           0,   300, 0x80,   0x69,  ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME( 0x80,    0,   1,           0,   301, 0x80,   0x69,  ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME( 0x80,    0,   1,           0,   302, 0x80,   0x69,  ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME( 0x80,    0,   1,           0,   301, 0x80,   0x69,  ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME( 0x80,    0,   1,           0,   302, 0x80,   0x69,  ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME( 0x80,    0,   1,           0,   301, 0x80,   0x69,  ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME( 0x80,    0,   1,           0,   302, 0x80,   0x69,  ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME( 0x80,    0,   1,           0,   301, 0x80,   0x69,  ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME( 0x80,    0,   1,           0,   302, 0x80,   0x69,  ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME( 0x80,    0,   1,           0,   301, 0x80,   0x69,  ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME( 0x80,    0,   1,           0,   302, 0x80,   0x69,  ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   6,           0,   303, 0x80,   0x69,  ATE,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   304, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_NEXT( pl24_special_08 ),
};

/* Char_Special 9
   Char_Special 10 */
const Sint32 pl24_special_09[] = {
	CG_FRAME(    0,    0,   2,           0,   286, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   287, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,   4,           0,   288, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,   4,           0,   289, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    1,   1,           0,   290, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,   1,           0,   291, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,   1,           0,   292, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,   2,           0,   293, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,   2,           0,   294, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,   1,           0,   295, 0x80,   0x69,  ATK,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_FRAME( 0x80,    0,   2,           0,   296, 0x80,   0x69,  ATK,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,   2,           0,   297, 0x80,   0x69,  ATK,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME( 0x80,    1,   2,           0,   298, 0x80,   0x69,  ATK,  0x00,       0x05,    0,   0,     0,      0,  83 ),
	CG_FRAME( 0x80,    0,   2,           0,   299, 0x80,   0x69,  ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME( 0x80,    0,   2,           0,   300, 0x80,   0x69,  ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME( 0x80,    0,   1,           0,   301, 0x80,   0x69,  ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME( 0x80,    0,   1,           0,   302, 0x80,   0x69,  ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME( 0x80,    0,   1,           0,   301, 0x80,   0x69,  ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME( 0x80,    0,   1,           0,   302, 0x80,   0x69,  ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME( 0x80,    0,   1,           0,   301, 0x80,   0x69,  ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME( 0x80,    0,   1,           0,   302, 0x80,   0x69,  ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME( 0x80,    0,   1,           0,   301, 0x80,   0x69,  ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME( 0x80,    0,   1,           0,   302, 0x80,   0x69,  ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME( 0x80,    0,   1,           0,   301, 0x80,   0x69,  ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME( 0x80,    0,   1,           0,   302, 0x80,   0x69,  ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   6,           0,   303, 0x80,   0x69,  ATE,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   304, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_NEXT( pl24_special_09 ),
};

/* Char_Special 11: hooligan_init (Hooligan Combination) */
const Sint32 pl24_special_11[] = {
	CG_FRAME(    0,    1,   6,           0,    73, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2,           0,    69, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2,           0,    68, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2,           0,    67, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   7 ),
};

const Sint32 pl24_special_11_loop[] = {
	CG_FRAME(    0,    1,   3,           0,   438, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  79 ),
	CG_FRAME(    0,    0,   3,           0,   439, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  79 ),
	CG_FRAME(    0,    0,   3,           0,   440, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  79 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   441, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  79 ),
	CG_NEXT( pl24_special_11_loop ),
};

/* Char_Special 12: hooligan_jump (Hooligan Combination) */
const Sint32 pl24_special_12[] = {
	CG_FRAME(    0,    0,   6,           0,    66, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   6,           0,    65, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   7 ),
};

const Sint32 pl24_special_12_loop[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA,    63, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl24_special_12_loop ),
};

/* Char_Special 13: hooligan_land (Hooligan Combination) */
const Sint32 pl24_special_13[] = {
	CG_FRAME(    0,    0,   1,           0,   451, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  81 ),
	CG_FRAME(    0,    0,   5,           0,   451, 0x80,   0x00,  ATK,  0x00, 0x29 | CSE,    0,   0,     0,      0,  81 ),
	CG_FRAME(    0,    1,   1,           0,   452, 0x80,   0x6a,  ATK,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,   6,           0,   451, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  81 ),
	CG_FRAME(    0,    0,   6,           0,    88, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  81 ),
	CG_FRAME(    0,    0,   6,           0,    76, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    75, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl24_special_13 ),
};

/* Char_Special 14 */
const Sint32 pl24_special_14[] = {
	CG_FRAME(    0,    0,   4,           0,   119, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    0,    1,  32,           0,   453, 0x80,   0x00,    0,  0x00,       0x0a,    0,   0,     0,      0,  84 ),
	CG_FRAME(    0,    0,   6,           0,    79, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    0,    0,  16,           0,   121, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    0,    0,   4,           0,    79, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    73, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_NEXT( pl24_special_14 ),
};

/* Char_Special 32 */
const Sint32 pl24_special_32[] = {
	CG_FRAME(    0,    0,   4,           0,   119, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    0,    1,  16,           0,   453, 0x80,   0x00,    0,  0x00,       0x0a,    0,   0,     0,      0,  84 ),
	CG_FRAME(    0,    0,   8,           0,    79, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    0,    0,  16,           0,   121, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    0,    0,   6,           0,    79, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,    73, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_NEXT( pl24_special_14 ),
};

/* Char_Special 15: smasher_init (Spin Drive Smasher (hyper)) */
const Sint32 pl24_special_15[] = {
	CG_FRAME(    0,    0,   4,           0,   156, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   286, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   287, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,   1,           0,   287, 0x80,   0x00,  ATK,  0x00,       0x18,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0, 32768, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0, 32769, 0x80,   0x48,  ATK,  0x00, 0x4b | CSE,    0,   0,     0,      0,   5 ),
};

const Sint32 pl24_special_15_loop[] = {
	CG_FRAME(    0,    1,   2,           0, 32770, 0x80,   0x68,  ATK,  0x01,          0,    0,   0,     0,      0,  88 ),
	CG_FRAME(    0,    1,   2,           0, 32773, 0x80,   0x68,  ATK,  0x00,          0,    0,   0,     0,      0,  88 ),
	CG_FRAME(    0,    1,   2,           0, 32771, 0x80,   0x68,  ATK,  0x00,          0,    0,   0,     0,      0,  88 ),
	CG_FRAME(    0,    1,   2,           0, 32772, 0x80,   0x68,  ATK,  0x01,          0,    0,   0,     0,      0,  88 ),
	CG_FRAME(    0,    1,   4, END_OF_DATA, 32775, 0x80,   0x68,  ATK,  0x00,          0,    0,   0,     0,      0,  88 ),
	CG_NEXT( pl24_special_15_loop ),
};

/* Char_Special 16: smasher_move (Spin Drive Smasher (hyper)) */
const Sint32 pl24_special_16[] = {
	CG_FRAME(    0,    0,   1,           0,   266, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   267, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   268, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   269, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   270, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    1,   4,           0,   271, 0x80,   0x00,  ATK,  0x00, 0x4b | CSE,    0,   0,     0,      0,  90 ),
};

const Sint32 pl24_special_16_loop[] = {
	CG_FRAME(    0,    0,   1,           0,   272, 0x80,   0x00,  ATK,  0x01,          0,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,    0,   1,           0,   273, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,    0,   1,           0,   274, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,    0,   1,           0,   275, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,    0,   1,           0,   276, 0x80,   0x00,  ATK,  0x01,          0,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,    0,   1,           0,   277, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,    0,   1,           0,   278, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,    0,   1,           0,   279, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   280, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  90 ),
	CG_NEXT( pl24_special_16_loop ),
};

/* Char_Extra 0: t_kick_init */
const Sint32 pl24_extra_00[] = {
	CG_FRAME(    0,    0,   2,           0,   225, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2,           0,   229, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,  -1, END_OF_DATA,   230, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  95 ),
	CG_NEXT( pl24_extra_00 ),
};

/* Char_Extra 1: t_kick_move */
const Sint32 pl24_extra_01[] = {
	CG_FRAME(    0,    1,   1,           0,    74, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   1,           0,    74, 0x80,   0x00,  ATE,  0x00,       0x1a,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   6,           0,    75, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,    92, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   6,           0,    93, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,    94, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,    95, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,  18, END_OF_DATA,    96, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl24_extra_01 ),
};

/* Char_Special 28: snapback_init (Snapback) */
const Sint32 pl24_special_28[] = {
	CG_FRAME(    0,    0,   3,           0,   168, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   169, 0x80,   0x4c,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,   3,           0,   169, 0x80,   0x4a,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    2,   1,           0,   170, 0x80,   0x4a,  ATK,  0x00,       0x05,    0,   0,     0,      0,   5 ),
	CG_FRAME( 0x80,    0,   1,           0,   171, 0x80,   0x4a,  ATK,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   1,           0,   172, 0x80,   0x4a,  ATK,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   1,           0,   171, 0x80,   0x4a,  ATK,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   1,           0,   172, 0x80,   0x4a,  ATE,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   1,           0,   171, 0x80,   0x4a,  ATE,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   1,           0,   172, 0x80,   0x4a,  ATE,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   1,           0,   171, 0x80,   0x4a,  ATE,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   1,           0,   172, 0x80,   0x4a,  ATE,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   1,           0,   171, 0x80,   0x4a,  ATE,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   1,           0,   172, 0x80,   0x4a,  ATE,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   4,           0,   173, 0x80,   0x4c,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   174, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl24_special_28 ),
};

/* Char_Special 29: hooligan_jump (Hooligan Combination) */
const Sint32 pl24_special_29[] = {
	CG_FRAME(    0,    1,  10,           0,   454, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  94 ),
	CG_FRAME(    0,    1,   4,           0,   455, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  94 ),
	CG_FRAME(    0,    1,   3,           0,   456, 0x80,   0x00,  ATK,  0x00,       0x03,    0,   0,     0,      0,  94 ),
	CG_FRAME(    0,    1,   3,           0,   456, 0x80,   0x00,  ATK,  0x00, 0x4b | CSE,    0,   0,     0,      0,  94 ),
};

const Sint32 pl24_special_29_loop[] = {
	CG_FRAME(    0,    0,   1,           0,   457, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   1,           0,   458, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   1,           0,   459, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   1,           0,   460, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   1,           0,   461, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   1,           0,   462, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   1,           0,   463, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   1,           0,   464, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   465, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_NEXT( pl24_special_29_loop ),
};

/* Char_Special 30: hooligan_kick_fall (Hooligan Combination) */
const Sint32 pl24_special_30[] = {
	CG_FRAME(    0,    2,   3,           0,    73, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    1,   3,           0,    74, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   6,           0,    75, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,    76, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl24_special_30 ),
};

/* Char_Special 31: revenge_counter (Cannon Revenge) */
const Sint32 pl24_special_31[] = {
	CG_FRAME(    0,    0,   6,           0,   197, 0x80,   0x49,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   198, 0x80,   0x49,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   199, 0x80,   0x49,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME( 0x80,    0,   2,           0,   200, 0x80,   0x49,  ATK,  0x00,       0x05,    0,   0,     0,      0, 110 ),
	CG_FRAME(    0,    0,   2,           0,   201, 0x80,   0x49,  ATK,  0x00,          0,    0,   0,     0,      0, 110 ),
	CG_FRAME(    0,    0,   1,           0,   202, 0x80,   0x49,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   201, 0x80,   0x49,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  10,           0,   202, 0x80,   0x49,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   203, 0x80,   0x49,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   198, 0x80,   0x49,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl24_special_31 ),
};

/* Char_Extra 2: sp_chouhatsu (Taunt), fly_start5 */
const Sint32 pl24_extra_02[] = {
	CG_FRAME(    0,    0,   1,           0,   338, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   7,           0,   338, 0x80,   0x00,    0,  0x00,       0x0b,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   339, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   340, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   341, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  12, END_OF_DATA,   342, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl24_extra_02 ),
};

/* Char_Extra 3: pl24_upper, fly_start5 */
const Sint32 pl24_extra_03[] = {
	CG_FRAME(    0,    0,   2,           0,    73, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   1,           0,   182, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   1,           0,   183, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   184, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   1,           0,   185, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME( 0x80,    0,   3,           0,   186, 0x80,   0x00,  ATK,  0x00,       0x05,    0,   0,     0,      0, 108 ),
	CG_FRAME(    0,    0,   1,           0,   187, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 108 ),
	CG_FRAME(    0,    0,   1,           0,   189, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 108 ),
	CG_FRAME(    0,    0,   1,           0,   187, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 109 ),
	CG_FRAME(    0,    0,   1,           0,   189, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 109 ),
	CG_FRAME(    0,    0,   1,           0,   187, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 109 ),
	CG_FRAME(    0,    0,   1,           0,   189, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 109 ),
	CG_FRAME(    0,    0,   1,           0,   187, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 109 ),
	CG_FRAME(    0,    0,   6,           0,   189, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 109 ),
	CG_FRAME(    0,    0,   3,           0,   185, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   184, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   188, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,    76, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   1,           0,    74, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,    73, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl24_extra_03 ),
};

/* group 27 0: pls24_00_cloak_init
   group 31 0 */
const Sint32 pl24_grp27_00[] = {
	EF_FRAME(    0,    0,   1,           0,   122 ),
};

const Sint32 pl24_grp27_00_loop[] = {
	EF_FRAME(    0,    0,   4,           0,   123 ),
	EF_FRAME(    0,    0,   4, END_OF_DATA,   124 ),
	CG_NEXT( pl24_grp27_00_loop ),
};

/* group 27 1: pls24_00_bison_init
   group 31 1 */
const Sint32 pl24_grp27_01[] = {
	EF_FRAME(    0,    0,   4,           0,   481 ),
	EF_FRAME(    0,    0,   4,           0,   480 ),
	EF_FRAME(    0,    0,   4,           0,   479 ),
	EF_FRAME(    0,    0,   4,           0,   478 ),
	EF_FRAME(    0,    0,   4,           0,   477 ),
	EF_FRAME(    0,    1,   1,           0,   475 ),
};

const Sint32 pl24_grp27_01_loop[] = {
	EF_FRAME(    0,    0,   4,           0,   475 ),
	EF_FRAME(    0,    0,   4, END_OF_DATA,   476 ),
	CG_NEXT( pl24_grp27_01_loop ),
};

/* Char_Caught 0 */
const Sint32 pl24_caught_00[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   343, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_NEXT( pl24_caught_00 ),
};

/* Char_Caught 1 */
const Sint32 pl24_caught_01[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   344, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_NEXT( pl24_caught_01 ),
};

/* Char_Caught 2 */
const Sint32 pl24_caught_02[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   345, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_NEXT( pl24_caught_02 ),
};

/* Char_Caught 3 */
const Sint32 pl24_caught_03[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   350, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_NEXT( pl24_caught_03 ),
};

/* Char_Caught 4 */
const Sint32 pl24_caught_04[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   351, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_NEXT( pl24_caught_04 ),
};

/* Char_Caught 5 */
const Sint32 pl24_caught_05[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   352, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_NEXT( pl24_caught_05 ),
};

/* Char_Caught 6 */
const Sint32 pl24_caught_06[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   357, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_NEXT( pl24_caught_06 ),
};

/* Char_Caught 7 */
const Sint32 pl24_caught_07[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   359, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_NEXT( pl24_caught_07 ),
};

/* Char_Caught 8 */
const Sint32 pl24_caught_08[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   361, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_NEXT( pl24_caught_08 ),
};

/* Char_Caught 9 */
const Sint32 pl24_caught_09[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   363, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_NEXT( pl24_caught_09 ),
};

/* Char_Caught 10 */
const Sint32 pl24_caught_10[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   364, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_NEXT( pl24_caught_10 ),
};

/* Char_Caught 11 */
const Sint32 pl24_caught_11[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   365, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_NEXT( pl24_caught_11 ),
};

/* Char_Caught 12 */
const Sint32 pl24_caught_12[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   373, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_NEXT( pl24_caught_12 ),
};

/* Char_Caught 13 */
const Sint32 pl24_caught_13[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   380, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_NEXT( pl24_caught_13 ),
};

/* Char_Caught 14 */
const Sint32 pl24_caught_14[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   382, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_NEXT( pl24_caught_14 ),
};

/* Char_Caught 15 */
const Sint32 pl24_caught_15[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   384, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_NEXT( pl24_caught_15 ),
};

/* Char_Caught 16 */
const Sint32 pl24_caught_16[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   386, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_NEXT( pl24_caught_16 ),
};

/* Char_Caught 17 */
const Sint32 pl24_caught_17[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   388, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_NEXT( pl24_caught_17 ),
};

/* Char_Caught 18 */
const Sint32 pl24_caught_18[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   399, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_NEXT( pl24_caught_18 ),
};

/* Char_Caught 19 */
const Sint32 pl24_caught_19[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   405, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_NEXT( pl24_caught_19 ),
};

/* Char_Caught 20 */
const Sint32 pl24_caught_20[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   396, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_NEXT( pl24_caught_20 ),
};

/* Char_Caught 21 */
const Sint32 pl24_caught_21[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   395, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_NEXT( pl24_caught_21 ),
};
