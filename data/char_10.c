/* Tron Bonne (character 10): the animations. PLAYER_WORK's char_adrs points here.

   spr_prm_10      a sprite's sizes and turn (SPR_PRM): a frame's zoom number picks a row, 0 for none.
   char_tbl_10     the groups: pl_char_set( pl_w, group, no ) takes the group's table from here and
                   starts its animation no. The character's module (s_pl10.c) says which group
                   and number each move uses.
   pl10_GROUP_tbl  a group's animations. GROUP is the group's name in player.h (stand for
                   Char_Stand), or grpNN for a group with no name there.
   pl10_GROUP_NN   an animation, named by the first group and number that has it: its frames in
                   order. The comment before it lists its places in the groups and, for each,
                   the routines that start it by that number, with their special move;
                   (2 + atck_level) is a routine that counts on from 2 by the attack's level.
                   A frame with END_OF_DATA is the last, and the word after it is the frame the
                   animation goes on from (CG_NEXT; pl10_GROUP_NN_loop where that is no
                   animation's first frame). An effect's animation has short frames (EF_FRAME)
                   and may end with no word.
   pl10_unused_NN  frames that no table or animation reaches.

   A player's frame (CG_FRAME, the 0x14 bytes of CHAR_DATA), with the names Capcom's tables used:
     prm, type  a number and a code for the character's module (a speed, an attack, a voice)
     ctr        the game frames it is shown for
     end        END_OF_DATA on the last frame
     spr        the sprite, -1 for none
     prio       added to the draw priority (a signed byte)
     shadow     the shadow's offset (the low five bits) and size, 0xff none
     atck       ATK while an attack is on its way or can hit, ATE in its frames after that;
                CG_ATCK_NG lets nothing cancel it
     flag1      a number for the module: most often the attack, a row of hit_dt_10
     se         a voice (1 to 63), or with CSE a sound effect; 0 none
     zoom       the row of spr_prm_10, 0 none
     unk        a byte the program is not seen to read
     flag2      a second code for the character's effects
     rival      where a held opponent is put (the low byte), and a code for the module (the high)
     box        the frame's row of frame_box_10 */
#include "player.h"
#include "char_tbl.h"

extern const Sint32 pl10_stand_00[], pl10_stand_02[], pl10_stand_03[], pl10_stand_06[],
	pl10_stand_07[], pl10_jump_00[], pl10_jump_00_loop[], pl10_jump_03[], pl10_jump_06[],
	pl10_jump_09[], pl10_dash_00[], pl10_dash_00_loop[], pl10_dash_02[], pl10_dash_01[],
	pl10_dash_03[], pl10_dash_04[], pl10_dash_04_loop[], pl10_dash_05[], pl10_crouch_00[],
	pl10_crouch_01[], pl10_crouch_02[], pl10_turn_00[], pl10_turn_01[], pl10_guard_00[],
	pl10_guard_01[], pl10_guard_02[], pl10_guard_03[], pl10_guard_03_loop[], pl10_guard_04[],
	pl10_guard_05[], pl10_guard_06[], pl10_guard_07[], pl10_guard_07_loop[], pl10_air_guard_00[],
	pl10_air_guard_01[], pl10_unused_00[], pl10_stand_p_00[], pl10_stand_p_01[], pl10_stand_p_02[],
	pl10_stand_k_00[], pl10_stand_k_01[], pl10_stand_k_02[], pl10_crouch_p_00[], pl10_crouch_p_01[],
	pl10_crouch_p_02[], pl10_extra_03[], pl10_crouch_k_00[], pl10_crouch_k_01[], pl10_crouch_k_02[],
	pl10_jump_p_00[], pl10_jump_p_01[], pl10_jump_p_02[], pl10_jump_k_00[], pl10_jump_k_01[],
	pl10_jump_k_02[], pl10_get_up_00[], pl10_get_up_01[], pl10_get_up_02[], pl10_get_up_03[],
	pl10_damage_00[], pl10_damage_01[], pl10_damage_02[], pl10_damage_03[], pl10_damage_04[],
	pl10_damage_05[], pl10_damage_06[], pl10_damage_07[], pl10_damage_08[], pl10_damage_10[],
	pl10_damage_11[], pl10_damage_12[], pl10_damage_13[], pl10_damage_09[], pl10_damage_09_loop[],
	pl10_damage_16[], pl10_damage_17[], pl10_damage_18[], pl10_damage_19[], pl10_damage_20[],
	pl10_damage_21[], pl10_damage_22[], pl10_damage_23[], pl10_damage_24[], pl10_damage_25[],
	pl10_damage_26[], pl10_damage_27[], pl10_damage_31[], pl10_damage_34[], pl10_damage_28[],
	pl10_damage_29[], pl10_caught_00[], pl10_caught_01[], pl10_caught_02[], pl10_caught_03[],
	pl10_caught_04[], pl10_caught_05[], pl10_caught_06[], pl10_caught_07[], pl10_caught_08[],
	pl10_caught_09[], pl10_caught_10[], pl10_caught_11[], pl10_caught_12[], pl10_caught_13[],
	pl10_caught_14[], pl10_caught_15[], pl10_caught_16[], pl10_caught_17[], pl10_caught_18[],
	pl10_caught_19[], pl10_caught_20[], pl10_caught_21[], pl10_dizzy_00[], pl10_catch_00[],
	pl10_catch_01[], pl10_catch_02[], pl10_catch_04[], pl10_catch_03[], pl10_win_00[],
	pl10_win_00_loop[], pl10_win_01[], pl10_win_01_loop[], pl10_win_03[], pl10_win_03_loop[],
	pl10_win_04[], pl10_appear_00[], pl10_appear_01[], pl10_appear_02[], pl10_extra_00[],
	pl10_extra_00_loop[], pl10_extra_01[], pl10_extra_02[], pl10_special_00[], pl10_special_01[],
	pl10_special_02[], pl10_special_03[], pl10_special_03_loop[], pl10_special_04[],
	pl10_special_04_loop[], pl10_special_05[], pl10_special_05_loop[], pl10_special_06[],
	pl10_special_06_loop[], pl10_special_07[], pl10_special_08[], pl10_special_09[], pl10_super_00[],
	pl10_super_01[], pl10_effect_12[], pl10_effect_13[], pl10_effect_17[], pl10_effect_17_loop[],
	pl10_effect_22[], pl10_effect_00[], pl10_effect_01[], pl10_effect_02[], pl10_effect_14[],
	pl10_effect_03[], pl10_effect_03_loop[], pl10_effect_06[], pl10_effect_06_loop[],
	pl10_effect_09[], pl10_effect_09_loop[], pl10_effect_04[], pl10_effect_04_loop[],
	pl10_effect_05[], pl10_effect_05_loop[], pl10_effect_08[], pl10_effect_08_loop[],
	pl10_effect_11[], pl10_effect_11_loop[], pl10_effect_16[], pl10_effect_16_loop[],
	pl10_effect_70[], pl10_effect_15[], pl10_effect_15_loop[], pl10_effect_18[], pl10_effect_27[],
	pl10_effect_27_loop[], pl10_effect_19[], pl10_effect_20[], pl10_effect_21[],
	pl10_effect_21_loop[], pl10_effect_25[], pl10_effect_26[], pl10_effect_28[],
	pl10_effect_28_loop[], pl10_effect_29[], pl10_effect_29_loop[], pl10_effect_30[],
	pl10_effect_30_loop[], pl10_effect_31[], pl10_effect_33[], pl10_effect_34[], pl10_effect_35[],
	pl10_effect_36[], pl10_effect_37[], pl10_effect_38[], pl10_effect_39[], pl10_effect_40[],
	pl10_effect_41[], pl10_effect_42[], pl10_effect_43[], pl10_effect_44[], pl10_effect_45[],
	pl10_effect_46[], pl10_effect_47[], pl10_effect_48[], pl10_effect_49[], pl10_effect_50[],
	pl10_effect_51[], pl10_effect_52[], pl10_effect_53[], pl10_effect_54[], pl10_effect_55[],
	pl10_effect_56[], pl10_effect_57[], pl10_effect_58[], pl10_effect_59[], pl10_effect_60[],
	pl10_effect_61[], pl10_effect_62[], pl10_effect_63[], pl10_effect_64[], pl10_effect_64_loop[],
	pl10_effect_65[], pl10_effect_66[], pl10_effect_66_loop[], pl10_effect_67[], pl10_effect_68[],
	pl10_effect_69[], pl10_effect_69_loop[], pl10_effect_71[], pl10_effect_72[], pl10_effect_73[],
	pl10_effect_74[], pl10_effect_75[], pl10_effect_76[], pl10_effect_77[], pl10_effect_78[],
	pl10_effect_79[], pl10_effect_80[], pl10_effect_23[], pl10_effect_24[], pl10_effect_32[],
	pl10_effect_81[], pl10_change_00[], pl10_change_00_loop[], pl10_change_01[], pl10_change_02[],
	pl10_change_02_loop[], pl10_change_03[], pl10_change_04[], pl10_grp27_00[], pl10_grp27_01[],
	pl10_grp27_02[], pl10_grp27_03[], pl10_grp27_04[], pl10_grp27_05[], pl10_grp27_06[],
	pl10_grp27_07[], pl10_grp27_08[], pl10_grp27_09[], pl10_grp27_10[], pl10_grp27_11[],
	pl10_grp27_12[], pl10_grp27_12_loop[], pl10_grp27_14[], pl10_grp27_15[], pl10_grp27_16[],
	pl10_grp27_17[], pl10_grp27_18[], pl10_grp27_19[], pl10_grp27_20[], pl10_grp27_21[],
	pl10_grp27_22[], pl10_grp27_23[], pl10_grp27_24[], pl10_grp27_25[], pl10_grp27_26[],
	pl10_grp27_27[], pl10_grp27_28[], pl10_grp27_29[], pl10_grp27_30[], pl10_grp27_31[],
	pl10_grp27_32[], pl10_grp27_33[], pl10_grp27_34[], pl10_grp27_34_loop[], pl10_grp27_35[],
	pl10_grp27_36[];
extern const Sint32 *const pl10_stand_tbl[], *const pl10_jump_tbl[], *const pl10_dash_tbl[],
	*const pl10_crouch_tbl[], *const pl10_turn_tbl[], *const pl10_guard_tbl[],
	*const pl10_air_guard_tbl[], *const pl10_stand_p_tbl[], *const pl10_stand_k_tbl[],
	*const pl10_crouch_p_tbl[], *const pl10_crouch_k_tbl[], *const pl10_jump_p_tbl[],
	*const pl10_jump_k_tbl[], *const pl10_damage_tbl[], *const pl10_dizzy_tbl[],
	*const pl10_catch_tbl[], *const pl10_caught_tbl[], *const pl10_get_up_tbl[],
	*const pl10_appear_tbl[], *const pl10_win_tbl[], *const pl10_extra_tbl[],
	*const pl10_special_tbl[], *const pl10_super_tbl[], *const pl10_effect_tbl[],
	*const pl10_change_tbl[], *const pl10_grp25_tbl[], *const pl10_grp27_tbl[];

/* the sprites' sizes and turns, by a frame's zoom number */
const SPR_PRM spr_prm_10[16] = {
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 1000, 1000, 0, 0, 0, 1, 0, 0 },
	{ 1000, 1000, -4096, 0, 0, 0, 0, 0 },
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
const Sint32 *const *const char_tbl_10[] = {
	pl10_stand_tbl, pl10_jump_tbl, pl10_dash_tbl, pl10_crouch_tbl, pl10_turn_tbl, pl10_guard_tbl,
	pl10_air_guard_tbl, pl10_stand_p_tbl, pl10_stand_k_tbl, pl10_crouch_p_tbl, pl10_crouch_k_tbl, pl10_jump_p_tbl,
	pl10_jump_k_tbl, pl10_damage_tbl, pl10_dizzy_tbl, pl10_catch_tbl, pl10_caught_tbl, pl10_get_up_tbl,
	pl10_appear_tbl, pl10_win_tbl, pl10_extra_tbl, pl10_special_tbl, pl10_super_tbl, pl10_effect_tbl,
	pl10_change_tbl, pl10_grp25_tbl, pl10_grp25_tbl, pl10_grp27_tbl, 0,
};

const Sint32 *const pl10_stand_tbl[] = {
	pl10_stand_00, pl10_stand_00, pl10_stand_02, pl10_stand_03, pl10_stand_00, pl10_stand_00,
	pl10_stand_06, pl10_stand_07, 0,
};

const Sint32 *const pl10_jump_tbl[] = {
	pl10_jump_00, pl10_jump_00, pl10_jump_00, pl10_jump_03, pl10_jump_03, pl10_jump_03,
	pl10_jump_06, pl10_jump_06, pl10_jump_06, pl10_jump_09, pl10_jump_09, pl10_jump_09,
	0,
};

const Sint32 *const pl10_dash_tbl[] = {
	pl10_dash_00, pl10_dash_01, pl10_dash_02, pl10_dash_03, pl10_dash_04, pl10_dash_05,
	0,
};

const Sint32 *const pl10_crouch_tbl[] = {
	pl10_crouch_00, pl10_crouch_01, pl10_crouch_02, 0,
};

const Sint32 *const pl10_turn_tbl[] = {
	pl10_turn_00, pl10_turn_01, 0,
};

const Sint32 *const pl10_guard_tbl[] = {
	pl10_guard_00, pl10_guard_01, pl10_guard_02, pl10_guard_03, pl10_guard_04, pl10_guard_05,
	pl10_guard_06, pl10_guard_07, 0,
};

const Sint32 *const pl10_air_guard_tbl[] = {
	pl10_air_guard_00, pl10_air_guard_01, pl10_air_guard_01, 0,
};

const Sint32 *const pl10_stand_p_tbl[] = {
	pl10_stand_p_00, pl10_stand_p_01, pl10_stand_p_02, 0,
};

const Sint32 *const pl10_stand_k_tbl[] = {
	pl10_stand_k_00, pl10_stand_k_01, pl10_stand_k_02, 0,
};

const Sint32 *const pl10_crouch_p_tbl[] = {
	pl10_crouch_p_00, pl10_crouch_p_01, pl10_crouch_p_02, 0,
};

const Sint32 *const pl10_crouch_k_tbl[] = {
	pl10_crouch_k_00, pl10_crouch_k_01, pl10_crouch_k_02, 0,
};

const Sint32 *const pl10_jump_p_tbl[] = {
	pl10_jump_p_00, pl10_jump_p_01, pl10_jump_p_02, pl10_jump_p_00, pl10_jump_p_01, pl10_jump_p_02,
	0,
};

const Sint32 *const pl10_jump_k_tbl[] = {
	pl10_jump_k_00, pl10_jump_k_01, pl10_jump_k_02, pl10_jump_k_00, pl10_jump_k_01, pl10_jump_k_02,
	0,
};

const Sint32 *const pl10_damage_tbl[] = {
	pl10_damage_00, pl10_damage_01, pl10_damage_02, pl10_damage_03, pl10_damage_04, pl10_damage_05,
	pl10_damage_06, pl10_damage_07, pl10_damage_08, pl10_damage_09, pl10_damage_10, pl10_damage_11,
	pl10_damage_12, pl10_damage_13, pl10_damage_13, pl10_damage_13, pl10_damage_16, pl10_damage_17,
	pl10_damage_18, pl10_damage_19, pl10_damage_20, pl10_damage_21, pl10_damage_22, pl10_damage_23,
	pl10_damage_24, pl10_damage_25, pl10_damage_26, pl10_damage_27, pl10_damage_28, pl10_damage_29,
	pl10_damage_27, pl10_damage_31, pl10_damage_27, pl10_damage_27, pl10_damage_34, 0,
};

const Sint32 *const pl10_dizzy_tbl[] = {
	pl10_dizzy_00, 0,
};

const Sint32 *const pl10_catch_tbl[] = {
	pl10_catch_00, pl10_catch_01, pl10_catch_02, pl10_catch_03, pl10_catch_04, 0,
};

const Sint32 *const pl10_caught_tbl[] = {
	pl10_caught_00, pl10_caught_01, pl10_caught_02, pl10_caught_03, pl10_caught_04, pl10_caught_05,
	pl10_caught_06, pl10_caught_07, pl10_caught_08, pl10_caught_09, pl10_caught_10, pl10_caught_11,
	pl10_caught_12, pl10_caught_13, pl10_caught_14, pl10_caught_15, pl10_caught_16, pl10_caught_17,
	pl10_caught_18, pl10_caught_19, pl10_caught_20, pl10_caught_21, 0,
};

const Sint32 *const pl10_get_up_tbl[] = {
	pl10_get_up_00, pl10_get_up_01, pl10_get_up_02, pl10_get_up_03, 0,
};

const Sint32 *const pl10_appear_tbl[] = {
	pl10_appear_00, pl10_appear_01, pl10_appear_02, 0,
};

const Sint32 *const pl10_win_tbl[] = {
	pl10_win_00, pl10_win_01, pl10_win_01, pl10_win_03, pl10_win_04, 0,
};

const Sint32 *const pl10_extra_tbl[] = {
	pl10_extra_00, pl10_extra_01, pl10_extra_02, pl10_extra_03, 0,
};

const Sint32 *const pl10_special_tbl[] = {
	pl10_special_00, pl10_special_01, pl10_special_02, pl10_special_03, pl10_special_04, pl10_special_05,
	pl10_special_06, pl10_special_07, pl10_special_08, pl10_special_09, 0,
};

const Sint32 *const pl10_super_tbl[] = {
	pl10_super_00, pl10_super_01, 0,
};

const Sint32 *const pl10_effect_tbl[] = {
	pl10_effect_00, pl10_effect_01, pl10_effect_02, pl10_effect_03, pl10_effect_04, pl10_effect_05,
	pl10_effect_06, pl10_effect_06, pl10_effect_08, pl10_effect_09, pl10_effect_09, pl10_effect_11,
	pl10_effect_12, pl10_effect_13, pl10_effect_14, pl10_effect_15, pl10_effect_16, pl10_effect_17,
	pl10_effect_18, pl10_effect_19, pl10_effect_20, pl10_effect_21, pl10_effect_22, pl10_effect_23,
	pl10_effect_24, pl10_effect_25, pl10_effect_26, pl10_effect_27, pl10_effect_28, pl10_effect_29,
	pl10_effect_30, pl10_effect_31, pl10_effect_32, pl10_effect_33, pl10_effect_34, pl10_effect_35,
	pl10_effect_36, pl10_effect_37, pl10_effect_38, pl10_effect_39, pl10_effect_40, pl10_effect_41,
	pl10_effect_42, pl10_effect_43, pl10_effect_44, pl10_effect_45, pl10_effect_46, pl10_effect_47,
	pl10_effect_48, pl10_effect_49, pl10_effect_50, pl10_effect_51, pl10_effect_52, pl10_effect_53,
	pl10_effect_54, pl10_effect_55, pl10_effect_56, pl10_effect_57, pl10_effect_58, pl10_effect_59,
	pl10_effect_60, pl10_effect_61, pl10_effect_62, pl10_effect_63, pl10_effect_64, pl10_effect_65,
	pl10_effect_66, pl10_effect_67, pl10_effect_68, pl10_effect_69, pl10_effect_70, pl10_effect_71,
	pl10_effect_72, pl10_effect_73, pl10_effect_74, pl10_effect_75, pl10_effect_76, pl10_effect_77,
	pl10_effect_78, pl10_effect_79, pl10_effect_80, pl10_effect_81, 0,
};

const Sint32 *const pl10_change_tbl[] = {
	pl10_change_00, pl10_change_01, pl10_change_02, pl10_change_03, pl10_change_04, 0,
};

const Sint32 *const pl10_grp25_tbl[] = {
	pl10_stand_00, 0,
};

const Sint32 *const pl10_grp27_tbl[] = {
	pl10_grp27_00, pl10_grp27_01, pl10_grp27_02, pl10_grp27_03, pl10_grp27_04, pl10_grp27_05,
	pl10_grp27_06, pl10_grp27_07, pl10_grp27_08, pl10_grp27_09, pl10_grp27_10, pl10_grp27_11,
	pl10_grp27_12, pl10_grp27_12, pl10_grp27_14, pl10_grp27_15, pl10_grp27_16, pl10_grp27_17,
	pl10_grp27_18, pl10_grp27_19, pl10_grp27_20, pl10_grp27_21, pl10_grp27_22, pl10_grp27_23,
	pl10_grp27_24, pl10_grp27_25, pl10_grp27_26, pl10_grp27_27, pl10_grp27_28, pl10_grp27_29,
	pl10_grp27_30, pl10_grp27_31, pl10_grp27_32, pl10_grp27_33, pl10_grp27_34, pl10_grp27_35,
	pl10_grp27_36, 0,
};

	/*         prm  type   ctr          end    spr  prio  shadow  atck  flag1          se  zoom  unk  flag2   rival  box */

/* Char_Stand 0: act_stand, appear_move, act_change_gattai_move, act_change_zero_move, ...
   Char_Stand 1
   Char_Stand 4
   Char_Stand 5
   group 25 0
   Char_Fly 0 */
const Sint32 pl10_stand_00[] = {
	CG_FRAME(    0,    0,   24,           0,   261, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   262, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   263, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   264, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   265, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   266, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   267, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   268, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   269, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   12,           0,   270, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,   271, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,   272, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,   273, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,   274, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,   275, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,   276, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   24,           0,   261, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   262, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   263, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   264, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   265, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   266, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   267, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   268, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   269, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   12,           0,   270, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,   271, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,   272, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,   273, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,   274, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,   275, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,   276, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   24,           0,   261, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   262, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   263, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   264, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   265, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   266, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   267, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   268, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   269, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   12,           0,   270, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,   271, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,   272, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,   273, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,   274, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,   275, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,   276, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   24,           0,   261, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   277, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   278, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   279, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   280, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   16,           0,   281, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   282, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   284, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   16,           0,   283, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   284, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   286, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   16,           0,   285, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   286, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   282, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   16,           0,   281, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   281, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   283, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   285, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   281, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   283, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   285, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   281, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   283, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   285, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   281, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   279, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   278, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   277, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   287, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   28,           0,   261, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   288, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   289, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   290, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   291, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   28,           0,   292, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   293, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   294, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   295, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   296, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   295, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   294, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   295, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   296, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   295, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   294, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   295, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   296, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   295, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   28,           0,   294, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   293, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   292, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   290, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   289, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   288, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl10_stand_00 ),
};

/* Char_Stand 2: act_walk (2 + no) */
const Sint32 pl10_stand_02[] = {
	CG_FRAME(    0,    0,    4,           0,   298, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   299, 0x80,   0x00,    0,  0x00,       0x1e,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   300, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   301, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   302, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   303, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   304, 0x80,   0x00,    0,  0x00,       0x1e,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   305, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   306, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   307, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl10_stand_02 ),
};

/* Char_Stand 3 */
const Sint32 pl10_stand_03[] = {
	CG_FRAME(    0,    0,    6,           0,   308, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   309, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   310, 0x80,   0x00,    0,  0x00,       0x1f,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   311, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   312, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   313, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   314, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   315, 0x80,   0x00,    0,  0x00,       0x1f,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   316, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   317, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl10_stand_03 ),
};

/* Char_Stand 6 */
const Sint32 pl10_stand_06[] = {
	CG_FRAME(    0,    0,    4,           0,   298, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   301, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   302, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   303, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   306, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   307, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl10_stand_06 ),
};

/* Char_Stand 7 */
const Sint32 pl10_stand_07[] = {
	CG_FRAME(    0,    0,    6,           0,   308, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   309, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   312, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   313, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   314, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl10_stand_07 ),
};

/* Char_Jump 0
   Char_Jump 1: act_wall_jump, act_change_gattai_init, act_change_zero_init, act_change_assist_init, ...
   Char_Jump 2 */
const Sint32 pl10_jump_00[] = {
	CG_FRAME(    0,    1,    1,           0,   323, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    1,    1,           0,   324, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    1,    2,           0,   325, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   24,           0,   328, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   329, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   330, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl10_jump_00_loop[] = {
	CG_FRAME(    0,    0,    4,           0,   331, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   332, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl10_jump_00_loop ),
};

/* Char_Jump 3
   Char_Jump 4
   Char_Jump 5 */
const Sint32 pl10_jump_03[] = {
	CG_FRAME(    0,    0,    1,           0,   324, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   324, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   323, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl10_jump_03 ),
};

/* Char_Jump 6
   Char_Jump 7
   Char_Jump 8 */
const Sint32 pl10_jump_06[] = {
	CG_FRAME(    0,    1,    1,           0,   323, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    1,    1,           0,   324, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    1,    2,           0,   325, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   326, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   24,           0,   327, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   328, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   329, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   330, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Jump 9: act_fall, act_air_guard_hold, act_wall_jump
   Char_Jump 10: change_in
   Char_Jump 11: act_change_out_recover */
const Sint32 pl10_jump_09[] = {
	CG_FRAME(    0,    0,    4,           0,   331, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   332, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl10_jump_09 ),
};

/* Char_Dash 0: act_dash */
const Sint32 pl10_dash_00[] = {
	CG_FRAME(    0,    1,    1,           0,   324, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   318, 0x80,   0x00,    0,  0x00, 0x4b | CSE,    0,   0,     0,      0,   1 ),
};

const Sint32 pl10_dash_00_loop[] = {
	CG_FRAME(    0,    0,    4,           0,   319, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   318, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl10_dash_00_loop ),
};

/* Char_Dash 2: dash_move */
const Sint32 pl10_dash_02[] = {
	CG_FRAME(    0,    0,    1,           0,   320, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    7,           0,   320, 0x80,   0x00,    0,  0x00,       0x20,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    4,           0,   323, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    4, END_OF_DATA,   324, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl10_dash_02 ),
};

/* Char_Dash 1: act_back_dash */
const Sint32 pl10_dash_01[] = {
	CG_FRAME(    0,    0,    4,           0,   321, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   322, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl10_dash_01 ),
};

/* Char_Dash 3: noki_jump */
const Sint32 pl10_dash_03[] = {
	CG_FRAME(    0,    0,    1,           0,   323, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    7,           0,   323, 0x80,   0x00,    0,  0x00,       0x20,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    8, END_OF_DATA,   324, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl10_dash_03 ),
};

/* Char_Dash 4: air_dash_init, fly_start_init, fly_start2_init */
const Sint32 pl10_dash_04[] = {
	CG_FRAME(   24,    0,   12,           0,   318, 0x80,   0x00,    0,  0x00, 0x4b | CSE,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    1,    4,           0,   319, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl10_dash_04_loop[] = {
	CG_FRAME(    0,    0,    4,           0,   318, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   319, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl10_dash_04_loop ),
};

/* Char_Dash 5: air_dash_move */
const Sint32 pl10_dash_05[] = {
	CG_FRAME(    0,    0,    1,           0,   320, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    7,           0,   320, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    1,    4,           0,   320, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    1,    4, END_OF_DATA,   320, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl10_dash_05 ),
};

/* Char_Crouch 0: act_crouch, act_stand_up */
const Sint32 pl10_crouch_00[] = {
	CG_FRAME(    0,    0,    4,           0,   333, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4,           0,   334, 0x80,   0x00,    0,  0x00,       0x22,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4,           0,   335, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4,           0,   336, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA,   337, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl10_crouch_00 ),
};

/* Char_Crouch 1: act_crouching, act_escape */
const Sint32 pl10_crouch_01[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   337, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl10_crouch_01 ),
};

/* Char_Crouch 2: act_stand_up, act_crouch */
const Sint32 pl10_crouch_02[] = {
	CG_FRAME(    0,    0,    4,           0,   338, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   339, 0x80,   0x00,    0,  0x00,       0x22,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   340, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   333, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA,   261, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl10_crouch_02 ),
};

/* Char_Turn 0: act_turn */
const Sint32 pl10_turn_00[] = {
	CG_FRAME(    0,    0,    4,           0,   342, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   343, 0x80,   0x00,    0,  0x00,       0x1e,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   344, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl10_turn_00 ),
};

/* Char_Turn 1: act_crouch_turn */
const Sint32 pl10_turn_01[] = {
	CG_FRAME(    0,    0,    4,           0,   345, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4,           0,   346, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   347, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl10_turn_01 ),
};

/* Char_Guard 0 */
const Sint32 pl10_guard_00[] = {
	CG_FRAME(    0,    0,    4,           0,   348, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   349, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

/* Char_Guard 1: act_guard_hold */
const Sint32 pl10_guard_01[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   350, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl10_guard_01 ),
};

/* Char_Guard 2 */
const Sint32 pl10_guard_02[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   348, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl10_guard_02 ),
};

/* Char_Guard 3 */
const Sint32 pl10_guard_03[] = {
	CG_FRAME(    0,    0,    4,           0,   349, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl10_guard_03_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   350, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl10_guard_03_loop ),
};

/* Char_Guard 4 */
const Sint32 pl10_guard_04[] = {
	CG_FRAME(    0,    0,    4,           0,   351, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4,           0,   352, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
};

/* Char_Guard 5: act_guard_hold */
const Sint32 pl10_guard_05[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   353, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl10_guard_05 ),
};

/* Char_Guard 6 */
const Sint32 pl10_guard_06[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   351, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl10_guard_06 ),
};

/* Char_Guard 7 */
const Sint32 pl10_guard_07[] = {
	CG_FRAME(    0,    0,    4,           0,   352, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
};

const Sint32 pl10_guard_07_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   353, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl10_guard_07_loop ),
};

/* Char_Air_Guard 0: act_air_guard */
const Sint32 pl10_air_guard_00[] = {
	CG_FRAME(    0,    0,    3,           0,   354, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   355, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   356, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   357, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Air_Guard 1: act_air_guard_start
   Char_Air_Guard 2: guard_char_set */
const Sint32 pl10_air_guard_01[] = {
	CG_FRAME(    0,    0,    3,           0,   358, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   359, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   360, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl10_air_guard_01 ),
};

const Sint32 pl10_unused_00[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,    3,           0,   355, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   356, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   357, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	0x00001754,
};

/* Char_Stand_P 0 (light) */
const Sint32 pl10_stand_p_00[] = {
	CG_FRAME(    0,    0,    4,           0,   465, 0x90,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    2,           0,   466, 0x90,   0x00,  ATK,  0x00,       0x03,    0,   0,     0,      0,  16 ),
	CG_FRAME(    0,    0,    4,           0,   467, 0x90,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  16 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   468, 0x90,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  17 ),
	CG_NEXT( pl10_stand_p_00 ),
};

/* Char_Stand_P 1 (medium) */
const Sint32 pl10_stand_p_01[] = {
	CG_FRAME(    0,    0,    2,           0,   469, 0x90,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    2,           0,   470, 0x90,   0x0a,  ATK,  0x00,       0x22,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    2,           0,   471, 0x90,   0x08,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    2,           0,   472, 0x90,   0x06,  ATK,  0x00,       0x1e,    0,   0,     0,      0,  18 ),
	CG_FRAME( 0x80,    0,    1,           0,   473, 0x90,   0x06,  ATK,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,    2,           0,   474, 0x90,   0x06,  ATK,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME( 0x80,    1,    1,           0,   475, 0x90,   0x06,  ATK,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,    2,           0,   476, 0x90,   0x06,  ATK,  0x00,          0,    0,   0,     0,      0,  19 ),
	CG_FRAME( 0x80,    1,    1,           0,   477, 0x90,   0x06,  ATK,  0x00,          0,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    0,    2,           0,   478, 0x90,   0x06,  ATK,  0x00,          0,    0,   0,     0,      0,  19 ),
	CG_FRAME( 0x80,    1,    1,           0,   479, 0x90,   0x06,  ATK,  0x00,          0,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    0,    8,           0,   480, 0x90,   0x06,  ATE,  0x00,          0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,    4,           0,   481, 0x90,   0x06,  ATE,  0x00,          0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,    4,           0,   482, 0x90,   0x06,  ATE,  0x00,          0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA,   261, 0x90,   0x00,  ATE,  0x00,          0,    0,   0,     0, 0xffe0,   5 ),
	CG_NEXT( pl10_stand_p_01 ),
};

/* Char_Stand_P 2 (heavy) */
const Sint32 pl10_stand_p_02[] = {
	CG_FRAME(    0,    0,    6,           0,   508, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    2,           0,   509, 0x80,   0x4d,  ATK,  0x00,       0x23,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    2,           0,   510, 0x80,   0x4a,  ATK,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    3,           0,   511, 0x80,   0x4a,  ATK,  0x00, 0x16 | CSE,    0,   0,     0,      0,  32 ),
	CG_FRAME(    0,    0,    3,           0,   512, 0x80,   0x4a,  ATK,  0x00,          0,    0,   0,     0,      0,  33 ),
	CG_FRAME(    0,    0,    3,           0,   513, 0x80,   0x4a,  ATK,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    1,    3,           0,   514, 0x80,   0x4a,  ATK,  0x00, 0x16 | CSE,    0,   0,     0,      0,  32 ),
	CG_FRAME(    0,    0,    3,           0,   515, 0x80,   0x4a,  ATK,  0x00,          0,    0,   0,     0,      0,  33 ),
	CG_FRAME(    0,    0,    3,           0,   517, 0x80,   0x4a,  ATK,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    1,    4,           0,   511, 0x80,   0x4a,  ATK,  0x00, 0x16 | CSE,    0,   0,     0,      0,  32 ),
	CG_FRAME(    0,    0,    4,           0,   512, 0x80,   0x4a,  ATK,  0x00,          0,    0,   0,     0,      0,  33 ),
	CG_FRAME(    0,    0,    4,           0,   513, 0x80,   0x4a,  ATK,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    1,    4,           0,   514, 0x80,   0x4a,  ATK,  0x00, 0x16 | CSE,    0,   0,     0,      0,  32 ),
	CG_FRAME(    0,    0,    4,           0,   515, 0x80,   0x4a,  ATK,  0x00,          0,    0,   0,     0,      0,  33 ),
	CG_FRAME(    0,    0,    4,           0,   517, 0x80,   0x4a,  ATK,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,    7,           0,   511, 0x80,   0x4a,  ATE,  0x00, 0x16 | CSE,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,   16,           0,   512, 0x80,   0x4a,  ATE,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,    8,           0,   518, 0x80,   0x4a,  ATE,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,    4,           0,   516, 0x80,   0x07,  ATE,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,    3,           0,   519, 0x80,   0x07,  ATE,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA,   261, 0x90,   0x00,  ATE,  0x00,       0x1e,    0,   0,     0, 0xffe6,   5 ),
	CG_NEXT( pl10_stand_p_02 ),
};

/* Char_Stand_K 0 (light) */
const Sint32 pl10_stand_k_00[] = {
	CG_FRAME(    0,    0,    3,           0,   495, 0x90,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    1,           0,   496, 0x90,   0x4d,  ATK,  0x00,       0x22,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    2,           0,   496, 0x90,   0x4d,  ATK,  0x00,       0x14,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    4,           0,   497, 0x90,   0x4d,  ATK,  0x00,          0,    0,   0,     0,      0,  22 ),
	CG_FRAME(    0,    0,    4,           0,   498, 0x90,   0x4d,  ATK,  0x00,          0,    0,   0,     0,      0,  22 ),
	CG_FRAME( 0x80,    1,    4,           0,   499, 0x90,   0x4d,  ATK,  0x00,          0,    0,   0,     0,      0,  23 ),
	CG_FRAME(    0,    0,    4,           0,   500, 0x90,   0x4d,  ATE,  0x00,          0,    0,   0,     0,      0,  37 ),
	CG_FRAME(    0,    0,   16,           0,   501, 0x90,   0x4d,  ATE,  0x00,          0,    0,   0,     0,      0,  37 ),
	CG_FRAME(    0,    0,    4,           0,   502, 0x90,   0x4d,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   503, 0x90,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl10_stand_k_00 ),
};

/* Char_Stand_K 1 (medium) */
const Sint32 pl10_stand_k_01[] = {
	CG_FRAME(    0,    0,    8,           0,   504, 0x90,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,    8,           0,   505, 0x90,   0x00,  ATK,  0x00, 0x4b | CSE,    0,   0,     0,      0,  24 ),
	CG_FRAME(    1,    1,    8,           0,   505, 0x90,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  24 ),
	CG_FRAME(    1,    1,   16,           0,   505, 0x90,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    0,    8,           0,   506, 0x90,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    8, END_OF_DATA,   507, 0x90,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl10_stand_k_01 ),
};

/* Char_Stand_K 2 (heavy) */
const Sint32 pl10_stand_k_02[] = {
	CG_FRAME(    0,    0,    2,           0,   483, 0x90,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    2,           0,   484, 0x90,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    4,           0,   485, 0x90,   0x00,  ATK,  0x00,       0x26,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,    2,           0,   486, 0x90,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,    2,           0,   487, 0x90,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_FRAME( 0x80,    0,    2,           0,   488, 0x90,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME( 0x80,    0,    3,           0,   489, 0x90,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME( 0x80,    0,    5,           0,   490, 0x90,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    1,           0,   490, 0x90,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,    1,           0,   490, 0x90,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    4,           0,   491, 0x90,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   12,           0,   491, 0x90,   0x00,  ATK,  0x00,          0,    2,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,    4,           0,   492, 0x90,   0x00,  ATE,  0x00,       0x15,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    4,           0,   493, 0x90,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    4,           0,   494, 0x90,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    4,           0,   324, 0x90,   0x00,  ATE,  0x00,       0x20,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   323, 0x90,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl10_stand_k_02 ),
};

/* Char_Crouch_P 0 (light) */
const Sint32 pl10_crouch_p_00[] = {
	CG_FRAME(    0,    0,    4,           0,   520, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  27 ),
	CG_FRAME(    0,    0,    2,           0,   521, 0x80,   0x00,  ATK,  0x00,       0x03,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,    4,           0,   522, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   523, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl10_crouch_p_00 ),
};

/* Char_Crouch_P 1 (medium) */
const Sint32 pl10_crouch_p_01[] = {
	CG_FRAME(    0,    0,    4,           0,   524, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    4,           0,   525, 0x80,   0x00,  ATK,  0x00,       0x22,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    1,    4,           0,   526, 0x80,   0x00,  ATK,  0x00,       0x2d,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    1,    4,           0,   527, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    1,   16,           0,   528, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    8,           0,   529, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    8,           0,   530, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    8, END_OF_DATA,   531, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl10_crouch_p_01 ),
};

/* Char_Crouch_P 2 (heavy) */
const Sint32 pl10_crouch_p_02[] = {
	CG_FRAME(    0,    0,    1,           0,   532, 0x90,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    5,           0,   532, 0x90,   0x00,  ATK,  0x00,       0x22,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    2,           0,   533, 0x90,   0x00,  ATK,  0x00,       0x04,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    2,           0,   534, 0x90,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    1,    2,           0,   535, 0x90,   0x00,  ATK,  0x00, 0x49 | CSE,    0,   0,     0,      0,  28 ),
	CG_FRAME(    1,    1,    2,           0,   536, 0x90,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  29 ),
	CG_FRAME(    2,    1,    2,           0,   535, 0x90,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  30 ),
	CG_FRAME(    3,    1,    3,           0,   536, 0x90,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  31 ),
	CG_FRAME(    4,    1,    3,           0,   536, 0x90,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  31 ),
	CG_FRAME(    5,    1,    3,           0,   536, 0x90,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  31 ),
	CG_FRAME(    6,    1,    3,           0,   536, 0x90,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  31 ),
	CG_FRAME(    7,    1,    3,           0,   536, 0x90,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  31 ),
	CG_FRAME(    8,    1,    3,           0,   536, 0x90,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  31 ),
	CG_FRAME(    0,    0,    6,           0,   537, 0x90,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  31 ),
	CG_FRAME(    0,    0,    6,           0,   538, 0x90,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   539, 0x90,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl10_crouch_p_02 ),
};

/* Char_Extra 3: upper_init, fly_start5 */
const Sint32 pl10_extra_03[] = {
	CG_FRAME(    0,    0,    1,           0,   532, 0x90,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    1,           0,   532, 0x90,   0x00,  ATK,  0x00,       0x22,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    4,           0,   532, 0x90,   0x00,  ATK,  0x00, 0x16 | CSE,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    2,           0,   533, 0x90,   0x00,  ATK,  0x00,       0x04,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    2,           0,   534, 0x90,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    1,    2,           0,   535, 0x90,   0x00,  ATK,  0x00, 0x49 | CSE,    0,   0,     0,      0,  28 ),
	CG_FRAME(    1,    1,    2,           0,   536, 0x90,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  29 ),
	CG_FRAME(    2,    1,    2,           0,   535, 0x90,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  30 ),
	CG_FRAME(    3,    1,    3,           0,   536, 0x90,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  31 ),
	CG_FRAME(    4,    1,    3,           0,   536, 0x90,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    5,    1,    3,           0,   536, 0x90,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    6,    1,    3,           0,   536, 0x90,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    7,    1,    3,           0,   536, 0x90,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    8,    1,    3,           0,   536, 0x90,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    6,           0,   537, 0x90,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    6,           0,   538, 0x90,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   539, 0x90,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl10_extra_03 ),
};

/* Char_Crouch_K 0 (light) */
const Sint32 pl10_crouch_k_00[] = {
	CG_FRAME(    0,    0,    2,           0,   540, 0x90,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    1,           0,   541, 0x90,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    2,           0,   542, 0x90,   0x4d,  ATK,  0x00,       0x23,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    4,           0,   543, 0x90,   0xa1,  ATK,  0x00,          0,    0,   0,     0,      0,  35 ),
	CG_FRAME(    0,    0,    4,           0,   544, 0x90,   0xa1,  ATK,  0x00,          0,    0,   0,     0,      0,  35 ),
	CG_FRAME(    0,    1,    2,           0,   543, 0x90,   0xa1,  ATK,  0x00,          0,    0,   0,     0,      0,  35 ),
	CG_FRAME(    0,    0,    2,           0,   544, 0x90,   0xa1,  ATK,  0x00,          0,    0,   0,     0,      0,  35 ),
	CG_FRAME(    0,    1,    2,           0,   543, 0x90,   0xa1,  ATK,  0x00,          0,    0,   0,     0,      0,  35 ),
	CG_FRAME(    0,    0,    2,           0,   544, 0x90,   0xa1,  ATK,  0x00,          0,    0,   0,     0,      0,  35 ),
	CG_FRAME(    0,    1,    2,           0,   543, 0x90,   0xa1,  ATK,  0x00,          0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,    2,           0,   544, 0x90,   0xa1,  ATK,  0x00,          0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,    4,           0,   542, 0x90,   0x4d,  ATE,  0x00,       0x22,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    4,           0,   541, 0x90,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   540, 0x90,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl10_crouch_k_00 ),
};

/* Char_Crouch_K 1 (medium) */
const Sint32 pl10_crouch_k_01[] = {
	CG_FRAME(    0,    0,    2,           0,   545, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    8,           0,   546, 0x80,   0x00,  ATK,  0x00,       0x24,    0,   0,     0,      0,   6 ),
	CG_FRAME( 0x80,    0,    2,           0,   548, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME( 0x80,    0,    2,           0,   549, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME( 0x80,    0,    2,           0,   550, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME( 0x80,    0,    2,           0,   551, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME( 0x80,    0,    2,           0,   552, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME( 0x80,    0,    2,           0,   553, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME( 0x80,    0,    2,           0,   554, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME( 0x80,    0,    2,           0,   555, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME( 0x80,    0,    2,           0,   556, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME( 0x80,    0,    2,           0,   557, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME( 0x80,    0,    2,           0,   558, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME( 0x80,    0,    2,           0,   559, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME( 0x80,    0,    2,           0,   560, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME( 0x80,    0,    2,           0,   561, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME( 0x80,    0,    2,           0,   562, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   12,           0,   563, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    6,           0,   564, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    6,           0,   565, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   531, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl10_crouch_k_01 ),
};

/* Char_Crouch_K 2 (heavy) */
const Sint32 pl10_crouch_k_02[] = {
	CG_FRAME(    0,    0,    4,           0,   566, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    4,           0,   567, 0x80,   0x00,  ATK,  0x00,       0x22,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    2,           0,   568, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,    2,           0,   569, 0x80,   0x00,  ATK,  0x00, 0x16 | CSE,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,    2,           0,   570, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,    2,           0,   571, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,    2,           0,   572, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,    2,           0,   569, 0x80,   0x00,  ATK,  0x00, 0x16 | CSE,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,    2,           0,   570, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,    2,           0,   573, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    4,           0,   574, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    4,           0,   575, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   531, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl10_crouch_k_02 ),
};

/* Char_Jump_P 0 (light)
   Char_Jump_P 3 */
const Sint32 pl10_jump_p_00[] = {
	CG_FRAME(    0,    0,    4,           0,   576, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    8,           0,   577, 0x80,   0x00,  ATK,  0x00,       0x03,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,    4,           0,   578, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   579, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl10_jump_p_00 ),
};

/* Char_Jump_P 1 (medium)
   Char_Jump_P 4 */
const Sint32 pl10_jump_p_01[] = {
	CG_FRAME(    0,    0,    4,           0,   580, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    4,           0,   581, 0x80,   0x00,  ATK,  0x00,       0x04,    0,   0,     0,      0,   7 ),
	CG_FRAME( 0x80,    0,    4,           0,   582, 0x80,   0x00,  ATK,  0x00,       0x2b,    0,   0,     0,      0,  49 ),
	CG_FRAME( 0x80,    0,    1,           0,   583, 0x80,   0x00,  ATK,  0x00, 0x4f | CSE,    0,   0,     0,      0,  49 ),
	CG_FRAME( 0x80,    0,    1,           0,   584, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME( 0x80,    0,    1,           0,   585, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME( 0x84,    0,    1,           0,   586, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME( 0x80,    0,    1,           0,   583, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME( 0x80,    0,    1,           0,   584, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME( 0x80,    0,    1,           0,   585, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME( 0x84,    0,    1,           0,   586, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME( 0x80,    0,    1,           0,   583, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME( 0x80,    0,    1,           0,   584, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME( 0x80,    0,    1,           0,   585, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME( 0x84,    0,    1,           0,   586, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME( 0x80,    0,    1,           0,   583, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME( 0x80,    0,    1,           0,   584, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME( 0x80,    0,    1,           0,   585, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME( 0x84,    0,    1,           0,   586, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,    8, END_OF_DATA,   587, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl10_jump_p_01 ),
};

/* Char_Jump_P 2 (heavy)
   Char_Jump_P 5 */
const Sint32 pl10_jump_p_02[] = {
	CG_FRAME(    0,    0,    4,           0,   588, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  40 ),
	CG_FRAME(    0,    0,    3,           0,   589, 0x80,   0x00,  ATK,  0x00,       0x23,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    3,           0,   590, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    3,           0,   591, 0x80,   0x00,  ATK,  0x00, 0x16 | CSE,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,    3,           0,   592, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,    3,           0,   593, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    3,           0,   594, 0x80,   0x00,  ATK,  0x00, 0x16 | CSE,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,    3,           0,   595, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,    3,           0,   597, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    4,           0,   598, 0x80,   0x00,  ATK,  0x00, 0x16 | CSE,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,    4,           0,   592, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,    4,           0,   600, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    4,           0,   594, 0x80,   0x00,  ATK,  0x00, 0x16 | CSE,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,    4,           0,   601, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,    4,           0,   597, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    7,           0,   591, 0x80,   0x00,  ATE,  0x00, 0x16 | CSE,    0,   0,     0,      0,  62 ),
	CG_FRAME(    0,    0,   16,           0,   592, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  62 ),
	CG_FRAME(    0,    0,    8,           0,   598, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  62 ),
	CG_FRAME(    0,    0,    8, END_OF_DATA,   596, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_NEXT( pl10_jump_p_02 ),
};

/* Char_Jump_K 0 (light)
   Char_Jump_K 3 */
const Sint32 pl10_jump_k_00[] = {
	CG_FRAME(    0,    0,    2,           0,   602, 0x90,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    2,           0,   603, 0x90,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    2,           0,   604, 0x90,   0x00,  ATK,  0x00,       0x23,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,    2,           0,   605, 0x90,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,    2,           0,   606, 0x90,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    1,    2,           0,   605, 0x90,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,    2,           0,   606, 0x90,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    1,    2,           0,   605, 0x90,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,    2,           0,   606, 0x90,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    1,    2,           0,   605, 0x90,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,    2,           0,   606, 0x90,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,    4,           0,   604, 0x90,   0x00,  ATE,  0x00,       0x22,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,    4,           0,   603, 0x90,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   602, 0x90,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl10_jump_k_00 ),
};

/* Char_Jump_K 1 (medium)
   Char_Jump_K 4 */
const Sint32 pl10_jump_k_01[] = {
	CG_FRAME(    0,    0,    4,           0,   607, 0x90,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    1,           0,   608, 0x90,   0x00,  ATK,  0x00,       0x22,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    1,           0,   608, 0x90,   0x00,  ATK,  0x00,       0x14,    0,   0,     0,      0,   7 ),
	CG_FRAME( 0x80,    0,    2,           0,   609, 0x90,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    4,           0,   610, 0x90,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    4,           0,   611, 0x90,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,    3,           0,   612, 0x90,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   613, 0x90,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl10_jump_k_01 ),
};

/* Char_Jump_K 2 (heavy)
   Char_Jump_K 5 */
const Sint32 pl10_jump_k_02[] = {
	CG_FRAME(    0,    0,    7,           0,   614, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    2,           0,   615, 0x80,   0x00,  ATK,  0x00,       0x22,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    2,           0,   616, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  44 ),
	CG_FRAME(    0,    0,    2,           0,   617, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,    2,           0,   618, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,    2,           0,   617, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,    2,           0,   618, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,    2,           0,   617, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,    2,           0,   618, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,    4,           0,   619, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   579, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl10_jump_k_02 ),
};

/* Char_Get_Up 0: dm_dizzy_wait */
const Sint32 pl10_get_up_00[] = {
	CG_FRAME(    0,    0,    6,           0,   361, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   362, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   363, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   364, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   365, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   323, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   324, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl10_get_up_00 ),
};

/* Char_Get_Up 1 */
const Sint32 pl10_get_up_01[] = {
	CG_FRAME(    0,    0,    2,           0,   366, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   367, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   367, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   368, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   371, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   369, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   370, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   323, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   324, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl10_get_up_01 ),
};

/* Char_Get_Up 2 */
const Sint32 pl10_get_up_02[] = {
	CG_FRAME(    0,    0,    4,           0,   366, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   367, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   367, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   368, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   371, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   368, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   371, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   368, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   371, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   369, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   370, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   323, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   324, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl10_get_up_02 ),
};

/* Char_Get_Up 3 */
const Sint32 pl10_get_up_03[] = {
	CG_FRAME(    0,    0,    6,           0,   366, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   367, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   367, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   368, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   371, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   369, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   370, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   323, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   324, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl10_get_up_03 ),
};

/* Char_Damage 0: dm_freeze, dm_freeze_air, caught_dm */
const Sint32 pl10_damage_00[] = {
	CG_FRAME(    0,    0,    6,           0,   753, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5, END_OF_DATA,   752, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl10_damage_00 ),
};

/* Char_Damage 1 */
const Sint32 pl10_damage_01[] = {
	CG_FRAME(    0,    0,    6,           0,   754, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   753, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   752, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl10_damage_01 ),
};

/* Char_Damage 2 */
const Sint32 pl10_damage_02[] = {
	CG_FRAME(    0,    0,    6,           0,   755, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   754, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   753, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   752, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl10_damage_02 ),
};

/* Char_Damage 3 */
const Sint32 pl10_damage_03[] = {
	CG_FRAME(    0,    0,    6,           0,   757, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5, END_OF_DATA,   756, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl10_damage_03 ),
};

/* Char_Damage 4 */
const Sint32 pl10_damage_04[] = {
	CG_FRAME(    0,    0,    6,           0,   758, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   757, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   756, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl10_damage_04 ),
};

/* Char_Damage 5 */
const Sint32 pl10_damage_05[] = {
	CG_FRAME(    0,    0,    6,           0,   759, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   758, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   757, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   756, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl10_damage_05 ),
};

/* Char_Damage 6 */
const Sint32 pl10_damage_06[] = {
	CG_FRAME(    0,    0,   11, END_OF_DATA,   760, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl10_damage_06 ),
};

/* Char_Damage 7 */
const Sint32 pl10_damage_07[] = {
	CG_FRAME(    0,    0,    9,           0,   761, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    9, END_OF_DATA,   760, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl10_damage_07 ),
};

/* Char_Damage 8 */
const Sint32 pl10_damage_08[] = {
	CG_FRAME(    0,    0,    8,           0,   762, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    8,           0,   761, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    8, END_OF_DATA,   760, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl10_damage_08 ),
};

/* Char_Damage 10: dm_ko_blow */
const Sint32 pl10_damage_10[] = {
	CG_FRAME(    0,    0,    5,           0,   764, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    5, END_OF_DATA,   763, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl10_damage_10 ),
};

/* Char_Damage 11 */
const Sint32 pl10_damage_11[] = {
	CG_FRAME(    0,    0,    4,           0,   765, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   764, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   763, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl10_damage_11 ),
};

/* Char_Damage 12 */
const Sint32 pl10_damage_12[] = {
	CG_FRAME(    0,    0,    3,           0,   766, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   765, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   764, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   763, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl10_damage_12 ),
};

/* Char_Damage 13: dm_snap_fly, dm_air
   Char_Damage 14
   Char_Damage 15 */
const Sint32 pl10_damage_13[] = {
	CG_FRAME(    0,    0,    8, END_OF_DATA,   784, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl10_damage_13 ),
};

/* Char_Damage 9: dm_sweep */
const Sint32 pl10_damage_09[] = {
	CG_FRAME(    0,    0,    3,           0,   767, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   768, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   769, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   770, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   771, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   772, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   783, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   784, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   785, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl10_damage_09_loop[] = {
	CG_FRAME(    0,    0,    8, END_OF_DATA,   786, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl10_damage_09_loop ),
};

/* Char_Damage 16: dm_blow_char_set (16 + no), dm_spark, caught_shock_init */
const Sint32 pl10_damage_16[] = {
	CG_FRAME(    0,    0,    4,           0,   773, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   774, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl10_damage_16 ),
};

/* Char_Damage 17: dm_07_1 */
const Sint32 pl10_damage_17[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   775, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl10_damage_17 ),
};

/* Char_Damage 18 */
const Sint32 pl10_damage_18[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   776, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl10_damage_18 ),
};

/* Char_Damage 19 */
const Sint32 pl10_damage_19[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   777, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl10_damage_19 ),
};

/* Char_Damage 20 */
const Sint32 pl10_damage_20[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   778, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl10_damage_20 ),
};

/* Char_Damage 21: dm_fall_char_set (21 + no) */
const Sint32 pl10_damage_21[] = {
	CG_FRAME(    0,    0,    4,           0,   773, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 22 */
const Sint32 pl10_damage_22[] = {
	CG_FRAME(    0,    0,    4,           0,   775, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 23 */
const Sint32 pl10_damage_23[] = {
	CG_FRAME(    0,    0,    4,           0,   776, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 24 */
const Sint32 pl10_damage_24[] = {
	CG_FRAME(    0,    0,    4,           0,   777, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 25 */
const Sint32 pl10_damage_25[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   778, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl10_damage_25 ),
};

/* Char_Damage 26: dm_buttobi_fly, dm_07_1, dm_away_fly, dm_bound_hop, ... */
const Sint32 pl10_damage_26[] = {
	CG_FRAME(    0,    0,    1,           0,   779, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    1,           0,   780, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    2,           0,   781, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    2,           0,   782, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    2,           0,   783, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    2,           0,   784, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   785, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl10_damage_26 ),
};

/* Char_Damage 27
   Char_Damage 30: act_air_recover, act_change_out_init, caught_blow_fly, caught_blow_fix_fly, ...
   Char_Damage 32: act_escape
   Char_Damage 33: act_escape */
const Sint32 pl10_damage_27[] = {
	CG_FRAME(    0,    0,    4,           0,   787, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    4,           0,   788, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   789, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl10_damage_27 ),
};

/* Char_Damage 31: scene_end, act_down, dm_dizzy_down */
const Sint32 pl10_damage_31[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   786, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl10_damage_31 ),
};

/* Char_Damage 34: dm_snap_fly, dm_snap_down */
const Sint32 pl10_damage_34[] = {
	CG_FRAME(    0,    0,    4,           0,   782, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    4,           0,   784, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   785, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl10_damage_34 ),
};

/* Char_Damage 28: dm_away_init, dm_bound_fly, dm_piyo, dm_away */
const Sint32 pl10_damage_28[] = {
	CG_FRAME(    0,    0,    4,           0,   790, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   791, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   792, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   793, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   794, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   795, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl10_damage_28 ),
};

/* Char_Damage 29: dm_away_fly, dm_bound_hop, dm_bound, caught_spin_fly, ... */
const Sint32 pl10_damage_29[] = {
	CG_FRAME(    0,    0,    4,           0,   796, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   797, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   798, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   799, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   800, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   801, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl10_damage_29 ),
};

/* Char_Caught 0 */
const Sint32 pl10_caught_00[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   753, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl10_caught_00 ),
};

/* Char_Caught 1 */
const Sint32 pl10_caught_01[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   754, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl10_caught_01 ),
};

/* Char_Caught 2 */
const Sint32 pl10_caught_02[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   754, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl10_caught_02 ),
};

/* Char_Caught 3 */
const Sint32 pl10_caught_03[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   757, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl10_caught_03 ),
};

/* Char_Caught 4 */
const Sint32 pl10_caught_04[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   758, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl10_caught_04 ),
};

/* Char_Caught 5 */
const Sint32 pl10_caught_05[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   759, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl10_caught_05 ),
};

/* Char_Caught 6 */
const Sint32 pl10_caught_06[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   760, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_NEXT( pl10_caught_06 ),
};

/* Char_Caught 7 */
const Sint32 pl10_caught_07[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   761, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_NEXT( pl10_caught_07 ),
};

/* Char_Caught 8 */
const Sint32 pl10_caught_08[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   762, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_NEXT( pl10_caught_08 ),
};

/* Char_Caught 9 */
const Sint32 pl10_caught_09[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   764, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_NEXT( pl10_caught_09 ),
};

/* Char_Caught 10 */
const Sint32 pl10_caught_10[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   765, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_NEXT( pl10_caught_10 ),
};

/* Char_Caught 11 */
const Sint32 pl10_caught_11[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   766, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_NEXT( pl10_caught_11 ),
};

/* Char_Caught 12 */
const Sint32 pl10_caught_12[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   784, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_NEXT( pl10_caught_12 ),
};

/* Char_Caught 13 */
const Sint32 pl10_caught_13[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   773, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_NEXT( pl10_caught_13 ),
};

/* Char_Caught 14 */
const Sint32 pl10_caught_14[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   775, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_NEXT( pl10_caught_14 ),
};

/* Char_Caught 15 */
const Sint32 pl10_caught_15[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   776, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_NEXT( pl10_caught_15 ),
};

/* Char_Caught 16 */
const Sint32 pl10_caught_16[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   777, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_NEXT( pl10_caught_16 ),
};

/* Char_Caught 17 */
const Sint32 pl10_caught_17[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   778, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_NEXT( pl10_caught_17 ),
};

/* Char_Caught 18 */
const Sint32 pl10_caught_18[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   790, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_NEXT( pl10_caught_18 ),
};

/* Char_Caught 19 */
const Sint32 pl10_caught_19[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   796, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_NEXT( pl10_caught_19 ),
};

/* Char_Caught 20 */
const Sint32 pl10_caught_20[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   786, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_NEXT( pl10_caught_20 ),
};

/* Char_Caught 21 */
const Sint32 pl10_caught_21[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   772, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  15 ),
	CG_NEXT( pl10_caught_21 ),
};

/* Char_Dizzy 0: shl10_04_catch_hit, act_dizzy */
const Sint32 pl10_dizzy_00[] = {
	CG_FRAME(    0,    0,    8,           0,   752, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   753, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,   754, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   753, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl10_dizzy_00 ),
};

/* Char_Catch 0: nml_p_set */
const Sint32 pl10_catch_00[] = {
	CG_FRAME(    0,    0,    4,           0,   620, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0001,   1 ),
	CG_FRAME(    0,    0,    4,           0,   621, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0002,   1 ),
	CG_FRAME(    1,    0,    4,           0,   622, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0003,   1 ),
	CG_FRAME(    0,    0,    4,           0,   623, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0004,   1 ),
	CG_FRAME(    0,    0,    4,           0,   624, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0005,   1 ),
	CG_FRAME(    0,    0,    4,           0,   625, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0006,   1 ),
	CG_FRAME(    0,    0,    4,           0,   626, 0x80,   0x00,    0,  0x00,       0x04,    0,   0,     0, 0x0007,   1 ),
	CG_FRAME(    0,    1,    4,           0,   627, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   628, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   324, 0x90,   0x00,  ATE,  0x00,       0x20,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   323, 0x90,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl10_catch_00 ),
};

/* Char_Catch 1: jump_p_set */
const Sint32 pl10_catch_01[] = {
	CG_FRAME(    0,    0,    4,           0,   637, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0008,   3 ),
	CG_FRAME(    0,    0,    4,           0,   638, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0009,   3 ),
	CG_FRAME(    1,    0,    4,           0,   639, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x000a,   3 ),
	CG_FRAME(    0,    0,    4,           0,   640, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x000b,   3 ),
	CG_FRAME(    0,    0,    4,           0,   641, 0x80,   0x00,    0,  0x00,       0x04,    0,   0,     0, 0x000c,   3 ),
	CG_FRAME(    0,    1,    4,           0,   642, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   643, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl10_catch_01 ),
};

/* Char_Catch 2: throw_init (a throw (assist)) */
const Sint32 pl10_catch_02[] = {
	CG_FRAME(    0,    0,    4,           0,   620, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    3,           0,   621, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,    1,           0,   621, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  64 ),
};

/* Char_Catch 4 */
const Sint32 pl10_catch_04[] = {
	CG_FRAME(    0,    0,    4,           0,   629, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    4,           0,   630, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    4,           0,   631, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    4,           0,   632, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    4,           0,   633, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    4,           0,   634, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    4,           0,   635, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    4,           0,   636, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   24, END_OF_DATA,   635, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl10_catch_04 ),
};

/* Char_Catch 3: throw_set */
const Sint32 pl10_catch_03[] = {
	CG_FRAME(    0,    0,    4,           0,   622, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0003,   5 ),
	CG_FRAME(    0,    0,    4,           0,   623, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0004,   5 ),
	CG_FRAME(    0,    0,    4,           0,   624, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0005,   5 ),
	CG_FRAME(    0,    0,    4,           0,   625, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0006,   5 ),
	CG_FRAME(    0,    0,    4,           0,   626, 0x80,   0x00,    0,  0x00,       0x03,    0,   0,     0, 0x0007,   5 ),
	CG_FRAME(    0,    1,    4,           0,   627, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   628, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl10_catch_03 ),
};

/* Char_Win 0: win_ko_init */
const Sint32 pl10_win_00[] = {
	CG_FRAME(    0,    0,    4,           0,   674, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   438, 0x80,   0x00,    0,  0x00,       0x0f,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   439, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   20,           0,   440, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   441, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl10_win_00_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   442, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl10_win_00_loop ),
};

/* Char_Win 1
   Char_Win 2: lose_init */
const Sint32 pl10_win_01[] = {
	CG_FRAME(    0,    1,   10,           0,   261, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl10_win_01_loop[] = {
	CG_FRAME(    0,    0,    4,           0,   392, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   394, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   395, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   396, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   397, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   394, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   395, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   396, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   397, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   394, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   16,           0,   392, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   393, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   394, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   16,           0,   392, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   393, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   394, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   392, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   393, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   394, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl10_win_01_loop ),
};

/* Char_Win 3: scene_stop */
const Sint32 pl10_win_03[] = {
	CG_FRAME(    0,    0,    4,           0,   261, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   262, 0x80,   0x00,    0,  0x00,       0x1c,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   263, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    4,           0,   264, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   265, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   266, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   267, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   268, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   32,           0,   444, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   445, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   446, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl10_win_03_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   445, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl10_win_03_loop ),
};

/* Char_Win 4: scene_wait */
const Sint32 pl10_win_04[] = {
	CG_FRAME(    0,    0,    3,           0,   767, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   768, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   769, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   770, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   771, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   772, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   783, 0x80,   0x00,    0,  0x00, 0x2c | CSE,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   784, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   785, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    8, END_OF_DATA,   786, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl10_win_04 ),
};

/* Char_Appear 0: appear_init */
const Sint32 pl10_appear_00[] = {
	CG_FRAME(    0,    1,    4,           0,   409, 0x80,   0x8a,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    4, END_OF_DATA,   410, 0x80,   0x8a,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl10_appear_00 ),
};

/* Char_Appear 1: appear_move */
const Sint32 pl10_appear_01[] = {
	CG_FRAME(    0,    1,    4, END_OF_DATA,   411, 0x80,   0x8a,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl10_appear_01 ),
};

/* Char_Appear 2: appear_stop */
const Sint32 pl10_appear_02[] = {
	CG_FRAME(    0,    1,   16,           0,   412, 0x80,   0x8a,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    4,           0,   413, 0x80,   0x8a,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    4,           0,   414, 0x80,   0x8a,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    4,           0,   415, 0x80,   0x8a,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    8,           0,   416, 0x80,   0x8a,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    4,           0,   417, 0x80,   0x8a,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   80,           0,   418, 0x80,   0x8a,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    6,           0,   419, 0x80,   0x8a,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   673, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   674, 0x80,   0x00,    0,  0x00,       0x1b,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl10_appear_02 ),
};

/* Char_Extra 0: t_kick_init */
const Sint32 pl10_extra_00[] = {
	CG_FRAME(    0,    0,    4,           0,   617, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
};

const Sint32 pl10_extra_00_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   618, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  44 ),
	CG_NEXT( pl10_extra_00_loop ),
};

/* Char_Extra 1: t_kick_jump */
const Sint32 pl10_extra_01[] = {
	CG_FRAME(    0,    0,    3,           0,   376, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   377, 0x80,   0x00,  ATE,  0x00,       0x0c,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,   378, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   379, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   378, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   381, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   380, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   378, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   380, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   381, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   380, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   381, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   379, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   382, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl10_extra_01 ),
};

/* Char_Extra 2: chouhatsu_init (the taunt), fly_start5 */
const Sint32 pl10_extra_02[] = {
	CG_FRAME(    0,    0,    4,           0,   420, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    6,           0,   421, 0x80,   0x00,    0,  0x00,       0x17,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    4,           0,   422, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   16,           0,   423, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    3,           0,   422, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    6,           0,   423, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    3,           0,   422, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    6,           0,   423, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    3,           0,   422, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    6,           0,   423, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    3,           0,   422, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    6,           0,   423, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    3,           0,   422, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    6,           0,   423, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    3,           0,   422, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    6,           0,   423, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    4,           0,   422, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   424, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl10_extra_02 ),
};

/* Char_Special 0 */
const Sint32 pl10_special_00[] = {
	CG_FRAME(    0,    0,    8,           0,   667, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    8,           0,   668, 0x80,   0x6e,  ATK,  0x00,       0x16,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    0,    8,           0,   669, 0x80,   0x8a,  ATK,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_FRAME(    1,    0,    4,           0,   670, 0x80,   0x8a,  ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    1,    0,    8,           0,   671, 0x80,   0x8a,  ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    1,    0,    8,           0,   670, 0x80,   0x8a,  ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    1,   16,           0,   671, 0x80,   0x8a,  ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,    6,           0,   672, 0x80,   0x8a,  ATE,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    0,    6,           0,   673, 0x80,   0x40,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   674, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl10_special_00 ),
};

/* Char_Special 1 */
const Sint32 pl10_special_01[] = {
	CG_FRAME(    0,    0,    8,           0,   667, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    8,           0,   668, 0x80,   0x6e,  ATK,  0x00,       0x16,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    0,    8,           0,   669, 0x80,   0x8a,  ATK,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_FRAME(    1,    0,    4,           0,   675, 0x80,   0x8a,  ATK,  0x00,          0,    0,   0,     0,      0,  68 ),
	CG_FRAME(    1,    0,    8,           0,   676, 0x80,   0x8a,  ATK,  0x00,          0,    0,   0,     0,      0,  68 ),
	CG_FRAME(    1,    0,    8,           0,   675, 0x80,   0x8a,  ATK,  0x00,          0,    0,   0,     0,      0,  68 ),
	CG_FRAME(    0,    1,   16,           0,   676, 0x80,   0x8a,  ATK,  0x00,          0,    0,   0,     0,      0,  68 ),
	CG_FRAME(    0,    0,    6,           0,   672, 0x80,   0x8a,  ATE,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    0,    6,           0,   673, 0x80,   0x40,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   674, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl10_special_01 ),
};

/* Char_Special 2: kobun_init (Kobun Launch) */
const Sint32 pl10_special_02[] = {
	CG_FRAME(    0,    0,    4,           0,   677, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    4,           0,   678, 0x80,   0x00,  ATK,  0x00,       0x15,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    4,           0,   679, 0x80,   0x4e,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    4,           0,   680, 0x80,   0x4e,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    4,           0,   681, 0x80,   0x4e,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,    4,           0,   682, 0x80,   0x4e,  ATK,  0x00, 0x46 | CSE,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    4,           0,   683, 0x80,   0x4e,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    8,           0,   684, 0x80,   0x4e,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    8,           0,   684, 0x80,   0x4e,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    6,           0,   468, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   674, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl10_special_02 ),
};

/* Char_Special 3: strike_init (Bonne Strike) */
const Sint32 pl10_special_03[] = {
	CG_FRAME(    0,    0,    4,           0,   644, 0x90,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    2,           0,   645, 0x90,   0x0a,  ATK,  0x00,       0x14,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    1,           0,   646, 0x90,   0x08,  ATK,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_FRAME(    1,    1,    2,           0,   647, 0x90,   0x06,  ATK,  0x00,       0x23,    0,   0,     0,      0,  70 ),
	CG_FRAME(    1,    0,    2,           0,   648, 0x90,   0x06,  ATK,  0x00,          0,    0,   0,     0,      0,  70 ),
	CG_FRAME(    1,    0,    2,           0,   649, 0x90,   0x06,  ATK,  0x00,          0,    0,   0,     0,      0,  70 ),
	CG_FRAME(    1,    0,    2,           0,   650, 0x90,   0x06,  ATK,  0x00,          0,    0,   0,     0,      0,  70 ),
	CG_FRAME(    1,    0,    2,           0,   651, 0x90,   0x06,  ATK,  0x00,          0,    0,   0,     0,      0,  70 ),
	CG_FRAME(    1,    0,    2,           0,   652, 0x90,   0x06,  ATK,  0x00,          0,    0,   0,     0,      0,  70 ),
	CG_FRAME(    1,    0,    2,           0,   653, 0x90,   0x06,  ATK,  0x00,          0,    0,   0,     0,      0,  70 ),
	CG_FRAME(    1,    0,    2,           0,   654, 0x90,   0x06,  ATK,  0x00,          0,    0,   0,     0,      0,  70 ),
	CG_FRAME(    1,    0,    2,           0,   655, 0x90,   0x06,  ATK,  0x00,          0,    0,   0,     0,      0,  70 ),
	CG_FRAME(    1,    0,    2,           0,   656, 0x90,   0x06,  ATK,  0x00,          0,    0,   0,     0,      0,  70 ),
	CG_FRAME(    1,    0,    2,           0,   657, 0x90,   0x06,  ATK,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    1,    0,    2,           0,   658, 0x90,   0x06,  ATK,  0x00,          0,    0,   0,     0,      0,  71 ),
};

const Sint32 pl10_special_03_loop[] = {
	CG_FRAME(    1,    0,    2,           0,   659, 0x90,   0x06,  ATK,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    1,    0,    2,           0,   660, 0x90,   0x06,  ATK,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    1,    0,    2,           0,   661, 0x90,   0x06,  ATK,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    1,    0,    2,           0,   662, 0x90,   0x06,  ATK,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    1,    0,    2,           0,   663, 0x90,   0x06,  ATK,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    1,    0,    2, END_OF_DATA,   664, 0x90,   0x06,  ATK,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_NEXT( pl10_special_03_loop ),
};

/* Char_Special 4: strike_rush (Bonne Strike) */
const Sint32 pl10_special_04[] = {
	CG_FRAME(    0,    0,   28,           0,   665, 0x90,   0x06,  ATE,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,    4,           0,   666, 0x90,   0x06,  ATE,  0x00,          0,    0,   0,     0,      0,  69 ),
};

const Sint32 pl10_special_04_loop[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   261, 0x90,   0x00,  ATE,  0x00,          0,    0,   0,     0, 0xffe0,   5 ),
	CG_NEXT( pl10_special_04_loop ),
};

/* Char_Special 5: strike_air_init (Bonne Strike) */
const Sint32 pl10_special_05[] = {
	CG_FRAME(    0,    0,    6,           0,   729, 0x90,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    3,           0,   730, 0x90,   0x00,  ATK,  0x00,       0x14,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    2,           0,   731, 0x90,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(    1,    1,    2,           0,   732, 0x90,   0x00,  ATK,  0x00,       0x23,    0,   0,     0,      0,  73 ),
	CG_FRAME(    1,    0,    2,           0,   733, 0x90,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    1,    0,    2,           0,   734, 0x90,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    1,    0,    2,           0,   735, 0x90,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    1,    0,    2,           0,   736, 0x90,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    1,    0,    2,           0,   737, 0x90,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    1,    0,    2,           0,   738, 0x90,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    1,    0,    2,           0,   739, 0x90,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    1,    0,    2,           0,   740, 0x90,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    1,    0,    2,           0,   741, 0x90,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    1,    0,    2,           0,   742, 0x90,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    1,    0,    2,           0,   743, 0x90,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  74 ),
};

const Sint32 pl10_special_05_loop[] = {
	CG_FRAME(    1,    0,    2,           0,   744, 0x90,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    1,    0,    2,           0,   745, 0x90,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    1,    0,    2,           0,   746, 0x90,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    1,    0,    2,           0,   747, 0x90,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    1,    0,    2,           0,   748, 0x90,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    1,    0,    2, END_OF_DATA,   749, 0x90,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_NEXT( pl10_special_05_loop ),
};

/* Char_Special 6: strike_air_rush (Bonne Strike) */
const Sint32 pl10_special_06[] = {
	CG_FRAME(    0,    0,   16,           0,   750, 0x90,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  72 ),
};

const Sint32 pl10_special_06_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   751, 0x90,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_NEXT( pl10_special_06_loop ),
};

/* Char_Special 7: snapback_init (the snapback) */
const Sint32 pl10_special_07[] = {
	CG_FRAME(    0,    0,    6,           0,   508, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    2,           0,   509, 0x80,   0x4d,  ATK,  0x00,       0x23,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,    2,           0,   510, 0x80,   0x4a,  ATK,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    3,           0,   511, 0x80,   0x4a,  ATK,  0x00, 0x16 | CSE,    0,   0,     0,      0,  32 ),
	CG_FRAME(    0,    0,    3,           0,   512, 0x80,   0x4a,  ATK,  0x00,          0,    0,   0,     0,      0,  33 ),
	CG_FRAME(    0,    0,    3,           0,   513, 0x80,   0x4a,  ATK,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    3,           0,   514, 0x80,   0x4a,  ATK,  0x00, 0x16 | CSE,    0,   0,     0,      0,  32 ),
	CG_FRAME(    0,    0,    3,           0,   515, 0x80,   0x4a,  ATK,  0x00,          0,    0,   0,     0,      0,  33 ),
	CG_FRAME(    0,    0,    3,           0,   517, 0x80,   0x4a,  ATK,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    7,           0,   511, 0x80,   0x4a,  ATE,  0x00, 0x16 | CSE,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,   16,           0,   512, 0x80,   0x4a,  ATE,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,    8,           0,   518, 0x80,   0x4a,  ATE,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,    4,           0,   516, 0x80,   0x07,  ATE,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,    3,           0,   519, 0x80,   0x07,  ATE,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA,   261, 0x90,   0x00,  ATE,  0x00,       0x1e,    0,   0,     0, 0xffe6,   5 ),
	CG_NEXT( pl10_special_07 ),
};

/* Char_Special 8: kick_init (a kick (assist)) */
const Sint32 pl10_special_08[] = {
	CG_FRAME(    0,    0,    8,           0,   504, 0x90,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,   32,           0,   505, 0x90,   0x00,  ATK,  0x00, 0x4b | CSE,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    0,    8,           0,   506, 0x90,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    8, END_OF_DATA,   507, 0x90,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl10_special_08 ),
};

/* Char_Special 9: fire_init (Gustaff's fire (assist)) */
const Sint32 pl10_special_09[] = {
	CG_FRAME(    0,    0,    4,           0,   524, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    4,           0,   525, 0x80,   0x00,  ATK,  0x00,       0x22,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    1,    4,           0,   526, 0x80,   0x00,  ATK,  0x00,       0x2d,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    1,    4,           0,   527, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    1,   16,           0,   528, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    8,           0,   529, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    8,           0,   530, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    8, END_OF_DATA,   531, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl10_special_09 ),
};

/* Char_Super 0: king_init (King Kobun (hyper)) */
const Sint32 pl10_super_00[] = {
	CG_FRAME(    0,    0,    4,           0,   383, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   384, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   385, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME( 0x80,    1,    4,           0,   386, 0x80,   0x00,  ATK,  0x00,       0x18,    0,   0,     0,      0,   1 ),
	CG_FRAME( 0x80,    1,    4,           0,   387, 0x80,   0x00,  ATK,  0x00,       0x2d,    0,   0,     0,      0,   1 ),
	CG_FRAME( 0x80,    1,    4,           0,   386, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME( 0x80,    1,    4,           0,   387, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME( 0x80,    1,    4,           0,   388, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME( 0x80,    1,    4,           0,   387, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME( 0x80,    1,    4,           0,   386, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME( 0x80,    1,    4,           0,   387, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1, -128,           0,   389, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   390, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   391, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   674, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl10_super_00 ),
};

/* Char_Super 1: lunch_init (Lunch Rush (hyper)) */
const Sint32 pl10_super_01[] = {
	CG_FRAME(    0,    0,    4,           0,   667, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   668, 0x80,   0x6e,  ATK,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    1,    8,           0,   669, 0x80,   0x8a,  ATK,  0x00,       0x18,    0,   0,     0,      0,  65 ),
	CG_FRAME(    1,    0,    2,           0,   670, 0x80,   0x8a,  ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    1,    0,    6,           0,   671, 0x80,   0x8a,  ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    1,    4,           0,   670, 0x80,   0x8a,  ATK,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    2,    4,           0,   671, 0x80,   0x8a,  ATE,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   80,           0,   672, 0x80,   0x8a,  ATE,  0x00,       0x1c,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    0,    6,           0,   673, 0x80,   0x40,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   674, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl10_super_01 ),
};

/* Char_Effect 12: pls10_03_scene_walk_init, pls10_03_walk_init */
const Sint32 pl10_effect_12[] = {
	CG_FRAME(    0,    0,    4,           0,   861, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   862, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   863, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   864, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   865, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   866, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   867, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   868, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl10_effect_12 ),
};

/* Char_Effect 13: pls10_03_leave_init, pls10_03_lunch_init, pls10_03_king_init, pls10_03_enter_init, ... */
const Sint32 pl10_effect_13[] = {
	CG_FRAME(    0,    0,    2,           0,   861, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   862, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   863, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   864, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   865, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   866, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   867, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   868, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl10_effect_13 ),
};

/* Char_Effect 17: pls10_03_appear_init, pls10_03_appear_wait */
const Sint32 pl10_effect_17[] = {
	CG_FRAME(    0,    0,    4,           0,   911, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   912, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   899, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   900, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   901, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl10_effect_17_loop[] = {
	CG_FRAME(    0,    0,    4,           0,   902, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   903, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl10_effect_17_loop ),
};

/* Char_Effect 22: pls10_03_appear_jump */
const Sint32 pl10_effect_22[] = {
	CG_FRAME(    0,    0,    4,           0,   904, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   905, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   906, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   907, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   908, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   909, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   910, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   833, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl10_effect_22 ),
};

/* Char_Effect 0: pls10_03_init, pls10_03_wait, pls10_03_king_run, pls10_03_scene_stand_init, ... */
const Sint32 pl10_effect_00[] = {
	CG_FRAME(    0,    0,    6,           0,   833, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   834, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   835, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   836, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   835, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   834, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   833, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   837, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   838, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   839, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   838, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   837, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl10_effect_00 ),
};

/* Char_Effect 1 */
const Sint32 pl10_effect_01[] = {
	CG_FRAME(    0,    0,    6,           0,   840, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   841, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   842, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   841, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   840, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   841, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   842, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   841, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   843, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   844, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   845, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   846, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   847, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   848, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   849, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   850, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   843, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   844, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   845, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   846, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   847, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   848, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   849, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   850, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl10_effect_01 ),
};

/* Char_Effect 2 */
const Sint32 pl10_effect_02[] = {
	CG_FRAME(    0,    0,    6,           0,   855, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   854, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   853, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   852, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   851, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   852, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   851, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   852, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   851, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   852, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   853, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   854, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   855, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   856, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   857, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   858, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   859, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   860, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   859, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   860, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   859, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   860, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   859, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   858, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   857, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   856, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl10_effect_02 ),
};

/* Char_Effect 14: pls10_03_turn_init, shl10_01_look, shl10_01_safe_look */
const Sint32 pl10_effect_14[] = {
	CG_FRAME(    0,    0,    4,           0,   893, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   894, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   895, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl10_effect_14 ),
};

/* Char_Effect 3 */
const Sint32 pl10_effect_03[] = {
	CG_FRAME(    0,    1,    4,           0,   896, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl10_effect_03_loop[] = {
	CG_FRAME(    0,    0,    4,           0,   897, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   898, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl10_effect_03_loop ),
};

/* Char_Effect 6
   Char_Effect 7: pls10_03_jump_up (7 + jump_dir) */
const Sint32 pl10_effect_06[] = {
	CG_FRAME(    0,    0,    4,           0,   899, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   900, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   901, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl10_effect_06_loop[] = {
	CG_FRAME(    0,    0,    4,           0,   902, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   903, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl10_effect_06_loop ),
};

/* Char_Effect 9
   Char_Effect 10: pls10_03_jump_fall (10 + jump_dir) */
const Sint32 pl10_effect_09[] = {
	CG_FRAME(    0,    0,    4,           0,   904, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   905, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   906, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   907, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   908, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   909, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl10_effect_09_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   910, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl10_effect_09_loop ),
};

/* Char_Effect 4: pls10_03_jump_init (4 + jump_dir) */
const Sint32 pl10_effect_04[] = {
	CG_FRAME(    0,    1,    4,           0,   896, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl10_effect_04_loop[] = {
	CG_FRAME(    0,    0,    4,           0,   911, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   912, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl10_effect_04_loop ),
};

/* Char_Effect 5 */
const Sint32 pl10_effect_05[] = {
	CG_FRAME(    0,    1,    4,           0,   913, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl10_effect_05_loop[] = {
	CG_FRAME(    0,    0,    4,           0,   914, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   915, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl10_effect_05_loop ),
};

/* Char_Effect 8 */
const Sint32 pl10_effect_08[] = {
	CG_FRAME(    0,    0,    4,           0,   916, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   917, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   918, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl10_effect_08_loop[] = {
	CG_FRAME(    0,    0,    4,           0,   919, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   920, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl10_effect_08_loop ),
};

/* Char_Effect 11 */
const Sint32 pl10_effect_11[] = {
	CG_FRAME(    0,    0,    4,           0,   921, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   922, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   923, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   924, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   925, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   926, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl10_effect_11_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   927, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl10_effect_11_loop ),
};

/* Char_Effect 16: pls10_03_damage_init */
const Sint32 pl10_effect_16[] = {
	CG_FRAME(    0,    0,    4,           0,  1047, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,  1048, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl10_effect_16_loop[] = {
	CG_FRAME(    0,    0,    4,           0,  1049, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,  1050, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,  1049, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,  1050, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,  1049, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,  1050, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,  1049, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,  1050, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl10_effect_16_loop ),
};

/* Char_Effect 70: pls10_03_damage_move */
const Sint32 pl10_effect_70[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,  1048, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl10_effect_70 ),
};

/* Char_Effect 15: pls10_03_chouhatsu_init, pls10_03_claw_init */
const Sint32 pl10_effect_15[] = {
	CG_FRAME(    0,    0,    4,           0,  1051, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl10_effect_15_loop[] = {
	CG_FRAME(    0,    0,    4,           0,  1052, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    1,           0,  1053, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,  1054, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl10_effect_15_loop ),
};

/* Char_Effect 18: pls10_03_lose_wait */
const Sint32 pl10_effect_18[] = {
	CG_FRAME(    0,    0,    4,           0,   435, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   436, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   437, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl10_effect_18 ),
};

/* Char_Effect 27: pls10_03_win_init */
const Sint32 pl10_effect_27[] = {
	CG_FRAME(    0,    0,    4,           0,  1045, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl10_effect_27_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,  1046, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl10_effect_27_loop ),
};

/* Char_Effect 19: pls10_03_scene_stand */
const Sint32 pl10_effect_19[] = {
	CG_FRAME(    0,    0,    4,           0,   447, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   448, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   449, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   450, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   451, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   452, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   451, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   452, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   451, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   452, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   451, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   452, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   451, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   452, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   451, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl10_effect_19 ),
};

/* Char_Effect 20: pls10_03_scene_turn, pls10_04_init */
const Sint32 pl10_effect_20[] = {
	CG_FRAME(    0,    0,    4,           0, 32768, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32769, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32770, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32771, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32772, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32773, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32774, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32775, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl10_effect_20 ),
};

/* Char_Effect 21: pls10_03_scene_run, pls10_04_run */
const Sint32 pl10_effect_21[] = {
	CG_FRAME(    0,    0,    4,           0, 32776, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32777, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32778, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl10_effect_21_loop[] = {
	CG_FRAME(    0,    0,    4,           0, 32779, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32780, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32781, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32782, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl10_effect_21_loop ),
};

/* Char_Effect 25: pls10_03_change_init */
const Sint32 pl10_effect_25[] = {
	CG_FRAME(    0,    0,    4,           0,  1051, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   929, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   930, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   931, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   932, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   931, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   930, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   929, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl10_effect_25 ),
};

/* Char_Effect 26: pls10_03_enter_run */
const Sint32 pl10_effect_26[] = {
	CG_FRAME(    0,    0,    4,           0,   933, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   934, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   16,           0,   935, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   934, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   933, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl10_effect_26 ),
};

/* Char_Effect 28 */
const Sint32 pl10_effect_28[] = {
	CG_FRAME(    0,    0,    1,           0,   986, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   986, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_FRAME(    0,    0,    4,           0,   987, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0,  98 ),
};

const Sint32 pl10_effect_28_loop[] = {
	CG_FRAME(    0,    0,    4,           0,   988, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   989, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_NEXT( pl10_effect_28_loop ),
};

/* Char_Effect 29: shl10_01_fly, shl10_01_safe_fly */
const Sint32 pl10_effect_29[] = {
	CG_FRAME(    0,    0,    4,           0,   990, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_FRAME(    0,    0,    4,           0,   991, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_FRAME(    0,    0,    4,           0,   992, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_FRAME(    0,    0,    4,           0,   993, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_FRAME(    0,    0,    4,           0,   994, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_FRAME(    0,    0,    4,           0,   995, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_FRAME(    0,    0,    4,           0,   996, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_FRAME(    0,    0,    4,           0,   997, 0x80,   0xe0,  ATK,  0x00,       0x25,    0,   0,     0,      0,  99 ),
};

const Sint32 pl10_effect_29_loop[] = {
	CG_FRAME(    0,    0,    1,           0,   998, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    0,    1,           0,  1002, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    0,    1,           0,   998, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    0,    1,           0,  1003, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    0,    1,           0,   999, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    0,    1,           0,  1004, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    0,    1,           0,   999, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    0,    1,           0,  1005, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    0,    1,           0,  1000, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    0,    1,           0,  1002, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    0,    1,           0,  1000, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    0,    1,           0,  1003, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    0,    1,           0,  1001, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    0,    1,           0,  1004, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    0,    1,           0,  1001, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA,  1005, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_NEXT( pl10_effect_29_loop ),
};

/* Char_Effect 30: shl10_01_fall, shl10_01_safe_fall */
const Sint32 pl10_effect_30[] = {
	CG_FRAME(    0,    0,    4,           0,  1006, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,  1007, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,  1008, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl10_effect_30_loop[] = {
	CG_FRAME(    0,    0,    4,           0,  1009, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,  1010, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl10_effect_30_loop ),
};

/* Char_Effect 31: shl10_01_land, shl10_01_safe_land */
const Sint32 pl10_effect_31[] = {
	CG_FRAME(    0,    0,    4,           0,  1011, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,  1010, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,  1012, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,  1013, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,  1014, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   928, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   838, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   837, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl10_effect_31 ),
};

/* Char_Effect 33: pls10_03_s14_init */
const Sint32 pl10_effect_33[] = {
	CG_FRAME(    0,    0,    4,           0,   893, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   936, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   937, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   936, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   937, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   936, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   937, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   936, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   937, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   936, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   937, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   936, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   937, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   936, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   937, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   893, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl10_effect_33 ),
};

/* Char_Effect 34 */
const Sint32 pl10_effect_34[] = {
	CG_FRAME(    0,    0,    2,           0, 32801, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32802, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32803, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32804, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32805, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32806, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32807, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA, 32808, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_NEXT( pl10_effect_34 ),
};

/* Char_Effect 35 */
const Sint32 pl10_effect_35[] = {
	CG_FRAME(    0,    0,    2,           0, 32809, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32810, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32811, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32812, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32813, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32814, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32815, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA, 32816, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_NEXT( pl10_effect_35 ),
};

/* Char_Effect 36 */
const Sint32 pl10_effect_36[] = {
	CG_FRAME(    0,    0,    2,           0, 32817, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32818, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32819, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32820, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32821, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32822, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32823, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA, 32824, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_NEXT( pl10_effect_36 ),
};

/* Char_Effect 37 */
const Sint32 pl10_effect_37[] = {
	CG_FRAME(    0,    0,    2,           0, 32825, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32826, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32827, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32828, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32829, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32830, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32831, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA, 32832, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_NEXT( pl10_effect_37 ),
};

/* Char_Effect 38 */
const Sint32 pl10_effect_38[] = {
	CG_FRAME(    0,    0,    2,           0, 32833, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32834, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32835, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32836, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32837, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32838, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32839, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA, 32840, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_NEXT( pl10_effect_38 ),
};

/* Char_Effect 39 */
const Sint32 pl10_effect_39[] = {
	CG_FRAME(    0,    0,    2,           0, 32841, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32842, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32843, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32844, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32845, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32846, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32847, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA, 32848, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_NEXT( pl10_effect_39 ),
};

/* Char_Effect 40 */
const Sint32 pl10_effect_40[] = {
	CG_FRAME(    0,    0,    2,           0, 32849, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32850, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32851, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32852, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32853, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32854, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32855, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA, 32856, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_NEXT( pl10_effect_40 ),
};

/* Char_Effect 41 */
const Sint32 pl10_effect_41[] = {
	CG_FRAME(    0,    0,    2,           0, 32857, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32858, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32859, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32860, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32861, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32862, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32863, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA, 32864, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_NEXT( pl10_effect_41 ),
};

/* Char_Effect 42 */
const Sint32 pl10_effect_42[] = {
	CG_FRAME(    0,    0,    2,           0, 32865, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32866, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32867, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32868, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32869, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32870, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32871, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA, 32872, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_NEXT( pl10_effect_42 ),
};

/* Char_Effect 43 */
const Sint32 pl10_effect_43[] = {
	CG_FRAME(    0,    0,    2,           0, 32873, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32874, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32875, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32876, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32877, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32878, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32879, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA, 32880, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_NEXT( pl10_effect_43 ),
};

/* Char_Effect 44 */
const Sint32 pl10_effect_44[] = {
	CG_FRAME(    0,    0,    2,           0, 32881, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32882, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32883, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32884, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32885, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32886, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32887, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA, 32888, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_NEXT( pl10_effect_44 ),
};

/* Char_Effect 45 */
const Sint32 pl10_effect_45[] = {
	CG_FRAME(    0,    0,    2,           0, 32889, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32890, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32891, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32892, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32893, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32894, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32895, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA, 32896, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_NEXT( pl10_effect_45 ),
};

/* Char_Effect 46 */
const Sint32 pl10_effect_46[] = {
	CG_FRAME(    0,    0,    2,           0, 32897, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32898, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32899, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32900, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32901, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32902, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32903, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA, 32904, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_NEXT( pl10_effect_46 ),
};

/* Char_Effect 47 */
const Sint32 pl10_effect_47[] = {
	CG_FRAME(    0,    0,    2,           0, 32905, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32906, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32907, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32908, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32909, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32910, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32911, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA, 32912, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_NEXT( pl10_effect_47 ),
};

/* Char_Effect 48 */
const Sint32 pl10_effect_48[] = {
	CG_FRAME(    0,    0,    2,           0, 32913, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32914, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32915, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32916, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32917, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32918, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32919, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA, 32920, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_NEXT( pl10_effect_48 ),
};

/* Char_Effect 49 */
const Sint32 pl10_effect_49[] = {
	CG_FRAME(    0,    0,    2,           0, 32921, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32922, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32923, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32924, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32925, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32926, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32927, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA, 32928, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_NEXT( pl10_effect_49 ),
};

/* Char_Effect 50 */
const Sint32 pl10_effect_50[] = {
	CG_FRAME(    0,    0,    2,           0, 32929, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32930, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32931, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32932, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32933, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32934, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32935, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA, 32936, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_NEXT( pl10_effect_50 ),
};

/* Char_Effect 51 */
const Sint32 pl10_effect_51[] = {
	CG_FRAME(    0,    0,    2,           0, 32937, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32938, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32939, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32940, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32941, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32942, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32943, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA, 32944, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_NEXT( pl10_effect_51 ),
};

/* Char_Effect 52 */
const Sint32 pl10_effect_52[] = {
	CG_FRAME(    0,    0,    2,           0, 32945, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32946, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32947, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32948, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32949, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32950, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32951, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA, 32952, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_NEXT( pl10_effect_52 ),
};

/* Char_Effect 53 */
const Sint32 pl10_effect_53[] = {
	CG_FRAME(    0,    0,    2,           0, 32953, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32954, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32955, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32956, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32957, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32958, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32959, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA, 32960, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_NEXT( pl10_effect_53 ),
};

/* Char_Effect 54 */
const Sint32 pl10_effect_54[] = {
	CG_FRAME(    0,    0,    2,           0, 32961, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32962, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32963, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32964, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32965, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32966, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32967, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA, 32968, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_NEXT( pl10_effect_54 ),
};

/* Char_Effect 55 */
const Sint32 pl10_effect_55[] = {
	CG_FRAME(    0,    0,    2,           0, 32969, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32970, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32971, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32972, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32973, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32974, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32975, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA, 32976, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_NEXT( pl10_effect_55 ),
};

/* Char_Effect 56 */
const Sint32 pl10_effect_56[] = {
	CG_FRAME(    0,    0,    2,           0, 32977, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32978, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32979, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32980, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32981, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32982, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32983, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA, 32984, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_NEXT( pl10_effect_56 ),
};

/* Char_Effect 57 */
const Sint32 pl10_effect_57[] = {
	CG_FRAME(    0,    0,    2,           0, 32985, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32986, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32987, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32988, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32989, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32990, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32991, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA, 32992, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_NEXT( pl10_effect_57 ),
};

/* Char_Effect 58 */
const Sint32 pl10_effect_58[] = {
	CG_FRAME(    0,    0,    2,           0, 32993, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32994, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32995, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32996, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 32997, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA, 32998, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_NEXT( pl10_effect_58 ),
};

/* Char_Effect 59 */
const Sint32 pl10_effect_59[] = {
	CG_FRAME(    0,    0,    2,           0, 32999, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 33000, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 33001, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 33002, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 33003, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA, 33004, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_NEXT( pl10_effect_59 ),
};

/* Char_Effect 60 */
const Sint32 pl10_effect_60[] = {
	CG_FRAME(    0,    0,    2,           0, 33005, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 33006, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 33007, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 33008, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 33009, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA, 33010, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_NEXT( pl10_effect_60 ),
};

/* Char_Effect 61 */
const Sint32 pl10_effect_61[] = {
	CG_FRAME(    0,    0,    2,           0, 33011, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 33012, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 33013, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 33014, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 33015, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA, 33016, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_NEXT( pl10_effect_61 ),
};

/* Char_Effect 62 */
const Sint32 pl10_effect_62[] = {
	CG_FRAME(    0,    0,    2,           0, 33017, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 33018, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 33019, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 33020, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 33021, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA, 33022, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_NEXT( pl10_effect_62 ),
};

/* Char_Effect 63 */
const Sint32 pl10_effect_63[] = {
	CG_FRAME(    0,    0,    2,           0, 33023, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 33024, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 33025, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 33026, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0, 33027, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA, 33028, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_NEXT( pl10_effect_63 ),
};

/* Char_Effect 64: pls10_03_lunch_run */
const Sint32 pl10_effect_64[] = {
	CG_FRAME(    0,    0,    4,           0,   893, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,  1056, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,  1057, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,  1058, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,  1059, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,  1060, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,  1061, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,  1062, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,  1063, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,  1064, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl10_effect_64_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,  1065, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl10_effect_64_loop ),
};

/* Char_Effect 65: pls10_03_lunch_wait */
const Sint32 pl10_effect_65[] = {
	CG_FRAME(    0,    0,    4,           0,  1066, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,  1067, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,  1068, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,  1069, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl10_effect_65 ),
};

/* Char_Effect 66: pls10_03_lunch_keep */
const Sint32 pl10_effect_66[] = {
	CG_FRAME(    0,    0,    4,           0,  1070, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,  1071, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,  1078, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,  1079, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,  1078, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,  1079, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,  1078, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,  1079, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,  1078, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,  1079, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,  1078, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,  1079, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,  1078, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,  1079, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,  1078, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,  1079, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,  1078, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,  1079, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,  1078, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,  1079, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,  1078, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,  1079, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,  1070, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,  1071, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,  1072, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,  1073, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,  1074, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,  1080, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,  1081, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,  1082, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,  1083, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,  1080, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,  1081, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,  1082, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,  1083, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,  1080, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,  1081, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,  1082, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,  1083, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,  1080, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,  1081, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,  1082, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,  1083, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,  1080, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,  1081, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,  1082, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,  1083, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,  1075, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,  1076, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,  1077, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl10_effect_66_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   850, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl10_effect_66_loop ),
};

/* Char_Effect 67: pls10_03_king_wait */
const Sint32 pl10_effect_67[] = {
	CG_FRAME(    0,    0,    4,           0,  1051, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,    4,           0,  1052, 0x80,   0xe0,  ATK,  0x00,       0x19,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,    4,           0,  1053, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,    4,           0,  1054, 0x80,   0xe0,  ATK,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    1,    4,           0,  1084, 0x80,   0x20,  ATK,  0x00,       0x30,    0,   0,     0,      0, 102 ),
	CG_FRAME(    0,    0,    4,           0,  1085, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0, 103 ),
	CG_FRAME(    0,    0,    4,           0,  1086, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 104 ),
	CG_FRAME(    0,    0,    4,           0,  1087, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 104 ),
	CG_FRAME(    0,    0,    4,           0,  1088, 0x80,   0x40,  ATK,  0x00,          0,    0,   0,     0,      0, 104 ),
	CG_FRAME(    0,    0,    4,           0,  1089, 0x80,   0x60,  ATK,  0x00,          0,    0,   0,     0,      0, 104 ),
	CG_FRAME(    0,    0,    4,           0,  1090, 0x80,   0x80,  ATK,  0x00,          0,    0,   0,     0,      0, 104 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,  1091, 0x80,   0x80,  ATK,  0x00,          0,    0,   0,     0,      0, 104 ),
	CG_NEXT( pl10_effect_67 ),
};

/* Char_Effect 68: pls10_03_king_grow */
const Sint32 pl10_effect_68[] = {
	CG_FRAME(    0,    0,    4,           0,  1092, 0x80,   0x80,  ATK,  0x00,          0,    0,   0,     0,      0, 105 ),
	CG_FRAME(    0,    0,    4,           0,  1093, 0x80,   0x80,  ATK,  0x00,          0,    0,   0,     0,      0, 105 ),
	CG_FRAME(    0,    0,    4,           0,  1094, 0x80,   0x80,  ATK,  0x00,       0x31,    0,   0,     0,      0, 105 ),
	CG_FRAME(    0,    0,    4,           0,  1095, 0x80,   0x80,  ATK,  0x00,          0,    0,   0,     0,      0, 105 ),
	CG_FRAME(    0,    0,    4,           0,  1096, 0x80,   0x80,  ATK,  0x00,          0,    0,   0,     0,      0, 105 ),
	CG_FRAME(    0,    0,    4,           0,  1097, 0x80,   0x80,  ATK,  0x00,          0,    0,   0,     0,      0, 105 ),
	CG_FRAME(    0,    0,    4,           0,  1098, 0x80,   0x80,  ATK,  0x00,          0,    0,   0,     0,      0, 105 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,  1099, 0x80,   0x80,  ATK,  0x00,          0,    0,   0,     0,      0, 105 ),
	CG_NEXT( pl10_effect_68 ),
};

/* Char_Effect 69: pls10_03_king_rush */
const Sint32 pl10_effect_69[] = {
	CG_FRAME(    0,    0,    4,           0,  1090, 0x80,   0x80,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,  1089, 0x80,   0x80,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,  1088, 0x80,   0x60,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,  1087, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,  1086, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,  1085, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,  1084, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,  1054, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,  1053, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,  1052, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl10_effect_69_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,  1051, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl10_effect_69_loop ),
};

/* Char_Effect 71: shl10_04_bot_init (71 + sub_no) */
const Sint32 pl10_effect_71[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   975, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl10_effect_71 ),
};

/* Char_Effect 72 */
const Sint32 pl10_effect_72[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   983, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl10_effect_72 ),
};

/* Char_Effect 73: shl10_04_bot_jump (73 + sub_no) */
const Sint32 pl10_effect_73[] = {
	CG_FRAME(    0,    0,    4,           0,   976, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   977, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl10_effect_73 ),
};

/* Char_Effect 74 */
const Sint32 pl10_effect_74[] = {
	CG_FRAME(    0,    0,    4,           0,   984, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   985, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl10_effect_74 ),
};

/* Char_Effect 75 */
const Sint32 pl10_effect_75[] = {
	CG_FRAME(    0,    0,    4,           0,   902, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   903, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl10_effect_75 ),
};

/* Char_Effect 76 */
const Sint32 pl10_effect_76[] = {
	CG_FRAME(    0,    0,    4,           0,   978, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   979, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   980, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   981, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl10_effect_76 ),
};

/* Char_Effect 77 */
const Sint32 pl10_effect_77[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   904, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl10_effect_77 ),
};

/* Char_Effect 78 */
const Sint32 pl10_effect_78[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   982, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl10_effect_78 ),
};

/* Char_Effect 79: pls10_03_leave_init */
const Sint32 pl10_effect_79[] = {
	CG_FRAME(    0,    0,    2,           0,   877, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   878, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   879, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   880, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   881, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   882, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   883, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   884, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl10_effect_79 ),
};

/* Char_Effect 80 */
const Sint32 pl10_effect_80[] = {
	CG_FRAME(    0,    0,    2,           0,   885, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   886, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   887, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   888, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   889, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   890, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   891, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   892, 0x80,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl10_effect_80 ),
};

/* Char_Effect 23 */
const Sint32 pl10_effect_23[] = {
	CG_FRAME(    0,    0,    4,           0, 32783, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32785, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_NEXT( pl10_effect_23 ),
};

/* Char_Effect 24 */
const Sint32 pl10_effect_24[] = {
	CG_FRAME(    0,    0,    4,           0, 32784, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32786, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl10_effect_24 ),
};

/* Char_Effect 32 */
const Sint32 pl10_effect_32[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,  1100, 0x90,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_NEXT( pl10_effect_32 ),
};

/* Char_Effect 81: shl10_04_catch_init */
const Sint32 pl10_effect_81[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   724, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  95 ),
	CG_NEXT( pl10_effect_81 ),
};

/* Char_Change 0: act_change_tag_init */
const Sint32 pl10_change_00[] = {
	CG_FRAME(    0,    0,    8,           0,   373, 0x80,   0x00,    0,  0x00,       0x0b,    1,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    9,           0,   374, 0x80,   0x52,    0,  0x00,       0x1e,    0,   0,     0,      0,   1 ),
};

const Sint32 pl10_change_00_loop[] = {
	CG_FRAME(    0,    0,    9, END_OF_DATA,   375, 0x80,   0x52,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl10_change_00_loop ),
};

/* Char_Change 1: act_change_vcounter_init */
const Sint32 pl10_change_01[] = {
	CG_FRAME(    0,    0,   10, END_OF_DATA,   372, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl10_change_01 ),
};

/* Char_Change 2: act_change_out_bound */
const Sint32 pl10_change_02[] = {
	CG_FRAME(    0,    0,    1,           0,   325, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl10_change_02_loop[] = {
	CG_FRAME(    0,    0,   -1, END_OF_DATA,   328, 0x80,   0x00,    0,  0x00,          0,    1,   0,     0,      0,   3 ),
	CG_NEXT( pl10_change_02_loop ),
};

/* Char_Change 3: act_stand, act_change_call */
const Sint32 pl10_change_03[] = {
	CG_FRAME(    0,    0,    4,           0,   383, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   384, 0x80,   0x00,    0,  0x00,       0x0d,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   385, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   386, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   387, 0x80,   0x00,    0,  0x00,       0x2d,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   386, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   387, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   388, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   387, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   386, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   387, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   389, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   390, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   391, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   674, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl10_change_03 ),
};

/* Char_Change 4: act_change_assist_end */
const Sint32 pl10_change_04[] = {
	CG_FRAME(    0,    0,    3,           0,   420, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,   421, 0x80,   0x00,    0,  0x00,       0x17,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   422, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    7,           0,   423, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   422, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   423, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   422, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   423, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   422, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   424, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl10_change_04 ),
};

/* group 27 0 */
const Sint32 pl10_grp27_00[] = {
	EF_FRAME(    0,    0,    4,           0,   407 ),
};

/* group 27 1 */
const Sint32 pl10_grp27_01[] = {
	EF_FRAME(    0,    0,    4,           0,   408 ),
};

/* group 27 2 */
const Sint32 pl10_grp27_02[] = {
	EF_FRAME(    0,    0,    4,           0,   398 ),
};

/* group 27 3 */
const Sint32 pl10_grp27_03[] = {
	EF_FRAME(    0,    0,    4,           0,   399 ),
};

/* group 27 4 */
const Sint32 pl10_grp27_04[] = {
	EF_FRAME(    0,    0,    4,           0,   400 ),
};

/* group 27 5 */
const Sint32 pl10_grp27_05[] = {
	EF_FRAME(    0,    0,    4,           0,   401 ),
};

/* group 27 6 */
const Sint32 pl10_grp27_06[] = {
	EF_FRAME(    0,    0,    4,           0,   402 ),
};

/* group 27 7 */
const Sint32 pl10_grp27_07[] = {
	EF_FRAME(    0,    0,    4,           0,   403 ),
};

/* group 27 8 */
const Sint32 pl10_grp27_08[] = {
	EF_FRAME(    0,    0,    4,           0,   406 ),
};

/* group 27 9 */
const Sint32 pl10_grp27_09[] = {
	EF_FRAME(    0,    0,    4,           0,   404 ),
};

/* group 27 10 */
const Sint32 pl10_grp27_10[] = {
	EF_FRAME(    0,    0,    4, END_OF_DATA,   405 ),
	CG_NEXT( pl10_grp27_00 ),
};

/* group 27 11: pls10_01_bound_init, pls10_01_bound_third */
const Sint32 pl10_grp27_11[] = {
	EF_FRAME(    0,    0,    4,           0,   425 ),
	EF_FRAME(    0,    0,    4,           0,   426 ),
	EF_FRAME(    0,    0,    4,           0,   427 ),
	EF_FRAME(    0,    0,    4,           0,   428 ),
	EF_FRAME(    0,    0,    4,           0,   429 ),
	EF_FRAME(    0,    0,    4,           0,   430 ),
	EF_FRAME(    0,    0,    4,           0,   431 ),
	EF_FRAME(    0,    0,    4, END_OF_DATA,   432 ),
	CG_NEXT( pl10_grp27_11 ),
};

/* group 27 12: pls10_01_bound_wait
   group 27 13 */
const Sint32 pl10_grp27_12[] = {
	EF_FRAME(    0,    0,    4,           0,   433 ),
	EF_FRAME(    0,    0,    4,           0,   434 ),
	EF_FRAME(    0,    0,    4,           0,   433 ),
	EF_FRAME(    0,    0,    2,           0,   434 ),
	EF_FRAME(    0,    0,    2,           0,   433 ),
};

const Sint32 pl10_grp27_12_loop[] = {
	EF_FRAME(    0,    0,   -1, END_OF_DATA,   434 ),
	CG_NEXT( pl10_grp27_12_loop ),
};

/* group 27 14 */
const Sint32 pl10_grp27_14[] = {
	EF_FRAME(    0,    0,    4,           0,   453 ),
	EF_FRAME(    0,    0,    4,           0,   454 ),
	EF_FRAME(    0,    0,    4,           0,   455 ),
	EF_FRAME(    0,    0,    4, END_OF_DATA,   456 ),
	CG_NEXT( pl10_grp27_14 ),
};

/* group 27 15 */
const Sint32 pl10_grp27_15[] = {
	EF_FRAME(    0,    0,    2,           0,   806 ),
	EF_FRAME(    0,    0,    2,           0,   807 ),
	EF_FRAME(    0,    0,    2,           0,   808 ),
	EF_FRAME(    0,    0,    2,           0,   809 ),
	EF_FRAME(    0,    0,    2,           0,   810 ),
	EF_FRAME(    0,    0,    2,           0,   811 ),
	EF_FRAME(    0,    0,    2,           0,   812 ),
	EF_FRAME(    0,    0,    2,           0,   813 ),
	EF_FRAME(    0,    0,    2, END_OF_DATA,   814 ),
	CG_NEXT( pl10_grp27_15 ),
};

/* group 27 16 */
const Sint32 pl10_grp27_16[] = {
	EF_FRAME(    0,    0,    4,           0,   716 ),
	EF_FRAME(    0,    0,    4,           0,   717 ),
	EF_FRAME(    0,    0,    4,           0,   718 ),
	EF_FRAME(    0,    0,    4, END_OF_DATA,   719 ),
	CG_NEXT( pl10_grp27_16 ),
};

/* group 27 17 */
const Sint32 pl10_grp27_17[] = {
	EF_FRAME(    0,    0,    4,           0,   720 ),
	EF_FRAME(    0,    0,    4,           0,   721 ),
	EF_FRAME(    0,    0,    4,           0,   722 ),
	EF_FRAME(    0,    0,    4, END_OF_DATA,   723 ),
	CG_NEXT( pl10_grp27_17 ),
};

/* group 27 18 */
const Sint32 pl10_grp27_18[] = {
	EF_FRAME(    0,    0,    1,           0,   693 ),
	EF_FRAME(    0,    0,    1,           0,   694 ),
	EF_FRAME(    0,    0,    1,           0,   695 ),
	EF_FRAME(    0,    0,    1,           0,   696 ),
	EF_FRAME(    0,    0,    1,           0,   697 ),
	EF_FRAME(    0,    0,    1,           0,   698 ),
	EF_FRAME(    0,    0,    1,           0,    -1 ),
	EF_FRAME(    0,    0,    1, END_OF_DATA,    -1 ),
	CG_NEXT( pl10_grp27_18 ),
};

/* group 27 19 */
const Sint32 pl10_grp27_19[] = {
	EF_FRAME(    0,    0,    1,           0,   685 ),
	EF_FRAME(    0,    0,    1,           0,   686 ),
	EF_FRAME(    0,    0,    1,           0,   687 ),
	EF_FRAME(    0,    0,    1,           0,   688 ),
	EF_FRAME(    0,    0,    1,           0,   689 ),
	EF_FRAME(    0,    0,    1,           0,   690 ),
	EF_FRAME(    0,    0,    1,           0,   691 ),
	EF_FRAME(    0,    0,    1, END_OF_DATA,   692 ),
	CG_NEXT( pl10_grp27_19 ),
};

/* group 27 20 */
const Sint32 pl10_grp27_20[] = {
	EF_FRAME(    0,    0,    1,           0,    -1 ),
	EF_FRAME(    0,    0,    1,           0,    -1 ),
	EF_FRAME(    0,    0,    1,           0,    -1 ),
	EF_FRAME(    0,    0,    1,           0,   699 ),
	EF_FRAME(    0,    0,    1,           0,   700 ),
	EF_FRAME(    0,    0,    1,           0,   701 ),
	EF_FRAME(    0,    0,    1,           0,   702 ),
	EF_FRAME(    0,    0,    1, END_OF_DATA,   703 ),
	CG_NEXT( pl10_grp27_20 ),
};

/* group 27 21 */
const Sint32 pl10_grp27_21[] = {
	EF_FRAME(    0,    0,    1,           0,    -1 ),
	EF_FRAME(    0,    0,    1,           0,   710 ),
	EF_FRAME(    0,    0,    1,           0,   711 ),
	EF_FRAME(    0,    0,    1,           0,   712 ),
	EF_FRAME(    0,    0,    1,           0,    -1 ),
	EF_FRAME(    0,    0,    1, END_OF_DATA,    -1 ),
	CG_NEXT( pl10_grp27_21 ),
};

/* group 27 22 */
const Sint32 pl10_grp27_22[] = {
	EF_FRAME(    0,    0,    1,           0,   704 ),
	EF_FRAME(    0,    0,    1,           0,   705 ),
	EF_FRAME(    0,    0,    1,           0,   706 ),
	EF_FRAME(    0,    0,    1,           0,   707 ),
	EF_FRAME(    0,    0,    1,           0,   708 ),
	EF_FRAME(    0,    0,    1, END_OF_DATA,   709 ),
	CG_NEXT( pl10_grp27_22 ),
};

/* group 27 23 */
const Sint32 pl10_grp27_23[] = {
	EF_FRAME(    0,    0,    1,           0,    -1 ),
	EF_FRAME(    0,    0,    1,           0,   713 ),
	EF_FRAME(    0,    0,    1,           0,   714 ),
	EF_FRAME(    0,    0,    1,           0,   715 ),
	EF_FRAME(    0,    0,    1,           0,    -1 ),
	EF_FRAME(    0,    0,    1, END_OF_DATA,    -1 ),
	CG_NEXT( pl10_grp27_23 ),
};

/* group 27 24 */
const Sint32 pl10_grp27_24[] = {
	EF_FRAME(    0,    0,    2,           0,   725 ),
};

/* group 27 25 */
const Sint32 pl10_grp27_25[] = {
	EF_FRAME(    0,    0,    2,           0,   726 ),
};

/* group 27 26 */
const Sint32 pl10_grp27_26[] = {
	EF_FRAME(    0,    0,    2,           0,   727 ),
};

/* group 27 27 */
const Sint32 pl10_grp27_27[] = {
	EF_FRAME(    0,    0,    2, END_OF_DATA,   728 ),
	CG_NEXT( pl10_grp27_24 ),
};

/* group 27 28 */
const Sint32 pl10_grp27_28[] = {
	EF_FRAME(    0,    0,    2,           0,  1101 ),
};

/* group 27 29 */
const Sint32 pl10_grp27_29[] = {
	EF_FRAME(    0,    0,    2,           0,  1102 ),
};

/* group 27 30 */
const Sint32 pl10_grp27_30[] = {
	EF_FRAME(    0,    0,    2,           0,  1103 ),
};

/* group 27 31 */
const Sint32 pl10_grp27_31[] = {
	EF_FRAME(    0,    0,    2,           0,  1104 ),
};

/* group 27 32 */
const Sint32 pl10_grp27_32[] = {
	EF_FRAME(    0,    0,    2,           0,  1105 ),
};

/* group 27 33 */
const Sint32 pl10_grp27_33[] = {
	EF_FRAME(    0,    0,    2, END_OF_DATA,  1106 ),
	CG_NEXT( pl10_grp27_33 ),
};

/* group 27 34 */
const Sint32 pl10_grp27_34[] = {
	EF_FRAME(    0,    0,    2,           0, 32787 ),
	EF_FRAME(    0,    0,    2,           0, 32788 ),
	EF_FRAME(    0,    0,    2,           0, 32789 ),
	EF_FRAME(    0,    0,    2,           0, 32790 ),
};

const Sint32 pl10_grp27_34_loop[] = {
	EF_FRAME(    0,    0,    2,           0, 32791 ),
	EF_FRAME(    0,    0,    2,           0, 32792 ),
	EF_FRAME(    0,    0,    2,           0, 32793 ),
	EF_FRAME(    0,    0,    2,           0, 32794 ),
	EF_FRAME(    0,    0,    2,           0, 32795 ),
	EF_FRAME(    0,    0,    2, END_OF_DATA, 32796 ),
	CG_NEXT( pl10_grp27_34_loop ),
};

/* group 27 35: pls10_00_kick_move */
const Sint32 pl10_grp27_35[] = {
	EF_FRAME(    0,    0,    2,           0, 32797 ),
	EF_FRAME(    0,    0,    2,           0, 32798 ),
	EF_FRAME(    0,    0,    2,           0, 32799 ),
	EF_FRAME(    0,    0,    2, END_OF_DATA, 32800 ),
	CG_NEXT( pl10_grp27_35 ),
};

/* group 27 36 */
const Sint32 pl10_grp27_36[] = {
	EF_FRAME(    0,    0,    1,           0,   829 ),
	EF_FRAME(    0,    0,    1,           0,   830 ),
	EF_FRAME(    0,    0,    1,           0,   831 ),
	EF_FRAME(    0,    0,    1, END_OF_DATA,   832 ),
	CG_NEXT( pl10_grp27_36 ),
};
