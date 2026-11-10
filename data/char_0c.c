/* Spiderman (character 0c): the animations. PLAYER_WORK's char_adrs points here.

   spr_prm_0c      a sprite's sizes and turn (SPR_PRM): a frame's zoom number picks a row, 0 for none.
   char_tbl_0c     the groups: pl_char_set( pl_w, group, no ) takes the group's table from here and
                   starts its animation no. The character's module (s_pl0c.c) says which group
                   and number each move uses.
   pl0c_GROUP_tbl  a group's animations. GROUP is the group's name in player.h (stand for
                   Char_Stand), or grpNN for a group with no name there.
   pl0c_GROUP_NN   an animation, named by the first group and number that has it: its frames in
                   order. The comment before it lists its places in the groups and, for each,
                   the routines that start it by that number, with their special move;
                   (2 + atck_level) is a routine that counts on from 2 by the attack's level.
                   A frame with END_OF_DATA is the last, and the word after it is the frame the
                   animation goes on from (CG_NEXT; pl0c_GROUP_NN_loop where that is no
                   animation's first frame). An effect's animation has short frames (EF_FRAME)
                   and may end with no word.
   pl0c_unused_NN  frames that no table or animation reaches.

   A player's frame (CG_FRAME, the 0x14 bytes of CHAR_DATA), with the names Capcom's tables used:
     prm, type  a number and a code for the character's module (a speed, an attack, a voice)
     ctr        the game frames it is shown for
     end        END_OF_DATA on the last frame
     spr        the sprite, -1 for none
     prio       added to the draw priority (a signed byte)
     shadow     the shadow's offset (the low five bits) and size, 0xff none
     atck       ATK while an attack is on its way or can hit, ATE in its frames after that;
                CG_ATCK_NG lets nothing cancel it
     flag1      a number for the module: most often the attack, a row of hit_dt_0c
     se         a voice (1 to 63), or with CSE a sound effect; 0 none
     zoom       the row of spr_prm_0c, 0 none
     unk        a byte the program is not seen to read
     flag2      a second code for the character's effects
     rival      where a held opponent is put (the low byte), and a code for the module (the high)
     box        the frame's row of frame_box_0c */
#include "player.h"
#include "char_tbl.h"

extern const Sint32 pl0c_stand_00[], pl0c_stand_02[], pl0c_stand_03[], pl0c_stand_06[],
	pl0c_stand_07[], pl0c_jump_00[], pl0c_jump_00_loop[], pl0c_jump_01[], pl0c_jump_01_loop[],
	pl0c_jump_02[], pl0c_jump_02_loop[], pl0c_jump_03[], pl0c_jump_06[], pl0c_jump_09[],
	pl0c_jump_07[], pl0c_jump_07_loop[], pl0c_jump_08[], pl0c_jump_08_loop[], pl0c_dash_00[],
	pl0c_dash_00_loop[], pl0c_dash_01[], pl0c_dash_02[], pl0c_dash_03[], pl0c_crouch_00[],
	pl0c_crouch_01[], pl0c_crouch_02[], pl0c_turn_00[], pl0c_turn_01[], pl0c_guard_00[],
	pl0c_guard_03[], pl0c_guard_01[], pl0c_guard_02[], pl0c_guard_04[], pl0c_guard_07[],
	pl0c_guard_05[], pl0c_guard_06[], pl0c_air_guard_00[], pl0c_air_guard_02[], pl0c_air_guard_01[],
	pl0c_stand_p_00[], pl0c_stand_p_01[], pl0c_stand_p_02[], pl0c_stand_p_03[], pl0c_stand_k_00[],
	pl0c_stand_k_01[], pl0c_stand_k_02[], pl0c_crouch_p_00[], pl0c_crouch_p_01[], pl0c_crouch_p_02[],
	pl0c_crouch_p_03[], pl0c_crouch_k_00[], pl0c_crouch_k_01[], pl0c_crouch_k_02[], pl0c_jump_p_00[],
	pl0c_jump_p_00_loop[], pl0c_jump_p_01[], pl0c_jump_p_01_loop[], pl0c_jump_p_02[],
	pl0c_jump_p_02_loop[], pl0c_jump_k_00[], pl0c_jump_k_00_loop[], pl0c_jump_k_01[],
	pl0c_jump_k_01_loop[], pl0c_jump_k_02[], pl0c_jump_k_02_loop[], pl0c_damage_00[],
	pl0c_damage_01[], pl0c_damage_02[], pl0c_damage_03[], pl0c_damage_04[], pl0c_damage_05[],
	pl0c_damage_06[], pl0c_damage_07[], pl0c_damage_08[], pl0c_damage_09[], pl0c_damage_09_loop[],
	pl0c_damage_10[], pl0c_damage_11[], pl0c_damage_12[], pl0c_damage_13[], pl0c_damage_16[],
	pl0c_damage_17[], pl0c_damage_18[], pl0c_damage_19[], pl0c_damage_20[], pl0c_damage_21[],
	pl0c_damage_22[], pl0c_damage_23[], pl0c_damage_24[], pl0c_damage_25[], pl0c_damage_26[],
	pl0c_damage_27[], pl0c_damage_27_loop[], pl0c_damage_28[], pl0c_damage_29[], pl0c_damage_30[],
	pl0c_damage_30_loop[], pl0c_damage_31[], pl0c_damage_32[], pl0c_damage_33[], pl0c_damage_34[],
	pl0c_dizzy_00[], pl0c_catch_00[], pl0c_catch_01[], pl0c_catch_01_loop[], pl0c_catch_02[],
	pl0c_catch_02_loop[], pl0c_caught_00[], pl0c_caught_01[], pl0c_caught_02[], pl0c_caught_03[],
	pl0c_caught_04[], pl0c_caught_05[], pl0c_caught_06[], pl0c_caught_07[], pl0c_caught_08[],
	pl0c_caught_09[], pl0c_caught_10[], pl0c_caught_11[], pl0c_caught_12[], pl0c_caught_13[],
	pl0c_caught_14[], pl0c_caught_15[], pl0c_caught_16[], pl0c_caught_17[], pl0c_caught_18[],
	pl0c_caught_19[], pl0c_caught_20[], pl0c_get_up_00[], pl0c_get_up_01[], pl0c_unused_00[],
	pl0c_get_up_02[], pl0c_get_up_03[], pl0c_appear_00[], pl0c_appear_01[], pl0c_appear_01_loop[],
	pl0c_appear_02[], pl0c_appear_02_loop[], pl0c_appear_03[], pl0c_appear_04[],
	pl0c_appear_04_loop[], pl0c_win_00[], pl0c_win_00_loop[], pl0c_win_01[], pl0c_win_01_loop[],
	pl0c_win_02[], pl0c_win_02_loop[], pl0c_unused_01[], pl0c_win_03[], pl0c_win_03_loop[],
	pl0c_jump_12[], pl0c_extra_00[], pl0c_extra_01[], pl0c_extra_02[], pl0c_special_00[],
	pl0c_special_00_loop[], pl0c_special_01[], pl0c_special_01_loop[], pl0c_special_03[],
	pl0c_special_03_loop[], pl0c_special_04[], pl0c_special_04_loop[], pl0c_special_05[],
	pl0c_special_06[], pl0c_special_02[], pl0c_special_02_loop[], pl0c_special_07[],
	pl0c_special_08[], pl0c_special_09[], pl0c_special_09_loop[], pl0c_special_10[],
	pl0c_special_10_loop[], pl0c_special_11[], pl0c_special_11_loop[], pl0c_special_12[],
	pl0c_special_12_loop[], pl0c_special_17[], pl0c_special_17_loop[], pl0c_special_13[],
	pl0c_special_13_loop[], pl0c_special_14[], pl0c_special_14_loop[], pl0c_special_15[],
	pl0c_special_15_loop[], pl0c_special_16[], pl0c_special_16_loop[], pl0c_special_18[],
	pl0c_special_18_loop[], pl0c_special_19[], pl0c_special_19_loop[], pl0c_special_21[],
	pl0c_special_22[], pl0c_catch_03[], pl0c_catch_03_loop[], pl0c_catch_04[], pl0c_special_24[],
	pl0c_special_25[], pl0c_special_26[], pl0c_super_00[], pl0c_super_01[], pl0c_super_01_loop[],
	pl0c_super_02[], pl0c_super_02_loop[], pl0c_super_03[], pl0c_super_03_loop[], pl0c_super_12[],
	pl0c_super_13[], pl0c_super_14[], pl0c_super_15[], pl0c_super_16[], pl0c_super_17[],
	pl0c_super_18[], pl0c_super_19[], pl0c_super_20[], pl0c_super_21[], pl0c_super_22[],
	pl0c_super_23[], pl0c_super_24[], pl0c_super_25[], pl0c_super_26[], pl0c_super_27[],
	pl0c_super_04[], pl0c_super_05[], pl0c_super_06[], pl0c_super_07[], pl0c_super_10[],
	pl0c_super_11[], pl0c_super_08[], pl0c_super_09[], pl0c_special_30[], pl0c_change_04[],
	pl0c_unused_02[], pl0c_special_28[], pl0c_special_28_loop[], pl0c_special_29[], pl0c_change_00[],
	pl0c_change_00_loop[], pl0c_change_02[], pl0c_change_02_loop[], pl0c_change_03[],
	pl0c_change_03_loop[], pl0c_super_29[], pl0c_super_29_loop[], pl0c_super_31[], pl0c_super_32[],
	pl0c_super_32_loop[], pl0c_super_34[], pl0c_super_34_loop[], pl0c_super_35[],
	pl0c_super_35_loop[], pl0c_super_36[], pl0c_super_36_loop[], pl0c_super_37[], pl0c_change_01[],
	pl0c_special_31[], pl0c_effect_23[], pl0c_effect_23_loop[], pl0c_effect_24[],
	pl0c_effect_24_loop[], pl0c_effect_16[], pl0c_effect_17[], pl0c_effect_18[], pl0c_effect_19[],
	pl0c_effect_20[], pl0c_effect_39[], pl0c_effect_40[], pl0c_effect_41[], pl0c_effect_42[],
	pl0c_effect_43[], pl0c_effect_44[], pl0c_effect_01[], pl0c_effect_01_loop[], pl0c_effect_03[],
	pl0c_effect_03_loop[], pl0c_effect_02[], pl0c_effect_04[], pl0c_effect_05[], pl0c_effect_06[],
	pl0c_effect_06_loop[], pl0c_effect_07[], pl0c_effect_07_loop[], pl0c_effect_08[],
	pl0c_effect_09[], pl0c_effect_10[], pl0c_effect_11[], pl0c_effect_12[], pl0c_effect_13[],
	pl0c_effect_14[], pl0c_effect_15[], pl0c_effect_00[], pl0c_effect_00_loop[], pl0c_effect_36[],
	pl0c_effect_36_loop[], pl0c_effect_45[], pl0c_effect_46[], pl0c_effect_47[], pl0c_effect_48[],
	pl0c_effect_49[], pl0c_effect_50[], pl0c_effect_51[], pl0c_effect_52[], pl0c_effect_53[],
	pl0c_effect_54[], pl0c_effect_65[], pl0c_effect_65_loop[], pl0c_effect_66[],
	pl0c_effect_66_loop[], pl0c_effect_67[], pl0c_effect_67_loop[], pl0c_effect_68[],
	pl0c_effect_68_loop[], pl0c_effect_69[], pl0c_effect_69_loop[], pl0c_effect_70[],
	pl0c_effect_70_loop[], pl0c_effect_71[], pl0c_effect_71_loop[], pl0c_effect_72[],
	pl0c_effect_72_loop[], pl0c_effect_73[], pl0c_effect_73_loop[], pl0c_effect_74[],
	pl0c_effect_74_loop[], pl0c_effect_37[], pl0c_effect_37_loop[], pl0c_effect_55[],
	pl0c_effect_56[], pl0c_effect_57[], pl0c_effect_58[], pl0c_effect_59[], pl0c_effect_60[],
	pl0c_effect_61[], pl0c_effect_62[], pl0c_effect_63[], pl0c_effect_64[], pl0c_effect_75[],
	pl0c_effect_75_loop[], pl0c_effect_76[], pl0c_effect_76_loop[], pl0c_effect_77[],
	pl0c_effect_77_loop[], pl0c_effect_78[], pl0c_effect_78_loop[], pl0c_effect_79[],
	pl0c_effect_79_loop[], pl0c_effect_80[], pl0c_effect_80_loop[], pl0c_effect_81[],
	pl0c_effect_81_loop[], pl0c_effect_82[], pl0c_effect_82_loop[], pl0c_effect_83[],
	pl0c_effect_83_loop[], pl0c_effect_84[], pl0c_effect_84_loop[], pl0c_effect_21[],
	pl0c_catch_05[], pl0c_catch_06[], pl0c_catch_07[], pl0c_catch_08[], pl0c_catch_09[],
	pl0c_catch_10[], pl0c_effect_22[], pl0c_effect_22_loop[], pl0c_effect_26[], pl0c_effect_27[],
	pl0c_effect_28[], pl0c_effect_29[], pl0c_effect_30[], pl0c_effect_31[], pl0c_effect_32[],
	pl0c_effect_33[], pl0c_effect_34[], pl0c_effect_35[];
extern const Sint32 *const pl0c_stand_tbl[], *const pl0c_jump_tbl[], *const pl0c_dash_tbl[],
	*const pl0c_crouch_tbl[], *const pl0c_turn_tbl[], *const pl0c_guard_tbl[],
	*const pl0c_air_guard_tbl[], *const pl0c_stand_p_tbl[], *const pl0c_stand_k_tbl[],
	*const pl0c_crouch_p_tbl[], *const pl0c_crouch_k_tbl[], *const pl0c_jump_p_tbl[],
	*const pl0c_jump_k_tbl[], *const pl0c_damage_tbl[], *const pl0c_dizzy_tbl[],
	*const pl0c_catch_tbl[], *const pl0c_caught_tbl[], *const pl0c_get_up_tbl[],
	*const pl0c_appear_tbl[], *const pl0c_win_tbl[], *const pl0c_extra_tbl[],
	*const pl0c_special_tbl[], *const pl0c_super_tbl[], *const pl0c_effect_tbl[],
	*const pl0c_change_tbl[], *const pl0c_grp25_tbl[], *const pl0c_fly_tbl[],
	*const pl0c_grp27_tbl[];

/* the sprites' sizes and turns, by a frame's zoom number */
const SPR_PRM spr_prm_0c[16] = {
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
const Sint32 *const *const char_tbl_0c[] = {
	pl0c_stand_tbl, pl0c_jump_tbl, pl0c_dash_tbl, pl0c_crouch_tbl, pl0c_turn_tbl, pl0c_guard_tbl,
	pl0c_air_guard_tbl, pl0c_stand_p_tbl, pl0c_stand_k_tbl, pl0c_crouch_p_tbl, pl0c_crouch_k_tbl, pl0c_jump_p_tbl,
	pl0c_jump_k_tbl, pl0c_damage_tbl, pl0c_dizzy_tbl, pl0c_catch_tbl, pl0c_caught_tbl, pl0c_get_up_tbl,
	pl0c_appear_tbl, pl0c_win_tbl, pl0c_extra_tbl, pl0c_special_tbl, pl0c_super_tbl, pl0c_effect_tbl,
	pl0c_change_tbl, pl0c_grp25_tbl, pl0c_fly_tbl, pl0c_grp27_tbl, 0,
};

const Sint32 *const pl0c_stand_tbl[] = {
	pl0c_stand_00, pl0c_stand_00, pl0c_stand_02, pl0c_stand_03, pl0c_stand_00, pl0c_stand_00,
	pl0c_stand_06, pl0c_stand_07, pl0c_stand_00, pl0c_stand_00, 0,
};

const Sint32 *const pl0c_jump_tbl[] = {
	pl0c_jump_00, pl0c_jump_01, pl0c_jump_02, pl0c_jump_03, pl0c_jump_03, pl0c_jump_03,
	pl0c_jump_06, pl0c_jump_07, pl0c_jump_08, pl0c_jump_09, pl0c_jump_09, pl0c_jump_09,
	pl0c_jump_12, 0,
};

const Sint32 *const pl0c_dash_tbl[] = {
	pl0c_dash_00, pl0c_dash_01, pl0c_dash_02, pl0c_dash_03, 0,
};

const Sint32 *const pl0c_crouch_tbl[] = {
	pl0c_crouch_00, pl0c_crouch_01, pl0c_crouch_02, 0,
};

const Sint32 *const pl0c_turn_tbl[] = {
	pl0c_turn_00, pl0c_turn_01, 0,
};

const Sint32 *const pl0c_guard_tbl[] = {
	pl0c_guard_00, pl0c_guard_01, pl0c_guard_02, pl0c_guard_03, pl0c_guard_04, pl0c_guard_05,
	pl0c_guard_06, pl0c_guard_07, 0,
};

const Sint32 *const pl0c_air_guard_tbl[] = {
	pl0c_air_guard_00, pl0c_air_guard_01, pl0c_air_guard_02, 0,
};

const Sint32 *const pl0c_stand_p_tbl[] = {
	pl0c_stand_p_00, pl0c_stand_p_01, pl0c_stand_p_02, pl0c_stand_p_03, 0,
};

const Sint32 *const pl0c_stand_k_tbl[] = {
	pl0c_stand_k_00, pl0c_stand_k_01, pl0c_stand_k_02, 0,
};

const Sint32 *const pl0c_crouch_p_tbl[] = {
	pl0c_crouch_p_00, pl0c_crouch_p_01, pl0c_crouch_p_02, pl0c_crouch_p_03, 0,
};

const Sint32 *const pl0c_crouch_k_tbl[] = {
	pl0c_crouch_k_00, pl0c_crouch_k_01, pl0c_crouch_k_02, 0,
};

const Sint32 *const pl0c_jump_p_tbl[] = {
	pl0c_jump_p_00, pl0c_jump_p_01, pl0c_jump_p_02, pl0c_jump_p_00, pl0c_jump_p_01, pl0c_jump_p_02,
	0,
};

const Sint32 *const pl0c_jump_k_tbl[] = {
	pl0c_jump_k_00, pl0c_jump_k_01, pl0c_jump_k_02, pl0c_jump_k_00, pl0c_jump_k_01, pl0c_jump_k_02,
	0,
};

const Sint32 *const pl0c_damage_tbl[] = {
	pl0c_damage_00, pl0c_damage_01, pl0c_damage_02, pl0c_damage_03, pl0c_damage_04, pl0c_damage_05,
	pl0c_damage_06, pl0c_damage_07, pl0c_damage_08, pl0c_damage_09, pl0c_damage_10, pl0c_damage_11,
	pl0c_damage_12, pl0c_damage_13, pl0c_damage_13, pl0c_damage_13, pl0c_damage_16, pl0c_damage_17,
	pl0c_damage_18, pl0c_damage_19, pl0c_damage_20, pl0c_damage_21, pl0c_damage_22, pl0c_damage_23,
	pl0c_damage_24, pl0c_damage_25, pl0c_damage_26, pl0c_damage_27, pl0c_damage_28, pl0c_damage_29,
	pl0c_damage_30, pl0c_damage_31, pl0c_damage_32, pl0c_damage_33, pl0c_damage_34, 0,
};

const Sint32 *const pl0c_dizzy_tbl[] = {
	pl0c_dizzy_00, 0,
};

const Sint32 *const pl0c_catch_tbl[] = {
	pl0c_catch_00, pl0c_catch_01, pl0c_catch_02, pl0c_catch_03, pl0c_catch_04, pl0c_catch_05,
	pl0c_catch_06, pl0c_catch_07, pl0c_catch_08, pl0c_catch_09, pl0c_catch_10, 0,
};

const Sint32 *const pl0c_caught_tbl[] = {
	pl0c_caught_00, pl0c_caught_01, pl0c_caught_02, pl0c_caught_03, pl0c_caught_04, pl0c_caught_05,
	pl0c_caught_06, pl0c_caught_07, pl0c_caught_08, pl0c_caught_09, pl0c_caught_10, pl0c_caught_11,
	pl0c_caught_12, pl0c_caught_13, pl0c_caught_14, pl0c_caught_15, pl0c_caught_16, pl0c_caught_17,
	pl0c_caught_18, pl0c_caught_19, pl0c_caught_20, pl0c_caught_20, 0,
};

const Sint32 *const pl0c_get_up_tbl[] = {
	pl0c_get_up_00, pl0c_get_up_01, pl0c_get_up_02, pl0c_get_up_03, 0,
};

const Sint32 *const pl0c_appear_tbl[] = {
	pl0c_appear_00, pl0c_appear_01, pl0c_appear_02, pl0c_appear_03, pl0c_appear_04, 0,
};

const Sint32 *const pl0c_win_tbl[] = {
	pl0c_win_00, pl0c_win_01, pl0c_win_02, pl0c_win_03, 0,
};

const Sint32 *const pl0c_extra_tbl[] = {
	pl0c_extra_00, pl0c_extra_01, pl0c_extra_02, 0,
};

const Sint32 *const pl0c_special_tbl[] = {
	pl0c_special_00, pl0c_special_01, pl0c_special_02, pl0c_special_03, pl0c_special_04, pl0c_special_05,
	pl0c_special_06, pl0c_special_07, pl0c_special_08, pl0c_special_09, pl0c_special_10, pl0c_special_11,
	pl0c_special_12, pl0c_special_13, pl0c_special_14, pl0c_special_15, pl0c_special_16, pl0c_special_17,
	pl0c_special_18, pl0c_special_19, pl0c_special_19, pl0c_special_21, pl0c_special_22, pl0c_special_22,
	pl0c_special_24, pl0c_special_25, pl0c_special_26, pl0c_special_26, pl0c_special_28, pl0c_special_29,
	pl0c_special_30, pl0c_special_31, 0,
};

const Sint32 *const pl0c_super_tbl[] = {
	pl0c_super_00, pl0c_super_01, pl0c_super_02, pl0c_super_03, pl0c_super_04, pl0c_super_05,
	pl0c_super_06, pl0c_super_07, pl0c_super_08, pl0c_super_09, pl0c_super_10, pl0c_super_11,
	pl0c_super_12, pl0c_super_13, pl0c_super_14, pl0c_super_15, pl0c_super_16, pl0c_super_17,
	pl0c_super_18, pl0c_super_19, pl0c_super_20, pl0c_super_21, pl0c_super_22, pl0c_super_23,
	pl0c_super_24, pl0c_super_25, pl0c_super_26, pl0c_super_27, pl0c_stand_00, pl0c_super_29,
	pl0c_stand_00, pl0c_super_31, pl0c_super_32, pl0c_stand_00, pl0c_super_34, pl0c_super_35,
	pl0c_super_36, pl0c_super_37, 0,
};

const Sint32 *const pl0c_effect_tbl[] = {
	pl0c_effect_00, pl0c_effect_01, pl0c_effect_02, pl0c_effect_03, pl0c_effect_04, pl0c_effect_05,
	pl0c_effect_06, pl0c_effect_07, pl0c_effect_08, pl0c_effect_09, pl0c_effect_10, pl0c_effect_11,
	pl0c_effect_12, pl0c_effect_13, pl0c_effect_14, pl0c_effect_15, pl0c_effect_16, pl0c_effect_17,
	pl0c_effect_18, pl0c_effect_19, pl0c_effect_20, pl0c_effect_21, pl0c_effect_22, pl0c_effect_23,
	pl0c_effect_24, pl0c_catch_10, pl0c_effect_26, pl0c_effect_27, pl0c_effect_28, pl0c_effect_29,
	pl0c_effect_30, pl0c_effect_31, pl0c_effect_32, pl0c_effect_33, pl0c_effect_34, pl0c_effect_35,
	pl0c_effect_36, pl0c_effect_37, pl0c_effect_37, pl0c_effect_39, pl0c_effect_40, pl0c_effect_41,
	pl0c_effect_42, pl0c_effect_43, pl0c_effect_44, pl0c_effect_45, pl0c_effect_46, pl0c_effect_47,
	pl0c_effect_48, pl0c_effect_49, pl0c_effect_50, pl0c_effect_51, pl0c_effect_52, pl0c_effect_53,
	pl0c_effect_54, pl0c_effect_55, pl0c_effect_56, pl0c_effect_57, pl0c_effect_58, pl0c_effect_59,
	pl0c_effect_60, pl0c_effect_61, pl0c_effect_62, pl0c_effect_63, pl0c_effect_64, pl0c_effect_65,
	pl0c_effect_66, pl0c_effect_67, pl0c_effect_68, pl0c_effect_69, pl0c_effect_70, pl0c_effect_71,
	pl0c_effect_72, pl0c_effect_73, pl0c_effect_74, pl0c_effect_75, pl0c_effect_76, pl0c_effect_77,
	pl0c_effect_78, pl0c_effect_79, pl0c_effect_80, pl0c_effect_81, pl0c_effect_82, pl0c_effect_83,
	pl0c_effect_84, 0,
};

const Sint32 *const pl0c_change_tbl[] = {
	pl0c_change_00, pl0c_change_01, pl0c_change_02, pl0c_change_03, pl0c_change_04, 0,
};

const Sint32 *const pl0c_grp25_tbl[] = {
	pl0c_stand_00, 0,
};

const Sint32 *const pl0c_fly_tbl[] = {
	pl0c_stand_00, 0,
};

const Sint32 *const pl0c_grp27_tbl[] = {
	pl0c_stand_00, 0,
};

	/*         prm  type   ctr          end    spr  prio  shadow  atck  flag1          se  zoom  unk  flag2   rival  box */

/* Char_Stand 0: drop_end, pose_end, shl0c_00_fly, act_stand, ...
   Char_Stand 1
   Char_Stand 4
   Char_Stand 5
   Char_Stand 8
   Char_Stand 9 */
const Sint32 pl0c_stand_00[] = {
	CG_FRAME(    0,    0,    3,           0,   159, 0x80,   0x52,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   160, 0x80,   0x52,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,   161, 0x80,   0x52,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,   162, 0x80,   0x52,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   163, 0x80,   0x52,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   164, 0x80,   0x52,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,   165, 0x80,   0x52,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,   166, 0x80,   0x52,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   167, 0x80,   0x52,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0c_stand_00 ),
};

/* Char_Stand 2: act_walk (2 + no) */
const Sint32 pl0c_stand_02[] = {
	CG_FRAME(    0,    0,    4,           0,   169, 0x80,   0x0f,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   170, 0x80,   0x0f,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   171, 0x80,   0x0f,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   172, 0x80,   0x0f,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   173, 0x80,   0x0f,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   174, 0x80,   0x0f,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   175, 0x80,   0x0f,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   176, 0x80,   0x0f,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   177, 0x80,   0x0f,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   178, 0x80,   0x0f,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   179, 0x80,   0x0f,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   180, 0x80,   0x0f,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0c_stand_02 ),
};

/* Char_Stand 3 */
const Sint32 pl0c_stand_03[] = {
	CG_FRAME(    0,    0,    4,           0,   179, 0x80,   0x0f,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   178, 0x80,   0x0f,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   177, 0x80,   0x0f,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   176, 0x80,   0x0f,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   175, 0x80,   0x0f,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   174, 0x80,   0x0f,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   173, 0x80,   0x0f,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   172, 0x80,   0x0f,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   171, 0x80,   0x0f,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   170, 0x80,   0x0f,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   169, 0x80,   0x0f,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   180, 0x80,   0x0f,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0c_stand_03 ),
};

/* Char_Stand 6 */
const Sint32 pl0c_stand_06[] = {
	CG_FRAME(    0,    0,    4,           0,   384, 0x80,   0x52,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   385, 0x80,   0x52,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   386, 0x80,   0x52,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   387, 0x80,   0x52,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   388, 0x80,   0x52,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   159, 0x80,   0x52,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0c_stand_06 ),
};

/* Char_Stand 7 */
const Sint32 pl0c_stand_07[] = {
	CG_FRAME(    0,    0,    4,           0,   388, 0x80,   0x52,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   387, 0x80,   0x52,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   386, 0x80,   0x52,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   385, 0x80,   0x52,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   384, 0x80,   0x52,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   159, 0x80,   0x52,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0c_stand_07 ),
};

/* Char_Jump 0 */
const Sint32 pl0c_jump_00[] = {
	CG_FRAME(    0,    1,    4,           0,   197, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    8,           0,   189, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4,           0,   190, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4,           0,   191, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4,           0,   192, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4,           0,   193, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4,           0,   192, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4,           0,   191, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4,           0,   194, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
};

const Sint32 pl0c_jump_00_loop[] = {
	CG_FRAME(    0,    0,    3,           0,   195, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   382, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl0c_jump_00_loop ),
};

/* Char_Jump 1: act_wall_jump, act_change_gattai_init, act_change_zero_init, act_change_assist_init, ... */
const Sint32 pl0c_jump_01[] = {
	CG_FRAME(    0,    1,    4,           0,   197, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   12,           0,   200, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2,           0,   201, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2,           0,   202, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2,           0,   203, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2,           0,   204, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2,           0,   205, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2,           0,   206, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2,           0,   207, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2,           0,   208, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2,           0,   209, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
};

const Sint32 pl0c_jump_01_loop[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   210, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl0c_jump_01_loop ),
};

/* Char_Jump 2 */
const Sint32 pl0c_jump_02[] = {
	CG_FRAME(    0,    1,    4,           0,   197, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2,           0,   211, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2,           0,   212, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2,           0,   213, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2,           0,   205, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2,           0,   204, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2,           0,   203, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2,           0,   383, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2,           0,   207, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2,           0,   214, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2,           0,   215, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
};

const Sint32 pl0c_jump_02_loop[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   216, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl0c_jump_02_loop ),
};

/* Char_Jump 3: sp_swing_end (Web Swing), maximum_end (Maximum Spider)
   Char_Jump 4
   Char_Jump 5 */
const Sint32 pl0c_jump_03[] = {
	CG_FRAME(    0,    0,    4,           0,   196, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   197, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   198, 0x80,   0x52,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   199, 0x80,   0x52,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0c_jump_03 ),
};

/* Char_Jump 6 */
const Sint32 pl0c_jump_06[] = {
	CG_FRAME(    0,    0,    4,           0,   189, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   381, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   189, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   381, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   189, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   381, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   190, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   191, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   192, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   193, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   192, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   191, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   194, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Jump 9: act_fall, act_air_guard_hold, act_wall_jump
   Char_Jump 10: change_in
   Char_Jump 11: act_change_out_recover */
const Sint32 pl0c_jump_09[] = {
	CG_FRAME(    0,    0,    3,           0,   195, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   382, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl0c_jump_09 ),
};

/* Char_Jump 7 */
const Sint32 pl0c_jump_07[] = {
	CG_FRAME(    0,    1,    4,           0,   197, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   12,           0,   200, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    5,           0,   201, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   202, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   203, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   204, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   205, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   206, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   207, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   383, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   203, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   204, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   205, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   206, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   207, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   191, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   194, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl0c_jump_07_loop[] = {
	CG_FRAME(    0,    0,    3,           0,   195, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   382, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl0c_jump_07_loop ),
};

/* Char_Jump 8 */
const Sint32 pl0c_jump_08[] = {
	CG_FRAME(    0,    1,    4,           0,   197, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   12,           0,   211, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    5,           0,   212, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   213, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   205, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   204, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   203, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   383, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   207, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   206, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   205, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   204, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   203, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   383, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   191, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   194, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl0c_jump_08_loop[] = {
	CG_FRAME(    0,    0,    3,           0,   195, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   382, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl0c_jump_08_loop ),
};

/* Char_Dash 0: act_dash */
const Sint32 pl0c_dash_00[] = {
	CG_FRAME(    0,    1,    4,           0,   181, 0x80,   0x0f,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    4,           0,   181, 0x80,   0x0f,    0,  0x00,       0x04,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    4,           0,   182, 0x80,   0x0f,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    4,           0,   183, 0x80,   0x0f,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
};

const Sint32 pl0c_dash_00_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   183, 0x80,   0x0f,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl0c_dash_00_loop ),
};

/* Char_Dash 1: act_back_dash */
const Sint32 pl0c_dash_01[] = {
	CG_FRAME(    0,    0,    4,           0,   185, 0x80,   0x0e,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   16,           0,   185, 0x80,   0x0e,    0,  0x00,       0x03,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    4,           0,   186, 0x80,   0x0e,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    4,           0,   187, 0x80,   0x0e,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   188, 0x80,   0x0e,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl0c_dash_01 ),
};

/* Char_Dash 2: dash_move */
const Sint32 pl0c_dash_02[] = {
	CG_FRAME(    0,    0,    4,           0,   182, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    1,    4,           0,   184, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    4,           0,   197, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   198, 0x80,   0x52,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   199, 0x80,   0x52,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0c_dash_02 ),
};

/* Char_Dash 3: noki_move */
const Sint32 pl0c_dash_03[] = {
	CG_FRAME(    0,    0,    4,           0,   197, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   198, 0x80,   0x52,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   199, 0x80,   0x52,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0c_dash_03 ),
};

/* Char_Crouch 0: act_crouch, act_stand_up */
const Sint32 pl0c_crouch_00[] = {
	CG_FRAME(    0,    2,    4,           0,   217, 0x80,   0x52,    0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    1,    4,           0,   218, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   219, 0x80,   0x4b,    0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl0c_crouch_01 ),
};

/* Char_Crouch 1: act_crouching, act_escape */
const Sint32 pl0c_crouch_01[] = {
	CG_FRAME(    0,    0,   12, END_OF_DATA,   168, 0x80,   0x4b,    0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl0c_crouch_01 ),
};

/* Char_Crouch 2: act_stand_up, act_crouch */
const Sint32 pl0c_crouch_02[] = {
	CG_FRAME(    0,    2,    4,           0,   220, 0x80,   0x4b,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    4,           0,   218, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   221, 0x80,   0x52,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0c_crouch_02 ),
};

/* Char_Turn 0: act_turn */
const Sint32 pl0c_turn_00[] = {
	CG_FRAME(    0,    0,    4,           0,   222, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   223, 0x80,   0x52,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   199, 0x80,   0x52,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0c_turn_00 ),
};

/* Char_Turn 1: act_crouch_turn */
const Sint32 pl0c_turn_01[] = {
	CG_FRAME(    0,    0,    4,           0,   224, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    4,           0,   225, 0x80,   0x4d,    0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   219, 0x80,   0x4b,    0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl0c_turn_01 ),
};

/* Char_Guard 0 */
const Sint32 pl0c_guard_00[] = {
	CG_FRAME(    0,    0,    2,           0,   226, 0x80,   0x53,    0,  0x00,          0,    0,   0,     0, 0x0100,   8 ),
};

/* Char_Guard 3 */
const Sint32 pl0c_guard_03[] = {
	CG_FRAME(    0,    1,    2,           0,   227, 0x80,   0x53,    0,  0x00,          0,    0,   0,     0, 0x0100,   8 ),
	CG_FRAME(    0,    2,    2,           0,   228, 0x80,   0x53,    0,  0x00,          0,    0,   0,     0, 0x0100,   8 ),
};

/* Char_Guard 1: act_guard_hold */
const Sint32 pl0c_guard_01[] = {
	CG_FRAME(    0,    3,    2, END_OF_DATA,   228, 0x80,   0x53,    0,  0x00,          0,    0,   0,     0, 0x0100,   8 ),
	CG_NEXT( pl0c_guard_01 ),
};

/* Char_Guard 2 */
const Sint32 pl0c_guard_02[] = {
	CG_FRAME(    0,    4,    2,           0,   229, 0x80,   0x53,    0,  0x00,          0,    0,   0,     0, 0x0100,   8 ),
	CG_FRAME(    0,    5,    2,           0,   230, 0x80,   0x53,    0,  0x00,          0,    0,   0,     0, 0x0100,   8 ),
	CG_FRAME(    0,    6,    2, END_OF_DATA,   199, 0x80,   0x53,    0,  0x01,          0,    0,   0,     0, 0x0100,   8 ),
	CG_NEXT( pl0c_guard_02 ),
};

/* Char_Guard 4 */
const Sint32 pl0c_guard_04[] = {
	CG_FRAME(    0,    0,    2,           0,   241, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0, 0x0200,   9 ),
};

/* Char_Guard 7 */
const Sint32 pl0c_guard_07[] = {
	CG_FRAME(    0,    1,    2,           0,   242, 0x80,   0x32,    0,  0x00,          0,    0,   0,     0, 0x0200,   9 ),
	CG_FRAME(    0,    2,    2,           0,   243, 0x80,   0x32,    0,  0x00,          0,    0,   0,     0, 0x0200,   9 ),
};

/* Char_Guard 5: act_guard_hold */
const Sint32 pl0c_guard_05[] = {
	CG_FRAME(    0,    3,    2, END_OF_DATA,   243, 0x80,   0x32,    0,  0x00,          0,    0,   0,     0, 0x0200,   9 ),
	CG_NEXT( pl0c_guard_05 ),
};

/* Char_Guard 6 */
const Sint32 pl0c_guard_06[] = {
	CG_FRAME(    0,    4,    2,           0,   244, 0x80,   0x32,    0,  0x00,          0,    0,   0,     0, 0x0200,   9 ),
	CG_FRAME(    0,    5,    2,           0,   245, 0x80,   0x32,    0,  0x00,          0,    0,   0,     0, 0x0200,   9 ),
	CG_FRAME(    0,    6,    2, END_OF_DATA,   219, 0x80,   0x32,    0,  0x01,          0,    0,   0,     0, 0x0200,   9 ),
	CG_NEXT( pl0c_guard_06 ),
};

/* Char_Air_Guard 0: act_air_guard */
const Sint32 pl0c_air_guard_00[] = {
	CG_FRAME(    0,    0,    2,           0,   256, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0, 0x0300,  10 ),
};

/* Char_Air_Guard 2: guard_char_set */
const Sint32 pl0c_air_guard_02[] = {
	CG_FRAME(    0,    1,    2,           0,   257, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0, 0x0300,  10 ),
	CG_FRAME(    0,    2,    2,           0,   258, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0, 0x0300,  10 ),
	CG_FRAME(    0,    3,    2,           0,   258, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0, 0x0300,  10 ),
};

/* Char_Air_Guard 1: act_air_guard_start */
const Sint32 pl0c_air_guard_01[] = {
	CG_FRAME(    0,    4,    2, END_OF_DATA,   258, 0x90,   0x20,    0,  0x01,          0,    0,   0,     0, 0x0300,  10 ),
	CG_NEXT( pl0c_air_guard_01 ),
};

/* Char_Stand_P 0 (light) */
const Sint32 pl0c_stand_p_00[] = {
	CG_FRAME(    0,    0,    2,           0,   411, 0x80,   0x51,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,    2,           0,   409, 0x80,   0x51,  ATK,  0x00,       0x03,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,    4,           0,   409, 0x80,   0x51,  ATK,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,    2,           0,   410, 0x80,   0x51,  ATK,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    1,    2,           0,   411, 0x80,   0x51,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   412, 0x80,   0x51,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_NEXT( pl0c_stand_p_00 ),
};

/* Char_Stand_P 1 (medium) */
const Sint32 pl0c_stand_p_01[] = {
	CG_FRAME(    0,    0,    4,           0,   413, 0x80,   0x51,  ATK,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,    1,           0,   414, 0x80,   0x0b,  ATK,  0x00,       0x04,    0,   0,     0,      0,  15 ),
	CG_FRAME(    0,    0,    2,           0,   415, 0x80,   0x26,  ATK,  0x00,          0,    0,   0,     0,      0,  16 ),
	CG_FRAME(    0,    0,    2,           0,   416, 0x80,   0x26,  ATK,  0x00,          0,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    0,    1,           0,   417, 0x80,   0x26,  ATK,  0x00,          0,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    0,   10,           0,   417, 0x80,   0x26,  ATK,  0x00,          0,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    0,    6,           0,   418, 0x80,   0x26,  ATE,  0x00,          0,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    0,    5,           0,   419, 0x80,   0x4a,  ATE,  0x00,          0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,    3,           0,   411, 0x80,   0x51,  ATE,  0x00,          0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   412, 0x80,   0x51,  ATK,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_NEXT( pl0c_stand_p_01 ),
};

/* Char_Stand_P 2 (heavy) */
const Sint32 pl0c_stand_p_02[] = {
	CG_FRAME(    0,    0,    6,           0,   420, 0x80,   0x0c,  ATK,  0x00,          0,    0,   0,     0,      0,  22 ),
	CG_FRAME(    0,    0,    1,           0,   421, 0x80,   0x09,  ATK,  0x00,       0x05,    0,   0,     0,      0,  22 ),
	CG_FRAME(    0,    0,    1,           0,   421, 0x80,   0x09,  ATK,  0x00,          0,    0,   0,     0,      0,  22 ),
	CG_FRAME(    0,    0,    2,           0,   423, 0x80,   0x03,  ATK,  0x00,          0,    0,   0,     0,      0,  23 ),
	CG_FRAME(    0,    0,    2,           0,   424, 0x80,   0x03,  ATK,  0x00,          0,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    0,    1,           0,   425, 0x80,   0x03,  ATE,  0x00,          0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,   13,           0,   426, 0x80,   0x03,  ATE,  0x00,          0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,    6,           0,   427, 0x80,   0x03,  ATE,  0x00,          0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,    6,           0,   428, 0x80,   0x0a,  ATE,  0x00,          0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,    7, END_OF_DATA,   221, 0x80,   0x52,  ATE,  0x00,          0,    0,   0,     0,      0,  26 ),
	CG_NEXT( pl0c_stand_p_02 ),
};

/* Char_Stand_P 3 */
const Sint32 pl0c_stand_p_03[] = {
	CG_FRAME(    0,    0,    2,           0,   408, 0x80,   0x51,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,    2,           0,   409, 0x80,   0x51,  ATK,  0x00,       0x03,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,    6,           0,   409, 0x80,   0x51,  ATK,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,    2,           0,   410, 0x80,   0x51,  ATK,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,    4,           0,   411, 0x80,   0x51,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,    5, END_OF_DATA,   412, 0x80,   0x51,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_NEXT( pl0c_stand_p_03 ),
};

/* Char_Stand_K 0 (light) */
const Sint32 pl0c_stand_k_00[] = {
	CG_FRAME(    0,    0,    3,           0,   429, 0x80,   0x0b,  ATK,  0x00,          0,    0,   0,     0,      0,  27 ),
	CG_FRAME(    0,    0,    3,           0,   430, 0x80,   0x0b,  ATK,  0x00,          0,    0,   0,     0,      0,  28 ),
	CG_FRAME(    0,    0,    5,           0,   430, 0x80,   0x0b,  ATK,  0x00,          0,    0,   0,     0,      0,  29 ),
	CG_FRAME(    0,    0,    3,           0,   431, 0x80,   0x0b,  ATK,  0x00,          0,    0,   0,     0,      0,  30 ),
	CG_FRAME(    0,    0,    4,           0,   429, 0x80,   0x0b,  ATK,  0x00,          0,    0,   0,     0,      0,  27 ),
	CG_FRAME(    0,    0,    5, END_OF_DATA,   412, 0x80,   0x52,  ATE,  0x00,          0,    0,   0,     0,      0,  27 ),
	CG_NEXT( pl0c_stand_k_00 ),
};

/* Char_Stand_K 1 (medium) */
const Sint32 pl0c_stand_k_01[] = {
	CG_FRAME(    0,    0,    2,           0,   432, 0x80,   0x60,  ATK,  0x00,          0,    0,   0,     0,      0,  31 ),
	CG_FRAME(    0,    0,    4,           0,   433, 0x80,   0x00,  ATK,  0x00,       0x03,    0,   0,     0,      0,  32 ),
	CG_FRAME(    0,    0,    6,           0,   435, 0x80,   0x09,  ATK,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,    6,           0,   437, 0x80,   0x09,  ATE,  0x00,          0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,    0,           0,   438, 0x80,   0x0a,  ATE,  0x00,          0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   221, 0x80,   0x52,  ATE,  0x00,          0,    0,   0,     0,      0,  37 ),
	CG_NEXT( pl0c_stand_k_01 ),
};

/* Char_Stand_K 2 (heavy) */
const Sint32 pl0c_stand_k_02[] = {
	CG_FRAME(    0,    0,    6,           0,   439, 0x80,   0x29,  ATK,  0x00,          0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,    1,           0,   440, 0x80,   0x06,  ATK,  0x00,       0x05,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,    2,           0,   441, 0x80,   0x44,  ATK,  0x00,          0,    0,   0,     0,      0,  39 ),
	CG_FRAME(    0,    0,    3,           0,   441, 0x80,   0x44,  ATK,  0x00,          0,    0,   0,     0,      0,  39 ),
	CG_FRAME(    0,    0,    4,           0,   442, 0x80,   0x44,  ATK,  0x00,          0,    0,   0,     0,      0,  39 ),
	CG_FRAME(    0,    0,    4,           0,   443, 0x80,   0x0d,  ATK,  0x00,          0,    0,   0,     0,      0,  40 ),
	CG_FRAME(    0,    0,    4,           0,   444, 0x80,   0x14,  ATK,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,    4,           0,   445, 0x80,   0x13,  ATE,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,    4,           0,   198, 0x80,   0x11,  ATE,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   199, 0x80,   0x52,  ATE,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_NEXT( pl0c_stand_k_02 ),
};

/* Char_Crouch_P 0 (light) */
const Sint32 pl0c_crouch_p_00[] = {
	CG_FRAME(    0,    0,    2,           0,   446, 0x80,   0x4b,  ATK,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,    3,           0,   447, 0x80,   0x4b,  ATK,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,    3,           0,   447, 0x80,   0x4b,  ATK,  0x00,          0,    0,   0,     0,      0,  44 ),
	CG_FRAME(    0,    0,    1,           0,   448, 0x80,   0x4b,  ATK,  0x00,          0,    0,   0,     0,      0,  44 ),
	CG_FRAME(    0,    0,    2,           0,   449, 0x80,   0x4b,  ATK,  0x00,          0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   449, 0x80,   0x4b,  ATE,  0x00,          0,    0,   0,     0,      0,  45 ),
	CG_NEXT( pl0c_crouch_p_00 ),
};

/* Char_Crouch_P 1 (medium) */
const Sint32 pl0c_crouch_p_01[] = {
	CG_FRAME(    0,    0,    4,           0,   450, 0x80,   0x4b,  ATK,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,    1,           0,   451, 0x80,   0x67,  ATK,  0x00,          0,    0,   0,     0,      0,  47 ),
	CG_FRAME( 0x80,    0,    2,           0,   452, 0x80,   0x67,  ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME( 0x80,    0,    1,           0,   453, 0x80,   0x67,  ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   12,           0,   454, 0x80,   0x67,  ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   456, 0x80,   0x4b,  ATE,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_NEXT( pl0c_crouch_p_01 ),
};

/* Char_Crouch_P 2 (heavy) */
const Sint32 pl0c_crouch_p_02[] = {
	CG_FRAME(    0,    0,    6,           0,   457, 0x80,   0x4c,  ATK,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,    2,           0,   458, 0x80,   0x81,  ATK,  0x00,       0x04,    0,   0,     0,      0,  52 ),
	CG_FRAME( 0x80,    0,    4,           0,   459, 0x80,   0x81,  ATK,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,   12,           0,   459, 0x80,   0x81,  ATE,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,    7,           0,   460, 0x80,   0x62,  ATE,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   461, 0x80,   0x45,  ATE,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_NEXT( pl0c_crouch_p_02 ),
};

/* Char_Crouch_P 3 */
const Sint32 pl0c_crouch_p_03[] = {
	CG_FRAME(    0,    0,    2,           0,   446, 0x80,   0x4b,  ATK,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,    3,           0,   447, 0x80,   0x4b,  ATK,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,    3,           0,   447, 0x80,   0x4b,  ATK,  0x00,          0,    0,   0,     0,      0,  44 ),
	CG_FRAME(    0,    0,    1,           0,   448, 0x80,   0x4b,  ATK,  0x00,          0,    0,   0,     0,      0,  44 ),
	CG_FRAME(    0,    0,    3,           0,   449, 0x80,   0x4b,  ATK,  0x00,          0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,    5, END_OF_DATA,   449, 0x80,   0x4b,  ATE,  0x00,          0,    0,   0,     0,      0,  45 ),
	CG_NEXT( pl0c_crouch_p_03 ),
};

/* Char_Crouch_K 0 (light) */
const Sint32 pl0c_crouch_k_00[] = {
	CG_FRAME(    0,    0,    4,           0,   462, 0x80,   0x4c,  ATK,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,    2,           0,   463, 0x80,   0x4c,  ATK,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,    4,           0,   463, 0x80,   0x4c,  ATK,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    2,           0,   464, 0x80,   0x4c,  ATK,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   462, 0x80,   0x4c,  ATK,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_NEXT( pl0c_crouch_k_00 ),
};

/* Char_Crouch_K 1 (medium) */
const Sint32 pl0c_crouch_k_01[] = {
	CG_FRAME(    0,    0,    4,           0,   468, 0x80,   0x4b,  ATK,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,    1,           0,   465, 0x80,   0x02,  ATK,  0x00,       0x04,    0,   0,     0,      0,  57 ),
	CG_FRAME( 0x80,    0,    2,           0,   466, 0x80,   0x02,  ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   12,           0,   466, 0x80,   0x02,  ATE,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,    7,           0,   468, 0x80,   0x02,  ATE,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   219, 0x80,   0x4b,  ATE,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_NEXT( pl0c_crouch_k_01 ),
};

/* Char_Crouch_K 2 (heavy) */
const Sint32 pl0c_crouch_k_02[] = {
	CG_FRAME(    0,    0,    6,           0,   469, 0x80,   0x01,  ATK,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,    3,           0,   470, 0x80,   0x01,  ATK,  0x00,       0x05,    0,   0,     0,      0,  46 ),
	CG_FRAME( 0x80,    0,    4,           0,   471, 0x80,   0x01,  ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,    7,           0,   471, 0x80,   0x01,  ATE,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,    8,           0,   472, 0x80,   0x01,  ATE,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,    7,           0,   473, 0x80,   0x01,  ATE,  0x00,          0,    0,   0,     0,      0,  62 ),
	CG_FRAME(    0,    0,    7,           0,   474, 0x80,   0x08,  ATE,  0x00,          0,    0,   0,     0,      0,  62 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   475, 0x80,   0x4b,  ATE,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_NEXT( pl0c_crouch_k_02 ),
};

/* Char_Jump_P 0 (light)
   Char_Jump_P 3 */
const Sint32 pl0c_jump_p_00[] = {
	CG_FRAME(    0,    0,    2,           0,   476, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,   10,           0,   477, 0x80,   0x20,  ATK,  0x00,       0x03,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,    3,           0,   478, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,    2,           0,   194, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  63 ),
};

const Sint32 pl0c_jump_p_00_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   195, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_NEXT( pl0c_jump_p_00_loop ),
};

/* Char_Jump_P 1 (medium)
   Char_Jump_P 4 */
const Sint32 pl0c_jump_p_01[] = {
	CG_FRAME(    0,    0,    4,           0,   479, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_FRAME( 0x80,    0,    2,           0,   480, 0x80,   0x20,  ATK,  0x00,       0x05,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,    4,           0,   481, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,    4,           0,   482, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,    4,           0,   483, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  68 ),
	CG_FRAME(    0,    0,    8,           0,   484, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  68 ),
	CG_FRAME(    0,    0,    4,           0,   478, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,    4,           0,   194, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  63 ),
};

const Sint32 pl0c_jump_p_01_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   195, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_NEXT( pl0c_jump_p_01_loop ),
};

/* Char_Jump_P 2 (heavy)
   Char_Jump_P 5 */
const Sint32 pl0c_jump_p_02[] = {
	CG_FRAME(    0,    0,    5,           0,   485, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,    2,           0,   486, 0x80,   0x20,  ATK,  0x00,       0x04,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,    2,           0,   487, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  70 ),
	CG_FRAME(    0,    0,    3,           0,   488, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   10,           0,   489, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,    6,           0,   191, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,    5,           0,   194, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  63 ),
};

const Sint32 pl0c_jump_p_02_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   195, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_NEXT( pl0c_jump_p_02_loop ),
};

/* Char_Jump_K 0 (light)
   Char_Jump_K 3 */
const Sint32 pl0c_jump_k_00[] = {
	CG_FRAME(    0,    0,    2,           0,   490, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,    8,           0,   491, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,    2,           0,   491, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    4,           0,   492, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    3,           0,   194, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  63 ),
};

const Sint32 pl0c_jump_k_00_loop[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   195, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_NEXT( pl0c_jump_k_00_loop ),
};

/* Char_Jump_K 1 (medium)
   Char_Jump_K 4 */
const Sint32 pl0c_jump_k_01[] = {
	CG_FRAME(    0,    0,    3,           0,   493, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,    1,           0,   494, 0x80,   0x20,  ATK,  0x00,       0x04,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,    2,           0,   495, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    4,           0,   496, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    4,           0,   497, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,    5,           0,   498, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,    4,           0,   191, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,    4,           0,   194, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  63 ),
};

const Sint32 pl0c_jump_k_01_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   195, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_NEXT( pl0c_jump_k_01_loop ),
};

/* Char_Jump_K 2 (heavy)
   Char_Jump_K 5 */
const Sint32 pl0c_jump_k_02[] = {
	CG_FRAME(    0,    0,    5,           0,   499, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,    2,           0,   500, 0x80,   0x20,  ATK,  0x00,       0x05,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,    6,           0,   501, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  78 ),
	CG_FRAME(    0,    0,    2,           0,   501, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  79 ),
	CG_FRAME(    0,    0,    5,           0,   212, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  79 ),
	CG_FRAME(    0,    0,    4,           0,   213, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,    3,           0,   205, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,    3,           0,   204, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,    3,           0,   203, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,    3,           0,   383, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,    4,           0,   191, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,    4,           0,   194, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  63 ),
};

const Sint32 pl0c_jump_k_02_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   195, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_NEXT( pl0c_jump_k_02_loop ),
};

/* Char_Damage 0: dm_freeze, dm_freeze_air, caught_dm */
const Sint32 pl0c_damage_00[] = {
	CG_FRAME(    0,    0,    9,           0,   726, 0x80,   0x53,    0,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   734, 0x80,   0x53,    0,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_NEXT( pl0c_damage_00 ),
};

/* Char_Damage 1 */
const Sint32 pl0c_damage_01[] = {
	CG_FRAME(    0,    0,    2,           0,   726, 0x80,   0x53,    0,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,   10,           0,   727, 0x80,   0x76,    0,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    4,           0,   733, 0x80,   0x53,    0,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    1,           0,   734, 0x80,   0x53,    0,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA,   221, 0x80,   0x53,    0,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_NEXT( pl0c_damage_01 ),
};

/* Char_Damage 2 */
const Sint32 pl0c_damage_02[] = {
	CG_FRAME(    0,    0,    2,           0,   727, 0x80,   0x76,    0,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,   10,           0,   728, 0x80,   0x76,    0,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    4,           0,   729, 0x80,   0x76,    0,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    2,           0,   730, 0x80,   0x76,    0,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    2,           0,   731, 0x80,   0x76,    0,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    1,           0,   732, 0x80,   0x54,    0,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    1,           0,   733, 0x80,   0x53,    0,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    1,           0,   734, 0x80,   0x53,    0,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA,   221, 0x80,   0x53,    0,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_NEXT( pl0c_damage_02 ),
};

/* Char_Damage 3 */
const Sint32 pl0c_damage_03[] = {
	CG_FRAME(    0,    0,    9,           0,   735, 0x80,   0x55,    0,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   736, 0x80,   0x54,    0,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_NEXT( pl0c_damage_03 ),
};

/* Char_Damage 4 */
const Sint32 pl0c_damage_04[] = {
	CG_FRAME(    0,    0,    8,           0,   737, 0x80,   0x14,    0,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    3,           0,   738, 0x80,   0x14,    0,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    3,           0,   739, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    2,           0,   198, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   199, 0x80,   0x53,    0,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_NEXT( pl0c_damage_04 ),
};

/* Char_Damage 5 */
const Sint32 pl0c_damage_05[] = {
	CG_FRAME(    0,    0,   10,           0,   740, 0x80,   0x13,    0,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    4,           0,   741, 0x80,   0x13,    0,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    2,           0,   742, 0x80,   0x13,    0,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    2,           0,   197, 0x80,   0x0e,    0,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    2,           0,   743, 0x80,   0x0e,    0,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    2,           0,   744, 0x80,   0x0e,    0,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    1,           0,   198, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA,   199, 0x80,   0x52,    0,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_NEXT( pl0c_damage_05 ),
};

/* Char_Damage 6 */
const Sint32 pl0c_damage_06[] = {
	CG_FRAME(    0,    0,    9,           0,   745, 0x80,   0x12,    0,  0x00,          0,    0,   0,     0,      0,  81 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   752, 0x80,   0x0d,    0,  0x00,          0,    0,   0,     0,      0,  81 ),
	CG_NEXT( pl0c_damage_06 ),
};

/* Char_Damage 7 */
const Sint32 pl0c_damage_07[] = {
	CG_FRAME(    0,    0,    2,           0,   745, 0x80,   0x12,    0,  0x00,          0,    0,   0,     0,      0,  81 ),
	CG_FRAME(    0,    0,   10,           0,   746, 0x80,   0x18,    0,  0x00,          0,    0,   0,     0,      0,  81 ),
	CG_FRAME(    0,    0,    2,           0,   751, 0x80,   0x12,    0,  0x00,          0,    0,   0,     0,      0,  81 ),
	CG_FRAME(    0,    0,    2,           0,   752, 0x80,   0x0e,    0,  0x00,          0,    0,   0,     0,      0,  81 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   219, 0x80,   0x0e,    0,  0x00,          0,    0,   0,     0,      0,  81 ),
	CG_NEXT( pl0c_damage_07 ),
};

/* Char_Damage 8 */
const Sint32 pl0c_damage_08[] = {
	CG_FRAME(    0,    0,    2,           0,   746, 0x80,   0x18,    0,  0x00,          0,    0,   0,     0,      0,  81 ),
	CG_FRAME(    0,    0,   10,           0,   747, 0x80,   0x17,    0,  0x00,          0,    0,   0,     0,      0,  81 ),
	CG_FRAME(    0,    0,    3,           0,   748, 0x80,   0x17,    0,  0x00,          0,    0,   0,     0,      0,  81 ),
	CG_FRAME(    0,    0,    3,           0,   749, 0x80,   0x17,    0,  0x00,          0,    0,   0,     0,      0,  81 ),
	CG_FRAME(    0,    0,    2,           0,   750, 0x80,   0x17,    0,  0x00,          0,    0,   0,     0,      0,  81 ),
	CG_FRAME(    0,    0,    2,           0,   751, 0x80,   0x12,    0,  0x00,          0,    0,   0,     0,      0,  81 ),
	CG_FRAME(    0,    0,    1,           0,   752, 0x80,   0x0e,    0,  0x00,          0,    0,   0,     0,      0,  81 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA,   219, 0x80,   0x0e,    0,  0x00,          0,    0,   0,     0,      0,  81 ),
	CG_NEXT( pl0c_damage_08 ),
};

/* Char_Damage 9: dm_sweep */
const Sint32 pl0c_damage_09[] = {
	CG_FRAME(    0,    0,    5,           0,   757, 0x80,   0x2d,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   758, 0x80,   0x2d,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   759, 0x80,   0x2b,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    5,           0,   760, 0x80,   0x26,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   761, 0x80,   0x2b,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   770, 0x80,   0x0c,    0,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    0,    0,    2,           0,   771, 0x80,   0x0d,    0,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    0,    0,    2,           0,   772, 0x80,   0x0d,    0,  0x00,          0,    0,   0,     0,      0,  83 ),
};

const Sint32 pl0c_damage_09_loop[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   773, 0x80,   0x0d,    0,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_NEXT( pl0c_damage_09_loop ),
};

/* Char_Damage 10: dm_ko_blow */
const Sint32 pl0c_damage_10[] = {
	CG_FRAME(    0,    0,   10, END_OF_DATA,   753, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_NEXT( pl0c_damage_10 ),
};

/* Char_Damage 11 */
const Sint32 pl0c_damage_11[] = {
	CG_FRAME(    0,    0,   10, END_OF_DATA,   754, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_NEXT( pl0c_damage_11 ),
};

/* Char_Damage 12 */
const Sint32 pl0c_damage_12[] = {
	CG_FRAME(    0,    0,   10, END_OF_DATA,   755, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_NEXT( pl0c_damage_12 ),
};

/* Char_Damage 13: dm_snap_fly, dm_air
   Char_Damage 14
   Char_Damage 15 */
const Sint32 pl0c_damage_13[] = {
	CG_FRAME( 0x80,    0,    3,           0,   756, 0x80,   0x2d,    0,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    0,    0,    3,           0,   772, 0x80,   0x0d,    0,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   773, 0x80,   0x0d,    0,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_NEXT( pl0c_damage_13 ),
};

/* Char_Damage 16: dm_blow_char_set (16 + no), dm_spark, caught_shock_init */
const Sint32 pl0c_damage_16[] = {
	CG_FRAME(    0,    0,    2,           0,   762, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   763, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_NEXT( pl0c_damage_16 ),
};

/* Char_Damage 17: dm_07_1 */
const Sint32 pl0c_damage_17[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   764, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_NEXT( pl0c_damage_17 ),
};

/* Char_Damage 18 */
const Sint32 pl0c_damage_18[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   765, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_NEXT( pl0c_damage_18 ),
};

/* Char_Damage 19 */
const Sint32 pl0c_damage_19[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   766, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_NEXT( pl0c_damage_19 ),
};

/* Char_Damage 20 */
const Sint32 pl0c_damage_20[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   767, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_NEXT( pl0c_damage_20 ),
};

/* Char_Damage 21: dm_fall_char_set (21 + no) */
const Sint32 pl0c_damage_21[] = {
	CG_FRAME(    0,    0,    4,           0,   762, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  82 ),
};

/* Char_Damage 22 */
const Sint32 pl0c_damage_22[] = {
	CG_FRAME(    0,    0,    4,           0,   764, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  82 ),
};

/* Char_Damage 23 */
const Sint32 pl0c_damage_23[] = {
	CG_FRAME(    0,    0,    4,           0,   765, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  82 ),
};

/* Char_Damage 24 */
const Sint32 pl0c_damage_24[] = {
	CG_FRAME(    0,    0,    4,           0,   766, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  82 ),
};

/* Char_Damage 25 */
const Sint32 pl0c_damage_25[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   767, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_NEXT( pl0c_damage_25 ),
};

/* Char_Damage 26: dm_buttobi_fly, dm_07_1, dm_away_fly, dm_bound_hop, ... */
const Sint32 pl0c_damage_26[] = {
	CG_FRAME(    0,    0,    1,           0,   768, 0x80,   0x2c,    0,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    0,    0,    1,           0,   769, 0x80,   0x2c,    0,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    0,    0,    2,           0,   770, 0x80,   0x0d,    0,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    0,    0,    2,           0,   771, 0x80,   0x0d,    0,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    0,    0,    2,           0,   772, 0x80,   0x0d,    0,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    0,    0,    2,           0,   773, 0x80,   0x0d,    0,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   774, 0x80,   0x0d,    0,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_NEXT( pl0c_damage_26 ),
};

/* Char_Damage 27 */
const Sint32 pl0c_damage_27[] = {
	CG_FRAME(    0,    0,    2,           0,   768, 0x80,   0x2b,    0,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    0,    0,    2,           0,   333, 0x80,   0x0e,    0,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    0,    0,    4,           0,   205, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    0,    0,    4,           0,   204, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    0,    0,    4,           0,   203, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    0,    0,    4,           0,   214, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    0,    0,    4,           0,   215, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  84 ),
};

const Sint32 pl0c_damage_27_loop[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   216, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_NEXT( pl0c_damage_27_loop ),
};

/* Char_Damage 28: dm_away_init, dm_bound_fly, dm_piyo, dm_away */
const Sint32 pl0c_damage_28[] = {
	CG_FRAME(    0,    0,    2,           0,   775, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,    2,           0,   776, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,    2,           0,   777, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,    2,           0,   778, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,    2,           0,   779, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   780, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_NEXT( pl0c_damage_28 ),
};

/* Char_Damage 29: dm_away_fly, dm_bound_hop, dm_bound, caught_spin_fly, ... */
const Sint32 pl0c_damage_29[] = {
	CG_FRAME(    0,    0,    2,           0,   781, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,    2,           0,   782, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,    2,           0,   783, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,    2,           0,   784, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,    2,           0,   785, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   786, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_NEXT( pl0c_damage_29 ),
};

/* Char_Damage 30: act_air_recover, act_change_out_init, caught_blow_fly, caught_blow_fix_fly, ... */
const Sint32 pl0c_damage_30[] = {
	CG_FRAME(    0,    0,    4,           0,   205, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    0,    0,    4,           0,   204, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    0,    0,    4,           0,   203, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    0,    0,    4,           0,   214, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    0,    0,    4,           0,   215, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  84 ),
};

const Sint32 pl0c_damage_30_loop[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   216, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_NEXT( pl0c_damage_30_loop ),
};

/* Char_Damage 31: act_down, dm_dizzy_down */
const Sint32 pl0c_damage_31[] = {
	CG_FRAME(    0,    0,   16, END_OF_DATA,   774, 0x80,   0x6e,    0,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_NEXT( pl0c_damage_31 ),
};

/* Char_Damage 32: act_escape */
const Sint32 pl0c_damage_32[] = {
	CG_FRAME(    0,    0,    4,           0,   205, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,    4,           0,   204, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,    4,           0,   203, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,    4,           0,   214, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   215, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_NEXT( pl0c_damage_32 ),
};

/* Char_Damage 33: act_escape */
const Sint32 pl0c_damage_33[] = {
	CG_FRAME(    0,    0,    2,           0,   333, 0x80,   0x2b,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   343, 0x80,   0x0d,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   341, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   340, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   339, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   338, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   337, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   342, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   197, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   198, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   199, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0c_damage_33 ),
};

/* Char_Damage 34: dm_snap_fly, dm_snap_down */
const Sint32 pl0c_damage_34[] = {
	CG_FRAME(    0,    0,    4,           0,   765, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,    4,           0,   766, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,    4,           0,   767, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,    2,           0,   768, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    0,    0,    2,           0,   769, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    0,    0,    2,           0,   770, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    0,    0,    2,           0,   771, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    0,    0,    2,           0,   772, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    0,    0,    2,           0,   773, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   774, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_NEXT( pl0c_damage_34 ),
};

/* Char_Dizzy 0: act_dizzy */
const Sint32 pl0c_dizzy_00[] = {
	CG_FRAME(    0,    0,    3,           0,   787, 0x80,   0x52,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   788, 0x80,   0x52,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   789, 0x80,   0x52,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   790, 0x80,   0x52,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   791, 0x80,   0x52,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   792, 0x80,   0x52,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0c_dizzy_00 ),
};

/* Char_Catch 0 */
const Sint32 pl0c_catch_00[] = {
	CG_FRAME(    0,    0,    1,           0,   502, 0x80,   0x53,    0,  0x00,          0,    0,   0,     0, 0x0001,  80 ),
	CG_FRAME(    0,    0,    9,           0,   502, 0x80,   0x53,    0,  0x00,          0,    0,   0,     0, 0x0001,  80 ),
	CG_FRAME(    0,    0,    7,           0,   503, 0x80,   0x53,    0,  0x00,          0,    0,   0,     0, 0x0002,  80 ),
	CG_FRAME(    0,    0,    6,           0,   504, 0x80,   0x13,    0,  0x00,          0,    0,   0,     0, 0x0003,  80 ),
	CG_FRAME(    0,    0,    4,           0,   505, 0x80,   0x14,    0,  0x00,          0,    0,   0,     0, 0x0004,  80 ),
	CG_FRAME(    0,    0,    3,           0,   506, 0x80,   0x15,    0,  0x00,          0,    0,   0,     0, 0x0005,  80 ),
	CG_FRAME(    0,    1,    8,           0,   507, 0x80,   0x1e,    0,  0x00,       0x04,    0,   0,     0, 0x0006,  80 ),
	CG_FRAME(    0,    0,    8,           0,   508, 0x80,   0x1e,    0,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    6,           0,   509, 0x80,   0x1e,    0,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    6,           0,   510, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   511, 0x80,   0x4d,    0,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_NEXT( pl0c_catch_00 ),
};

/* Char_Catch 1 */
const Sint32 pl0c_catch_01[] = {
	CG_FRAME(    0,    0,    1,           0,   512, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0, 0x0007,  80 ),
	CG_FRAME(    0,    0,    8,           0,   512, 0x80,   0x11,    0,  0x00,          0,    0,   0,     0, 0x0007,  80 ),
	CG_FRAME(    0,    1,    4,           0,   513, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0, 0x0008,  80 ),
	CG_FRAME(    0,    0,    4,           0,   514, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0, 0x0009,  80 ),
	CG_FRAME(    0,    0,    4,           0,   515, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0, 0x000a,  80 ),
	CG_FRAME(    0,    0,    4,           0,   516, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0, 0x000b,  80 ),
	CG_FRAME(    0,    0,    4,           0,   517, 0x80,   0x20,    0,  0x00, 0x16 | CSE,    0,   0,     0, 0x000c,  80 ),
	CG_FRAME(    0,    0,    4,           0,   518, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0, 0x000d,  80 ),
	CG_FRAME(    0,    0,    4,           0,   519, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0, 0x000e,  80 ),
	CG_FRAME(    0,    0,   10,           0,   520, 0x80,   0x20,    0,  0x01,       0x05,    0,   0,     0, 0x000f,  80 ),
	CG_FRAME(    0,    0,    6,           0,   192, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    6,           0,   191, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    6,           0,   194, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  80 ),
};

const Sint32 pl0c_catch_01_loop[] = {
	CG_FRAME(    0,    0,    6,           0,   195, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   382, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_NEXT( pl0c_catch_01_loop ),
};

/* Char_Catch 2 */
const Sint32 pl0c_catch_02[] = {
	CG_FRAME(    0,    0,    1,           0,   604, 0x80,   0x20,    0,  0x00,       0x04,    0,   0,     0, 0x0025,  80 ),
	CG_FRAME(    0,    0,    8,           0,   604, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0, 0x0025,  80 ),
	CG_FRAME(    0,    0,    4,           0,   605, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0, 0x0027,  80 ),
	CG_FRAME(    0,    0,    4,           0,   606, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0, 0x0028,  80 ),
	CG_FRAME(    0,    0,    4,           0,   607, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0, 0x0029,  80 ),
	CG_FRAME(    0,    0,    4,           0,   608, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0, 0x002a,  80 ),
	CG_FRAME(    0,    0,    4,           0,   609, 0x80,   0x20,    0,  0x00, 0x16 | CSE,    0,   0,     0, 0x002b,  80 ),
	CG_FRAME(    0,    0,    4,           0,   610, 0x80,   0x20,    0,  0x01,       0x05,    0,   0,     0, 0x002c,  80 ),
	CG_FRAME(    0,    0,   10,           0,   611, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0, 0x002e,  80 ),
	CG_FRAME(    0,    0,    6,           0,   192, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    6,           0,   191, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    6,           0,   194, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  80 ),
};

const Sint32 pl0c_catch_02_loop[] = {
	CG_FRAME(    0,    0,    6, END_OF_DATA,   195, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_NEXT( pl0c_catch_02_loop ),
};

/* Char_Caught 0 */
const Sint32 pl0c_caught_00[] = {
	CG_FRAME( 0x80,    0,    2, END_OF_DATA,   726, 0x80,   0x53,    0,  0x00,          0,    0,   0,     0,      0,  85 ),
	CG_NEXT( pl0c_caught_00 ),
};

/* Char_Caught 1 */
const Sint32 pl0c_caught_01[] = {
	CG_FRAME( 0x80,    0,    2, END_OF_DATA,   727, 0x80,   0x76,    0,  0x00,          0,    0,   0,     0,      0,  86 ),
	CG_NEXT( pl0c_caught_00 ),
};

/* Char_Caught 2 */
const Sint32 pl0c_caught_02[] = {
	CG_FRAME( 0x80,    0,    2, END_OF_DATA,   728, 0x80,   0x76,    0,  0x00,          0,    0,   0,     0,      0,  87 ),
	CG_NEXT( pl0c_caught_02 ),
};

/* Char_Caught 3 */
const Sint32 pl0c_caught_03[] = {
	CG_FRAME( 0x80,    0,    2, END_OF_DATA,   735, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  88 ),
	CG_NEXT( pl0c_caught_03 ),
};

/* Char_Caught 4 */
const Sint32 pl0c_caught_04[] = {
	CG_FRAME( 0x80,    0,    2, END_OF_DATA,   737, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  85 ),
	CG_NEXT( pl0c_caught_04 ),
};

/* Char_Caught 5 */
const Sint32 pl0c_caught_05[] = {
	CG_FRAME( 0x80,    0,    2, END_OF_DATA,   740, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  85 ),
	CG_NEXT( pl0c_caught_05 ),
};

/* Char_Caught 6 */
const Sint32 pl0c_caught_06[] = {
	CG_FRAME( 0x80,    0,    2, END_OF_DATA,   745, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  90 ),
	CG_NEXT( pl0c_caught_06 ),
};

/* Char_Caught 7 */
const Sint32 pl0c_caught_07[] = {
	CG_FRAME( 0x80,    0,    2, END_OF_DATA,   746, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  90 ),
	CG_NEXT( pl0c_caught_07 ),
};

/* Char_Caught 8 */
const Sint32 pl0c_caught_08[] = {
	CG_FRAME( 0x80,    0,    2, END_OF_DATA,   747, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  91 ),
	CG_NEXT( pl0c_caught_08 ),
};

/* Char_Caught 9 */
const Sint32 pl0c_caught_09[] = {
	CG_FRAME( 0x80,    0,    2, END_OF_DATA,   753, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  92 ),
	CG_NEXT( pl0c_caught_09 ),
};

/* Char_Caught 10 */
const Sint32 pl0c_caught_10[] = {
	CG_FRAME( 0x80,    0,    2, END_OF_DATA,   754, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_NEXT( pl0c_caught_10 ),
};

/* Char_Caught 11 */
const Sint32 pl0c_caught_11[] = {
	CG_FRAME( 0x80,    0,    2, END_OF_DATA,   755, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  94 ),
	CG_NEXT( pl0c_caught_11 ),
};

/* Char_Caught 12 */
const Sint32 pl0c_caught_12[] = {
	CG_FRAME( 0x80,    0,    2, END_OF_DATA,   756, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  95 ),
	CG_NEXT( pl0c_caught_12 ),
};

/* Char_Caught 13 */
const Sint32 pl0c_caught_13[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   762, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_NEXT( pl0c_caught_13 ),
};

/* Char_Caught 14 */
const Sint32 pl0c_caught_14[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   764, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_NEXT( pl0c_caught_14 ),
};

/* Char_Caught 15 */
const Sint32 pl0c_caught_15[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   765, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_NEXT( pl0c_caught_15 ),
};

/* Char_Caught 16 */
const Sint32 pl0c_caught_16[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   766, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_NEXT( pl0c_caught_16 ),
};

/* Char_Caught 17 */
const Sint32 pl0c_caught_17[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   767, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_NEXT( pl0c_caught_17 ),
};

/* Char_Caught 18 */
const Sint32 pl0c_caught_18[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   775, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_NEXT( pl0c_caught_18 ),
};

/* Char_Caught 19 */
const Sint32 pl0c_caught_19[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   781, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_NEXT( pl0c_caught_19 ),
};

/* Char_Caught 20
   Char_Caught 21 */
const Sint32 pl0c_caught_20[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   774, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  95 ),
	CG_NEXT( pl0c_caught_20 ),
};

/* Char_Get_Up 0: dm_dizzy_wait */
const Sint32 pl0c_get_up_00[] = {
	CG_FRAME(    0,    0,    4,           0,   332, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   333, 0x80,   0x0d,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   334, 0x80,   0x0c,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    5,           0,   335, 0x80,   0x0e,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    5,           0,   197, 0x80,   0x0e,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   198, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   199, 0x80,   0x52,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0c_get_up_00 ),
};

/* Char_Get_Up 1 */
const Sint32 pl0c_get_up_01[] = {
	CG_FRAME(    0,    0,    1,           0,   332, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   333, 0x80,   0x0d,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   336, 0x80,   0x0c,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   337, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   338, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   339, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   340, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   341, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   197, 0x80,   0x0d,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   198, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   199, 0x80,   0x52,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0c_get_up_01 ),
};

const Sint32 pl0c_unused_00[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,    1,           0,   332, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   333, 0x80,   0x0d,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   336, 0x80,   0x0c,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   337, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   338, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   339, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   340, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   341, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   342, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   337, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   338, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   339, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   340, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   341, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   197, 0x80,   0x0d,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   198, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   199, 0x80,   0x52,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	0x00003354,
};

/* Char_Get_Up 2 */
const Sint32 pl0c_get_up_02[] = {
	CG_FRAME(    0,    0,    2,           0,   332, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   333, 0x80,   0x0d,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   336, 0x80,   0x0c,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   337, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   338, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   339, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   340, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   341, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   342, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   337, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   338, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   339, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   340, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   341, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   197, 0x80,   0x0d,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   198, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   199, 0x80,   0x52,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0c_get_up_02 ),
};

/* Char_Get_Up 3 */
const Sint32 pl0c_get_up_03[] = {
	CG_FRAME(    0,    0,    1,           0,   332, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   333, 0x80,   0x0d,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   336, 0x80,   0x0c,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   337, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   342, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   341, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   340, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   339, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   338, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   337, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   342, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   341, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   340, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   339, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   197, 0x80,   0x0d,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   198, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   199, 0x80,   0x52,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0c_get_up_03 ),
};

/* Char_Appear 0: drop_wait */
const Sint32 pl0c_appear_00[] = {
	CG_FRAME(    0,    0,   20,           0,   316, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   317, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   318, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   319, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   320, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0c_appear_00 ),
};

/* Char_Appear 1: drop_flash */
const Sint32 pl0c_appear_01[] = {
	CG_FRAME(    0,    0,   10,           0,   321, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl0c_appear_01_loop[] = {
	CG_FRAME(    0,    1,    4, END_OF_DATA,   321, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0c_appear_01_loop ),
};

/* Char_Appear 2: drop_fall */
const Sint32 pl0c_appear_02[] = {
	CG_FRAME(    0,    0,   16,           0,   321, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    2,    3,           0,   330, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    3,           0,   331, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    3,           0,   207, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    3,           0,   206, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    3,           0,   205, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    3,           0,   204, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    3,           0,   203, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    3,           0,   191, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    3,           0,   194, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl0c_appear_02_loop[] = {
	CG_FRAME(    0,    1,    3,           0,   195, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    3, END_OF_DATA,   382, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl0c_appear_02_loop ),
};

/* Char_Appear 3: drop_land */
const Sint32 pl0c_appear_03[] = {
	CG_FRAME(    0,    0,    6,           0,   196, 0x80,   0x0d,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   197, 0x80,   0x0d,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    8,           0,   198, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   199, 0x80,   0x52,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0c_appear_03 ),
};

/* Char_Appear 4: pose_init */
const Sint32 pl0c_appear_04[] = {
	CG_FRAME(    0,    0,   24,           0,   276, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   277, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   278, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   279, 0x80,   0x00,    0,  0x00,       0x11,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   278, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   279, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   278, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   279, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   280, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   278, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   279, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   278, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   279, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   278, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   279, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   280, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   271, 0x80,   0x13,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   281, 0x80,   0x51,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   282, 0x80,   0x51,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   283, 0x80,   0x51,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   284, 0x80,   0x51,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   283, 0x80,   0x51,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   284, 0x80,   0x51,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   283, 0x80,   0x51,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   284, 0x80,   0x51,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   285, 0x80,   0x51,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   283, 0x80,   0x51,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   284, 0x80,   0x51,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   283, 0x80,   0x51,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   284, 0x80,   0x51,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   283, 0x80,   0x51,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   284, 0x80,   0x51,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   285, 0x80,   0x51,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   230, 0x80,   0x52,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl0c_appear_04_loop[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   199, 0x80,   0x52,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0c_appear_04_loop ),
};

/* Char_Win 0: win_init */
const Sint32 pl0c_win_00[] = {
	CG_FRAME(    0,    0,    4,           0,   412, 0x80,   0x52,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   270, 0x80,   0x52,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,   271, 0x80,   0x13,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   272, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    7,           0,   273, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    7,           0,   274, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   275, 0x80,   0x00,    0,  0x00,       0x10,    0,   0,     0,      0,   1 ),
};

const Sint32 pl0c_win_00_loop[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   276, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0c_win_00_loop ),
};

/* Char_Win 1: win_init */
const Sint32 pl0c_win_01[] = {
	CG_FRAME(    0,    0,    2,           0,   286, 0x80,   0x54,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    2,           0,   287, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    2,           0,   288, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   289, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   290, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   291, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   292, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    2,    2,           0,   289, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   288, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    2,           0,   287, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    2,    2,           0,   293, 0x80,   0x2d,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    7,           0,   294, 0x80,   0x2d,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    7,           0,   295, 0x80,   0x2d,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   296, 0x80,   0x2d,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,   297, 0x80,   0x00,    0,  0x00,       0x0f,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   298, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   299, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   300, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   301, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   302, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   303, 0x80,   0x00,    0,  0x01,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl0c_win_01_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   302, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0c_win_01_loop ),
};

/* Char_Win 2: win_init */
const Sint32 pl0c_win_02[] = {
	CG_FRAME(    0,    0,    4,           0,   412, 0x80,   0x54,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   306, 0x80,   0x54,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   307, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   308, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl0c_win_02_loop[] = {
	CG_FRAME(    0,    0,    4,           0,   309, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   310, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   311, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   309, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   310, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   311, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   309, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   310, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   311, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   309, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   310, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   311, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   309, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   310, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   311, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0c_win_02_loop ),
};

const Sint32 pl0c_unused_01[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,    4,           0,   310, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   308, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   307, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   306, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   412, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	0x00003fd8,
};

/* Char_Win 3: win_init */
const Sint32 pl0c_win_03[] = {
	CG_FRAME(    0,    0,    5,           0,   412, 0x80,   0x54,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,   306, 0x80,   0x54,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,   312, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   24,           0,   313, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,   314, 0x80,   0x00, 0x12,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   24,           0,   315, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   314, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl0c_win_03_loop[] = {
	CG_FRAME(    0,    1,    6, END_OF_DATA,   313, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0c_win_03_loop ),
};

/* Char_Jump 12: act_wall_jump */
const Sint32 pl0c_jump_12[] = {
	CG_FRAME(    0,    0,    4,           0,   521, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 180 ),
	CG_FRAME(    0,    0,    4,           0,   522, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 180 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   523, 0x80,   0x20,    0,  0x01,          0,    0,   0,     0,      0, 180 ),
	CG_NEXT( pl0c_jump_12 ),
};

/* Char_Extra 0: swing_init, swing_move */
const Sint32 pl0c_extra_00[] = {
	CG_FRAME(    1,   33,    8,           0,   524, 0x80,   0x20,    0,  0x01,          0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,   33,   16,           0,   524, 0x80,   0x20,    0,  0x01,       0x04,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,   33,   -1,           0,   524, 0x80,   0x20, 0x1d,  0x01,          0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,   32,   32, END_OF_DATA,   525, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl0c_extra_00 ),
};

/* Char_Extra 1 */
const Sint32 pl0c_extra_01[] = {
	CG_FRAME(    0,    0,    4,           0,   413, 0x80,   0x51,  ATK,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,    1,           0,   414, 0x80,   0x0b,  ATK,  0x00,       0x04,    0,   0,     0,      0,  15 ),
	CG_FRAME(    0,    0,    2,           0,   415, 0x80,   0x26,  ATK,  0x00,          0,    0,   0,     0,      0,  16 ),
	CG_FRAME(    0,    0,    2,           0,   416, 0x80,   0x26,  ATK,  0x00,          0,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    0,    1,           0,   417, 0x80,   0x26,  ATK,  0x00,          0,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    0,   10,           0,   417, 0x80,   0x26,  ATK,  0x00,          0,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    0,    6,           0,   418, 0x80,   0x26,  ATE,  0x00,          0,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    0,    5,           0,   419, 0x80,   0x4a,  ATE,  0x00,          0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,    3,           0,   411, 0x80,   0x51,  ATE,  0x00,          0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   412, 0x80,   0x51,  ATK,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_NEXT( pl0c_extra_01 ),
};

/* Char_Extra 2: fly_start5 */
const Sint32 pl0c_extra_02[] = {
	CG_FRAME(    0,    0,    4,           0,   468, 0x80,   0x4b,  ATK,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,    1,           0,   465, 0x80,   0x02,  ATK,  0x00,       0x04,    0,   0,     0,      0,  57 ),
	CG_FRAME( 0x80,    0,    2,           0,   466, 0x80,   0x02,  ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   12,           0,   466, 0x80,   0x02,  ATK,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,    7,           0,   468, 0x80,   0x02,  ATK,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   219, 0x80,   0x4b,  ATE,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_NEXT( pl0c_extra_02 ),
};

/* Char_Special 0 */
const Sint32 pl0c_special_00[] = {
	CG_FRAME(    0,    0,    3,           0,   547, 0x80,   0x53,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   548, 0x80,   0x52,  ATK,  0x00,       0x15,    0,   0,     0,      0,  96 ),
	CG_FRAME(    0,    0,    2,           0,   549, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    1,    1,           0,   549, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,    3,           0,   550, 0x80,   0x20,  ATK,  0x00, 0x16 | CSE,    0,   0,     0,      0,  98 ),
	CG_FRAME(    0,    1,    3,           0,   551, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_FRAME(    0,    0,    4,           0,   552, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    1,    4,           0,   552, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  99 ),
};

const Sint32 pl0c_special_00_loop[] = {
	CG_FRAME(    0,    1,    4,           0,   552, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    1,    4, END_OF_DATA,   552, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_NEXT( pl0c_special_00_loop ),
};

/* Char_Special 1 */
const Sint32 pl0c_special_01[] = {
	CG_FRAME(    0,    0,    5,           0,   547, 0x80,   0x53,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   548, 0x80,   0x52,  ATK,  0x00,       0x15,    0,   0,     0,      0,  96 ),
	CG_FRAME(    0,    0,    2,           0,   549, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    1,    1,           0,   549, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,    3,           0,   550, 0x80,   0x20,  ATK,  0x00, 0x16 | CSE,    0,   0,     0,      0,  98 ),
	CG_FRAME(    0,    1,    3,           0,   551, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    4,           0,   552, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    1,    4,           0,   552, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  99 ),
};

const Sint32 pl0c_special_01_loop[] = {
	CG_FRAME(    0,    1,    4,           0,   552, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    1,    4, END_OF_DATA,   552, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_NEXT( pl0c_special_01_loop ),
};

/* Char_Special 3: sting_top (Spider Sting) */
const Sint32 pl0c_special_03[] = {
	CG_FRAME(    0,    0,    8,           0,   562, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,    6,           0,   563, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,    4,           0,   564, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,    6,           0,   194, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0, 101 ),
};

const Sint32 pl0c_special_03_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   195, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_NEXT( pl0c_special_03_loop ),
};

/* Char_Special 4: sting_fall (Spider Sting) */
const Sint32 pl0c_special_04[] = {
	CG_FRAME(    0,    0,    4,           0,   561, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,    4,           0,   194, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0, 101 ),
};

const Sint32 pl0c_special_04_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   195, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_NEXT( pl0c_special_04_loop ),
};

/* Char_Special 5 */
const Sint32 pl0c_special_05[] = {
	CG_FRAME(    0,    0,    3,           0,   196, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 102 ),
	CG_FRAME(    0,    0,    4,           0,   197, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 102 ),
	CG_FRAME(    0,    0,    3,           0,   198, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 102 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   199, 0x80,   0x52,  ATE,  0x00,          0,    0,   0,     0,      0, 102 ),
	CG_NEXT( pl0c_special_05 ),
};

/* Char_Special 6 */
const Sint32 pl0c_special_06[] = {
	CG_FRAME(    0,    0,    4,           0,   196, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 102 ),
	CG_FRAME(    0,    0,    8,           0,   197, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 102 ),
	CG_FRAME(    0,    0,    6,           0,   198, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 102 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   199, 0x80,   0x52,  ATE,  0x00,          0,    0,   0,     0,      0, 102 ),
	CG_NEXT( pl0c_special_06 ),
};

/* Char_Special 2: sting_top (Spider Sting) */
const Sint32 pl0c_special_02[] = {
	CG_FRAME(    0,    0,    1,           0,   553, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0, 103 ),
	CG_FRAME(    0,    0,    3,           0,   554, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0, 103 ),
	CG_FRAME(    0,    0,    3,           0,   555, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0, 103 ),
	CG_FRAME(    0,    0,    3,           0,   556, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0, 103 ),
	CG_FRAME(    0,    0,    3,           0,   557, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0, 104 ),
	CG_FRAME(    0,    0,    3,           0,   558, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0, 105 ),
	CG_FRAME(    0,    0,    3,           0,   559, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0, 106 ),
};

const Sint32 pl0c_special_02_loop[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   560, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0, 107 ),
	CG_NEXT( pl0c_special_02_loop ),
};

/* Char_Special 7 */
const Sint32 pl0c_special_07[] = {
	CG_FRAME(    0,    0,    1,           0,   230, 0xb0,   0x53,  ATK,  0x00,          0,    0,   0,     0,      0, 108 ),
	CG_FRAME(    0,    0,    5,           0,   527, 0xb0,   0x53,  ATK,  0x00,       0x16,    0,   0,     0,      0, 109 ),
	CG_FRAME(    0,    0,    2,           0,   528, 0xb0,   0x51,  ATK,  0x00,          0,    0,   0,     0,      0, 109 ),
	CG_FRAME(    0,    0,    2,           0,   529, 0xb0,   0x51,  ATK,  0x00,          0,    0,   0,     0,      0, 109 ),
	CG_FRAME(    0,    2,    1,           0,   530, 0xb0,   0x51,  ATK,  0x00,          0,    0,   0,     0,      0, 109 ),
	CG_FRAME(    0,    0,    1,           0,   531, 0xb0,   0x51,  ATK,  0x00,          0,    0,   0,     0,      0, 109 ),
	CG_FRAME(    0,    0,    1,           0,   532, 0xb0,   0x51,  ATK,  0x00,       0x21,    0,   0,     0,      0, 109 ),
	CG_FRAME(    0,    1,   11,           0,   533, 0xb0,   0x51,  ATK,  0x00,          0,    0,   0,     0,      0, 109 ),
	CG_FRAME(    0,    0,    5,           0,   534, 0xb0,   0x51,  ATK,  0x00,          0,    0,   0,     0,      0, 109 ),
	CG_FRAME(    0,    0,    2,           0,   534, 0xb0,   0x51,  ATE,  0x00,          0,    0,   0,     0,      0, 109 ),
	CG_FRAME(    0,    1,    4,           0,   535, 0xb0,   0x51,  ATE,  0x00,          0,    0,   0,     0,      0, 109 ),
	CG_FRAME(    0,    0,    6,           0,   230, 0xb0,   0x53,  ATE,  0x00,          0,    0,   0,     0,      0, 108 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   199, 0xb0,   0x52,  ATE,  0x00,          0,    0,   0,     0,      0, 110 ),
	CG_NEXT( pl0c_special_07 ),
};

/* Char_Special 8 */
const Sint32 pl0c_special_08[] = {
	CG_FRAME(    0,    0,    4,           0,   230, 0xb0,   0x53,  ATK,  0x00,          0,    0,   0,     0,      0, 108 ),
	CG_FRAME(    0,    0,    5,           0,   526, 0xb0,   0x53,  ATK,  0x00,       0x16,    0,   0,     0,      0, 109 ),
	CG_FRAME(    0,    0,    8,           0,   527, 0xb0,   0x51,  ATK,  0x00,          0,    0,   0,     0,      0, 109 ),
	CG_FRAME(    0,    0,    3,           0,   528, 0xb0,   0x51,  ATK,  0x00,          0,    0,   0,     0,      0, 109 ),
	CG_FRAME(    0,    0,    2,           0,   529, 0xb0,   0x51,  ATK,  0x00,          0,    0,   0,     0,      0, 109 ),
	CG_FRAME(    0,    2,    2,           0,   530, 0xb0,   0x51,  ATK,  0x00,          0,    0,   0,     0,      0, 109 ),
	CG_FRAME(    0,    0,    2,           0,   531, 0xb0,   0x51,  ATK,  0x00,          0,    0,   0,     0,      0, 109 ),
	CG_FRAME(    0,    0,    2,           0,   532, 0xb0,   0x51,  ATK,  0x00,       0x21,    0,   0,     0,      0, 109 ),
	CG_FRAME(    0,    1,   16,           0,   533, 0xb0,   0x51,  ATK,  0x00,          0,    0,   0,     0,      0, 109 ),
	CG_FRAME(    0,    0,    6,           0,   534, 0xb0,   0x51,  ATE,  0x00,          0,    0,   0,     0,      0, 109 ),
	CG_FRAME(    0,    1,    4,           0,   535, 0xb0,   0x51,  ATE,  0x00,          0,    0,   0,     0,      0, 109 ),
	CG_FRAME(    0,    0,    8,           0,   230, 0xb0,   0x53,  ATE,  0x00,          0,    0,   0,     0,      0, 108 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   199, 0xb0,   0x52,  ATE,  0x00,          0,    0,   0,     0,      0, 110 ),
	CG_NEXT( pl0c_special_08 ),
};

/* Char_Special 9 */
const Sint32 pl0c_special_09[] = {
	CG_FRAME(    0,    0,    4,           0,   579, 0xb0,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0, 111 ),
	CG_FRAME(    0,    0,    3,           0,   580, 0xb0,   0x20,  ATK,  0x00,       0x16,    0,   0,     0,      0, 111 ),
	CG_FRAME(    0,    0,    3,           0,   581, 0xb0,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0, 112 ),
	CG_FRAME(    0,    0,    2,           0,   582, 0xb0,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0, 112 ),
	CG_FRAME(    0,    2,    2,           0,   583, 0xb0,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0, 112 ),
	CG_FRAME(    0,    0,    2,           0,   584, 0xb0,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0, 112 ),
	CG_FRAME(    0,    0,    2,           0,   585, 0xb0,   0x20,  ATK,  0x00,       0x21,    0,   0,     0,      0, 112 ),
	CG_FRAME(    0,    1,    3,           0,   586, 0xb0,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0, 112 ),
	CG_FRAME(    0,    0,    2,           0,   587, 0xb0,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0, 112 ),
	CG_FRAME(    0,    0,    4,           0,   588, 0xb0,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0, 112 ),
	CG_FRAME(    0,    0,    2,           0,   191, 0xb0,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0, 113 ),
	CG_FRAME(    0,    1,    3,           0,   194, 0xb0,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0, 113 ),
};

const Sint32 pl0c_special_09_loop[] = {
	CG_FRAME(    0,    0,    3,           0,   195, 0xb0,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0, 113 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   382, 0xb0,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0, 113 ),
	CG_NEXT( pl0c_special_09_loop ),
};

/* Char_Special 10 */
const Sint32 pl0c_special_10[] = {
	CG_FRAME(    0,    0,    5,           0,   579, 0xb0,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0, 111 ),
	CG_FRAME(    0,    0,    6,           0,   580, 0xb0,   0x20,  ATK,  0x00,       0x16,    0,   0,     0,      0, 111 ),
	CG_FRAME(    0,    0,    4,           0,   581, 0xb0,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0, 112 ),
	CG_FRAME(    0,    0,    4,           0,   582, 0xb0,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0, 112 ),
	CG_FRAME(    0,    2,    3,           0,   583, 0xb0,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0, 112 ),
	CG_FRAME(    0,    0,    3,           0,   584, 0xb0,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0, 112 ),
	CG_FRAME(    0,    0,    3,           0,   585, 0xb0,   0x20,  ATK,  0x00,       0x21,    0,   0,     0,      0, 112 ),
	CG_FRAME(    0,    1,    3,           0,   586, 0xb0,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0, 112 ),
	CG_FRAME(    0,    0,    2,           0,   587, 0xb0,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0, 112 ),
	CG_FRAME(    0,    0,   11,           0,   588, 0xb0,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0, 112 ),
	CG_FRAME(    0,    0,    3,           0,   191, 0xb0,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0, 113 ),
	CG_FRAME(    0,    1,    3,           0,   194, 0xb0,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0, 113 ),
	CG_FRAME(    0,    0,    3,           0,   195, 0xb0,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0, 113 ),
	CG_FRAME(    0,    0,    3,           0,   382, 0xb0,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0, 113 ),
	CG_FRAME(    0,    0,    2,           0,   195, 0xb0,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0, 113 ),
	CG_FRAME(    0,    0,    2,           0,   382, 0xb0,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0, 113 ),
};

const Sint32 pl0c_special_10_loop[] = {
	CG_FRAME(    0,    0,    3,           0,   195, 0xb0,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0, 113 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   382, 0xb0,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0, 113 ),
	CG_NEXT( pl0c_special_10_loop ),
};

/* Char_Special 11 */
const Sint32 pl0c_special_11[] = {
	CG_FRAME(    0,    6,    5,           0,   565, 0xb0,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0, 116 ),
	CG_FRAME(    0,    6,    1,           0,   565, 0xb0,   0x20,  ATK,  0x01,       0x17,    0,   0,     0,      0, 116 ),
	CG_FRAME(    0,    7,    2,           0,   566, 0xb0,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0, 116 ),
	CG_FRAME(    0,    8,    2,           0,   567, 0xb0,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0, 117 ),
	CG_FRAME(    0,    9,    4,           0,   569, 0xb0,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0, 118 ),
	CG_FRAME(    0,   10,    3,           0,   570, 0xb0,   0x20,  ATK,  0x00, 0x16 | CSE,    0,   0,     0,      0, 119 ),
};

const Sint32 pl0c_special_11_loop[] = {
	CG_FRAME(    0,   10,    3, END_OF_DATA,   570, 0xb0,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0, 119 ),
	CG_NEXT( pl0c_special_11_loop ),
};

/* Char_Special 12 */
const Sint32 pl0c_special_12[] = {
	CG_FRAME(    0,    6,    5,           0,   565, 0xb0,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0, 116 ),
	CG_FRAME(    0,    6,    1,           0,   565, 0xb0,   0x20,  ATK,  0x01,       0x17,    0,   0,     0,      0, 116 ),
	CG_FRAME(    0,    7,    4,           0,   566, 0xb0,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0, 116 ),
	CG_FRAME(    0,    8,    4,           0,   567, 0xb0,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0, 117 ),
	CG_FRAME(    0,    9,    4,           0,   569, 0xb0,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0, 118 ),
	CG_FRAME(    0,   10,    3,           0,   570, 0xb0,   0x20,  ATK,  0x00, 0x16 | CSE,    0,   0,     0,      0, 119 ),
};

const Sint32 pl0c_special_12_loop[] = {
	CG_FRAME(    0,   10,    3, END_OF_DATA,   570, 0xb0,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0, 119 ),
	CG_NEXT( pl0c_special_12_loop ),
};

/* Char_Special 17 */
const Sint32 pl0c_special_17[] = {
	CG_FRAME(    0,    6,    5,           0,   565, 0xb0,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0, 184 ),
	CG_FRAME(    0,    6,    1,           0,   565, 0xb0,   0x20,  ATK,  0x01,       0x17,    0,   0,     0,      0, 184 ),
	CG_FRAME(    0,    7,    3,           0,   566, 0xb0,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0, 184 ),
	CG_FRAME(    0,    8,    3,           0,   567, 0xb0,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0, 117 ),
	CG_FRAME(    0,    9,    4,           0,   569, 0xb0,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0, 118 ),
	CG_FRAME(    0,   10,    3,           0,   570, 0xb0,   0x20,  ATK,  0x00, 0x16 | CSE,    0,   0,     0,      0, 119 ),
};

const Sint32 pl0c_special_17_loop[] = {
	CG_FRAME(    0,   10,    3, END_OF_DATA,   570, 0xb0,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0, 119 ),
	CG_NEXT( pl0c_special_17_loop ),
};

/* Char_Special 13 */
const Sint32 pl0c_special_13[] = {
	CG_FRAME(    0,    0,    3,           0,   571, 0xb0,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0, 113 ),
	CG_FRAME(    0,    0,    3,           0,   205, 0xb0,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0, 113 ),
	CG_FRAME(    0,    0,    3,           0,   204, 0xb0,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0, 113 ),
	CG_FRAME(    0,    0,    3,           0,   203, 0xb0,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0, 113 ),
	CG_FRAME(    0,    0,    3,           0,   383, 0xb0,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0, 113 ),
	CG_FRAME(    0,    0,    3,           0,   207, 0xb0,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0, 113 ),
	CG_FRAME(    0,    0,    4,           0,   214, 0xb0,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0, 113 ),
	CG_FRAME(    0,    0,    4,           0,   215, 0xb0,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0, 113 ),
};

const Sint32 pl0c_special_13_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   216, 0xb0,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0, 113 ),
	CG_NEXT( pl0c_special_13_loop ),
};

/* Char_Special 14 */
const Sint32 pl0c_special_14[] = {
	CG_FRAME(    0,   12,    5,           0,   572, 0xb0,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0, 116 ),
	CG_FRAME(    0,   12,    1,           0,   572, 0xb0,   0x20,  ATK,  0x01,       0x17,    0,   0,     0,      0, 116 ),
	CG_FRAME(    0,   13,    2,           0,   573, 0xb0,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0, 116 ),
	CG_FRAME(    0,   14,    2,           0,   574, 0xb0,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0, 116 ),
	CG_FRAME(    0,   15,    3,           0,   576, 0xb0,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0, 121 ),
	CG_FRAME(    0,   16,    3,           0,   577, 0xb0,   0x20,  ATK,  0x00, 0x16 | CSE,    0,   0,     0,      0, 122 ),
};

const Sint32 pl0c_special_14_loop[] = {
	CG_FRAME(    0,   16,    3, END_OF_DATA,   577, 0xb0,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0, 122 ),
	CG_NEXT( pl0c_special_14_loop ),
};

/* Char_Special 15 */
const Sint32 pl0c_special_15[] = {
	CG_FRAME(    0,   12,    5,           0,   572, 0xb0,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0, 116 ),
	CG_FRAME(    0,   12,    1,           0,   572, 0xb0,   0x20,  ATK,  0x01,       0x17,    0,   0,     0,      0, 116 ),
	CG_FRAME(    0,   13,    4,           0,   573, 0xb0,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0, 116 ),
	CG_FRAME(    0,   14,    4,           0,   574, 0xb0,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0, 116 ),
	CG_FRAME(    0,   15,    4,           0,   576, 0xb0,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0, 121 ),
	CG_FRAME(    0,   16,    3,           0,   577, 0xb0,   0x20,  ATK,  0x00, 0x16 | CSE,    0,   0,     0,      0, 122 ),
};

const Sint32 pl0c_special_15_loop[] = {
	CG_FRAME(    0,   16,    3, END_OF_DATA,   577, 0xb0,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0, 122 ),
	CG_NEXT( pl0c_special_15_loop ),
};

/* Char_Special 16 */
const Sint32 pl0c_special_16[] = {
	CG_FRAME(    0,    1,    5,           0,   578, 0xb0,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0, 113 ),
	CG_FRAME(    0,    1,    5,           0,   205, 0xb0,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0, 113 ),
	CG_FRAME(    0,    1,    4,           0,   204, 0xb0,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0, 113 ),
	CG_FRAME(    0,    1,    4,           0,   203, 0xb0,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0, 113 ),
	CG_FRAME(    0,    1,    4,           0,   383, 0xb0,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0, 113 ),
	CG_FRAME(    0,    1,    5,           0,   191, 0xb0,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0, 113 ),
	CG_FRAME(    0,    1,    5,           0,   194, 0xb0,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0, 113 ),
	CG_FRAME(    0,    1,    4,           0,   195, 0xb0,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0, 113 ),
	CG_FRAME(    0,    0,    4,           0,   382, 0xb0,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0, 113 ),
	CG_FRAME(    0,    0,    4,           0,   195, 0xb0,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0, 113 ),
	CG_FRAME(    0,    0,    4,           0,   382, 0xb0,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0, 113 ),
};

const Sint32 pl0c_special_16_loop[] = {
	CG_FRAME(    0,    0,    3,           0,   195, 0xb0,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0, 113 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   382, 0xb0,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0, 113 ),
	CG_NEXT( pl0c_special_16_loop ),
};

/* Char_Special 18: webthrow_init (Web Throw, 18 + atck_level) */
const Sint32 pl0c_special_18[] = {
	CG_FRAME(    0,    0,    3,           0,   226, 0xb0,   0x53,  ATK,  0x00,          0,    0,   0,     0,      0, 110 ),
	CG_FRAME(    0,    0,    4,           0,   663, 0xb0,   0x52,  ATK,  0x00,          0,    0,   0,     0,      0, 110 ),
	CG_FRAME(    0,    0,    8,           0,   664, 0xb0,   0x52,  ATK,  0x00,          0,    0,   0,     0,      0, 110 ),
	CG_FRAME(    0,    0,    5,           0,   665, 0xb0,   0x51,  ATK,  0x00,          0,    0,   0,     0,      0, 110 ),
	CG_FRAME(    0,    0,    2,           0,   666, 0xb0,   0x51,  ATK,  0x00,       0x19,    0,   0,     0,      0, 123 ),
	CG_FRAME(    0,    0,    2,           0,   666, 0xb0,   0x51,  ATK,  0x00,       0x20,    0,   0,     0,      0, 123 ),
};

const Sint32 pl0c_special_18_loop[] = {
	CG_FRAME(   24,    0,    2,           0,   667, 0xb0,   0x51,  ATE,  0x01,          0,    0,   0,     0,      0, 123 ),
	CG_FRAME(   24,    0,    2, END_OF_DATA,   692, 0xb0,   0x51,  ATE,  0x00,          0,    0,   0,     0,      0, 123 ),
	CG_NEXT( pl0c_special_18_loop ),
};

/* Char_Special 19
   Char_Special 20 */
const Sint32 pl0c_special_19[] = {
	CG_FRAME(    0,    0,    2,           0,   226, 0xb0,   0x53,  ATK,  0x00,          0,    0,   0,     0,      0, 110 ),
	CG_FRAME(    0,    0,    3,           0,   663, 0xb0,   0x52,  ATK,  0x00,          0,    0,   0,     0,      0, 110 ),
	CG_FRAME(    0,    0,    6,           0,   664, 0xb0,   0x52,  ATK,  0x00,          0,    0,   0,     0,      0, 110 ),
	CG_FRAME(    0,    0,    4,           0,   665, 0xb0,   0x51,  ATK,  0x00,          0,    0,   0,     0,      0, 110 ),
	CG_FRAME(    0,    1,    2,           0,   668, 0xb0,   0x51,  ATK,  0x00,       0x19,    0,   0,     0,      0, 124 ),
	CG_FRAME(    0,    1,    2,           0,   668, 0xb0,   0x51,  ATK,  0x00,       0x20,    0,   0,     0,      0, 124 ),
};

const Sint32 pl0c_special_19_loop[] = {
	CG_FRAME(   28,    1,    2,           0,   669, 0xb0,   0x51,  ATE,  0x01,          0,    0,   0,     0,      0, 124 ),
	CG_FRAME(   28,    1,    2, END_OF_DATA,   693, 0xb0,   0x51,  ATE,  0x00,          0,    0,   0,     0,      0, 124 ),
	CG_NEXT( pl0c_special_19_loop ),
};

/* Char_Special 21: webthrow_pull (Web Throw, 21 + atck_level) */
const Sint32 pl0c_special_21[] = {
	CG_FRAME(   24,    0,    9,           0,   666, 0xb0,   0x51,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   24,    0,    4,           0,   667, 0xb0,   0x51,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   24,    3,    4,           0,   672, 0xb0,   0x60,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   24,    4,    4,           0,   673, 0xb0,   0x60,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   24,    5,    7, END_OF_DATA,   674, 0xb0,   0x60,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0c_special_21 ),
};

/* Char_Special 22
   Char_Special 23 */
const Sint32 pl0c_special_22[] = {
	CG_FRAME(   28,    1,    9,           0,   668, 0xb0,   0x51,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   26,    1,    4,           0,   669, 0xb0,   0x51,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   24,    3,    4,           0,   672, 0xb0,   0x60,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   23,    4,    4,           0,   673, 0xb0,   0x60,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   24,    5,    7, END_OF_DATA,   674, 0xb0,   0x60,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0c_special_22 ),
};

/* Char_Catch 3: webthrow_catch (Web Throw) */
const Sint32 pl0c_catch_03[] = {
	CG_FRAME(    0,    0,    8,           0,   675, 0xb0,   0x60,    0,  0x00,       0x18,    0,   0,     0, 0x0015,   0 ),
	CG_FRAME(    0,    0,    6,           0,   676, 0xb0,   0x60,    0,  0x01,       0x1f,    0,   0,     0, 0x0016,   0 ),
	CG_FRAME(    0,    0,    7,           0,   677, 0xb0,   0x60,    0,  0x00,          0,    0,   0,     0, 0x0017,   0 ),
	CG_FRAME(    0,    0,    9,           0,   678, 0xb0,   0x60,    0,  0x00,          0,    0,   0,     0, 0x0018,   0 ),
	CG_FRAME(    0,    0,    6,           0,   679, 0xb0,   0x60,    0,  0x00,          0,    0,   0,     0, 0x0019,   0 ),
	CG_FRAME(    0,    0,    4,           0,   680, 0xb0,   0x60,    0,  0x00,          0,    0,   0,     0, 0x001a,   0 ),
};

const Sint32 pl0c_catch_03_loop[] = {
	CG_FRAME(    0,    3,    4,           0,   681, 0xb0,   0x60,    0,  0x01,          0,    0,   0,     0, 0x001b,   0 ),
	CG_FRAME(    0,    0,    6,           0,   682, 0xb0,   0x60,    0,  0x00,          0,    0,   0,     0, 0x001c,   0 ),
	CG_FRAME(    0,    0,    4,           0,   675, 0xb0,   0x60,    0,  0x00,          0,    0,   0,     0, 0x0015,   0 ),
	CG_FRAME(    0,    0,    2,           0,   676, 0xb0,   0x60,    0,  0x00,       0x1f,    0,   0,     0, 0x0016,   0 ),
	CG_FRAME(    0,    2,    4,           0,   677, 0xb0,   0x60,    0,  0x00,          0,    0,   0,     0, 0x0017,   0 ),
	CG_FRAME(    0,    0,    6,           0,   678, 0xb0,   0x60,    0,  0x00,          0,    0,   0,     0, 0x0018,   0 ),
	CG_FRAME(    0,    0,    4,           0,   679, 0xb0,   0x60,    0,  0x00,          0,    0,   0,     0, 0x0019,   0 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   680, 0xb0,   0x60,    0,  0x00,          0,    0,   0,     0, 0x001a,   0 ),
	CG_NEXT( pl0c_catch_03_loop ),
};

/* Char_Catch 4: webthrow_swing (Web Throw) */
const Sint32 pl0c_catch_04[] = {
	CG_FRAME(    0,    0,    4,           0,   683, 0xb0,   0x60,    0,  0x00,          0,    0,   0,     0, 0x001d,   0 ),
	CG_FRAME(    0,    0,    6,           0,   684, 0xb0,   0x60,    0,  0x00,          0,    0,   0,     0, 0x001e,   0 ),
	CG_FRAME(    0,    0,    5,           0,   685, 0xb0,   0x60,    0,  0x00,          0,    0,   0,     0, 0x001f,   0 ),
	CG_FRAME(    0,    0,    4,           0,   686, 0xb0,   0x60,    0,  0x00,          0,    0,   0,     0, 0x0020,   0 ),
	CG_FRAME(    0,    0,    4,           0,   687, 0xb0,   0x60,    0,  0x00,          0,    0,   0,     0, 0x0021,   0 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   688, 0xb0,   0x60,    0,  0x00,       0x03,    0,   0,     0, 0x0022,   0 ),
	CG_NEXT( pl0c_catch_04 ),
};

/* Char_Special 24: webthrow_throw (Web Throw) */
const Sint32 pl0c_special_24[] = {
	CG_FRAME(    0,    0,   12,           0,   689, 0xb0,   0x60,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    3,           0,   412, 0xb0,   0x53,    0,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    3,           0,   306, 0xb0,   0x53,    0,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    3,           0,   695, 0xb0,   0x71,    0,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    3,           0,   696, 0xb0,   0x71,    0,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    3,           0,   697, 0xb0,   0x71,    0,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    3,           0,   698, 0xb0,   0x71,    0,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    3,           0,   699, 0xb0,   0x71,    0,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,   16,           0,   700, 0xb0,   0x71,    0,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    3,           0,   306, 0xb0,   0x53,    0,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   412, 0xb0,   0x53,    0,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_NEXT( pl0c_special_24 ),
};

/* Char_Special 25: webthrow_pull (Web Throw, 25 + atck_level), webthrow_catch (Web Throw, 25 + atck_level) */
const Sint32 pl0c_special_25[] = {
	CG_FRAME(    0,    0,    2,           0,   667, 0xb0,   0x60,  ATE,  0x00,          0,    0,   0,     0,      0, 123 ),
	CG_FRAME(    0,    0,    2,           0,   692, 0xb0,   0x60,  ATE,  0x00,          0,    0,   0,     0,      0, 123 ),
	CG_FRAME(    0,    0,    2,           0,   667, 0xb0,   0x60,  ATE,  0x00,          0,    0,   0,     0,      0, 123 ),
	CG_FRAME(    0,    0,    2,           0,   692, 0xb0,   0x60,  ATE,  0x00,          0,    0,   0,     0,      0, 123 ),
	CG_FRAME(    0,    0,    2,           0,   667, 0xb0,   0x60,  ATE,  0x00,          0,    0,   0,     0,      0, 123 ),
	CG_FRAME(    0,    0,    2,           0,   692, 0xb0,   0x60,  ATE,  0x00,          0,    0,   0,     0,      0, 123 ),
	CG_FRAME(    0,    0,    3,           0,   667, 0xb0,   0x60,  ATE,  0x00,          0,    0,   0,     0,      0, 123 ),
	CG_FRAME(    0,    0,    3,           0,   692, 0xb0,   0x60,  ATE,  0x00,          0,    0,   0,     0,      0, 123 ),
	CG_FRAME(    0,    0,    3,           0,   230, 0xb0,   0x53,  ATE,  0x00,          0,    0,   0,     0,      0, 108 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   199, 0xb0,   0x53,  ATE,  0x00,          0,    0,   0,     0,      0, 110 ),
	CG_NEXT( pl0c_special_25 ),
};

/* Char_Special 26
   Char_Special 27 */
const Sint32 pl0c_special_26[] = {
	CG_FRAME(    4,    0,    2,           0,   669, 0xb0,   0x60,  ATE,  0x00,          0,    0,   0,     0,      0, 124 ),
	CG_FRAME(    4,    0,    2,           0,   693, 0xb0,   0x60,  ATE,  0x00,          0,    0,   0,     0,      0, 124 ),
	CG_FRAME(    4,    0,    2,           0,   669, 0xb0,   0x60,  ATE,  0x00,          0,    0,   0,     0,      0, 124 ),
	CG_FRAME(    4,    0,    2,           0,   693, 0xb0,   0x60,  ATE,  0x00,          0,    0,   0,     0,      0, 124 ),
	CG_FRAME(    4,    0,    2,           0,   669, 0xb0,   0x60,  ATE,  0x00,          0,    0,   0,     0,      0, 124 ),
	CG_FRAME(    4,    0,    2,           0,   693, 0xb0,   0x60,  ATE,  0x00,          0,    0,   0,     0,      0, 124 ),
	CG_FRAME(    4,    0,    3,           0,   669, 0xb0,   0x60,  ATE,  0x00,          0,    0,   0,     0,      0, 124 ),
	CG_FRAME(    4,    0,    3,           0,   693, 0xb0,   0x60,  ATE,  0x00,          0,    0,   0,     0,      0, 124 ),
	CG_FRAME(    0,    0,    3,           0,   230, 0xb0,   0x53,  ATE,  0x00,          0,    0,   0,     0,      0, 108 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   199, 0xb0,   0x53,  ATE,  0x00,          0,    0,   0,     0,      0, 110 ),
	CG_NEXT( pl0c_special_26 ),
};

/* Char_Super 0 */
const Sint32 pl0c_super_00[] = {
	CG_FRAME(    0,    0,    4,           0,   168, 0x80,   0x2d,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    4,           0,   439, 0xc0,   0x0c,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
};

/* Char_Super 1 */
const Sint32 pl0c_super_01[] = {
	CG_FRAME(    0,    0,    4,           0,   211, 0xc0,   0x20,  ATK,  0x01,          0,    0,   0,     0,      0, 158 ),
	CG_FRAME(    0,    0,    4,           0,   212, 0xc0,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0, 158 ),
	CG_FRAME(    0,    0,    4,           0,   213, 0xc0,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0, 158 ),
};

const Sint32 pl0c_super_01_loop[] = {
	CG_FRAME(    0,    0,    3,           0,   205, 0xc0,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0, 158 ),
	CG_FRAME(    0,    0,    3,           0,   204, 0xc0,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0, 158 ),
	CG_FRAME(    0,    0,    3,           0,   203, 0xc0,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0, 158 ),
	CG_FRAME(    0,    0,    3,           0,   383, 0xc0,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0, 158 ),
	CG_FRAME(    0,    0,    3,           0,   207, 0xc0,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0, 158 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   206, 0xc0,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0, 158 ),
	CG_NEXT( pl0c_super_01_loop ),
};

/* Char_Super 2: maximum_dash_hit (Maximum Spider) */
const Sint32 pl0c_super_02[] = {
	CG_FRAME(    0,    0,    3,           0,   200, 0x90,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,    5,           0,   201, 0x90,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,    4,           0,   202, 0x90,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  82 ),
};

const Sint32 pl0c_super_02_loop[] = {
	CG_FRAME(    0,    0,    4,           0,   203, 0x90,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,    4,           0,   204, 0x90,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,    4,           0,   205, 0x90,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,    4,           0,   206, 0x90,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,    4,           0,   207, 0x90,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   383, 0x90,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_NEXT( pl0c_super_02_loop ),
};

/* Char_Super 3: maximum_dash_move (Maximum Spider), maximum_dash_hit (Maximum Spider) */
const Sint32 pl0c_super_03[] = {
	CG_FRAME(    0,    0,    4,           0,   211, 0x90,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,    4,           0,   212, 0x90,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,    4,           0,   213, 0x90,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  82 ),
};

const Sint32 pl0c_super_03_loop[] = {
	CG_FRAME(    0,    0,    4,           0,   205, 0x90,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,    4,           0,   204, 0x90,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,    4,           0,   203, 0x90,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,    4,           0,   383, 0x90,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,    4,           0,   207, 0x90,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   206, 0x90,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_NEXT( pl0c_super_03_loop ),
};

/* Char_Super 12 */
const Sint32 pl0c_super_12[] = {
	CG_FRAME(    8,    0,    4,           0,   620, 0xc0,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0, 167 ),
};

/* Char_Super 13 */
const Sint32 pl0c_super_13[] = {
	CG_FRAME(    8,    1,    4,           0,   621, 0xc0,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0, 168 ),
};

/* Char_Super 14 */
const Sint32 pl0c_super_14[] = {
	CG_FRAME(    6,    2,    4,           0,   622, 0xc0,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0, 169 ),
};

/* Char_Super 15 */
const Sint32 pl0c_super_15[] = {
	CG_FRAME(    0,    3,    4,           0,   623, 0xc0,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0, 170 ),
};

/* Char_Super 16 */
const Sint32 pl0c_super_16[] = {
	CG_FRAME(    2,    4,    4,           0,   624, 0xc0,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0, 171 ),
};

/* Char_Super 17 */
const Sint32 pl0c_super_17[] = {
	CG_FRAME(    2,    5,    4,           0,   625, 0xc0,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0, 172 ),
};

/* Char_Super 18 */
const Sint32 pl0c_super_18[] = {
	CG_FRAME(    4,    6,    4,           0,   626, 0xc0,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0, 173 ),
};

/* Char_Super 19 */
const Sint32 pl0c_super_19[] = {
	CG_FRAME(    4,    7,    4,           0,   627, 0xc0,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0, 174 ),
};

/* Char_Super 20 */
const Sint32 pl0c_super_20[] = {
	CG_FRAME(    4,    8,    4,           0,   612, 0xc0,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0, 159 ),
};

/* Char_Super 21 */
const Sint32 pl0c_super_21[] = {
	CG_FRAME(    4,    9,    4,           0,   613, 0xc0,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0, 160 ),
};

/* Char_Super 22 */
const Sint32 pl0c_super_22[] = {
	CG_FRAME(    4,   10,    4,           0,   614, 0xc0,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0, 161 ),
};

/* Char_Super 23 */
const Sint32 pl0c_super_23[] = {
	CG_FRAME(    2,   11,    4,           0,   615, 0xc0,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0, 162 ),
};

/* Char_Super 24 */
const Sint32 pl0c_super_24[] = {
	CG_FRAME(    6,   12,    4,           0,   616, 0xc0,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0, 163 ),
};

/* Char_Super 25 */
const Sint32 pl0c_super_25[] = {
	CG_FRAME(    6,   13,    4,           0,   617, 0xc0,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0, 164 ),
};

/* Char_Super 26 */
const Sint32 pl0c_super_26[] = {
	CG_FRAME(    6,   14,    4,           0,   618, 0xc0,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0, 165 ),
};

/* Char_Super 27 */
const Sint32 pl0c_super_27[] = {
	CG_FRAME(    8,   15,    4, END_OF_DATA,   619, 0xc0,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0, 166 ),
	CG_NEXT( pl0c_super_12 ),
};

/* Char_Super 4 */
const Sint32 pl0c_super_04[] = {
	CG_FRAME(    0,    0,    1,           0,   628, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 175 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   629, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 175 ),
	CG_NEXT( pl0c_super_04 ),
};

/* Char_Super 5 */
const Sint32 pl0c_super_05[] = {
	CG_FRAME(    0,    0,    1,           0,   630, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 175 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   631, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 175 ),
	CG_NEXT( pl0c_super_05 ),
};

/* Char_Super 6 */
const Sint32 pl0c_super_06[] = {
	CG_FRAME(    0,    2,    1,           0,   632, 0xc0,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0, 176 ),
	CG_FRAME(    0,    2,    3, END_OF_DATA,   633, 0xc0,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0, 176 ),
	CG_NEXT( pl0c_super_06 ),
};

/* Char_Super 7 */
const Sint32 pl0c_super_07[] = {
	CG_FRAME(    0,    2,    1,           0,   634, 0xc0,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0, 176 ),
	CG_FRAME(    0,    2,    3, END_OF_DATA,   635, 0xc0,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0, 176 ),
	CG_NEXT( pl0c_super_07 ),
};

/* Char_Super 10: maximum_rise (Maximum Spider) */
const Sint32 pl0c_super_10[] = {
	CG_FRAME(    0,    4,    1,           0,   636, 0xc0,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0, 177 ),
	CG_FRAME(    0,    4,    3, END_OF_DATA,   637, 0xc0,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0, 177 ),
	CG_NEXT( pl0c_super_10 ),
};

/* Char_Super 11 */
const Sint32 pl0c_super_11[] = {
	CG_FRAME(    0,    4,    1,           0,   638, 0xc0,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0, 177 ),
	CG_FRAME(    0,    4,    3, END_OF_DATA,   639, 0xc0,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0, 177 ),
	CG_NEXT( pl0c_super_11 ),
};

/* Char_Super 8 */
const Sint32 pl0c_super_08[] = {
	CG_FRAME(    0,    6,    1,           0,   640, 0xc0,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0, 178 ),
	CG_FRAME(    0,    6,    3, END_OF_DATA,   641, 0xc0,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0, 178 ),
	CG_NEXT( pl0c_super_08 ),
};

/* Char_Super 9 */
const Sint32 pl0c_super_09[] = {
	CG_FRAME(    0,    6,    1,           0,   642, 0xc0,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0, 178 ),
	CG_FRAME(    0,    6,    3, END_OF_DATA,   643, 0xc0,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0, 178 ),
	CG_NEXT( pl0c_super_09 ),
};

/* Char_Special 30: chouhatsu_init (Taunt) */
const Sint32 pl0c_special_30[] = {
	CG_FRAME(    0,    0,    3,           0,   412, 0x80,   0x53,    0,  0x00,          0,    0,   0,     0,      0, 179 ),
	CG_FRAME(    0,    0,    3,           0,   306, 0x80,   0x53,    0,  0x00,          0,    0,   0,     0,      0, 179 ),
	CG_FRAME(    0,    0,    4,           0,   695, 0x80,   0x60,    0,  0x00,          0,    0,   0,     0,      0, 179 ),
	CG_FRAME(    0,    0,    4,           0,   696, 0x80,   0x60,    0,  0x00,          0,    0,   0,     0,      0, 179 ),
	CG_FRAME(    0,    0,    4,           0,   697, 0x80,   0x60,    0,  0x00,          0,    0,   0,     0,      0, 179 ),
	CG_FRAME(    0,    0,    4,           0,   698, 0x80,   0x60,    0,  0x00,          0,    0,   0,     0,      0, 179 ),
	CG_FRAME(    0,    1,    4,           0,   699, 0x80,   0x60,    0,  0x00,          0,    0,   0,     0,      0, 179 ),
	CG_FRAME(    0,    0,   26,           0,   700, 0x80,   0x60,    0,  0x00,          0,    0,   0,     0,      0, 179 ),
	CG_FRAME(    0,    2,    4,           0,   306, 0x80,   0x53,    0,  0x00,          0,    0,   0,     0,      0, 179 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   412, 0x80,   0x53,    0,  0x00,          0,    0,   0,     0,      0, 179 ),
	CG_NEXT( pl0c_special_30 ),
};

/* Char_Change 4: act_change_assist_end */
const Sint32 pl0c_change_04[] = {
	CG_FRAME(    0,    0,    3,           0,   412, 0x80,   0x53,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   306, 0x80,   0x53,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   695, 0x80,   0x60,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   696, 0x80,   0x60,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   697, 0x80,   0x60,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   698, 0x80,   0x60,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    4,           0,   699, 0x80,   0x60,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   26,           0,   700, 0x80,   0x60,    0,  0x00,       0x0d,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    2,    4,           0,   306, 0x80,   0x53,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   412, 0x80,   0x53,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0c_change_04 ),
};

const Sint32 pl0c_unused_02[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,    3,           0,   412, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 179 ),
	CG_FRAME(    0,    0,    3,           0,   306, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 179 ),
	CG_FRAME(    0,    0,    4,           0,   695, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 179 ),
	CG_FRAME(    0,    0,    4,           0,   696, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 179 ),
	CG_FRAME(    0,    0,    4,           0,   697, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 179 ),
	CG_FRAME(    0,    0,    4,           0,   698, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 179 ),
	CG_FRAME(    0,    1,    4,           0,   699, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 179 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   700, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 179 ),
	0x00005cb4,
};

/* Char_Special 28: t_kick_init */
const Sint32 pl0c_special_28[] = {
	CG_FRAME(    0,    0,    4,           0,   615, 0x80,   0x20,  ATK,  0x00,       0x04,    0,   0,     0,      0, 183 ),
};

const Sint32 pl0c_special_28_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   615, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0, 183 ),
	CG_NEXT( pl0c_special_28_loop ),
};

/* Char_Special 29: t_kick_move */
const Sint32 pl0c_special_29[] = {
	CG_FRAME(    0,    0,    2,           0,   412, 0x80,   0x53,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   270, 0x80,   0x53,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   389, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   390, 0x80,   0x00,    0,  0x00,       0x0b,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   391, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,   392, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   393, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   13,           0,   394, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   389, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   270, 0x80,   0x53,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   412, 0x80,   0x53,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0c_special_29 ),
};

/* Char_Change 0: act_change_tag_init */
const Sint32 pl0c_change_00[] = {
	CG_FRAME(    0,    0,    1,           0,   412, 0x80,   0x53,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   270, 0x80,   0x53,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   271, 0x80,   0x13,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   272, 0x80,   0x12,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   273, 0x80,   0x12,    0,  0x00,       0x0d,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    7,           0,   274, 0x80,   0x12,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    6,           0,   275, 0x80,   0x12,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl0c_change_00_loop[] = {
	CG_FRAME(    0,    0,    6, END_OF_DATA,   276, 0x80,   0x12,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0c_change_00_loop ),
};

/* Char_Change 2: act_change_out_bound */
const Sint32 pl0c_change_02[] = {
	CG_FRAME(    0,    0,    3,           0,   211, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   212, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   213, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl0c_change_02_loop[] = {
	CG_FRAME(    0,    0,    3,           0,   205, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   204, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   203, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   383, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   207, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl0c_change_02_loop ),
};

/* Char_Change 3: act_stand, act_change_call */
const Sint32 pl0c_change_03[] = {
	CG_FRAME(    0,    0,    3,           0,   222, 0x80,   0x60,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   395, 0x80,   0x60,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    3,           0,   396, 0x80,   0x4f,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   397, 0x80,   0x4f,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   398, 0x80,   0x4f,    0,  0x00,       0x0c,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   399, 0x80,   0x4f,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   400, 0x80,   0x4f,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   40,           0,   401, 0x80,   0x4f,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   222, 0x80,   0x60,    0,  0x00,          0,    0,   0,     0,      0, 185 ),
};

const Sint32 pl0c_change_03_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   223, 0x80,   0x53,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0c_change_03_loop ),
};

/* Char_Super 29: crawler_init (Crawler Assault) */
const Sint32 pl0c_super_29[] = {
	CG_FRAME(    0,    0,    6,           0,   197, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    7,           0,   198, 0x80,   0x52,  ATK,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,   11,           0,   644, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    1,    0,    4,           0,   645, 0x80,   0x00,  ATK,  0x5e,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    2,           0,   646, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,    2,           0,   647, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 187 ),
	CG_FRAME(    0,    0,    2,           0,   648, 0x80,   0x00,  ATK,  0x00,       0x03,    0,   0,     0,      0, 186 ),
	CG_FRAME(    0,    0,    3,           0,   650, 0x80,   0x00,  ATK,  0x5f,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,    3,           0,   652, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 186 ),
	CG_FRAME(    0,    0,    2,           0,   654, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,    2,           0,   647, 0x80,   0x00,  ATK,  0x60,          0,    0,   0,     0,      0, 187 ),
	CG_FRAME(    0,    0,    2,           0,   648, 0x80,   0x00,  ATK,  0x00,       0x03,    0,   0,     0,      0, 186 ),
	CG_FRAME(    0,    0,    4,           0,   198, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    3,           0,   413, 0x80,   0x00,  ATK,  0x5f,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,    3,           0,   415, 0x80,   0x00,  ATK,  0x00,       0x05,    0,   0,     0,      0, 182 ),
	CG_FRAME(    0,    0,    3,           0,   421, 0x80,   0x00,  ATK,  0x60,          0,    0,   0,     0,      0,  22 ),
	CG_FRAME(    0,    0,    2,           0,   423, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  22 ),
	CG_FRAME(    0,    0,    2,           0,   424, 0x80,   0x00,  ATK,  0x00,       0x03,    0,   0,     0,      0, 188 ),
	CG_FRAME(    0,    0,    2,           0,   427, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    2,           0,   432, 0x80,   0x00,  ATK,  0x5f,          0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,    3,           0,   435, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  39 ),
	CG_FRAME(    0,    0,    1,           0,   436, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  40 ),
	CG_FRAME(    0,    0,    1,           0,   437, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,    1,           0,   438, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    3,           0,   439, 0x80,   0x00,  ATK,  0x61,          0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,    1,           0,   441, 0x80,   0x00,  ATK,  0x00,       0x03,    0,   0,     0,      0,  39 ),
	CG_FRAME(    0,    0,    1,           0,   442, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  40 ),
	CG_FRAME(    0,    0,    1,           0,   443, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,    1,           0,   445, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    3,           0,   197, 0x80,   0x00,  ATK,  0x63,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    3,           0,   655, 0x90,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,    2,           0,   656, 0x90,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,    1,           0,   657, 0x90,   0x20,  ATK,  0x00,       0x04,    0,   0,     0,      0,  82 ),
};

const Sint32 pl0c_super_29_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   658, 0xc0,   0x20,  ATK,  0x62,          0,    0,   0,     0,      0, 162 ),
	CG_NEXT( pl0c_super_29_loop ),
};

/* Char_Super 31: crawler_move (Crawler Assault) */
const Sint32 pl0c_super_31[] = {
	CG_FRAME(    0,    0,    4,           0,   659, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    5,           0,   660, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    7,           0,   197, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    8,           0,   198, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   199, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    7, END_OF_DATA,   412, 0x80,   0x54,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0c_super_31 ),
};

/* Char_Super 32: hwebthrow_init (Ultimate Web Throw) */
const Sint32 pl0c_super_32[] = {
	CG_FRAME(    0,    1,    8,           0,   197, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    4,           0,   211, 0x90,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,   16,           0,   661, 0x90,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,    7,           0,   662, 0x90,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    2,    2,           0,   579, 0xb0,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0, 111 ),
	CG_FRAME(    0,    0,    2,           0,   580, 0xb0,   0x20,  ATK,  0x00,       0x19,    0,   0,     0,      0, 111 ),
	CG_FRAME(    0,    0,    2,           0,   581, 0xb0,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0, 112 ),
	CG_FRAME(    0,    1,    3,           0,   586, 0xb0,   0x20,  ATK,  0x00,       0x20,    0,   0,     0,      0, 112 ),
	CG_FRAME(    0,    0,    6,           0,   587, 0xb0,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0, 112 ),
};

const Sint32 pl0c_super_32_loop[] = {
	CG_FRAME(    0,   52,    6, END_OF_DATA,   588, 0xb0,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0, 112 ),
	CG_NEXT( pl0c_super_32_loop ),
};

/* Char_Super 34: hwebthrow_catch (Ultimate Web Throw) */
const Sint32 pl0c_super_34[] = {
	CG_FRAME(    0,    0,    4,           0,   191, 0xb0,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0, 113 ),
	CG_FRAME(    0,    1,    4,           0,   194, 0xb0,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0, 113 ),
	CG_FRAME(    0,    0,    3,           0,   195, 0xb0,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0, 113 ),
	CG_FRAME(    0,    0,    3,           0,   382, 0xb0,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0, 113 ),
	CG_FRAME(    0,    0,    3,           0,   195, 0xb0,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0, 113 ),
	CG_FRAME(    0,    0,    3,           0,   382, 0xb0,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0, 113 ),
};

const Sint32 pl0c_super_34_loop[] = {
	CG_FRAME(    0,    0,    3,           0,   195, 0xb0,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0, 113 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   382, 0xb0,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0, 113 ),
	CG_NEXT( pl0c_super_34_loop ),
};

/* Char_Super 35: hwebthrow_catch (Ultimate Web Throw) */
const Sint32 pl0c_super_35[] = {
	CG_FRAME(    0,   25,    8,           0,   586, 0xb0,   0x20,  ATK,  0x14,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl0c_super_35_loop[] = {
	CG_FRAME(    0,   19,    4,           0,   383, 0x80,   0x20,    0,  0x0c,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,   20,    4,           0,   203, 0x90,   0x20,  ATK,  0x04,          0,    0,   0,     5,      0,   0 ),
	CG_FRAME(    0,   21,    4,           0,   204, 0x90,   0x20,  ATK,  0x00,       0x1f,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,   22,    4,           0,   205, 0x90,   0x20,  ATK,  0x1c,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,   23,    4,           0,   206, 0x90,   0x20,  ATK,  0x14,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,   24,    4, END_OF_DATA,   207, 0x90,   0x20,  ATK,  0x10,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0c_super_35_loop ),
};

/* Char_Super 36: hwebthrow_pull (Ultimate Web Throw) */
const Sint32 pl0c_super_36[] = {
	CG_FRAME(    0,   27,    4,           0,   606, 0x80,   0x20,  ATK,  0x0c,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,   28,    4,           0,   607, 0x80,   0x20,  ATK,  0x08,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,   29,    4,           0,   608, 0x80,   0x20,  ATK,  0x04,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,   30,    4,           0,   609, 0x80,   0x20,  ATK,  0x1c,          0,    0,   0,     0, 0x002b,   0 ),
	CG_FRAME(    0,   31,    4,           0,   610, 0x80,   0x20,  ATK,  0x12,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    1,   31,    4,           0,   610, 0x80,   0x20,  ATK,  0x0c,       0x04,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   610, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,   10,           0,   611, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,    6,           0,   192, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,    6,           0,   191, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,    6,           0,   194, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  82 ),
};

const Sint32 pl0c_super_36_loop[] = {
	CG_FRAME(    0,    0,    6, END_OF_DATA,   195, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_NEXT( pl0c_super_36_loop ),
};

/* Char_Super 37 */
const Sint32 pl0c_super_37[] = {
	CG_FRAME(    0,    0,   11,           0,   196, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    8,           0,   197, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    4,           0,   198, 0x80,   0x52,    0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    2,           0,   199, 0x80,   0x52,    0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   10, END_OF_DATA,   159, 0x80,   0x52,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0c_super_37 ),
};

/* Char_Change 1: act_change_vcounter_init */
const Sint32 pl0c_change_01[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   230, 0xb0,   0x53,  ATK,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_NEXT( pl0c_change_01 ),
};

/* Char_Special 31: snapback_init (Snapback) */
const Sint32 pl0c_special_31[] = {
	CG_FRAME(    0,    0,    6,           0,   420, 0x80,   0x0c,  ATK,  0x00,          0,    0,   0,     0,      0,  22 ),
	CG_FRAME(    0,    0,    2,           0,   421, 0x80,   0x09,  ATK,  0x00,          0,    0,   0,     0,      0,  22 ),
	CG_FRAME(    0,    1,    1,           0,   421, 0x80,   0x09,  ATK,  0x00,          0,    0,   0,     0,      0,  22 ),
	CG_FRAME(    0,    2,    1,           0,   421, 0x80,   0x09,  ATK,  0x00,       0x05,    0,   0,     0,      0,  22 ),
	CG_FRAME(    0,    0,    2,           0,   423, 0x80,   0x03,  ATK,  0x00,          0,    0,   0,     0,      0,  23 ),
	CG_FRAME(    0,    0,    2,           0,   424, 0x80,   0x03,  ATK,  0x00,          0,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    0,    1,           0,   425, 0x80,   0x03,  ATE,  0x00,          0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,   13,           0,   426, 0x80,   0x03,  ATE,  0x00,          0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,    6,           0,   427, 0x80,   0x03,  ATE,  0x00,          0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,    6,           0,   428, 0x80,   0x0a,  ATE,  0x00,          0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,    7, END_OF_DATA,   221, 0x80,   0x52,  ATE,  0x00,          0,    0,   0,     0,      0,  26 ),
	CG_NEXT( pl0c_special_31 ),
};

/* Char_Effect 23: shl0c_02_init */
const Sint32 pl0c_effect_23[] = {
	CG_FRAME(    0,    0,    2,           0,  1104, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,  1105, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,  1106, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,  1107, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,  1108, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 189 ),
};

const Sint32 pl0c_effect_23_loop[] = {
	CG_FRAME(    0,    0,   26, END_OF_DATA,  1108, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0c_effect_23_loop ),
};

/* Char_Effect 24: shl0c_02_move */
const Sint32 pl0c_effect_24[] = {
	CG_FRAME(    0,    0,    3,           0,  1109, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,  1110, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,  1111, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,  1112, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl0c_effect_24_loop[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,  1113, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0c_effect_24_loop ),
};

/* Char_Effect 16 */
const Sint32 pl0c_effect_16[] = {
	CG_FRAME(    0,    0,    1,           0, 32838, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32839, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32840, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32841, 0x00,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32842, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32843, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32844, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32845, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32846, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32847, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32848, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32849, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32850, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32851, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32852, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32853, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32854, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32855, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32856, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32857, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32858, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32859, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32860, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32861, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32830, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32831, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32832, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32833, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32834, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32835, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32836, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA, 32837, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0c_effect_16 ),
};

/* Char_Effect 17 */
const Sint32 pl0c_effect_17[] = {
	CG_FRAME(    0,    0,    1,           0, 32870, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32871, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32872, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32873, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32874, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32875, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32876, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32877, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32878, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32879, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32880, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32881, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32882, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32883, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32884, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32885, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32886, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32887, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32888, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32889, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32890, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32891, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32892, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32893, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32862, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32863, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32864, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32865, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32866, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32867, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32868, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA, 32869, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0c_effect_17 ),
};

/* Char_Effect 18 */
const Sint32 pl0c_effect_18[] = {
	CG_FRAME(    0,    0,    1,           0, 32902, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32903, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32904, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32905, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32906, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32907, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32908, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32909, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32910, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32911, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32912, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32913, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32914, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32915, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32916, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32917, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32918, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32919, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32920, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32921, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32922, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32923, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32924, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32925, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32894, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32895, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32896, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32897, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32898, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32899, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32900, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA, 32901, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0c_effect_18 ),
};

/* Char_Effect 19 */
const Sint32 pl0c_effect_19[] = {
	CG_FRAME(    0,    0,   16, END_OF_DATA, 32926, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0c_effect_19 ),
};

/* Char_Effect 20: pls0c_06_char_set */
const Sint32 pl0c_effect_20[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,  1071, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0c_effect_20 ),
};

/* Char_Effect 39 */
const Sint32 pl0c_effect_39[] = {
	CG_FRAME(    0,    0,    1,           0,   231, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   232, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   233, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   234, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   235, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   236, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   237, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   238, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   239, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   240, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0c_effect_39 ),
};

/* Char_Effect 40 */
const Sint32 pl0c_effect_40[] = {
	CG_FRAME(    0,    0,    2,           0,   246, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   247, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   248, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   249, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   250, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   251, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   252, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   253, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   254, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   255, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0c_effect_40 ),
};

/* Char_Effect 41 */
const Sint32 pl0c_effect_41[] = {
	CG_FRAME(    0,    0,    2,           0,   259, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   260, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   261, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   262, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   269, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   263, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   264, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   265, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   266, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   267, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   268, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0c_effect_41 ),
};

/* Char_Effect 42 */
const Sint32 pl0c_effect_42[] = {
	CG_FRAME(    0,    0,    2,           0,   349, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   350, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   351, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   352, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   353, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   354, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   235, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   236, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   237, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   238, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   239, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   240, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0c_effect_42 ),
};

/* Char_Effect 43 */
const Sint32 pl0c_effect_43[] = {
	CG_FRAME(    0,    0,    2,           0,   359, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   360, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   361, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   362, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   363, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   364, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   366, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   250, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   251, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   252, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   253, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   254, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   255, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0c_effect_43 ),
};

/* Char_Effect 44 */
const Sint32 pl0c_effect_44[] = {
	CG_FRAME(    0,    0,    2,           0,   372, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   373, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   374, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   375, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   376, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   377, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   378, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   380, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   263, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   264, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   265, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   266, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   267, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   268, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0c_effect_44 ),
};

/* Char_Effect 1: shl0c_00_init (1 + no) */
const Sint32 pl0c_effect_01[] = {
	CG_FRAME(    0,    0,    3,           0,   536, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 114 ),
};

const Sint32 pl0c_effect_01_loop[] = {
	CG_FRAME(    0,    0,    3,           0,   537, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 114 ),
	CG_FRAME(    0,    0,    3,           0,   538, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 114 ),
	CG_FRAME(    0,    0,    3,           0,   539, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 114 ),
	CG_FRAME(    0,    0,    3,           0,   540, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 114 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   536, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 114 ),
	CG_NEXT( pl0c_effect_01_loop ),
};

/* Char_Effect 3 */
const Sint32 pl0c_effect_03[] = {
	CG_FRAME(    0,    0,    3,           0,   589, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 115 ),
};

const Sint32 pl0c_effect_03_loop[] = {
	CG_FRAME(    0,    0,    3,           0,   590, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 115 ),
	CG_FRAME(    0,    0,    3,           0,   591, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 115 ),
	CG_FRAME(    0,    0,    3,           0,   592, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 115 ),
	CG_FRAME(    0,    0,    3,           0,   593, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 115 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   589, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 115 ),
	CG_NEXT( pl0c_effect_03_loop ),
};

/* Char_Effect 2 */
const Sint32 pl0c_effect_02[] = {
	CG_FRAME(    0,    0,    3,           0,   541, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   542, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   543, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   544, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   545, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0c_effect_02 ),
};

/* Char_Effect 4 */
const Sint32 pl0c_effect_04[] = {
	CG_FRAME(    0,    0,    3,           0,   594, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   595, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   596, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   597, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   598, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0c_effect_04 ),
};

/* Char_Effect 5: shl0c_00_fly */
const Sint32 pl0c_effect_05[] = {
	CG_FRAME(    0,    0,    3,           0,   599, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   600, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   601, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   602, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   603, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0c_effect_05 ),
};

/* Char_Effect 6: shl0c_00_fly (6 + no) */
const Sint32 pl0c_effect_06[] = {
	CG_FRAME(    0,    0,    4,           0, 32768, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    4,           0, 32769, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32770, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32771, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32772, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32773, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl0c_effect_06_loop[] = {
	CG_FRAME(    0,    2, -128,           0, 32774, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32769, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32770, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32771, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32772, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32773, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32774, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0c_effect_06_loop ),
};

/* Char_Effect 7 */
const Sint32 pl0c_effect_07[] = {
	CG_FRAME(    0,    0,    4,           0, 32775, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    4,           0, 32776, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32777, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32778, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32779, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32780, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl0c_effect_07_loop[] = {
	CG_FRAME(    0,    2, -128,           0, 32781, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32776, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32777, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32778, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32779, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32780, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32781, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0c_effect_07_loop ),
};

/* Char_Effect 8 */
const Sint32 pl0c_effect_08[] = {
	CG_FRAME(    0,    0,    4,           0, 32806, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32807, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32808, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32809, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32810, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32811, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0c_effect_08 ),
};

/* Char_Effect 9 */
const Sint32 pl0c_effect_09[] = {
	CG_FRAME(    0,    0,    4,           0, 32812, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32813, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32814, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32815, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32816, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32817, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0c_effect_09 ),
};

/* Char_Effect 10 */
const Sint32 pl0c_effect_10[] = {
	CG_FRAME(    0,    0,    4,           0, 32818, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32819, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32820, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32821, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32822, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32823, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0c_effect_10 ),
};

/* Char_Effect 11 */
const Sint32 pl0c_effect_11[] = {
	CG_FRAME(    0,    0,    4,           0, 32824, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32825, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32826, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32827, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32828, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32829, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0c_effect_11 ),
};

/* Char_Effect 12 */
const Sint32 pl0c_effect_12[] = {
	CG_FRAME(    0,    0,    4,           0, 32782, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32783, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32784, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32785, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32786, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32787, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0c_effect_12 ),
};

/* Char_Effect 13 */
const Sint32 pl0c_effect_13[] = {
	CG_FRAME(    0,    0,    4,           0, 32788, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32789, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32790, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32791, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32792, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32793, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0c_effect_13 ),
};

/* Char_Effect 14 */
const Sint32 pl0c_effect_14[] = {
	CG_FRAME(    0,    0,    4,           0, 32794, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32795, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32796, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32797, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32798, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32799, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0c_effect_14 ),
};

/* Char_Effect 15 */
const Sint32 pl0c_effect_15[] = {
	CG_FRAME(    0,    0,    4,           0, 32800, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32801, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32802, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32803, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32804, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32805, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0c_effect_15 ),
};

/* Char_Effect 0: pls0c_03_a_init */
const Sint32 pl0c_effect_00[] = {
	CG_FRAME(    0,    0,    3,           0,   322, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   323, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   324, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   325, 0xb0,   0x00,    0,  0x00,       0x0a,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   326, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   327, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   328, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl0c_effect_00_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   329, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0c_effect_00_loop ),
};

/* Char_Effect 36: shl0c_01_init (36 + atck_level) */
const Sint32 pl0c_effect_36[] = {
	CG_FRAME(    0,    0,    1,           0,   793, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 126 ),
	CG_FRAME(    0,    1,    1,           0,   807, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 127 ),
	CG_FRAME(    0,    2,    1,           0,   814, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 128 ),
	CG_FRAME(    0,    3,    1,           0,   821, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 129 ),
	CG_FRAME(    0,    4,    2,           0,   828, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 130 ),
	CG_FRAME(    0,    5,    2,           0,   835, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 131 ),
	CG_FRAME(    0,    6,    2,           0,   842, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 132 ),
	CG_FRAME(    0,    7,    2,           0,   849, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 133 ),
	CG_FRAME(    0,    8,    2,           0,   856, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 134 ),
};

const Sint32 pl0c_effect_36_loop[] = {
	CG_FRAME(    0,    9,    2, END_OF_DATA,   863, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 135 ),
	CG_NEXT( pl0c_effect_36_loop ),
};

/* Char_Effect 45 */
const Sint32 pl0c_effect_45[] = {
	CG_FRAME(    0,    0,    4,           0,   798, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 126 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   799, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 126 ),
	CG_NEXT( pl0c_effect_45 ),
};

/* Char_Effect 46 */
const Sint32 pl0c_effect_46[] = {
	CG_FRAME(    0,    1,    4,           0,   812, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 127 ),
	CG_FRAME(    0,    1,    4, END_OF_DATA,   813, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 127 ),
	CG_NEXT( pl0c_effect_46 ),
};

/* Char_Effect 47 */
const Sint32 pl0c_effect_47[] = {
	CG_FRAME(    0,    2,    4,           0,   819, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 128 ),
	CG_FRAME(    0,    2,    4, END_OF_DATA,   820, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 128 ),
	CG_NEXT( pl0c_effect_47 ),
};

/* Char_Effect 48 */
const Sint32 pl0c_effect_48[] = {
	CG_FRAME(    0,    3,    4,           0,   826, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 129 ),
	CG_FRAME(    0,    3,    4, END_OF_DATA,   827, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 129 ),
	CG_NEXT( pl0c_effect_48 ),
};

/* Char_Effect 49 */
const Sint32 pl0c_effect_49[] = {
	CG_FRAME(    0,    4,    4,           0,   833, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 130 ),
	CG_FRAME(    0,    4,    4, END_OF_DATA,   834, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 130 ),
	CG_NEXT( pl0c_effect_49 ),
};

/* Char_Effect 50 */
const Sint32 pl0c_effect_50[] = {
	CG_FRAME(    0,    5,    4,           0,   840, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 131 ),
	CG_FRAME(    0,    5,    4, END_OF_DATA,   841, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 131 ),
	CG_NEXT( pl0c_effect_50 ),
};

/* Char_Effect 51 */
const Sint32 pl0c_effect_51[] = {
	CG_FRAME(    0,    6,    4,           0,   847, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 132 ),
	CG_FRAME(    0,    6,    4, END_OF_DATA,   848, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 132 ),
	CG_NEXT( pl0c_effect_51 ),
};

/* Char_Effect 52 */
const Sint32 pl0c_effect_52[] = {
	CG_FRAME(    0,    7,    4,           0,   854, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 133 ),
	CG_FRAME(    0,    7,    4, END_OF_DATA,   855, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 133 ),
	CG_NEXT( pl0c_effect_52 ),
};

/* Char_Effect 53 */
const Sint32 pl0c_effect_53[] = {
	CG_FRAME(    0,    8,    4,           0,   861, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 134 ),
	CG_FRAME(    0,    8,    4, END_OF_DATA,   862, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 134 ),
	CG_NEXT( pl0c_effect_53 ),
};

/* Char_Effect 54 */
const Sint32 pl0c_effect_54[] = {
	CG_FRAME(    0,    9,    4,           0,   868, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 135 ),
	CG_FRAME(    0,    9,    4, END_OF_DATA,   869, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 135 ),
	CG_NEXT( pl0c_effect_54 ),
};

/* Char_Effect 65 */
const Sint32 pl0c_effect_65[] = {
	CG_FRAME(    0,    0,    4,           0,   794, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   795, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   796, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl0c_effect_65_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   797, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0c_effect_65_loop ),
};

/* Char_Effect 66 */
const Sint32 pl0c_effect_66[] = {
	CG_FRAME(    0,    0,    4,           0,   808, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   809, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   810, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl0c_effect_66_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   811, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0c_effect_66_loop ),
};

/* Char_Effect 67 */
const Sint32 pl0c_effect_67[] = {
	CG_FRAME(    0,    0,    4,           0,   815, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   816, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   817, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl0c_effect_67_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   818, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0c_effect_67_loop ),
};

/* Char_Effect 68 */
const Sint32 pl0c_effect_68[] = {
	CG_FRAME(    0,    0,    4,           0,   822, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   823, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   824, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl0c_effect_68_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   825, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0c_effect_68_loop ),
};

/* Char_Effect 69 */
const Sint32 pl0c_effect_69[] = {
	CG_FRAME(    0,    0,    4,           0,   829, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   830, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   831, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl0c_effect_69_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   832, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0c_effect_69_loop ),
};

/* Char_Effect 70 */
const Sint32 pl0c_effect_70[] = {
	CG_FRAME(    0,    0,    4,           0,   836, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   837, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   838, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl0c_effect_70_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   839, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0c_effect_70_loop ),
};

/* Char_Effect 71 */
const Sint32 pl0c_effect_71[] = {
	CG_FRAME(    0,    0,    4,           0,   843, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   844, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   845, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl0c_effect_71_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   846, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0c_effect_71_loop ),
};

/* Char_Effect 72 */
const Sint32 pl0c_effect_72[] = {
	CG_FRAME(    0,    0,    4,           0,   850, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   851, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   852, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl0c_effect_72_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   853, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0c_effect_72_loop ),
};

/* Char_Effect 73 */
const Sint32 pl0c_effect_73[] = {
	CG_FRAME(    0,    0,    4,           0,   857, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   858, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   859, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl0c_effect_73_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   860, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0c_effect_73_loop ),
};

/* Char_Effect 74 */
const Sint32 pl0c_effect_74[] = {
	CG_FRAME(    0,    0,    4,           0,   864, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   865, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   866, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl0c_effect_74_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   867, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0c_effect_74_loop ),
};

/* Char_Effect 37
   Char_Effect 38: pls0c_01_move (38 + no) */
const Sint32 pl0c_effect_37[] = {
	CG_FRAME(    0,    0,    1,           0,   884, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 136 ),
	CG_FRAME(    0,    1,    1,           0,   898, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 137 ),
	CG_FRAME(    0,    2,    1,           0,   905, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 138 ),
	CG_FRAME(    0,    3,    1,           0,   912, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 139 ),
	CG_FRAME(    0,    4,    2,           0,   919, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 140 ),
	CG_FRAME(    0,    5,    2,           0,   926, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 141 ),
	CG_FRAME(    0,    6,    2,           0,   933, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 142 ),
	CG_FRAME(    0,    7,    2,           0,   940, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 143 ),
	CG_FRAME(    0,    8,    2,           0,   947, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 144 ),
};

const Sint32 pl0c_effect_37_loop[] = {
	CG_FRAME(    0,    9,    2, END_OF_DATA,   954, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 145 ),
	CG_NEXT( pl0c_effect_37_loop ),
};

/* Char_Effect 55 */
const Sint32 pl0c_effect_55[] = {
	CG_FRAME(    0,    0,    4,           0,   889, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 136 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   890, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 136 ),
	CG_NEXT( pl0c_effect_55 ),
};

/* Char_Effect 56 */
const Sint32 pl0c_effect_56[] = {
	CG_FRAME(    0,    1,    4,           0,   903, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 137 ),
	CG_FRAME(    0,    1,    4, END_OF_DATA,   904, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 137 ),
	CG_NEXT( pl0c_effect_56 ),
};

/* Char_Effect 57 */
const Sint32 pl0c_effect_57[] = {
	CG_FRAME(    0,    2,    4,           0,   910, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 138 ),
	CG_FRAME(    0,    2,    4, END_OF_DATA,   911, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 138 ),
	CG_NEXT( pl0c_effect_57 ),
};

/* Char_Effect 58 */
const Sint32 pl0c_effect_58[] = {
	CG_FRAME(    0,    3,    4,           0,   917, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 139 ),
	CG_FRAME(    0,    3,    4, END_OF_DATA,   918, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 139 ),
	CG_NEXT( pl0c_effect_58 ),
};

/* Char_Effect 59 */
const Sint32 pl0c_effect_59[] = {
	CG_FRAME(    0,    4,    4,           0,   924, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 140 ),
	CG_FRAME(    0,    4,    4, END_OF_DATA,   925, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 140 ),
	CG_NEXT( pl0c_effect_59 ),
};

/* Char_Effect 60 */
const Sint32 pl0c_effect_60[] = {
	CG_FRAME(    0,    5,    4,           0,   931, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 141 ),
	CG_FRAME(    0,    5,    4, END_OF_DATA,   932, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 141 ),
	CG_NEXT( pl0c_effect_60 ),
};

/* Char_Effect 61 */
const Sint32 pl0c_effect_61[] = {
	CG_FRAME(    0,    6,    4,           0,   938, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 142 ),
	CG_FRAME(    0,    6,    4, END_OF_DATA,   939, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 142 ),
	CG_NEXT( pl0c_effect_61 ),
};

/* Char_Effect 62 */
const Sint32 pl0c_effect_62[] = {
	CG_FRAME(    0,    7,    4,           0,   945, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 143 ),
	CG_FRAME(    0,    7,    4, END_OF_DATA,   946, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 143 ),
	CG_NEXT( pl0c_effect_62 ),
};

/* Char_Effect 63 */
const Sint32 pl0c_effect_63[] = {
	CG_FRAME(    0,    8,    4,           0,   952, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 144 ),
	CG_FRAME(    0,    8,    4, END_OF_DATA,   953, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 144 ),
	CG_NEXT( pl0c_effect_63 ),
};

/* Char_Effect 64 */
const Sint32 pl0c_effect_64[] = {
	CG_FRAME(    0,    9,    4,           0,   959, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 145 ),
	CG_FRAME(    0,    9,    4, END_OF_DATA,   960, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 145 ),
	CG_NEXT( pl0c_effect_64 ),
};

/* Char_Effect 75 */
const Sint32 pl0c_effect_75[] = {
	CG_FRAME(    0,    0,    4,           0,   885, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   886, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   887, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl0c_effect_75_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   888, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0c_effect_75_loop ),
};

/* Char_Effect 76 */
const Sint32 pl0c_effect_76[] = {
	CG_FRAME(    0,    0,    4,           0,   899, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   900, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   901, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl0c_effect_76_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   902, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0c_effect_76_loop ),
};

/* Char_Effect 77 */
const Sint32 pl0c_effect_77[] = {
	CG_FRAME(    0,    0,    4,           0,   906, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   907, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   908, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl0c_effect_77_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   909, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0c_effect_77_loop ),
};

/* Char_Effect 78 */
const Sint32 pl0c_effect_78[] = {
	CG_FRAME(    0,    0,    4,           0,   913, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   914, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   915, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl0c_effect_78_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   916, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0c_effect_78_loop ),
};

/* Char_Effect 79 */
const Sint32 pl0c_effect_79[] = {
	CG_FRAME(    0,    0,    4,           0,   920, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   921, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   922, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl0c_effect_79_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   923, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0c_effect_79_loop ),
};

/* Char_Effect 80 */
const Sint32 pl0c_effect_80[] = {
	CG_FRAME(    0,    0,    4,           0,   927, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   928, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   929, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl0c_effect_80_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   930, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0c_effect_80_loop ),
};

/* Char_Effect 81 */
const Sint32 pl0c_effect_81[] = {
	CG_FRAME(    0,    0,    4,           0,   934, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   935, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   936, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl0c_effect_81_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   937, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0c_effect_81_loop ),
};

/* Char_Effect 82 */
const Sint32 pl0c_effect_82[] = {
	CG_FRAME(    0,    0,    4,           0,   941, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   942, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   943, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl0c_effect_82_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   944, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0c_effect_82_loop ),
};

/* Char_Effect 83 */
const Sint32 pl0c_effect_83[] = {
	CG_FRAME(    0,    0,    4,           0,   948, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   949, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   950, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl0c_effect_83_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   951, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0c_effect_83_loop ),
};

/* Char_Effect 84 */
const Sint32 pl0c_effect_84[] = {
	CG_FRAME(    0,    0,    4,           0,   955, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   956, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   957, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl0c_effect_84_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   958, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0c_effect_84_loop ),
};

/* Char_Effect 21: pls0c_03_d_init */
const Sint32 pl0c_effect_21[] = {
	CG_FRAME(    0,    0,    3,           0,   800, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   801, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   802, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   803, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   804, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   805, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   806, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0c_effect_21 ),
};

/* Char_Catch 5 */
const Sint32 pl0c_catch_05[] = {
	CG_FRAME(    0,    0,    4,           0,  1066, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0010,   0 ),
};

/* Char_Catch 6 */
const Sint32 pl0c_catch_06[] = {
	CG_FRAME(    0,    4,    4,           0,  1067, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0011,   0 ),
};

/* Char_Catch 7 */
const Sint32 pl0c_catch_07[] = {
	CG_FRAME(    0,    8,    4,           0,  1068, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0012,   0 ),
};

/* Char_Catch 8 */
const Sint32 pl0c_catch_08[] = {
	CG_FRAME(    0,   12,    4,           0,  1069, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0013,   0 ),
};

/* Char_Catch 9 */
const Sint32 pl0c_catch_09[] = {
	CG_FRAME(    0,   16,    4, END_OF_DATA,  1070, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0014,   0 ),
	CG_NEXT( pl0c_catch_05 ),
};

/* Char_Catch 10: pls0c_02_char_set, pls0c_05_char_set
   Char_Effect 25 */
const Sint32 pl0c_catch_10[] = {
	CG_FRAME(    0,    0,    4,           0,  1068, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0012,   0 ),
	CG_FRAME(    0,    0,    4,           0,  1069, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0023,   0 ),
	CG_FRAME(    0,    0,    4,           0,  1070, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0024,   0 ),
	CG_FRAME(    0,    0,    4,           0,  1078, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0026,   0 ),
	CG_FRAME(    0,    0,    4,           0,  1077, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x002d,   0 ),
	CG_FRAME(    0,    0,    4,           0,  1076, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x002f,   0 ),
	CG_FRAME(    0,    0,    4,           0,  1066, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0010,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,  1067, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0011,   0 ),
	CG_NEXT( pl0c_catch_10 ),
};

/* Char_Effect 22: pls0c_03_b_init */
const Sint32 pl0c_effect_22[] = {
	CG_FRAME(    0,    0,    1,           0,   304, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    3,           0,   305, 0x80,   0x00,    0,  0x00,       0x22,    0,   0,     0,      0,   0 ),
};

const Sint32 pl0c_effect_22_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   304, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0c_effect_22_loop ),
};

/* Char_Effect 26: pls0c_03_c_init (26 + no) */
const Sint32 pl0c_effect_26[] = {
	CG_FRAME(    0,    0,   24, END_OF_DATA,   701, 0xb0,   0x00,    0,  0x00,       0x0b,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0c_effect_26 ),
};

/* Char_Effect 27 */
const Sint32 pl0c_effect_27[] = {
	CG_FRAME(    0,    0,   24, END_OF_DATA,   702, 0xb0,   0x00,    0,  0x00,       0x0d,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0c_effect_27 ),
};

/* Char_Effect 28 */
const Sint32 pl0c_effect_28[] = {
	CG_FRAME(    0,    0,   24, END_OF_DATA,   703, 0xb0,   0x00,    0,  0x00,       0x0c,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0c_effect_28 ),
};

/* Char_Effect 29 */
const Sint32 pl0c_effect_29[] = {
	CG_FRAME(    0,    0,   24, END_OF_DATA,   704, 0xb0,   0x00,    0,  0x00,       0x0b,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0c_effect_29 ),
};

/* Char_Effect 30 */
const Sint32 pl0c_effect_30[] = {
	CG_FRAME(    0,    0,   24, END_OF_DATA,   705, 0xb0,   0x00,    0,  0x00,       0x0d,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0c_effect_30 ),
};

/* Char_Effect 31 */
const Sint32 pl0c_effect_31[] = {
	CG_FRAME(    0,    0,   24, END_OF_DATA,   706, 0xb0,   0x00,    0,  0x00,       0x0c,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0c_effect_31 ),
};

/* Char_Effect 32 */
const Sint32 pl0c_effect_32[] = {
	CG_FRAME(    0,    0,   24, END_OF_DATA,   402, 0xb0,   0x00,    0,  0x00,       0x0b,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0c_effect_32 ),
};

/* Char_Effect 33 */
const Sint32 pl0c_effect_33[] = {
	CG_FRAME(    0,    0,   24, END_OF_DATA,   403, 0xb0,   0x00,    0,  0x00,       0x0c,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0c_effect_33 ),
};

/* Char_Effect 34 */
const Sint32 pl0c_effect_34[] = {
	CG_FRAME(    0,    0,   24, END_OF_DATA,   405, 0xb0,   0x00,    0,  0x00,       0x0b,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0c_effect_34 ),
};

/* Char_Effect 35 */
const Sint32 pl0c_effect_35[] = {
	CG_FRAME(    0,    0,   24, END_OF_DATA,   406, 0xb0,   0x00,    0,  0x00,       0x0c,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0c_effect_35 ),
};
