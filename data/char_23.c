/* Dan (character 23): the animations. PLAYER_WORK's char_adrs points here.

   spr_prm_23      a sprite's sizes and turn (SPR_PRM): a frame's zoom number picks a row, 0 for none.
   char_tbl_23     the groups: pl_char_set( pl_w, group, no ) takes the group's table from here and
                   starts its animation no. The character's module (s_pl23.c) says which group
                   and number each move uses.
   pl23_GROUP_tbl  a group's animations. GROUP is the group's name in player.h (stand for
                   Char_Stand), or grpNN for a group with no name there.
   pl23_GROUP_NN   an animation, named by the first group and number that has it: its frames in
                   order. The comment before it lists its places in the groups and, for each,
                   the routines that start it by that number, with their special move;
                   (2 + atck_level) is a routine that counts on from 2 by the attack's level.
                   A frame with END_OF_DATA is the last, and the word after it is the frame the
                   animation goes on from (CG_NEXT; pl23_GROUP_NN_loop where that is no
                   animation's first frame). An effect's animation has short frames (EF_FRAME)
                   and may end with no word.
   pl23_unused_NN  frames that no table or animation reaches.

   A player's frame (CG_FRAME, the 0x14 bytes of CHAR_DATA), with the names Capcom's tables used:
     prm, type  a number and a code for the character's module (a speed, an attack, a voice)
     ctr        the game frames it is shown for
     end        END_OF_DATA on the last frame
     spr        the sprite, -1 for none
     prio       added to the draw priority (a signed byte)
     shadow     the shadow's offset (the low five bits) and size, 0xff none
     atck       ATK while an attack is on its way or can hit, ATE in its frames after that;
                CG_ATCK_NG lets nothing cancel it
     flag1      a number for the module: most often the attack, a row of hit_dt_23
     se         a voice (1 to 63), or with CSE a sound effect; 0 none
     zoom       the row of spr_prm_23, 0 none
     unk        a byte the program is not seen to read
     flag2      a second code for the character's effects
     rival      where a held opponent is put (the low byte), and a code for the module (the high)
     box        the frame's row of frame_box_23 */
#include "player.h"
#include "char_tbl.h"

extern const Sint32 pl23_stand_00[], pl23_stand_02[], pl23_stand_03[], pl23_jump_00[],
	pl23_jump_00_loop[], pl23_jump_03[], pl23_jump_06[], pl23_jump_09[], pl23_dash_00[],
	pl23_dash_00_loop[], pl23_dash_02[], pl23_dash_02_loop[], pl23_dash_01[], pl23_dash_01_loop[],
	pl23_dash_03[], pl23_dash_03_loop[], pl23_crouch_00[], pl23_crouch_01[], pl23_crouch_02[],
	pl23_turn_00[], pl23_turn_01[], pl23_guard_00[], pl23_guard_01[], pl23_guard_02[],
	pl23_guard_03[], pl23_guard_04[], pl23_guard_05[], pl23_guard_06[], pl23_guard_07[],
	pl23_air_guard_00[], pl23_air_guard_01[], pl23_stand_p_00[], pl23_stand_p_01[],
	pl23_stand_p_02[], pl23_stand_k_00[], pl23_stand_k_01[], pl23_stand_k_02[], pl23_crouch_p_00[],
	pl23_crouch_p_01[], pl23_crouch_p_02[], pl23_extra_02[], pl23_crouch_k_00[], pl23_crouch_k_01[],
	pl23_crouch_k_02[], pl23_jump_p_00[], pl23_jump_p_01[], pl23_jump_p_02[], pl23_jump_k_00[],
	pl23_jump_k_01[], pl23_jump_k_02[], pl23_get_up_00[], pl23_get_up_01[], pl23_get_up_02[],
	pl23_get_up_03[], pl23_damage_00[], pl23_damage_01[], pl23_damage_02[], pl23_damage_03[],
	pl23_damage_04[], pl23_damage_05[], pl23_damage_06[], pl23_damage_07[], pl23_damage_08[],
	pl23_damage_10[], pl23_damage_11[], pl23_damage_12[], pl23_damage_13[], pl23_damage_13_loop[],
	pl23_damage_09[], pl23_damage_09_loop[], pl23_damage_16[], pl23_damage_17[], pl23_damage_18[],
	pl23_damage_19[], pl23_damage_20[], pl23_damage_21[], pl23_damage_22[], pl23_damage_23[],
	pl23_damage_24[], pl23_damage_25[], pl23_damage_26[], pl23_damage_27[], pl23_damage_31[],
	pl23_damage_33[], pl23_damage_28[], pl23_damage_29[], pl23_caught_00[], pl23_caught_01[],
	pl23_caught_02[], pl23_caught_03[], pl23_caught_04[], pl23_caught_05[], pl23_caught_06[],
	pl23_caught_07[], pl23_caught_08[], pl23_caught_09[], pl23_caught_10[], pl23_caught_11[],
	pl23_caught_12[], pl23_caught_13[], pl23_caught_14[], pl23_caught_15[], pl23_caught_16[],
	pl23_caught_17[], pl23_caught_18[], pl23_caught_19[], pl23_caught_20[], pl23_caught_21[],
	pl23_dizzy_00[], pl23_win_00[], pl23_win_00_loop[], pl23_win_01[], pl23_win_01_loop[],
	pl23_win_02[], pl23_win_02_loop[], pl23_win_03[], pl23_win_03_loop[], pl23_appear_00[],
	pl23_appear_00_loop[], pl23_appear_01[], pl23_appear_01_loop[], pl23_unused_00[],
	pl23_unused_01[], pl23_unused_02[], pl23_unused_03[], pl23_extra_00[], pl23_extra_00_loop[],
	pl23_change_03[], pl23_change_03_loop[], pl23_change_01[], pl23_change_01_loop[],
	pl23_change_02[], pl23_change_04[], pl23_change_04_loop[], pl23_catch_00[], pl23_catch_01[],
	pl23_catch_02[], pl23_catch_03[], pl23_special_00[], pl23_special_01[], pl23_special_10[],
	pl23_special_11[], pl23_special_12[], pl23_special_13[], pl23_effect_03[], pl23_effect_03_loop[],
	pl23_effect_04[], pl23_effect_05[], pl23_special_25[], pl23_special_25_loop[], pl23_special_28[],
	pl23_special_31[], pl23_effect_06[], pl23_effect_07[], pl23_effect_08[], pl23_special_03[],
	pl23_special_03_loop[], pl23_special_04[], pl23_special_37[], pl23_special_38[],
	pl23_special_06[], pl23_special_06_loop[], pl23_special_44[], pl23_special_44_loop[],
	pl23_special_07[], pl23_special_07_loop[], pl23_special_45[], pl23_special_45_loop[],
	pl23_special_40[], pl23_special_41[], pl23_special_43[], pl23_super_04[], pl23_super_04_loop[],
	pl23_super_05[], pl23_super_00[], pl23_super_01[], pl23_super_02[], pl23_super_03[],
	pl23_super_06[], pl23_super_06_loop[], pl23_super_07[], pl23_special_14[],
	pl23_special_14_loop[], pl23_special_15[], pl23_special_15_loop[], pl23_special_16[],
	pl23_special_16_loop[], pl23_special_17[], pl23_special_17_loop[], pl23_special_18[],
	pl23_special_18_loop[], pl23_special_19[], pl23_special_20[], pl23_special_21[],
	pl23_special_22[], pl23_special_22_loop[], pl23_special_23[], pl23_special_23_loop[],
	pl23_special_34[], pl23_special_34_loop[], pl23_special_35[], pl23_special_36[], pl23_extra_01[],
	pl23_extra_01_loop[], pl23_change_00[], pl23_change_00_loop[], pl23_effect_00[],
	pl23_effect_01[], pl23_effect_02[];
extern const Sint32 *const pl23_stand_tbl[], *const pl23_jump_tbl[], *const pl23_dash_tbl[],
	*const pl23_crouch_tbl[], *const pl23_turn_tbl[], *const pl23_guard_tbl[],
	*const pl23_air_guard_tbl[], *const pl23_stand_p_tbl[], *const pl23_stand_k_tbl[],
	*const pl23_crouch_p_tbl[], *const pl23_crouch_k_tbl[], *const pl23_jump_p_tbl[],
	*const pl23_jump_k_tbl[], *const pl23_damage_tbl[], *const pl23_dizzy_tbl[],
	*const pl23_catch_tbl[], *const pl23_caught_tbl[], *const pl23_get_up_tbl[],
	*const pl23_appear_tbl[], *const pl23_win_tbl[], *const pl23_extra_tbl[],
	*const pl23_special_tbl[], *const pl23_super_tbl[], *const pl23_effect_tbl[],
	*const pl23_change_tbl[];

/* the sprites' sizes and turns, by a frame's zoom number */
const SPR_PRM spr_prm_23[16] = {
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
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
	{ 1000, 1000, 0, 0, 0, 0, 0, 0 },
};

/* the groups */
const Sint32 *const *const char_tbl_23[] = {
	pl23_stand_tbl, pl23_jump_tbl, pl23_dash_tbl, pl23_crouch_tbl, pl23_turn_tbl, pl23_guard_tbl,
	pl23_air_guard_tbl, pl23_stand_p_tbl, pl23_stand_k_tbl, pl23_crouch_p_tbl, pl23_crouch_k_tbl, pl23_jump_p_tbl,
	pl23_jump_k_tbl, pl23_damage_tbl, pl23_dizzy_tbl, pl23_catch_tbl, pl23_caught_tbl, pl23_get_up_tbl,
	pl23_appear_tbl, pl23_win_tbl, pl23_extra_tbl, pl23_special_tbl, pl23_super_tbl, pl23_effect_tbl,
	pl23_change_tbl, pl23_change_tbl, 0,
};

const Sint32 *const pl23_stand_tbl[] = {
	pl23_stand_00, pl23_stand_00, pl23_stand_02, pl23_stand_03, pl23_stand_00, pl23_stand_00,
	pl23_stand_02, pl23_stand_03, 0,
};

const Sint32 *const pl23_jump_tbl[] = {
	pl23_jump_00, pl23_jump_00, pl23_jump_00, pl23_jump_03, pl23_jump_03, pl23_jump_03,
	pl23_jump_06, pl23_jump_06, pl23_jump_06, pl23_jump_09, pl23_jump_09, pl23_jump_09,
	0,
};

const Sint32 *const pl23_dash_tbl[] = {
	pl23_dash_00, pl23_dash_01, pl23_dash_02, pl23_dash_03, 0,
};

const Sint32 *const pl23_crouch_tbl[] = {
	pl23_crouch_00, pl23_crouch_01, pl23_crouch_02, 0,
};

const Sint32 *const pl23_turn_tbl[] = {
	pl23_turn_00, pl23_turn_01, 0,
};

const Sint32 *const pl23_guard_tbl[] = {
	pl23_guard_00, pl23_guard_01, pl23_guard_02, pl23_guard_03, pl23_guard_04, pl23_guard_05,
	pl23_guard_06, pl23_guard_07, 0,
};

const Sint32 *const pl23_air_guard_tbl[] = {
	pl23_air_guard_00, pl23_air_guard_01, pl23_air_guard_01, 0,
};

const Sint32 *const pl23_stand_p_tbl[] = {
	pl23_stand_p_00, pl23_stand_p_01, pl23_stand_p_02, 0,
};

const Sint32 *const pl23_stand_k_tbl[] = {
	pl23_stand_k_00, pl23_stand_k_01, pl23_stand_k_02, 0,
};

const Sint32 *const pl23_crouch_p_tbl[] = {
	pl23_crouch_p_00, pl23_crouch_p_01, pl23_crouch_p_02, 0,
};

const Sint32 *const pl23_crouch_k_tbl[] = {
	pl23_crouch_k_00, pl23_crouch_k_01, pl23_crouch_k_02, 0,
};

const Sint32 *const pl23_jump_p_tbl[] = {
	pl23_jump_p_00, pl23_jump_p_01, pl23_jump_p_02, pl23_jump_p_00, pl23_jump_p_01, pl23_jump_p_02,
	0,
};

const Sint32 *const pl23_jump_k_tbl[] = {
	pl23_jump_k_00, pl23_jump_k_01, pl23_jump_k_02, pl23_jump_k_00, pl23_jump_k_01, pl23_jump_k_02,
	0,
};

const Sint32 *const pl23_damage_tbl[] = {
	pl23_damage_00, pl23_damage_01, pl23_damage_02, pl23_damage_03, pl23_damage_04, pl23_damage_05,
	pl23_damage_06, pl23_damage_07, pl23_damage_08, pl23_damage_09, pl23_damage_10, pl23_damage_11,
	pl23_damage_12, pl23_damage_13, pl23_damage_13, pl23_damage_13, pl23_damage_16, pl23_damage_17,
	pl23_damage_18, pl23_damage_19, pl23_damage_20, pl23_damage_21, pl23_damage_22, pl23_damage_23,
	pl23_damage_24, pl23_damage_25, pl23_damage_26, pl23_damage_27, pl23_damage_28, pl23_damage_29,
	pl23_damage_27, pl23_damage_31, pl23_damage_27, pl23_damage_33, pl23_damage_13, 0,
};

const Sint32 *const pl23_dizzy_tbl[] = {
	pl23_dizzy_00, 0,
};

const Sint32 *const pl23_catch_tbl[] = {
	pl23_catch_00, pl23_catch_01, pl23_catch_02, pl23_catch_03, 0,
};

const Sint32 *const pl23_caught_tbl[] = {
	pl23_caught_00, pl23_caught_01, pl23_caught_02, pl23_caught_03, pl23_caught_04, pl23_caught_05,
	pl23_caught_06, pl23_caught_07, pl23_caught_08, pl23_caught_09, pl23_caught_10, pl23_caught_11,
	pl23_caught_12, pl23_caught_13, pl23_caught_14, pl23_caught_15, pl23_caught_16, pl23_caught_17,
	pl23_caught_18, pl23_caught_19, pl23_caught_20, pl23_caught_21, 0,
};

const Sint32 *const pl23_get_up_tbl[] = {
	pl23_get_up_00, pl23_get_up_01, pl23_get_up_02, pl23_get_up_03, 0,
};

const Sint32 *const pl23_appear_tbl[] = {
	pl23_appear_00, pl23_appear_01, 0,
};

const Sint32 *const pl23_win_tbl[] = {
	pl23_win_00, pl23_win_01, pl23_win_02, pl23_win_03, 0,
};

const Sint32 *const pl23_extra_tbl[] = {
	pl23_extra_00, pl23_extra_01, pl23_extra_02, 0,
};

const Sint32 *const pl23_special_tbl[] = {
	pl23_special_00, pl23_special_01, pl23_special_01, pl23_special_03, pl23_special_04, pl23_special_04,
	pl23_special_06, pl23_special_07, pl23_special_07, pl23_special_07, pl23_special_10, pl23_special_11,
	pl23_special_12, pl23_special_13, pl23_special_14, pl23_special_15, pl23_special_16, pl23_special_17,
	pl23_special_18, pl23_special_19, pl23_special_20, pl23_special_21, pl23_special_22, pl23_special_23,
	pl23_extra_01, pl23_special_25, pl23_special_25, pl23_special_25, pl23_special_28, pl23_special_28,
	pl23_special_28, pl23_special_31, pl23_special_31, pl23_special_31, pl23_special_34, pl23_special_35,
	pl23_special_36, pl23_special_37, pl23_special_38, pl23_special_38, pl23_special_40, pl23_special_41,
	pl23_special_41, pl23_special_43, pl23_special_44, pl23_special_45, 0,
};

const Sint32 *const pl23_super_tbl[] = {
	pl23_super_00, pl23_super_01, pl23_super_02, pl23_super_03, pl23_super_04, pl23_super_05,
	pl23_super_06, pl23_super_07, 0,
};

const Sint32 *const pl23_effect_tbl[] = {
	pl23_effect_00, pl23_effect_01, pl23_effect_02, pl23_effect_03, pl23_effect_04, pl23_effect_05,
	pl23_effect_06, pl23_effect_07, pl23_effect_08, 0,
};

const Sint32 *const pl23_change_tbl[] = {
	pl23_change_00, pl23_change_01, pl23_change_02, pl23_change_03, pl23_change_04, 0,
};

	/*         prm  type  ctr          end  spr  prio  shadow  atck  flag1          se  zoom  unk  flag2   rival  box */

/* Char_Stand 0: act_stand, appear_move, act_change_gattai_move, act_change_zero_move, ...
   Char_Stand 1
   Char_Stand 4
   Char_Stand 5 */
const Sint32 pl23_stand_00[] = {
	CG_FRAME(    0,    0,   4,           0,   0, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   1, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   2, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   3, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   4, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   5, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   6, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   7, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl23_stand_00 ),
};

/* Char_Stand 2: act_walk (2 + no)
   Char_Stand 6 */
const Sint32 pl23_stand_02[] = {
	CG_FRAME(    0,    0,   3,           0,  19, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   8, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   9, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,  10, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,  11, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,  12, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl23_stand_02 ),
};

/* Char_Stand 3
   Char_Stand 7 */
const Sint32 pl23_stand_03[] = {
	CG_FRAME(    0,    0,   3,           0,  12, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,  11, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,  10, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   9, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   8, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,  19, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl23_stand_03 ),
};

/* Char_Jump 0
   Char_Jump 1: act_wall_jump, act_change_gattai_init, act_change_zero_init, act_change_assist_init, ...
   Char_Jump 2 */
const Sint32 pl23_jump_00[] = {
	CG_FRAME(    0,    1,   4,           0,  19, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   6,           0,  13, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   6,           0,  14, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   6,           0,  15, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   6,           0,  16, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   6,           0,  17, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl23_jump_00_loop[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,  18, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl23_jump_00_loop ),
};

/* Char_Jump 3
   Char_Jump 4
   Char_Jump 5 */
const Sint32 pl23_jump_03[] = {
	CG_FRAME(    0,    0,   6,           0,  21, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   0, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl23_jump_03 ),
};

/* Char_Jump 6
   Char_Jump 7
   Char_Jump 8 */
const Sint32 pl23_jump_06[] = {
	CG_FRAME(    0,    1,   4,           0,  19, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,  13, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,   4,           0,  57, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,   4,           0,  14, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,   4,           0,  15, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,   4,           0,  16, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,   4,           0,  17, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  54 ),
};

/* Char_Jump 9: act_fall, act_air_guard_hold, act_wall_jump
   Char_Jump 10: change_in
   Char_Jump 11: act_change_out_recover */
const Sint32 pl23_jump_09[] = {
	CG_FRAME(    0,    0,   4,           0,  18, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,  58, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_NEXT( pl23_jump_09 ),
};

/* Char_Dash 0: act_dash */
const Sint32 pl23_dash_00[] = {
	CG_FRAME(    0, 0x80,   2,           0,  19, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0, 0x80,   1,           0,  20, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0, 0x80,   1,           0,  21, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
};

const Sint32 pl23_dash_00_loop[] = {
	CG_FRAME(    0,    0,   3,           0,  44, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,  45, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,  46, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,  47, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl23_dash_00_loop ),
};

/* Char_Dash 2: dash_stop */
const Sint32 pl23_dash_02[] = {
	CG_FRAME(    0,    0,   4,           0,  21, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,  20, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
};

const Sint32 pl23_dash_02_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,  19, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl23_dash_02_loop ),
};

/* Char_Dash 1: act_back_dash */
const Sint32 pl23_dash_01[] = {
	CG_FRAME(    0, 0x80,   1,           0,  19, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0, 0x80,   2,           0,  20, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0, 0x80,   1,           0,  21, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
};

const Sint32 pl23_dash_01_loop[] = {
	CG_FRAME(    0,    0,   3,           0,  44, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,  47, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,  46, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,  45, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl23_dash_01_loop ),
};

/* Char_Dash 3: noki_stop */
const Sint32 pl23_dash_03[] = {
	CG_FRAME(    0,    0,   4,           0,  21, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,  20, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
};

const Sint32 pl23_dash_03_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,  19, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl23_dash_03_loop ),
};

/* Char_Crouch 0: act_crouch, act_stand_up */
const Sint32 pl23_crouch_00[] = {
	CG_FRAME(    0,    0,   4,           0,  19, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,  20, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
};

/* Char_Crouch 1: act_crouching, act_escape */
const Sint32 pl23_crouch_01[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,  21, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl23_crouch_01 ),
};

/* Char_Crouch 2: act_stand_up, act_crouch */
const Sint32 pl23_crouch_02[] = {
	CG_FRAME(    0,    0,   4,           0,  21, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,  20, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,  19, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl23_crouch_02 ),
};

/* Char_Turn 0: act_turn */
const Sint32 pl23_turn_00[] = {
	CG_FRAME(    0,    0,   2,           0,  22, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,  23, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,  24, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl23_turn_00 ),
};

/* Char_Turn 1: act_crouch_turn */
const Sint32 pl23_turn_01[] = {
	CG_FRAME(    0,    0,   2,           0,  25, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2,           0,  26, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,  27, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl23_turn_01 ),
};

/* Char_Guard 0 */
const Sint32 pl23_guard_00[] = {
	CG_FRAME(    0,    0,   2,           0,  28, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,  29, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

/* Char_Guard 1: act_guard_hold */
const Sint32 pl23_guard_01[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,  28, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl23_guard_01 ),
};

/* Char_Guard 2 */
const Sint32 pl23_guard_02[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,  28, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl23_guard_02 ),
};

/* Char_Guard 3 */
const Sint32 pl23_guard_03[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,  29, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl23_guard_03 ),
};

/* Char_Guard 4 */
const Sint32 pl23_guard_04[] = {
	CG_FRAME(    0,    0,   2,           0,  30, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2,           0,  31, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
};

/* Char_Guard 5: act_guard_hold */
const Sint32 pl23_guard_05[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,  30, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl23_guard_05 ),
};

/* Char_Guard 6 */
const Sint32 pl23_guard_06[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,  30, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl23_guard_06 ),
};

/* Char_Guard 7 */
const Sint32 pl23_guard_07[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,  31, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl23_guard_07 ),
};

/* Char_Air_Guard 0: act_air_guard */
const Sint32 pl23_air_guard_00[] = {
	CG_FRAME(    0,    0,   4,           0,  32, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Air_Guard 1: act_air_guard_start
   Char_Air_Guard 2: guard_char_set */
const Sint32 pl23_air_guard_01[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,  33, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl23_air_guard_01 ),
};

/* Char_Stand_P 0 (light) */
const Sint32 pl23_stand_p_00[] = {
	CG_FRAME(    0,    0,   3,           0,  68, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,  69, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,  68, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl23_stand_p_00 ),
};

/* Char_Stand_P 1 (medium) */
const Sint32 pl23_stand_p_01[] = {
	CG_FRAME(    0,    0,   4,           0, 120, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   6,           0,  70, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   8,           0,  76, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,  77, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl23_stand_p_01 ),
};

/* Char_Stand_P 2 (heavy) */
const Sint32 pl23_stand_p_02[] = {
	CG_FRAME(    0,    0,   8,           0,  71, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,  72, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,  73, 0x80,   0x00,  ATK,  0x00,       0x03,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,   4,           0,  74, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_FRAME(    0,    0,   4,           0,  75, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_FRAME(    0,    0,   4,           0,  76, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,  77, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl23_stand_p_02 ),
};

/* Char_Stand_K 0 (light) */
const Sint32 pl23_stand_k_00[] = {
	CG_FRAME(    0,    0,   2,           0,  78, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,  79, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,  80, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   4,           0,  79, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,  77, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl23_stand_k_00 ),
};

/* Char_Stand_K 1 (medium) */
const Sint32 pl23_stand_k_01[] = {
	CG_FRAME(    0,    0,   3,           0,  78, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   3,           0,  79, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,  80, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,   8,           0,  81, 0x80,   0x00,  ATK,  0x19,          0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   4,           0,  80, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  44 ),
	CG_FRAME(    0,    0,   4,           0,  79, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  44 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,  77, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl23_stand_k_01 ),
};

/* Char_Stand_K 2 (heavy) */
const Sint32 pl23_stand_k_02[] = {
	CG_FRAME(    0,    0,   2,           0,  78, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   3,           0,  79, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   6,           0,  82, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,  83, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,  84, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   4,           0,  85, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,  86, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,  87, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl23_stand_k_02 ),
};

/* Char_Crouch_P 0 (light) */
const Sint32 pl23_crouch_p_00[] = {
	CG_FRAME(    0,    0,   3,           0,  88, 0x80,   0x40,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,  89, 0x80,   0x40,  ATK,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,  88, 0x80,   0x40,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl23_crouch_p_00 ),
};

/* Char_Crouch_P 1 (medium) */
const Sint32 pl23_crouch_p_01[] = {
	CG_FRAME(    0,    0,   2,           0,  90, 0x80,   0x40,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,  91, 0x80,   0x40,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,  92, 0x80,   0x40,  ATK,  0x00,          0,    0,   0,     0,      0,  15 ),
	CG_FRAME(    0,    0,   8,           0,  91, 0x80,   0x40,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,  90, 0x80,   0x40,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl23_crouch_p_01 ),
};

/* Char_Crouch_P 2 (heavy) */
const Sint32 pl23_crouch_p_02[] = {
	CG_FRAME(    0,    0,   4,           0,  93, 0x80,   0x40,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,  94, 0x80,   0x40,  ATK,  0x00,       0x03,    0,   0,     0,      0,  16 ),
	CG_FRAME(    0,    0,   8,           0,  95, 0x80,   0x40,  ATK,  0x00,          0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,  12, END_OF_DATA,  94, 0x80,   0x40,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl23_crouch_p_02 ),
};

/* Char_Extra 2: pl23_upper, fly_start5 */
const Sint32 pl23_extra_02[] = {
	CG_FRAME(    0,    0,   4,           0,  93, 0x80,   0x40,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,  94, 0x80,   0x40,  ATK,  0x00,       0x03,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   8,           0,  95, 0x80,   0x40,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,  12, END_OF_DATA,  94, 0x80,   0x40,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl23_extra_02 ),
};

/* Char_Crouch_K 0 (light) */
const Sint32 pl23_crouch_k_00[] = {
	CG_FRAME(    0,    0,   5,           0,  96, 0x80,   0x40,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   0,           0,  97, 0x80,   0x40,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   6,           0,  98, 0x80,   0x40,  ATK,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,   2,           0,  97, 0x80,   0x40,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,  96, 0x80,   0x40,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl23_crouch_k_00 ),
};

/* Char_Crouch_K 1 (medium) */
const Sint32 pl23_crouch_k_01[] = {
	CG_FRAME(    0,    0,   5,           0,  96, 0x80,   0x40,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   0,           0,  97, 0x80,   0x40,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   6,           0,  98, 0x80,   0x40,  ATK,  0x00,          0,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    0,   2,           0,  97, 0x80,   0x40,  ATE,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,  96, 0x80,   0x40,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl23_crouch_k_01 ),
};

/* Char_Crouch_K 2 (heavy) */
const Sint32 pl23_crouch_k_02[] = {
	CG_FRAME(    0,    0,   6,           0,  99, 0x80,   0x40,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   6,           0, 100, 0x80,   0x40,  ATK,  0x00,          0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,   6,           0, 101, 0x80,   0x40,  ATE,  0x00,          0,    0,   0,     0,      0,  47 ),
	CG_FRAME(    0,    0,  10,           0, 102, 0x80,   0x40,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,  10, END_OF_DATA,  25, 0x80,   0x40,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl23_crouch_k_02 ),
};

/* Char_Jump_P 0 (light)
   Char_Jump_P 3 */
const Sint32 pl23_jump_p_00[] = {
	CG_FRAME(    0,    0,   5,           0, 103, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,  10,           0, 104, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,  10, END_OF_DATA, 103, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl23_jump_p_00 ),
};

/* Char_Jump_P 1 (medium)
   Char_Jump_P 4 */
const Sint32 pl23_jump_p_01[] = {
	CG_FRAME(    0,    0,   3,           0, 103, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0, 106, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   8,           0, 105, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  22 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 103, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl23_jump_p_01 ),
};

/* Char_Jump_P 2 (heavy)
   Char_Jump_P 5 */
const Sint32 pl23_jump_p_02[] = {
	CG_FRAME(    0,    0,   4,           0, 103, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0, 106, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   6,           0, 107, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   6,           0, 108, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  23 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA, 103, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl23_jump_p_02 ),
};

/* Char_Jump_K 0 (light)
   Char_Jump_K 3 */
const Sint32 pl23_jump_k_00[] = {
	CG_FRAME(    0,    0,   4,           0, 110, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   5,           0, 111, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   7,           0, 109, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    0,   5,           0, 111, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA, 126, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl23_jump_k_00 ),
};

/* Char_Jump_K 1 (medium)
   Char_Jump_K 4 */
const Sint32 pl23_jump_k_01[] = {
	CG_FRAME(    0,    0,   4,           0, 110, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0, 111, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,  10,           0, 112, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA, 111, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl23_jump_k_01 ),
};

/* Char_Jump_K 2 (heavy)
   Char_Jump_K 5 */
const Sint32 pl23_jump_k_02[] = {
	CG_FRAME(    0,    0,   4,           0, 110, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0, 113, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   8,           0, 114, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,   6,           0, 113, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA, 110, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl23_jump_k_02 ),
};

/* Char_Get_Up 0: appear_drop_down, dm_dizzy_wait */
const Sint32 pl23_get_up_00[] = {
	CG_FRAME(    0,    0,   8,           0,  42, 0x80,   0x60,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,  43, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,  20, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,  19, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl23_get_up_00 ),
};

/* Char_Get_Up 1 */
const Sint32 pl23_get_up_01[] = {
	CG_FRAME(    0,    0,   1,           0,  44, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,  45, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,  46, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,  47, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,  44, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,  45, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,  46, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,  47, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,  21, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,  20, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,  19, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl23_get_up_01 ),
};

/* Char_Get_Up 2 */
const Sint32 pl23_get_up_02[] = {
	CG_FRAME(    0,    0,   2,           0,  44, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  45, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  46, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,  47, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,  44, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,  45, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,  46, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,  47, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,  44, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,  45, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,  46, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,  47, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,  21, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,  20, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,  19, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl23_get_up_02 ),
};

/* Char_Get_Up 3 */
const Sint32 pl23_get_up_03[] = {
	CG_FRAME(    0,    0,   3,           0,  47, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,  46, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,  45, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,  44, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,  47, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,  46, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,  45, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,  44, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  21, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  20, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,  19, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl23_get_up_03 ),
};

/* Char_Damage 0: dm_freeze, dm_freeze_air, caught_dm */
const Sint32 pl23_damage_00[] = {
	CG_FRAME(    0,    0,  11, END_OF_DATA, 161, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl23_damage_00 ),
};

/* Char_Damage 1 */
const Sint32 pl23_damage_01[] = {
	CG_FRAME(    0,    0,   4,           0, 161, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  10,           0, 162, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA, 161, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl23_damage_01 ),
};

/* Char_Damage 2 */
const Sint32 pl23_damage_02[] = {
	CG_FRAME(    0,    0,   2,           0, 161, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0, 162, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0, 163, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0, 164, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0, 163, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0, 162, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 161, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl23_damage_02 ),
};

/* Char_Damage 3 */
const Sint32 pl23_damage_03[] = {
	CG_FRAME(    0,    0,  11, END_OF_DATA, 169, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl23_damage_03 ),
};

/* Char_Damage 4 */
const Sint32 pl23_damage_04[] = {
	CG_FRAME( 0x80,    0,   2,           0, 169, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME( 0x82,    0,  10,           0, 170, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA, 169, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl23_damage_04 ),
};

/* Char_Damage 5 */
const Sint32 pl23_damage_05[] = {
	CG_FRAME( 0x80,    0,   2,           0, 169, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME( 0x80,    0,   2,           0, 170, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME( 0x80,    0,   4,           0, 171, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0, 172, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0, 171, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0, 170, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 169, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl23_damage_05 ),
};

/* Char_Damage 6 */
const Sint32 pl23_damage_06[] = {
	CG_FRAME(    0,    0,  11, END_OF_DATA, 177, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl23_damage_06 ),
};

/* Char_Damage 7 */
const Sint32 pl23_damage_07[] = {
	CG_FRAME(    0,    0,   2,           0, 177, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,  10,           0, 178, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA, 177, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl23_damage_07 ),
};

/* Char_Damage 8 */
const Sint32 pl23_damage_08[] = {
	CG_FRAME(    0,    0,   2,           0, 177, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2,           0, 178, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,  10,           0, 179, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   6,           0, 178, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA, 177, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl23_damage_08 ),
};

/* Char_Damage 10: dm_ko_blow */
const Sint32 pl23_damage_10[] = {
	CG_FRAME(    0,    0,  10, END_OF_DATA, 180, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl23_damage_10 ),
};

/* Char_Damage 11 */
const Sint32 pl23_damage_11[] = {
	CG_FRAME(    0,    0,   4,           0, 180, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0, 181, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA, 180, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl23_damage_11 ),
};

/* Char_Damage 12 */
const Sint32 pl23_damage_12[] = {
	CG_FRAME(    0,    0,   1,           0, 180, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   1,           0, 181, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   1,           0, 182, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0, 183, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   1,           0, 182, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   1,           0, 181, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA, 180, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl23_damage_12 ),
};

/* Char_Damage 13: dm_snap_fly, dm_air
   Char_Damage 14
   Char_Damage 15
   Char_Damage 34: appear_drop_fall, dm_snap_fly, dm_snap_down */
const Sint32 pl23_damage_13[] = {
	CG_FRAME(    0,    0,   4,           0, 184, 0x80,   0x6e,    0,  0x00,          0,    0,   0,     0,      0,  43 ),
};

const Sint32 pl23_damage_13_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA, 185, 0x80,   0x6e,    0,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_NEXT( pl23_damage_13_loop ),
};

/* Char_Damage 9: dm_sweep */
const Sint32 pl23_damage_09[] = {
	CG_FRAME(    0,    0,   5,           0, 186, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0, 187, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0, 188, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0, 196, 0x80,   0x6e,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0, 197, 0x80,   0x6e,    0,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,   3,           0, 198, 0x80,   0x6e,    0,  0x00,          0,    0,   0,     0,      0,  43 ),
};

const Sint32 pl23_damage_09_loop[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA, 199, 0x80,   0x6e,    0,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_NEXT( pl23_damage_09_loop ),
};

/* Char_Damage 16: dm_blow_char_set (16 + no), dm_spark, caught_shock_init */
const Sint32 pl23_damage_16[] = {
	CG_FRAME(    0,    0,   4,           0, 189, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA, 190, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl23_damage_16 ),
};

/* Char_Damage 17: dm_07_1 */
const Sint32 pl23_damage_17[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA, 191, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl23_damage_17 ),
};

/* Char_Damage 18 */
const Sint32 pl23_damage_18[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA, 192, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl23_damage_18 ),
};

/* Char_Damage 19 */
const Sint32 pl23_damage_19[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA, 193, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl23_damage_19 ),
};

/* Char_Damage 20 */
const Sint32 pl23_damage_20[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA, 194, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl23_damage_20 ),
};

/* Char_Damage 21: dm_fall_char_set (21 + no) */
const Sint32 pl23_damage_21[] = {
	CG_FRAME(    0,    0,   2,           0, 189, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0, 190, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 22 */
const Sint32 pl23_damage_22[] = {
	CG_FRAME(    0,    0,   2,           0, 191, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 23 */
const Sint32 pl23_damage_23[] = {
	CG_FRAME(    0,    0,   2,           0, 192, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 24 */
const Sint32 pl23_damage_24[] = {
	CG_FRAME(    0,    0,   2,           0, 193, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 25 */
const Sint32 pl23_damage_25[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA, 194, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl23_damage_25 ),
};

/* Char_Damage 26: dm_buttobi_fly, dm_07_1, dm_away_fly, dm_bound_hop, ... */
const Sint32 pl23_damage_26[] = {
	CG_FRAME(    0,    0,   2,           0, 188, 0x80,   0x15,    0,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,   2,           0, 196, 0x80,   0x8c,    0,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,   2,           0, 197, 0x80,   0x8c,    0,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,   2,           0, 198, 0x80,   0x8c,    0,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA, 199, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_NEXT( pl23_damage_26 ),
};

/* Char_Damage 27
   Char_Damage 30: act_air_recover, act_change_out_init, caught_blow_fly, caught_blow_fix_fly, ...
   Char_Damage 32: act_escape */
const Sint32 pl23_damage_27[] = {
	CG_FRAME(    0,    0,   4,           0, 201, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0, 202, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0, 203, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,  16, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,  17, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,  18, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl23_damage_27 ),
};

/* Char_Damage 31: appear_drop_land, act_down, dm_dizzy_down */
const Sint32 pl23_damage_31[] = {
	CG_FRAME(    0,    0,  16, END_OF_DATA, 200, 0x80,   0x8c,    0,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_NEXT( pl23_damage_31 ),
};

/* Char_Damage 33: act_escape */
const Sint32 pl23_damage_33[] = {
	CG_FRAME(    0,    0,   4,           0, 219, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0, 220, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,  14, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,  19, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl23_damage_33 ),
};

/* Char_Damage 28: dm_away_init, dm_bound_fly, dm_piyo, dm_away */
const Sint32 pl23_damage_28[] = {
	CG_FRAME(    0,    0,   4,           0, 204, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0, 205, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0, 206, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0, 207, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0, 208, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA, 209, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl23_damage_28 ),
};

/* Char_Damage 29: dm_away_fly, dm_bound_hop, dm_bound, caught_spin_fly, ... */
const Sint32 pl23_damage_29[] = {
	CG_FRAME(    0,    0,   4,           0, 210, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0, 211, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0, 212, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0, 213, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0, 214, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA, 215, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl23_damage_29 ),
};

/* Char_Caught 0 */
const Sint32 pl23_caught_00[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA, 162, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  37 ),
	CG_NEXT( pl23_caught_00 ),
};

/* Char_Caught 1 */
const Sint32 pl23_caught_01[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA, 163, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  37 ),
	CG_NEXT( pl23_caught_01 ),
};

/* Char_Caught 2 */
const Sint32 pl23_caught_02[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA, 164, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  37 ),
	CG_NEXT( pl23_caught_02 ),
};

/* Char_Caught 3 */
const Sint32 pl23_caught_03[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA, 170, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  38 ),
	CG_NEXT( pl23_caught_03 ),
};

/* Char_Caught 4 */
const Sint32 pl23_caught_04[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA, 171, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  38 ),
	CG_NEXT( pl23_caught_04 ),
};

/* Char_Caught 5 */
const Sint32 pl23_caught_05[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA, 172, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  38 ),
	CG_NEXT( pl23_caught_05 ),
};

/* Char_Caught 6 */
const Sint32 pl23_caught_06[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA, 177, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  39 ),
	CG_NEXT( pl23_caught_06 ),
};

/* Char_Caught 7 */
const Sint32 pl23_caught_07[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA, 178, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  39 ),
	CG_NEXT( pl23_caught_07 ),
};

/* Char_Caught 8 */
const Sint32 pl23_caught_08[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA, 179, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  39 ),
	CG_NEXT( pl23_caught_08 ),
};

/* Char_Caught 9 */
const Sint32 pl23_caught_09[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA, 181, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  38 ),
	CG_NEXT( pl23_caught_00 ),
};

/* Char_Caught 10 */
const Sint32 pl23_caught_10[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA, 182, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  38 ),
	CG_NEXT( pl23_caught_10 ),
};

/* Char_Caught 11 */
const Sint32 pl23_caught_11[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA, 183, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  38 ),
	CG_NEXT( pl23_caught_11 ),
};

/* Char_Caught 12 */
const Sint32 pl23_caught_12[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA, 184, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  40 ),
	CG_NEXT( pl23_caught_12 ),
};

/* Char_Caught 13 */
const Sint32 pl23_caught_13[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA, 189, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_NEXT( pl23_caught_13 ),
};

/* Char_Caught 14 */
const Sint32 pl23_caught_14[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA, 191, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_NEXT( pl23_caught_14 ),
};

/* Char_Caught 15 */
const Sint32 pl23_caught_15[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA, 192, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_NEXT( pl23_caught_15 ),
};

/* Char_Caught 16 */
const Sint32 pl23_caught_16[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA, 193, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_NEXT( pl23_caught_16 ),
};

/* Char_Caught 17 */
const Sint32 pl23_caught_17[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA, 194, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_NEXT( pl23_caught_17 ),
};

/* Char_Caught 18 */
const Sint32 pl23_caught_18[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA, 204, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_NEXT( pl23_caught_18 ),
};

/* Char_Caught 19 */
const Sint32 pl23_caught_19[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA, 210, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_NEXT( pl23_caught_19 ),
};

/* Char_Caught 20 */
const Sint32 pl23_caught_20[] = {
	CG_FRAME(    0,    0,  16, END_OF_DATA, 199, 0x80,   0x8c,    0,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_NEXT( pl23_caught_20 ),
};

/* Char_Caught 21 */
const Sint32 pl23_caught_21[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA, 186, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  38 ),
	CG_NEXT( pl23_caught_21 ),
};

/* Char_Dizzy 0: act_dizzy */
const Sint32 pl23_dizzy_00[] = {
	CG_FRAME(    0,    0,   6,           0, 216, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0, 217, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0, 218, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA, 217, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl23_dizzy_00 ),
};

/* Char_Win 0 */
const Sint32 pl23_win_00[] = {
	CG_FRAME(    0,    0,   6,           0,  34, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,  35, 0x80,   0x00,    0,  0x00,       0x1b,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   6,           0,  37, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl23_win_00_loop[] = {
	CG_FRAME(    0,    0,   6,           0,  36, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,  37, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl23_win_00_loop ),
};

/* Char_Win 1 */
const Sint32 pl23_win_01[] = {
	CG_FRAME(    0,    0,   4,           0,  38, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,  39, 0x80,   0x00,    0,  0x00,       0x0f,    0,   0,     0,      0,   1 ),
};

const Sint32 pl23_win_01_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,  40, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl23_win_01_loop ),
};

/* Char_Win 2: win_time_init */
const Sint32 pl23_win_02[] = {
	CG_FRAME(    0,    0,   6,           0,  59, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,  60, 0x80,   0x00,    0,  0x00,       0x1b,    0,   0,     0,      0,   1 ),
};

const Sint32 pl23_win_02_loop[] = {
	CG_FRAME(    0,    0,   6,           0,  61, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,  62, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl23_win_02_loop ),
};

/* Char_Win 3: lose_init, lose_ko_init */
const Sint32 pl23_win_03[] = {
	CG_FRAME(    0,    0,   6,           0,  19, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,  20, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,  21, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl23_win_03_loop[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA,  41, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl23_win_03_loop ),
};

/* Char_Appear 0: appear_walk_init */
const Sint32 pl23_appear_00[] = {
	CG_FRAME(    0,    0,   4,           0,  44, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,  45, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,  46, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,  47, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,  44, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,  45, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,  46, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,  47, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   4,           0,  21, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,  20, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,  19, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl23_appear_00_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   0, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl23_appear_00_loop ),
};

/* Char_Appear 1: appear_walk_stop */
const Sint32 pl23_appear_01[] = {
	CG_FRAME(    0,    0,   6,           0,  59, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,  60, 0x80,   0x00,    0,  0x00,       0x0a,    0,   0,     0,      0,   1 ),
};

const Sint32 pl23_appear_01_loop[] = {
	CG_FRAME(    0,    0,   6,           0,  61, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,  62, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl23_appear_01_loop ),
};

const Sint32 pl23_unused_00[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,   4,           0,  19, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,  20, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,  21, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,  44, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,  45, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,  46, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,  47, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,  90, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,  91, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0, 137, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA, 138, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	0x000023a8,
};

const Sint32 pl23_unused_01[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,   4,           0,  19, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,  20, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,  21, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,  47, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,  46, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,  45, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,  44, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,  90, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,  91, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0, 137, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA, 138, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	0x00002488,
};

const Sint32 pl23_unused_02[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,   4,           0, 110, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0, 139, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA, 140, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	0x000024c8,
};

const Sint32 pl23_unused_03[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,   4,           0,  19, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,  20, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,  21, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0, 137, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA, 138, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	0x00002530,
};

/* Char_Extra 0: t_kick_init */
const Sint32 pl23_extra_00[] = {
	CG_FRAME(    0,    0,   2,           0, 110, 0xa0,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0, 111, 0xa0,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
};

const Sint32 pl23_extra_00_loop[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA, 112, 0xa0,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_NEXT( pl23_extra_00_loop ),
};

/* Char_Change 3: act_stand, act_change_call
   group 25 3 */
const Sint32 pl23_change_03[] = {
	CG_FRAME(    0,    0,   4,           0,  63, 0x30,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,  64, 0x30,   0x00,    0,  0x00,       0x1b,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   2,           0,  64, 0x30,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl23_change_03_loop[] = {
	CG_FRAME(    0,    0,   4,           0,  65, 0x30,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,  66, 0x30,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl23_change_03_loop ),
};

/* Char_Change 1: act_change_vcounter_init
   group 25 1 */
const Sint32 pl23_change_01[] = {
	CG_FRAME(    0,    0,   4,           0,  78, 0x30,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl23_change_01_loop[] = {
	CG_FRAME(    0,    1,   8, END_OF_DATA,  77, 0x30,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl23_change_01_loop ),
};

/* Char_Change 2: act_change_out_bound
   group 25 2 */
const Sint32 pl23_change_02[] = {
	CG_FRAME(    0,    0,   4,           0,  18, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,  58, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl23_change_02 ),
};

/* Char_Change 4: act_change_assist_end
   group 25 4 */
const Sint32 pl23_change_04[] = {
	CG_FRAME(    0,    0,   4,           0,  59, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,  60, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,  61, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,  62, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,  61, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,  62, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,  61, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,  62, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,  61, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl23_change_04_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,  62, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl23_change_04_loop ),
};

/* Char_Catch 0: nml_p_set */
const Sint32 pl23_catch_00[] = {
	CG_FRAME(    0,    0,   1,           0,  78, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0001,   0 ),
	CG_FRAME(    0,    0,   8,           0, 115, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0002,   0 ),
	CG_FRAME(    0,    0,   4,           0, 116, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0003,   0 ),
	CG_FRAME(    0,    0,   2,           0, 117, 0x80,   0x00,    0,  0x00,       0x03,    0,   0,     0, 0x0004,   0 ),
	CG_FRAME(    0,    1,  38, END_OF_DATA, 118, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl23_catch_00 ),
};

/* Char_Catch 1: jump_p_set */
const Sint32 pl23_catch_01[] = {
	CG_FRAME(    0,    0,   1,           0, 132, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0005,   0 ),
	CG_FRAME(    0,    0,   6,           0, 133, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0006,   0 ),
	CG_FRAME(    0,    0,   4,           0, 134, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0007,   0 ),
	CG_FRAME(    0, 0x80,   2,           0, 135, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0008,   0 ),
	CG_FRAME(    0,    0,   4,           0, 118, 0x80,   0x00,    0,  0x00,       0x03,    0,   0,     0, 0x0009,   0 ),
	CG_FRAME(    0,    1,  16, END_OF_DATA, 118, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl23_catch_01 ),
};

/* Char_Catch 2: nml_k_set */
const Sint32 pl23_catch_02[] = {
	CG_FRAME(    0,    0,   2,           0,  69, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x000a,   0 ),
	CG_FRAME(    0,    0,   2,           0,  68, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x000b,   0 ),
	CG_FRAME(    0,    0,   2,           0,  19, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x000c,   0 ),
	CG_FRAME(    0,    0,   2,           0,  20, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x000d,   0 ),
	CG_FRAME(    0,    0,   3,           0,  21, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x000e,   0 ),
	CG_FRAME(    0,    0,   3,           0,  93, 0x80,   0x00,    0,  0x00,       0x17,    0,   0,     0, 0x000f,   0 ),
	CG_FRAME(    0,    1,   4,           0,  94, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0010,   0 ),
	CG_FRAME(    0,    0,   4,           0,  95, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0011,   0 ),
	CG_FRAME(    0,    0,   4,           0,  94, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,  77, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl23_catch_02 ),
};

/* Char_Catch 3: otoko_reach (Otoko Michi (level 3 hyper)), otoko_set */
const Sint32 pl23_catch_03[] = {
	CG_FRAME(    0,    0,  16,           0,  74, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0012,   1 ),
	CG_FRAME(    0,    0,   6,           0,  75, 0x80,   0x00,    0,  0x00,       0x10,    0,   0,     0, 0x0013,   1 ),
	CG_FRAME(    0,    0,   6,           0,  70, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0014,   1 ),
	CG_FRAME(    0,    1,  72,           0, 216, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0015,   1 ),
	CG_FRAME(    0,    2,   8,           0, 216, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0015,   1 ),
	CG_FRAME(    0,    4,   8, END_OF_DATA, 216, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0015,   1 ),
	CG_NEXT( pl23_catch_03 ),
};

/* Char_Special 0: gadou_init (Gadouken) */
const Sint32 pl23_special_00[] = {
	CG_FRAME(    0,    0,   8,           0,  71, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,  76, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    1,  34, END_OF_DATA, 119, 0x80,   0x00,  ATK,  0x00,       0x18,    0,   0,     0,      0,  27 ),
	CG_NEXT( pl23_special_00 ),
};

/* Char_Special 1
   Char_Special 2 */
const Sint32 pl23_special_01[] = {
	CG_FRAME(    0,    0,   8,           0,  71, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,  76, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    1,  38, END_OF_DATA, 119, 0x80,   0x00,  ATK,  0x00,       0x18,    0,   0,     0,      0,  27 ),
	CG_NEXT( pl23_special_01 ),
};

/* Char_Special 10 */
const Sint32 pl23_special_10[] = {
	CG_FRAME(    0,    0,   2,           0, 221, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  35 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 222, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  35 ),
	CG_NEXT( pl23_special_10 ),
};

/* Char_Special 11 */
const Sint32 pl23_special_11[] = {
	CG_FRAME(    0,    0,   2,           0, 223, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  35 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 224, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  35 ),
	CG_NEXT( pl23_special_11 ),
};

/* Char_Special 12 */
const Sint32 pl23_special_12[] = {
	CG_FRAME(    0,    0,   2,           0, 225, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  35 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 226, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  35 ),
	CG_NEXT( pl23_special_12 ),
};

/* Char_Special 13 */
const Sint32 pl23_special_13[] = {
	CG_FRAME(    0,    0,   3,           0, 242, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0, 243, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0, 244, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0, 245, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0, 246, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA, 247, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl23_special_13 ),
};

/* Char_Effect 3: shl23_00_gadou_init, shl23_00_shinkuu_init */
const Sint32 pl23_effect_03[] = {
	CG_FRAME(    0,    0,   3,           0, 221, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
};

const Sint32 pl23_effect_03_loop[] = {
	CG_FRAME(    0,    0,   3,           0, 222, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  35 ),
	CG_FRAME(    0,    0,   3,           0, 223, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  35 ),
	CG_FRAME(    0,    0,   3,           0, 224, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  35 ),
	CG_FRAME(    0,    0,   3,           0, 225, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  35 ),
	CG_FRAME(    0,    0,   3,           0, 226, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  35 ),
	CG_FRAME(    0,    0,   3,           0, 227, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  35 ),
	CG_FRAME(    0,    0,   3,           0, 228, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  35 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA, 221, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  35 ),
	CG_NEXT( pl23_effect_03_loop ),
};

/* Char_Effect 4: shl23_00_gadou_move, shl23_00_shinkuu_move */
const Sint32 pl23_effect_04[] = {
	CG_FRAME(    0,    0,   3,           0, 229, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0, 230, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0, 231, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0, 232, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0, 233, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0, 234, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA, 235, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl23_effect_04 ),
};

/* Char_Effect 5: shl23_00_gadou_move */
const Sint32 pl23_effect_05[] = {
	CG_FRAME(    0,    0,   3,           0, 236, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0, 237, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0, 238, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0, 239, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0, 240, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA, 241, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl23_effect_05 ),
};

/* Char_Special 25: sign_init (Premium Sign)
   Char_Special 26
   Char_Special 27: sign_init (Premium Sign) */
const Sint32 pl23_special_25[] = {
	CG_FRAME(    0,    0,   3,           0, 146, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   3,           0, 147, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
};

const Sint32 pl23_special_25_loop[] = {
	CG_FRAME(    0,    0,   3,           0, 148, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA, 149, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl23_special_25_loop ),
};

/* Char_Special 28: sign_keep (Premium Sign)
   Char_Special 29
   Char_Special 30: sign_keep (Premium Sign) */
const Sint32 pl23_special_28[] = {
	CG_FRAME(    0,    0,   3,           0, 150, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,  32,           0, 151, 0x80,   0x00,  ATK,  0x00,       0x1b,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,   6,           0, 150, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   6,           0, 152, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,  16,           0, 153, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   7,           0, 154, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   1,           0, 154, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0, 0x80,  16,           0, 155, 0x80,   0x00,  ATK,  0x00,       0x1c,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   8,           0, 156, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,  77, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl23_special_28 ),
};

/* Char_Special 31: sign_keep (Premium Sign)
   Char_Special 32
   Char_Special 33: sign_keep (Premium Sign) */
const Sint32 pl23_special_31[] = {
	CG_FRAME(    0,    0,   3,           0, 150, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    1,  32,           0, 157, 0x80,   0x00,  ATK,  0x00,       0x1b,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,   6,           0, 150, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   6,           0, 152, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,  16,           0, 153, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   7,           0, 154, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   1,           0, 154, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0, 0x80,  16,           0, 155, 0x80,   0x00,  ATK,  0x00,       0x1c,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   8,           0, 156, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,  77, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl23_special_31 ),
};

/* Char_Effect 6: shl23_00_sign_init */
const Sint32 pl23_effect_06[] = {
	CG_FRAME(    0,    0,   3,           0, 263, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   3,           0, 264, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   3,           0, 265, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA, 266, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_NEXT( pl23_effect_06 ),
};

/* Char_Effect 7 */
const Sint32 pl23_effect_07[] = {
	CG_FRAME(    0,    0,   2,           0, 263, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   2,           0, 264, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   2,           0, 265, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 266, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_NEXT( pl23_effect_07 ),
};

/* Char_Effect 8: shl23_00_sign_init */
const Sint32 pl23_effect_08[] = {
	CG_FRAME(    0,    0,   1,           0, 263, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   1,           0, 264, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   1,           0, 265, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA, 266, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_NEXT( pl23_effect_08 ),
};

/* Char_Special 3 */
const Sint32 pl23_special_03[] = {
	CG_FRAME(    0, 0x80,   1,           0, 120, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  28 ),
	CG_FRAME(    0, 0x80,   1,           0, 120, 0x80,   0x00,  ATK,  0x00, 0x16 | CSE,    0,   0,     0,      0,  28 ),
	CG_FRAME(    0, 0x80,   2,           0, 120, 0x80,   0x00,  ATK,  0x00,       0x19,    0,   0,     0,      0,  28 ),
	CG_FRAME(   51, 0x80,   4,           0, 121, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  29 ),
	CG_FRAME(   52,    0,  10,           0, 122, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  30 ),
	CG_FRAME(    0,    0,   6,           0, 123, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0, 0x80,   2,           0, 124, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
};

const Sint32 pl23_special_03_loop[] = {
	CG_FRAME(    0, 0x80,   6, END_OF_DATA, 125, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl23_special_03_loop ),
};

/* Char_Special 4
   Char_Special 5 */
const Sint32 pl23_special_04[] = {
	CG_FRAME(    0, 0x80,   3,           0, 120, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  28 ),
	CG_FRAME(    0, 0x80,   1,           0, 120, 0x80,   0x00,  ATK,  0x00, 0x16 | CSE,    0,   0,     0,      0,  28 ),
	CG_FRAME(   55, 0x80,   4,           0, 121, 0x80,   0x00,  ATK,  0x00,       0x19,    0,   0,     0,      0,  29 ),
	CG_FRAME(   56,    0,  18,           0, 122, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  30 ),
	CG_FRAME(    0,    0,   6,           0, 123, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0, 0x80,   2,           0, 124, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0, 0x80,   6, END_OF_DATA, 125, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl23_special_04 ),
};

/* Char_Special 37 */
const Sint32 pl23_special_37[] = {
	CG_FRAME(    0, 0x80,   3,           0, 120, 0x80,   0x00,  ATK,  0x00, 0x16 | CSE,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0, 0x80,   2,           0, 120, 0x80,   0x00,  ATK,  0x00,       0x19,    0,   0,     0,      0,  55 ),
	CG_FRAME(   51, 0x80,   4,           0, 121, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(   52,    0,  10,           0, 122, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,   6,           0, 123, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0, 0x80,   2,           0, 124, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0, 0x80,   6, END_OF_DATA, 125, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl23_special_37 ),
};

/* Char_Special 38
   Char_Special 39 */
const Sint32 pl23_special_38[] = {
	CG_FRAME(    0, 0x80,   3,           0, 120, 0x80,   0x00,  ATK,  0x00, 0x16 | CSE,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0, 0x80,   3,           0, 120, 0x80,   0x00,  ATK,  0x00,       0x19,    0,   0,     0,      0,  55 ),
	CG_FRAME(   55, 0x80,   4,           0, 121, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(   56,    0,  18,           0, 122, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,   6,           0, 123, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0, 0x80,   2,           0, 124, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0, 0x80,   6, END_OF_DATA, 125, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl23_special_38 ),
};

/* Char_Special 6 */
const Sint32 pl23_special_06[] = {
	CG_FRAME(    0, 0x80,   4,           0, 120, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0, 126, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  31 ),
	CG_FRAME(    0,    0,   4,           0, 127, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  31 ),
	CG_FRAME(    0,    0,   8,           0, 128, 0x80,   0x20,  ATK,  0x39,       0x16,    0,   0,     0,      0,  32 ),
};

const Sint32 pl23_special_06_loop[] = {
	CG_FRAME(    0, 0x80,  48, END_OF_DATA, 126, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  31 ),
	CG_NEXT( pl23_special_06_loop ),
};

/* Char_Special 44: jdanku_init (Dankuukyaku in the air) */
const Sint32 pl23_special_44[] = {
	CG_FRAME(    0,    0,   4,           0, 126, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  31 ),
	CG_FRAME(    0,    0,   4,           0, 127, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  31 ),
	CG_FRAME(    0,    0,   8,           0, 128, 0x80,   0x20,  ATK,  0x39,       0x16,    0,   0,     0,      0,  32 ),
};

const Sint32 pl23_special_44_loop[] = {
	CG_FRAME(    0, 0x80,  10, END_OF_DATA, 126, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  31 ),
	CG_NEXT( pl23_special_44_loop ),
};

/* Char_Special 7
   Char_Special 8
   Char_Special 9 */
const Sint32 pl23_special_07[] = {
	CG_FRAME(    0, 0x80,   4,           0, 120, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  31 ),
	CG_FRAME(    0,    0,   2,           0, 126, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  31 ),
	CG_FRAME(    0,    0,   2,           0, 127, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  31 ),
	CG_FRAME(    0,    0,   8,           0, 128, 0x80,   0x20,  ATK,  0x3c,       0x16,    0,   0,     0,      0,  32 ),
	CG_FRAME(    0,    0,   2,           0, 126, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  31 ),
	CG_FRAME(    0,    2,   2,           0, 129, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  31 ),
	CG_FRAME(    0,    0,   8,           0, 130, 0x80,   0x20,  ATK,  0x3d,       0x16,    0,   0,     0,      0,  33 ),
	CG_FRAME(    0,    0,   2,           0, 129, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  31 ),
	CG_FRAME(    0,    0,   2,           0, 126, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  31 ),
	CG_FRAME(    0,    4,   2,           0, 127, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  31 ),
	CG_FRAME(    0,    0,   8,           0, 131, 0x80,   0x20,  ATK,  0x3e,       0x17,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,   4,           0, 127, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  31 ),
};

const Sint32 pl23_special_07_loop[] = {
	CG_FRAME(    0, 0x80,  48, END_OF_DATA, 126, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  31 ),
	CG_NEXT( pl23_special_07_loop ),
};

/* Char_Special 45: jdanku_init (Dankuukyaku in the air) */
const Sint32 pl23_special_45[] = {
	CG_FRAME(    0,    0,   2,           0, 126, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  31 ),
	CG_FRAME(    0,    0,   2,           0, 127, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  31 ),
	CG_FRAME(    0,    0,   8,           0, 128, 0x80,   0x20,  ATK,  0x3a,       0x16,    0,   0,     0,      0,  32 ),
	CG_FRAME(    0,    0,   2,           0, 126, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  31 ),
	CG_FRAME(    0,    2,   2,           0, 129, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  31 ),
	CG_FRAME(    0,    0,   8,           0, 130, 0x80,   0x20,  ATK,  0x3b,       0x16,    0,   0,     0,      0,  33 ),
	CG_FRAME(    0,    0,   2,           0, 129, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  31 ),
	CG_FRAME(    0,    0,   2,           0, 126, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  31 ),
	CG_FRAME(    0,    4,   2,           0, 127, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  31 ),
	CG_FRAME(    0,    0,   8,           0, 131, 0x80,   0x20,  ATK,  0x56,       0x17,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,   4,           0, 127, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  31 ),
};

const Sint32 pl23_special_45_loop[] = {
	CG_FRAME(    0, 0x80,  16, END_OF_DATA, 126, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  31 ),
	CG_NEXT( pl23_special_45_loop ),
};

/* Char_Special 40 */
const Sint32 pl23_special_40[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA,  21, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl23_special_40 ),
};

/* Char_Special 41
   Char_Special 42 */
const Sint32 pl23_special_41[] = {
	CG_FRAME(    0,    0,  16, END_OF_DATA,  21, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl23_special_41 ),
};

/* Char_Special 43: snapback_init (Snapback) */
const Sint32 pl23_special_43[] = {
	CG_FRAME(    0,    0,   2,           0,  78, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,  79, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    1,   4,           0,  82, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   2,           0,  83, 0x80,   0x00,  ATK,  0x00, 0x16 | CSE,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   3,           0,  84, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   4,           0,  85, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,  86, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,  87, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl23_special_43 ),
};

/* Char_Super 4: jburaiken_init (Hisshou Buraiken in the air (hyper, unused)) */
const Sint32 pl23_super_04[] = {
	CG_FRAME(    0,    0,   4,           0, 110, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    1,   2,           0, 139, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
};

const Sint32 pl23_super_04_loop[] = {
	CG_FRAME(    0,    0,   2,           0, 126, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0, 129, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   6,           0, 130, 0x80,   0x20,  ATK,  0x4d,          0,    0,   0,     0,      0,  33 ),
	CG_FRAME(    0,    0,   2,           0, 129, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0, 126, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0, 127, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   6,           0, 131, 0x80,   0x20,  ATK,  0x4d,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 127, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl23_super_04_loop ),
};

/* Char_Super 5: jburaiken_hit (Hisshou Buraiken in the air (hyper, unused)) */
const Sint32 pl23_super_05[] = {
	CG_FRAME(    0,    0,   2,           0, 126, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0, 127, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   6,           0, 131, 0x80,   0x20,  ATK,  0x4e,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 127, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl23_super_05 ),
};

/* Char_Super 0: shinkuu_init (Shinkuu Gadouken (hyper)) */
const Sint32 pl23_super_00[] = {
	CG_FRAME(    0,    0,   4,           0,  71, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    1,   6,           0,  71, 0x80,   0x00,  ATK,  0x00,       0x30,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   8,           0,  76, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  27 ),
	CG_FRAME(    0,    1,  56, END_OF_DATA, 119, 0x80,   0x00,  ATK,  0x00,       0x18,    0,   0,     0,      0,  27 ),
	CG_NEXT( pl23_super_00 ),
};

/* Char_Super 1: rekka_init (Kouryuu Rekka (hyper)) */
const Sint32 pl23_super_01[] = {
	CG_FRAME(    0,    0,   4,           0, 120, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,   -1,   2,           0, 120, 0x80,   0x00,  ATK,  0x00,       0x2e,    0,   0,     0,      0,   0 ),
	CG_FRAME( 0x80,    0,   2,           0, 121, 0x80,   0x00,  ATK,  0x00,       0x2f,    0,   0,     0,      0,  51 ),
	CG_FRAME( 0x80,    0,   8,           0, 122, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME( 0x80,    0,   4,           0, 123, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   1,           0, 124, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0, 125, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0, 0x80,   1,           0, 120, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  28 ),
	CG_FRAME(    0, 0x80,   3,           0, 120, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  28 ),
	CG_FRAME(   55, 0x80,   4,           0, 121, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME(   56,    0,  18,           0, 122, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,  16,           0, 123, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0, 0x80,   2,           0, 124, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0, 0x80,   6, END_OF_DATA, 125, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl23_super_01 ),
};

/* Char_Super 2: buraiken_init (Hisshou Buraiken (hyper)) */
const Sint32 pl23_super_02[] = {
	CG_FRAME(    0,    0,   8,           0,  28, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0, 0x80,   1,           0,  28, 0x80,   0x00,  ATK,  0x00,       0x0b,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,  68, 0x80,   0x00,  ATK,  0x42,       0x14,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   3,           0,  69, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3,           0, 120, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   5,           0,  70, 0x80,   0x00,  ATK,  0x43,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   3,           0,  79, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   3,           0,  80, 0x80,   0x00,  ATK,  0x44,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3,           0,  72, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   3,           0,  73, 0x80,   0x00,  ATK,  0x45,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,   3,           0,  74, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_FRAME(    0,    0,   3,           0,  75, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_FRAME(    0,    0,   2,           0,  79, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   1,           0,  80, 0x80,   0x00,  ATK,  0x46,          0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,   7,           0,  81, 0x80,   0x00,  ATK,  0x47,          0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   3,           0,  80, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   3,           0,  93, 0x80,   0x40,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,  94, 0x80,   0x40,  ATK,  0x48,          0,    0,   0,     0,      0,  16 ),
	CG_FRAME(    0,    0,   7,           0,  95, 0x80,   0x40,  ATK,  0x00,          0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,   4,           0,  96, 0x80,   0x40,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   1,           0,  97, 0x80,   0x40,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   5,           0,  98, 0x80,   0x40,  ATK,  0x49,          0,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    0,   3,           0,  83, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,  84, 0x80,   0x00,  ATK,  0x4a,          0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,  85, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl23_super_02 ),
};

/* Char_Super 3: buraiken_move (Hisshou Buraiken (hyper)) */
const Sint32 pl23_super_03[] = {
	CG_FRAME(    0, 0x80,   1,           0, 120, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  28 ),
	CG_FRAME(    0, 0x80,   3,           0, 120, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  28 ),
	CG_FRAME(    0, 0x80,   4,           0, 121, 0x80,   0x00,  ATK,  0x00,       0x15,    0,   0,     0,      0,  29 ),
	CG_FRAME(    0,    0,  18,           0, 122, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  30 ),
	CG_FRAME(    0,    0,   6,           0, 123, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0, 0x80,   2,           0, 124, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0, 0x80,   6, END_OF_DATA, 125, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl23_super_03 ),
};

/* Char_Super 6: otoko_init (Otoko Michi (level 3 hyper)) */
const Sint32 pl23_super_06[] = {
	CG_FRAME(    0,    0,   4,           0,  48, 0x80,   0x00,    0,  0x00,       0x15,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0, 0x80,   4,           0,  48, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
};

const Sint32 pl23_super_06_loop[] = {
	CG_FRAME(    0,    0,  48, END_OF_DATA,  49, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_NEXT( pl23_super_06_loop ),
};

/* Char_Super 7: otoko_reach (Otoko Michi (level 3 hyper)) */
const Sint32 pl23_super_07[] = {
	CG_FRAME(    0,    0,   6,           0,  50, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   6,           0,  29, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,  28, 0x80,   0x0d,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl23_super_07 ),
};

/* Char_Special 14 */
const Sint32 pl23_special_14[] = {
	CG_FRAME(    0,    0,   4,           0,  59, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,  60, 0x80,   0x00,    0,  0x00,       0x0b,    0,   0,     0,      0,   4 ),
};

const Sint32 pl23_special_14_loop[] = {
	CG_FRAME(    0,    0,   4,           0,  61, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,  62, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl23_special_14_loop ),
};

/* Char_Special 15 */
const Sint32 pl23_special_15[] = {
	CG_FRAME(    0,    0,   4,           0,  21, 0x80,   0x00,    0,  0x00,       0x1d,    0,   0,     0,      0,   5 ),
};

const Sint32 pl23_special_15_loop[] = {
	CG_FRAME(    0,    0,   4,           0, 137, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA, 138, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl23_special_15_loop ),
};

/* Char_Special 16 */
const Sint32 pl23_special_16[] = {
	CG_FRAME(    0,    0,   4,           0, 110, 0x80,   0x00,    0,  0x00,       0x2d,    0,   0,     0,      0,   6 ),
};

const Sint32 pl23_special_16_loop[] = {
	CG_FRAME(    0,    0,   4,           0, 139, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA, 140, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl23_special_16_loop ),
};

/* Char_Special 17: zenten_init (Zenten Chouhatsu (rolling taunt)) */
const Sint32 pl23_special_17[] = {
	CG_FRAME(    0,    0,   4,           0,  19, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,  20, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,  21, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,   4,           0,  44, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,  45, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,  46, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,  47, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,  44, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,  45, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,  46, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,  47, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,   4,           0,  90, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,  91, 0x80,   0x00,    0,  0x00,       0x2e,    0,   0,     0,      0,   5 ),
};

const Sint32 pl23_special_17_loop[] = {
	CG_FRAME(    0,    0,   4,           0, 137, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA, 138, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl23_special_17_loop ),
};

/* Char_Special 18: kouten_init (Kouten Chouhatsu (back roll taunt)) */
const Sint32 pl23_special_18[] = {
	CG_FRAME(    0,    0,   4,           0,  19, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,  20, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,  21, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,   4,           0,  47, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,  46, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,  45, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,  44, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,  47, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,  46, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,  45, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,  44, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,   4,           0,  90, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,  91, 0x80,   0x00,    0,  0x00,       0x0b,    0,   0,     0,      0,   5 ),
};

const Sint32 pl23_special_18_loop[] = {
	CG_FRAME(    0,    0,   4,           0, 137, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA, 138, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl23_special_18_loop ),
};

/* Char_Special 19: densetsu_init (Chouhatsu Densetsu (hyper taunt)) */
const Sint32 pl23_special_19[] = {
	CG_FRAME(    0,    0,   4,           0,  59, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    1,   4,           0,  60, 0x80,   0x00,    0,  0x00,       0x0b,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,  61, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,  62, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,  19, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,  20, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,  21, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl23_special_19 ),
};

/* Char_Special 20: densetsu_roll_init (Chouhatsu Densetsu (hyper taunt)) */
const Sint32 pl23_special_20[] = {
	CG_FRAME(    0,    1,   4,           0,  44, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,  45, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,  46, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,  47, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl23_special_20 ),
};

/* Char_Special 21: densetsu_crouch_init (Chouhatsu Densetsu (hyper taunt)) */
const Sint32 pl23_special_21[] = {
	CG_FRAME(    0,    0,   4,           0,  90, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,  91, 0x80,   0x00,    0,  0x00,       0x1d,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0, 137, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0, 138, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0, 137, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0, 138, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0, 137, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA, 138, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl23_special_21 ),
};

/* Char_Special 22: densetsu_jump_init (Chouhatsu Densetsu (hyper taunt)) */
const Sint32 pl23_special_22[] = {
	CG_FRAME(    0,    0,   4,           0, 110, 0x80,   0x00,    0,  0x00,       0x2d,    0,   0,     0,      0,   6 ),
};

const Sint32 pl23_special_22_loop[] = {
	CG_FRAME(    0,    0,   4,           0, 139, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA, 140, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl23_special_22_loop ),
};

/* Char_Special 23: densetsu_jump (Chouhatsu Densetsu (hyper taunt)) */
const Sint32 pl23_special_23[] = {
	CG_FRAME(    0,    0,   4,           0,  21, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   0, 0x80,   0x20,    0,  0x00,       0x0f,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,  38, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,  39, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl23_special_23_loop[] = {
	CG_FRAME(    0,    0,  38, END_OF_DATA,  40, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl23_special_23_loop ),
};

/* Char_Special 34: densetsu_sign_init (Chouhatsu Densetsu (hyper taunt)) */
const Sint32 pl23_special_34[] = {
	CG_FRAME(    0,    0,   3,           0, 146, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   3,           0, 147, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
};

const Sint32 pl23_special_34_loop[] = {
	CG_FRAME(    0,    0,   3,           0, 148, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA, 149, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl23_special_34_loop ),
};

/* Char_Special 35: densetsu_sign_keep (Chouhatsu Densetsu (hyper taunt)) */
const Sint32 pl23_special_35[] = {
	CG_FRAME(    0,    0,   3,           0, 150, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,  32, END_OF_DATA, 151, 0x80,   0x00,    0,  0x00,       0x1b,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl23_special_35 ),
};

/* Char_Special 36: densetsu_sign_keep (Chouhatsu Densetsu (hyper taunt)) */
const Sint32 pl23_special_36[] = {
	CG_FRAME(    0,    0,   3,           0, 150, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,  32, END_OF_DATA, 157, 0x80,   0x00,    0,  0x00,       0x1b,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl23_special_36 ),
};

/* Char_Extra 1: t_kick_move
   Char_Special 24: densetsu_pose_init (Chouhatsu Densetsu (hyper taunt)) */
const Sint32 pl23_extra_01[] = {
	CG_FRAME(    0,    0,   4,           0,  19, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0, 143, 0x80,   0x40,  ATE,  0x00,       0x13,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   6,           0, 144, 0x80,   0x40,  ATE,  0x00,       0x1a,    0,   0,     0,      0,   2 ),
};

const Sint32 pl23_extra_01_loop[] = {
	CG_FRAME(    0,    0,  24, END_OF_DATA, 145, 0x80,   0x40,  ATE,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl23_extra_01_loop ),
};

/* Char_Change 0: act_change_tag_init
   group 25 0 */
const Sint32 pl23_change_00[] = {
	CG_FRAME(    0,    0,   4,           0,  19, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0, 143, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0, 144, 0x80,   0x40,    0,  0x00,       0x1a,    0,   0,     0,      0,   0 ),
};

const Sint32 pl23_change_00_loop[] = {
	CG_FRAME(    0,    0,  10, END_OF_DATA, 145, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl23_change_00_loop ),
};

/* Char_Effect 0: pls23_00_t0_init */
const Sint32 pl23_effect_00[] = {
	CG_FRAME(    0,    0,   1,           0, 253, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA, 254, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl23_effect_00 ),
};

/* Char_Effect 1: pls23_00_piece_init */
const Sint32 pl23_effect_01[] = {
	CG_FRAME(    0,    0,   4,           0, 248, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 249, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 250, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA, 251, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl23_effect_01 ),
};

/* Char_Effect 2: appear_drop_init */
const Sint32 pl23_effect_02[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA, 252, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl23_effect_02 ),
};
