/* Felicia (character 20): the animations. PLAYER_WORK's char_adrs points here.

   spr_prm_20      a sprite's sizes and turn (SPR_PRM): a frame's zoom number picks a row, 0 for none.
   char_tbl_20     the groups: pl_char_set( pl_w, group, no ) takes the group's table from here and
                   starts its animation no. The character's module (s_pl20.c) says which group
                   and number each move uses.
   pl20_GROUP_tbl  a group's animations. GROUP is the group's name in player.h (stand for
                   Char_Stand), or grpNN for a group with no name there.
   pl20_GROUP_NN   an animation, named by the first group and number that has it: its frames in
                   order. The comment before it lists its places in the groups and, for each,
                   the routines that start it by that number, with their special move;
                   (2 + atck_level) is a routine that counts on from 2 by the attack's level.
                   A frame with END_OF_DATA is the last, and the word after it is the frame the
                   animation goes on from (CG_NEXT; pl20_GROUP_NN_loop where that is no
                   animation's first frame). An effect's animation has short frames (EF_FRAME)
                   and may end with no word.
   pl20_unused_NN  frames that no table or animation reaches.

   A player's frame (CG_FRAME, the 0x14 bytes of CHAR_DATA), with the names Capcom's tables used:
     prm, type  a number and a code for the character's module (a speed, an attack, a voice)
     ctr        the game frames it is shown for
     end        END_OF_DATA on the last frame
     spr        the sprite, -1 for none
     prio       added to the draw priority (a signed byte)
     shadow     the shadow's offset (the low five bits) and size, 0xff none
     atck       ATK while an attack is on its way or can hit, ATE in its frames after that;
                CG_ATCK_NG lets nothing cancel it
     flag1      a number for the module: most often the attack, a row of hit_dt_20
     se         a voice (1 to 63), or with CSE a sound effect; 0 none
     zoom       the row of spr_prm_20, 0 none
     unk        a byte the program is not seen to read
     flag2      a second code for the character's effects
     rival      where a held opponent is put (the low byte), and a code for the module (the high)
     box        the frame's row of frame_box_20 */
#include "player.h"
#include "char_tbl.h"

extern const Sint32 pl20_stand_00[], pl20_stand_02[], pl20_stand_03[], pl20_jump_00[],
	pl20_jump_09[], pl20_jump_01[], pl20_jump_10[], pl20_jump_02[], pl20_jump_11[], pl20_jump_03[],
	pl20_jump_04[], pl20_jump_05[], pl20_jump_06[], pl20_jump_06_loop[], pl20_jump_07[],
	pl20_jump_07_loop[], pl20_jump_08[], pl20_jump_08_loop[], pl20_dash_00[], pl20_dash_00_loop[],
	pl20_dash_02[], pl20_dash_01[], pl20_dash_01_loop[], pl20_dash_03[], pl20_crouch_00[],
	pl20_crouch_01[], pl20_crouch_02[], pl20_turn_00[], pl20_turn_01[], pl20_guard_00[],
	pl20_guard_01[], pl20_guard_02[], pl20_guard_03[], pl20_guard_04[], pl20_guard_05[],
	pl20_guard_06[], pl20_guard_07[], pl20_air_guard_00[], pl20_air_guard_02[], pl20_stand_p_00[],
	pl20_stand_p_03[], pl20_stand_p_01[], pl20_stand_p_04[], pl20_stand_p_02[], pl20_stand_p_05[],
	pl20_stand_k_00[], pl20_stand_k_03[], pl20_stand_k_01[], pl20_stand_k_04[], pl20_stand_k_02[],
	pl20_stand_k_05[], pl20_crouch_p_00[], pl20_crouch_p_01[], pl20_crouch_p_02[],
	pl20_crouch_k_00[], pl20_crouch_k_01[], pl20_crouch_k_02[], pl20_jump_p_00[],
	pl20_jump_p_00_loop[], pl20_jump_p_01[], pl20_jump_p_02[], pl20_jump_k_00[],
	pl20_jump_k_00_loop[], pl20_jump_k_01[], pl20_jump_k_02[], pl20_damage_00[], pl20_damage_01[],
	pl20_damage_02[], pl20_damage_03[], pl20_damage_04[], pl20_damage_05[], pl20_damage_06[],
	pl20_damage_07[], pl20_damage_08[], pl20_damage_09[], pl20_damage_09_loop[], pl20_damage_10[],
	pl20_damage_11[], pl20_damage_12[], pl20_damage_13[], pl20_damage_16[], pl20_damage_17[],
	pl20_damage_18[], pl20_damage_19[], pl20_damage_20[], pl20_damage_21[], pl20_damage_22[],
	pl20_damage_23[], pl20_damage_24[], pl20_damage_25[], pl20_damage_26[], pl20_damage_27[],
	pl20_damage_27_loop[], pl20_damage_28[], pl20_damage_29[], pl20_damage_30[],
	pl20_damage_30_loop[], pl20_damage_31[], pl20_damage_33[], pl20_dizzy_00[], pl20_catch_00[],
	pl20_catch_00_loop[], pl20_catch_01[], pl20_catch_01_loop[], pl20_catch_02[], pl20_catch_03[],
	pl20_catch_04[], pl20_catch_04_loop[], pl20_catch_05[], pl20_catch_06[], pl20_get_up_00[],
	pl20_get_up_01[], pl20_get_up_02[], pl20_get_up_03[], pl20_appear_00[], pl20_win_00[],
	pl20_win_00_loop[], pl20_win_01[], pl20_win_01_loop[], pl20_win_02[], pl20_win_02_loop[],
	pl20_win_03[], pl20_win_03_loop[], pl20_extra_00[], pl20_extra_00_loop[], pl20_extra_01[],
	pl20_extra_02[], pl20_extra_02_loop[], pl20_extra_03[], pl20_extra_03_loop[], pl20_extra_04[],
	pl20_extra_05[], pl20_extra_06[], pl20_special_00[], pl20_special_28[], pl20_special_28_loop[],
	pl20_special_01[], pl20_special_29[], pl20_special_29_loop[], pl20_special_02[],
	pl20_special_02_loop[], pl20_special_03[], pl20_special_03_loop[], pl20_special_04[],
	pl20_special_04_loop[], pl20_special_05[], pl20_special_05_loop[], pl20_special_06[],
	pl20_special_07[], pl20_special_08[], pl20_special_08_loop[], pl20_special_10[],
	pl20_special_10_loop[], pl20_special_12[], pl20_special_14[], pl20_special_14_loop[],
	pl20_special_09[], pl20_special_09_loop[], pl20_special_11[], pl20_special_11_loop[],
	pl20_special_13[], pl20_special_15[], pl20_special_16[], pl20_special_17[], pl20_special_18[],
	pl20_special_18_loop[], pl20_special_19[], pl20_special_19_loop[], pl20_special_20[],
	pl20_special_20_loop[], pl20_special_21[], pl20_special_22[], pl20_special_23[],
	pl20_special_24[], pl20_special_25[], pl20_special_26[], pl20_special_27[], pl20_unused_00[],
	pl20_super_00[], pl20_super_00_loop[], pl20_super_01[], pl20_super_02[], pl20_super_03[],
	pl20_super_04[], pl20_super_04_loop[], pl20_super_05[], pl20_super_05_loop[], pl20_super_06[],
	pl20_super_07[], pl20_super_07_loop[], pl20_super_11[], pl20_effect_01[], pl20_effect_00[],
	pl20_effect_02[], pl20_effect_03[], pl20_effect_04[], pl20_effect_05[], pl20_effect_06[],
	pl20_effect_07[], pl20_effect_08[], pl20_catch_07[], pl20_catch_07_loop[], pl20_effect_10[],
	pl20_effect_10_loop[], pl20_effect_11[], pl20_effect_11_loop[], pl20_effect_12[],
	pl20_effect_12_loop[], pl20_effect_13[], pl20_effect_13_loop[], pl20_effect_14[],
	pl20_effect_14_loop[], pl20_effect_15[], pl20_effect_15_loop[], pl20_effect_16[],
	pl20_effect_16_loop[], pl20_effect_17[], pl20_change_01[], pl20_change_02[], pl20_change_03[],
	pl20_unused_01[], pl20_change_04[], pl20_caught_00[], pl20_caught_01[], pl20_caught_02[],
	pl20_caught_03[], pl20_caught_04[], pl20_caught_05[], pl20_caught_06[], pl20_caught_07[],
	pl20_caught_08[], pl20_caught_09[], pl20_caught_10[], pl20_caught_11[], pl20_caught_12[],
	pl20_caught_13[], pl20_caught_14[], pl20_caught_15[], pl20_caught_16[], pl20_caught_17[],
	pl20_caught_18[], pl20_caught_19[], pl20_caught_20[], pl20_caught_21[], pl20_grp27_00[],
	pl20_grp27_00_loop[], pl20_grp27_01[], pl20_grp27_02[], pl20_grp27_03[], pl20_grp27_03_loop[],
	pl20_grp27_04[], pl20_grp27_05[], pl20_grp27_06[], pl20_grp27_06_loop[], pl20_grp27_07[],
	pl20_grp27_08[], pl20_grp27_08_loop[], pl20_grp27_09[], pl20_effect_62[], pl20_effect_66[],
	pl20_effect_66_loop[], pl20_effect_63[], pl20_effect_67[], pl20_effect_67_loop[],
	pl20_effect_64[], pl20_effect_68[], pl20_effect_68_loop[], pl20_effect_65[], pl20_effect_69[],
	pl20_effect_69_loop[], pl20_effect_20[], pl20_effect_21[], pl20_effect_22[], pl20_effect_23[],
	pl20_effect_24[], pl20_effect_25[], pl20_effect_26[], pl20_effect_27[], pl20_effect_28[],
	pl20_effect_29[], pl20_effect_30[], pl20_effect_31[], pl20_effect_32[], pl20_effect_33[],
	pl20_effect_34[], pl20_effect_35[], pl20_effect_36[], pl20_effect_37[], pl20_effect_38[],
	pl20_effect_39[], pl20_effect_40[], pl20_effect_41[], pl20_effect_42[], pl20_effect_43[],
	pl20_effect_44[], pl20_effect_45[], pl20_effect_46[], pl20_effect_47[], pl20_effect_48[],
	pl20_effect_49[], pl20_effect_50[], pl20_effect_51[], pl20_effect_52[], pl20_effect_53[],
	pl20_effect_54[], pl20_effect_55[], pl20_effect_56[], pl20_effect_57[], pl20_effect_58[],
	pl20_effect_59[], pl20_effect_60[], pl20_effect_61[];
extern const Sint32 *const pl20_stand_tbl[], *const pl20_jump_tbl[], *const pl20_dash_tbl[],
	*const pl20_crouch_tbl[], *const pl20_turn_tbl[], *const pl20_guard_tbl[],
	*const pl20_air_guard_tbl[], *const pl20_stand_p_tbl[], *const pl20_stand_k_tbl[],
	*const pl20_crouch_p_tbl[], *const pl20_crouch_k_tbl[], *const pl20_jump_p_tbl[],
	*const pl20_jump_k_tbl[], *const pl20_damage_tbl[], *const pl20_dizzy_tbl[],
	*const pl20_catch_tbl[], *const pl20_caught_tbl[], *const pl20_get_up_tbl[],
	*const pl20_appear_tbl[], *const pl20_win_tbl[], *const pl20_extra_tbl[],
	*const pl20_special_tbl[], *const pl20_super_tbl[], *const pl20_effect_tbl[],
	*const pl20_change_tbl[], *const pl20_grp25_tbl[], *const pl20_grp27_tbl[];

/* the sprites' sizes and turns, by a frame's zoom number */
const SPR_PRM spr_prm_20[16] = {
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 2000, 2000, 0, 0, 0, 0, 0, 0 },
	{ 1008, 1000, 0, 0, 0, 0, 0, 0 },
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
const Sint32 *const *const char_tbl_20[] = {
	pl20_stand_tbl, pl20_jump_tbl, pl20_dash_tbl, pl20_crouch_tbl, pl20_turn_tbl, pl20_guard_tbl,
	pl20_air_guard_tbl, pl20_stand_p_tbl, pl20_stand_k_tbl, pl20_crouch_p_tbl, pl20_crouch_k_tbl, pl20_jump_p_tbl,
	pl20_jump_k_tbl, pl20_damage_tbl, pl20_dizzy_tbl, pl20_catch_tbl, pl20_caught_tbl, pl20_get_up_tbl,
	pl20_appear_tbl, pl20_win_tbl, pl20_extra_tbl, pl20_special_tbl, pl20_super_tbl, pl20_effect_tbl,
	pl20_change_tbl, pl20_grp25_tbl, pl20_stand_tbl, pl20_grp27_tbl, 0,
};

const Sint32 *const pl20_stand_tbl[] = {
	pl20_stand_00, pl20_stand_00, pl20_stand_02, pl20_stand_03, pl20_stand_00, pl20_stand_00,
	pl20_stand_00, pl20_stand_00, 0,
};

const Sint32 *const pl20_jump_tbl[] = {
	pl20_jump_00, pl20_jump_01, pl20_jump_02, pl20_jump_03, pl20_jump_04, pl20_jump_05,
	pl20_jump_06, pl20_jump_07, pl20_jump_08, pl20_jump_09, pl20_jump_10, pl20_jump_11,
	0,
};

const Sint32 *const pl20_dash_tbl[] = {
	pl20_dash_00, pl20_dash_01, pl20_dash_02, pl20_dash_03, 0,
};

const Sint32 *const pl20_crouch_tbl[] = {
	pl20_crouch_00, pl20_crouch_01, pl20_crouch_02, 0,
};

const Sint32 *const pl20_turn_tbl[] = {
	pl20_turn_00, pl20_turn_01, 0,
};

const Sint32 *const pl20_guard_tbl[] = {
	pl20_guard_00, pl20_guard_01, pl20_guard_02, pl20_guard_03, pl20_guard_04, pl20_guard_05,
	pl20_guard_06, pl20_guard_07, 0,
};

const Sint32 *const pl20_air_guard_tbl[] = {
	pl20_air_guard_00, pl20_air_guard_00, pl20_air_guard_02, 0,
};

const Sint32 *const pl20_stand_p_tbl[] = {
	pl20_stand_p_00, pl20_stand_p_01, pl20_stand_p_02, pl20_stand_p_03, pl20_stand_p_04, pl20_stand_p_05,
	0,
};

const Sint32 *const pl20_stand_k_tbl[] = {
	pl20_stand_k_00, pl20_stand_k_01, pl20_stand_k_02, pl20_stand_k_03, pl20_stand_k_04, pl20_stand_k_05,
	0,
};

const Sint32 *const pl20_crouch_p_tbl[] = {
	pl20_crouch_p_00, pl20_crouch_p_01, pl20_crouch_p_02, 0,
};

const Sint32 *const pl20_crouch_k_tbl[] = {
	pl20_crouch_k_00, pl20_crouch_k_01, pl20_crouch_k_02, 0,
};

const Sint32 *const pl20_jump_p_tbl[] = {
	pl20_jump_p_00, pl20_jump_p_01, pl20_jump_p_02, pl20_jump_p_00, pl20_jump_p_01, pl20_jump_p_02,
	0,
};

const Sint32 *const pl20_jump_k_tbl[] = {
	pl20_jump_k_00, pl20_jump_k_01, pl20_jump_k_02, pl20_jump_k_00, pl20_jump_k_01, pl20_jump_k_02,
	0,
};

const Sint32 *const pl20_damage_tbl[] = {
	pl20_damage_00, pl20_damage_01, pl20_damage_02, pl20_damage_03, pl20_damage_04, pl20_damage_05,
	pl20_damage_06, pl20_damage_07, pl20_damage_08, pl20_damage_09, pl20_damage_10, pl20_damage_11,
	pl20_damage_12, pl20_damage_13, pl20_damage_13, pl20_damage_13, pl20_damage_16, pl20_damage_17,
	pl20_damage_18, pl20_damage_19, pl20_damage_20, pl20_damage_21, pl20_damage_22, pl20_damage_23,
	pl20_damage_24, pl20_damage_25, pl20_damage_26, pl20_damage_27, pl20_damage_28, pl20_damage_29,
	pl20_damage_30, pl20_damage_31, pl20_damage_27, pl20_damage_33, pl20_damage_13, 0,
};

const Sint32 *const pl20_dizzy_tbl[] = {
	pl20_dizzy_00, 0,
};

const Sint32 *const pl20_catch_tbl[] = {
	pl20_catch_00, pl20_catch_01, pl20_catch_02, pl20_catch_03, pl20_catch_04, pl20_catch_05,
	pl20_catch_06, pl20_catch_07, 0,
};

const Sint32 *const pl20_caught_tbl[] = {
	pl20_caught_00, pl20_caught_01, pl20_caught_02, pl20_caught_03, pl20_caught_04, pl20_caught_05,
	pl20_caught_06, pl20_caught_07, pl20_caught_08, pl20_caught_09, pl20_caught_10, pl20_caught_11,
	pl20_caught_12, pl20_caught_13, pl20_caught_14, pl20_caught_15, pl20_caught_16, pl20_caught_17,
	pl20_caught_18, pl20_caught_19, pl20_caught_20, pl20_caught_21, 0,
};

const Sint32 *const pl20_get_up_tbl[] = {
	pl20_get_up_00, pl20_get_up_01, pl20_get_up_02, pl20_get_up_03, 0,
};

const Sint32 *const pl20_appear_tbl[] = {
	pl20_appear_00, 0,
};

const Sint32 *const pl20_win_tbl[] = {
	pl20_win_00, pl20_win_01, pl20_win_02, pl20_win_03, pl20_win_03, 0,
};

const Sint32 *const pl20_extra_tbl[] = {
	pl20_extra_00, pl20_extra_01, pl20_extra_02, pl20_extra_03, pl20_extra_04, pl20_extra_05,
	pl20_extra_06, 0,
};

const Sint32 *const pl20_special_tbl[] = {
	pl20_special_00, pl20_special_01, pl20_special_02, pl20_special_03, pl20_special_04, pl20_special_05,
	pl20_special_06, pl20_special_07, pl20_special_08, pl20_special_09, pl20_special_10, pl20_special_11,
	pl20_special_12, pl20_special_13, pl20_special_14, pl20_special_15, pl20_special_16, pl20_special_17,
	pl20_special_18, pl20_special_19, pl20_special_20, pl20_special_21, pl20_special_22, pl20_special_23,
	pl20_special_24, pl20_special_25, pl20_special_26, pl20_special_27, pl20_special_28, pl20_special_29,
	0,
};

const Sint32 *const pl20_super_tbl[] = {
	pl20_super_00, pl20_super_01, pl20_super_02, pl20_super_03, pl20_super_04, pl20_super_05,
	pl20_super_06, pl20_super_07, pl20_super_07, pl20_super_07, pl20_super_07, pl20_super_11,
	0,
};

const Sint32 *const pl20_effect_tbl[] = {
	pl20_effect_00, pl20_effect_01, pl20_effect_02, pl20_effect_03, pl20_effect_04, pl20_effect_05,
	pl20_effect_06, pl20_effect_07, pl20_effect_08, pl20_catch_07, pl20_effect_10, pl20_effect_11,
	pl20_effect_12, pl20_effect_13, pl20_effect_14, pl20_effect_15, pl20_effect_16, pl20_effect_17,
	pl20_effect_17, pl20_effect_17, pl20_effect_20, pl20_effect_21, pl20_effect_22, pl20_effect_23,
	pl20_effect_24, pl20_effect_25, pl20_effect_26, pl20_effect_27, pl20_effect_28, pl20_effect_29,
	pl20_effect_30, pl20_effect_31, pl20_effect_32, pl20_effect_33, pl20_effect_34, pl20_effect_35,
	pl20_effect_36, pl20_effect_37, pl20_effect_38, pl20_effect_39, pl20_effect_40, pl20_effect_41,
	pl20_effect_42, pl20_effect_43, pl20_effect_44, pl20_effect_45, pl20_effect_46, pl20_effect_47,
	pl20_effect_48, pl20_effect_49, pl20_effect_50, pl20_effect_51, pl20_effect_52, pl20_effect_53,
	pl20_effect_54, pl20_effect_55, pl20_effect_56, pl20_effect_57, pl20_effect_58, pl20_effect_59,
	pl20_effect_60, pl20_effect_61, pl20_effect_62, pl20_effect_63, pl20_effect_64, pl20_effect_65,
	pl20_effect_66, pl20_effect_67, pl20_effect_68, pl20_effect_69, 0,
};

const Sint32 *const pl20_change_tbl[] = {
	pl20_effect_17, pl20_change_01, pl20_change_02, pl20_change_03, pl20_change_04, 0,
};

const Sint32 *const pl20_grp25_tbl[] = {
	pl20_stand_00, 0,
};

const Sint32 *const pl20_grp27_tbl[] = {
	pl20_grp27_00, pl20_grp27_01, pl20_grp27_02, pl20_grp27_03, pl20_grp27_04, pl20_grp27_05,
	pl20_grp27_06, pl20_grp27_07, pl20_grp27_08, pl20_grp27_09, 0,
};

	/*         prm  type  ctr          end    spr  prio  shadow  atck  flag1          se  zoom   unk  flag2   rival  box */

/* Char_Stand 0: act_stand, appear_move, act_change_gattai_move, act_change_zero_move, ...
   Char_Stand 1
   Char_Stand 4
   Char_Stand 5
   Char_Stand 6
   Char_Stand 7 */
const Sint32 pl20_stand_00[] = {
	CG_FRAME(    0,    0,   4,           0,    42, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    43, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    44, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    45, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    46, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    47, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    48, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    49, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    50, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    51, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    52, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    53, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    54, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    55, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl20_stand_00 ),
};

/* Char_Stand 2: act_walk (2 + no)
   Char_Fly 2 */
const Sint32 pl20_stand_02[] = {
	CG_FRAME(    0,    0,   3,           0,    62, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    63, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    64, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    65, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    66, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    67, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    68, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    69, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    70, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    71, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    72, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    73, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl20_stand_02 ),
};

/* Char_Stand 3
   Char_Fly 3 */
const Sint32 pl20_stand_03[] = {
	CG_FRAME(    0,    0,   3,           0,    74, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    75, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    76, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    77, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    78, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    79, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    80, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    81, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    82, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    83, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    84, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,    85, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl20_stand_03 ),
};

/* Char_Jump 0 */
const Sint32 pl20_jump_00[] = {
	CG_FRAME(    0,    1,   4,           0,    99, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   100, 0x80,   0x20,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   101, 0x80,   0x20,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
};

/* Char_Jump 9: act_fall, act_air_guard_hold, act_wall_jump */
const Sint32 pl20_jump_09[] = {
	CG_FRAME(    0,    0,   3,           0,   102, 0x80,   0x20,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   103, 0x80,   0x20,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   104, 0x80,   0x20,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   105, 0x80,   0x20,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_NEXT( pl20_jump_09 ),
};

/* Char_Jump 1: tjump_land, act_wall_jump, act_change_gattai_init, act_change_zero_init, ... */
const Sint32 pl20_jump_01[] = {
	CG_FRAME(    0,    1,   4,           0,   111, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   112, 0x80,   0x20,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   113, 0x80,   0x20,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
};

/* Char_Jump 10: change_in */
const Sint32 pl20_jump_10[] = {
	CG_FRAME(    0,    0,   3,           0,   114, 0x80,   0x20,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   115, 0x80,   0x20,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   116, 0x80,   0x20,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   117, 0x80,   0x20,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_NEXT( pl20_jump_10 ),
};

/* Char_Jump 2 */
const Sint32 pl20_jump_02[] = {
	CG_FRAME(    0,    1,   4,           0,   123, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   124, 0x80,   0x20,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   125, 0x80,   0x20,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
};

/* Char_Jump 11: act_change_out_recover */
const Sint32 pl20_jump_11[] = {
	CG_FRAME(    0,    0,   3,           0,   126, 0x80,   0x20,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   127, 0x80,   0x20,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   128, 0x80,   0x20,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   129, 0x80,   0x20,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_NEXT( pl20_jump_11 ),
};

/* Char_Jump 3: p_catch_fall */
const Sint32 pl20_jump_03[] = {
	CG_FRAME(    0,    0,   4,           0,   106, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   107, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   108, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   109, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    3,    0,   3, END_OF_DATA,   110, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl20_jump_03 ),
};

/* Char_Jump 4 */
const Sint32 pl20_jump_04[] = {
	CG_FRAME(    0,    0,   4,           0,   118, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   119, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   120, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   121, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    3,    0,   3, END_OF_DATA,   122, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl20_jump_04 ),
};

/* Char_Jump 5 */
const Sint32 pl20_jump_05[] = {
	CG_FRAME(    0,    0,   4,           0,   130, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   131, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   132, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   133, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    3,    0,   3, END_OF_DATA,   134, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl20_jump_05 ),
};

/* Char_Jump 6 */
const Sint32 pl20_jump_06[] = {
	CG_FRAME(    0,    1,   4,           0,   161, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   162, 0x80,   0x20,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   163, 0x80,   0x20,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
};

const Sint32 pl20_jump_06_loop[] = {
	CG_FRAME(    0,    0,   3,           0,   164, 0x80,   0x20,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   165, 0x80,   0x20,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   166, 0x80,   0x20,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   167, 0x80,   0x20,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_NEXT( pl20_jump_06_loop ),
};

/* Char_Jump 7 */
const Sint32 pl20_jump_07[] = {
	CG_FRAME(    0,    1,   4,           0,   173, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   174, 0x80,   0x20,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   175, 0x80,   0x20,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
};

const Sint32 pl20_jump_07_loop[] = {
	CG_FRAME(    0,    0,   3,           0,   176, 0x80,   0x20,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   177, 0x80,   0x20,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   178, 0x80,   0x20,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   179, 0x80,   0x20,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_NEXT( pl20_jump_07_loop ),
};

/* Char_Jump 8 */
const Sint32 pl20_jump_08[] = {
	CG_FRAME(    0,    1,   4,           0,   185, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   186, 0x80,   0x20,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   187, 0x80,   0x20,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
};

const Sint32 pl20_jump_08_loop[] = {
	CG_FRAME(    0,    0,   3,           0,   188, 0x80,   0x20,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   189, 0x80,   0x20,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   190, 0x80,   0x20,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   191, 0x80,   0x20,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_NEXT( pl20_jump_08_loop ),
};

/* Char_Dash 0: act_dash */
const Sint32 pl20_dash_00[] = {
	CG_FRAME(    0,    0,   2,           0,    86, 0x80,   0x54,    0,  0x00,          0,    0,    0,     0,      0,   5 ),
};

const Sint32 pl20_dash_00_loop[] = {
	CG_FRAME(    0,    1,   3,           0,    87, 0x80,   0xa0,    0,  0x00,          0,    0,    0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,    88, 0x80,   0xa0,    0,  0x00,          0,    0,    0,     0,      0,   5 ),
	CG_NEXT( pl20_dash_00_loop ),
};

/* Char_Dash 2: dash_move */
const Sint32 pl20_dash_02[] = {
	CG_FRAME(    0,    0,   2,           0,    89, 0x80,   0x6b,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    90, 0x80,   0x4d,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,    91, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl20_dash_02 ),
};

/* Char_Dash 1: act_back_dash */
const Sint32 pl20_dash_01[] = {
	CG_FRAME(    0,    0,   2,           0,    93, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
};

const Sint32 pl20_dash_01_loop[] = {
	CG_FRAME(    0,    1,   3,           0,    94, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,    95, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl20_dash_01_loop ),
};

/* Char_Dash 3: noki_move */
const Sint32 pl20_dash_03[] = {
	CG_FRAME(    0,    0,   3,           0,    97, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,    98, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl20_dash_03 ),
};

/* Char_Crouch 0: act_crouch, act_stand_up */
const Sint32 pl20_crouch_00[] = {
	CG_FRAME(    0,    0,   3,           0,   135, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   136, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   137, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   138, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   2 ),
	CG_NEXT( pl20_crouch_00 ),
};

/* Char_Crouch 1: act_crouching, act_escape */
const Sint32 pl20_crouch_01[] = {
	CG_FRAME(    0,    0,   8,           0,    56, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   9,           0,    57, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   9,           0,    58, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   9,           0,    59, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   9,           0,    60, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   9, END_OF_DATA,    61, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   2 ),
	CG_NEXT( pl20_crouch_01 ),
};

/* Char_Crouch 2: act_stand_up, act_crouch */
const Sint32 pl20_crouch_02[] = {
	CG_FRAME(    0,    0,   3,           0,   140, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   141, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   142, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   143, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl20_crouch_02 ),
};

/* Char_Turn 0: act_turn */
const Sint32 pl20_turn_00[] = {
	CG_FRAME(    0,    0,   3,           0,   144, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   145, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   146, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   147, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl20_turn_00 ),
};

/* Char_Turn 1: act_crouch_turn */
const Sint32 pl20_turn_01[] = {
	CG_FRAME(    0,    0,   3,           0,   148, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   149, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   150, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,   151, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   2 ),
	CG_NEXT( pl20_turn_01 ),
};

/* Char_Guard 0 */
const Sint32 pl20_guard_00[] = {
	CG_FRAME(    0,    0,   3,           0,   152, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
};

/* Char_Guard 1: act_guard_hold */
const Sint32 pl20_guard_01[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   153, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl20_guard_01 ),
};

/* Char_Guard 2 */
const Sint32 pl20_guard_02[] = {
	CG_FRAME(    0,    0,   3,           0,   153, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   152, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl20_guard_02 ),
};

/* Char_Guard 3 */
const Sint32 pl20_guard_03[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   154, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl20_guard_03 ),
};

/* Char_Guard 4 */
const Sint32 pl20_guard_04[] = {
	CG_FRAME(    0,    0,   3,           0,   155, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   2 ),
};

/* Char_Guard 5: act_guard_hold */
const Sint32 pl20_guard_05[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   156, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   2 ),
	CG_NEXT( pl20_guard_05 ),
};

/* Char_Guard 6 */
const Sint32 pl20_guard_06[] = {
	CG_FRAME(    0,    0,   3,           0,   156, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   155, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   2 ),
	CG_NEXT( pl20_guard_06 ),
};

/* Char_Guard 7 */
const Sint32 pl20_guard_07[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   157, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   2 ),
	CG_NEXT( pl20_guard_07 ),
};

/* Char_Air_Guard 0: act_air_guard
   Char_Air_Guard 1: act_air_guard_start */
const Sint32 pl20_air_guard_00[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   159, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_NEXT( pl20_air_guard_00 ),
};

/* Char_Air_Guard 2: guard_char_set */
const Sint32 pl20_air_guard_02[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   160, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_NEXT( pl20_air_guard_02 ),
};

/* Char_Stand_P 0 (light) */
const Sint32 pl20_stand_p_00[] = {
	CG_FRAME(    0,    0,   2,           0,   459, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   460, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   6 ),
	CG_FRAME( 0x80,    0,   3,           0,   461, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   9 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,   462, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   6 ),
	CG_NEXT( pl20_stand_p_00 ),
};

/* Char_Stand_P 3 */
const Sint32 pl20_stand_p_03[] = {
	CG_FRAME(    0,    0,   3,           0,   355, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   356, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  10 ),
	CG_FRAME(    0,    0,   6,           0,   357, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   358, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   6 ),
	CG_NEXT( pl20_stand_p_03 ),
};

/* Char_Stand_P 1 (medium) */
const Sint32 pl20_stand_p_01[] = {
	CG_FRAME(    0,    0,   2,           0,   463, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   464, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   465, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   3,           0,   466, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   6 ),
	CG_FRAME( 0x80,    0,   2,           0,   467, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   8,           0,   468, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   469, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   6 ),
	CG_NEXT( pl20_stand_p_01 ),
};

/* Char_Stand_P 4 */
const Sint32 pl20_stand_p_04[] = {
	CG_FRAME(    0,    0,   4,           0,   359, 0x80,   0x40,  ATK,  0x00,          0,    0,    0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   1,           0,   360, 0x80,   0x4e,  ATK,  0x00,          0,    0,    0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   361, 0x80,   0x4d,  ATK,  0x00,          0,    0,    0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   2,           0,   362, 0x80,   0x4d,  ATK,  0x00,          0,    0,    0,     0,      0,  15 ),
	CG_FRAME(    0,    0,   6,           0,   363, 0x80,   0x4d,  ATK,  0x00,          0,    0,    0,     0,      0,  15 ),
	CG_FRAME(    0,    0,   2,           0,   364, 0x80,   0x4d,  ATK,  0x00,          0,    0,    0,     0,      0,  15 ),
	CG_FRAME(    0,    0,   4,           0,   365, 0x80,   0x4d,  ATK,  0x00,          0,    0,    0,     0,      0,  15 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   366, 0x80,   0x4e,  ATK,  0x00,          0,    0,    0,     0,      0,   6 ),
	CG_NEXT( pl20_stand_p_04 ),
};

/* Char_Stand_P 2 (heavy) */
const Sint32 pl20_stand_p_02[] = {
	CG_FRAME(    0,    0,   2,           0,   470, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   6,           0,   471, 0x80,   0x00,  ATK,  0x00,       0x03,    0,    0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   472, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  16 ),
	CG_FRAME(    0,    1,   2,           0,   473, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  17 ),
	CG_FRAME(    0,    0,   2,           0,   474, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  18 ),
	CG_FRAME(    0,    0,   8,           0,   475, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  19 ),
	CG_FRAME(    0,    0,   2,           0,   476, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  19 ),
	CG_FRAME(    0,    0,   8,           0,   477, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  19 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,   478, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   6 ),
	CG_NEXT( pl20_stand_p_02 ),
};

/* Char_Stand_P 5 */
const Sint32 pl20_stand_p_05[] = {
	CG_FRAME(    0,    0,   2,           0,   367, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   7,           0,   368, 0x80,   0x00,  ATK,  0x00,       0x03,    0,    0,     0,      0,   6 ),
	CG_FRAME( 0x80,    0,   2,           0,   369, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  20 ),
	CG_FRAME(    0,    0,   4,           0,   370, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  21 ),
	CG_FRAME(    0,    1,   3,           0,   371, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  22 ),
	CG_FRAME( 0x80,    2,   2,           0,   372, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  23 ),
	CG_FRAME( 0x80,    0,   2,           0,   373, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  24 ),
	CG_FRAME(    0,    0,  10,           0,   374, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  24 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   375, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   6 ),
	CG_NEXT( pl20_stand_p_05 ),
};

/* Char_Stand_K 0 (light) */
const Sint32 pl20_stand_k_00[] = {
	CG_FRAME(    0,    0,   3,           0,   479, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0, 114 ),
	CG_FRAME(    0,    0,   2,           0,   480, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  25 ),
	CG_FRAME(    0,    0,   5,           0,   480, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0, 114 ),
	CG_FRAME(    0,    0,   2,           0,   481, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   482, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   6 ),
	CG_NEXT( pl20_stand_k_00 ),
};

/* Char_Stand_K 3 */
const Sint32 pl20_stand_k_03[] = {
	CG_FRAME(    0,    0,   2,           0,   376, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   377, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   378, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  26 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,   379, 0x80,   0x00,    0,  0x00,          0,    0, 0x40,     0,      0,  27 ),
	CG_NEXT( pl20_stand_k_03 ),
};

/* Char_Stand_K 1 (medium) */
const Sint32 pl20_stand_k_01[] = {
	CG_FRAME(    0,    0,   5,           0,   483, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   3,           0,   484, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  28 ),
	CG_FRAME(    0,    0,   8,           0,   485, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  29 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   486, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   6 ),
	CG_NEXT( pl20_stand_k_01 ),
};

/* Char_Stand_K 4 */
const Sint32 pl20_stand_k_04[] = {
	CG_FRAME(    0,    0,   4,           0,   380, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   1,           0,   381, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   6 ),
	CG_FRAME( 0x80,    0,   3,           0,   382, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  30 ),
	CG_FRAME(    0,    0,   9,           0,   383, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  31 ),
	CG_FRAME(    0,    0,   3,           0,   384, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,   385, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   6 ),
	CG_NEXT( pl20_stand_k_04 ),
};

/* Char_Stand_K 2 (heavy) */
const Sint32 pl20_stand_k_02[] = {
	CG_FRAME(    0,    0,   5,           0,   487, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   3,           0,   488, 0x80,   0x00,  ATK,  0x00,       0x04,    0,    0,     0,      0,   6 ),
	CG_FRAME( 0x80,    0,   5,           0,   489, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  32 ),
	CG_FRAME(    0,    0,   8,           0,   490, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   7, END_OF_DATA,   490, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   6 ),
	CG_NEXT( pl20_stand_k_02 ),
};

/* Char_Stand_K 5 */
const Sint32 pl20_stand_k_05[] = {
	CG_FRAME(    0,    0,   4,           0,   386, 0x80,   0x4e,  ATK,  0x00,          0,    0,    0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   3,           0,   387, 0x80,   0x61,  ATK,  0x00,       0x04,    0,    0,     0,      0, 116 ),
	CG_FRAME(    0,    0,   3,           0,   388, 0x80,   0x61,  ATK,  0x00,          0,    0,    0,     0,      0,  34 ),
	CG_FRAME(    0,    0,   4,           0,   389, 0x80,   0x61,  ATK,  0x00,          0,    0,    0,     0,      0,  35 ),
	CG_FRAME(    0,    1,   3,           0,   390, 0x80,   0x61,  ATK,  0x00,          0,    0,    0,     0,      0,  36 ),
	CG_FRAME(    0,    0,   6,           0,   391, 0x80,   0x61,  ATE,  0x00,          0,    0,    0,     0,      0,  37 ),
	CG_FRAME(    0,    0,   3,           0,   392, 0x80,   0x61,  ATE,  0x00,          0,    0,    0,     0,      0,  37 ),
	CG_FRAME(    0,    0,   3,           0,   393, 0x80,   0x61,  ATE,  0x00,          0,    0,    0,     0,      0, 115 ),
	CG_FRAME(    0,    0,   3,           0,   394, 0x80,   0x4b,  ATE,  0x00,          0,    0,    0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   3,           0,   395, 0x80,   0x0e,  ATE,  0x00,          0,    0,    0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   396, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   6 ),
	CG_NEXT( pl20_stand_k_05 ),
};

/* Char_Crouch_P 0 (light) */
const Sint32 pl20_crouch_p_00[] = {
	CG_FRAME(    0,    0,   3,           0,   397, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2,           0,   398, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  38 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   398, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   7 ),
	CG_NEXT( pl20_crouch_p_00 ),
};

/* Char_Crouch_P 1 (medium) */
const Sint32 pl20_crouch_p_01[] = {
	CG_FRAME(    0,    0,   5,           0,   399, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   4,           0,   400, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  39 ),
	CG_FRAME(    0,    0,   6,           0,   401, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  40 ),
	CG_FRAME(    0,    0,   3,           0,   402, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   403, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   7 ),
	CG_NEXT( pl20_crouch_p_01 ),
};

/* Char_Crouch_P 2 (heavy) */
const Sint32 pl20_crouch_p_02[] = {
	CG_FRAME(    0,    0,   3,           0,   404, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2,           0,   405, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2,           0,   406, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2,           0,   412, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  41 ),
	CG_FRAME( 0x80,    0,   2,           0,   407, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  41 ),
	CG_FRAME( 0x80,    0,   2,           0,   408, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  41 ),
	CG_FRAME(    0,    0,   2,           0,   413, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  42 ),
	CG_FRAME(    0,    0,   2,           0,   409, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  42 ),
	CG_FRAME(    0,    0,   3,           0,   410, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  42 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   411, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   7 ),
	CG_NEXT( pl20_crouch_p_02 ),
};

/* Char_Crouch_K 0 (light) */
const Sint32 pl20_crouch_k_00[] = {
	CG_FRAME(    0,    0,   3,           0,   414, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2,           0,   415, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  43 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   415, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   7 ),
	CG_NEXT( pl20_crouch_k_00 ),
};

/* Char_Crouch_K 1 (medium) */
const Sint32 pl20_crouch_k_01[] = {
	CG_FRAME(    0,    0,   5,           0,   416, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3,           0,   417, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  44 ),
	CG_FRAME(    0,    0,  10,           0,   418, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   419, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   7 ),
	CG_NEXT( pl20_crouch_k_01 ),
};

/* Char_Crouch_K 2 (heavy) */
const Sint32 pl20_crouch_k_02[] = {
	CG_FRAME(    0,    0,   5,           0,   420, 0x80,   0x08,  ATK,  0x00,          0,    0,    0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3,           0,   421, 0x80,   0x46,  ATK,  0x00,          0,    0,    0,     0,      0,   7 ),
	CG_FRAME( 0x80,    0,   2,           0,   422, 0x80,   0x81,  ATK,  0x00,          0,    0,    0,     0,      0,  46 ),
	CG_FRAME(    0,    0,   6,           0,   423, 0x80,   0x81,  ATE,  0x00,          0,    0,    0,     0,      0,  47 ),
	CG_FRAME(    0,    0,   6,           0,   424, 0x80,   0x81,  ATE,  0x00,          0,    0,    0,     0,      0,  47 ),
	CG_FRAME(    0,    0,   5,           0,   425, 0x80,   0x62,  ATE,  0x00,          0,    0,    0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   426, 0x80,   0x08,  ATE,  0x00,          0,    0,    0,     0,      0,   7 ),
	CG_NEXT( pl20_crouch_k_02 ),
};

/* Char_Jump_P 0 (light)
   Char_Jump_P 3 */
const Sint32 pl20_jump_p_00[] = {
	CG_FRAME(    0,    0,   2,           0,   427, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   2,           0,   428, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_FRAME(    0,    0,  10,           0,   431, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  48 ),
};

const Sint32 pl20_jump_p_00_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   430, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   431, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  49 ),
	CG_NEXT( pl20_jump_p_00_loop ),
};

/* Char_Jump_P 1 (medium)
   Char_Jump_P 4 */
const Sint32 pl20_jump_p_01[] = {
	CG_FRAME(    0,    0,   4,           0,   432, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   2,           0,   433, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4,           0,   434, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   1,           0,   435, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  51 ),
	CG_FRAME(    0,    0,   1,           0,   436, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_FRAME(    0,    1,   4,           0,   437, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,   438, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  51 ),
	CG_NEXT( pl20_jump_p_01 ),
};

/* Char_Jump_P 2 (heavy)
   Char_Jump_P 5 */
const Sint32 pl20_jump_p_02[] = {
	CG_FRAME(    0,    0,   2,           0,   439, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   1,           0,   440, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   5,           0,   441, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   5,           0,   442, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   4,           0,   442, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  53 ),
	CG_FRAME(    0,    0,   4,           0,   443, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  53 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   444, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_NEXT( pl20_jump_p_02 ),
};

/* Char_Jump_K 0 (light)
   Char_Jump_K 3 */
const Sint32 pl20_jump_k_00[] = {
	CG_FRAME(    0,    0,   3,           0,   445, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_FRAME( 0x80,    0,  10,           0,   446, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  54 ),
};

const Sint32 pl20_jump_k_00_loop[] = {
	CG_FRAME(    0,    0,   6,           0,   447, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  55 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   448, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  55 ),
	CG_NEXT( pl20_jump_k_00_loop ),
};

/* Char_Jump_K 1 (medium)
   Char_Jump_K 4 */
const Sint32 pl20_jump_k_01[] = {
	CG_FRAME(    0,    0,   3,           0,   449, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   2,           0,   450, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_FRAME( 0x80,    0,   3,           0,   451, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  56 ),
	CG_FRAME(    0,    0,  10, END_OF_DATA,   452, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  57 ),
	CG_NEXT( pl20_jump_k_01 ),
};

/* Char_Jump_K 2 (heavy)
   Char_Jump_K 5 */
const Sint32 pl20_jump_k_02[] = {
	CG_FRAME(    0,    0,   2,           0,   453, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   2,           0,   454, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4,           0,   455, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_FRAME( 0x80,    0,   4,           0,   456, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   8,           0,   457, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,   458, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_NEXT( pl20_jump_k_02 ),
};

/* Char_Damage 0: dm_freeze, dm_freeze_air, caught_dm */
const Sint32 pl20_damage_00[] = {
	CG_FRAME(    0,    0,   5,           0,   830, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   831, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl20_damage_00 ),
};

/* Char_Damage 1 */
const Sint32 pl20_damage_01[] = {
	CG_FRAME(    0,    0,   4,           0,   830, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  10,           0,   831, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   832, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl20_damage_01 ),
};

/* Char_Damage 2 */
const Sint32 pl20_damage_02[] = {
	CG_FRAME(    0,    0,   4,           0,   830, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   831, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   832, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   833, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl20_damage_02 ),
};

/* Char_Damage 3 */
const Sint32 pl20_damage_03[] = {
	CG_FRAME(    0,    0,   5,           0,   835, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   836, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl20_damage_03 ),
};

/* Char_Damage 4 */
const Sint32 pl20_damage_04[] = {
	CG_FRAME(    0,    0,   4,           0,   835, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  10,           0,   836, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   837, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl20_damage_04 ),
};

/* Char_Damage 5 */
const Sint32 pl20_damage_05[] = {
	CG_FRAME(    0,    0,   4,           0,   835, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   836, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   837, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   838, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl20_damage_05 ),
};

/* Char_Damage 6 */
const Sint32 pl20_damage_06[] = {
	CG_FRAME(    0,    0,   5,           0,   840, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   841, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   2 ),
	CG_NEXT( pl20_damage_06 ),
};

/* Char_Damage 7 */
const Sint32 pl20_damage_07[] = {
	CG_FRAME(    0,    0,   4,           0,   840, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   2 ),
	CG_FRAME(    0,    0,  10,           0,   841, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   842, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   2 ),
	CG_NEXT( pl20_damage_07 ),
};

/* Char_Damage 8 */
const Sint32 pl20_damage_08[] = {
	CG_FRAME(    0,    0,   4,           0,   840, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   6,           0,   841, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   8,           0,   842, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   843, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   2 ),
	CG_NEXT( pl20_damage_08 ),
};

/* Char_Damage 9: dm_sweep */
const Sint32 pl20_damage_09[] = {
	CG_FRAME(    0,    0,   2,           0,   851, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  14,           0,   852, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   853, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   854, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   855, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   856, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   4 ),
};

const Sint32 pl20_damage_09_loop[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   857, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   4 ),
	CG_NEXT( pl20_damage_09_loop ),
};

/* Char_Damage 10: dm_ko_blow */
const Sint32 pl20_damage_10[] = {
	CG_FRAME(    0,    0,   5,           0,   844, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,   845, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_NEXT( pl20_damage_10 ),
};

/* Char_Damage 11 */
const Sint32 pl20_damage_11[] = {
	CG_FRAME(    0,    0,   3,           0,   844, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   845, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   846, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_NEXT( pl20_damage_11 ),
};

/* Char_Damage 12 */
const Sint32 pl20_damage_12[] = {
	CG_FRAME(    0,    0,   2,           0,   844, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   845, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   846, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   847, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_NEXT( pl20_damage_12 ),
};

/* Char_Damage 13: dm_snap_fly, dm_air
   Char_Damage 14
   Char_Damage 15
   Char_Damage 34: dm_snap_fly, dm_snap_down */
const Sint32 pl20_damage_13[] = {
	CG_FRAME(    0,    0,   4,           0,   849, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   850, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   4 ),
	CG_NEXT( pl20_damage_13 ),
};

/* Char_Damage 16: dm_blow_char_set (16 + no), dm_spark, caught_shock_init */
const Sint32 pl20_damage_16[] = {
	CG_FRAME(    0,    0,   4,           0,   858, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   859, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_NEXT( pl20_damage_16 ),
};

/* Char_Damage 17: dm_07_1 */
const Sint32 pl20_damage_17[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   860, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_NEXT( pl20_damage_17 ),
};

/* Char_Damage 18 */
const Sint32 pl20_damage_18[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   861, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_NEXT( pl20_damage_18 ),
};

/* Char_Damage 19 */
const Sint32 pl20_damage_19[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   862, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_NEXT( pl20_damage_19 ),
};

/* Char_Damage 20 */
const Sint32 pl20_damage_20[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   863, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_NEXT( pl20_damage_20 ),
};

/* Char_Damage 21: dm_fall_char_set (21 + no) */
const Sint32 pl20_damage_21[] = {
	CG_FRAME(    0,    0,   4,           0,   858, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   859, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
};

/* Char_Damage 22 */
const Sint32 pl20_damage_22[] = {
	CG_FRAME(    0,    0,   4,           0,   860, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
};

/* Char_Damage 23 */
const Sint32 pl20_damage_23[] = {
	CG_FRAME(    0,    0,   4,           0,   861, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
};

/* Char_Damage 24 */
const Sint32 pl20_damage_24[] = {
	CG_FRAME(    0,    0,   4,           0,   862, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
};

/* Char_Damage 25 */
const Sint32 pl20_damage_25[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   863, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_NEXT( pl20_damage_25 ),
};

/* Char_Damage 26: dm_buttobi_fly, dm_07_1, dm_away_fly, dm_bound_hop, ... */
const Sint32 pl20_damage_26[] = {
	CG_FRAME(    0,    0,   1,           0,   864, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   1,           0,   865, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   866, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   867, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   868, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   869, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   870, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   4 ),
	CG_NEXT( pl20_damage_26 ),
};

/* Char_Damage 27
   Char_Damage 32: act_escape */
const Sint32 pl20_damage_27[] = {
	CG_FRAME(    0,    0,   3,           0,   876, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
};

const Sint32 pl20_damage_27_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   877, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   878, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   879, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   880, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_NEXT( pl20_damage_27_loop ),
};

/* Char_Damage 28: dm_away_init, dm_bound_fly, dm_piyo, dm_away */
const Sint32 pl20_damage_28[] = {
	CG_FRAME(    0,    0,   2,           0,   885, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   886, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   887, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   888, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   889, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   890, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_NEXT( pl20_damage_28 ),
};

/* Char_Damage 29: dm_away_fly, dm_bound_hop, dm_bound, caught_spin_fly, ... */
const Sint32 pl20_damage_29[] = {
	CG_FRAME(    0,    0,   2,           0,   891, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   892, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   893, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   894, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   895, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   896, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_NEXT( pl20_damage_29 ),
};

/* Char_Damage 30: act_air_recover, act_change_out_init, caught_blow_fly, caught_blow_fix_fly, ... */
const Sint32 pl20_damage_30[] = {
	CG_FRAME(    0,    0,   3,           0,   876, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
};

const Sint32 pl20_damage_30_loop[] = {
	CG_FRAME(    0,    0,   3,           0,   877, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   878, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   879, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   880, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_NEXT( pl20_damage_30_loop ),
};

/* Char_Damage 31: act_down, dm_dizzy_down */
const Sint32 pl20_damage_31[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   870, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   4 ),
	CG_NEXT( pl20_damage_31 ),
};

/* Char_Damage 33: act_escape */
const Sint32 pl20_damage_33[] = {
	CG_FRAME(    0,    0,   3,           0,   903, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   904, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   905, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   906, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   907, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   908, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   909, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_NEXT( pl20_damage_33 ),
};

/* Char_Dizzy 0: act_dizzy */
const Sint32 pl20_dizzy_00[] = {
	CG_FRAME(    0,    0,   8,           0,   897, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  15,           0,   898, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   7,           0,   899, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   900, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  15,           0,   901, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   7, END_OF_DATA,   902, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl20_dizzy_00 ),
};

/* Char_Catch 0: p_set */
const Sint32 pl20_catch_00[] = {
	CG_FRAME(    0,    0,   6,           0,   559, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0, 0x0001,   0 ),
	CG_FRAME(    0,    0,   6,           0,   560, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0, 0x0002,   0 ),
	CG_FRAME(    0,    0,   6,           0,   561, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0, 0x0003,   0 ),
	CG_FRAME(    0,    1,   4,           0,   491, 0x80,   0x46,    0,  0x00,          0,    0,    0,     0, 0x0004,   0 ),
	CG_FRAME(    0,    0,   4,           0,   492, 0x80,   0x46,    0,  0x00,          0,    0,    0,     0, 0x0004,   0 ),
	CG_FRAME(    0,    0,   2,           0,   493, 0x80,   0x46,    0,  0x00,          0,    0,    0,     0, 0x0004,   0 ),
};

const Sint32 pl20_catch_00_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   494, 0x80,   0x46,    0,  0x00,          0,    0,    0,     0, 0x0004,   0 ),
	CG_FRAME(    0,    0,   2,           0,   495, 0x80,   0x46,    0,  0x00,          0,    0,    0,     0, 0x0004,   0 ),
	CG_FRAME(    0,    1,   4,           0,   496, 0x80,   0x46,    0,  0x00,       0x19,    0,    0,     0, 0x0005,   0 ),
	CG_FRAME(    0,    0,   2,           0,   497, 0x80,   0x46,    0,  0x00,          0,    0,    0,     0, 0x0005,   0 ),
	CG_FRAME(    0,    0,   2,           0,   498, 0x80,   0x46,    0,  0x00,          0,    0,    0,     0, 0x0004,   0 ),
	CG_FRAME(    0,    0,   2,           0,   499, 0x80,   0x46,    0,  0x00,          0,    0,    0,     0, 0x0004,   0 ),
	CG_FRAME(    0,    2,   4,           0,   500, 0x80,   0x46,    0,  0x00,       0x19,    0,    0,     0, 0x0004,   0 ),
	CG_FRAME(    0, 0x80,  16, END_OF_DATA,   493, 0x80,   0x46,    0,  0x00,          0,    0,    0,     0, 0x0004,   0 ),
	CG_NEXT( pl20_catch_00_loop ),
};

/* Char_Catch 1: p_catch_hit */
const Sint32 pl20_catch_01[] = {
	CG_FRAME(    0,    0,   2,           0,   493, 0x80,   0x46,    0,  0x00,          0,    0,    0,     0, 0x0004,   0 ),
	CG_FRAME(    0,    0,   2,           0,   492, 0x80,   0x46,    0,  0x00,          0,    0,    0,     0, 0x0004,   0 ),
	CG_FRAME(    0,    0,   3,           0,   564, 0x80,   0x20,    0,  0x00,       0x18,    0,    0,     0, 0x0006,   0 ),
};

const Sint32 pl20_catch_01_loop[] = {
	CG_FRAME(    0,    1,   3,           0,   126, 0x80,   0x20,    0,  0x00,          0,    0,    0,     0, 0x0006,   0 ),
	CG_FRAME(    0,    1,   3,           0,   127, 0x80,   0x20,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   3,           0,   128, 0x80,   0x20,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   3, END_OF_DATA,   129, 0x80,   0x20,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_NEXT( pl20_catch_01_loop ),
};

/* Char_Catch 2: k_set */
const Sint32 pl20_catch_02[] = {
	CG_FRAME(    0,    0,   6,           0,   559, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0, 0x0001,   0 ),
	CG_FRAME(    0,    0,   6,           0,   560, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0, 0x0002,   0 ),
	CG_FRAME(    0,    0,   6,           0,   561, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0, 0x0003,   0 ),
	CG_FRAME(    0,    1,   8,           0,   501, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0, 0x0007,   0 ),
	CG_FRAME(    0,    1,   3,           0,   502, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0, 0x0008,   0 ),
	CG_FRAME(    0,    0,   3,           0,   503, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0, 0x0009,   0 ),
	CG_FRAME(    0,    0,   3,           0,   504, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0, 0x000a,   0 ),
	CG_FRAME(    0,    0,   3,           0,   505, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0, 0x000b,   0 ),
	CG_FRAME(    0,    0,   3,           0,   506, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0, 0x000c,   0 ),
	CG_FRAME(    0,    0,   3,           0,   503, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0, 0x0009,   0 ),
	CG_FRAME(    0,    0,   3,           0,   504, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0, 0x000a,   0 ),
	CG_FRAME(    0,    0,   3,           0,   505, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0, 0x000b,   0 ),
	CG_FRAME(    0,    1,   1,           0,   506, 0x80,   0x00,    0,  0x00,       0x1a,    0,    0,     0, 0x000c,   0 ),
	CG_FRAME(    0,    0,   5,           0,   507, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0, 0x000d,   0 ),
	CG_FRAME(    0,    0,   6,           0,   508, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   509, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_NEXT( pl20_catch_02 ),
};

/* Char_Catch 3: k_catch_end */
const Sint32 pl20_catch_03[] = {
	CG_FRAME(    0,    0,   2,           0,   510, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   511, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   512, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   513, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   514, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   515, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   516, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   517, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_NEXT( pl20_catch_03 ),
};

/* Char_Catch 4: jump_set */
const Sint32 pl20_catch_04[] = {
	CG_FRAME(    0,    0,   5,           0,   518, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0, 0x000e,   0 ),
	CG_FRAME(    0,    0,   4,           0,   519, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0, 0x000f,   0 ),
	CG_FRAME(    0,    0,   3,           0,   520, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0, 0x0010,   0 ),
	CG_FRAME(    0,    0,   3,           0,   521, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0, 0x0011,   0 ),
	CG_FRAME(    0,    0,   3,           0,   522, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0, 0x0012,   0 ),
	CG_FRAME(    0,    0,   3,           0,   523, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0, 0x0010,   0 ),
	CG_FRAME(    0,    0,   3,           0,   524, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0, 0x0011,   0 ),
	CG_FRAME(    0,    0,   3,           0,   525, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0, 0x0013,   0 ),
	CG_FRAME(    0,    0,   6,           0,   526, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0, 0x0014,   0 ),
	CG_FRAME(    0,    0,   4,           0,   527, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0, 0x0015,   0 ),
	CG_FRAME(    0,    0,   3,           0,   528, 0x80,   0x00,    0,  0x00,       0x02,    0,    0,     0, 0x0016,   0 ),
	CG_FRAME(    0,    1,   2,           0,   529, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0, 0x0017,   0 ),
	CG_FRAME(    0,    0,   2,           0,   530, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
};

const Sint32 pl20_catch_04_loop[] = {
	CG_FRAME(    0,    1,   3,           0,   531, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   3,           0,   532, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   3, END_OF_DATA,   533, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_NEXT( pl20_catch_04_loop ),
};

/* Char_Catch 5: jump_catch_fall */
const Sint32 pl20_catch_05[] = {
	CG_FRAME(    0,    0,   3,           0,   534, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   535, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   536, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   537, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   538, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_NEXT( pl20_catch_05 ),
};

/* Char_Catch 6: hellcat_set */
const Sint32 pl20_catch_06[] = {
	CG_FRAME(    0,    0,   6,           0,   561, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0, 0x0003,   0 ),
	CG_FRAME(    0,    0,   5,           0,   539, 0x80,   0x00,    0,  0x00, 0x16 | CSE,    0,    0,     0, 0x0018,   0 ),
	CG_FRAME(    0,    0,   3,           0,   540, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0, 0x0019,   0 ),
	CG_FRAME(    0,    1,   3,           0,   541, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0, 0x001a,   0 ),
	CG_FRAME(    0,    0,   3,           0,   542, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0, 0x001b,   0 ),
	CG_FRAME(    0,    0,   3,           0,   543, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0, 0x001c,   0 ),
	CG_FRAME(    0,    0,   3,           0,   544, 0x80,   0x00,    0,  0x00, 0x16 | CSE,    0,    0,     0, 0x001d,   0 ),
	CG_FRAME(    0,    0,   3,           0,   545, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0, 0x001e,   0 ),
	CG_FRAME(    0,    0,   3,           0,   540, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0, 0x001f,   0 ),
	CG_FRAME(    0,    1,   3,           0,   541, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0, 0x0020,   0 ),
	CG_FRAME(    0,    0,   3,           0,   542, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0, 0x0021,   0 ),
	CG_FRAME(    0,    0,   3,           0,   543, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0, 0x0022,   0 ),
	CG_FRAME(    0,    0,   3,           0,   544, 0x80,   0x00,    0,  0x00, 0x16 | CSE,    0,    0,     0, 0x0023,   0 ),
	CG_FRAME(    0,    0,   3,           0,   545, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0, 0x0024,   0 ),
	CG_FRAME(    0,    0,   3,           0,   540, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0, 0x0025,   0 ),
	CG_FRAME(    0,    1,   3,           0,   541, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0, 0x0026,   0 ),
	CG_FRAME(    0,    0,   3,           0,   542, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0, 0x0027,   0 ),
	CG_FRAME(    0,    0,   3,           0,   543, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0, 0x0028,   0 ),
	CG_FRAME(    0,    0,   3,           0,   544, 0x80,   0x00,    0,  0x00, 0x16 | CSE,    0,    0,     0, 0x0029,   0 ),
	CG_FRAME(    0,    0,   3,           0,   545, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0, 0x002a,   0 ),
	CG_FRAME(    0,    0,   3,           0,   546, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0, 0x002b,   0 ),
	CG_FRAME(    0,   -1,   7,           0,   547, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0, 0x002c,   0 ),
	CG_FRAME(    0,    0,   3,           0,   548, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0, 0x002d,   0 ),
	CG_FRAME(    0,    0,   3,           0,   549, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0, 0x002e,   0 ),
	CG_FRAME(    0,    1,   3,           0,   550, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0, 0x002f,   0 ),
	CG_FRAME(    0,    0,   5,           0,   551, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   552, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   553, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  12,           0,   554, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  10,           0,   555, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   556, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   557, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   558, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_NEXT( pl20_catch_06 ),
};

/* Char_Get_Up 0: dm_dizzy_wait */
const Sint32 pl20_get_up_00[] = {
	CG_FRAME(    0,    0,   3,           0,   197, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   198, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   199, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   200, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   201, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   202, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   203, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   204, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   205, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl20_get_up_00 ),
};

/* Char_Get_Up 1 */
const Sint32 pl20_get_up_01[] = {
	CG_FRAME(    0,    0,   2,           0,   206, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   207, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   208, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   209, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   210, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   211, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   212, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   213, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   214, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   215, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   216, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   217, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_NEXT( pl20_get_up_01 ),
};

/* Char_Get_Up 2 */
const Sint32 pl20_get_up_02[] = {
	CG_FRAME(    0,    0,   4,           0,   206, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   207, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   208, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   209, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   210, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   211, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   208, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   209, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   210, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   211, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   212, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   213, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   214, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   215, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   216, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   217, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_NEXT( pl20_get_up_02 ),
};

/* Char_Get_Up 3 */
const Sint32 pl20_get_up_03[] = {
	CG_FRAME(    0,    0,   2,           0,   218, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   219, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   220, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   221, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   222, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   223, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   224, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   225, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   226, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   227, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   228, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   229, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_NEXT( pl20_get_up_03 ),
};

/* Char_Appear 0: appear_init */
const Sint32 pl20_appear_00[] = {
	CG_FRAME(    0,    0,   4,           0,   272, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   273, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   274, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   275, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   276, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   277, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  16,           0,   278, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   279, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   280, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   281, 0x80,   0x00,    0,  0x00,       0x0b,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   282, 0x80,   0x00,    0,  0x00,       0x0a,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   283, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   284, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   285, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   286, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   287, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   288, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_NEXT( pl20_appear_00 ),
};

/* Char_Win 0 */
const Sint32 pl20_win_00[] = {
	CG_FRAME(    0,    0,   4,           0,   296, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   297, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   298, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   299, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   300, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   301, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   302, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   303, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   304, 0x80,   0x00,    0,  0x00,       0x19,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   305, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   306, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   307, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
};

const Sint32 pl20_win_00_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   308, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl20_win_00_loop ),
};

/* Char_Win 1 */
const Sint32 pl20_win_01[] = {
	CG_FRAME(    0,    0,   4,           0,   309, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   310, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   311, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   4,           0,   312, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
};

const Sint32 pl20_win_01_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   313, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   314, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   315, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   316, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   317, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   318, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl20_win_01_loop ),
};

/* Char_Win 2 */
const Sint32 pl20_win_02[] = {
	CG_FRAME(    0,    0,   4,           0,   319, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   320, 0x80,   0x07,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   321, 0x80,   0x04,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   322, 0x80,   0x10,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   323, 0x80,   0x10,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   324, 0x80,   0x10,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   325, 0x80,   0x10,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   326, 0x80,   0x10,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   327, 0x80,   0x10,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   328, 0x80,   0x10,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   329, 0x80,   0x10,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   330, 0x80,   0x10,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   331, 0x80,   0x10,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   332, 0x80,   0x10,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   333, 0x80,   0x10,    0,  0x00,       0x10,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   334, 0x80,   0x10,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  16,           0,   335, 0x80,   0x10,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  18,           0,   336, 0x80,   0x10,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   337, 0x80,   0x10,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
};

const Sint32 pl20_win_02_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   338, 0x80,   0x10,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl20_win_02_loop ),
};

/* Char_Win 3: lose_time_ko
   Char_Win 4 */
const Sint32 pl20_win_03[] = {
	CG_FRAME(    0,    0,  10,           0,   266, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   267, 0x80,   0x00,    0,  0x00,       0x10,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   268, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  30,           0,   269, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   270, 0x80,   0x60,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
};

const Sint32 pl20_win_03_loop[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA,   271, 0x80,   0x60,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl20_win_03_loop ),
};

/* Char_Extra 0: t_kick_init */
const Sint32 pl20_extra_00[] = {
	CG_FRAME(    0,    0,   3,           0,   450, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   451, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   3 ),
};

const Sint32 pl20_extra_00_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   452, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  98 ),
	CG_NEXT( pl20_extra_00_loop ),
};

/* Char_Extra 1: t_kick_move */
const Sint32 pl20_extra_01[] = {
	CG_FRAME(    0,    0,   4,           0,   236, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   237, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   238, 0x80,   0x00,  ATE,  0x00,       0x0a,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   239, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   240, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  10,           0,   241, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   242, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   243, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl20_extra_01 ),
};

/* Char_Extra 2: tjump_init, fly_start5 */
const Sint32 pl20_extra_02[] = {
	CG_FRAME(    0,    0,   2,           0,   562, 0x80,   0x00,    0,  0x00,       0x19,    0,    0,     0,      0,  99 ),
};

const Sint32 pl20_extra_02_loop[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   563, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,  99 ),
	CG_NEXT( pl20_extra_02_loop ),
};

/* Char_Extra 3: fly_start5 */
const Sint32 pl20_extra_03[] = {
	CG_FRAME(    0,    0,   6,           0,   565, 0x80,   0x00,    0,  0x00,       0x1e,    0,    0,     0,      0,  99 ),
	CG_FRAME(    0,    0,   6,           0,   566, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,  99 ),
	CG_FRAME(    0,    0,   6,           0,   565, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,  99 ),
	CG_FRAME(    0,    0,   6,           0,   566, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,  99 ),
	CG_FRAME(    0,    0,   6,           0,   565, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,  99 ),
	CG_FRAME(    0,    0,   6,           0,   566, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,  99 ),
};

const Sint32 pl20_extra_03_loop[] = {
	CG_FRAME(    0,    0,   6,           0,   567, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,  99 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   568, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,  99 ),
	CG_NEXT( pl20_extra_03_loop ),
};

/* Char_Extra 4: tjump_fall */
const Sint32 pl20_extra_04[] = {
	CG_FRAME(    0,    0,   3,           0,   562, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,  99 ),
	CG_FRAME(    0,    0,   3,           0,   563, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,  99 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   564, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,  99 ),
	CG_NEXT( pl20_extra_04 ),
};

/* Char_Extra 5: pl20_upper */
const Sint32 pl20_extra_05[] = {
	CG_FRAME(    0,    0,   3,           0,   487, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   488, 0x80,   0x00,  ATK,  0x00,       0x04,    0,    0,     0,      0,   1 ),
	CG_FRAME( 0x80,    0,   5,           0,   489, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0, 100 ),
	CG_FRAME(    0,    0,   3,           0,   490, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   7, END_OF_DATA,   490, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl20_extra_05 ),
};

/* Char_Extra 6: sp_chouhatsu (Taunt) */
const Sint32 pl20_extra_06[] = {
	CG_FRAME(    0,    0,   3,           0,   289, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   290, 0x80,   0x00,    0,  0x00,       0x1b,    0,    0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   291, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   292, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   293, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   294, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   295, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   293, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   294, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   295, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   293, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   294, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   295, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   292, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   291, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   290, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   289, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   2 ),
	CG_NEXT( pl20_extra_06 ),
};

/* Char_Special 0 */
const Sint32 pl20_special_00[] = {
	CG_FRAME(    0,    1,   3,           0,   612, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    1,   3,           0,   613, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   3 ),
};

/* Char_Special 28 */
const Sint32 pl20_special_28[] = {
	CG_FRAME(    0,    0,   3,           0,   614, 0x80,   0x00,  ATK,  0x00,       0x30,    0,    0,     0,      0,  60 ),
	CG_FRAME( 0x80,    1,   3,           0,   615, 0x80,   0x00,  ATK,  0x00,       0x32,    0,    0,     0,      0,  61 ),
};

const Sint32 pl20_special_28_loop[] = {
	CG_FRAME(    0,    0,   3,           0,   616, 0x80,   0x00,  ATK,  0x00,       0x32,    0,    0,     0,      0,  61 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   617, 0x80,   0x00,  ATK,  0x00,       0x32,    0,    0,     0,      0,  61 ),
	CG_NEXT( pl20_special_28_loop ),
};

/* Char_Special 1 */
const Sint32 pl20_special_01[] = {
	CG_FRAME(    0,    1,   3,           0,   612, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    1,   3,           0,   613, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   3 ),
};

/* Char_Special 29 */
const Sint32 pl20_special_29[] = {
	CG_FRAME(    0,    0,   3,           0,   614, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  60 ),
	CG_FRAME( 0x80,    1,   3,           0,   615, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  61 ),
};

const Sint32 pl20_special_29_loop[] = {
	CG_FRAME(    0,    0,   3,           0,   616, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  61 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   617, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  61 ),
	CG_NEXT( pl20_special_29_loop ),
};

/* Char_Special 2 */
const Sint32 pl20_special_02[] = {
	CG_FRAME(    0,    0,   2,           0,   618, 0x80,   0x00,  ATK,  0x00,       0x02,    0,    0,     0,      0,  62 ),
	CG_FRAME(    0,    0,   2,           0,   445, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME( 0x80,    0,   2,           0,   446, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  63 ),
};

const Sint32 pl20_special_02_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   447, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  63 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   448, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  63 ),
	CG_NEXT( pl20_special_02_loop ),
};

/* Char_Special 3 */
const Sint32 pl20_special_03[] = {
	CG_FRAME(    0,    0,   2,           0,   618, 0x80,   0x00,  ATK,  0x00,       0x02,    0,    0,     0,      0,  62 ),
	CG_FRAME(    0,    0,   2,           0,   445, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME( 0x80,    0,   2,           0,   446, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  63 ),
};

const Sint32 pl20_special_03_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   447, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  63 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   448, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  63 ),
	CG_NEXT( pl20_special_03_loop ),
};

/* Char_Special 4 */
const Sint32 pl20_special_04[] = {
	CG_FRAME(    0,    0,   2,           0,   450, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME( 0x80,    0,   2,           0,   451, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  64 ),
};

const Sint32 pl20_special_04_loop[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   452, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  64 ),
	CG_NEXT( pl20_special_04_loop ),
};

/* Char_Special 5 */
const Sint32 pl20_special_05[] = {
	CG_FRAME(    0,    0,   3,           0,   450, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME( 0x80,    0,   2,           0,   451, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  64 ),
};

const Sint32 pl20_special_05_loop[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   452, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  64 ),
	CG_NEXT( pl20_special_05_loop ),
};

/* Char_Special 6 */
const Sint32 pl20_special_06[] = {
	CG_FRAME(    0,    0,  12,           0,   106, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   107, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   108, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   109, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   110, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl20_special_06 ),
};

/* Char_Special 7 */
const Sint32 pl20_special_07[] = {
	CG_FRAME(    0,    0,  12,           0,   106, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   107, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   108, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   109, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   110, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl20_special_07 ),
};

/* Char_Special 8 */
const Sint32 pl20_special_08[] = {
	CG_FRAME(    0,    1,   2,           0,   716, 0x80,   0x59,  ATK,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   2,           0,   717, 0x80,   0x59,  ATK,  0x00,       0x21,    0,    0,     0,      0,   1 ),
};

const Sint32 pl20_special_08_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   619, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  65 ),
	CG_FRAME(    0,    0,   2,           0,   620, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  65 ),
	CG_FRAME(    0,    0,   2,           0,   621, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  65 ),
	CG_FRAME(    0,    0,   2,           0,   622, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  65 ),
	CG_FRAME(    0,    0,   2,           0,   623, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  65 ),
	CG_FRAME(    0,    0,   2,           0,   624, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  65 ),
	CG_FRAME(    0,    0,   2,           0,   625, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  65 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   626, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  65 ),
	CG_NEXT( pl20_special_08_loop ),
};

/* Char_Special 10 */
const Sint32 pl20_special_10[] = {
	CG_FRAME(    0,    1,   3,           0,   710, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   711, 0x80,   0x00,  ATK,  0x01,       0x04,    0,    0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   3,           0,   712, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  67 ),
	CG_FRAME(    0,    1,   4,           0,   713, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  68 ),
	CG_FRAME(    0,    0,   2,           0,   714, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0, 113 ),
	CG_FRAME(    0,    0,   2,           0,   715, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   3 ),
};

const Sint32 pl20_special_10_loop[] = {
	CG_FRAME(    0,    0,   3,           0,   102, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   103, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   104, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   105, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_NEXT( pl20_special_10_loop ),
};

/* Char_Special 12 */
const Sint32 pl20_special_12[] = {
	CG_FRAME(    0,    0,   3,           0,   102, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   103, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   104, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   105, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_NEXT( pl20_special_12 ),
};

/* Char_Special 14 */
const Sint32 pl20_special_14[] = {
	CG_FRAME(    0,    0,   3,           0,   106, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   107, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   108, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   109, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   1 ),
};

const Sint32 pl20_special_14_loop[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   110, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl20_special_14_loop ),
};

/* Char_Special 9 */
const Sint32 pl20_special_09[] = {
	CG_FRAME(    0,    1,   4,           0,   716, 0x80,   0x59,  ATK,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   2,           0,   717, 0x80,   0x59,  ATK,  0x00,       0x21,    0,    0,     0,      0,   1 ),
};

const Sint32 pl20_special_09_loop[] = {
	CG_FRAME(    0,    0,   1,           0,   619, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  65 ),
	CG_FRAME(    0,    0,   1,           0,   620, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  65 ),
	CG_FRAME(    0,    0,   1,           0,   621, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  65 ),
	CG_FRAME(    0,    0,   1,           0,   622, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  65 ),
	CG_FRAME(    0,    0,   1,           0,   623, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  65 ),
	CG_FRAME(    0,    0,   1,           0,   624, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  65 ),
	CG_FRAME(    0,    0,   1,           0,   625, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  65 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   626, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  65 ),
	CG_NEXT( pl20_special_09_loop ),
};

/* Char_Special 11 */
const Sint32 pl20_special_11[] = {
	CG_FRAME(    0,    1,   3,           0,   710, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   711, 0x80,   0x00,  ATK,  0x00,       0x04,    0,    0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   3,           0,   712, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  67 ),
	CG_FRAME(    0,    1,  13,           0,   713, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  68 ),
	CG_FRAME(    0,    0,   2,           0,   714, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0, 113 ),
	CG_FRAME(    0,    0,   2,           0,   715, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   3 ),
};

const Sint32 pl20_special_11_loop[] = {
	CG_FRAME(    0,    0,   3,           0,   102, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   103, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   104, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   105, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_NEXT( pl20_special_11_loop ),
};

/* Char_Special 13 */
const Sint32 pl20_special_13[] = {
	CG_FRAME(    0,    0,   3,           0,   102, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   103, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   104, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   105, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_NEXT( pl20_special_13 ),
};

/* Char_Special 15 */
const Sint32 pl20_special_15[] = {
	CG_FRAME(    0,    0,   3,           0,   106, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   107, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   108, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   109, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,   110, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl20_special_15 ),
};

/* Char_Special 16 */
const Sint32 pl20_special_16[] = {
	CG_FRAME(    0,    0,   3,           0,   760, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   761, 0x80,   0x08,  ATK,  0x00,          0,    0,    0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   762, 0x80,   0x46,  ATK,  0x00,       0x19,    0,    0,     0,      0,   2 ),
	CG_FRAME( 0x80,    1,   3,           0,   763, 0x80,   0x81,  ATK,  0x00,          0,    0,    0,     0,      0, 119 ),
	CG_FRAME(    0,    0,   3,           0,   764, 0x80,   0x81,  ATK,  0x00,          0,    0,    0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   765, 0x80,   0x81,  ATK,  0x00,          0,    0,    0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   766, 0x80,   0x81,  ATK,  0x00,          0,    0,    0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   767, 0x80,   0x81,  ATK,  0x00,          0,    0,    0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   8,           0,   767, 0x80,   0x81,  ATE,  0x00,          0,    0,    0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   768, 0x80,   0x62,  ATE,  0x00,          0,    0,    0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   769, 0x80,   0x08,  ATE,  0x00,          0,    0,    0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   770, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   771, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   2 ),
	CG_NEXT( pl20_special_16 ),
};

/* Char_Special 17 */
const Sint32 pl20_special_17[] = {
	CG_FRAME(    0,    0,   3,           0,   760, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   761, 0x80,   0x08,  ATK,  0x00,          0,    0,    0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   762, 0x80,   0x46,  ATK,  0x00,       0x19,    0,    0,     0,      0,   2 ),
	CG_FRAME( 0x80,    1,   3,           0,   763, 0x80,   0x81,  ATK,  0x00,          0,    0,    0,     0,      0, 119 ),
	CG_FRAME(    0,    0,   3,           0,   764, 0x80,   0x81,  ATK,  0x00,          0,    0,    0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   765, 0x80,   0x81,  ATK,  0x00,          0,    0,    0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   766, 0x80,   0x81,  ATK,  0x00,          0,    0,    0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   767, 0x80,   0x81,  ATK,  0x00,          0,    0,    0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   8,           0,   767, 0x80,   0x81,  ATE,  0x00,          0,    0,    0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   768, 0x80,   0x62,  ATE,  0x00,          0,    0,    0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   769, 0x80,   0x08,  ATE,  0x00,          0,    0,    0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   770, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   771, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   2 ),
	CG_NEXT( pl20_special_17 ),
};

/* Char_Special 18: spike_init (Cat Spike, 18 + atck_level) */
const Sint32 pl20_special_18[] = {
	CG_FRAME(    0,    1,   3,           0,   627, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   3,           0,   628, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   2 ),
	CG_FRAME(    0,    1,   3,           0,   648, 0x80,   0x74,  ATK,  0x20,       0x19,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   2,           0,   649, 0x80,   0xb8,  ATK,  0x10,          0,    0,    0,     0,      0,  73 ),
	CG_FRAME(    0,    1,   2,           0,   650, 0x80,   0xb8,  ATK,  0x00,          0,    0,    0,     0,      0,  73 ),
	CG_FRAME(    0,    0,   3,           0,   651, 0x80,   0x80,  ATE,  0x00,          0,    0,    0,     0,      0,  74 ),
	CG_FRAME(    0,    0,   3,           0,   652, 0x80,   0x40,  ATE,  0x00,          0,    0,    0,     0,      0,   3 ),
};

const Sint32 pl20_special_18_loop[] = {
	CG_FRAME(    0,    0,   3,           0,   653, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   654, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_NEXT( pl20_special_18_loop ),
};

/* Char_Special 19 */
const Sint32 pl20_special_19[] = {
	CG_FRAME(    0,    1,   3,           0,   627, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    1,   3,           0,   628, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   629, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   638, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   639, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   640, 0x80,   0x00,  ATK,  0x00,       0x19,    0,    0,     0,      0,   3 ),
	CG_FRAME(    1,    0,   2,           0,   641, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  75 ),
	CG_FRAME(    2,    0,   2,           0,   642, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  76 ),
	CG_FRAME(    3,    0,   2,           0,   643, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   3 ),
};

const Sint32 pl20_special_19_loop[] = {
	CG_FRAME(    3,    0,   3,           0,   644, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    3,    0,   3,           0,   645, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    3,    0,   3,           0,   646, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    3,    0,   3, END_OF_DATA,   647, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_NEXT( pl20_special_19_loop ),
};

/* Char_Special 20 */
const Sint32 pl20_special_20[] = {
	CG_FRAME(    0,    1,   3,           0,   627, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    1,   3,           0,   628, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   629, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   630, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   631, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   632, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   633, 0x80,   0x00,  ATK,  0x00,       0x1c,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   632, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   633, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   632, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   633, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   634, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    4,    0,   3,           0,   635, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  77 ),
	CG_FRAME(    5,    0,   3,           0,   636, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  78 ),
	CG_FRAME(    6,    0,   3,           0,   637, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  79 ),
};

const Sint32 pl20_special_20_loop[] = {
	CG_FRAME(    6,    0,   3,           0,   644, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    6,    0,   3,           0,   645, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    6,    0,   3,           0,   646, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    6,    0,   3, END_OF_DATA,   647, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_NEXT( pl20_special_20_loop ),
};

/* Char_Special 21: spike_rise (Cat Spike, 21 + atck_level) */
const Sint32 pl20_special_21[] = {
	CG_FRAME(    0,    0,   3,           0,   655, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   628, 0x80,   0x00,  ATE,  0x18,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   656, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   657, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl20_special_21 ),
};

/* Char_Special 22 */
const Sint32 pl20_special_22[] = {
	CG_FRAME(    0,    0,   4,           0,   118, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   119, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   120, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   121, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   122, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl20_special_22 ),
};

/* Char_Special 23 */
const Sint32 pl20_special_23[] = {
	CG_FRAME(    0,    0,   4,           0,   118, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   119, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   120, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   121, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   122, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl20_special_23 ),
};

/* Char_Special 24: hellcat_init (Hell Cat) */
const Sint32 pl20_special_24[] = {
	CG_FRAME(    0,    0,   6,           0,   559, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0, 0x0001,   1 ),
	CG_FRAME(    0,    1,   6,           0,   560, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  14, END_OF_DATA,   561, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl20_special_24 ),
};

/* Char_Special 25: rolling_punch (Rolling Buckler) */
const Sint32 pl20_special_25[] = {
	CG_FRAME(    0,    1,   4,           0,   700, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   4,           0,   701, 0x80,   0x00,  ATK,  0x00,       0x1c,    0,    0,     0,      0,   1 ),
	CG_FRAME( 0x80,    1,   2,           0,   702, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  69 ),
	CG_FRAME(    0,    1,  10,           0,   703, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  70 ),
	CG_FRAME(    0,    1,   4,           0,   704, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   8, END_OF_DATA,   705, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl20_special_25 ),
};

/* Char_Special 26: rolling_kick (Rolling Buckler) */
const Sint32 pl20_special_26[] = {
	CG_FRAME(    0,    1,   4,           0,   706, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   2 ),
	CG_FRAME(    0,    1,   4,           0,   707, 0x80,   0x4a,  ATK,  0x00, 0x29 | CSE,    0,    0,     0,      0,   2 ),
	CG_FRAME( 0x80,    1,   2,           0,   708, 0x80,   0x88,  ATK,  0x00,          0,    0,    0,     0,      0,  71 ),
	CG_FRAME(    0,    1,   6,           0,   709, 0x80,   0x88,  ATK,  0x00,          0,    0,    0,     0,      0,  71 ),
	CG_FRAME(    0,    1,   8,           0,   709, 0x80,   0x88,  ATE,  0x00,          0,    0,    0,     0,      0,  72 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   707, 0x80,   0x4a,  ATE,  0x00,          0,    0,    0,     0,      0,  72 ),
	CG_NEXT( pl20_special_26 ),
};

/* Char_Special 27: snapback_init (Snapback) */
const Sint32 pl20_special_27[] = {
	CG_FRAME(    0,    0,   5,           0,   416, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   7 ),
	CG_FRAME(    0,    1,   2,           0,   416, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   7 ),
	CG_FRAME(    0,    2,   2,           0,   416, 0x80,   0x00,  ATK,  0x00,       0x1a,    0,    0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3,           0,   417, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  44 ),
	CG_FRAME(    0,    0,  10,           0,   418, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   419, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   7 ),
	CG_NEXT( pl20_special_27 ),
};

const Sint32 pl20_unused_00[] = {	/* nothing reaches it */
	CG_FRAME(    0,    1,   2,           0,   367, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    2,   4,           0,   368, 0x80,   0x00,  ATK,  0x00,       0x1a,    0,    0,     0,      0,   1 ),
	CG_FRAME( 0x80,    0,   2,           0,   369, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  20 ),
	CG_FRAME(    0,    0,   4,           0,   370, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  21 ),
	CG_FRAME(    0,    0,   3,           0,   371, 0x80,   0x00,    0,  0x00,          0,    0, 0x80,     0,      0,  22 ),
	CG_FRAME( 0x80,    0,   2,           0,   372, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  23 ),
	CG_FRAME(    0,    0,   4,           0,   373, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  24 ),
	CG_FRAME(    0,    0,  10,           0,   374, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  24 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   375, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl20_special_27 ),
};

/* Char_Super 0: dancing_init (Dancing Flash (hyper)) */
const Sint32 pl20_super_00[] = {
	CG_FRAME(    0,    0,   2,           0,   144, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   145, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   2,           0,   146, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   368, 0x80,   0x59,  ATK,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   716, 0x80,   0x59,  ATK,  0x20,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   717, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   1,           0,   678, 0x80,   0x00,  ATK,  0x00,       0x21,    0,    0,     0,      0,  81 ),
};

const Sint32 pl20_super_00_loop[] = {
	CG_FRAME(    0,    0,   1,           0,   679, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  81 ),
	CG_FRAME(    0,    0,   1,           0,   680, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  81 ),
	CG_FRAME(    0,    0,   1,           0,   681, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  81 ),
	CG_FRAME(    0,    0,   1,           0,   682, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  81 ),
	CG_FRAME(    0,    0,   1,           0,   683, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  81 ),
	CG_FRAME(    0,    0,   1,           0,   684, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  81 ),
	CG_FRAME(    0,    0,   1,           0,   685, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  81 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   678, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  81 ),
	CG_NEXT( pl20_super_00_loop ),
};

/* Char_Super 1: dancing_roll (Dancing Flash (hyper)) */
const Sint32 pl20_super_01[] = {
	CG_FRAME(    0,    0,   2,           0,   108, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   109, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   110, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   459, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   460, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME( 0x80,    0,   2,           0,   461, 0x80,   0x00,  ATK,  0x01,          0,    0,    0,     0,      0,  82 ),
	CG_FRAME(    0,    0,   2,           0,   462, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   463, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   464, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   465, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   466, 0x80,   0x00,  ATK,  0x02,          0,    0,    0,     0,      0,  83 ),
	CG_FRAME(    0,    0,   2,           0,   467, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   468, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   459, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   460, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME( 0x80,    0,   2,           0,   461, 0x80,   0x00,  ATK,  0x01,          0,    0,    0,     0,      0,  84 ),
	CG_FRAME(    0,    0,   2,           0,   462, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   463, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   464, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   465, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   466, 0x80,   0x00,  ATK,  0x02,          0,    0,    0,     0,      0,  85 ),
	CG_FRAME(    0,    0,   2,           0,   467, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   468, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   376, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   377, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   378, 0x80,   0x00,  ATK,  0x03,          0,    0,    0,     0,      0,  86 ),
	CG_FRAME(    0,    0,   2,           0,   379, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   359, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   360, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   361, 0x80,   0x00,  ATK,  0x02,          0,    0,    0,     0,      0,  87 ),
	CG_FRAME(    0,    0,   2,           0,   362, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   363, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   483, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   484, 0x80,   0x00,  ATK,  0x03,          0,    0,    0,     0,      0,  88 ),
	CG_FRAME(    0,    0,   3,           0,   485, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   487, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   488, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   489, 0x80,   0x00,  ATK,  0x04,          0,    0,    0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   2,           0,   490, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   487, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   3,           0,   710, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   711, 0x80,   0x00,  ATK,  0x00,       0x18,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   712, 0x80,   0x00,  ATK,  0x01,          0,    0,    0,     0,      0,  90 ),
	CG_FRAME(    0,    0,  12,           0,   713, 0x80,   0x00,  ATK,  0x02,          0,    0,    0,     0,      0,  91 ),
	CG_FRAME(    0,    0,   3,           0,   714, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   3,           0,   715, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    2,   3,           0,   102, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   3 ),
};

/* Char_Super 2: dancing_roll (Dancing Flash (hyper)) */
const Sint32 pl20_super_02[] = {
	CG_FRAME(    0,    0,   3,           0,   103, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   104, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   105, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   102, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_NEXT( pl20_super_02 ),
};

/* Char_Super 3: dancing_roll (Dancing Flash (hyper)), dancing_jump (Dancing Flash (hyper)), dancing_fall (Dancing Flash (hyper)) */
const Sint32 pl20_super_03[] = {
	CG_FRAME(    0,    0,   3,           0,   106, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   107, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   108, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   109, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   7, END_OF_DATA,   110, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl20_super_03 ),
};

/* Char_Super 4: help_init (Please Help Me (hyper)) */
const Sint32 pl20_super_04[] = {
	CG_FRAME(    0,    1,   4,           0,   741, 0x80,   0x00,  ATK,  0x00,       0x15,    0,    0,     0,      0,   1 ),
};

const Sint32 pl20_super_04_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   742, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   743, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   744, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   743, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl20_super_04_loop ),
};

/* Char_Super 5: help_check (Please Help Me (hyper)) */
const Sint32 pl20_super_05[] = {
	CG_FRAME(    0,    0,   4,           0,   745, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   746, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   747, 0x80,   0x00,  ATK,  0x00,       0x16,    0,    0,     0,      0,   0 ),
};

const Sint32 pl20_super_05_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   748, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   749, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   750, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   749, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_NEXT( pl20_super_05_loop ),
};

/* Char_Super 6: help_mash (Please Help Me (hyper)) */
const Sint32 pl20_super_06[] = {
	CG_FRAME(    0,    0,   4,           0,   751, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   752, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   753, 0x80,   0x00,  ATK,  0x00,       0x0f,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   754, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   755, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   756, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   757, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   758, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   759, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_NEXT( pl20_super_06 ),
};

/* Char_Super 7
   Char_Super 8
   Char_Super 9
   Char_Super 10: ssand_init (Super Sand Splash (hyper)) */
const Sint32 pl20_super_07[] = {
	CG_FRAME(    0,    0,   4,           0,   760, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   2 ),
	CG_FRAME(    0,    1,   4,           0,   761, 0x80,   0x08,  ATK,  0x00,          0,    0,    0,     0,      0,   2 ),
	CG_FRAME( 0x80,    1,   3,           0,   763, 0x80,   0x81,  ATK,  0x00,       0x18,    0,    0,     0,      0, 118 ),
	CG_FRAME(    0,    0,   3,           0,   764, 0x80,   0x81,  ATK,  0x00,          0,    0,    0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   765, 0x80,   0x81,  ATK,  0x00,          0,    0,    0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   766, 0x80,   0x81,  ATK,  0x00,          0,    0,    0,     0,      0,   2 ),
};

const Sint32 pl20_super_07_loop[] = {
	CG_FRAME(    0,    0,  12, END_OF_DATA,   767, 0x80,   0x81,  ATK,  0x00,          0,    0,    0,     0,      0,   2 ),
	CG_NEXT( pl20_super_07_loop ),
};

/* Char_Super 11: ssand_hold (Super Sand Splash (hyper)) */
const Sint32 pl20_super_11[] = {
	CG_FRAME(    0,    0,  16,           0,   767, 0x80,   0x81,  ATE,  0x00,          0,    0,    0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   768, 0x80,   0x62,  ATK,  0x00,          0,    0,    0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   769, 0x80,   0x08,  ATK,  0x00,          0,    0,    0,     0,      0,   2 ),
	CG_FRAME(    0,    0,  10,           0,   770, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   771, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   2 ),
	CG_NEXT( pl20_super_11 ),
};

/* Char_Effect 1: shl20_00_spike_fly, shl20_00_spike_bound, shl20_00_spike_vanish */
const Sint32 pl20_effect_01[] = {
	CG_FRAME(    0,    1,   1,           0,   670, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,  80 ),
	CG_FRAME(    0,    1,   2,           0,   670, 0x80,   0x00,    0,  0x00,       0x20,    0,    0,     0,      0,  80 ),
	CG_FRAME(    0,    1,   3,           0,   671, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,  80 ),
};

/* Char_Effect 0: shl20_00_spike_init, shl20_00_spike_bound, shl20_00_spike_vanish */
const Sint32 pl20_effect_00[] = {
	CG_FRAME(    0,    0,   3,           0,   668, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,  80 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   669, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,  80 ),
	CG_NEXT( pl20_effect_00 ),
};

/* Char_Effect 2: shl20_00_spike_fly */
const Sint32 pl20_effect_02[] = {
	CG_FRAME(    0,    0,   3,           0,   672, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   673, 0x80,   0x00,    0,  0x00,       0x1f,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   674, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   675, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   676, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   677, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_NEXT( pl20_effect_02 ),
};

/* Char_Effect 3: shl20_00_sand_init */
const Sint32 pl20_effect_03[] = {
	CG_FRAME(    0,    0,   3,           0,   772, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   773, 0x80,   0x00,  ATK,  0x00,       0x23,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   774, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0, 102 ),
	CG_FRAME(    0,    0,   3,           0,   775, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0, 102 ),
	CG_FRAME(    0,    0,   3,           0,   776, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0, 103 ),
	CG_FRAME(    0,    0,   3,           0,   777, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0, 104 ),
	CG_FRAME(    0,    0,   3,           0,   778, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0, 104 ),
	CG_FRAME(    0,    0,   3,           0,   779, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   780, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_NEXT( pl20_effect_03 ),
};

/* Char_Effect 4: shl20_01_0_init, shl20_01_0_land */
const Sint32 pl20_effect_04[] = {
	CG_FRAME(    0,    0,   4,           0,   569, 0x80,   0x20,  ATK,  0x00,          0,    0,    0,     0,      0, 101 ),
	CG_FRAME(    0,    0,   4,           0,   570, 0x80,   0x20,  ATK,  0x00,          0,    0,    0,     0,      0, 101 ),
	CG_FRAME(    0,    0,   4,           0,   571, 0x80,   0x20,  ATK,  0x00,          0,    0,    0,     0,      0, 101 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   572, 0x80,   0x20,  ATK,  0x00,          0,    0,    0,     0,      0, 101 ),
	CG_NEXT( pl20_effect_04 ),
};

/* Char_Effect 5: shl20_01_1_wait */
const Sint32 pl20_effect_05[] = {
	CG_FRAME(    0,    0,   4,           0,   584, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   585, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_NEXT( pl20_effect_05 ),
};

/* Char_Effect 6: shl20_01_2_wait */
const Sint32 pl20_effect_06[] = {
	CG_FRAME(    0,    0,   4,           0,   593, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   594, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_NEXT( pl20_effect_06 ),
};

/* Char_Effect 7: shl20_01_3_wait */
const Sint32 pl20_effect_07[] = {
	CG_FRAME(    0,    0,   4,           0,   602, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   603, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_NEXT( pl20_effect_07 ),
};

/* Char_Effect 8: shl20_01_0_fly, shl20_01_0_fall */
const Sint32 pl20_effect_08[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   583, 0x80,   0x20,  ATK,  0x00,          0,    0,    0,     0,      0, 101 ),
	CG_NEXT( pl20_effect_08 ),
};

/* Char_Catch 7: shl20_01_0_fly
   Char_Effect 9 */
const Sint32 pl20_catch_07[] = {
	CG_FRAME(    0,    0,   4,           0,   573, 0x80,   0x2e,    0,  0x00,          0,    0,    0,     0, 0x0030,   0 ),
	CG_FRAME(    0,    0,   4,           0,   574, 0x80,   0x2a,    0,  0x00,          0,    0,    0,     0, 0x0030,   0 ),
};

const Sint32 pl20_catch_07_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   575, 0x80,   0x2a,    0,  0x00,          0,    0,    0,     0, 0x0030,   0 ),
	CG_NEXT( pl20_catch_07_loop ),
};

/* Char_Effect 10: shl20_01_1_fly */
const Sint32 pl20_effect_10[] = {
	CG_FRAME(    0,    0,   4,           0,   586, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   587, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
};

const Sint32 pl20_effect_10_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   588, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_NEXT( pl20_effect_10_loop ),
};

/* Char_Effect 11: shl20_01_2_fly */
const Sint32 pl20_effect_11[] = {
	CG_FRAME(    0,    0,   4,           0,   595, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   596, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
};

const Sint32 pl20_effect_11_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   597, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_NEXT( pl20_effect_11_loop ),
};

/* Char_Effect 12: shl20_01_3_fly */
const Sint32 pl20_effect_12[] = {
	CG_FRAME(    0,    0,   4,           0,   604, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   605, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
};

const Sint32 pl20_effect_12_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   606, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_NEXT( pl20_effect_12_loop ),
};

/* Char_Effect 13: shl20_01_catch_end */
const Sint32 pl20_effect_13[] = {
	CG_FRAME(    0,    0,   4,           0,   576, 0x80,   0x2a,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   577, 0x80,   0x20,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   578, 0x80,   0x20,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
};

const Sint32 pl20_effect_13_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   579, 0x80,   0x20,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   580, 0x80,   0x20,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   581, 0x80,   0x20,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   582, 0x80,   0x20,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_NEXT( pl20_effect_13_loop ),
};

/* Char_Effect 14: shl20_01_1_catch_end */
const Sint32 pl20_effect_14[] = {
	CG_FRAME(    0,    0,   4,           0,   589, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   590, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
};

const Sint32 pl20_effect_14_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   591, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   592, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_NEXT( pl20_effect_14_loop ),
};

/* Char_Effect 15: shl20_01_2_catch_end */
const Sint32 pl20_effect_15[] = {
	CG_FRAME(    0,    0,   4,           0,   598, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   599, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
};

const Sint32 pl20_effect_15_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   600, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   601, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_NEXT( pl20_effect_15_loop ),
};

/* Char_Effect 16: shl20_01_3_catch_end */
const Sint32 pl20_effect_16[] = {
	CG_FRAME(    0,    0,   4,           0,   607, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   608, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
};

const Sint32 pl20_effect_16_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   609, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   610, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_NEXT( pl20_effect_16_loop ),
};

/* Char_Effect 17
   Char_Effect 18
   Char_Effect 19
   Char_Change 0: act_change_tag_init */
const Sint32 pl20_effect_17[] = {
	CG_FRAME(    0,    0,   3,           0,   231, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   232, 0x80,   0x00,  ATK,  0x00,       0x16,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   233, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   234, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   231, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   232, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   233, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   3,           0,   234, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   235, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_NEXT( pl20_effect_17 ),
};

/* Char_Change 1: act_change_vcounter_init */
const Sint32 pl20_change_01[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   230, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   6 ),
	CG_NEXT( pl20_change_01 ),
};

/* Char_Change 2: act_change_out_bound */
const Sint32 pl20_change_02[] = {
	CG_FRAME(    0,    0,   3,           0,    94, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,    95, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_NEXT( pl20_change_02 ),
};

/* Char_Change 3: act_stand, act_change_call */
const Sint32 pl20_change_03[] = {
	CG_FRAME(    0,    0,   4,           0,   244, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   245, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   246, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   247, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   248, 0x80,   0x00,    0,  0x00,       0x14,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   249, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   250, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   251, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   252, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   253, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   254, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   255, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   256, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   257, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   258, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   259, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   4,           0,   260, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   261, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   262, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   263, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl20_change_03 ),
};

const Sint32 pl20_unused_01[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,   4,           0,   264, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   265, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl20_change_03 ),
};

/* Char_Change 4: act_change_assist_end */
const Sint32 pl20_change_04[] = {
	CG_FRAME(    0,    0,   3,           0,   289, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   290, 0x80,   0x00,    0,  0x00,       0x1b,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   291, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   292, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   293, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   294, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   295, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   293, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   294, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   295, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   293, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   294, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   295, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   292, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   291, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   290, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   289, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl20_change_04 ),
};

/* Char_Caught 0 */
const Sint32 pl20_caught_00[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   830, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,  92 ),
	CG_NEXT( pl20_caught_00 ),
};

/* Char_Caught 1 */
const Sint32 pl20_caught_01[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   831, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,  92 ),
	CG_NEXT( pl20_caught_01 ),
};

/* Char_Caught 2 */
const Sint32 pl20_caught_02[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   832, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,  92 ),
	CG_NEXT( pl20_caught_02 ),
};

/* Char_Caught 3 */
const Sint32 pl20_caught_03[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   835, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,  92 ),
	CG_NEXT( pl20_caught_03 ),
};

/* Char_Caught 4 */
const Sint32 pl20_caught_04[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   836, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,  92 ),
	CG_NEXT( pl20_caught_04 ),
};

/* Char_Caught 5 */
const Sint32 pl20_caught_05[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   837, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,  92 ),
	CG_NEXT( pl20_caught_05 ),
};

/* Char_Caught 6 */
const Sint32 pl20_caught_06[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   840, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,  93 ),
	CG_NEXT( pl20_caught_06 ),
};

/* Char_Caught 7 */
const Sint32 pl20_caught_07[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   841, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,  93 ),
	CG_NEXT( pl20_caught_07 ),
};

/* Char_Caught 8 */
const Sint32 pl20_caught_08[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   842, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,  93 ),
	CG_NEXT( pl20_caught_08 ),
};

/* Char_Caught 9 */
const Sint32 pl20_caught_09[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   910, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,  94 ),
	CG_NEXT( pl20_caught_09 ),
};

/* Char_Caught 10 */
const Sint32 pl20_caught_10[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   911, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,  94 ),
	CG_NEXT( pl20_caught_10 ),
};

/* Char_Caught 11 */
const Sint32 pl20_caught_11[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   912, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,  94 ),
	CG_NEXT( pl20_caught_11 ),
};

/* Char_Caught 12 */
const Sint32 pl20_caught_12[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   849, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,  95 ),
	CG_NEXT( pl20_caught_12 ),
};

/* Char_Caught 13 */
const Sint32 pl20_caught_13[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   914, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,  92 ),
	CG_NEXT( pl20_caught_13 ),
};

/* Char_Caught 14 */
const Sint32 pl20_caught_14[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   916, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,  92 ),
	CG_NEXT( pl20_caught_14 ),
};

/* Char_Caught 15 */
const Sint32 pl20_caught_15[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   917, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,  92 ),
	CG_NEXT( pl20_caught_15 ),
};

/* Char_Caught 16 */
const Sint32 pl20_caught_16[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   918, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,  92 ),
	CG_NEXT( pl20_caught_16 ),
};

/* Char_Caught 17 */
const Sint32 pl20_caught_17[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   919, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,  92 ),
	CG_NEXT( pl20_caught_17 ),
};

/* Char_Caught 18 */
const Sint32 pl20_caught_18[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   887, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,  92 ),
	CG_NEXT( pl20_caught_18 ),
};

/* Char_Caught 19 */
const Sint32 pl20_caught_19[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   893, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,  92 ),
	CG_NEXT( pl20_caught_19 ),
};

/* Char_Caught 20 */
const Sint32 pl20_caught_20[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   857, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,  96 ),
	CG_NEXT( pl20_caught_20 ),
};

/* Char_Caught 21 */
const Sint32 pl20_caught_21[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   851, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,  97 ),
	CG_NEXT( pl20_caught_21 ),
};

/* group 27 0: pls20_00_rolling_init */
const Sint32 pl20_grp27_00[] = {
	EF_FRAME(    0,    0,   3,           0,   686 ),
	EF_FRAME(    0,    0,   3,           0,   687 ),
	EF_FRAME(    0,    0,   3,           0,   688 ),
	EF_FRAME(    0,    0,   3,           0,   689 ),
	EF_FRAME(    0,    0,   3,           0,   690 ),
	EF_FRAME(    0,    0,   3,           0,   691 ),
};

const Sint32 pl20_grp27_00_loop[] = {
	EF_FRAME(    0,    0,   3,           0,   692 ),
	EF_FRAME(    0,    0,   3,           0,   693 ),
	EF_FRAME(    0,    0,   3,           0,   694 ),
	EF_FRAME(    0,    0,   3,           0,   695 ),
	EF_FRAME(    0,    0,   3,           0,   696 ),
	EF_FRAME(    0,    0,   3,           0,   697 ),
	EF_FRAME(    0,    0,   3,           0,   698 ),
	EF_FRAME(    0,    0,   3, END_OF_DATA,   699 ),
	CG_NEXT( pl20_grp27_00_loop ),
};

/* group 27 1: pls20_00_spike_init */
const Sint32 pl20_grp27_01[] = {
	EF_FRAME(    0,    0,   3, END_OF_DATA,   663 ),
	CG_NEXT( pl20_grp27_01 ),
};

/* group 27 2 */
const Sint32 pl20_grp27_02[] = {
	EF_FRAME(    0,    0,   3, END_OF_DATA,   664 ),
	CG_NEXT( pl20_grp27_02 ),
};

/* group 27 3 */
const Sint32 pl20_grp27_03[] = {
	EF_FRAME(    0,    0,   3,           0,   665 ),
	EF_FRAME(    0,    0,   3,           0,   666 ),
	EF_FRAME(    0,    0,   3,           0,   667 ),
};

const Sint32 pl20_grp27_03_loop[] = {
	EF_FRAME(    0,    0,   3, END_OF_DATA,    -1 ),
	CG_NEXT( pl20_grp27_03_loop ),
};

/* group 27 4 */
const Sint32 pl20_grp27_04[] = {
	EF_FRAME(    0,    0,   3, END_OF_DATA,   658 ),
	CG_NEXT( pl20_grp27_04 ),
};

/* group 27 5 */
const Sint32 pl20_grp27_05[] = {
	EF_FRAME(    0,    0,   3, END_OF_DATA,   659 ),
	CG_NEXT( pl20_grp27_05 ),
};

/* group 27 6 */
const Sint32 pl20_grp27_06[] = {
	EF_FRAME(    0,    0,   3,           0,   660 ),
	EF_FRAME(    0,    0,   3,           0,   661 ),
	EF_FRAME(    0,    0,   3,           0,   662 ),
};

const Sint32 pl20_grp27_06_loop[] = {
	EF_FRAME(    0,    0,   3, END_OF_DATA,    -1 ),
	CG_NEXT( pl20_grp27_06_loop ),
};

/* group 27 7: pls20_00_help_part_init */
const Sint32 pl20_grp27_07[] = {
	EF_FRAME(    0,    0,   3,           0,   722 ),
	EF_FRAME(    0,    0,   3,           0,   723 ),
	EF_FRAME(    0,    0,   3,           0,   724 ),
	EF_FRAME(    0,    0,   3,           0,   725 ),
	EF_FRAME(    0,    0,   3,           0,   726 ),
	EF_FRAME(    0,    0,   3,           0,   727 ),
	EF_FRAME(    0,    0,   3,           0,   728 ),
	EF_FRAME(    0,    0,   3,           0,   729 ),
	EF_FRAME(    0,    0,   3, END_OF_DATA,   730 ),
	CG_NEXT( pl20_grp27_07 ),
};

/* group 27 8: pls20_00_help_init */
const Sint32 pl20_grp27_08[] = {
	EF_FRAME(    0,    0,   3,           0,   731 ),
	EF_FRAME(    0,    0,   3,           0,   732 ),
};

const Sint32 pl20_grp27_08_loop[] = {
	EF_FRAME(    0,    0,   3,           0,   733 ),
	EF_FRAME(    0,    0,   3,           0,   734 ),
	EF_FRAME(    0,    0,   3,           0,   735 ),
	EF_FRAME(    0,    0,   3,           0,   736 ),
	EF_FRAME(    0,    0,   3,           0,   737 ),
	EF_FRAME(    0,    0,   3, END_OF_DATA,   738 ),
	CG_NEXT( pl20_grp27_08_loop ),
};

/* group 27 9: pls20_00_help_wait */
const Sint32 pl20_grp27_09[] = {
	EF_FRAME(    0,    0,   3,           0,   739 ),
	EF_FRAME(    0,    0,   3, END_OF_DATA,   740 ),
	CG_NEXT( pl20_grp27_09 ),
};

/* Char_Effect 62: pls20_00_win_init (62 + sub_type) */
const Sint32 pl20_effect_62[] = {
	CG_FRAME(    0,    0,   3,           0,   339, 0x80,   0x20,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   340, 0x80,   0x20,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_NEXT( pl20_effect_62 ),
};

/* Char_Effect 66: pls20_00_win_move (66 + sub_type) */
const Sint32 pl20_effect_66[] = {
	CG_FRAME(    0,    0,   6,           0,   341, 0x80,   0x20,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
};

const Sint32 pl20_effect_66_loop[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   342, 0x80,   0x20,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_NEXT( pl20_effect_66_loop ),
};

/* Char_Effect 63 */
const Sint32 pl20_effect_63[] = {
	CG_FRAME(    0,    0,   3,           0,   343, 0x80,   0x20,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   344, 0x80,   0x20,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_NEXT( pl20_effect_63 ),
};

/* Char_Effect 67 */
const Sint32 pl20_effect_67[] = {
	CG_FRAME(    0,    0,   6,           0,   345, 0x80,   0x20,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
};

const Sint32 pl20_effect_67_loop[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   346, 0x80,   0x20,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_NEXT( pl20_effect_67_loop ),
};

/* Char_Effect 64 */
const Sint32 pl20_effect_64[] = {
	CG_FRAME(    0,    0,   3,           0,   347, 0x80,   0x20,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   348, 0x80,   0x20,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_NEXT( pl20_effect_64 ),
};

/* Char_Effect 68 */
const Sint32 pl20_effect_68[] = {
	CG_FRAME(    0,    0,   6,           0,   349, 0x80,   0x20,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
};

const Sint32 pl20_effect_68_loop[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   350, 0x80,   0x20,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_NEXT( pl20_effect_68_loop ),
};

/* Char_Effect 65 */
const Sint32 pl20_effect_65[] = {
	CG_FRAME(    0,    0,   3,           0,   351, 0x80,   0x20,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   352, 0x80,   0x20,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_NEXT( pl20_effect_65 ),
};

/* Char_Effect 69 */
const Sint32 pl20_effect_69[] = {
	CG_FRAME(    0,    0,   3,           0,   353, 0x80,   0x20,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
};

const Sint32 pl20_effect_69_loop[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   354, 0x80,   0x20,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_NEXT( pl20_effect_69_loop ),
};

/* Char_Effect 20 */
const Sint32 pl20_effect_20[] = {
	CG_FRAME(    0,    0, 127, END_OF_DATA, 32768, 0x80,   0x00,  ATK,  0x00,          0,    2,    0,     0,      0, 117 ),
	CG_NEXT( pl20_effect_20 ),
};

/* Char_Effect 21 */
const Sint32 pl20_effect_21[] = {
	CG_FRAME(    0,    0, 127, END_OF_DATA, 32769, 0x80,   0x00,  ATK,  0x00,          0,    2,    0,     0,      0, 117 ),
	CG_NEXT( pl20_effect_21 ),
};

/* Char_Effect 22 */
const Sint32 pl20_effect_22[] = {
	CG_FRAME(    0,    0, 127, END_OF_DATA, 32770, 0x80,   0x00,  ATK,  0x00,          0,    2,    0,     0,      0, 117 ),
	CG_NEXT( pl20_effect_22 ),
};

/* Char_Effect 23 */
const Sint32 pl20_effect_23[] = {
	CG_FRAME(    0,    0, 127, END_OF_DATA, 32771, 0x80,   0x00,  ATK,  0x00,          0,    2,    0,     0,      0, 117 ),
	CG_NEXT( pl20_effect_23 ),
};

/* Char_Effect 24 */
const Sint32 pl20_effect_24[] = {
	CG_FRAME(    0,    0, 127, END_OF_DATA, 32772, 0x80,   0x00,  ATK,  0x00,          0,    2,    0,     0,      0, 117 ),
	CG_NEXT( pl20_effect_24 ),
};

/* Char_Effect 25 */
const Sint32 pl20_effect_25[] = {
	CG_FRAME(    0,    0, 127, END_OF_DATA, 32773, 0x80,   0x00,  ATK,  0x00,          0,    2,    0,     0,      0, 117 ),
	CG_NEXT( pl20_effect_25 ),
};

/* Char_Effect 26 */
const Sint32 pl20_effect_26[] = {
	CG_FRAME(    0,    0, 127, END_OF_DATA, 32774, 0x80,   0x00,  ATK,  0x00,          0,    2,    0,     0,      0, 117 ),
	CG_NEXT( pl20_effect_26 ),
};

/* Char_Effect 27 */
const Sint32 pl20_effect_27[] = {
	CG_FRAME(    0,    0, 127, END_OF_DATA, 32775, 0x80,   0x00,  ATK,  0x00,          0,    2,    0,     0,      0, 117 ),
	CG_NEXT( pl20_effect_27 ),
};

/* Char_Effect 28 */
const Sint32 pl20_effect_28[] = {
	CG_FRAME(    0,    0, 127, END_OF_DATA, 32776, 0x80,   0x00,  ATK,  0x00,          0,    2,    0,     0,      0, 117 ),
	CG_NEXT( pl20_effect_28 ),
};

/* Char_Effect 29 */
const Sint32 pl20_effect_29[] = {
	CG_FRAME(    0,    0, 127, END_OF_DATA, 32777, 0x80,   0x00,  ATK,  0x00,          0,    2,    0,     0,      0, 117 ),
	CG_NEXT( pl20_effect_29 ),
};

/* Char_Effect 30 */
const Sint32 pl20_effect_30[] = {
	CG_FRAME(    0,    0, 127, END_OF_DATA, 32778, 0x80,   0x00,  ATK,  0x00,          0,    2,    0,     0,      0, 117 ),
	CG_NEXT( pl20_effect_30 ),
};

/* Char_Effect 31 */
const Sint32 pl20_effect_31[] = {
	CG_FRAME(    0,    0, 127, END_OF_DATA, 32779, 0x80,   0x00,  ATK,  0x00,          0,    2,    0,     0,      0,   0 ),
	CG_NEXT( pl20_effect_31 ),
};

/* Char_Effect 32 */
const Sint32 pl20_effect_32[] = {
	CG_FRAME(    0,    0, 127, END_OF_DATA, 32780, 0x80,   0x00,  ATK,  0x00,          0,    2,    0,     0,      0,   0 ),
	CG_NEXT( pl20_effect_32 ),
};

/* Char_Effect 33 */
const Sint32 pl20_effect_33[] = {
	CG_FRAME(    0,    0, 127, END_OF_DATA, 32781, 0x80,   0x00,  ATK,  0x00,          0,    2,    0,     0,      0,   0 ),
	CG_NEXT( pl20_effect_33 ),
};

/* Char_Effect 34 */
const Sint32 pl20_effect_34[] = {
	CG_FRAME(    0,    0, 127, END_OF_DATA, 32782, 0x80,   0x00,  ATK,  0x00,          0,    2,    0,     0,      0,   0 ),
	CG_NEXT( pl20_effect_34 ),
};

/* Char_Effect 35 */
const Sint32 pl20_effect_35[] = {
	CG_FRAME(    0,    0, 127, END_OF_DATA, 32783, 0x80,   0x00,  ATK,  0x00,          0,    2,    0,     0,      0,   0 ),
	CG_NEXT( pl20_effect_35 ),
};

/* Char_Effect 36 */
const Sint32 pl20_effect_36[] = {
	CG_FRAME(    0,    0, 127, END_OF_DATA, 32784, 0x80,   0x00,  ATK,  0x00,          0,    2,    0,     0,      0,   0 ),
	CG_NEXT( pl20_effect_36 ),
};

/* Char_Effect 37 */
const Sint32 pl20_effect_37[] = {
	CG_FRAME(    0,    0, 127, END_OF_DATA, 32785, 0x80,   0x00,  ATK,  0x00,          0,    2,    0,     0,      0,   0 ),
	CG_NEXT( pl20_effect_37 ),
};

/* Char_Effect 38 */
const Sint32 pl20_effect_38[] = {
	CG_FRAME(    0,    0, 127, END_OF_DATA, 32786, 0x80,   0x00,  ATK,  0x00,          0,    2,    0,     0,      0,   0 ),
	CG_NEXT( pl20_effect_38 ),
};

/* Char_Effect 39 */
const Sint32 pl20_effect_39[] = {
	CG_FRAME(    0,    0, 127, END_OF_DATA, 32787, 0x80,   0x00,  ATK,  0x00,          0,    2,    0,     0,      0,   0 ),
	CG_NEXT( pl20_effect_39 ),
};

/* Char_Effect 40 */
const Sint32 pl20_effect_40[] = {
	CG_FRAME(    0,    0, 127, END_OF_DATA, 32788, 0x80,   0x00,  ATK,  0x00,          0,    2,    0,     0,      0,   0 ),
	CG_NEXT( pl20_effect_40 ),
};

/* Char_Effect 41 */
const Sint32 pl20_effect_41[] = {
	CG_FRAME(    0,    0, 127, END_OF_DATA, 32789, 0x80,   0x00,  ATK,  0x00,          0,    2,    0,     0,      0, 117 ),
	CG_NEXT( pl20_effect_41 ),
};

/* Char_Effect 42 */
const Sint32 pl20_effect_42[] = {
	CG_FRAME(    0,    0, 127, END_OF_DATA, 32790, 0x80,   0x00,  ATK,  0x00,          0,    2,    0,     0,      0, 117 ),
	CG_NEXT( pl20_effect_42 ),
};

/* Char_Effect 43 */
const Sint32 pl20_effect_43[] = {
	CG_FRAME(    0,    0, 127, END_OF_DATA, 32791, 0x80,   0x00,  ATK,  0x00,          0,    2,    0,     0,      0, 117 ),
	CG_NEXT( pl20_effect_43 ),
};

/* Char_Effect 44 */
const Sint32 pl20_effect_44[] = {
	CG_FRAME(    0,    0, 127, END_OF_DATA, 32792, 0x80,   0x00,  ATK,  0x00,          0,    2,    0,     0,      0, 117 ),
	CG_NEXT( pl20_effect_44 ),
};

/* Char_Effect 45 */
const Sint32 pl20_effect_45[] = {
	CG_FRAME(    0,    0, 127, END_OF_DATA, 32793, 0x80,   0x00,  ATK,  0x00,          0,    2,    0,     0,      0, 117 ),
	CG_NEXT( pl20_effect_45 ),
};

/* Char_Effect 46 */
const Sint32 pl20_effect_46[] = {
	CG_FRAME(    0,    0, 127, END_OF_DATA, 32794, 0x80,   0x00,  ATK,  0x00,          0,    2,    0,     0,      0, 117 ),
	CG_NEXT( pl20_effect_46 ),
};

/* Char_Effect 47 */
const Sint32 pl20_effect_47[] = {
	CG_FRAME(    0,    0, 127, END_OF_DATA, 32795, 0x80,   0x00,  ATK,  0x00,          0,    2,    0,     0,      0, 117 ),
	CG_NEXT( pl20_effect_47 ),
};

/* Char_Effect 48 */
const Sint32 pl20_effect_48[] = {
	CG_FRAME(    0,    0, 127, END_OF_DATA, 32796, 0x80,   0x00,  ATK,  0x00,          0,    2,    0,     0,      0, 117 ),
	CG_NEXT( pl20_effect_48 ),
};

/* Char_Effect 49 */
const Sint32 pl20_effect_49[] = {
	CG_FRAME(    0,    0, 127, END_OF_DATA, 32797, 0x80,   0x00,  ATK,  0x00,          0,    2,    0,     0,      0, 117 ),
	CG_NEXT( pl20_effect_49 ),
};

/* Char_Effect 50 */
const Sint32 pl20_effect_50[] = {
	CG_FRAME(    0,    0, 127, END_OF_DATA, 32798, 0x80,   0x00,  ATK,  0x00,          0,    2,    0,     0,      0, 117 ),
	CG_NEXT( pl20_effect_50 ),
};

/* Char_Effect 51 */
const Sint32 pl20_effect_51[] = {
	CG_FRAME(    0,    0, 127, END_OF_DATA, 32799, 0x80,   0x00,  ATK,  0x00,          0,    2,    0,     0,      0, 117 ),
	CG_NEXT( pl20_effect_51 ),
};

/* Char_Effect 52 */
const Sint32 pl20_effect_52[] = {
	CG_FRAME(    0,    0, 127, END_OF_DATA, 32800, 0x80,   0x00,  ATK,  0x00,          0,    2,    0,     0,      0, 117 ),
	CG_NEXT( pl20_effect_52 ),
};

/* Char_Effect 53 */
const Sint32 pl20_effect_53[] = {
	CG_FRAME(    0,    0, 127, END_OF_DATA, 32801, 0x80,   0x00,  ATK,  0x00,          0,    2,    0,     0,      0, 117 ),
	CG_NEXT( pl20_effect_53 ),
};

/* Char_Effect 54 */
const Sint32 pl20_effect_54[] = {
	CG_FRAME(    0,    0, 127, END_OF_DATA, 32802, 0x80,   0x00,  ATK,  0x00,          0,    2,    0,     0,      0, 117 ),
	CG_NEXT( pl20_effect_54 ),
};

/* Char_Effect 55 */
const Sint32 pl20_effect_55[] = {
	CG_FRAME(    0,    0, 127, END_OF_DATA, 32803, 0x80,   0x00,  ATK,  0x00,          0,    2,    0,     0,      0, 117 ),
	CG_NEXT( pl20_effect_55 ),
};

/* Char_Effect 56 */
const Sint32 pl20_effect_56[] = {
	CG_FRAME(    0,    0, 127, END_OF_DATA, 32804, 0x80,   0x00,  ATK,  0x00,          0,    2,    0,     0,      0, 117 ),
	CG_NEXT( pl20_effect_56 ),
};

/* Char_Effect 57 */
const Sint32 pl20_effect_57[] = {
	CG_FRAME(    0,    0, 127, END_OF_DATA, 32805, 0x80,   0x00,  ATK,  0x00,          0,    2,    0,     0,      0, 117 ),
	CG_NEXT( pl20_effect_57 ),
};

/* Char_Effect 58 */
const Sint32 pl20_effect_58[] = {
	CG_FRAME(    0,    0, 127, END_OF_DATA, 32806, 0x80,   0x00,  ATK,  0x00,          0,    2,    0,     0,      0, 117 ),
	CG_NEXT( pl20_effect_58 ),
};

/* Char_Effect 59 */
const Sint32 pl20_effect_59[] = {
	CG_FRAME(    0,    0, 127, END_OF_DATA, 32807, 0x80,   0x00,  ATK,  0x00,          0,    2,    0,     0,      0, 117 ),
	CG_NEXT( pl20_effect_59 ),
};

/* Char_Effect 60 */
const Sint32 pl20_effect_60[] = {
	CG_FRAME(    0,    0, 127, END_OF_DATA, 32808, 0x80,   0x00,  ATK,  0x00,          0,    2,    0,     0,      0, 117 ),
	CG_NEXT( pl20_effect_60 ),
};

/* Char_Effect 61 */
const Sint32 pl20_effect_61[] = {
	CG_FRAME(    0,    0, 127, END_OF_DATA, 32809, 0x80,   0x00,  ATK,  0x00,          0,    2,    0,     0,      0, 117 ),
	CG_NEXT( pl20_effect_61 ),
};
