/* Sonson (character 14): the animations. PLAYER_WORK's char_adrs points here.

   spr_prm_14      a sprite's sizes and turn (SPR_PRM): a frame's zoom number picks a row, 0 for none.
   char_tbl_14     the groups: pl_char_set( pl_w, group, no ) takes the group's table from here and
                   starts its animation no. The character's module (s_pl14.c) says which group
                   and number each move uses.
   pl14_GROUP_tbl  a group's animations. GROUP is the group's name in player.h (stand for
                   Char_Stand), or grpNN for a group with no name there.
   pl14_GROUP_NN   an animation, named by the first group and number that has it: its frames in
                   order. The comment before it lists its places in the groups and, for each,
                   the routines that start it by that number, with their special move;
                   (2 + atck_level) is a routine that counts on from 2 by the attack's level.
                   A frame with END_OF_DATA is the last, and the word after it is the frame the
                   animation goes on from (CG_NEXT; pl14_GROUP_NN_loop where that is no
                   animation's first frame). An effect's animation has short frames (EF_FRAME)
                   and may end with no word.
   pl14_unused_NN  frames that no table or animation reaches.

   A player's frame (CG_FRAME, the 0x14 bytes of CHAR_DATA), with the names Capcom's tables used:
     prm, type  a number and a code for the character's module (a speed, an attack, a voice)
     ctr        the game frames it is shown for
     end        END_OF_DATA on the last frame
     spr        the sprite, -1 for none
     prio       added to the draw priority (a signed byte)
     shadow     the shadow's offset (the low five bits) and size, 0xff none
     atck       ATK while an attack is on its way or can hit, ATE in its frames after that;
                CG_ATCK_NG lets nothing cancel it
     flag1      a number for the module: most often the attack, a row of hit_dt_14
     se         a voice (1 to 63), or with CSE a sound effect; 0 none
     zoom       the row of spr_prm_14, 0 none
     unk        a byte the program is not seen to read
     flag2      a second code for the character's effects
     rival      where a held opponent is put (the low byte), and a code for the module (the high)
     box        the frame's row of frame_box_14 */
#include "player.h"
#include "char_tbl.h"

extern const Sint32 pl14_change_00[], pl14_change_00_loop[], pl14_change_01[], pl14_change_02[],
	pl14_change_02_loop[], pl14_change_03[], pl14_change_03_loop[], pl14_change_04[],
	pl14_stand_00[], pl14_stand_02[], pl14_stand_03[], pl14_jump_00[], pl14_jump_01[],
	pl14_jump_02[], pl14_jump_03[], pl14_jump_04[], pl14_jump_05[], pl14_jump_06[],
	pl14_jump_06_loop[], pl14_jump_08[], pl14_jump_08_loop[], pl14_jump_09[], pl14_dash_00[],
	pl14_dash_02[], pl14_dash_01[], pl14_dash_03[], pl14_crouch_00[], pl14_crouch_01[],
	pl14_crouch_02[], pl14_turn_00[], pl14_turn_01[], pl14_guard_00[], pl14_guard_01[],
	pl14_guard_02[], pl14_guard_03[], pl14_guard_04[], pl14_guard_05[], pl14_guard_06[],
	pl14_guard_07[], pl14_air_guard_00[], pl14_air_guard_01[], pl14_stand_p_00[], pl14_stand_p_01[],
	pl14_stand_p_02[], pl14_stand_p_03[], pl14_stand_k_00[], pl14_stand_k_01[], pl14_stand_k_02[],
	pl14_crouch_p_00[], pl14_crouch_p_01[], pl14_crouch_p_02[], pl14_crouch_k_00[],
	pl14_crouch_k_01[], pl14_crouch_k_02[], pl14_jump_p_00[], pl14_jump_p_01[], pl14_jump_p_02[],
	pl14_jump_k_00[], pl14_jump_k_01[], pl14_jump_k_02[], pl14_get_up_00[], pl14_get_up_01[],
	pl14_get_up_02[], pl14_get_up_03[], pl14_damage_00[], pl14_damage_01[], pl14_damage_02[],
	pl14_damage_03[], pl14_damage_04[], pl14_damage_05[], pl14_damage_06[], pl14_damage_07[],
	pl14_damage_08[], pl14_damage_09[], pl14_damage_09_loop[], pl14_damage_10[], pl14_damage_11[],
	pl14_damage_12[], pl14_damage_13[], pl14_damage_13_loop[], pl14_damage_16[], pl14_damage_17[],
	pl14_damage_18[], pl14_damage_19[], pl14_damage_20[], pl14_damage_21[], pl14_damage_22[],
	pl14_damage_23[], pl14_damage_24[], pl14_damage_25[], pl14_damage_26[], pl14_damage_31[],
	pl14_damage_27[], pl14_damage_27_loop[], pl14_damage_28[], pl14_damage_29[], pl14_catch_00[],
	pl14_catch_00_loop[], pl14_catch_02[], pl14_catch_01[], pl14_catch_05[], pl14_catch_03[],
	pl14_catch_03_loop[], pl14_catch_04[], pl14_catch_04_loop[], pl14_caught_00[], pl14_caught_01[],
	pl14_caught_02[], pl14_caught_03[], pl14_caught_04[], pl14_caught_05[], pl14_caught_06[],
	pl14_caught_07[], pl14_caught_08[], pl14_caught_09[], pl14_caught_10[], pl14_caught_11[],
	pl14_caught_12[], pl14_caught_13[], pl14_caught_14[], pl14_caught_15[], pl14_caught_16[],
	pl14_caught_17[], pl14_caught_18[], pl14_caught_19[], pl14_caught_20[], pl14_caught_21[],
	pl14_dizzy_00[], pl14_win_00[], pl14_win_00_loop[], pl14_win_01[], pl14_win_01_loop[],
	pl14_win_03[], pl14_win_03_loop[], pl14_win_05[], pl14_win_05_loop[], pl14_win_07[],
	pl14_win_07_loop[], pl14_win_09[], pl14_appear_00[], pl14_special_00[], pl14_special_01[],
	pl14_special_02[], pl14_special_02_loop[], pl14_special_03[], pl14_special_04[],
	pl14_special_04_loop[], pl14_special_05[], pl14_special_06[], pl14_special_07[],
	pl14_special_08[], pl14_special_09[], pl14_special_09_loop[], pl14_special_10[],
	pl14_special_10_loop[], pl14_special_11[], pl14_special_11_loop[], pl14_special_26[],
	pl14_special_26_loop[], pl14_special_27[], pl14_special_27_loop[], pl14_special_12[],
	pl14_special_12_loop[], pl14_special_13[], pl14_special_14[], pl14_special_30[],
	pl14_special_30_loop[], pl14_special_16[], pl14_special_17[], pl14_special_17_loop[],
	pl14_dash_04[], pl14_dash_04_loop[], pl14_dash_06[], pl14_dash_06_loop[], pl14_extra_00[],
	pl14_extra_01[], pl14_extra_02[], pl14_extra_03[], pl14_super_00[], pl14_super_01[],
	pl14_super_65[], pl14_super_65_loop[], pl14_super_64[], pl14_super_02[], pl14_super_03[],
	pl14_super_04[], pl14_super_05[], pl14_super_06[], pl14_super_06_loop[], pl14_super_07[],
	pl14_super_07_loop[], pl14_super_08[], pl14_super_09[], pl14_super_10[], pl14_super_11[],
	pl14_super_12[], pl14_super_13[], pl14_super_13_loop[], pl14_super_14[], pl14_super_15[],
	pl14_super_16[], pl14_super_18[], pl14_super_17[], pl14_super_19[], pl14_super_20[],
	pl14_super_22[], pl14_super_21[], pl14_super_23[], pl14_super_24[], pl14_super_26[],
	pl14_super_25[], pl14_super_27[], pl14_super_28[], pl14_super_29[], pl14_super_30[],
	pl14_super_31[], pl14_super_32[], pl14_super_52[], pl14_super_33[], pl14_super_34[],
	pl14_super_38[], pl14_super_40[], pl14_super_43[], pl14_super_45[], pl14_super_46[],
	pl14_super_48[], pl14_super_49[], pl14_super_51[], pl14_super_53[], pl14_super_53_loop[],
	pl14_super_55[], pl14_super_56[], pl14_super_57[], pl14_super_58[], pl14_super_59[],
	pl14_super_60[], pl14_super_61[], pl14_super_62[], pl14_super_66[], pl14_super_67[],
	pl14_special_18[], pl14_special_18_loop[], pl14_special_19[], pl14_special_20[],
	pl14_special_20_loop[], pl14_special_21[], pl14_special_21_loop[], pl14_special_23[],
	pl14_special_25[], pl14_special_25_loop[], pl14_special_22[], pl14_special_28[],
	pl14_special_29[], pl14_grp27_00[], pl14_grp27_01[], pl14_grp27_02[], pl14_grp27_03[],
	pl14_grp27_04[], pl14_grp27_05[], pl14_grp27_06[], pl14_grp27_07[], pl14_grp27_08[],
	pl14_grp27_09[], pl14_grp27_09_loop[], pl14_grp27_10[], pl14_grp27_11[], pl14_grp27_11_loop[],
	pl14_grp27_13[], pl14_grp27_13_loop[], pl14_grp27_14[], pl14_grp27_14_loop[], pl14_grp27_15[],
	pl14_grp27_15_loop[], pl14_grp27_16[], pl14_grp27_17[], pl14_grp27_19[], pl14_grp27_20[],
	pl14_grp27_20_loop[], pl14_grp27_21[], pl14_grp27_22[], pl14_grp27_23[], pl14_grp27_24[],
	pl14_grp27_25[], pl14_grp27_26[], pl14_grp27_32[], pl14_grp27_33[], pl14_grp27_34[],
	pl14_grp27_35[], pl14_grp27_36[], pl14_grp27_37[], pl14_grp27_38[], pl14_grp27_39[],
	pl14_grp27_40[], pl14_grp27_41[];
extern const Sint32 *const pl14_stand_tbl[], *const pl14_jump_tbl[], *const pl14_dash_tbl[],
	*const pl14_crouch_tbl[], *const pl14_turn_tbl[], *const pl14_guard_tbl[],
	*const pl14_air_guard_tbl[], *const pl14_stand_p_tbl[], *const pl14_stand_k_tbl[],
	*const pl14_crouch_p_tbl[], *const pl14_crouch_k_tbl[], *const pl14_jump_p_tbl[],
	*const pl14_jump_k_tbl[], *const pl14_damage_tbl[], *const pl14_dizzy_tbl[],
	*const pl14_catch_tbl[], *const pl14_caught_tbl[], *const pl14_get_up_tbl[],
	*const pl14_appear_tbl[], *const pl14_win_tbl[], *const pl14_extra_tbl[],
	*const pl14_special_tbl[], *const pl14_super_tbl[], *const pl14_effect_tbl[],
	*const pl14_change_tbl[], *const pl14_grp25_tbl[], *const pl14_grp27_tbl[];

/* the sprites' sizes and turns, by a frame's zoom number */
const SPR_PRM spr_prm_14[16] = {
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 1000, 1000, 0, 0, 0, 1, 0, 0 },
	{ 1000, 1000, -32768, 0, -56, 0, 0, 0 },
	{ 1000, 1000, -7168, 0, 0, 0, 0, 0 },
	{ 1000, 1000, -3072, 0, 0, 0, 0, 0 },
	{ 1000, 1000, 3072, 0, 0, 0, 0, 0 },
	{ 1000, 1000, 6144, 0, 0, 0, 0, 0 },
	{ 1000, 1000, -16384, 0, -64, 0, 0, 0 },
	{ 500, 500, 0, 0, 0, 0, 0, 0 },
	{ 1000, 1000, -10240, 0, -64, 0, 0, 0 },
	{ 1000, 1000, 0, 0, -16, 0, 0, 0 },
	{ 1000, 1000, -16384, 0, -16, 0, 0, 0 },
	{ 1000, 1000, 16384, 0, -16, 0, 0, 0 },
	{ 1000, 1000, -32768, 0, 0, 0, 0, 0 },
	{ 1000, 1000, 0, 0, 0, 1, 0, 0 },
	{ 1000, 1000, 0, 0, 0, 0, 0, 0 },
};

/* the groups */
const Sint32 *const *const char_tbl_14[] = {
	pl14_stand_tbl, pl14_jump_tbl, pl14_dash_tbl, pl14_crouch_tbl, pl14_turn_tbl, pl14_guard_tbl,
	pl14_air_guard_tbl, pl14_stand_p_tbl, pl14_stand_k_tbl, pl14_crouch_p_tbl, pl14_crouch_k_tbl, pl14_jump_p_tbl,
	pl14_jump_k_tbl, pl14_damage_tbl, pl14_dizzy_tbl, pl14_catch_tbl, pl14_caught_tbl, pl14_get_up_tbl,
	pl14_appear_tbl, pl14_win_tbl, pl14_extra_tbl, pl14_special_tbl, pl14_super_tbl, pl14_effect_tbl,
	pl14_change_tbl, pl14_grp25_tbl, pl14_grp25_tbl, pl14_grp27_tbl, 0,
};

const Sint32 *const pl14_stand_tbl[] = {
	pl14_stand_00, pl14_stand_00, pl14_stand_02, pl14_stand_03, pl14_stand_00, pl14_stand_00,
	pl14_stand_02, pl14_stand_03, 0,
};

const Sint32 *const pl14_jump_tbl[] = {
	pl14_jump_00, pl14_jump_01, pl14_jump_02, pl14_jump_03, pl14_jump_04, pl14_jump_05,
	pl14_jump_06, pl14_jump_06, pl14_jump_08, pl14_jump_09, pl14_jump_09, pl14_jump_09,
	0,
};

const Sint32 *const pl14_dash_tbl[] = {
	pl14_dash_00, pl14_dash_01, pl14_dash_02, pl14_dash_03, pl14_dash_04, pl14_dash_04,
	pl14_dash_06, 0,
};

const Sint32 *const pl14_crouch_tbl[] = {
	pl14_crouch_00, pl14_crouch_01, pl14_crouch_02, 0,
};

const Sint32 *const pl14_turn_tbl[] = {
	pl14_turn_00, pl14_turn_01, 0,
};

const Sint32 *const pl14_guard_tbl[] = {
	pl14_guard_00, pl14_guard_01, pl14_guard_02, pl14_guard_03, pl14_guard_04, pl14_guard_05,
	pl14_guard_06, pl14_guard_07, 0,
};

const Sint32 *const pl14_air_guard_tbl[] = {
	pl14_air_guard_00, pl14_air_guard_01, pl14_air_guard_01, 0,
};

const Sint32 *const pl14_stand_p_tbl[] = {
	pl14_stand_p_00, pl14_stand_p_01, pl14_stand_p_02, pl14_stand_p_03, 0,
};

const Sint32 *const pl14_stand_k_tbl[] = {
	pl14_stand_k_00, pl14_stand_k_01, pl14_stand_k_02, 0,
};

const Sint32 *const pl14_crouch_p_tbl[] = {
	pl14_crouch_p_00, pl14_crouch_p_01, pl14_crouch_p_02, 0,
};

const Sint32 *const pl14_crouch_k_tbl[] = {
	pl14_crouch_k_00, pl14_crouch_k_01, pl14_crouch_k_02, 0,
};

const Sint32 *const pl14_jump_p_tbl[] = {
	pl14_jump_p_00, pl14_jump_p_01, pl14_jump_p_02, pl14_jump_p_00, pl14_jump_p_01, pl14_jump_p_02,
	0,
};

const Sint32 *const pl14_jump_k_tbl[] = {
	pl14_jump_k_00, pl14_jump_k_01, pl14_jump_k_02, pl14_jump_k_00, pl14_jump_k_01, pl14_jump_k_02,
	0,
};

const Sint32 *const pl14_damage_tbl[] = {
	pl14_damage_00, pl14_damage_01, pl14_damage_02, pl14_damage_03, pl14_damage_04, pl14_damage_05,
	pl14_damage_06, pl14_damage_07, pl14_damage_08, pl14_damage_09, pl14_damage_10, pl14_damage_11,
	pl14_damage_12, pl14_damage_13, pl14_damage_13, pl14_damage_13, pl14_damage_16, pl14_damage_17,
	pl14_damage_18, pl14_damage_19, pl14_damage_20, pl14_damage_21, pl14_damage_22, pl14_damage_23,
	pl14_damage_24, pl14_damage_25, pl14_damage_26, pl14_damage_27, pl14_damage_28, pl14_damage_29,
	pl14_damage_27, pl14_damage_31, pl14_damage_27, pl14_damage_27, pl14_damage_13, 0,
};

const Sint32 *const pl14_dizzy_tbl[] = {
	pl14_dizzy_00, 0,
};

const Sint32 *const pl14_catch_tbl[] = {
	pl14_catch_00, pl14_catch_01, pl14_catch_02, pl14_catch_03, pl14_catch_04, pl14_catch_05,
	0,
};

const Sint32 *const pl14_caught_tbl[] = {
	pl14_caught_00, pl14_caught_01, pl14_caught_02, pl14_caught_03, pl14_caught_04, pl14_caught_05,
	pl14_caught_06, pl14_caught_07, pl14_caught_08, pl14_caught_09, pl14_caught_10, pl14_caught_11,
	pl14_caught_12, pl14_caught_13, pl14_caught_14, pl14_caught_15, pl14_caught_16, pl14_caught_17,
	pl14_caught_18, pl14_caught_19, pl14_caught_20, pl14_caught_21, 0,
};

const Sint32 *const pl14_get_up_tbl[] = {
	pl14_get_up_00, pl14_get_up_01, pl14_get_up_02, pl14_get_up_03, 0,
};

const Sint32 *const pl14_appear_tbl[] = {
	pl14_appear_00, 0,
};

const Sint32 *const pl14_win_tbl[] = {
	pl14_win_00, pl14_win_01, pl14_win_01, pl14_win_03, pl14_win_03, pl14_win_05,
	pl14_win_05, pl14_win_07, pl14_win_07, pl14_win_09, 0,
};

const Sint32 *const pl14_extra_tbl[] = {
	pl14_extra_00, pl14_extra_01, pl14_extra_02, pl14_extra_03, 0,
};

const Sint32 *const pl14_special_tbl[] = {
	pl14_special_00, pl14_special_01, pl14_special_02, pl14_special_03, pl14_special_04, pl14_special_05,
	pl14_special_06, pl14_special_07, pl14_special_08, pl14_special_09, pl14_special_10, pl14_special_11,
	pl14_special_12, pl14_special_13, pl14_special_14, pl14_special_14, pl14_special_16, pl14_special_17,
	pl14_special_18, pl14_special_19, pl14_special_20, pl14_special_21, pl14_special_22, pl14_special_23,
	pl14_special_23, pl14_special_25, pl14_special_26, pl14_special_27, pl14_special_28, pl14_special_29,
	pl14_special_30, 0,
};

const Sint32 *const pl14_super_tbl[] = {
	pl14_super_00, pl14_super_01, pl14_super_02, pl14_super_03, pl14_super_04, pl14_super_05,
	pl14_super_06, pl14_super_07, pl14_super_08, pl14_super_09, pl14_super_10, pl14_super_11,
	pl14_super_12, pl14_super_13, pl14_super_14, pl14_super_15, pl14_super_16, pl14_super_17,
	pl14_super_18, pl14_super_19, pl14_super_20, pl14_super_21, pl14_super_22, pl14_super_23,
	pl14_super_24, pl14_super_25, pl14_super_26, pl14_super_27, pl14_super_28, pl14_super_29,
	pl14_super_30, pl14_super_31, pl14_super_32, pl14_super_33, pl14_super_34, pl14_super_34,
	pl14_super_34, pl14_super_34, pl14_super_38, pl14_super_38, pl14_super_40, pl14_super_40,
	pl14_super_40, pl14_super_43, pl14_super_43, pl14_super_45, pl14_super_46, pl14_super_46,
	pl14_super_48, pl14_super_49, pl14_super_49, pl14_super_51, pl14_super_52, pl14_super_53,
	pl14_super_53, pl14_super_55, pl14_super_56, pl14_super_57, pl14_super_58, pl14_super_59,
	pl14_super_60, pl14_super_61, pl14_super_62, pl14_super_62, pl14_super_64, pl14_super_65,
	pl14_super_66, pl14_super_67, 0,
};

const Sint32 *const pl14_effect_tbl[] = {
	pl14_stand_00, 0,
};

const Sint32 *const pl14_change_tbl[] = {
	pl14_change_00, pl14_change_01, pl14_change_02, pl14_change_03, pl14_change_04, 0,
};

const Sint32 *const pl14_grp25_tbl[] = {
	pl14_stand_00, 0,
};

const Sint32 *const pl14_grp27_tbl[] = {
	pl14_grp27_00, pl14_grp27_01, pl14_grp27_02, pl14_grp27_03, pl14_grp27_04, pl14_grp27_05,
	pl14_grp27_06, pl14_grp27_07, pl14_grp27_08, pl14_grp27_09, pl14_grp27_10, pl14_grp27_11,
	pl14_grp27_11, pl14_grp27_13, pl14_grp27_14, pl14_grp27_15, pl14_grp27_16, pl14_grp27_17,
	pl14_grp27_17, pl14_grp27_19, pl14_grp27_20, pl14_grp27_21, pl14_grp27_22, pl14_grp27_23,
	pl14_grp27_24, pl14_grp27_25, pl14_grp27_26, pl14_grp27_26, pl14_grp27_26, pl14_grp27_26,
	pl14_grp27_26, pl14_grp27_26, pl14_grp27_32, pl14_grp27_33, pl14_grp27_34, pl14_grp27_35,
	pl14_grp27_36, pl14_grp27_37, pl14_grp27_38, pl14_grp27_39, pl14_grp27_40, pl14_grp27_41,
	0,
};

	/*         prm  type  ctr          end    spr  prio  shadow  atck  flag1          se  zoom   unk  flag2   rival  box */

/* Char_Change 0: act_change_tag_init */
const Sint32 pl14_change_00[] = {
	CG_FRAME(    0,    0,   3,           0,    87, 0x80,   0x00,    0,  0x00,       0x0d,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   160, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   161, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   162, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   163, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   164, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
};

const Sint32 pl14_change_00_loop[] = {
	CG_FRAME(    0,    0,   3,           0,   162, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   163, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2, END_OF_DATA,   164, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_NEXT( pl14_change_00_loop ),
};

/* Char_Change 1: act_change_vcounter_init */
const Sint32 pl14_change_01[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   159, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl14_change_01 ),
};

/* Char_Change 2: act_change_out_bound */
const Sint32 pl14_change_02[] = {
	CG_FRAME(    0,    0,   4,           0,    81, 0x80,   0x00,    0,  0x00,          0,    1,    0,     0,      0,   3 ),
};

const Sint32 pl14_change_02_loop[] = {
	CG_FRAME(    0,    1,  -1, END_OF_DATA,    83, 0x80,   0x00,    0,  0x00,          0,    1,    0,     0,      0,   3 ),
	CG_NEXT( pl14_change_02_loop ),
};

/* Char_Change 3: act_stand, act_change_call */
const Sint32 pl14_change_03[] = {
	CG_FRAME(    0,    1,   4,           0,   165, 0x80,   0x00,    0,  0x00,       0x04,    0,    0,     0,      0,   1 ),
};

const Sint32 pl14_change_03_loop[] = {
	CG_FRAME(    0,    1,   4,           0,   166, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   4, END_OF_DATA,   167, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl14_change_03_loop ),
};

/* Char_Change 4: act_change_assist_end */
const Sint32 pl14_change_04[] = {
	CG_FRAME(    0,    0,   4,           0,   225, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   226, 0x80,   0x00,    0,  0x00,       0x0c,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   227, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   228, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   229, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   224, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   225, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   226, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   227, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   228, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   229, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   224, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl14_change_04 ),
};

/* Char_Stand 0: act_stand, appear_move, act_change_gattai_move, act_change_zero_move, ...
   Char_Stand 1
   Char_Stand 4
   Char_Stand 5
   Char_Effect 0
   group 25 0 */
const Sint32 pl14_stand_00[] = {
	CG_FRAME(    0,    0,   5,           0,    45, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,    46, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,    47, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,    48, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,    49, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,    50, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,    51, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,    52, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,    53, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,    54, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,    45, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,    46, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,    47, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,    48, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,    49, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,    50, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,    51, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,    52, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,    53, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,    54, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,    55, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,    56, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,    57, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,    58, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,    59, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,    60, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,    61, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,    62, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,    63, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,    64, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl14_stand_00 ),
};

/* Char_Stand 2: wall_init (Wall Walk), act_walk (2 + no)
   Char_Stand 6 */
const Sint32 pl14_stand_02[] = {
	CG_FRAME(    0,    0,   3,           0,    68, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    69, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    70, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    71, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    72, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    65, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    66, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,    67, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl14_stand_02 ),
};

/* Char_Stand 3
   Char_Stand 7 */
const Sint32 pl14_stand_03[] = {
	CG_FRAME(    0,    0,   3,           0,    76, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    77, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    78, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    79, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    80, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    73, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    74, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,    75, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl14_stand_03 ),
};

/* Char_Jump 0 */
const Sint32 pl14_jump_00[] = {
	CG_FRAME(    0,    1,   4,           0,    87, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   6,           0,    88, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   6,           0,    89, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   8,           0,    90, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    91, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    92, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    93, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    94, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    95, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,  24, END_OF_DATA,    96, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_NEXT( pl14_jump_00 ),
};

/* Char_Jump 1: appear_back, act_wall_jump, act_change_gattai_init, act_change_zero_init, ... */
const Sint32 pl14_jump_01[] = {
	CG_FRAME(    0,    1,   4,           0,    97, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   6,           0,    98, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   5,           0,    99, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   100, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   101, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   102, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   5,           0,   103, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,  14,           0,   104, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,  24, END_OF_DATA,   105, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_NEXT( pl14_jump_01 ),
};

/* Char_Jump 2: pow_hop (Pow (hyper)) */
const Sint32 pl14_jump_02[] = {
	CG_FRAME(    0,    1,   4,           0,   107, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   108, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   109, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   110, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   111, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   112, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   113, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   110, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   111, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   112, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   113, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   110, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   111, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   112, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   113, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   110, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   111, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   112, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   113, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_NEXT( pl14_jump_02 ),
};

/* Char_Jump 3: appear_fall */
const Sint32 pl14_jump_03[] = {
	CG_FRAME(    0,    0,   4,           0,   117, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   120, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   121, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   122, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl14_jump_03 ),
};

/* Char_Jump 4 */
const Sint32 pl14_jump_04[] = {
	CG_FRAME(    0,    0,   4,           0,   106, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   120, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   121, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   122, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl14_jump_04 ),
};

/* Char_Jump 5 */
const Sint32 pl14_jump_05[] = {
	CG_FRAME(    0,    0,   2,           0,   114, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   115, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   117, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   120, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   121, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   122, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl14_jump_05 ),
};

/* Char_Jump 6
   Char_Jump 7 */
const Sint32 pl14_jump_06[] = {
	CG_FRAME(    0,    1,   4,           0,   142, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   143, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   142, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   143, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   142, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    89, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    90, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    91, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    92, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    93, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    94, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    95, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
};

const Sint32 pl14_jump_06_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   144, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   145, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_NEXT( pl14_jump_06_loop ),
};

/* Char_Jump 8 */
const Sint32 pl14_jump_08[] = {
	CG_FRAME(    0,    1,   4,           0,   142, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   143, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   142, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   143, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   142, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    95, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    94, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    93, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    92, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    91, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    90, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    89, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
};

const Sint32 pl14_jump_08_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   144, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   145, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_NEXT( pl14_jump_08_loop ),
};

/* Char_Jump 9: act_fall, act_air_guard_hold, act_wall_jump
   Char_Jump 10: change_in
   Char_Jump 11: act_change_out_recover */
const Sint32 pl14_jump_09[] = {
	CG_FRAME(    0,    0,   4,           0,   144, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   145, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_NEXT( pl14_jump_09 ),
};

/* Char_Dash 0: act_dash */
const Sint32 pl14_dash_00[] = {
	CG_FRAME(    0,    0,   2,           0,    81, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,    82, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   5 ),
	CG_FRAME(    0,    1,   8,           0,    83, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   8,           0,    84, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   5 ),
	CG_FRAME(    0,    1,   4,           0,    85, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,    81, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,    86, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   5 ),
	CG_NEXT( pl14_dash_00 ),
};

/* Char_Dash 2 */
const Sint32 pl14_dash_02[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,    81, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl14_dash_02 ),
};

/* Char_Dash 1: act_back_dash */
const Sint32 pl14_dash_01[] = {
	CG_FRAME(    0,    0,   2,           0,   107, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   5,           0,   108, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   5,           0,   109, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   5,           0,   114, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   5,           0,   115, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   7 ),
	CG_FRAME(    0,    0,  -1, END_OF_DATA,   117, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   7 ),
	CG_NEXT( pl14_dash_01 ),
};

/* Char_Dash 3: noki_jump */
const Sint32 pl14_dash_03[] = {
	CG_FRAME(    0,    0,   3,           0,   120, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3,           0,   121, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   122, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   7 ),
	CG_NEXT( pl14_dash_03 ),
};

/* Char_Crouch 0: act_crouch, act_stand_up */
const Sint32 pl14_crouch_00[] = {
	CG_FRAME(    0,    0,   4,           0,   116, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   117, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   118, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   2 ),
	CG_NEXT( pl14_crouch_00 ),
};

/* Char_Crouch 1: act_crouching, act_escape */
const Sint32 pl14_crouch_01[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   119, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   2 ),
	CG_NEXT( pl14_crouch_01 ),
};

/* Char_Crouch 2: act_stand_up, act_crouch */
const Sint32 pl14_crouch_02[] = {
	CG_FRAME(    0,    0,   4,           0,   120, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   121, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   122, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl14_crouch_02 ),
};

/* Char_Turn 0: act_turn */
const Sint32 pl14_turn_00[] = {
	CG_FRAME(    0,    0,   4,           0,   125, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   124, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   123, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl14_turn_00 ),
};

/* Char_Turn 1: act_crouch_turn */
const Sint32 pl14_turn_01[] = {
	CG_FRAME(    0,    0,   4,           0,   129, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   128, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   127, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   126, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   2 ),
	CG_NEXT( pl14_turn_01 ),
};

/* Char_Guard 0 */
const Sint32 pl14_guard_00[] = {
	CG_FRAME(    0,    0,   2,           0,   130, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   2,           0,   131, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,  12 ),
};

/* Char_Guard 1: act_guard_hold */
const Sint32 pl14_guard_01[] = {
	CG_FRAME(    0,    1,   2, END_OF_DATA,   132, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,  12 ),
	CG_NEXT( pl14_guard_01 ),
};

/* Char_Guard 2 */
const Sint32 pl14_guard_02[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   133, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,  12 ),
	CG_NEXT( pl14_guard_02 ),
};

/* Char_Guard 3 */
const Sint32 pl14_guard_03[] = {
	CG_FRAME(    0,    1,   4, END_OF_DATA,   132, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,  12 ),
	CG_NEXT( pl14_guard_03 ),
};

/* Char_Guard 4 */
const Sint32 pl14_guard_04[] = {
	CG_FRAME(    0,    0,   2,           0,   134, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   2,           0,   135, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,  13 ),
};

/* Char_Guard 5: act_guard_hold */
const Sint32 pl14_guard_05[] = {
	CG_FRAME(    0,    1,   2, END_OF_DATA,   136, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,  13 ),
	CG_NEXT( pl14_guard_05 ),
};

/* Char_Guard 6 */
const Sint32 pl14_guard_06[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   137, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,  13 ),
	CG_NEXT( pl14_guard_06 ),
};

/* Char_Guard 7 */
const Sint32 pl14_guard_07[] = {
	CG_FRAME(    0,    1,   4, END_OF_DATA,   136, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,  13 ),
	CG_NEXT( pl14_guard_07 ),
};

/* Char_Air_Guard 0: act_air_guard */
const Sint32 pl14_air_guard_00[] = {
	CG_FRAME(    0,    0,   2,           0,   138, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   2,           0,   139, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,  14 ),
};

/* Char_Air_Guard 1: act_air_guard_start
   Char_Air_Guard 2: guard_char_set */
const Sint32 pl14_air_guard_01[] = {
	CG_FRAME(    0,    1,   4, END_OF_DATA,   140, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,  14 ),
	CG_NEXT( pl14_air_guard_01 ),
};

/* Char_Stand_P 0 (light) */
const Sint32 pl14_stand_p_00[] = {
	CG_FRAME(    0,    0,   3,           0,   254, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   1,           0,   255, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_FRAME( 0x80,    0,   2,           0,   256, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  16 ),
	CG_FRAME(    0,    0,   6,           0,   257, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  17 ),
	CG_FRAME(    0,    0,   4,           0,   258, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  17 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   259, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_NEXT( pl14_stand_p_00 ),
};

/* Char_Stand_P 1 (medium) */
const Sint32 pl14_stand_p_01[] = {
	CG_FRAME(    0,    0,   4,           0,   260, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  57 ),
	CG_FRAME(    0,    0,   2,           0,   261, 0x80,   0x00,  ATK,  0x00,       0x03,    0,    0,     0,      0,  57 ),
	CG_FRAME( 0x80,    0,   2,           0,   262, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  18 ),
	CG_FRAME(    0,    0,   4,           0,   263, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  20 ),
	CG_FRAME(    0,    0,   2,           0,   264, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  20 ),
	CG_FRAME(    0,    1,   2,           0,   265, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  20 ),
	CG_FRAME( 0x80,    0,   2,           0,   266, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  19 ),
	CG_FRAME( 0x80,    0,   2,           0,   267, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  20 ),
	CG_FRAME(    0,    0,   8,           0,   268, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  57 ),
	CG_FRAME(    0,    0,   5,           0,   269, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  57 ),
	CG_FRAME(    0,    0,   4,           0,   270, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  57 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   271, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_NEXT( pl14_stand_p_01 ),
};

/* Char_Stand_P 2 (heavy) */
const Sint32 pl14_stand_p_02[] = {
	CG_FRAME(    0,    0,   5,           0,   272, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_FRAME(    0,   10,   2,           0,   273, 0x80,   0x00,  ATK,  0x00,       0x04,    0,    0,     0,      0,   8 ),
	CG_FRAME(    0,    8,   1,           0,   274, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   1,           0,   275, 0x80,   0x00,  ATK,  0x00, 0x16 | CSE,    0,    0,     0,      0,  58 ),
	CG_FRAME( 0x80,    8,   2,           0,   276, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  21 ),
	CG_FRAME( 0x80,    0,   4,           0,   277, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  59 ),
	CG_FRAME( 0x80,    0,   4,           0,   278, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  59 ),
	CG_FRAME(    0,    0,  10,           0,   279, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   6,           0,   280, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   5,           0,   281, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  58 ),
	CG_FRAME(    0,  -26,   5,           0,   282, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   4,           0,   258, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   259, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_NEXT( pl14_stand_p_02 ),
};

/* Char_Stand_P 3 */
const Sint32 pl14_stand_p_03[] = {
	CG_FRAME(    0,    0,   1,           0,   430, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   1,           0,   431, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   5,           0,   432, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   2,           0,   433, 0x80,   0x00,  ATK,  0x00,       0x04,    0,    0,     0,      0,  55 ),
	CG_FRAME( 0x80,    0,   2,           0,   434, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  56 ),
	CG_FRAME( 0x80,    0,   1,           0,   435, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  99 ),
	CG_FRAME( 0x80,    0,   1,           0,   436, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  99 ),
	CG_FRAME( 0x80,    0,   2,           0,   437, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  99 ),
	CG_FRAME(    0,    0,  12,           0,   438, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  99 ),
	CG_FRAME(    0,    0,   6,           0,   439, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  55 ),
	CG_FRAME(    0,    0,   5,           0,   313, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   314, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_NEXT( pl14_stand_p_03 ),
};

/* Char_Stand_K 0 (light) */
const Sint32 pl14_stand_k_00[] = {
	CG_FRAME(    0,    0,   1,           0,   283, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   3,           0,   284, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_FRAME( 0x80,    0,   2,           0,   285, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  22 ),
	CG_FRAME(    0,    0,   6,           0,   286, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  23 ),
	CG_FRAME(    0,    0,   4,           0,   287, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   288, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_NEXT( pl14_stand_k_00 ),
};

/* Char_Stand_K 1 (medium) */
const Sint32 pl14_stand_k_01[] = {
	CG_FRAME(    0,    0,   4,           0,   289, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   2,           0,   290, 0x80,   0x00,  ATK,  0x00,       0x04,    0,    0,     0,      0,   8 ),
	CG_FRAME( 0x80,    0,   2,           0,   291, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  24 ),
	CG_FRAME(    0,    0,   5,           0,   292, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0, 100 ),
	CG_FRAME(    0,    0,   2,           0,   293, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0, 100 ),
	CG_FRAME(    0,    0,   2,           0,   294, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  26 ),
	CG_FRAME( 0x80,    1,   2,           0,   295, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  25 ),
	CG_FRAME(    0,    0,   9,           0,   296, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  26 ),
	CG_FRAME(    0,    0,   5,           0,   297, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  26 ),
	CG_FRAME(    0,    0,   5,           0,   298, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4,           0,   299, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   300, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_NEXT( pl14_stand_k_01 ),
};

/* Char_Stand_K 2 (heavy) */
const Sint32 pl14_stand_k_02[] = {
	CG_FRAME(    0,    0,   3,           0,   301, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   3,           0,   302, 0x80,   0x00,  ATK,  0x00,       0x03,    0,    0,     0,      0,   8 ),
	CG_FRAME( 0x80,   24,   2,           0,   303, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_FRAME(    0,   40,   4,           0,   304, 0x80,   0x00,  ATK,  0x00,       0x24,    0,    0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   2,           0,   305, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  27 ),
	CG_FRAME(    0,    0,   3,           0,   306, 0x80,   0x00,  ATK,  0x00, 0x16 | CSE,    0,    0,     0,      0,  27 ),
	CG_FRAME(    0,    0,   4,           0,   307, 0x80,   0x00,  ATK,  0x01,          0,    0,    0,     0,      0,  29 ),
	CG_FRAME(    0,    0,   2,           0,   308, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0, 105 ),
	CG_FRAME(    0,    0,   2,           0,   309, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  30 ),
	CG_FRAME(    0,    0,   4,           0,   310, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  27 ),
	CG_FRAME(    0,    0,   6,           0,   311, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_FRAME(    0,  -10,   6,           0,   312, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_FRAME(    0,  -54,   6,           0,   313, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   314, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_NEXT( pl14_stand_k_02 ),
};

/* Char_Crouch_P 0 (light) */
const Sint32 pl14_crouch_p_00[] = {
	CG_FRAME(    0,    0,   1,           0,   315, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   9 ),
	CG_FRAME(    0,    0,   3,           0,   316, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   9 ),
	CG_FRAME( 0x80,    0,   2,           0,   317, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  31 ),
	CG_FRAME(    0,    0,   6,           0,   318, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  32 ),
	CG_FRAME(    0,    0,   4,           0,   319, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  32 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   320, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   9 ),
	CG_NEXT( pl14_crouch_p_00 ),
};

/* Char_Crouch_P 1 (medium) */
const Sint32 pl14_crouch_p_01[] = {
	CG_FRAME(    0,    0,   4,           0,   321, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   9 ),
	CG_FRAME(    0,    0,   2,           0,   322, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   9 ),
	CG_FRAME(    0,    0,   2,           0,   323, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  33 ),
	CG_FRAME( 0x80,    1,   2,           0,   324, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  34 ),
	CG_FRAME( 0x80,    0,   2,           0,   325, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  35 ),
	CG_FRAME(    0,    0,   9,           0,   326, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  35 ),
	CG_FRAME(    0,    0,   5,           0,   327, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  35 ),
	CG_FRAME(    0,    0,   4,           0,   328, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   9 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   320, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   9 ),
	CG_NEXT( pl14_crouch_p_01 ),
};

/* Char_Crouch_P 2 (heavy) */
const Sint32 pl14_crouch_p_02[] = {
	CG_FRAME(    0,    0,   2,           0,   329, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   9 ),
	CG_FRAME(    0,   18,   5,           0,   330, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   9 ),
	CG_FRAME(    0,   27,   1,           0,   331, 0x80,   0x00,  ATK,  0x00,       0x04,    0,    0,     0,      0,   9 ),
	CG_FRAME(    0,    0,   1,           0,   332, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   9 ),
	CG_FRAME(    0,    0,   1,           0,   333, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   9 ),
	CG_FRAME(    0,    0,   3,           0,   334, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  36 ),
	CG_FRAME(    0,    0,   2,           0,   335, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  37 ),
	CG_FRAME(    0,    0,   2,           0,   336, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  37 ),
	CG_FRAME(    0,    0,  12,           0,   337, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  37 ),
	CG_FRAME(    0,    0,   2,           0,   338, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   9 ),
	CG_FRAME(    0,  -24,   3,           0,   339, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   9 ),
	CG_FRAME(    0,  -13,   3,           0,   340, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   9 ),
	CG_FRAME(    0,   -6,   4, END_OF_DATA,   320, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   9 ),
	CG_NEXT( pl14_crouch_p_02 ),
};

/* Char_Crouch_K 0 (light) */
const Sint32 pl14_crouch_k_00[] = {
	CG_FRAME(    0,    0,   3,           0,   341, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   9 ),
	CG_FRAME( 0x80,    0,   2,           0,   342, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  38 ),
	CG_FRAME( 0x80,    0,   2,           0,   343, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  37 ),
	CG_FRAME(    0,    0,   6,           0,   344, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  37 ),
	CG_FRAME(    0,    0,   4,           0,   345, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  37 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   346, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   9 ),
	CG_NEXT( pl14_crouch_k_00 ),
};

/* Char_Crouch_K 1 (medium) */
const Sint32 pl14_crouch_k_01[] = {
	CG_FRAME(    0,    0,   2,           0,   120, 0x80,   0x00,    0,  0x00,          0,    0, 0x80,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   347, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   9 ),
	CG_FRAME( 0x80,    0,   2,           0,   348, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  40 ),
	CG_FRAME( 0x80,    0,   2,           0,   349, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  40 ),
	CG_FRAME(    0,    0,   8,           0,   350, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  41 ),
	CG_FRAME(    0,    0,   5,           0,   351, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  41 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,   352, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   9 ),
	CG_NEXT( pl14_crouch_k_01 ),
};

/* Char_Crouch_K 2 (heavy) */
const Sint32 pl14_crouch_k_02[] = {
	CG_FRAME(    0,    0,   4,           0,   128, 0x80,   0x00,    0,  0x00,          0,    0, 0x80,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2,           0,   353, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   9 ),
	CG_FRAME(    0,    0,   2,           0,   354, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  43 ),
	CG_FRAME(    0,    0,   2,           0,   355, 0x80,   0x00,  ATK,  0x00,       0x04,    0,    0,     0,      0,  43 ),
	CG_FRAME( 0x80,    0,   2,           0,   356, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  42 ),
	CG_FRAME( 0x80,    0,   3,           0,   357, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0, 101 ),
	CG_FRAME(    0,    0,  11,           0,   358, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0, 101 ),
	CG_FRAME(    0,    0,   5,           0,   359, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0, 101 ),
	CG_FRAME(    0,    0,   5,           0,   360, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  43 ),
	CG_FRAME(    0,    0,   4,           0,   361, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   9 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   346, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   9 ),
	CG_NEXT( pl14_crouch_k_02 ),
};

/* Char_Jump_P 0 (light)
   Char_Jump_P 3 */
const Sint32 pl14_jump_p_00[] = {
	CG_FRAME(    0,    0,   4,           0,   362, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  10 ),
	CG_FRAME( 0x80,    0,   2,           0,   363, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  44 ),
	CG_FRAME(    0,    0,   4,           0,   364, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  44 ),
	CG_FRAME(    0,    0,   6,           0,   364, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   365, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  10 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   366, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  10 ),
	CG_NEXT( pl14_jump_p_00 ),
};

/* Char_Jump_P 1 (medium)
   Char_Jump_P 4 */
const Sint32 pl14_jump_p_01[] = {
	CG_FRAME(    0,    0,   4,           0,   367, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  10 ),
	CG_FRAME(    0,    0,   2,           0,   368, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  10 ),
	CG_FRAME( 0x80,    0,   2,           0,   369, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  46 ),
	CG_FRAME( 0x80,    0,   2,           0,   370, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  47 ),
	CG_FRAME(    0,    0,   9,           0,   371, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  10 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   372, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  10 ),
	CG_NEXT( pl14_jump_p_01 ),
};

/* Char_Jump_P 2 (heavy)
   Char_Jump_P 5 */
const Sint32 pl14_jump_p_02[] = {
	CG_FRAME(    0,    0,   2,           0,   373, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  10 ),
	CG_FRAME(    0,    0,   6,           0,   374, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  10 ),
	CG_FRAME(    0,    0,   2,           0,   375, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  10 ),
	CG_FRAME(    0,    1,   2,           0,   376, 0x80,   0x00,  ATK,  0x01,          0,    0,    0,     0,      0,  48 ),
	CG_FRAME(    0,    1,   2,           0,   376, 0x80,   0x00,  ATK,  0x01,          0,    0,    0,     0,      0,  48 ),
	CG_FRAME(    0,    1,   2,           0,   376, 0x80,   0x00,  ATK,  0x02,          0,    0,    0,     0,      0,  48 ),
	CG_FRAME(    0,    1,  12,           0,   377, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,   378, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  10 ),
	CG_NEXT( pl14_jump_p_02 ),
};

/* Char_Jump_K 0 (light)
   Char_Jump_K 3 */
const Sint32 pl14_jump_k_00[] = {
	CG_FRAME(    0,    0,   4,           0,   379, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  10 ),
	CG_FRAME( 0x80,    0,   2,           0,   380, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  50 ),
	CG_FRAME( 0x80,    0,   2,           0,   381, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  50 ),
	CG_FRAME( 0x80,    0,   4,           0,   382, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   5,           0,   382, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  51 ),
	CG_FRAME(    0,    0,   5,           0,   383, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  10 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   362, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  10 ),
	CG_NEXT( pl14_jump_k_00 ),
};

/* Char_Jump_K 1 (medium)
   Char_Jump_K 4 */
const Sint32 pl14_jump_k_01[] = {
	CG_FRAME(    0,    0,   4,           0,   384, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  10 ),
	CG_FRAME(    0,    0,   2,           0,   385, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  10 ),
	CG_FRAME( 0x80,    0,   2,           0,   386, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   8,           0,   387, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  53 ),
	CG_FRAME(    0,    0,   6,           0,   388, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  53 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   372, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  10 ),
	CG_NEXT( pl14_jump_k_01 ),
};

/* Char_Jump_K 2 (heavy)
   Char_Jump_K 5 */
const Sint32 pl14_jump_k_02[] = {
	CG_FRAME(    0,    0,   3,           0,   389, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  10 ),
	CG_FRAME(    0,    0,   3,           0,   390, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  10 ),
	CG_FRAME(    0,    0,   2,           0,   391, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  62 ),
	CG_FRAME( 0x80,    0,   2,           0,   392, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  54 ),
	CG_FRAME(    0,    0,  10,           0,   393, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  62 ),
	CG_FRAME(    0,    0,   6,           0,   394, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  62 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,   372, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  10 ),
	CG_NEXT( pl14_jump_k_02 ),
};

/* Char_Get_Up 0: dm_dizzy_wait */
const Sint32 pl14_get_up_00[] = {
	CG_FRAME(    0,    0,   2,           0,   146, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   147, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   148, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   149, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   150, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   151, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   152, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   117, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   118, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   119, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   121, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   3, END_OF_DATA,   122, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl14_get_up_00 ),
};

/* Char_Get_Up 1 */
const Sint32 pl14_get_up_01[] = {
	CG_FRAME(    0,    0,   2,           0,   156, 0x80,   0x00,    0,  0x00,          0,    1,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   153, 0x80,   0x00,    0,  0x00,          0,    1,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   154, 0x80,   0x00,    0,  0x00,          0,    1,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   155, 0x80,   0x00,    0,  0x00,          0,    1,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   157, 0x80,   0x00,    0,  0x00,          0,    1,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   158, 0x80,   0x00,    0,  0x00,          0,    1,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   117, 0x80,   0x00,    0,  0x00,          0,    1,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   120, 0x80,   0x00,    0,  0x00,          0,    1,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   121, 0x80,   0x00,    0,  0x00,          0,    1,    0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   1, END_OF_DATA,   122, 0x80,   0x00,    0,  0x00,          0,    1,    0,     0,      0,   0 ),
	CG_NEXT( pl14_get_up_01 ),
};

/* Char_Get_Up 2 */
const Sint32 pl14_get_up_02[] = {
	CG_FRAME(    0,    0,   5,           0,   156, 0x80,   0x00,    0,  0x00,          0,    1,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   153, 0x80,   0x00,    0,  0x00,          0,    1,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   154, 0x80,   0x00,    0,  0x00,          0,    1,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   155, 0x80,   0x00,    0,  0x00,          0,    1,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   157, 0x80,   0x00,    0,  0x00,          0,    1,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   158, 0x80,   0x00,    0,  0x00,          0,    1,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   117, 0x80,   0x00,    0,  0x00,          0,    1,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   120, 0x80,   0x00,    0,  0x00,          0,    1,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   121, 0x80,   0x00,    0,  0x00,          0,    1,    0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   4, END_OF_DATA,   122, 0x80,   0x00,    0,  0x00,          0,    1,    0,     0,      0,   0 ),
	CG_NEXT( pl14_get_up_02 ),
};

/* Char_Get_Up 3 */
const Sint32 pl14_get_up_03[] = {
	CG_FRAME(    0,    0,   3,           0,   156, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   153, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   154, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   155, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   157, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   158, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   117, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   120, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   121, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   3, END_OF_DATA,   122, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_NEXT( pl14_get_up_03 ),
};

/* Char_Damage 0: dm_freeze, dm_freeze_air, caught_dm */
const Sint32 pl14_damage_00[] = {
	CG_FRAME(    0,    0,   5,           0,   696, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   697, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl14_damage_00 ),
};

/* Char_Damage 1 */
const Sint32 pl14_damage_01[] = {
	CG_FRAME(    0,    0,   6,           0,   695, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   696, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   697, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl14_damage_01 ),
};

/* Char_Damage 2 */
const Sint32 pl14_damage_02[] = {
	CG_FRAME(    0,    0,   6,           0,   694, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   695, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   696, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   697, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl14_damage_02 ),
};

/* Char_Damage 3 */
const Sint32 pl14_damage_03[] = {
	CG_FRAME(    0,    0,   5,           0,   700, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   701, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl14_damage_03 ),
};

/* Char_Damage 4 */
const Sint32 pl14_damage_04[] = {
	CG_FRAME(    0,    0,   6,           0,   699, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   700, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   701, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl14_damage_04 ),
};

/* Char_Damage 5 */
const Sint32 pl14_damage_05[] = {
	CG_FRAME(    0,    0,   6,           0,   698, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   699, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   700, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   701, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl14_damage_05 ),
};

/* Char_Damage 6 */
const Sint32 pl14_damage_06[] = {
	CG_FRAME(    0,    0,   5,           0,   704, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   705, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   2 ),
	CG_NEXT( pl14_damage_06 ),
};

/* Char_Damage 7 */
const Sint32 pl14_damage_07[] = {
	CG_FRAME(    0,    0,   6,           0,   703, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   6,           0,   704, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   705, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   2 ),
	CG_NEXT( pl14_damage_07 ),
};

/* Char_Damage 8 */
const Sint32 pl14_damage_08[] = {
	CG_FRAME(    0,    0,   6,           0,   702, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   6,           0,   703, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   6,           0,   704, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   705, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   2 ),
	CG_NEXT( pl14_damage_08 ),
};

/* Char_Damage 9: dm_sweep */
const Sint32 pl14_damage_09[] = {
	CG_FRAME(    0,    0,   2,           0,   712, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   713, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   720, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   719, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   718, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   714, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   721, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   722, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   723, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   724, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   725, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
};

const Sint32 pl14_damage_09_loop[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA,   726, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,  15 ),
	CG_NEXT( pl14_damage_09_loop ),
};

/* Char_Damage 10: dm_ko_blow */
const Sint32 pl14_damage_10[] = {
	CG_FRAME(    0,    0,   5,           0,   708, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,   709, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_NEXT( pl14_damage_10 ),
};

/* Char_Damage 11 */
const Sint32 pl14_damage_11[] = {
	CG_FRAME(    0,    0,   3,           0,   707, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   708, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   709, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_NEXT( pl14_damage_11 ),
};

/* Char_Damage 12 */
const Sint32 pl14_damage_12[] = {
	CG_FRAME(    0,    0,   2,           0,   706, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   707, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   708, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   709, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_NEXT( pl14_damage_12 ),
};

/* Char_Damage 13: dm_snap_fly, dm_air
   Char_Damage 14
   Char_Damage 15
   Char_Damage 34: dm_snap_fly, dm_snap_down */
const Sint32 pl14_damage_13[] = {
	CG_FRAME(    0,    0,   4,           0,   710, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,  15 ),
};

const Sint32 pl14_damage_13_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   711, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,  15 ),
	CG_NEXT( pl14_damage_13_loop ),
};

/* Char_Damage 16: dm_blow_char_set (16 + no), dm_spark, caught_shock_init */
const Sint32 pl14_damage_16[] = {
	CG_FRAME(    0,    0,   4,           0,   715, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   716, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_NEXT( pl14_damage_16 ),
};

/* Char_Damage 17: dm_07_1 */
const Sint32 pl14_damage_17[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   717, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_NEXT( pl14_damage_17 ),
};

/* Char_Damage 18 */
const Sint32 pl14_damage_18[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   718, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_NEXT( pl14_damage_18 ),
};

/* Char_Damage 19 */
const Sint32 pl14_damage_19[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   719, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_NEXT( pl14_damage_19 ),
};

/* Char_Damage 20 */
const Sint32 pl14_damage_20[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   720, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_NEXT( pl14_damage_20 ),
};

/* Char_Damage 21: dm_fall_char_set (21 + no) */
const Sint32 pl14_damage_21[] = {
	CG_FRAME(    0,    0,   4,           0,   715, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   716, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
};

/* Char_Damage 22 */
const Sint32 pl14_damage_22[] = {
	CG_FRAME(    0,    0,   4,           0,   717, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
};

/* Char_Damage 23 */
const Sint32 pl14_damage_23[] = {
	CG_FRAME(    0,    0,   4,           0,   718, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
};

/* Char_Damage 24 */
const Sint32 pl14_damage_24[] = {
	CG_FRAME(    0,    0,   4,           0,   719, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
};

/* Char_Damage 25 */
const Sint32 pl14_damage_25[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   720, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_NEXT( pl14_damage_20 ),
};

/* Char_Damage 26: dm_buttobi_fly, dm_07_1, dm_away_fly, dm_bound_hop, ... */
const Sint32 pl14_damage_26[] = {
	CG_FRAME(    0,    0,   2,           0,   721, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,  15 ),
	CG_FRAME(    0,    0,   2,           0,   722, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,  15 ),
	CG_FRAME(    0,    0,   2,           0,   723, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,  15 ),
	CG_FRAME(    0,    0,   2,           0,   724, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,  15 ),
	CG_FRAME(    0,    0,   2,           0,   725, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,  15 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   726, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,  15 ),
	CG_NEXT( pl14_damage_26 ),
};

/* Char_Damage 31: act_down, dm_dizzy_down */
const Sint32 pl14_damage_31[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   146, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,  15 ),
	CG_NEXT( pl14_damage_31 ),
};

/* Char_Damage 27
   Char_Damage 30: act_air_recover, act_change_out_init, caught_blow_fly, caught_blow_fix_fly, ...
   Char_Damage 32: act_escape
   Char_Damage 33: act_escape */
const Sint32 pl14_damage_27[] = {
	CG_FRAME(    0,    0,   3,           0,   108, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   109, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   110, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
};

const Sint32 pl14_damage_27_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   111, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_NEXT( pl14_damage_27_loop ),
};

/* Char_Damage 28: dm_away_init, dm_bound_fly, dm_piyo, dm_away */
const Sint32 pl14_damage_28[] = {
	CG_FRAME(    0,    0,   4,           0,   727, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   728, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   729, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   730, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   731, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   732, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_NEXT( pl14_damage_28 ),
};

/* Char_Damage 29: dm_away_fly, dm_bound_hop, dm_bound, caught_spin_fly, ... */
const Sint32 pl14_damage_29[] = {
	CG_FRAME(    0,    0,   4,           0,   733, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   734, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   735, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   736, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   737, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   738, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_NEXT( pl14_damage_29 ),
};

/* Char_Catch 0: nml_p_set */
const Sint32 pl14_catch_00[] = {
	CG_FRAME(    0,    0,   4,           0,   395, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0, 0x0001,   3 ),
	CG_FRAME(    0,    0,   4,           0,   396, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0, 0x0002,   3 ),
	CG_FRAME(    0,    0,   4,           0,   397, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0, 0x0003,   3 ),
	CG_FRAME(    0,    0,   4,           0,   398, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0, 0x0004,   3 ),
	CG_FRAME(    0,    0,   4,           0,   399, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0, 0x0005,   3 ),
	CG_FRAME(    0,    0,   4,           0,   400, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0, 0x0006,   3 ),
	CG_FRAME(    0,    0,   4,           0,   401, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0, 0x0007,   3 ),
	CG_FRAME(    0,    1,   4,           0,   402, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0, 0x0008,   3 ),
	CG_FRAME(    0,    0,   4,           0,   403, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0, 0x0009,   3 ),
	CG_FRAME(    0,    0,   4,           0,   404, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0, 0x000a,   3 ),
	CG_FRAME(    0,    1,   4,           0,   405, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0, 0x000b,   3 ),
	CG_FRAME(    0,    0,   4,           0,   406, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0, 0x000c,   3 ),
	CG_FRAME(    0,    0,   4,           0,   407, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0, 0x000d,   3 ),
	CG_FRAME(    0,    0,   4,           0,   408, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0, 0x000e,   3 ),
	CG_FRAME(    0,    1,   4,           0,   409, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0, 0x000f,   3 ),
	CG_FRAME(    0,    2,   4,           0,   410, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   103, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   8,           0,   104, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
};

const Sint32 pl14_catch_00_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   105, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_NEXT( pl14_catch_00_loop ),
};

/* Char_Catch 2: p_catch_fall */
const Sint32 pl14_catch_02[] = {
	CG_FRAME(    0,    0,   4,           0,   106, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   121, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   122, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_NEXT( pl14_catch_02 ),
};

/* Char_Catch 1 */
const Sint32 pl14_catch_01[] = {
	CG_FRAME(    0,    0,   4,           0,   412, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0, 0x0010,   1 ),
	CG_FRAME(    0,    0,   4,           0,   413, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0, 0x0011,   1 ),
	CG_FRAME(    0,    0,   4,           0,   414, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0, 0x0012,   1 ),
	CG_FRAME(    0,    0,   4,           0,   415, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0, 0x0013,   1 ),
	CG_FRAME(    0,    0,   4,           0,   416, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0, 0x0014,   1 ),
	CG_FRAME(    0,    1,  42,           0,   417, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   418, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   419, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl14_catch_01 ),
};

/* Char_Catch 5: nml_k_set */
const Sint32 pl14_catch_05[] = {
	CG_FRAME( 0x80,    0,   4,           0,   975, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0, 0x0011,   1 ),
	CG_FRAME(    0,    1,   4,           0,   976, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0, 0x0011,   1 ),
	CG_FRAME(    0,    2,   4,           0,   976, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0, 0x0012,   1 ),
	CG_FRAME(    0,    3,   4,           0,   977, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0, 0x0013,   1 ),
	CG_FRAME(    0,    4,   4,           0,   977, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0, 0x0014,   1 ),
	CG_FRAME(    0,    5,  42,           0,   977, 0x80,   0x00,    0,  0x01,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    6,   4,           0,   977, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   978, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl14_catch_05 ),
};

/* Char_Catch 3: jump_p_set */
const Sint32 pl14_catch_03[] = {
	CG_FRAME(    0,    0,   4,           0,   397, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0, 0x0017,   3 ),
	CG_FRAME(    0,    0,   4,           0,   398, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0, 0x0017,   3 ),
	CG_FRAME(    0,    0,   4,           0,   399, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0, 0x0018,   3 ),
	CG_FRAME(    0,    0,   4,           0,   400, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0, 0x0019,   3 ),
	CG_FRAME(    0,    0,   4,           0,   407, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0, 0x0026,   3 ),
	CG_FRAME(    0,    0,   4,           0,   408, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0, 0x0027,   3 ),
	CG_FRAME(    0,    1,   4,           0,   409, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0, 0x0028,   3 ),
	CG_FRAME(    0,    2,   4,           0,   410, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   103, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   8,           0,   104, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
};

const Sint32 pl14_catch_03_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   105, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_NEXT( pl14_catch_03_loop ),
};

/* Char_Catch 4: hisago_init (Kingin no Hisago) */
const Sint32 pl14_catch_04[] = {
	CG_FRAME(    0,    0,   4,           0,   440, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   441, 0x80,   0x00,  ATK,  0x00,       0x17,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   442, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   4,           0,   443, 0x80,   0x00,  ATK,  0x00,       0x20,    0,    0,     0,      0,   1 ),
};

const Sint32 pl14_catch_04_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   444, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0, 0x0029,  96 ),
	CG_FRAME(    0,    0,   4,           0,   445, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0, 0x0029,  96 ),
	CG_FRAME(    0,    0,   4,           0,   444, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0, 0x0029,  96 ),
	CG_FRAME(    0,    0,   4,           0,   445, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0, 0x0029,  96 ),
	CG_FRAME(    0,    0,   4,           0,   444, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0, 0x0029,  96 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   445, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0, 0x0029,  96 ),
	CG_NEXT( pl14_catch_04_loop ),
};

/* Char_Caught 0 */
const Sint32 pl14_caught_00[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   696, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,  83 ),
	CG_NEXT( pl14_caught_00 ),
};

/* Char_Caught 1 */
const Sint32 pl14_caught_01[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   695, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,  83 ),
	CG_NEXT( pl14_caught_01 ),
};

/* Char_Caught 2 */
const Sint32 pl14_caught_02[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   694, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,  83 ),
	CG_NEXT( pl14_caught_02 ),
};

/* Char_Caught 3 */
const Sint32 pl14_caught_03[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   700, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,  83 ),
	CG_NEXT( pl14_caught_03 ),
};

/* Char_Caught 4 */
const Sint32 pl14_caught_04[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   699, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,  83 ),
	CG_NEXT( pl14_caught_04 ),
};

/* Char_Caught 5 */
const Sint32 pl14_caught_05[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   698, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,  83 ),
	CG_NEXT( pl14_caught_05 ),
};

/* Char_Caught 6 */
const Sint32 pl14_caught_06[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   704, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,  82 ),
	CG_NEXT( pl14_caught_06 ),
};

/* Char_Caught 7 */
const Sint32 pl14_caught_07[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   703, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,  82 ),
	CG_NEXT( pl14_caught_07 ),
};

/* Char_Caught 8 */
const Sint32 pl14_caught_08[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   702, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,  82 ),
	CG_NEXT( pl14_caught_08 ),
};

/* Char_Caught 9 */
const Sint32 pl14_caught_09[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   708, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,  86 ),
	CG_NEXT( pl14_caught_09 ),
};

/* Char_Caught 10 */
const Sint32 pl14_caught_10[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   707, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,  86 ),
	CG_NEXT( pl14_caught_10 ),
};

/* Char_Caught 11 */
const Sint32 pl14_caught_11[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   706, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,  86 ),
	CG_NEXT( pl14_caught_11 ),
};

/* Char_Caught 12 */
const Sint32 pl14_caught_12[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   710, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,  87 ),
	CG_NEXT( pl14_caught_12 ),
};

/* Char_Caught 13 */
const Sint32 pl14_caught_13[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   715, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,  86 ),
	CG_NEXT( pl14_caught_13 ),
};

/* Char_Caught 14 */
const Sint32 pl14_caught_14[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   717, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,  86 ),
	CG_NEXT( pl14_caught_14 ),
};

/* Char_Caught 15 */
const Sint32 pl14_caught_15[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   718, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,  86 ),
	CG_NEXT( pl14_caught_15 ),
};

/* Char_Caught 16 */
const Sint32 pl14_caught_16[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   719, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,  84 ),
	CG_NEXT( pl14_caught_16 ),
};

/* Char_Caught 17 */
const Sint32 pl14_caught_17[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   720, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,  82 ),
	CG_NEXT( pl14_caught_17 ),
};

/* Char_Caught 18 */
const Sint32 pl14_caught_18[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   727, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,  86 ),
	CG_NEXT( pl14_caught_18 ),
};

/* Char_Caught 19 */
const Sint32 pl14_caught_19[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   733, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,  86 ),
	CG_NEXT( pl14_caught_19 ),
};

/* Char_Caught 20 */
const Sint32 pl14_caught_20[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   146, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,  87 ),
	CG_NEXT( pl14_caught_20 ),
};

/* Char_Caught 21 */
const Sint32 pl14_caught_21[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   707, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,  86 ),
	CG_NEXT( pl14_caught_21 ),
};

/* Char_Dizzy 0: act_dizzy */
const Sint32 pl14_dizzy_00[] = {
	CG_FRAME(    0,    0,   8,           0,   184, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   8,           0,   185, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   8,           0,   186, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   8,           0,   187, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   8,           0,   186, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   185, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_NEXT( pl14_dizzy_00 ),
};

/* Char_Win 0: win_ko_init */
const Sint32 pl14_win_00[] = {
	CG_FRAME(    0,    0,   3,           0,   212, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   213, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   214, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   193, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   194, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   196, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   197, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   194, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   195, 0x80,   0x00,    0,  0x00,       0x03,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   196, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   193, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   194, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   195, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   196, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   193, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   194, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   195, 0x80,   0x00,    0,  0x00,       0x03,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   196, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   193, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   194, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   195, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   196, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   193, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   194, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   195, 0x80,   0x00,    0,  0x00,       0x03,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   196, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   193, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   194, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   195, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   196, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   198, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   199, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   200, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   201, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   202, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   203, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   200, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   204, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   205, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   206, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   207, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   204, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   205, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   206, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   207, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   204, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   205, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   206, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   207, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   204, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   215, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   206, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   216, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   216, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   216, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   218, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   219, 0x80,   0x00,    0,  0x00,       0x10,    0,    0,     0,      0,   0 ),
};

const Sint32 pl14_win_00_loop[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   220, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_NEXT( pl14_win_00_loop ),
};

/* Char_Win 1
   Char_Win 2: win_ko_init */
const Sint32 pl14_win_01[] = {
	CG_FRAME(    0,    0,   3,           0,   212, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   213, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   214, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   193, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   194, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   196, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   197, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   194, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   195, 0x80,   0x00,    0,  0x00,       0x03,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   196, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   193, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   194, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   195, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   196, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   193, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   194, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   195, 0x80,   0x00,    0,  0x00,       0x03,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   196, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   193, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   194, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   195, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   196, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   193, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   194, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   195, 0x80,   0x00,    0,  0x00,       0x03,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   196, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   193, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   194, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   195, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   196, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   198, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   199, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   200, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   201, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   202, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   203, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   200, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   204, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   205, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   206, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   207, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   204, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   205, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   206, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   207, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   204, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   205, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   206, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   208, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   209, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   210, 0x80,   0x00,    0,  0x00, 0x00 | CSE,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   210, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   210, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   211, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   211, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
};

const Sint32 pl14_win_01_loop[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   211, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_NEXT( pl14_win_01_loop ),
};

/* Char_Win 3
   Char_Win 4: win_ko_init */
const Sint32 pl14_win_03[] = {
	CG_FRAME(    0,    0,   4,           0,    87, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   221, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   222, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   223, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   224, 0x80,   0x00,    0,  0x00,       0x0f,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   225, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   226, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8,           0,   227, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   228, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   229, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8,           0,   224, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   225, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   226, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8,           0,   227, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   228, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   230, 0x80,   0x00,    0,  0x00,       0x1b,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  16,           0,   231, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   233, 0x80,   0x00,    0,  0x00,       0x0f,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   234, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8,           0,   235, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   236, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   237, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8,           0,   232, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   233, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   234, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8,           0,   235, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   236, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   238, 0x80,   0x00,    0,  0x00,       0x1b,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  16,           0,   239, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   225, 0x80,   0x00,    0,  0x00,       0x0f,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   226, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8,           0,   227, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   228, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   229, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8,           0,   224, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   225, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   226, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8,           0,   227, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   228, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   240, 0x80,   0x00,    0,  0x00,       0x1b,    0,    0,     0,      0,   0 ),
};

const Sint32 pl14_win_03_loop[] = {
	CG_FRAME(    0,    0,  16, END_OF_DATA,   241, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_NEXT( pl14_win_03_loop ),
};

/* Char_Win 5: win_ko_init
   Char_Win 6: win_time */
const Sint32 pl14_win_05[] = {
	CG_FRAME(    0,    0,   4,           0,    87, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   250, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   251, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   252, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   249, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,    87, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   253, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   242, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   243, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   244, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   245, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   246, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   247, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   248, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   247, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   248, 0x80,   0x00,    0,  0x00,       0x11,    0,    0,     0,      0,   0 ),
};

const Sint32 pl14_win_05_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   247, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_NEXT( pl14_win_05_loop ),
};

/* Char_Win 7: lose_time_ko
   Char_Win 8: lose_time, draw_time */
const Sint32 pl14_win_07[] = {
	CG_FRAME(    0,    0,  12,           0,   184, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  12,           0,   185, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  12,           0,   186, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  12,           0,   187, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  12,           0,   188, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  12,           0,   189, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   190, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
};

const Sint32 pl14_win_07_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   191, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   192, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_NEXT( pl14_win_07_loop ),
};

/* Char_Win 9: sp_chouhatsu (the taunt) */
const Sint32 pl14_win_09[] = {
	CG_FRAME(    0,    0,   4,           0,   225, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4,           0,   226, 0x80,   0x00,    0,  0x00,       0x0c,    0,    0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4,           0,   227, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4,           0,   228, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4,           0,   229, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4,           0,   224, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4,           0,   225, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4,           0,   226, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4,           0,   227, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4,           0,   228, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4,           0,   229, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   224, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_NEXT( pl14_win_09 ),
};

/* Char_Appear 0 */
const Sint32 pl14_appear_00[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,    45, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl14_stand_00 ),
};

/* Char_Special 0: shienbu_init (Shienbu) */
const Sint32 pl14_special_00[] = {
	CG_FRAME(    0,    0,   4,           0,   524, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  63 ),
	CG_FRAME(    0,    0,   2,           0,   525, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  63 ),
	CG_FRAME(    0,    0,   4,           0,   526, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  63 ),
	CG_FRAME(    0,    0,   1,           0,   527, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  63 ),
	CG_FRAME(    0,    0,   1,           0,   528, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  63 ),
	CG_FRAME(    0,    0,   1,           0,   529, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  63 ),
	CG_FRAME(    0,    0,   2,           0,   530, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  63 ),
	CG_FRAME(    0,    0,   2,           0,   531, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  63 ),
	CG_FRAME(    0,    1,   2,           0,   532, 0x80,   0x00,  ATK,  0x00,       0x16,    0,    0,     0,      0,  63 ),
	CG_FRAME(    0,    0,   3,           0,   530, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  63 ),
	CG_FRAME(    0,    0,   3,           0,   531, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  63 ),
	CG_FRAME(    0,    0,   3,           0,   532, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  63 ),
	CG_FRAME(    0,    0,   4,           0,   530, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  63 ),
	CG_FRAME(    0,    0,   1,           0,   531, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  63 ),
	CG_FRAME(    0,    0,   3,           0,   531, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  63 ),
	CG_FRAME(    0,    0,   4,           0,   532, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  63 ),
	CG_FRAME(    0,    0,   4,           0,   530, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  63 ),
	CG_FRAME(    0,    0,   4,           0,   531, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  63 ),
	CG_FRAME(    0,    0,  16,           0,   532, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  63 ),
	CG_FRAME(    0,    0,   5,           0,   533, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  63 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,   534, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  63 ),
	CG_NEXT( pl14_special_00 ),
};

/* Char_Special 1: shienbu_init (Shienbu) */
const Sint32 pl14_special_01[] = {
	CG_FRAME(    0,    0,   4,           0,   590, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  63 ),
	CG_FRAME(    0,    0,   4,           0,   591, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  63 ),
	CG_FRAME(    0,    0,   4,           0,   592, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  63 ),
	CG_FRAME(    0,    0,   2,           0,   593, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  63 ),
	CG_FRAME(    0,    0,   2,           0,   594, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  63 ),
	CG_FRAME(    0,    0,   2,           0,   595, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  63 ),
	CG_FRAME(    0,    0,   2,           0,   596, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  63 ),
	CG_FRAME(    0,    0,   4,           0,   597, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  63 ),
	CG_FRAME(    0,    1,  16,           0,   598, 0x80,   0x00,  ATK,  0x00,       0x16,    0,    0,     0,      0,  63 ),
	CG_FRAME(    0,    0,   4,           0,   599, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  63 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   600, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  63 ),
	CG_NEXT( pl14_special_01 ),
};

/* Char_Special 2: fuusetsu_init (Fuusetsu Zan) */
const Sint32 pl14_special_02[] = {
	CG_FRAME(    0,    0,   2,           0,   255, 0x80,   0x00,  ATK,  0x00,       0x14,    0,    0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   2,           0,   508, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   8 ),
};

const Sint32 pl14_special_02_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   509, 0x80,   0x00,  ATK,  0x00, 0x14 | CSE,    0,    0,     0,      0,  65 ),
	CG_FRAME(    0,    0,   2,           0,   510, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   2,           0,   511, 0x80,   0x00,  ATK,  0x00, 0x14 | CSE,    0,    0,     0,      0,  65 ),
	CG_FRAME(    0,    0,   2,           0,   512, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   2,           0,   509, 0x80,   0x00,  ATK,  0x01, 0x14 | CSE,    0,    0,     0,      0,  65 ),
	CG_FRAME(    0,    0,   2,           0,   510, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   2,           0,   511, 0x80,   0x00,  ATK,  0x00, 0x14 | CSE,    0,    0,     0,      0,  65 ),
	CG_FRAME(    0,    0,   2,           0,   512, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   2,           0,   513, 0x80,   0x00,  ATK,  0x01, 0x14 | CSE,    0,    0,     0,      0,  64 ),
	CG_FRAME(    0,    0,   2,           0,   514, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   2,           0,   515, 0x80,   0x00,  ATK,  0x00, 0x14 | CSE,    0,    0,     0,      0,  64 ),
	CG_FRAME(    0,    0,   2,           0,   516, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   2,           0,   509, 0x80,   0x00,  ATK,  0x01, 0x14 | CSE,    0,    0,     0,      0,  65 ),
	CG_FRAME(    0,    0,   2,           0,   510, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   2,           0,   511, 0x80,   0x00,  ATK,  0x00, 0x14 | CSE,    0,    0,     0,      0,  65 ),
	CG_FRAME(    0,    0,   2,           0,   512, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   2,           0,   509, 0x80,   0x00,  ATK,  0x01, 0x14 | CSE,    0,    0,     0,      0,  65 ),
	CG_FRAME(    0,    0,   2,           0,   510, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   2,           0,   511, 0x80,   0x00,  ATK,  0x00, 0x14 | CSE,    0,    0,     0,      0,  65 ),
	CG_FRAME(    0,    0,   2,           0,   512, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   2,           0,   509, 0x80,   0x00,  ATK,  0x01, 0x14 | CSE,    0,    0,     0,      0,  65 ),
	CG_FRAME(    0,    0,   2,           0,   510, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   2,           0,   511, 0x80,   0x00,  ATK,  0x00, 0x14 | CSE,    0,    0,     0,      0,  65 ),
	CG_FRAME(    0,    0,   2,           0,   512, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   2,           0,   513, 0x80,   0x00,  ATK,  0x01, 0x14 | CSE,    0,    0,     0,      0,  64 ),
	CG_FRAME(    0,    0,   2,           0,   514, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   2,           0,   515, 0x80,   0x00,  ATK,  0x00, 0x14 | CSE,    0,    0,     0,      0,  64 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   516, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  93 ),
	CG_NEXT( pl14_special_02_loop ),
};

/* Char_Special 3 */
const Sint32 pl14_special_03[] = {
	CG_FRAME(    0,    0,   2,           0,   517, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   2,           0,   518, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   2,           0,   519, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   2,           0,   520, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   6,           0,   521, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   6,           0,   522, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   2,           0,   523, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   2,           0,   258, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   259, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_NEXT( pl14_special_03 ),
};

/* Char_Special 4: fuusetsu_init (Fuusetsu Zan) */
const Sint32 pl14_special_04[] = {
	CG_FRAME(    0,    0,   2,           0,   574, 0x80,   0x00,  ATK,  0x00,       0x14,    0,    0,     0,      0,  93 ),
};

const Sint32 pl14_special_04_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   575, 0x80,   0x00,  ATK,  0x00, 0x14 | CSE,    0,    0,     0,      0,  65 ),
	CG_FRAME(    0,    0,   2,           0,   576, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   2,           0,   577, 0x80,   0x00,  ATK,  0x00, 0x14 | CSE,    0,    0,     0,      0,  65 ),
	CG_FRAME(    0,    0,   2,           0,   578, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   2,           0,   575, 0x80,   0x00,  ATK,  0x01, 0x14 | CSE,    0,    0,     0,      0,  65 ),
	CG_FRAME(    0,    0,   2,           0,   576, 0x80,   0x00,  ATK,  0x01,          0,    0,    0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   2,           0,   577, 0x80,   0x00,  ATK,  0x00, 0x14 | CSE,    0,    0,     0,      0,  65 ),
	CG_FRAME(    0,    0,   2,           0,   578, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   2,           0,   579, 0x80,   0x00,  ATK,  0x01, 0x14 | CSE,    0,    0,     0,      0,  64 ),
	CG_FRAME(    0,    0,   2,           0,   580, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   2,           0,   581, 0x80,   0x00,  ATK,  0x00, 0x14 | CSE,    0,    0,     0,      0,  64 ),
	CG_FRAME(    0,    0,   2,           0,   582, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   2,           0,   583, 0x80,   0x00,  ATK,  0x01, 0x14 | CSE,    0,    0,     0,      0,  61 ),
	CG_FRAME(    0,    0,   2,           0,   584, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   2,           0,   585, 0x80,   0x00,  ATK,  0x00, 0x14 | CSE,    0,    0,     0,      0,  61 ),
	CG_FRAME(    0,    0,   2,           0,   579, 0x80,   0x00,  ATK,  0x01,          0,    0,    0,     0,      0,  64 ),
	CG_FRAME(    0,    0,   2,           0,   580, 0x80,   0x00,  ATK,  0x00, 0x14 | CSE,    0,    0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   2,           0,   581, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  64 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   582, 0x80,   0x00,  ATK,  0x00, 0x14 | CSE,    0,    0,     0,      0,  93 ),
	CG_NEXT( pl14_special_04_loop ),
};

/* Char_Special 5 */
const Sint32 pl14_special_05[] = {
	CG_FRAME(    0,    0,   2,           0,   583, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   2,           0,   584, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   2,           0,   585, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   2,           0,   586, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   6,           0,   587, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   6,           0,   588, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   589, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  93 ),
	CG_NEXT( pl14_special_05 ),
};

/* Char_Special 6 */
const Sint32 pl14_special_06[] = {
	CG_FRAME(    0,    0,   2,           0,   812, 0x80,   0x00,  ATK,  0x00,       0x1b,    0,    0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   2,           0,   813, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   2,           0,   814, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   2,           0,   815, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   2,           0,   816, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   2,           0,   817, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   2,           0,   818, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   819, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  66 ),
	CG_NEXT( pl14_special_06 ),
};

/* Char_Special 7 */
const Sint32 pl14_special_07[] = {
	CG_FRAME(    0,    0,   2,           0,   820, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   2,           0,   821, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   2,           0,   822, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   2,           0,   823, 0x80,   0x00,  ATK,  0x00,       0x1b,    0,    0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   2,           0,   825, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   2,           0,   826, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   827, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  66 ),
	CG_NEXT( pl14_special_07 ),
};

/* Char_Special 8 */
const Sint32 pl14_special_08[] = {
	CG_FRAME(    0,    0,   2,           0,   828, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   2,           0,   829, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   2,           0,   830, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   2,           0,   831, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   2,           0,   832, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   2,           0,   833, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   2,           0,   834, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   835, 0x80,   0x00,  ATK,  0x00,       0x1b,    0,    0,     0,      0,  66 ),
	CG_NEXT( pl14_special_08 ),
};

/* Char_Special 9: shl14_00_hit */
const Sint32 pl14_special_09[] = {
	CG_FRAME(    0,    0,   4,           0,   836, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   837, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   838, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   839, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   840, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   841, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   842, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
};

const Sint32 pl14_special_09_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   843, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   844, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   845, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_NEXT( pl14_special_09_loop ),
};

/* Char_Special 10 */
const Sint32 pl14_special_10[] = {
	CG_FRAME(    0,    0,   2,           0,   535, 0x80,   0x00,  ATK,  0x00,       0x15,    0,    0,     0,      0, 102 ),
	CG_FRAME(    0,    0,   2,           0,   536, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0, 102 ),
	CG_FRAME( 0x80,    1,   2,           0,   537, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  67 ),
	CG_FRAME(    0,    0,   2,           0,   538, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  94 ),
	CG_FRAME( 0x80,    0,   2,           0,   539, 0x80,   0x00,  ATK,  0x00, 0x14 | CSE,    0,    0,     0,      0,  94 ),
	CG_FRAME(    0,    0,   2,           0,   540, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  94 ),
	CG_FRAME( 0x80,    0,   2,           0,   541, 0x80,   0x00,  ATK,  0x00, 0x14 | CSE,    0,    0,     0,      0,  94 ),
	CG_FRAME(    0,    0,   2,           0,   542, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  94 ),
	CG_FRAME( 0x80,    0,   2,           0,   543, 0x80,   0x00,  ATK,  0x00, 0x14 | CSE,    0,    0,     0,      0,  94 ),
	CG_FRAME(    0,    0,   2,           0,   544, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  94 ),
	CG_FRAME( 0x80,    0,   2,           0,   545, 0x80,   0x00,  ATK,  0x00, 0x14 | CSE,    0,    0,     0,      0,  94 ),
	CG_FRAME(    0,    0,   2,           0,   546, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  94 ),
	CG_FRAME( 0x80,    0,   2,           0,   547, 0x80,   0x00,  ATK,  0x00, 0x14 | CSE,    0,    0,     0,      0,  94 ),
	CG_FRAME(    0,    0,   2,           0,   548, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  94 ),
	CG_FRAME( 0x80,    0,   2,           0,   549, 0x80,   0x00,  ATK,  0x00, 0x14 | CSE,    0,    0,     0,      0,  94 ),
	CG_FRAME(    0,    0,   2,           0,   550, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  94 ),
	CG_FRAME(    0,    0,   2,           0,   551, 0x80,   0x00,  ATK,  0x00, 0x14 | CSE,    0,    0,     0,      0,  10 ),
	CG_FRAME(    0,    0,   2,           0,   552, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  10 ),
	CG_FRAME(    0,    0,   2,           0,   553, 0x80,   0x00,  ATE,  0x00, 0x14 | CSE,    0,    0,     0,      0,  10 ),
	CG_FRAME(    0,    0,   2,           0,   554, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  10 ),
};

const Sint32 pl14_special_10_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   144, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  10 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   145, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  10 ),
	CG_NEXT( pl14_special_10_loop ),
};

/* Char_Special 11 */
const Sint32 pl14_special_11[] = {
	CG_FRAME(    0,    0,   1,           0,   110, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  10 ),
	CG_FRAME(    0,    0,   1,           0,   111, 0x80,   0x00,  ATK,  0x00,       0x15,    0,    0,     0,      0,  10 ),
	CG_FRAME(    0,    0,   1,           0,   112, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  10 ),
	CG_FRAME(    0,    0,   1,           0,   113, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  10 ),
	CG_FRAME(    0,    0,   1,           0,   114, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  10 ),
	CG_FRAME(    0,    0,   2,           0,   555, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  10 ),
	CG_FRAME( 0x80,    1,   2,           0,   556, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  67 ),
	CG_FRAME(    0,    0,   2,           0,   557, 0x80,   0x00,  ATK,  0x00, 0x14 | CSE,    0,    0,     0,      0,  94 ),
	CG_FRAME( 0x80,    0,   2,           0,   558, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  94 ),
	CG_FRAME(    0,    0,   2,           0,   559, 0x80,   0x00,  ATK,  0x00, 0x14 | CSE,    0,    0,     0,      0,  94 ),
	CG_FRAME( 0x80,    0,   2,           0,   560, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  94 ),
	CG_FRAME(    0,    0,   2,           0,   561, 0x80,   0x00,  ATK,  0x00, 0x14 | CSE,    0,    0,     0,      0,  94 ),
	CG_FRAME( 0x80,    0,   2,           0,   562, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  10 ),
	CG_FRAME(    0,    0,   2,           0,   563, 0x80,   0x00,  ATK,  0x00, 0x14 | CSE,    0,    0,     0,      0,  10 ),
	CG_FRAME( 0x80,    0,   2,           0,   564, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  94 ),
	CG_FRAME(    0,    0,   2,           0,   565, 0x80,   0x00,  ATK,  0x00, 0x14 | CSE,    0,    0,     0,      0,  94 ),
	CG_FRAME( 0x80,    0,   2,           0,   566, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  94 ),
	CG_FRAME(    0,    0,   2,           0,   567, 0x80,   0x00,  ATK,  0x00, 0x14 | CSE,    0,    0,     0,      0,  94 ),
	CG_FRAME(    0,    0,   2,           0,   568, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  10 ),
	CG_FRAME(    0,    0,   2,           0,   569, 0x80,   0x00,  ATE,  0x00, 0x14 | CSE,    0,    0,     0,      0,  10 ),
	CG_FRAME(    0,    0,   2,           0,   570, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  10 ),
	CG_FRAME(    0,    0,   2,           0,   571, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  10 ),
	CG_FRAME(    0,    0,   2,           0,   572, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  10 ),
	CG_FRAME(    0,    0,   2,           0,   573, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  10 ),
};

const Sint32 pl14_special_11_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   144, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  10 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   145, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  10 ),
	CG_NEXT( pl14_special_11_loop ),
};

/* Char_Special 26 */
const Sint32 pl14_special_26[] = {
	CG_FRAME(    0,    0,   2,           0,   535, 0x80,   0x00,  ATK,  0x00,       0x15,    0,    0,     0,      0, 102 ),
	CG_FRAME(    0,    0,   2,           0,   536, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0, 102 ),
	CG_FRAME( 0x80,    1,   2,           0,   537, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  67 ),
	CG_FRAME(    0,    0,   2,           0,   538, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  94 ),
	CG_FRAME( 0x80,    0,   2,           0,   539, 0x80,   0x00,  ATK,  0x00, 0x14 | CSE,    0,    0,     0,      0,  94 ),
	CG_FRAME(    0,    0,   2,           0,   540, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  94 ),
	CG_FRAME( 0x80,    0,   2,           0,   541, 0x80,   0x00,  ATK,  0x00, 0x14 | CSE,    0,    0,     0,      0,  94 ),
	CG_FRAME(    0,    0,   2,           0,   542, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  94 ),
	CG_FRAME( 0x80,    0,   2,           0,   543, 0x80,   0x00,  ATK,  0x00, 0x14 | CSE,    0,    0,     0,      0,  94 ),
	CG_FRAME(    0,    0,   2,           0,   544, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  94 ),
	CG_FRAME( 0x80,    0,   2,           0,   545, 0x80,   0x00,  ATK,  0x00, 0x14 | CSE,    0,    0,     0,      0,  94 ),
	CG_FRAME(    0,    0,   2,           0,   546, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  94 ),
	CG_FRAME( 0x80,    0,   2,           0,   547, 0x80,   0x00,  ATK,  0x00, 0x14 | CSE,    0,    0,     0,      0,  94 ),
	CG_FRAME(    0,    0,   2,           0,   548, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  94 ),
	CG_FRAME( 0x80,    0,   2,           0,   549, 0x80,   0x00,  ATK,  0x00, 0x14 | CSE,    0,    0,     0,      0,  94 ),
	CG_FRAME(    0,    0,   2,           0,   550, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  94 ),
	CG_FRAME(    0,    0,   2,           0,   551, 0x80,   0x00,  ATK,  0x00, 0x14 | CSE,    0,    0,     0,      0,  10 ),
	CG_FRAME(    0,    0,   2,           0,   552, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  10 ),
	CG_FRAME(    0,    0,   2,           0,   553, 0x80,   0x00,  ATK,  0x00, 0x14 | CSE,    0,    0,     0,      0,  10 ),
	CG_FRAME(    0,    0,   2,           0,   554, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  10 ),
};

const Sint32 pl14_special_26_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   144, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  10 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   145, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  10 ),
	CG_NEXT( pl14_special_26_loop ),
};

/* Char_Special 27 */
const Sint32 pl14_special_27[] = {
	CG_FRAME(    0,    0,   1,           0,   110, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  10 ),
	CG_FRAME(    0,    0,   1,           0,   111, 0x80,   0x00,  ATK,  0x00,       0x15,    0,    0,     0,      0,  10 ),
	CG_FRAME(    0,    0,   1,           0,   112, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  10 ),
	CG_FRAME(    0,    0,   1,           0,   113, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  10 ),
	CG_FRAME(    0,    0,   1,           0,   114, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  10 ),
	CG_FRAME(    0,    0,   2,           0,   555, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  10 ),
	CG_FRAME( 0x80,    1,   2,           0,   556, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  67 ),
	CG_FRAME(    0,    0,   2,           0,   557, 0x80,   0x00,  ATK,  0x00, 0x14 | CSE,    0,    0,     0,      0,  94 ),
	CG_FRAME( 0x80,    0,   2,           0,   558, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  94 ),
	CG_FRAME(    0,    0,   2,           0,   559, 0x80,   0x00,  ATK,  0x00, 0x14 | CSE,    0,    0,     0,      0,  94 ),
	CG_FRAME( 0x80,    0,   2,           0,   560, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  94 ),
	CG_FRAME(    0,    0,   2,           0,   561, 0x80,   0x00,  ATK,  0x00, 0x14 | CSE,    0,    0,     0,      0,  94 ),
	CG_FRAME( 0x80,    0,   2,           0,   562, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  94 ),
	CG_FRAME(    0,    0,   2,           0,   563, 0x80,   0x00,  ATK,  0x00, 0x14 | CSE,    0,    0,     0,      0,  94 ),
	CG_FRAME( 0x80,    0,   2,           0,   564, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  94 ),
	CG_FRAME(    0,    0,   2,           0,   565, 0x80,   0x00,  ATK,  0x00, 0x14 | CSE,    0,    0,     0,      0,  94 ),
	CG_FRAME( 0x80,    0,   2,           0,   566, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  94 ),
	CG_FRAME(    0,    0,   2,           0,   567, 0x80,   0x00,  ATK,  0x00, 0x14 | CSE,    0,    0,     0,      0,  94 ),
	CG_FRAME(    0,    0,   2,           0,   568, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  10 ),
	CG_FRAME(    0,    0,   2,           0,   569, 0x80,   0x00,  ATE,  0x00, 0x14 | CSE,    0,    0,     0,      0,  10 ),
	CG_FRAME(    0,    0,   2,           0,   570, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  10 ),
	CG_FRAME(    0,    0,   2,           0,   571, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  10 ),
	CG_FRAME(    0,    0,   2,           0,   572, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  10 ),
	CG_FRAME(    0,    1,   2,           0,   573, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  10 ),
};

const Sint32 pl14_special_27_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   144, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  10 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   145, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  10 ),
	CG_NEXT( pl14_special_27_loop ),
};

/* Char_Special 12: hofuku_init (Hofuku Zenshin) */
const Sint32 pl14_special_12[] = {
	CG_FRAME(    0,    0,   3,           0,   493, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   3,           0,   494, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,  69 ),
};

const Sint32 pl14_special_12_loop[] = {
	CG_FRAME(    0,    0,   3,           0,   495, 0x80,   0x00,    0,  0x00,          0,    0, 0x80,     0,      0,  69 ),
	CG_FRAME(    0,    0,   3,           0,   496, 0x80,   0x00,    0,  0x00,          0,    0, 0x80,     0,      0,  69 ),
	CG_FRAME(    0,    0,   3,           0,   497, 0x80,   0x00,    0,  0x00,          0,    0, 0x80,     0,      0,  69 ),
	CG_FRAME(    0,    0,   3,           0,   498, 0x80,   0x00,    0,  0x00,          0,    0, 0x80,     0,      0,  69 ),
	CG_FRAME(    0,    0,   3,           0,   499, 0x80,   0x00,    0,  0x00,          0,    0, 0x80,     0,      0,  69 ),
	CG_FRAME(    0,    0,   3,           0,   500, 0x80,   0x00,    0,  0x00,          0,    0, 0x80,     0,      0,  69 ),
	CG_FRAME(    0,    0,   3,           0,   501, 0x80,   0x00,    0,  0x00,          0,    0, 0x80,     0,      0,  69 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   502, 0x80,   0x00,    0,  0x00,          0,    0, 0x80,     0,      0,  69 ),
	CG_NEXT( pl14_special_12_loop ),
};

/* Char_Special 13: hofuku_move (Hofuku Zenshin) */
const Sint32 pl14_special_13[] = {
	CG_FRAME(    0,    0,   3,           0,   503, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   504, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   505, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   506, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   507, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   117, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   118, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl14_special_13 ),
};

/* Char_Special 14
   Char_Special 15: sp_catch_spin */
const Sint32 pl14_special_14[] = {
	CG_FRAME(    0,    0,   4,           0,   446, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   447, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   448, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   449, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   450, 0x80,   0x00,    0,  0x00,       0x21,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   451, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl14_special_14 ),
};

/* Char_Special 30: sp_catch_drop */
const Sint32 pl14_special_30[] = {
	CG_FRAME(    0,    0,   4,           0,   452, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   453, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
};

const Sint32 pl14_special_30_loop[] = {
	CG_FRAME(    0,    1,   5,           0,   454, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   455, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,   456, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl14_special_30_loop ),
};

/* Char_Special 16: sp_catch_hit */
const Sint32 pl14_special_16[] = {
	CG_FRAME(    0,    0,   4,           0,   457, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   458, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   459, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   460, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  32,           0,   461, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   149, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   150, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   151, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   152, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   117, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   121, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   122, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl14_special_16 ),
};

/* Char_Special 17: hisago_move (Kingin no Hisago) */
const Sint32 pl14_special_17[] = {
	CG_FRAME(    0,    0,   4,           0,   462, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4,           0,   463, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4,           0,   464, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4,           0,   465, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4,           0,   466, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4,           0,   467, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4,           0,   468, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4,           0,   466, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4,           0,   467, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4,           0,   468, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4,           0,   465, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4,           0,   442, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4,           0,   441, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4,           0,   440, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   8 ),
};

const Sint32 pl14_special_17_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   254, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_NEXT( pl14_special_17_loop ),
};

/* Char_Dash 4: fly_start_init, fly_start2_init
   Char_Dash 5: appear_init */
const Sint32 pl14_dash_04[] = {
	CG_FRAME(    0,    0,   4,           0,   601, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   602, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   603, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   604, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   6 ),
	CG_FRAME(    0,    1,   4,           0,   605, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   6 ),
};

const Sint32 pl14_dash_04_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   606, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   607, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   606, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   607, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   606, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   6 ),
	CG_FRAME(    0,    2,   4, END_OF_DATA,   607, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   6 ),
	CG_NEXT( pl14_dash_04_loop ),
};

/* Char_Dash 6 */
const Sint32 pl14_dash_06[] = {
	CG_FRAME(    0,    0,   4,           0,   601, 0x80,   0x00,    0,  0x00,          0,   14,    0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   602, 0x80,   0x00,    0,  0x00,          0,   14,    0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   603, 0x80,   0x00,    0,  0x00,          0,   14,    0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   604, 0x80,   0x00,    0,  0x00,          0,   14,    0,     0,      0,   6 ),
	CG_FRAME(    0,    1,   4,           0,   605, 0x80,   0x00,    0,  0x00,          0,   14,    0,     0,      0,   6 ),
};

const Sint32 pl14_dash_06_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   606, 0x80,   0x00,    0,  0x00,          0,   14,    0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   607, 0x80,   0x00,    0,  0x00,          0,   14,    0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   606, 0x80,   0x00,    0,  0x00,          0,   14,    0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   607, 0x80,   0x00,    0,  0x00,          0,   14,    0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   606, 0x80,   0x00,    0,  0x00,          0,   14,    0,     0,      0,   6 ),
	CG_FRAME(    0,    2,   4, END_OF_DATA,   607, 0x80,   0x00,    0,  0x00,          0,   14,    0,     0,      0,   6 ),
	CG_NEXT( pl14_dash_06_loop ),
};

/* Char_Extra 0: t_kick_init */
const Sint32 pl14_extra_00[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   387, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  70 ),
	CG_NEXT( pl14_extra_00 ),
};

/* Char_Extra 1 */
const Sint32 pl14_extra_01[] = {
	CG_FRAME(    0,    0,   4,           0,   372, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   4,           0,   117, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   4,           0,   120, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   4,           0,   121, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,  71 ),
	CG_FRAME(    0,    0,  24,           0,   122, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   122, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,  71 ),
	CG_NEXT( pl14_extra_01 ),
};

/* Char_Extra 2: t_kick_move, fly_start5 */
const Sint32 pl14_extra_02[] = {
	CG_FRAME(    0,    0,   4,           0,   645, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  32,           0,   651, 0x80,   0x00,    0,  0x00,       0x18,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   652, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl14_extra_02 ),
};

/* Char_Extra 3: pl14_upper, fly_start5 */
const Sint32 pl14_extra_03[] = {
	CG_FRAME(    0,    0,   4,           0,   294, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0, 100 ),
	CG_FRAME( 0x80,    1,   2,           0,   295, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0, 100 ),
	CG_FRAME(    0,    0,   3,           0,   296, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  25 ),
	CG_FRAME(    0,    0,  10,           0,   296, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  26 ),
	CG_FRAME(    0,    0,   4,           0,   297, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  26 ),
	CG_FRAME(    0,    0,   4,           0,   298, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   299, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   300, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_NEXT( pl14_extra_03 ),
};

/* Char_Super 0: tenchi_init (Tenchi Tsuukan (hyper)) */
const Sint32 pl14_super_00[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   618, 0x80,   0x00,  ATK,  0x00,       0x18,    0,    0,     0,      0, 104 ),
	CG_NEXT( pl14_super_00 ),
};

/* Char_Super 1: tenchi_move (Tenchi Tsuukan (hyper)) */
const Sint32 pl14_super_01[] = {
	CG_FRAME(    0,    0,   8,           0,   619, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0, 104 ),
	CG_FRAME(    0,    0,   4,           0,   620, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0, 104 ),
};

/* Char_Super 65: tenchi_move (Tenchi Tsuukan (hyper)) */
const Sint32 pl14_super_65[] = {
	CG_FRAME(    0,    1,  28,           0,   621, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0, 104 ),
	CG_FRAME(    0,    0,   4,           0,   622, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0, 104 ),
	CG_FRAME(    0,    0,   4,           0,   623, 0x80,   0x00,  ATE,  0x00,       0x1a,    0,    0,     0,      0, 104 ),
	CG_FRAME(    0,    0,   4,           0,   624, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0, 104 ),
	CG_FRAME(    0,    0,  32,           0,   625, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0, 104 ),
	CG_FRAME(    0,    0,   5,           0,   626, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0, 104 ),
	CG_FRAME(    0,    0,   5,           0,   627, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0, 104 ),
	CG_FRAME(    0,    0,   4,           0,   628, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0, 104 ),
};

const Sint32 pl14_super_65_loop[] = {
	CG_FRAME(    0,    0,  10, END_OF_DATA,   629, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0, 104 ),
	CG_NEXT( pl14_super_65_loop ),
};

/* Char_Super 64: tenchi_init (Tenchi Tsuukan (hyper)) */
const Sint32 pl14_super_64[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   620, 0x80,   0x00,  ATK,  0x00,       0x18,    0,    0,     0,      0, 104 ),
	CG_NEXT( pl14_super_64 ),
};

/* Char_Super 2: shl14_01_init */
const Sint32 pl14_super_02[] = {
	CG_FRAME(    0,    0,   1,           0,   739, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  72 ),
	CG_FRAME(    0,    0,   1,           0,   744, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  72 ),
	CG_FRAME(    0,    0,   1,           0,   743, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  72 ),
	CG_FRAME(    0,    0,   1,           0,   742, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  72 ),
	CG_FRAME(    0,    0,   1,           0,   741, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  72 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   740, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  72 ),
	CG_NEXT( pl14_super_02 ),
};

/* Char_Super 3: shl14_01_wait */
const Sint32 pl14_super_03[] = {
	CG_FRAME(    0,    0,   4,           0, 32811, 0x80,   0xc0,  ATK,  0x00,          0,    0,    0,     0,      0,  73 ),
	CG_FRAME(    0,    1,   4, END_OF_DATA, 32811, 0x80,   0xc0,  ATK,  0x00,          0,    0,    0,     0,      0,  73 ),
	CG_NEXT( pl14_super_03 ),
};

/* Char_Super 4: shl14_01_part_set */
const Sint32 pl14_super_04[] = {
	CG_FRAME(    0,    0,   4,           0, 32812, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  98 ),
	CG_FRAME(    0,    1,   4, END_OF_DATA, 32812, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  98 ),
	CG_NEXT( pl14_super_04 ),
};

/* Char_Super 5: pow_init (Pow (hyper)) */
const Sint32 pl14_super_05[] = {
	CG_FRAME(    0,    0,   4,           0,    87, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4,           0,   631, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   8,           0,   632, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4,           0,   633, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_FRAME( 0x80,    1,   4,           0,   634, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_FRAME( 0x80,    0,   4,           0,   635, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_FRAME( 0x82,    0,   4,           0,   634, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_FRAME( 0x80,    0,   4,           0,   635, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_FRAME( 0x80,    0,   4,           0,   634, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_FRAME( 0x80,    0,   4,           0,   635, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   8 ),
};

/* Char_Super 6: pow_fall (Pow (hyper)) */
const Sint32 pl14_super_06[] = {
	CG_FRAME( 0x80,    0,   4,           0,   636, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_FRAME( 0x80,    0,   4,           0,   117, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_FRAME( 0x80,    0,   4,           0,   118, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   8 ),
};

const Sint32 pl14_super_06_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   119, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_NEXT( pl14_super_06_loop ),
};

/* Char_Super 7: pow_jump_on (Pow (hyper)), shl14_04_set (7 + char_no.b.no), shl14_04_ride (7 + char_no.b.no) */
const Sint32 pl14_super_07[] = {
	CG_FRAME(    0,    0,   4,           0,    97, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4,           0,    98, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4,           0,    99, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4,           0,   100, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4,           0,   101, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4,           0,   102, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4,           0,   103, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4,           0,   104, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   8 ),
};

const Sint32 pl14_super_07_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   105, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_NEXT( pl14_super_07_loop ),
};

/* Char_Super 8: pow_jump (Pow (hyper)) */
const Sint32 pl14_super_08[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   637, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_NEXT( pl14_super_08 ),
};

/* Char_Super 9: pow_ride (Pow (hyper)) */
const Sint32 pl14_super_09[] = {
	CG_FRAME(    0,    0,   4,           0,   638, 0x80,   0x00,  ATE,  0x00,       0x1f,    0,    0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4,           0,   639, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4,           0,   638, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4,           0,   639, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4,           0,   638, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   639, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_NEXT( pl14_super_09 ),
};

/* Char_Super 10: pow_off (Pow (hyper)) */
const Sint32 pl14_super_10[] = {
	CG_FRAME(    0,    0,   4,           0,   640, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4,           0,   641, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4,           0,   642, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4,           0,   643, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   644, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_NEXT( pl14_super_10 ),
};

/* Char_Super 11: pow_slide (Pow (hyper)) */
const Sint32 pl14_super_11[] = {
	CG_FRAME(    0,    0,  16,           0,   645, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   8 ),
};

/* Char_Super 12 */
const Sint32 pl14_super_12[] = {
	CG_FRAME(    0,    0,   4,           0,   646, 0x80,   0x00,  ATE,  0x00,       0x0b,    0,    0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4,           0,   647, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4,           0,   646, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4,           0,   647, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4,           0,   646, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4,           0,   647, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   8 ),
};

/* Char_Super 13 */
const Sint32 pl14_super_13[] = {
	CG_FRAME(    0,    0,   6,           0,   648, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_FRAME(    0,    1,   4,           0,   649, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4,           0,   650, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4,           0,   651, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   8 ),
};

const Sint32 pl14_super_13_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   652, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_NEXT( pl14_super_13_loop ),
};

/* Char_Super 14: shl14_02_init */
const Sint32 pl14_super_14[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   849, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  74 ),
	CG_NEXT( pl14_super_14 ),
};

/* Char_Super 15: shl14_02_throw_set */
const Sint32 pl14_super_15[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   850, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_NEXT( pl14_super_15 ),
};

/* Char_Super 16 */
const Sint32 pl14_super_16[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   851, 0x80,   0xc0,  ATE,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_NEXT( pl14_super_16 ),
};

/* Char_Super 18 */
const Sint32 pl14_super_18[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   852, 0x80,   0xc0,  ATE,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_NEXT( pl14_super_18 ),
};

/* Char_Super 17 */
const Sint32 pl14_super_17[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   853, 0x80,   0xc0,  ATE,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_NEXT( pl14_super_17 ),
};

/* Char_Super 19 */
const Sint32 pl14_super_19[] = {
	CG_FRAME(    0,    0,   4,           0,   854, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   855, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_NEXT( pl14_super_19 ),
};

/* Char_Super 20 */
const Sint32 pl14_super_20[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   854, 0x80,   0xe0,  ATE,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_NEXT( pl14_super_20 ),
};

/* Char_Super 22 */
const Sint32 pl14_super_22[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   855, 0x80,   0xe0,  ATE,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_NEXT( pl14_super_22 ),
};

/* Char_Super 21 */
const Sint32 pl14_super_21[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   856, 0x80,   0xe0,  ATE,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_NEXT( pl14_super_21 ),
};

/* Char_Super 23 */
const Sint32 pl14_super_23[] = {
	CG_FRAME(    0,    0,   4,           0,   863, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   864, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   865, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_NEXT( pl14_super_23 ),
};

/* Char_Super 24 */
const Sint32 pl14_super_24[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   857, 0x80,   0xc0,  ATE,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_NEXT( pl14_super_24 ),
};

/* Char_Super 26 */
const Sint32 pl14_super_26[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   858, 0x80,   0xc0,  ATE,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_NEXT( pl14_super_26 ),
};

/* Char_Super 25 */
const Sint32 pl14_super_25[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   859, 0x80,   0xc0,  ATE,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_NEXT( pl14_super_25 ),
};

/* Char_Super 27 */
const Sint32 pl14_super_27[] = {
	CG_FRAME(    0,    0,   4,           0,   863, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   864, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   865, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_NEXT( pl14_super_27 ),
};

/* Char_Super 28: enou_stand_set (En'Ou (hyper)), enou_auto_stand_set (En'Ou, automatic (team hyper)) */
const Sint32 pl14_super_28[] = {
	CG_FRAME(    0,    0,   4,           0,   653, 0x78,   0xe0,    0,  0x00,          0,    0,    0,     0,      0,  76 ),
	CG_FRAME(    0,    0,   4,           0,   654, 0x78,   0xe0,    0,  0x00,          0,    0,    0,     0,      0,  76 ),
	CG_FRAME(    0,    0,   4,           0,   655, 0x78,   0xe0,    0,  0x00,          0,    0,    0,     0,      0,  76 ),
	CG_FRAME(    0,    0,   4,           0,   656, 0x78,   0xe0,    0,  0x00,          0,    0,    0,     0,      0,  76 ),
	CG_FRAME(    0,    0,   4,           0,   657, 0x78,   0xe0,    0,  0x00,          0,    0,    0,     0,      0,  76 ),
	CG_FRAME(    0,    0,   4,           0,   658, 0x78,   0xe0,    0,  0x00,          0,    0,    0,     0,      0,  76 ),
	CG_FRAME(    0,    0,   4,           0,   659, 0x78,   0xe0,    0,  0x00,          0,    0,    0,     0,      0,  76 ),
	CG_FRAME(    0,    0,   4,           0,   660, 0x78,   0xe0,    0,  0x00,          0,    0,    0,     0,      0,  76 ),
	CG_FRAME(    0,    0,   4,           0,   661, 0x78,   0xe0,    0,  0x00,          0,    0,    0,     0,      0,  76 ),
	CG_FRAME(    0,    0,   4,           0,   662, 0x78,   0xe0,    0,  0x00,          0,    0,    0,     0,      0,  76 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   663, 0x78,   0xe0,    0,  0x00,          0,    0,    0,     0,      0,  76 ),
	CG_NEXT( pl14_super_28 ),
};

/* Char_Super 29: enou_walk_ck (En'Ou (hyper), 29 + jump_dir) */
const Sint32 pl14_super_29[] = {
	CG_FRAME(    0,    0,   8,           0,   664, 0x78,   0xe0,    0,  0x00,          0,    0,    0,     0,      0,  76 ),
	CG_FRAME(    0,    1,   8,           0,   665, 0x78,   0xe0,    0,  0x00,          0,    0,    0,     0,      0,  76 ),
	CG_FRAME(    0,    0,   8,           0,   666, 0x78,   0xe0,    0,  0x00,          0,    0,    0,     0,      0,  76 ),
	CG_FRAME(    0,    0,   8,           0,   667, 0x78,   0xe0,    0,  0x00,          0,    0,    0,     0,      0,  76 ),
	CG_FRAME(    0,    0,   8,           0,   668, 0x78,   0xe0,    0,  0x00,          0,    0,    0,     0,      0,  76 ),
	CG_FRAME(    0,    1,   8,           0,   669, 0x78,   0xe0,    0,  0x00,          0,    0,    0,     0,      0,  76 ),
	CG_FRAME(    0,    0,   8,           0,   670, 0x78,   0xe0,    0,  0x00,          0,    0,    0,     0,      0,  76 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   671, 0x78,   0xe0,    0,  0x00,          0,    0,    0,     0,      0,  76 ),
	CG_NEXT( pl14_super_29 ),
};

/* Char_Super 30 */
const Sint32 pl14_super_30[] = {
	CG_FRAME(    0,    0,   8,           0,   671, 0x78,   0xe0,    0,  0x00,          0,    0,    0,     0,      0,  76 ),
	CG_FRAME(    0,    0,   8,           0,   670, 0x78,   0xe0,    0,  0x00,          0,    0,    0,     0,      0,  76 ),
	CG_FRAME(    0,    1,   8,           0,   669, 0x78,   0xe0,    0,  0x00,          0,    0,    0,     0,      0,  76 ),
	CG_FRAME(    0,    0,   8,           0,   668, 0x78,   0xe0,    0,  0x00,          0,    0,    0,     0,      0,  76 ),
	CG_FRAME(    0,    0,   8,           0,   667, 0x78,   0xe0,    0,  0x00,          0,    0,    0,     0,      0,  76 ),
	CG_FRAME(    0,    0,   8,           0,   666, 0x78,   0xe0,    0,  0x00,          0,    0,    0,     0,      0,  76 ),
	CG_FRAME(    0,    1,   8,           0,   665, 0x78,   0xe0,    0,  0x00,          0,    0,    0,     0,      0,  76 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   664, 0x78,   0xe0,    0,  0x00,          0,    0,    0,     0,      0,  76 ),
	CG_NEXT( pl14_super_30 ),
};

/* Char_Super 31: enou_turn_ck (En'Ou (hyper)), enou_auto_turn_ck (En'Ou, automatic (team hyper)) */
const Sint32 pl14_super_31[] = {
	CG_FRAME(    0,    0,   8,           0,   672, 0x78,   0xe0,    0,  0x00,          0,    0,    0,     0,      0,  76 ),
	CG_FRAME(    0,    0,   8,           0,   673, 0x80,   0xe0,    0,  0x00,       0x78,    0,    0,     0,      0,  76 ),
	CG_FRAME(    0,    0,   8,           0,   674, 0x78,   0xe0,    0,  0x00,          0,    0,    0,     0,      0,  76 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   675, 0x78,   0xe0,    0,  0x00,          0,    0,    0,     0,      0,  76 ),
	CG_NEXT( pl14_super_31 ),
};

/* Char_Super 32: enou_atck_ck (En'Ou (hyper)), enou_auto_atck_ck (En'Ou, automatic (team hyper)) */
const Sint32 pl14_super_32[] = {
	CG_FRAME(    0,    0,   6,           0,   676, 0x78,   0xe0,  ATK,  0x00,          0,    0,    0,     0,      0,  76 ),
	CG_FRAME(    0,    0,   4,           0,   677, 0x78,   0xe0,  ATK,  0x00, 0x17 | CSE,    0,    0,     0,      0,  76 ),
	CG_FRAME(    0,    1,   3,           0,   678, 0x78,   0xe0,  ATK,  0x00,          0,    0,    0,     0,      0,  78 ),
	CG_FRAME(    0,    3,   4,           0,   679, 0x78,   0xe0,  ATK,  0x00,          0,    0,    0,     0,      0, 106 ),
	CG_FRAME(    0,    0,  12,           0,   680, 0x78,   0xe0,  ATE,  0x00,          0,    0,    0,     0,      0,  76 ),
	CG_FRAME(    0,    0,   4,           0,   681, 0x78,   0xe0,  ATE,  0x00,          0,    0,    0,     0,      0,  76 ),
	CG_FRAME(    0,    2,   4, END_OF_DATA,   675, 0x78,   0xe0,  ATE,  0x00,          0,    1,    0,     0,      0,  76 ),
	CG_NEXT( pl14_super_32 ),
};

/* Char_Super 52: enou_atck_ck (En'Ou (hyper)), enou_auto_punch (En'Ou, automatic (team hyper)), enou_auto_atck_ck (En'Ou, automatic (team hyper)) */
const Sint32 pl14_super_52[] = {
	CG_FRAME(    0,    0,   1,           0,   675, 0x78,   0xe0,  ATK,  0x00,          0,    1,    0,     0,      0,  76 ),
	CG_FRAME(    0,    0,   8,           0,   685, 0x78,   0xe0,  ATK,  0x00,          0,    0,    0,     0,      0,  76 ),
	CG_FRAME(    0,    0,   2,           0,   686, 0x78,   0xe0,  ATK,  0x00, 0x17 | CSE,    0,    0,     0,      0,  76 ),
	CG_FRAME(    0,    0,   2,           0,   687, 0x78,   0xe0,  ATK,  0x00,          0,    0,    0,     0,      0,  80 ),
	CG_FRAME(    0,    1,   2,           0,   688, 0x78,   0xe0,  ATK,  0x00,          0,    0,    0,     0,      0, 107 ),
	CG_FRAME(    0,    3,  12,           0,   689, 0x78,   0xe0,  ATE,  0x00,          0,    0,    0,     0,      0,  76 ),
	CG_FRAME(    0,    0,   4,           0,   690, 0x78,   0xe0,  ATE,  0x00,          0,    0,    0,     0,      0,  76 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   675, 0x78,   0xe0,  ATE,  0x00,          0,    1,    0,     0,      0,  76 ),
	CG_NEXT( pl14_super_52 ),
};

/* Char_Super 33: enou_atck_ck (En'Ou (hyper)), enou_auto_start (En'Ou, automatic (team hyper)), enou_auto_atck_ck (En'Ou, automatic (team hyper)) */
const Sint32 pl14_super_33[] = {
	CG_FRAME(    0,    0,   4,           0, 32778, 0x78,   0xe0,  ATK,  0x00,          0,    0,    0,     0,      0,  76 ),
	CG_FRAME(    0,    1,   4, END_OF_DATA, 32778, 0x78,   0xe0,  ATK,  0x00,          0,    0,    0,     0,      0,  76 ),
	CG_NEXT( pl14_super_33 ),
};

/* Char_Super 34
   Char_Super 35
   Char_Super 36
   Char_Super 37: enou_rush_wait (En'Ou (hyper)), enou_auto_rush (En'Ou, automatic (team hyper)) */
const Sint32 pl14_super_34[] = {
	CG_FRAME(    0,    0,  16,           0,   675, 0x78,   0xe0,  ATK,  0x00,          0,    1,    0,     0,      0,  76 ),
	CG_FRAME(    0,    0,  16, END_OF_DATA,   675, 0x78,   0xe0,  ATK,  0x00,          0,    1,    0,     0,      0,  76 ),
	CG_NEXT( pl14_super_34 ),
};

/* Char_Super 38: enou_rush_up (En'Ou (hyper)), enou_auto_rush_top (En'Ou, automatic (team hyper))
   Char_Super 39: enou_rush_top (En'Ou (hyper)), enou_auto_rush_up (En'Ou, automatic (team hyper)) */
const Sint32 pl14_super_38[] = {
	CG_FRAME(    0,    0,   2,           0, 32783, 0x78,   0xe0,  ATK,  0x00,          0,    0,    0,     0,      0,  76 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32784, 0x78,   0xe0,  ATK,  0x00,          0,    0,    0,     0,      0,  76 ),
	CG_NEXT( pl14_super_38 ),
};

/* Char_Super 40: enou_rush_init (En'Ou (hyper)), enou_rush_mid (En'Ou (hyper))
   Char_Super 41: enou_rush_init (En'Ou (hyper)), enou_rush_up (En'Ou (hyper)), enou_rush_down (En'Ou (hyper)), enou_auto_rush_init (En'Ou, automatic (team hyper)), ...
   Char_Super 42: enou_rush_init (En'Ou (hyper)), enou_rush_mid (En'Ou (hyper)) */
const Sint32 pl14_super_40[] = {
	CG_FRAME(    0,    0,   2,           0, 32781, 0x78,   0xe0,  ATK,  0x00,          0,    0,    0,     0,      0,  76 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32782, 0x78,   0xe0,  ATK,  0x00,          0,    0,    0,     0,      0,  76 ),
	CG_NEXT( pl14_super_40 ),
};

/* Char_Super 43: enou_rush_bottom (En'Ou (hyper)), enou_auto_rush_down (En'Ou, automatic (team hyper))
   Char_Super 44: enou_rush_down (En'Ou (hyper)), enou_auto_rush_bottom (En'Ou, automatic (team hyper)) */
const Sint32 pl14_super_43[] = {
	CG_FRAME(    0,    0,   2,           0, 32779, 0x78,   0xe0,  ATK,  0x00,          0,    0,    0,     0,      0,  76 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32780, 0x78,   0xe0,  ATK,  0x00,          0,    0,    0,     0,      0,  76 ),
	CG_NEXT( pl14_super_43 ),
};

/* Char_Super 45 */
const Sint32 pl14_super_45[] = {
	CG_FRAME(    0,    0,   2,           0, 32785, 0x80,   0x00,    0,  0x00,          0,    3,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32786, 0x80,   0x00,    0,  0x00,          0,    3,    0,     0,      0,   0 ),
	CG_NEXT( pl14_super_45 ),
};

/* Char_Super 46
   Char_Super 47 */
const Sint32 pl14_super_46[] = {
	CG_FRAME(    0,    0,   2,           0, 32785, 0x80,   0x00,    0,  0x00,          0,    4,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32786, 0x80,   0x00,    0,  0x00,          0,    4,    0,     0,      0,   0 ),
	CG_NEXT( pl14_super_46 ),
};

/* Char_Super 48 */
const Sint32 pl14_super_48[] = {
	CG_FRAME(    0,    0,   2,           0, 32785, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32786, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_NEXT( pl14_super_48 ),
};

/* Char_Super 49
   Char_Super 50 */
const Sint32 pl14_super_49[] = {
	CG_FRAME(    0,    0,   2,           0, 32785, 0x80,   0x00,    0,  0x00,          0,    5,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32786, 0x80,   0x00,    0,  0x00,          0,    5,    0,     0,      0,   0 ),
	CG_NEXT( pl14_super_49 ),
};

/* Char_Super 51 */
const Sint32 pl14_super_51[] = {
	CG_FRAME(    0,    0,   2,           0, 32785, 0x80,   0x00,    0,  0x00,          0,    6,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32786, 0x80,   0x00,    0,  0x00,          0,    6,    0,     0,      0,   0 ),
	CG_NEXT( pl14_super_51 ),
};

/* Char_Super 53
   Char_Super 54: enou_grow_init */
const Sint32 pl14_super_53[] = {
	CG_FRAME(    0,    0,   4,           0,   373, 0x80,   0x00,    0,  0x00,          0,    9,    0,     0,      0,  10 ),
	CG_FRAME(    0,    0,   4,           0,   374, 0x80,   0x00,    0,  0x00,          0,    9,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   375, 0x80,   0x00,    0,  0x00,          0,    7,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   376, 0x80,   0x00,    0,  0x00,          0,    7,    0,     0,      0,   0 ),
};

const Sint32 pl14_super_53_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   376, 0x80,   0x00,    0,  0x00,          0,    7,    0,     0,      0,   0 ),
	CG_NEXT( pl14_super_53_loop ),
};

/* Char_Super 55: enou_grow_up */
const Sint32 pl14_super_55[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA, 32778, 0x80,   0xe0,  ATK,  0x00,          0,    0,    0,     0,      0,  77 ),
	CG_NEXT( pl14_super_55 ),
};

/* Char_Super 56 */
const Sint32 pl14_super_56[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA,   693, 0x80,   0x00,    0,  0x00,          0,    8,    0,     0,      0,   1 ),
	CG_NEXT( pl14_super_56 ),
};

/* Char_Super 57: pls14_02_aura_init */
const Sint32 pl14_super_57[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA,   693, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl14_super_57 ),
};

/* Char_Super 58: shl14_05_set */
const Sint32 pl14_super_58[] = {
	CG_FRAME(    0,    0,   1,           0, 32787, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32788, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32789, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0, 32790, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
};

/* Char_Super 59 */
const Sint32 pl14_super_59[] = {
	CG_FRAME(    0,    0,   1,           0, 32791, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32792, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32793, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA, 32794, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_NEXT( pl14_super_59 ),
};

/* Char_Super 60: shl14_05_ride */
const Sint32 pl14_super_60[] = {
	CG_FRAME(    0,    0,   1,           0, 32795, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32796, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32797, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA, 32798, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_NEXT( pl14_super_60 ),
};

/* Char_Super 61: enou_shrink_down */
const Sint32 pl14_super_61[] = {
	CG_FRAME(    0,    0,   4,           0,    -1, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  10, END_OF_DATA,   376, 0x80,   0x00,    0,  0x00,          0,    7,    0,     0,      0,   1 ),
	CG_NEXT( pl14_super_61 ),
};

/* Char_Super 62
   Char_Super 63: enou_grow_wait */
const Sint32 pl14_super_62[] = {
	CG_FRAME(    0,    0,   8,           0, 32778, 0x80,   0xe0,  ATK,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    1,  16, END_OF_DATA, 32778, 0x80,   0xe0,  ATK,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_NEXT( pl14_super_62 ),
};

/* Char_Super 66: shl14_06_set */
const Sint32 pl14_super_66[] = {
	CG_FRAME(    0,    1,   8,           0,   850, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  97 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   850, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_NEXT( pl14_super_66 ),
};

/* Char_Super 67: enou_shrink_init */
const Sint32 pl14_super_67[] = {
	CG_FRAME(    0,    0,   8,           0, 32778, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    1,  16, END_OF_DATA, 32778, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_NEXT( pl14_super_62 ),
};

/* Char_Special 18: peach_init (the assist's healing peach) */
const Sint32 pl14_special_18[] = {
	CG_FRAME(    0,    0,   8,           0,   117, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,  92 ),
	CG_FRAME(    0,    0,   8,           0,   118, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,  92 ),
	CG_FRAME(    0,    0,   8,           0,   119, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,  92 ),
	CG_FRAME(    0,    0,   8,           0,   168, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,  92 ),
	CG_FRAME(    0,    1,   8,           0,   169, 0x80,   0x00,    0,  0x00,       0x0a,    0,    0,     0,      0,  92 ),
};

const Sint32 pl14_special_18_loop[] = {
	CG_FRAME(    0,    0,   8,           0,   170, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,  92 ),
	CG_FRAME(    0,    0,   8,           0,   171, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,  92 ),
	CG_FRAME(    0,    0,   8,           0,   172, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,  92 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   173, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,  92 ),
	CG_NEXT( pl14_special_18_loop ),
};

/* Char_Special 19: peach_move (the assist's healing peach) */
const Sint32 pl14_special_19[] = {
	CG_FRAME(    0,    0,   8,           0,   174, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,  92 ),
	CG_FRAME(    0,    0,   8,           0,   175, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,  92 ),
	CG_FRAME(    0,    0,   8,           0,   176, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,  92 ),
	CG_FRAME(    0,    1,   8, END_OF_DATA,   177, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,  92 ),
	CG_NEXT( pl14_special_19 ),
};

/* Char_Special 20: shl14_03_init */
const Sint32 pl14_special_20[] = {
	CG_FRAME(    0,    0,   8,           0,   866, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
};

const Sint32 pl14_special_20_loop[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA,   867, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,  91 ),
	CG_NEXT( pl14_special_20_loop ),
};

/* Char_Special 21 */
const Sint32 pl14_special_21[] = {
	CG_FRAME(    0,    0,  32,           0,   867, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8,           0,   868, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
};

const Sint32 pl14_special_21_loop[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA,   869, 0x80,   0x00,    0,  0x00,          0,    0,    0,     0,      0,   0 ),
	CG_NEXT( pl14_special_21_loop ),
};

/* Char_Special 23: snapback_init (the snapback)
   Char_Special 24: snapback_move (the snapback) */
const Sint32 pl14_special_23[] = {
	CG_FRAME(    0,    0,   8,           0,   272, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_FRAME(    1,   10,   3,           0,   273, 0x80,   0x00,  ATK,  0x00,       0x04,    0,    0,     0,      0,   8 ),
	CG_FRAME(    0,    8,   2,           0,   274, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   2,           0,   275, 0x80,   0x00,  ATK,  0x00, 0x16 | CSE,    0,    0,     0,      0,  58 ),
	CG_FRAME( 0x80,    8,   2,           0,   276, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  21 ),
	CG_FRAME(    0,    0,   2,           0,   277, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   2,           0,   278, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   2,           0,   279, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   2,           0,   280, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   2,           0,   281, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  58 ),
	CG_FRAME(    0,  -26,   2,           0,   282, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   2,           0,   258, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   259, 0x80,   0x00,  ATE,  0x00,          0,    0,    0,     0,      0,   8 ),
	CG_NEXT( pl14_special_23 ),
};

/* Char_Special 25: wall_off_ck (Wall Walk) */
const Sint32 pl14_special_25[] = {
	CG_FRAME(    0,    0,   2,           0,   384, 0x80,   0x00,  ATK,  0x00,       0x04,    0,    0,     0,      0,  10 ),
	CG_FRAME(    0,    0,   2,           0,   385, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  10 ),
};

const Sint32 pl14_special_25_loop[] = {
	CG_FRAME( 0x80,    0,   2, END_OF_DATA,   387, 0x80,   0x00,  ATK,  0x00,          0,    0,    0,     0,      0,  52 ),
	CG_NEXT( pl14_special_25_loop ),
};

/* Char_Special 22: wall_ceil (Wall Walk) */
const Sint32 pl14_special_22[] = {
	CG_FRAME(    0,    0,   3,           0,    68, 0x80,   0x00,    0,  0x00,          0,   13,    0,     0,      0, 110 ),
	CG_FRAME(    0,    0,   3,           0,    69, 0x80,   0x00,    0,  0x00,          0,   13,    0,     0,      0, 110 ),
	CG_FRAME(    0,    0,   3,           0,    70, 0x80,   0x00,    0,  0x00,          0,   13,    0,     0,      0, 110 ),
	CG_FRAME(    0,    0,   3,           0,    71, 0x80,   0x00,    0,  0x00,          0,   13,    0,     0,      0, 110 ),
	CG_FRAME(    0,    0,   3,           0,    72, 0x80,   0x00,    0,  0x00,          0,   13,    0,     0,      0, 110 ),
	CG_FRAME(    0,    0,   3,           0,    65, 0x80,   0x00,    0,  0x00,          0,   13,    0,     0,      0, 110 ),
	CG_FRAME(    0,    0,   3,           0,    66, 0x80,   0x00,    0,  0x00,          0,   13,    0,     0,      0, 110 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,    67, 0x80,   0x00,    0,  0x00,          0,   13,    0,     0,      0, 110 ),
	CG_NEXT( pl14_special_22 ),
};

/* Char_Special 28: wall_wall (Wall Walk) */
const Sint32 pl14_special_28[] = {
	CG_FRAME(    0,    0,   3,           0,    68, 0x80,   0x00,    0,  0x00,          0,   11,    0,     0,      0, 108 ),
	CG_FRAME(    0,    0,   3,           0,    69, 0x80,   0x00,    0,  0x00,          0,   11,    0,     0,      0, 108 ),
	CG_FRAME(    0,    0,   3,           0,    70, 0x80,   0x00,    0,  0x00,          0,   11,    0,     0,      0, 108 ),
	CG_FRAME(    0,    0,   3,           0,    71, 0x80,   0x00,    0,  0x00,          0,   11,    0,     0,      0, 108 ),
	CG_FRAME(    0,    0,   3,           0,    72, 0x80,   0x00,    0,  0x00,          0,   11,    0,     0,      0, 108 ),
	CG_FRAME(    0,    0,   3,           0,    65, 0x80,   0x00,    0,  0x00,          0,   11,    0,     0,      0, 108 ),
	CG_FRAME(    0,    0,   3,           0,    66, 0x80,   0x00,    0,  0x00,          0,   11,    0,     0,      0, 108 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,    67, 0x80,   0x00,    0,  0x00,          0,   11,    0,     0,      0, 108 ),
	CG_NEXT( pl14_special_28 ),
};

/* Char_Special 29: wall_wall2 (Wall Walk) */
const Sint32 pl14_special_29[] = {
	CG_FRAME(    0,    0,   3,           0,    68, 0x80,   0x00,    0,  0x00,          0,   12,    0,     0,      0, 109 ),
	CG_FRAME(    0,    0,   3,           0,    69, 0x80,   0x00,    0,  0x00,          0,   12,    0,     0,      0, 109 ),
	CG_FRAME(    0,    0,   3,           0,    70, 0x80,   0x00,    0,  0x00,          0,   12,    0,     0,      0, 109 ),
	CG_FRAME(    0,    0,   3,           0,    71, 0x80,   0x00,    0,  0x00,          0,   12,    0,     0,      0, 109 ),
	CG_FRAME(    0,    0,   3,           0,    72, 0x80,   0x00,    0,  0x00,          0,   12,    0,     0,      0, 109 ),
	CG_FRAME(    0,    0,   3,           0,    65, 0x80,   0x00,    0,  0x00,          0,   12,    0,     0,      0, 109 ),
	CG_FRAME(    0,    0,   3,           0,    66, 0x80,   0x00,    0,  0x00,          0,   12,    0,     0,      0, 109 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,    67, 0x80,   0x00,    0,  0x00,          0,   12,    0,     0,      0, 109 ),
	CG_NEXT( pl14_special_29 ),
};

/* group 27 0 */
const Sint32 pl14_grp27_00[] = {
	EF_FRAME(    0,    0,   1,           0,   761 ),
	EF_FRAME(    0,    1,   1,           0,   762 ),
	EF_FRAME(    0,    0,   1,           0,   763 ),
	EF_FRAME(    0,    0,   1,           0,   764 ),
	EF_FRAME(    0,    0,   1,           0,   765 ),
	EF_FRAME(    0,    0,   1,           0,   766 ),
	EF_FRAME(    0,    0,   1, END_OF_DATA,   767 ),
	CG_NEXT( pl14_grp27_00 ),
};

/* group 27 1 */
const Sint32 pl14_grp27_01[] = {
	EF_FRAME(    0,    0,   1,           0,   768 ),
	EF_FRAME(    0,    1,   1,           0,   769 ),
	EF_FRAME(    0,    0,   1,           0,   770 ),
	EF_FRAME(    0,    0,   1,           0,   771 ),
	EF_FRAME(    0,    0,   1,           0,   772 ),
	EF_FRAME(    0,    0,   1,           0,   773 ),
	EF_FRAME(    0,    0,   1, END_OF_DATA,   774 ),
	CG_NEXT( pl14_grp27_01 ),
};

/* group 27 2 */
const Sint32 pl14_grp27_02[] = {
	EF_FRAME(    0,    0,   1,           0,   775 ),
	EF_FRAME(    0,    1,   1,           0,   776 ),
	EF_FRAME(    0,    0,   1,           0,   777 ),
	EF_FRAME(    0,    0,   1,           0,   778 ),
	EF_FRAME(    0,    0,   1,           0,   779 ),
	EF_FRAME(    0,    0,   1,           0,   780 ),
	EF_FRAME(    0,    0,   1, END_OF_DATA,   781 ),
	CG_NEXT( pl14_grp27_02 ),
};

/* group 27 3 */
const Sint32 pl14_grp27_03[] = {
	EF_FRAME(    0,    0,   1,           0,   739 ),
	EF_FRAME(    0,    0,   1,           0,   744 ),
	EF_FRAME(    0,    0,   1,           0,   743 ),
	EF_FRAME(    0,    0,   1,           0,   742 ),
	EF_FRAME(    0,    0,   1,           0,   741 ),
	EF_FRAME(    0,    0,   1, END_OF_DATA,   740 ),
	CG_NEXT( pl14_grp27_03 ),
};

/* group 27 4 */
const Sint32 pl14_grp27_04[] = {
	EF_FRAME(    0,    0,   1,           0,   745 ),
	EF_FRAME(    0,    0,   1,           0,   746 ),
	EF_FRAME(    0,    0,   1,           0,   747 ),
	EF_FRAME(    0,    0,   2,           0,   748 ),
	EF_FRAME(    0,    0,   3,           0,   749 ),
	EF_FRAME(    0,    0,   4,           0,   750 ),
	EF_FRAME(    0,    0,   4,           0,   751 ),
	EF_FRAME(    0,    0,   4, END_OF_DATA,   752 ),
	CG_NEXT( pl14_grp27_04 ),
};

/* group 27 5 */
const Sint32 pl14_grp27_05[] = {
	EF_FRAME(    0,    0,   1,           0,   753 ),
	EF_FRAME(    0,    0,   1,           0,   754 ),
	EF_FRAME(    0,    0,   1,           0,   755 ),
	EF_FRAME(    0,    0,   2,           0,   756 ),
	EF_FRAME(    0,    0,   3,           0,   757 ),
	EF_FRAME(    0,    0,   4,           0,   758 ),
	EF_FRAME(    0,    0,   4,           0,   759 ),
	EF_FRAME(    0,    0,   4, END_OF_DATA,   760 ),
	CG_NEXT( pl14_grp27_05 ),
};

/* group 27 6 */
const Sint32 pl14_grp27_06[] = {
	EF_FRAME(    0,    0,   1,           0,   782 ),
	EF_FRAME(    0,    0,   1,           0,   783 ),
	EF_FRAME(    0,    0,   1,           0,   784 ),
	EF_FRAME(    0,    0,   1, END_OF_DATA,   785 ),
	CG_NEXT( pl14_grp27_06 ),
};

/* group 27 7 */
const Sint32 pl14_grp27_07[] = {
	EF_FRAME(    0,    0,   1,           0,   786 ),
	EF_FRAME(    0,    0,   1,           0,   787 ),
	EF_FRAME(    0,    0,   1,           0,   788 ),
	EF_FRAME(    0,    0,   1, END_OF_DATA,   789 ),
	CG_NEXT( pl14_grp27_07 ),
};

/* group 27 8 */
const Sint32 pl14_grp27_08[] = {
	EF_FRAME(    0,    0,   1,           0,   790 ),
	EF_FRAME(    0,    0,   1,           0,   791 ),
	EF_FRAME(    0,    0,   1,           0,   792 ),
	EF_FRAME(    0,    0,   1, END_OF_DATA,   793 ),
	CG_NEXT( pl14_grp27_08 ),
};

/* group 27 9 */
const Sint32 pl14_grp27_09[] = {
	EF_FRAME(    0,    0,   4,           0,   486 ),
	EF_FRAME(    0,    0,   4,           0,   487 ),
	EF_FRAME(    0,    0,   4,           0,   488 ),
	EF_FRAME(    0,    0,   4,           0,   489 ),
	EF_FRAME(    0,    0,   4,           0,   490 ),
	EF_FRAME(    0,    0,   4,           0,   491 ),
	EF_FRAME(    0,    0,   4,           0,   492 ),
	EF_FRAME(    0,    0,   5,           0,   475 ),
	EF_FRAME(    0,    0,   5,           0,   476 ),
	EF_FRAME(    0,    0,   5,           0,   477 ),
	EF_FRAME(    0,    0,   5,           0,   483 ),
	EF_FRAME(    0,    0,   5,           0,   478 ),
	EF_FRAME(    0,    0,   5,           0,   479 ),
	EF_FRAME(    0,    0,   5,           0,   480 ),
	EF_FRAME(    0,    0,   5,           0,   481 ),
	EF_FRAME(    0,    0,   5,           0,   482 ),
	EF_FRAME(    0,    0,   5,           0,   483 ),
	EF_FRAME(    0,    0,   5,           0,   484 ),
	EF_FRAME(    0,    0,   5,           0,   485 ),
	EF_FRAME(    0,    0,   5,           0,   480 ),
	EF_FRAME(    0,    0,   5,           0,   481 ),
	EF_FRAME(    0,    0,   5,           0,   482 ),
	EF_FRAME(    0,    0,   5,           0,   483 ),
	EF_FRAME(    0,    1,   5,           0,   484 ),
};

const Sint32 pl14_grp27_09_loop[] = {
	EF_FRAME(    0,    0,   5, END_OF_DATA,   485 ),
	CG_NEXT( pl14_grp27_09_loop ),
};

/* group 27 10 */
const Sint32 pl14_grp27_10[] = {
	EF_FRAME(    0,    0,   4,           0,   794 ),
	EF_FRAME(    0,    0,   4,           0,   795 ),
	EF_FRAME(    0,    0,   4,           0,   796 ),
	EF_FRAME(    0,    0,   4,           0,   797 ),
	EF_FRAME(    0,    0,   4,           0,   798 ),
	EF_FRAME(    0,    0,   4, END_OF_DATA,   799 ),
	CG_NEXT( pl14_grp27_10 ),
};

/* group 27 11
   group 27 12 */
const Sint32 pl14_grp27_11[] = {
	EF_FRAME(    0,    0,   2,           0,   808 ),
	EF_FRAME(    0,    0,   2,           0,   809 ),
	EF_FRAME(    0,    0,   2,           0,   810 ),
};

const Sint32 pl14_grp27_11_loop[] = {
	EF_FRAME(    0,    1,   2, END_OF_DATA,   811 ),
	CG_NEXT( pl14_grp27_11_loop ),
};

/* group 27 13 */
const Sint32 pl14_grp27_13[] = {
	EF_FRAME(    0,    0,   8,           0, 32768 ),
};

const Sint32 pl14_grp27_13_loop[] = {
	EF_FRAME(    0,    0,   8,           0, 32769 ),
	EF_FRAME(    0,    0,   8,           0, 32770 ),
	EF_FRAME(    0,    0,   8,           0, 32771 ),
	EF_FRAME(    0,    0,   8, END_OF_DATA, 32772 ),
	CG_NEXT( pl14_grp27_13_loop ),
};

/* group 27 14 */
const Sint32 pl14_grp27_14[] = {
	EF_FRAME(    0,    0,   8,           0, 32777 ),
};

const Sint32 pl14_grp27_14_loop[] = {
	EF_FRAME(    0,    0,   8,           0, 32773 ),
	EF_FRAME(    0,    0,   8,           0, 32774 ),
	EF_FRAME(    0,    0,   8,           0, 32775 ),
	EF_FRAME(    0,    0,   8, END_OF_DATA, 32776 ),
	CG_NEXT( pl14_grp27_14_loop ),
};

/* group 27 15 */
const Sint32 pl14_grp27_15[] = {
	EF_FRAME(    0,    0,   2,           0,   608 ),
	EF_FRAME(    0,    0,   2,           0,   609 ),
	EF_FRAME(    0,    1,   2,           0,   610 ),
};

const Sint32 pl14_grp27_15_loop[] = {
	EF_FRAME(    0,    0,   2,           0,   611 ),
	EF_FRAME(    0,    0,   2,           0,   612 ),
	EF_FRAME(    0,    0,   2,           0,   613 ),
	EF_FRAME(    0,    0,   2,           0,   614 ),
	EF_FRAME(    0,    0,   2,           0,   615 ),
	EF_FRAME(    0,    0,   2,           0,   616 ),
	EF_FRAME(    0,    0,   2,           0,   617 ),
	EF_FRAME(    0,    0,   2, END_OF_DATA,   610 ),
	CG_NEXT( pl14_grp27_15_loop ),
};

/* group 27 16 */
const Sint32 pl14_grp27_16[] = {
	EF_FRAME(    0,    0,   4,           0,   860 ),
	EF_FRAME(    0,    0,   4,           0,   861 ),
	EF_FRAME(    0,    0,   4,           0,   862 ),
	EF_FRAME(    0,    0,   4,           0,   860 ),
	EF_FRAME(    0,    0,   4,           0,   861 ),
	EF_FRAME(    0,    0,   4, END_OF_DATA,   862 ),
	CG_NEXT( pl14_grp27_16 ),
};

/* group 27 17
   group 27 18 */
const Sint32 pl14_grp27_17[] = {
	EF_FRAME(    0,    0,   4,           0,   863 ),
	EF_FRAME(    0,    0,   4,           0,   864 ),
	EF_FRAME(    0,    0,   4,           0,   865 ),
	EF_FRAME(    0,    0,   4,           0,   863 ),
	EF_FRAME(    0,    0,   4,           0,   864 ),
	EF_FRAME(    0,    0,   4, END_OF_DATA,   865 ),
	CG_NEXT( pl14_grp27_17 ),
};

/* group 27 19 */
const Sint32 pl14_grp27_19[] = {
	EF_FRAME(    0,    0,   3,           0,    -1 ),
	EF_FRAME(    0,    0,   3,           0,    -1 ),
	EF_FRAME(    0,    0,   3,           0,    -1 ),
	EF_FRAME(    0,    0,   3,           0,   886 ),
	EF_FRAME(    0,    0,   3,           0,   887 ),
	EF_FRAME(    0,    0,   3,           0,   888 ),
	EF_FRAME(    0,    0,   3,           0,   889 ),
	EF_FRAME(    0,    0,   3,           0,   890 ),
	EF_FRAME(    0,    0,   3,           0,   891 ),
	EF_FRAME(    0,    0,   3,           0,   892 ),
	EF_FRAME(    0,    0,   3,           0,   870 ),
	EF_FRAME(    0,    0,   3,           0,   871 ),
	EF_FRAME(    0,    0,   3,           0,   872 ),
	EF_FRAME(    0,    0,   3,           0,   873 ),
	EF_FRAME(    0,    0,   3,           0,   874 ),
	EF_FRAME(    0,    0,   3,           0,   875 ),
	EF_FRAME(    0,    0,   3,           0,   876 ),
	EF_FRAME(    0,    0,   3,           0,   877 ),
	EF_FRAME(    0,    0,   3,           0,   878 ),
	EF_FRAME(    0,    0,   3,           0,   879 ),
	EF_FRAME(    0,    0,   3,           0,   880 ),
	EF_FRAME(    0,    0,   3,           0,   881 ),
	EF_FRAME(    0,    0,   3,           0,   882 ),
	EF_FRAME(    0,    0,   3,           0,   883 ),
	EF_FRAME(    0,    0,   3,           0,   893 ),
	EF_FRAME(    0,    0,   3,           0,   894 ),
	EF_FRAME(    0,    0,   3,           0,   895 ),
	EF_FRAME(    0,    0,   3,           0,   896 ),
	EF_FRAME(    0,    0,   3,           0,   897 ),
	EF_FRAME(    0,    0,   3,           0,   898 ),
	EF_FRAME(    0,    0,   3,           0,   899 ),
	EF_FRAME(    0,    0,   3,           0,   900 ),
	EF_FRAME(    0,    0,   3,           0,   901 ),
	EF_FRAME(    0,    0,   3,           0,   902 ),
	EF_FRAME(    0,    0,   3,           0,   903 ),
	EF_FRAME(    0,    0,   3,           0,   904 ),
	EF_FRAME(    0,    0,   3,           0,   905 ),
	EF_FRAME(    0,    0,   3,           0,   906 ),
	EF_FRAME(    0,    0,   3,           0,   907 ),
	EF_FRAME(    0,    0,   3,           0,   908 ),
	EF_FRAME(    0,    0,   3,           0,   909 ),
	EF_FRAME(    0,    0,   3,           0,   910 ),
	EF_FRAME(    0,    0,   3,           0,   911 ),
	EF_FRAME(    0,    0,   3,           0,   912 ),
	EF_FRAME(    0,    0,   3,           0,   913 ),
	EF_FRAME(    0,    0,   3,           0,   914 ),
	EF_FRAME(    0,    0,   3,           0,   915 ),
	EF_FRAME(    0,    0,   3,           0,   916 ),
	EF_FRAME(    0,    0,   3,           0,   917 ),
	EF_FRAME(    0,    0,   3, END_OF_DATA,   918 ),
	CG_NEXT( pl14_grp27_19 ),
};

/* group 27 20 */
const Sint32 pl14_grp27_20[] = {
	EF_FRAME(    0,    0,   3,           0,    -1 ),
	EF_FRAME(    0,    0,   3,           0,    -1 ),
	EF_FRAME(    0,    0,   3,           0,    -1 ),
	EF_FRAME(    0,    0,   3,           0,   935 ),
	EF_FRAME(    0,    0,   3,           0,   936 ),
	EF_FRAME(    0,    0,   3,           0,   937 ),
	EF_FRAME(    0,    0,   3,           0,   938 ),
	EF_FRAME(    0,    0,   3,           0,   939 ),
	EF_FRAME(    0,    0,   3,           0,   940 ),
	EF_FRAME(    0,    0,   3,           0,   941 ),
	EF_FRAME(    0,    0,   3,           0,   919 ),
	EF_FRAME(    0,    0,   3,           0,   920 ),
	EF_FRAME(    0,    0,   3,           0,   921 ),
	EF_FRAME(    0,    0,   3,           0,   922 ),
	EF_FRAME(    0,    0,   3,           0,   923 ),
	EF_FRAME(    0,    0,   3,           0,   924 ),
	EF_FRAME(    0,    0,   3,           0,   925 ),
	EF_FRAME(    0,    0,   3,           0,   926 ),
	EF_FRAME(    0,    0,   3,           0,   927 ),
	EF_FRAME(    0,    0,   3,           0,   928 ),
	EF_FRAME(    0,    0,   3,           0,   929 ),
	EF_FRAME(    0,    0,   3,           0,   930 ),
	EF_FRAME(    0,    0,   3,           0,   931 ),
	EF_FRAME(    0,    0,   3,           0,   932 ),
	EF_FRAME(    0,    0,   3,           0,   942 ),
	EF_FRAME(    0,    0,   3,           0,   943 ),
	EF_FRAME(    0,    0,   3,           0,   944 ),
	EF_FRAME(    0,    0,   3,           0,   945 ),
	EF_FRAME(    0,    0,   3,           0,   946 ),
	EF_FRAME(    0,    0,   3,           0,   947 ),
	EF_FRAME(    0,    0,   3,           0,   948 ),
	EF_FRAME(    0,    0,   3,           0,   949 ),
	EF_FRAME(    0,    0,   3,           0,   950 ),
	EF_FRAME(    0,    0,   3,           0,   951 ),
	EF_FRAME(    0,    0,   3,           0,   952 ),
	EF_FRAME(    0,    0,   3,           0,   953 ),
	EF_FRAME(    0,    0,   3,           0,   954 ),
	EF_FRAME(    0,    0,   3,           0,   955 ),
	EF_FRAME(    0,    0,   3,           0,   956 ),
	EF_FRAME(    0,    0,   3,           0,   957 ),
	EF_FRAME(    0,    0,   3,           0,   958 ),
	EF_FRAME(    0,    0,   3,           0,   959 ),
	EF_FRAME(    0,    0,   3,           0,   960 ),
	EF_FRAME(    0,    0,   3,           0,   961 ),
	EF_FRAME(    0,    0,   3,           0,   962 ),
	EF_FRAME(    0,    0,   3,           0,   963 ),
	EF_FRAME(    0,    0,   3,           0,   964 ),
	EF_FRAME(    0,    0,   3,           0,   965 ),
	EF_FRAME(    0,    0,   3,           0,   966 ),
	EF_FRAME(    0,    0,   3,           0,   967 ),
	EF_FRAME(    0,    0,   3,           0,   968 ),
};

const Sint32 pl14_grp27_20_loop[] = {
	EF_FRAME(    0,    0,   3, END_OF_DATA,   969 ),
	CG_NEXT( pl14_grp27_20_loop ),
};

/* group 27 21 */
const Sint32 pl14_grp27_21[] = {
	EF_FRAME(    0,    0,   4,           0,   846 ),
	EF_FRAME(    0,    0,   4,           0,   847 ),
	EF_FRAME(    0,    0,   4,           0,   848 ),
	EF_FRAME(    0,    0,   4, END_OF_DATA,   847 ),
	CG_NEXT( pl14_grp27_21 ),
};

/* group 27 22 */
const Sint32 pl14_grp27_22[] = {
	EF_FRAME(    0,    0,   3, END_OF_DATA,   682 ),
	CG_NEXT( pl14_grp27_22 ),
};

/* group 27 23 */
const Sint32 pl14_grp27_23[] = {
	EF_FRAME(    0,    0,   3, END_OF_DATA,   683 ),
	CG_NEXT( pl14_grp27_23 ),
};

/* group 27 24 */
const Sint32 pl14_grp27_24[] = {
	EF_FRAME(    0,    0,   3, END_OF_DATA,   684 ),
	CG_NEXT( pl14_grp27_24 ),
};

/* group 27 25 */
const Sint32 pl14_grp27_25[] = {
	EF_FRAME(    0,    0,   4, END_OF_DATA, 32778 ),
	CG_NEXT( pl14_grp27_25 ),
};

/* group 27 26
   group 27 27
   group 27 28
   group 27 29
   group 27 30
   group 27 31 */
const Sint32 pl14_grp27_26[] = {
	EF_FRAME(    0,    0,   4, END_OF_DATA,   411 ),
	CG_NEXT( pl14_grp27_26 ),
};

/* group 27 32 */
const Sint32 pl14_grp27_32[] = {
	EF_FRAME(    0,    0,   3, END_OF_DATA,   691 ),
	CG_NEXT( pl14_grp27_32 ),
};

/* group 27 33 */
const Sint32 pl14_grp27_33[] = {
	EF_FRAME(    0,    0,   3, END_OF_DATA,   692 ),
	CG_NEXT( pl14_grp27_33 ),
};

/* group 27 34: pls14_02_puff_init */
const Sint32 pl14_grp27_34[] = {
	EF_FRAME(    0,    0,   3,           0,   799 ),
	EF_FRAME(    0,    0,   3,           0,   798 ),
	EF_FRAME(    0,    0,   3,           0,   797 ),
	EF_FRAME(    0,    0,   3,           0,   796 ),
	EF_FRAME(    0,    0,   3,           0,   795 ),
	EF_FRAME(    0,    0,   3, END_OF_DATA,   794 ),
	CG_NEXT( pl14_grp27_34 ),
};

/* group 27 35: pls14_00_catch_wait (35 + cg_type), pls14_00_catch_move (35 + cg_type) */
const Sint32 pl14_grp27_35[] = {
	EF_FRAME(    0,    0,  40, END_OF_DATA,   974 ),
	CG_NEXT( pl14_grp27_35 ),
};

/* group 27 36 */
const Sint32 pl14_grp27_36[] = {
	EF_FRAME(    0,    0,   4,           0,   800 ),
};

/* group 27 37 */
const Sint32 pl14_grp27_37[] = {
	EF_FRAME(    0,    0,   4,           0,   800 ),
};

/* group 27 38 */
const Sint32 pl14_grp27_38[] = {
	EF_FRAME(    0,    0,   4,           0,   803 ),
};

/* group 27 39 */
const Sint32 pl14_grp27_39[] = {
	EF_FRAME(    0,    0,   4,           0,   802 ),
};

/* group 27 40 */
const Sint32 pl14_grp27_40[] = {
	EF_FRAME(    0,    0,   4,           0,   801 ),
};

/* group 27 41 */
const Sint32 pl14_grp27_41[] = {
	EF_FRAME(    0,    0,   4, END_OF_DATA,   800 ),
	CG_NEXT( pl14_grp27_26 ),
};
