/* Silver Samurai (character 2f): the animations. PLAYER_WORK's char_adrs points here.

   spr_prm_2f      a sprite's sizes and turn (SPR_PRM): a frame's zoom number picks a row, 0 for none.
   char_tbl_2f     the groups: pl_char_set( pl_w, group, no ) takes the group's table from here and
                   starts its animation no. The character's module (s_pl2f.c) says which group
                   and number each move uses.
   pl2f_GROUP_tbl  a group's animations. GROUP is the group's name in player.h (stand for
                   Char_Stand), or grpNN for a group with no name there.
   pl2f_GROUP_NN   an animation, named by the first group and number that has it: its frames in
                   order. The comment before it lists its places in the groups and, for each,
                   the routines that start it by that number, with their special move;
                   (2 + atck_level) is a routine that counts on from 2 by the attack's level.
                   A frame with END_OF_DATA is the last, and the word after it is the frame the
                   animation goes on from (CG_NEXT; pl2f_GROUP_NN_loop where that is no
                   animation's first frame). An effect's animation has short frames (EF_FRAME)
                   and may end with no word.
   pl2f_unused_NN  frames that no table or animation reaches.

   A player's frame (CG_FRAME, the 0x14 bytes of CHAR_DATA), with the names Capcom's tables used:
     prm, type  a number and a code for the character's module (a speed, an attack, a voice)
     ctr        the game frames it is shown for
     end        END_OF_DATA on the last frame
     spr        the sprite, -1 for none
     prio       added to the draw priority (a signed byte)
     shadow     the shadow's offset (the low five bits) and size, 0xff none
     atck       ATK while an attack is on its way or can hit, ATE in its frames after that;
                CG_ATCK_NG lets nothing cancel it
     flag1      a number for the module: most often the attack, a row of hit_dt_2f
     se         a voice (1 to 63), or with CSE a sound effect; 0 none
     zoom       the row of spr_prm_2f, 0 none
     unk        a byte the program is not seen to read
     flag2      a second code for the character's effects
     rival      where a held opponent is put (the low byte), and a code for the module (the high)
     box        the frame's row of frame_box_2f */
#include "player.h"
#include "char_tbl.h"

extern const Sint32 pl2f_stand_00[], pl2f_stand_02[], pl2f_stand_03[], pl2f_stand_06[],
	pl2f_jump_00[], pl2f_jump_09[], pl2f_jump_01[], pl2f_jump_10[], pl2f_jump_02[], pl2f_jump_11[],
	pl2f_jump_03[], pl2f_jump_04[], pl2f_jump_05[], pl2f_dash_00[], pl2f_dash_01[], pl2f_dash_03[],
	pl2f_crouch_00[], pl2f_crouch_01[], pl2f_crouch_02[], pl2f_turn_00[], pl2f_turn_01[],
	pl2f_guard_00[], pl2f_guard_01[], pl2f_guard_02[], pl2f_guard_04[], pl2f_guard_05[],
	pl2f_guard_06[], pl2f_air_guard_00[], pl2f_air_guard_01[], pl2f_air_guard_02[],
	pl2f_air_guard_03[], pl2f_air_guard_04[], pl2f_special_37[], pl2f_special_37_loop[],
	pl2f_stand_p_00[], pl2f_stand_p_00_loop[], pl2f_stand_p_03[], pl2f_stand_p_03_loop[],
	pl2f_stand_p_01[], pl2f_stand_p_01_loop[], pl2f_stand_p_02[], pl2f_stand_p_02_loop[],
	pl2f_stand_p_04[], pl2f_stand_p_04_loop[], pl2f_stand_k_00[], pl2f_stand_k_00_loop[],
	pl2f_stand_k_01[], pl2f_stand_k_01_loop[], pl2f_extra_06[], pl2f_extra_06_loop[],
	pl2f_stand_k_02[], pl2f_stand_k_02_loop[], pl2f_crouch_p_00[], pl2f_crouch_p_00_loop[],
	pl2f_crouch_p_03[], pl2f_crouch_p_03_loop[], pl2f_crouch_p_01[], pl2f_crouch_p_01_loop[],
	pl2f_crouch_p_02[], pl2f_crouch_p_02_loop[], pl2f_extra_07[], pl2f_extra_07_loop[],
	pl2f_extra_08[], pl2f_extra_08_loop[], pl2f_crouch_k_00[], pl2f_crouch_k_00_loop[],
	pl2f_crouch_k_01[], pl2f_crouch_k_01_loop[], pl2f_crouch_k_02[], pl2f_crouch_k_02_loop[],
	pl2f_jump_p_00[], pl2f_jump_p_00_loop[], pl2f_special_38[], pl2f_jump_p_01[],
	pl2f_jump_p_01_loop[], pl2f_jump_p_02[], pl2f_jump_p_02_loop[], pl2f_jump_k_00[],
	pl2f_jump_k_00_loop[], pl2f_jump_k_01[], pl2f_jump_k_01_loop[], pl2f_jump_k_02[],
	pl2f_jump_k_02_loop[], pl2f_damage_00[], pl2f_damage_01[], pl2f_damage_02[], pl2f_damage_03[],
	pl2f_damage_04[], pl2f_damage_05[], pl2f_damage_06[], pl2f_damage_07[], pl2f_damage_08[],
	pl2f_damage_09[], pl2f_damage_09_loop[], pl2f_damage_10[], pl2f_damage_11[], pl2f_damage_12[],
	pl2f_damage_13[], pl2f_damage_16[], pl2f_damage_17[], pl2f_damage_18[], pl2f_damage_19[],
	pl2f_damage_20[], pl2f_damage_21[], pl2f_damage_22[], pl2f_damage_23[], pl2f_damage_24[],
	pl2f_damage_25[], pl2f_damage_26[], pl2f_damage_27[], pl2f_damage_30[], pl2f_damage_28[],
	pl2f_damage_29[], pl2f_damage_31[], pl2f_damage_33[], pl2f_dizzy_00[], pl2f_catch_00[],
	pl2f_catch_01[], pl2f_catch_02[], pl2f_caught_00[], pl2f_caught_01[], pl2f_caught_02[],
	pl2f_caught_03[], pl2f_caught_04[], pl2f_caught_05[], pl2f_caught_06[], pl2f_caught_07[],
	pl2f_caught_08[], pl2f_caught_09[], pl2f_caught_10[], pl2f_caught_11[], pl2f_caught_12[],
	pl2f_caught_13[], pl2f_caught_14[], pl2f_caught_15[], pl2f_caught_16[], pl2f_caught_17[],
	pl2f_caught_18[], pl2f_caught_19[], pl2f_caught_20[], pl2f_caught_21[], pl2f_get_up_00[],
	pl2f_get_up_01[], pl2f_get_up_02[], pl2f_get_up_03[], pl2f_appear_00[], pl2f_appear_00_loop[],
	pl2f_win_00[], pl2f_win_00_loop[], pl2f_win_01[], pl2f_win_01_loop[], pl2f_win_02[],
	pl2f_win_02_loop[], pl2f_win_04[], pl2f_win_04_loop[], pl2f_special_00[], pl2f_special_56[],
	pl2f_special_57[], pl2f_special_58[], pl2f_special_59[], pl2f_special_14[], pl2f_special_61[],
	pl2f_special_61_loop[], pl2f_special_62[], pl2f_special_62_loop[], pl2f_special_63[],
	pl2f_special_01[], pl2f_special_01_loop[], pl2f_special_02[], pl2f_special_03[],
	pl2f_special_03_loop[], pl2f_special_04[], pl2f_special_04_loop[], pl2f_special_40[],
	pl2f_special_40_loop[], pl2f_special_39[], pl2f_special_39_loop[], pl2f_special_05[],
	pl2f_unused_00[], pl2f_special_06[], pl2f_super_00[], pl2f_super_00_loop[], pl2f_special_11[],
	pl2f_special_64[], pl2f_super_07_loop[], pl2f_special_12[], pl2f_special_12_loop[],
	pl2f_special_18[], pl2f_special_13[], pl2f_extra_03[], pl2f_extra_04[], pl2f_special_19[],
	pl2f_extra_05[], pl2f_extra_05_loop[], pl2f_special_10[], pl2f_special_15[], pl2f_special_16[],
	pl2f_special_07[], pl2f_special_29[], pl2f_special_08[], pl2f_special_30[], pl2f_special_09[],
	pl2f_special_31[], pl2f_special_24[], pl2f_special_32[], pl2f_special_25[], pl2f_special_33[],
	pl2f_special_26[], pl2f_special_34[], pl2f_special_27[], pl2f_special_35[], pl2f_special_36[],
	pl2f_special_28[], pl2f_super_01[], pl2f_super_01_loop[], pl2f_super_02[], pl2f_super_03[],
	pl2f_super_04[], pl2f_super_05[], pl2f_super_08[], pl2f_super_09[], pl2f_super_06[],
	pl2f_super_06_loop[], pl2f_special_41[], pl2f_special_45[], pl2f_special_49[], pl2f_special_43[],
	pl2f_special_47[], pl2f_special_51[], pl2f_special_53[], pl2f_special_53_loop[],
	pl2f_special_54[], pl2f_special_54_loop[], pl2f_special_42[], pl2f_special_46[],
	pl2f_special_50[], pl2f_special_44[], pl2f_special_48[], pl2f_special_52[], pl2f_special_55[],
	pl2f_special_55_loop[], pl2f_super_07[], pl2f_effect_00[], pl2f_change_00[], pl2f_change_01[],
	pl2f_change_03[], pl2f_change_03_loop[], pl2f_change_04[], pl2f_grp27_00[], pl2f_grp27_01[],
	pl2f_grp27_02[], pl2f_grp27_03[], pl2f_grp27_04[], pl2f_grp27_05[], pl2f_grp27_06[],
	pl2f_grp27_07[], pl2f_grp27_08[], pl2f_grp27_09[], pl2f_grp27_10[], pl2f_grp27_11[],
	pl2f_grp27_12[], pl2f_grp27_13[];
extern const Sint32 *const pl2f_stand_tbl[], *const pl2f_jump_tbl[], *const pl2f_dash_tbl[],
	*const pl2f_crouch_tbl[], *const pl2f_turn_tbl[], *const pl2f_guard_tbl[],
	*const pl2f_air_guard_tbl[], *const pl2f_stand_p_tbl[], *const pl2f_stand_k_tbl[],
	*const pl2f_crouch_p_tbl[], *const pl2f_crouch_k_tbl[], *const pl2f_jump_p_tbl[],
	*const pl2f_jump_k_tbl[], *const pl2f_damage_tbl[], *const pl2f_dizzy_tbl[],
	*const pl2f_catch_tbl[], *const pl2f_caught_tbl[], *const pl2f_get_up_tbl[],
	*const pl2f_appear_tbl[], *const pl2f_win_tbl[], *const pl2f_extra_tbl[],
	*const pl2f_special_tbl[], *const pl2f_super_tbl[], *const pl2f_effect_tbl[],
	*const pl2f_change_tbl[], *const pl2f_grp25_tbl[], *const pl2f_grp27_tbl[];

/* the sprites' sizes and turns, by a frame's zoom number */
const SPR_PRM spr_prm_2f[16] = {
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 1000, 1000, -32768, 0, 0, 0, 0, 0 },
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
const Sint32 *const *const char_tbl_2f[] = {
	pl2f_stand_tbl, pl2f_jump_tbl, pl2f_dash_tbl, pl2f_crouch_tbl, pl2f_turn_tbl, pl2f_guard_tbl,
	pl2f_air_guard_tbl, pl2f_stand_p_tbl, pl2f_stand_k_tbl, pl2f_crouch_p_tbl, pl2f_crouch_k_tbl, pl2f_jump_p_tbl,
	pl2f_jump_k_tbl, pl2f_damage_tbl, pl2f_dizzy_tbl, pl2f_catch_tbl, pl2f_caught_tbl, pl2f_get_up_tbl,
	pl2f_appear_tbl, pl2f_win_tbl, pl2f_extra_tbl, pl2f_special_tbl, pl2f_super_tbl, pl2f_effect_tbl,
	pl2f_change_tbl, pl2f_grp25_tbl, pl2f_grp25_tbl, pl2f_grp27_tbl, 0,
};

const Sint32 *const pl2f_stand_tbl[] = {
	pl2f_stand_00, pl2f_stand_00, pl2f_stand_02, pl2f_stand_03, pl2f_stand_00, pl2f_stand_00,
	pl2f_stand_06, pl2f_stand_06, 0,
};

const Sint32 *const pl2f_jump_tbl[] = {
	pl2f_jump_00, pl2f_jump_01, pl2f_jump_02, pl2f_jump_03, pl2f_jump_04, pl2f_jump_05,
	pl2f_jump_00, pl2f_jump_01, pl2f_jump_02, pl2f_jump_09, pl2f_jump_10, pl2f_jump_11,
	0,
};

const Sint32 *const pl2f_dash_tbl[] = {
	pl2f_dash_00, pl2f_dash_01, pl2f_dash_00, pl2f_dash_03, 0,
};

const Sint32 *const pl2f_crouch_tbl[] = {
	pl2f_crouch_00, pl2f_crouch_01, pl2f_crouch_02, 0,
};

const Sint32 *const pl2f_turn_tbl[] = {
	pl2f_turn_00, pl2f_turn_01, 0,
};

const Sint32 *const pl2f_guard_tbl[] = {
	pl2f_guard_00, pl2f_guard_01, pl2f_guard_02, pl2f_guard_01, pl2f_guard_04, pl2f_guard_05,
	pl2f_guard_06, pl2f_guard_05, 0,
};

const Sint32 *const pl2f_air_guard_tbl[] = {
	pl2f_air_guard_00, pl2f_air_guard_01, pl2f_air_guard_02, pl2f_air_guard_03, pl2f_air_guard_04, pl2f_air_guard_01,
	0,
};

const Sint32 *const pl2f_stand_p_tbl[] = {
	pl2f_stand_p_00, pl2f_stand_p_01, pl2f_stand_p_02, pl2f_stand_p_03, pl2f_stand_p_04, 0,
};

const Sint32 *const pl2f_stand_k_tbl[] = {
	pl2f_stand_k_00, pl2f_stand_k_01, pl2f_stand_k_02, 0,
};

const Sint32 *const pl2f_crouch_p_tbl[] = {
	pl2f_crouch_p_00, pl2f_crouch_p_01, pl2f_crouch_p_02, pl2f_crouch_p_03, 0,
};

const Sint32 *const pl2f_crouch_k_tbl[] = {
	pl2f_crouch_k_00, pl2f_crouch_k_01, pl2f_crouch_k_02, 0,
};

const Sint32 *const pl2f_jump_p_tbl[] = {
	pl2f_jump_p_00, pl2f_jump_p_01, pl2f_jump_p_02, pl2f_jump_p_00, pl2f_jump_p_01, pl2f_jump_p_02,
	0,
};

const Sint32 *const pl2f_jump_k_tbl[] = {
	pl2f_jump_k_00, pl2f_jump_k_01, pl2f_jump_k_02, pl2f_jump_k_00, pl2f_jump_k_01, pl2f_jump_k_02,
	0,
};

const Sint32 *const pl2f_damage_tbl[] = {
	pl2f_damage_00, pl2f_damage_01, pl2f_damage_02, pl2f_damage_03, pl2f_damage_04, pl2f_damage_05,
	pl2f_damage_06, pl2f_damage_07, pl2f_damage_08, pl2f_damage_09, pl2f_damage_10, pl2f_damage_11,
	pl2f_damage_12, pl2f_damage_13, pl2f_damage_13, pl2f_damage_13, pl2f_damage_16, pl2f_damage_17,
	pl2f_damage_18, pl2f_damage_19, pl2f_damage_20, pl2f_damage_21, pl2f_damage_22, pl2f_damage_23,
	pl2f_damage_24, pl2f_damage_25, pl2f_damage_26, pl2f_damage_27, pl2f_damage_28, pl2f_damage_29,
	pl2f_damage_30, pl2f_damage_31, pl2f_damage_30, pl2f_damage_33, pl2f_damage_13, 0,
};

const Sint32 *const pl2f_dizzy_tbl[] = {
	pl2f_dizzy_00, 0,
};

const Sint32 *const pl2f_catch_tbl[] = {
	pl2f_catch_00, pl2f_catch_01, pl2f_catch_02, 0,
};

const Sint32 *const pl2f_caught_tbl[] = {
	pl2f_caught_00, pl2f_caught_01, pl2f_caught_02, pl2f_caught_03, pl2f_caught_04, pl2f_caught_05,
	pl2f_caught_06, pl2f_caught_07, pl2f_caught_08, pl2f_caught_09, pl2f_caught_10, pl2f_caught_11,
	pl2f_caught_12, pl2f_caught_13, pl2f_caught_14, pl2f_caught_15, pl2f_caught_16, pl2f_caught_17,
	pl2f_caught_18, pl2f_caught_19, pl2f_caught_20, pl2f_caught_21, 0,
};

const Sint32 *const pl2f_get_up_tbl[] = {
	pl2f_get_up_00, pl2f_get_up_01, pl2f_get_up_02, pl2f_get_up_03, 0,
};

const Sint32 *const pl2f_appear_tbl[] = {
	pl2f_appear_00, 0,
};

const Sint32 *const pl2f_win_tbl[] = {
	pl2f_win_00, pl2f_win_01, pl2f_win_02, pl2f_win_02, pl2f_win_04, pl2f_win_02,
	0,
};

const Sint32 *const pl2f_extra_tbl[] = {
	pl2f_stand_p_03, pl2f_stand_p_04, pl2f_crouch_p_03, pl2f_extra_03, pl2f_extra_04, pl2f_extra_05,
	pl2f_extra_06, pl2f_extra_07, pl2f_extra_08, 0,
};

const Sint32 *const pl2f_special_tbl[] = {
	pl2f_special_00, pl2f_special_01, pl2f_special_02, pl2f_special_03, pl2f_special_04, pl2f_special_05,
	pl2f_special_06, pl2f_special_07, pl2f_special_08, pl2f_special_09, pl2f_special_10, pl2f_special_11,
	pl2f_special_12, pl2f_special_13, pl2f_special_14, pl2f_special_15, pl2f_special_16, pl2f_special_16,
	pl2f_special_18, pl2f_special_19, pl2f_special_06, pl2f_special_07, pl2f_special_08, pl2f_special_09,
	pl2f_special_24, pl2f_special_25, pl2f_special_26, pl2f_special_27, pl2f_special_28, pl2f_special_29,
	pl2f_special_30, pl2f_special_31, pl2f_special_32, pl2f_special_33, pl2f_special_34, pl2f_special_35,
	pl2f_special_36, pl2f_special_37, pl2f_special_38, pl2f_special_39, pl2f_special_40, pl2f_special_41,
	pl2f_special_42, pl2f_special_43, pl2f_special_44, pl2f_special_45, pl2f_special_46, pl2f_special_47,
	pl2f_special_48, pl2f_special_49, pl2f_special_50, pl2f_special_51, pl2f_special_52, pl2f_special_53,
	pl2f_special_54, pl2f_special_55, pl2f_special_56, pl2f_special_57, pl2f_special_58, pl2f_special_59,
	pl2f_special_14, pl2f_special_61, pl2f_special_62, pl2f_special_63, pl2f_special_64, 0,
};

const Sint32 *const pl2f_super_tbl[] = {
	pl2f_super_00, pl2f_super_01, pl2f_super_02, pl2f_super_03, pl2f_super_04, pl2f_super_05,
	pl2f_super_06, pl2f_super_07, pl2f_super_08, pl2f_super_09, 0,
};

const Sint32 *const pl2f_effect_tbl[] = {
	pl2f_effect_00, 0,
};

const Sint32 *const pl2f_change_tbl[] = {
	pl2f_change_00, pl2f_change_01, pl2f_jump_09, pl2f_change_03, pl2f_change_04, 0,
};

const Sint32 *const pl2f_grp25_tbl[] = {
	pl2f_stand_00, 0,
};

const Sint32 *const pl2f_grp27_tbl[] = {
	pl2f_grp27_00, pl2f_grp27_01, pl2f_grp27_02, pl2f_grp27_03, pl2f_grp27_04, pl2f_grp27_05,
	pl2f_grp27_06, pl2f_grp27_07, pl2f_grp27_08, pl2f_grp27_09, pl2f_grp27_10, pl2f_grp27_11,
	pl2f_grp27_12, pl2f_grp27_13, 0,
};

	/*         prm  type  ctr          end    spr  prio  shadow  atck  flag1          se  zoom  unk  flag2   rival  box */

/* Char_Stand 0: act_stand, appear_move, act_change_gattai_move, act_change_zero_move, ...
   Char_Stand 1
   Char_Stand 4
   Char_Stand 5
   group 25 0
   Char_Fly 0 */
const Sint32 pl2f_stand_00[] = {
	CG_FRAME(    0,    0,   6,           0,   102, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0, 0x0001,  89 ),
	CG_FRAME(    0,    1,   6,           0,   103, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0, 0x0002,  89 ),
	CG_FRAME(    0,    2,   6,           0,   104, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0, 0x0003,  89 ),
	CG_FRAME(    0,    3,   6,           0,   105, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0, 0x0004,  89 ),
	CG_FRAME(    0,    4,   6,           0,   106, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0, 0x0005,  89 ),
	CG_FRAME(    0,    5,   6,           0,   107, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0, 0x0006,  89 ),
	CG_FRAME(    0,    6,   6,           0,   108, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0, 0x0007,  89 ),
	CG_FRAME(    0,    7,   6,           0,   109, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0, 0x0008,  89 ),
	CG_FRAME(    0,    8,   6,           0,   110, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0, 0x0009,  89 ),
	CG_FRAME(    0,    9,   6,           0,   111, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0, 0x000a,  89 ),
	CG_FRAME(    0,   10,   6,           0,   112, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0, 0x000b,  89 ),
	CG_FRAME(    0,   11,   6,           0,   113, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0, 0x000c,  89 ),
	CG_FRAME(    0,   12,   6,           0,   114, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0, 0x000d,  89 ),
	CG_FRAME(    0,   13,   6,           0,   115, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0, 0x000e,  89 ),
	CG_FRAME(    0,   14,   6, END_OF_DATA,   116, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0, 0x000f,  89 ),
	CG_NEXT( pl2f_stand_00 ),
};

/* Char_Stand 2: act_walk (2 + no) */
const Sint32 pl2f_stand_02[] = {
	CG_FRAME(    0,    0,   6,           0,   119, 0x80,   0x2f,    0,  0x00,          0,    0,   0,     0, 0x0011,  89 ),
	CG_FRAME(    0,    0,   6,           0,   120, 0x80,   0x2f,    0,  0x00,          0,    0,   0,     0, 0x0012,  89 ),
	CG_FRAME(    0,    0,   6,           0,   121, 0x80,   0x2f,    0,  0x00,          0,    0,   0,     0, 0x0013,  89 ),
	CG_FRAME(    0,    0,   6,           0,   122, 0x80,   0x2f,    0,  0x00,          0,    0,   0,     0, 0x0014,  89 ),
	CG_FRAME(    0,    0,   6,           0,   123, 0x80,   0x2f,    0,  0x00,          0,    0,   0,     0, 0x0015,  89 ),
	CG_FRAME(    0,    0,   6,           0,   124, 0x80,   0x2f,    0,  0x00,          0,    0,   0,     0, 0x0016,  89 ),
	CG_FRAME(    0,    0,   6,           0,   125, 0x80,   0x2f,    0,  0x00,          0,    0,   0,     0, 0x0017,  89 ),
	CG_FRAME(    0,    0,   6,           0,   126, 0x80,   0x2f,    0,  0x00,          0,    0,   0,     0, 0x0018,  89 ),
	CG_FRAME(    0,    0,   6,           0,   127, 0x80,   0x2f,    0,  0x00,          0,    0,   0,     0, 0x0019,  89 ),
	CG_FRAME(    0,    0,   6,           0,   128, 0x80,   0x2f,    0,  0x00,          0,    0,   0,     0, 0x001a,  89 ),
	CG_FRAME(    0,    0,   6,           0,   129, 0x80,   0x2f,    0,  0x00,          0,    0,   0,     0, 0x001b,  89 ),
	CG_FRAME(    0,    0,   6,           0,   130, 0x80,   0x2f,    0,  0x00,          0,    0,   0,     0, 0x001c,  89 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   131, 0x80,   0x2f,    0,  0x00,          0,    0,   0,     0, 0x001d,  89 ),
	CG_NEXT( pl2f_stand_02 ),
};

/* Char_Stand 3 */
const Sint32 pl2f_stand_03[] = {
	CG_FRAME(    0,    0,   3,           0,   132, 0x80,   0x2e,    0,  0x00,          0,    0,   0,     0, 0x001e,  89 ),
	CG_FRAME(    0,    0,   3,           0,   133, 0x80,   0x2e,    0,  0x00,          0,    0,   0,     0, 0x001f,  89 ),
	CG_FRAME(    0,    0,   3,           0,   134, 0x80,   0x2e,    0,  0x00,          0,    0,   0,     0, 0x0020,  89 ),
	CG_FRAME(    0,    0,   3,           0,   135, 0x80,   0x2e,    0,  0x00,          0,    0,   0,     0, 0x0021,  89 ),
	CG_FRAME(    0,    0,   3,           0,   136, 0x80,   0x2e,    0,  0x00,          0,    0,   0,     0, 0x0022,  89 ),
	CG_FRAME(    0,    0,   3,           0,   137, 0x80,   0x2e,    0,  0x00,          0,    0,   0,     0, 0x0023,  89 ),
	CG_FRAME(    0,    0,   3,           0,   138, 0x80,   0x2e,    0,  0x00,          0,    0,   0,     0, 0x0024,  89 ),
	CG_FRAME(    0,    0,   3,           0,   139, 0x80,   0x2e,    0,  0x00,          0,    0,   0,     0, 0x0025,  89 ),
	CG_FRAME(    0,    0,   3,           0,   140, 0x80,   0x2e,    0,  0x00,          0,    0,   0,     0, 0x0026,  89 ),
	CG_FRAME(    0,    0,   3,           0,   141, 0x80,   0x2e,    0,  0x00,          0,    0,   0,     0, 0x0027,  89 ),
	CG_FRAME(    0,    0,   3,           0,   142, 0x80,   0x2e,    0,  0x00,          0,    0,   0,     0, 0x0028,  89 ),
	CG_FRAME(    0,    0,   3,           0,   143, 0x80,   0x2e,    0,  0x00,          0,    0,   0,     0, 0x0029,  89 ),
	CG_FRAME(    0,    0,   3,           0,   144, 0x80,   0x2e,    0,  0x00,          0,    0,   0,     0, 0x002a,  89 ),
	CG_FRAME(    0,    0,   3,           0,   145, 0x80,   0x2e,    0,  0x00,          0,    0,   0,     0, 0x002b,  89 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   146, 0x80,   0x2e,    0,  0x00,          0,    0,   0,     0, 0x002c,  89 ),
	CG_NEXT( pl2f_stand_03 ),
};

/* Char_Stand 6
   Char_Stand 7 */
const Sint32 pl2f_stand_06[] = {
	CG_FRAME(    0,    0,   6,           0,   137, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0, 0x0023,  89 ),
	CG_FRAME(    0,    0,   6,           0,   138, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0, 0x0024,  89 ),
	CG_FRAME(    0,    0,   6,           0,   139, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0, 0x0025,  89 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   138, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0, 0x0024,  89 ),
	CG_NEXT( pl2f_stand_06 ),
};

/* Char_Jump 0
   Char_Jump 6 */
const Sint32 pl2f_jump_00[] = {
	CG_FRAME(    0,    1,   4,           0,   163, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,   2,           0,   164, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,   2,           0,   165, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,   2,           0,   164, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,   2,           0,   165, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,   2,           0,   164, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,   2,           0,   165, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,   4,           0,   166, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,  12,           0,   167, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,   6,           0,   168, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,   4,           0,   169, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 100 ),
};

/* Char_Jump 9: act_fall, act_air_guard_hold, act_wall_jump
   Char_Change 2: act_change_out_bound */
const Sint32 pl2f_jump_09[] = {
	CG_FRAME(    0,    0,   2,           0,   170, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   171, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_NEXT( pl2f_jump_09 ),
};

/* Char_Jump 1: act_wall_jump, act_change_gattai_init, act_change_zero_init, act_change_assist_init, ...
   Char_Jump 7 */
const Sint32 pl2f_jump_01[] = {
	CG_FRAME(    0,    1,   4,           0,   163, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,   2,           0,   177, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,   2,           0,   178, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,   2,           0,   177, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,   2,           0,   178, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,   2,           0,   177, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,   2,           0,   178, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,   4,           0,   179, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,   4,           0,   180, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,   4,           0,   181, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,   6,           0,   182, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 100 ),
};

/* Char_Jump 10: change_in */
const Sint32 pl2f_jump_10[] = {
	CG_FRAME(    0,    0,   2,           0,   183, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   184, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_NEXT( pl2f_jump_10 ),
};

/* Char_Jump 2
   Char_Jump 8 */
const Sint32 pl2f_jump_02[] = {
	CG_FRAME(    0,    1,   4,           0,   163, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,   2,           0,   190, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,   2,           0,   191, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,   2,           0,   190, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,   2,           0,   191, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,   2,           0,   190, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,   2,           0,   191, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,   4,           0,   192, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,   4,           0,   193, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,   4,           0,   194, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,   6,           0,   195, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 100 ),
};

/* Char_Jump 11: nml_p_catch, act_change_out_recover */
const Sint32 pl2f_jump_11[] = {
	CG_FRAME(    0,    0,   2,           0,   196, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   197, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_NEXT( pl2f_jump_11 ),
};

/* Char_Jump 3 */
const Sint32 pl2f_jump_03[] = {
	CG_FRAME(    0,    0,   4,           0,   172, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   4,           0,   173, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   4,           0,   174, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   4,           0,   163, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   4,           0,   175, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   176, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_NEXT( pl2f_jump_03 ),
};

/* Char_Jump 4 */
const Sint32 pl2f_jump_04[] = {
	CG_FRAME(    0,    0,   4,           0,   185, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   4,           0,   186, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   4,           0,   187, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   4,           0,   188, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   189, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_NEXT( pl2f_jump_04 ),
};

/* Char_Jump 5 */
const Sint32 pl2f_jump_05[] = {
	CG_FRAME(    0,    0,   4,           0,   198, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   4,           0,   199, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   4,           0,   200, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   4,           0,   201, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   202, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_NEXT( pl2f_jump_05 ),
};

/* Char_Dash 0: act_dash
   Char_Dash 2 */
const Sint32 pl2f_dash_00[] = {
	CG_FRAME(    0,    0,   8,           0,   147, 0x80,   0x45,    0,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_FRAME(    0,    0,   8,           0,   148, 0x80,   0x45,    0,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_FRAME(    0,    0,   8,           0,   149, 0x80,   0x45,    0,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_FRAME(    0,    0,   8,           0,   150, 0x80,   0x45,    0,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_FRAME(    0,    0,   8,           0,   151, 0x80,   0x45,    0,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   152, 0x80,   0x45,    0,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_NEXT( pl2f_dash_00 ),
};

/* Char_Dash 1: act_back_dash */
const Sint32 pl2f_dash_01[] = {
	CG_FRAME(    0,    1,   2,           0,   153, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,   4,           0,   154, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,  14,           0,   155, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,  11,           0,   156, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,  10,           0,   157, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,  10, END_OF_DATA,   158, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  35 ),
	CG_NEXT( pl2f_dash_01 ),
};

/* Char_Dash 3: noki_move */
const Sint32 pl2f_dash_03[] = {
	CG_FRAME(    0,    1,   4,           0,   159, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  35 ),
	CG_FRAME(    0,    1,   4,           0,   160, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  35 ),
	CG_FRAME(    0,    1,   4,           0,   161, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    1,   4, END_OF_DATA,   162, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_NEXT( pl2f_dash_03 ),
};

/* Char_Crouch 0: act_crouch, act_stand_up */
const Sint32 pl2f_crouch_00[] = {
	CG_FRAME(    0,    0,   4,           0,   203, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  35 ),
	CG_FRAME(    1,    0,   4,           0,   204, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  35 ),
	CG_FRAME(    2,    0,   4, END_OF_DATA,   206, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  35 ),
	CG_NEXT( pl2f_crouch_00 ),
};

/* Char_Crouch 1: act_crouching, act_escape */
const Sint32 pl2f_crouch_01[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA,   117, 0x80,   0x51,    0,  0x00,          0,    0,   0,     0, 0x002d,  35 ),
	CG_NEXT( pl2f_crouch_01 ),
};

/* Char_Crouch 2: act_stand_up, act_crouch */
const Sint32 pl2f_crouch_02[] = {
	CG_FRAME(    2,    0,   4,           0,   205, 0x80,   0x75,    0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    2,    0,   4,           0,   207, 0x80,   0x51,    0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    1,    0,   4,           0,   208, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   209, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_NEXT( pl2f_crouch_02 ),
};

/* Char_Turn 0: act_turn */
const Sint32 pl2f_turn_00[] = {
	CG_FRAME(    0,    0,   4,           0,   210, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   4,           0,   211, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   4,           0,   212, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   4,           0,   213, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   214, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_NEXT( pl2f_turn_00 ),
};

/* Char_Turn 1: act_crouch_turn */
const Sint32 pl2f_turn_01[] = {
	CG_FRAME(    0,    0,   2,           0,   215, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  35 ),
	CG_FRAME(    0,    0,   3,           0,   216, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  35 ),
	CG_FRAME(    0,    0,   2,           0,   217, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  35 ),
	CG_FRAME(    0,    0,   2,           0,   218, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  35 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   219, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  35 ),
	CG_NEXT( pl2f_turn_01 ),
};

/* Char_Guard 0 */
const Sint32 pl2f_guard_00[] = {
	CG_FRAME(    0,    0,   4,           0,   220, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  89 ),
};

/* Char_Guard 1: act_guard_hold
   Char_Guard 3 */
const Sint32 pl2f_guard_01[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   221, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_NEXT( pl2f_guard_01 ),
};

/* Char_Guard 2 */
const Sint32 pl2f_guard_02[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   220, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_NEXT( pl2f_guard_02 ),
};

/* Char_Guard 4 */
const Sint32 pl2f_guard_04[] = {
	CG_FRAME(    0,    0,   4,           0,   222, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  35 ),
};

/* Char_Guard 5: act_guard_hold
   Char_Guard 7 */
const Sint32 pl2f_guard_05[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   223, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  35 ),
	CG_NEXT( pl2f_guard_05 ),
};

/* Char_Guard 6 */
const Sint32 pl2f_guard_06[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   222, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  35 ),
	CG_NEXT( pl2f_guard_06 ),
};

/* Char_Air_Guard 0: act_air_guard */
const Sint32 pl2f_air_guard_00[] = {
	CG_FRAME(    0,    0,   2,           0,   225, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 100 ),
};

/* Char_Air_Guard 1: act_air_guard_start
   Char_Air_Guard 5 */
const Sint32 pl2f_air_guard_01[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   226, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_NEXT( pl2f_air_guard_01 ),
};

/* Char_Air_Guard 2: guard_char_set */
const Sint32 pl2f_air_guard_02[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   224, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_NEXT( pl2f_air_guard_02 ),
};

/* Char_Air_Guard 3 */
const Sint32 pl2f_air_guard_03[] = {
	CG_FRAME(    0,    0,   2,           0,   183, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   184, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_NEXT( pl2f_air_guard_03 ),
};

/* Char_Air_Guard 4 */
const Sint32 pl2f_air_guard_04[] = {
	CG_FRAME(    0,    0,   2,           0,   196, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   197, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_NEXT( pl2f_air_guard_04 ),
};

/* Char_Special 37: sp_chouhatsu (Taunt) */
const Sint32 pl2f_special_37[] = {
	CG_FRAME(    0,    0,   4,           0,   286, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   4,           0,   287, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  89 ),
};

const Sint32 pl2f_special_37_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   288, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   4,           0,   289, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   290, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_NEXT( pl2f_special_37_loop ),
};

/* Char_Stand_P 0 (light) */
const Sint32 pl2f_stand_p_00[] = {
	CG_FRAME(    0,    0,   1,           0,   298, 0x80,   0x4e,  ATK,  0x00,          0,    0,   0,     0, 0x8034,  89 ),
	CG_FRAME(    0,    0,   3,           0,   302, 0x80,   0x4e,  ATK,  0x00, 0x18 | CSE,    0,   0,     0, 0x8038,  89 ),
	CG_FRAME(    0,    0,   2,           0,   303, 0x80,   0x4d,  ATK,  0x00,          0,    0,   0,     0, 0x0039, 105 ),
	CG_FRAME(    0,    0,   1,           0,   303, 0x80,   0x4d,  ATK,  0x00,          0,    0,   0,     0, 0x0039, 103 ),
	CG_FRAME(    0,    0,   5,           0,   304, 0x80,   0x4d,  ATK,  0x00,          0,    0,   0,     0, 0x003a,  39 ),
	CG_FRAME(    0,    1,   3,           0,   305, 0x80,   0x4d,  ATK,  0x00,          0,    0,   0,     0, 0x003b,  39 ),
	CG_FRAME(    0,    1,   3,           0,   306, 0x80,   0x30,  ATK,  0x00,          0,    0,   0,     0, 0x803c,  89 ),
	CG_FRAME(    0,    1,   4,           0,   307, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0, 0x003d,  89 ),
};

const Sint32 pl2f_stand_p_00_loop[] = {
	CG_FRAME(    0,    1,   5, END_OF_DATA,   308, 0x80,   0x31,  ATE,  0x00,          0,    0,   0,     0, 0x003e,  89 ),
	CG_NEXT( pl2f_stand_p_00_loop ),
};

/* Char_Stand_P 3
   Char_Extra 0 */
const Sint32 pl2f_stand_p_03[] = {
	CG_FRAME(    0,    0,   1,           0,   299, 0x80,   0x4c,  ATK,  0x00,          0,    0,   0,     0, 0x0035,  58 ),
	CG_FRAME(    0,    0,   3,           0,   299, 0x80,   0x4c,  ATK,  0x00, 0x19 | CSE,    0,   0,     0, 0x0035, 106 ),
	CG_FRAME(    0,    0,   1,           0,   299, 0x80,   0x4c,  ATK,  0x00,       0x03,    0,   0,     0, 0x0035, 102 ),
	CG_FRAME(    0,    0,   3,           0,   300, 0x80,   0x4c,  ATK,  0x00,          0,    0,   0,     0, 0x0036,  58 ),
	CG_FRAME(    0,    0,   3,           0,   301, 0x80,   0x4c,  ATK,  0x00,          0,    0,   0,     0, 0x0037,  57 ),
	CG_FRAME(    0,    1,   3,           0,   306, 0x80,   0x30,  ATK,  0x00,          0,    0,   0,     0, 0x803c,  89 ),
	CG_FRAME(    0,    1,   4,           0,   307, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0, 0x003d,  89 ),
};

const Sint32 pl2f_stand_p_03_loop[] = {
	CG_FRAME(    0,    1,   5, END_OF_DATA,   308, 0x80,   0x31,  ATE,  0x00,          0,    0,   0,     0, 0x003e,  89 ),
	CG_NEXT( pl2f_stand_p_03_loop ),
};

/* Char_Stand_P 1 (medium) */
const Sint32 pl2f_stand_p_01[] = {
	CG_FRAME(    0,    0,   7,           0,   309, 0x80,   0x4e,  ATK,  0x00, 0x19 | CSE,    0,   0,     0, 0x803f,  31 ),
	CG_FRAME(    0,    0,   1,           0,   310, 0x80,   0x4e,  ATK,  0x00,          0,    0,   0,     0, 0x0040, 143 ),
	CG_FRAME(    0,    0,   1,           0,   311, 0x80,   0x4e,  ATK,  0x00,          0,    0,   0,     0, 0x0041, 144 ),
	CG_FRAME(    0,    0,   1,           0,   312, 0x80,   0x4e,  ATK,  0x00,          0,    0,   0,     0, 0x0042,  40 ),
	CG_FRAME(    0,    0,   3,           0,   313, 0x80,   0x4e,  ATK,  0x00,          0,    0,   0,     0, 0x0043,  40 ),
	CG_FRAME(    0,    1,   2,           0,   314, 0x80,   0x4e,  ATK,  0x00,          0,    0,   0,     0, 0x0044,  40 ),
	CG_FRAME(    0,    0,   1,           0,   315, 0x80,   0x4e,  ATK,  0x00, 0x1a | CSE,    0,   0,     0, 0x0045, 125 ),
	CG_FRAME(    0,    0,   1,           0,   315, 0x80,   0x4e,  ATK,  0x00,       0x03,    0,   0,     0, 0x0045, 125 ),
	CG_FRAME(    0,    0,   1,           0,   316, 0x80,   0x4e,  ATK,  0x00,          0,    0,   0,     0, 0x0046,  84 ),
	CG_FRAME(    0,    0,   1,           0,   317, 0x80,   0x4e,  ATK,  0x00,          0,    0,   0,     0, 0x0047, 108 ),
	CG_FRAME(    0,    0,   1,           0,   318, 0x80,   0x4e,  ATK,  0x00,          0,    0,   0,     0, 0x0048,  59 ),
	CG_FRAME(    0,    0,   1,           0,   319, 0x80,   0x4e,  ATK,  0x00,          0,    0,   0,     0, 0x0049,  59 ),
	CG_FRAME(    0,    0,  20,           0,   320, 0x80,   0x4e,  ATK,  0x00,          0,    0,   0,     0, 0x004a,  59 ),
	CG_FRAME(    0,    0,   4,           0,   321, 0x80,   0x4e,  ATE,  0x00,          0,    0,   0,     0, 0x804b,  89 ),
	CG_FRAME(    0,    0,   4,           0,   322, 0x80,   0x31,  ATE,  0x00,          0,    0,   0,     0, 0x004c,  89 ),
};

const Sint32 pl2f_stand_p_01_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   323, 0x80,   0x31,  ATE,  0x00,          0,    0,   0,     0, 0x004d,  89 ),
	CG_NEXT( pl2f_stand_p_01_loop ),
};

/* Char_Stand_P 2 (heavy) */
const Sint32 pl2f_stand_p_02[] = {
	CG_FRAME(    0,    0,   4,           0,   324, 0x80,   0x66,  ATK,  0x00,       0x04,    0,   0,     0, 0x004e,  89 ),
	CG_FRAME(    0,    0,   2,           0,   325, 0x80,   0x00,  ATK,  0x00, 0x1a | CSE,    0,   0,     0, 0x004f,  43 ),
	CG_FRAME(    0,    0,   2,           0,   326, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0050,  43 ),
	CG_FRAME(    0,    0,   2,           0,   327, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0051, 148 ),
	CG_FRAME(    0,    0,   1,           0,   328, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0052, 109 ),
	CG_FRAME(    0,    0,   1,           0,   329, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0053, 137 ),
	CG_FRAME(    0,    0,   1,           0,   330, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0054,  41 ),
	CG_FRAME(    0,    0,   1,           0,   331, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0055,  41 ),
	CG_FRAME(    0,    0,   1,           0,   332, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0056,  41 ),
	CG_FRAME(    0,    0,   8,           0,   333, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0057,  41 ),
	CG_FRAME(    0,    1,  13,           0,   333, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0057,  41 ),
	CG_FRAME(    0,    1,   6,           0,   334, 0x80,   0x2b,  ATE,  0x00,          0,    0,   0,     0, 0x0058,  89 ),
	CG_FRAME(    0,    1,   7,           0,   342, 0x80,   0x31,  ATE,  0x00,          0,    0,   0,     0, 0x0060,  89 ),
};

const Sint32 pl2f_stand_p_02_loop[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA,   343, 0x80,   0x31,  ATE,  0x00,          0,    0,   0,     0, 0x0061,  89 ),
	CG_NEXT( pl2f_stand_p_02_loop ),
};

/* Char_Stand_P 4: nml_move_stand_hp
   Char_Extra 1 */
const Sint32 pl2f_stand_p_04[] = {
	CG_FRAME(    0,    0,   4,           0,   334, 0x80,   0x2b,  ATK,  0x00, 0x1a | CSE,    0,   0,     0, 0x0058,  89 ),
	CG_FRAME(    0,    0,   2,           0,   335, 0x80,   0x78,  ATK,  0x00,          0,    0,   0,     0, 0x0059,  35 ),
	CG_FRAME(    0,    0,   2,           0,   336, 0x80,   0x75,  ATK,  0x00,          0,    0,   0,     0, 0x005a,  35 ),
	CG_FRAME(    0,    0,   4,           0,   337, 0x80,   0x75,  ATK,  0x00,          0,    0,   0,     0, 0x005b, 110 ),
	CG_FRAME(    0,    0,   4,           0,   338, 0x80,   0x75,  ATK,  0x00,          0,    0,   0,     0, 0x005c,  33 ),
	CG_FRAME(    0,    0,   5,           0,   339, 0x80,   0x75,  ATK,  0x00,          0,    0,   0,     0, 0x005d,  33 ),
	CG_FRAME(    0,    0,  10,           0,   340, 0x80,   0x75,  ATK,  0x00,          0,    0,   0,     0, 0x005e,  33 ),
	CG_FRAME(    0,    0,   6,           0,   341, 0x80,   0x52,  ATE,  0x00,          0,    0,   0,     0, 0x005f,  35 ),
	CG_FRAME(    0,    0,   7,           0,   342, 0x80,   0x31,  ATE,  0x00,          0,    0,   0,     0, 0x0060,  89 ),
};

const Sint32 pl2f_stand_p_04_loop[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA,   343, 0x80,   0x31,  ATE,  0x00,          0,    0,   0,     0, 0x0061,  89 ),
	CG_NEXT( pl2f_stand_p_04_loop ),
};

/* Char_Stand_K 0 (light) */
const Sint32 pl2f_stand_k_00[] = {
	CG_FRAME(    0,    0,   3,           0,   344, 0x80,   0x31,  ATK,  0x00, 0x18 | CSE,    0,   0,     0, 0x0062,  89 ),
	CG_FRAME(    0,    0,   3,           0,   345, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0, 0x0063, 117 ),
	CG_FRAME(    0,    0,   1,           0,   345, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0, 0x0063, 126 ),
	CG_FRAME(    0,    0,  12,           0,   346, 0x80,   0x31,  ATE,  0x00,          0,    0,   0,     0, 0x0064,  46 ),
};

const Sint32 pl2f_stand_k_00_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   344, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0, 0x0062,  89 ),
	CG_NEXT( pl2f_stand_k_00_loop ),
};

/* Char_Stand_K 1 (medium) */
const Sint32 pl2f_stand_k_01[] = {
	CG_FRAME(    0,    0,   8,           0,   347, 0x80,   0x31,  ATK,  0x00, 0x19 | CSE,    0,   0,     0, 0x0065,  89 ),
	CG_FRAME(    0,    0,   4,           0,   348, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0, 0x0066, 118 ),
	CG_FRAME(    0,    0,   1,           0,   348, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0, 0x0066, 127 ),
	CG_FRAME(    0,    0,  18,           0,   349, 0x80,   0x31,  ATE,  0x00,          0,    0,   0,     0, 0x0067,  60 ),
};

const Sint32 pl2f_stand_k_01_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   344, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0, 0x0062,  89 ),
	CG_NEXT( pl2f_stand_k_01_loop ),
};

/* Char_Extra 6: snapback_init (Snapback) */
const Sint32 pl2f_extra_06[] = {
	CG_FRAME(    0,    0,   8,           0,   347, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0, 0x0065,  89 ),
	CG_FRAME(    0,    1,   2,           0,   347, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0, 0x0065,  89 ),
	CG_FRAME(    0,    0,   1,           0,   347, 0x80,   0x31,  ATK,  0x00, 0x19 | CSE,    0,   0,     0, 0x0065,  89 ),
	CG_FRAME(    0,    0,   3,           0,   347, 0x80,   0x31,  ATK,  0x00,       0x03,    0,   0,     0, 0x0065,  89 ),
	CG_FRAME(    0,    0,   4,           0,   348, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0, 0x0066,  52 ),
	CG_FRAME(    0,    0,   1,           0,   348, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0, 0x0066,  53 ),
	CG_FRAME(    0,    0,  18,           0,   349, 0x80,   0x31,  ATE,  0x00,          0,    0,   0,     0, 0x0067,  60 ),
};

const Sint32 pl2f_extra_06_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   344, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0, 0x0062,  89 ),
	CG_NEXT( pl2f_extra_06_loop ),
};

/* Char_Stand_K 2 (heavy) */
const Sint32 pl2f_stand_k_02[] = {
	CG_FRAME(    0,    0,   3,           0,   350, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0, 0x0068,  89 ),
	CG_FRAME(    0,    1,   4,           0,   351, 0x80,   0x31,  ATK,  0x00,       0x04,    0,   0,     0, 0x0069,  61 ),
	CG_FRAME(    0,    0,   6,           0,   352, 0x80,   0x31,  ATK,  0x00, 0x1a | CSE,    0,   0,     0, 0x006a,  61 ),
	CG_FRAME(    0,    0,   2,           0,   353, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0, 0x006b,  61 ),
	CG_FRAME(    0,    0,   1,           0,   354, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0, 0x006c, 119 ),
	CG_FRAME(    0,    0,   7,           0,   354, 0x80,   0x31,  ATE,  0x00,          0,    0,   0,     0, 0x006c,  62 ),
	CG_FRAME(    0,    0,  20,           0,   355, 0x80,   0x31,  ATE,  0x00,          0,    0,   0,     0, 0x006d,  62 ),
};

const Sint32 pl2f_stand_k_02_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   356, 0x80,   0x31,  ATE,  0x00,          0,    0,   0,     0, 0x006e,  89 ),
	CG_NEXT( pl2f_stand_k_02_loop ),
};

/* Char_Crouch_P 0 (light) */
const Sint32 pl2f_crouch_p_00[] = {
	CG_FRAME(    0,    0,   2,           0,   357, 0x80,   0x31,  ATK,  0x00, 0x18 | CSE,    0,   0,     0, 0x806f,  35 ),
	CG_FRAME(    0,    0,   1,           0,   358, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0, 0x0070,  35 ),
	CG_FRAME(    0,    0,   2,           0,   359, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0, 0x0071, 138 ),
	CG_FRAME(    0,    0,   1,           0,   359, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0, 0x0071, 140 ),
	CG_FRAME(    0,    0,   3,           0,   360, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0, 0x0072,  34 ),
	CG_FRAME(    0,    1,   3,           0,   360, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0, 0x0072,  34 ),
	CG_FRAME(    0,    1,   3,           0,   362, 0x80,   0x31,  ATE,  0x00,          0,    0,   0,     0, 0x0074,  35 ),
};

const Sint32 pl2f_crouch_p_00_loop[] = {
	CG_FRAME(    0,    1,   4, END_OF_DATA,   364, 0x80,   0x31,  ATE,  0x00,          0,    0,   0,     0, 0x0076,  35 ),
	CG_NEXT( pl2f_crouch_p_00_loop ),
};

/* Char_Crouch_P 3: nml_move_crouch_lp
   Char_Extra 2: fly_start5 */
const Sint32 pl2f_crouch_p_03[] = {
	CG_FRAME(    0,    0,   2,           0,   357, 0x80,   0x31,  ATK,  0x00, 0x19 | CSE,    0,   0,     0, 0x806f,  35 ),
	CG_FRAME(    0,    0,   1,           0,   358, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0, 0x0070,  35 ),
	CG_FRAME(    0,    0,   2,           0,   359, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0, 0x0071, 138 ),
	CG_FRAME(    0,    0,   1,           0,   359, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0, 0x0071, 140 ),
	CG_FRAME(    0,    0,   8,           0,   360, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0, 0x0072,  34 ),
};

const Sint32 pl2f_crouch_p_03_loop[] = {
	CG_FRAME(    0,    1,  12, END_OF_DATA,   361, 0x80,   0x31,  ATE,  0x00,          0,    0,   0,     0, 0x8073,  35 ),
	CG_NEXT( pl2f_crouch_p_03_loop ),
};

/* Char_Crouch_P 1 (medium) */
const Sint32 pl2f_crouch_p_01[] = {
	CG_FRAME(    0,    0,   9,           0,   386, 0x80,   0x31,  ATK,  0x00,       0x03,    0,   0,     0, 0x8077,  35 ),
	CG_FRAME(    0,    0,   1,           0,   387, 0x80,   0x31,  ATK,  0x00, 0x19 | CSE,    0,   0,     0, 0x8078,  47 ),
	CG_FRAME(    0,    0,   2,           0,   388, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0, 0x0079, 142 ),
	CG_FRAME(    0,    0,   1,           0,   388, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0, 0x0079, 145 ),
	CG_FRAME(    0,    0,   4,           0,   388, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0, 0x0079,  48 ),
	CG_FRAME(    0,    0,  16,           0,   389, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0, 0x007a,  48 ),
};

const Sint32 pl2f_crouch_p_01_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   390, 0x80,   0x31,  ATE,  0x00,          0,    0,   0,     0, 0x007b,  35 ),
	CG_NEXT( pl2f_crouch_p_01_loop ),
};

/* Char_Crouch_P 2 (heavy) */
const Sint32 pl2f_crouch_p_02[] = {
	CG_FRAME(    0,    0,   8,           0,   372, 0x80,   0x31,  ATK,  0x00,       0x04,    0,   0,     0, 0x807c,  35 ),
	CG_FRAME(    0,    0,   2,           0,   373, 0x80,   0x31,  ATK,  0x00, 0x1a | CSE,    0,   0,     0, 0x007d, 139 ),
	CG_FRAME(    0,    0,   1,           0,   374, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0, 0x007e, 112 ),
	CG_FRAME(    0,    0,   1,           0,   374, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0, 0x007e, 141 ),
	CG_FRAME(    0,    0,   1,           0,   375, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0, 0x007f,  42 ),
	CG_FRAME(    0,    0,   1,           0,   376, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0, 0x0080,  42 ),
	CG_FRAME(    0,    0,   2,           0,   377, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0, 0x0081,  42 ),
	CG_FRAME(    0,    0,  20,           0,   378, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0, 0x0082,  42 ),
	CG_FRAME(    0,    0,   4,           0,   379, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0, 0x0083,  42 ),
	CG_FRAME(    0,    0,   5,           0,   380, 0x80,   0x31,  ATE,  0x00,          0,    0,   0,     0, 0x0084,  35 ),
};

const Sint32 pl2f_crouch_p_02_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   382, 0x80,   0x31,  ATE,  0x00,          0,    0,   0,     0, 0x0086,  35 ),
	CG_NEXT( pl2f_crouch_p_02_loop ),
};

/* Char_Extra 7: sp_crouch_hp (crouching heavy punch (gamma assist)) */
const Sint32 pl2f_extra_07[] = {
	CG_FRAME(    0,    0,   6,           0,   372, 0x80,   0x31,  ATK,  0x00,       0x04,    0,   0,     0, 0x807c,  35 ),
	CG_FRAME(    0,    0,   2,           0,   373, 0x80,   0x31,  ATK,  0x00, 0x1a | CSE,    0,   0,     0, 0x007d, 139 ),
	CG_FRAME(    0,    0,   2,           0,   374, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0, 0x007e, 112 ),
	CG_FRAME(    0,    0,   2,           0,   375, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0, 0x007f, 141 ),
	CG_FRAME(    0,    0,   2,           0,   376, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0, 0x0080,  42 ),
	CG_FRAME(    0,    0,   4,           0,   377, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0, 0x0081,  42 ),
	CG_FRAME(    0,    0,  10,           0,   378, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0, 0x0082,  42 ),
	CG_FRAME(    0,    0,   5,           0,   379, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0, 0x0083,  42 ),
	CG_FRAME(    0,    0,   6,           0,   380, 0x80,   0x31,  ATE,  0x00,          0,    0,   0,     0, 0x0084,  35 ),
	CG_FRAME(    0,    0,   7,           0,   381, 0x80,   0x31,  ATE,  0x00,          0,    0,   0,     0, 0x0085,  35 ),
};

const Sint32 pl2f_extra_07_loop[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA,   382, 0x80,   0x31,  ATE,  0x00,          0,    0,   0,     0, 0x0086,  35 ),
	CG_NEXT( pl2f_extra_07_loop ),
};

/* Char_Extra 8: pl2f_upper */
const Sint32 pl2f_extra_08[] = {
	CG_FRAME(    0,    0,   6,           0,   372, 0x80,   0x31,  ATK,  0x00,       0x04,    0,   0,     0, 0x807c,  35 ),
	CG_FRAME(    0,    0,   2,           0,   373, 0x80,   0x31,  ATK,  0x00, 0x1a | CSE,    0,   0,     0, 0x007d,  35 ),
	CG_FRAME(    0,    0,   2,           0,   374, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0, 0x007e, 162 ),
	CG_FRAME(    0,    0,   2,           0,   375, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0, 0x007f, 163 ),
	CG_FRAME(    0,    0,   2,           0,   376, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0, 0x0080, 164 ),
	CG_FRAME(    0,    0,   4,           0,   377, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0, 0x0081,  42 ),
	CG_FRAME(    0,    0,  10,           0,   378, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0, 0x0082,  42 ),
	CG_FRAME(    0,    0,   5,           0,   379, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0, 0x0083,  42 ),
	CG_FRAME(    0,    0,   6,           0,   380, 0x80,   0x31,  ATE,  0x00,          0,    0,   0,     0, 0x0084,  35 ),
	CG_FRAME(    0,    0,   7,           0,   381, 0x80,   0x31,  ATE,  0x00,          0,    0,   0,     0, 0x0085,  35 ),
};

const Sint32 pl2f_extra_08_loop[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA,   382, 0x80,   0x31,  ATE,  0x00,          0,    0,   0,     0, 0x0086,  35 ),
	CG_NEXT( pl2f_extra_08_loop ),
};

/* Char_Crouch_K 0 (light) */
const Sint32 pl2f_crouch_k_00[] = {
	CG_FRAME(    0,    0,   4,           0,   383, 0x80,   0x31,  ATK,  0x00, 0x18 | CSE,    0,   0,     0, 0x0087,  35 ),
	CG_FRAME(    0,    0,   2,           0,   384, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0, 0x0088, 120 ),
	CG_FRAME(    0,    0,   1,           0,   384, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0, 0x0088, 128 ),
	CG_FRAME(    0,    0,   8,           0,   385, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0, 0x0089,  63 ),
};

const Sint32 pl2f_crouch_k_00_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   383, 0x80,   0x31,  ATE,  0x00,          0,    0,   0,     0, 0x0087,  35 ),
	CG_NEXT( pl2f_crouch_k_00_loop ),
};

/* Char_Crouch_K 1 (medium) */
const Sint32 pl2f_crouch_k_01[] = {
	CG_FRAME(    0,    0,   6,           0,   365, 0x80,   0x31,  ATK,  0x00,       0x03,    0,   0,     0, 0x808a,  35 ),
	CG_FRAME(    0,    0,   1,           0,   366, 0x80,   0x31,  ATK,  0x00, 0x19 | CSE,    0,   0,     0, 0x008b,  35 ),
	CG_FRAME(    0,    0,   2,           0,   367, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0, 0x008c, 111 ),
	CG_FRAME(    0,    0,  12,           0,   368, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0, 0x008d,  45 ),
	CG_FRAME(    0,    1,   6,           0,   369, 0x80,   0x31,  ATE,  0x00,          0,    0,   0,     0, 0x808e,  35 ),
};

const Sint32 pl2f_crouch_k_01_loop[] = {
	CG_FRAME(    0,    1,   4, END_OF_DATA,   371, 0x80,   0x31,  ATE,  0x00,          0,    0,   0,     0, 0x0090,  35 ),
	CG_NEXT( pl2f_crouch_k_01_loop ),
};

/* Char_Crouch_K 2 (heavy) */
const Sint32 pl2f_crouch_k_02[] = {
	CG_FRAME(    0,    0,   2,           0,   391, 0x80,   0x31,  ATK,  0x00, 0x1a | CSE,    0,   0,     0, 0x8091,  35 ),
	CG_FRAME(    0,    0,   8,           0,   392, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0, 0x8092,  35 ),
	CG_FRAME(    0,    0,   2,           0,   394, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0, 0x0094, 121 ),
	CG_FRAME(    0,    0,   1,           0,   395, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0, 0x0096, 107 ),
	CG_FRAME(    0,    0,  24,           0,   395, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0, 0x0096,  51 ),
	CG_FRAME(    0,    0,   6,           0,   396, 0x80,   0x31,  ATE,  0x00,          0,    0,   0,     0, 0x8097,  35 ),
};

const Sint32 pl2f_crouch_k_02_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   398, 0x80,   0x31,  ATE,  0x00,          0,    0,   0,     0, 0x0099,  35 ),
	CG_NEXT( pl2f_crouch_k_02_loop ),
};

/* Char_Jump_P 0 (light)
   Char_Jump_P 3 */
const Sint32 pl2f_jump_p_00[] = {
	CG_FRAME(    0,    0,   1,           0,   399, 0x80,   0x31,  ATK,  0x00, 0x18 | CSE,    0,   0,     0, 0x00a6,  64 ),
	CG_FRAME(    0,    0,   4,           0,   400, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0, 0x00a7,  64 ),
	CG_FRAME(    0,    0,  10,           0,   401, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0, 0x00a8, 113 ),
};

const Sint32 pl2f_jump_p_00_loop[] = {
	CG_FRAME(    0,    0,  16, END_OF_DATA,   402, 0x80,   0x31,  ATE,  0x00,          0,    0,   0,     0, 0x00a9,  16 ),
	CG_NEXT( pl2f_jump_p_00_loop ),
};

/* Char_Special 38: t_kick_init */
const Sint32 pl2f_special_38[] = {
	CG_FRAME(    0,    0,  16, END_OF_DATA,   401, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  22 ),
	CG_NEXT( pl2f_special_38 ),
};

/* Char_Jump_P 1 (medium)
   Char_Jump_P 4 */
const Sint32 pl2f_jump_p_01[] = {
	CG_FRAME(    0,    0,   4,           0,   403, 0x80,   0x31,  ATK,  0x00, 0x19 | CSE,    0,   0,     0, 0x00aa,  64 ),
	CG_FRAME(    0,    0,   1,           0,   404, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0, 0x00ab,  64 ),
	CG_FRAME(    0,    0,   6,           0,   405, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0, 0x00ac, 114 ),
	CG_FRAME(    0,    0,   3,           0,   407, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0, 0x00ae, 115 ),
	CG_FRAME(    0,    0,   2,           0,   408, 0x80,   0x31,  ATE,  0x00,          0,    0,   0,     0, 0x00af,  19 ),
	CG_FRAME(    0,    1,   4,           0,   403, 0x80,   0x31,  ATE,  0x00,          0,    0,   0,     0, 0x00aa,  64 ),
};

const Sint32 pl2f_jump_p_01_loop[] = {
	CG_FRAME(    0,    1,   6, END_OF_DATA,   404, 0x80,   0x31,  ATE,  0x00,          0,    0,   0,     0, 0x00ab,  64 ),
	CG_NEXT( pl2f_jump_p_01_loop ),
};

/* Char_Jump_P 2 (heavy)
   Char_Jump_P 5 */
const Sint32 pl2f_jump_p_02[] = {
	CG_FRAME(    0,    0,   6,           0,   409, 0x80,   0x31,  ATK,  0x00,       0x04,    0,   0,     0, 0x00b0,  64 ),
	CG_FRAME(    0,    0,   2,           0,   410, 0x80,   0x31,  ATK,  0x00, 0x1a | CSE,    0,   0,     0, 0x00b1,  64 ),
	CG_FRAME(    0,    0,   1,           0,   411, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0, 0x00b2,  64 ),
	CG_FRAME(    0,    0,   1,           0,   412, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0, 0x00b3, 116 ),
	CG_FRAME(    0,    0,   1,           0,   413, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0, 0x00b4,  85 ),
	CG_FRAME(    0,    0,   1,           0,   414, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0, 0x00b5,   8 ),
	CG_FRAME(    0,    0,   1,           0,   415, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0, 0x00b6,   8 ),
	CG_FRAME(    0,    0,   1,           0,   416, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0, 0x00b7,   8 ),
	CG_FRAME(    0,    0,   1,           0,   417, 0x80,   0x31,  ATE,  0x00,          0,    0,   0,     0, 0x00b8,  64 ),
};

const Sint32 pl2f_jump_p_02_loop[] = {
	CG_FRAME(    0,    0, -17, END_OF_DATA,   417, 0x80,   0x31,  ATE,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_NEXT( pl2f_jump_p_02_loop ),
};

/* Char_Jump_K 0 (light)
   Char_Jump_K 3 */
const Sint32 pl2f_jump_k_00[] = {
	CG_FRAME(    0,    0,   2,           0,   418, 0x80,   0x31,  ATK,  0x00, 0x14 | CSE,    0,   0,     0, 0x809a,  64 ),
	CG_FRAME(    0,    0,   2,           0,   419, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0, 0x809b,  64 ),
	CG_FRAME(    0,    0,   4,           0,   420, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0, 0x809c, 122 ),
	CG_FRAME(    0,    0,  14,           0,   421, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0, 0x809d, 122 ),
};

const Sint32 pl2f_jump_k_00_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   418, 0x80,   0x31,  ATE,  0x00,          0,    0,   0,     0, 0x809a,  64 ),
	CG_NEXT( pl2f_jump_k_00_loop ),
};

/* Char_Jump_K 1 (medium)
   Char_Jump_K 4 */
const Sint32 pl2f_jump_k_01[] = {
	CG_FRAME(    0,    0,   4,           0,   422, 0x80,   0x31,  ATK,  0x00,       0x03,    0,   0,     0, 0x009e,  64 ),
	CG_FRAME(    0,    0,   1,           0,   423, 0x80,   0x31,  ATK,  0x00, 0x19 | CSE,    0,   0,     0, 0x009f,  64 ),
	CG_FRAME(    0,    0,   2,           0,   424, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0, 0x00a0, 123 ),
	CG_FRAME(    0,    0,  10,           0,   425, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0, 0x00a1,   5 ),
};

const Sint32 pl2f_jump_k_01_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   423, 0x80,   0x31,  ATE,  0x00,          0,    0,   0,     0, 0x009f,  64 ),
	CG_NEXT( pl2f_jump_k_01_loop ),
};

/* Char_Jump_K 2 (heavy)
   Char_Jump_K 5 */
const Sint32 pl2f_jump_k_02[] = {
	CG_FRAME(    0,    0,   6,           0,   426, 0x80,   0x31,  ATK,  0x00,       0x04,    0,   0,     0, 0x00a2,  64 ),
	CG_FRAME(    0,    0,   2,           0,   427, 0x80,   0x31,  ATK,  0x00, 0x1a | CSE,    0,   0,     0, 0x00a3,  64 ),
	CG_FRAME(    0,    0,   1,           0,   428, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0, 0x00a4, 124 ),
	CG_FRAME(    0,    0,  13,           0,   429, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0, 0x00a5,   7 ),
};

const Sint32 pl2f_jump_k_02_loop[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA,   418, 0x80,   0x31,  ATE,  0x00,          0,    0,   0,     0, 0x809a,  64 ),
	CG_NEXT( pl2f_jump_k_02_loop ),
};

/* Char_Damage 0: dm_freeze, dm_freeze_air, caught_dm */
const Sint32 pl2f_damage_00[] = {
	CG_FRAME(    0,    0,   7,           0,   521, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   520, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_NEXT( pl2f_damage_00 ),
};

/* Char_Damage 1 */
const Sint32 pl2f_damage_01[] = {
	CG_FRAME(    0,    0,  13,           0,   522, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,   520, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_NEXT( pl2f_damage_01 ),
};

/* Char_Damage 2 */
const Sint32 pl2f_damage_02[] = {
	CG_FRAME(    0,    0,   2,           0,   523, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   6,           0,   524, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   4,           0,   525, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   4,           0,   526, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   4,           0,   527, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   528, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_NEXT( pl2f_damage_02 ),
};

/* Char_Damage 3 */
const Sint32 pl2f_damage_03[] = {
	CG_FRAME(    0,    0,   7,           0,   530, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   529, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_NEXT( pl2f_damage_03 ),
};

/* Char_Damage 4 */
const Sint32 pl2f_damage_04[] = {
	CG_FRAME(    0,    0,  13,           0,   531, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,   529, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_NEXT( pl2f_damage_04 ),
};

/* Char_Damage 5 */
const Sint32 pl2f_damage_05[] = {
	CG_FRAME(    0,    0,   4,           0,   533, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   6,           0,   534, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   2,           0,   535, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   3,           0,   536, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   3,           0,   537, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   3,           0,   538, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   539, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_NEXT( pl2f_damage_05 ),
};

/* Char_Damage 6 */
const Sint32 pl2f_damage_06[] = {
	CG_FRAME(    0,    0,   7,           0,   541, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  35 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   540, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  35 ),
	CG_NEXT( pl2f_damage_06 ),
};

/* Char_Damage 7 */
const Sint32 pl2f_damage_07[] = {
	CG_FRAME(    0,    0,  13,           0,   542, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  35 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,   540, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  35 ),
	CG_NEXT( pl2f_damage_07 ),
};

/* Char_Damage 8 */
const Sint32 pl2f_damage_08[] = {
	CG_FRAME(    0,    0,   2,           0,   543, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  35 ),
	CG_FRAME(    0,    0,   2,           0,   544, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  35 ),
	CG_FRAME(    0,    0,   2,           0,   545, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  35 ),
	CG_FRAME(    0,    0,   8,           0,   546, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  35 ),
	CG_FRAME(    0,    0,   6,           0,   547, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  35 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   548, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  35 ),
	CG_NEXT( pl2f_damage_08 ),
};

/* Char_Damage 9: dm_sweep */
const Sint32 pl2f_damage_09[] = {
	CG_FRAME(    0,    0,   4,           0,   555, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  10,           0,   556, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   557, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   558, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   559, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  23 ),
	CG_FRAME(    0,    0,   2,           0,   561, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  23 ),
	CG_FRAME(    0,    0,   2,           0,   559, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  23 ),
};

const Sint32 pl2f_damage_09_loop[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   560, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  23 ),
	CG_NEXT( pl2f_damage_09_loop ),
};

/* Char_Damage 10: dm_ko_blow */
const Sint32 pl2f_damage_10[] = {
	CG_FRAME(    0,    0,  10, END_OF_DATA,   549, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_NEXT( pl2f_damage_10 ),
};

/* Char_Damage 11 */
const Sint32 pl2f_damage_11[] = {
	CG_FRAME(    0,    0,  10, END_OF_DATA,   550, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_NEXT( pl2f_damage_11 ),
};

/* Char_Damage 12 */
const Sint32 pl2f_damage_12[] = {
	CG_FRAME(    0,    0,  10, END_OF_DATA,   551, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_NEXT( pl2f_damage_12 ),
};

/* Char_Damage 13: dm_snap_fly, dm_air
   Char_Damage 14
   Char_Damage 15
   Char_Damage 34: dm_snap_fly, dm_snap_down */
const Sint32 pl2f_damage_13[] = {
	CG_FRAME( 0x80,    0,   4,           0,   553, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  23 ),
	CG_FRAME( 0x82,    0,   4, END_OF_DATA,   554, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  23 ),
	CG_NEXT( pl2f_damage_13 ),
};

/* Char_Damage 16: dm_blow_char_set (16 + no), dm_spark, caught_shock_init */
const Sint32 pl2f_damage_16[] = {
	CG_FRAME(    0,    0,   2,           0,   562, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   563, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_NEXT( pl2f_damage_16 ),
};

/* Char_Damage 17: dm_07_1 */
const Sint32 pl2f_damage_17[] = {
	CG_FRAME(    0,    0,   2,           0,   564, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   564, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_NEXT( pl2f_damage_17 ),
};

/* Char_Damage 18 */
const Sint32 pl2f_damage_18[] = {
	CG_FRAME(    0,    0,   2,           0,   565, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   565, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_NEXT( pl2f_damage_18 ),
};

/* Char_Damage 19 */
const Sint32 pl2f_damage_19[] = {
	CG_FRAME(    0,    0,   2,           0,   566, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   566, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_NEXT( pl2f_damage_19 ),
};

/* Char_Damage 20 */
const Sint32 pl2f_damage_20[] = {
	CG_FRAME(    0,    0,   2,           0,   567, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   567, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_NEXT( pl2f_damage_20 ),
};

/* Char_Damage 21: dm_fall_char_set (21 + no) */
const Sint32 pl2f_damage_21[] = {
	CG_FRAME(    0,    0,   2,           0,   562, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,   2,           0,   563, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 100 ),
};

/* Char_Damage 22 */
const Sint32 pl2f_damage_22[] = {
	CG_FRAME(    0,    0,   2,           0,   564, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,   2,           0,   564, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 100 ),
};

/* Char_Damage 23 */
const Sint32 pl2f_damage_23[] = {
	CG_FRAME(    0,    0,   2,           0,   565, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,   2,           0,   565, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 100 ),
};

/* Char_Damage 24 */
const Sint32 pl2f_damage_24[] = {
	CG_FRAME(    0,    0,   2,           0,   566, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,   2,           0,   566, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 100 ),
};

/* Char_Damage 25 */
const Sint32 pl2f_damage_25[] = {
	CG_FRAME(    0,    0,   2,           0,   567, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   567, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_NEXT( pl2f_damage_25 ),
};

/* Char_Damage 26: dm_buttobi_fly, dm_07_1, dm_away_fly, dm_bound_hop, ... */
const Sint32 pl2f_damage_26[] = {
	CG_FRAME(    0,    0,   2,           0,   568, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  23 ),
	CG_FRAME(    0,    0,   2,           0,   569, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  23 ),
	CG_FRAME(    0,    0,   2,           0,   570, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  23 ),
	CG_FRAME(    0,    0,   3,           0,   571, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  23 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   572, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  23 ),
	CG_NEXT( pl2f_damage_26 ),
};

/* Char_Damage 27 */
const Sint32 pl2f_damage_27[] = {
	CG_FRAME(    0,    0,   4,           0,   568, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,   4,           0,   573, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,   4,           0,   574, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   575, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_NEXT( pl2f_damage_27 ),
};

/* Char_Damage 30: act_air_recover, act_change_out_init, caught_blow_fly, caught_blow_fix_fly, ...
   Char_Damage 32: act_escape */
const Sint32 pl2f_damage_30[] = {
	CG_FRAME(    0,    0,   6,           0,   573, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,   6,           0,   574, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   575, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_NEXT( pl2f_damage_30 ),
};

/* Char_Damage 28: dm_away_init, dm_bound_fly, dm_piyo, dm_away */
const Sint32 pl2f_damage_28[] = {
	CG_FRAME(    0,    0,   2,           0,   576, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,   2,           0,   577, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,   2,           0,   578, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,   2,           0,   579, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,   2,           0,   580, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   581, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_NEXT( pl2f_damage_28 ),
};

/* Char_Damage 29: dm_away_fly, dm_bound_hop, dm_bound, caught_spin_fly, ... */
const Sint32 pl2f_damage_29[] = {
	CG_FRAME(    0,    0,   2,           0,   582, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,   2,           0,   583, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,   2,           0,   584, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,   2,           0,   585, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,   2,           0,   586, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   587, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_NEXT( pl2f_damage_29 ),
};

/* Char_Damage 31: act_down, dm_dizzy_down */
const Sint32 pl2f_damage_31[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   227, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  23 ),
	CG_NEXT( pl2f_damage_31 ),
};

/* Char_Damage 33: act_escape */
const Sint32 pl2f_damage_33[] = {
	CG_FRAME(    0,    0,   6,           0,   252, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  35 ),
	CG_FRAME(    0,    0,   6,           0,   253, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  35 ),
	CG_FRAME(    0,    0,   6,           0,   254, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  35 ),
	CG_FRAME(    0,    0,   6,           0,   255, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  35 ),
	CG_FRAME(    0,    0,   6,           0,   256, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   6,           0,   257, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   6,           0,   258, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   6,           0,   259, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   260, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_NEXT( pl2f_damage_33 ),
};

/* Char_Dizzy 0: act_dizzy */
const Sint32 pl2f_dizzy_00[] = {
	CG_FRAME(    0,    0,   8,           0,   588, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   8,           0,   589, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   8,           0,   590, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   8,           0,   591, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   8,           0,   592, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   593, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_NEXT( pl2f_dizzy_00 ),
};

/* Char_Catch 0 */
const Sint32 pl2f_catch_00[] = {
	CG_FRAME(    0, 0x80,  22,           0,   444, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0, 0x800f,   0 ),
	CG_FRAME(    0,    1,   4,           0,   445, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0, 0x8010,   0 ),
	CG_FRAME(    0,    1,   6,           0,   446, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0, 0x8011,   0 ),
	CG_FRAME(    0,    1,   8,           0,   447, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0, 0x8012,   0 ),
	CG_FRAME(    1,    1,  14,           0,   448, 0x80,   0x31,    0,  0x00,       0x05,    0,   0,     0, 0x8013,   0 ),
	CG_FRAME( 0x80,    1,   2,           0,   449, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0, 0x8014,   0 ),
	CG_FRAME(    0,    1,   2,           0,   450, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0, 0x8015,   0 ),
	CG_FRAME(    0,    0,  18,           0,   451, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0, 0x8016,  89 ),
	CG_FRAME(    0, 0x80,   1,           0,   451, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0, 0x8016,  89 ),
	CG_FRAME(    0,    0,   6,           0,   452, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0, 0x8000,  89 ),
	CG_FRAME(    0,    0,   6,           0,   453, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0, 0x8000,  89 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   454, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0, 0x8000,  89 ),
	CG_NEXT( pl2f_catch_00 ),
};

/* Char_Catch 1 */
const Sint32 pl2f_catch_01[] = {
	CG_FRAME(    0,    0,  10,           0,   430, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0, 0x8001,   0 ),
	CG_FRAME(    0,    0,   8,           0,   431, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0, 0x8002,   0 ),
	CG_FRAME(    1,    0,   6,           0,   432, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0, 0x8003,   0 ),
	CG_FRAME(    1,    0,   4,           0,   433, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0, 0x8004,   0 ),
	CG_FRAME(    1,    0,   1,           0,   434, 0x80,   0x31,    0,  0x00,       0x04,    0,   0,     0, 0x8005,   0 ),
	CG_FRAME(    1, 0x80,  13,           0,   434, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0, 0x8000,  35 ),
	CG_FRAME(    1,    0,   6,           0,   435, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0, 0x8000,  35 ),
	CG_FRAME(    1,    0,   6,           0,   436, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0, 0x8000,  35 ),
	CG_FRAME(    0,    0,   6,           0,   437, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0, 0x8000,  35 ),
	CG_FRAME(    0,    0,  10,           0,   438, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0, 0x8000,  35 ),
	CG_FRAME(    0,    0,   4,           0,   439, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0, 0x8000,  89 ),
	CG_FRAME(    0,    0,   4,           0,   440, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0, 0x8000,  89 ),
	CG_FRAME(    0,    0,   4,           0,   441, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0, 0x8000,  89 ),
	CG_FRAME(    0,    0,   4,           0,   442, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0, 0x8000,  89 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   443, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0, 0x8000,  89 ),
	CG_NEXT( pl2f_catch_01 ),
};

/* Char_Catch 2 */
const Sint32 pl2f_catch_02[] = {
	CG_FRAME(    0,    0,  10,           0,   430, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0, 0x8001,   0 ),
	CG_FRAME(    0,    0,   6,           0,   432, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0, 0x8003,   0 ),
	CG_FRAME(    0,    0,   4,           0,   433, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0, 0x8004,   0 ),
	CG_FRAME(    0,    0,   1,           0,   434, 0x80,   0x31,    0,  0x00,       0x04,    0,   0,     0, 0x8005,   0 ),
	CG_FRAME(    0, 0x80,  13,           0,   434, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0, 0x8000, 100 ),
	CG_FRAME(    0,    0,   6,           0,   435, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0, 0x8000, 100 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   436, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0, 0x8000, 100 ),
	CG_NEXT( pl2f_catch_02 ),
};

/* Char_Caught 0 */
const Sint32 pl2f_caught_00[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   521, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 152 ),
	CG_NEXT( pl2f_caught_00 ),
};

/* Char_Caught 1 */
const Sint32 pl2f_caught_01[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   522, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 152 ),
	CG_NEXT( pl2f_caught_01 ),
};

/* Char_Caught 2 */
const Sint32 pl2f_caught_02[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   523, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 152 ),
	CG_NEXT( pl2f_caught_02 ),
};

/* Char_Caught 3 */
const Sint32 pl2f_caught_03[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   530, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 152 ),
	CG_NEXT( pl2f_caught_03 ),
};

/* Char_Caught 4 */
const Sint32 pl2f_caught_04[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   531, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 153 ),
	CG_NEXT( pl2f_caught_04 ),
};

/* Char_Caught 5 */
const Sint32 pl2f_caught_05[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   532, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 154 ),
	CG_NEXT( pl2f_caught_05 ),
};

/* Char_Caught 6 */
const Sint32 pl2f_caught_06[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   541, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 155 ),
	CG_NEXT( pl2f_caught_06 ),
};

/* Char_Caught 7 */
const Sint32 pl2f_caught_07[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   542, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 155 ),
	CG_NEXT( pl2f_caught_07 ),
};

/* Char_Caught 8 */
const Sint32 pl2f_caught_08[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   543, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 155 ),
	CG_NEXT( pl2f_caught_08 ),
};

/* Char_Caught 9 */
const Sint32 pl2f_caught_09[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   549, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 154 ),
	CG_NEXT( pl2f_caught_09 ),
};

/* Char_Caught 10 */
const Sint32 pl2f_caught_10[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   550, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 156 ),
	CG_NEXT( pl2f_caught_10 ),
};

/* Char_Caught 11 */
const Sint32 pl2f_caught_11[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   551, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 156 ),
	CG_NEXT( pl2f_caught_11 ),
};

/* Char_Caught 12 */
const Sint32 pl2f_caught_12[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   553, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 157 ),
	CG_NEXT( pl2f_caught_12 ),
};

/* Char_Caught 13 */
const Sint32 pl2f_caught_13[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   562, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 154 ),
	CG_NEXT( pl2f_caught_13 ),
};

/* Char_Caught 14 */
const Sint32 pl2f_caught_14[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   564, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 156 ),
	CG_NEXT( pl2f_caught_14 ),
};

/* Char_Caught 15 */
const Sint32 pl2f_caught_15[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   565, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 156 ),
	CG_NEXT( pl2f_caught_15 ),
};

/* Char_Caught 16 */
const Sint32 pl2f_caught_16[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   566, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 156 ),
	CG_NEXT( pl2f_caught_16 ),
};

/* Char_Caught 17 */
const Sint32 pl2f_caught_17[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   567, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 156 ),
	CG_NEXT( pl2f_caught_17 ),
};

/* Char_Caught 18 */
const Sint32 pl2f_caught_18[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   578, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 153 ),
	CG_NEXT( pl2f_caught_18 ),
};

/* Char_Caught 19 */
const Sint32 pl2f_caught_19[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   584, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 153 ),
	CG_NEXT( pl2f_caught_19 ),
};

/* Char_Caught 20 */
const Sint32 pl2f_caught_20[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   227, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 158 ),
	CG_NEXT( pl2f_caught_20 ),
};

/* Char_Caught 21 */
const Sint32 pl2f_caught_21[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   555, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0, 156 ),
	CG_NEXT( pl2f_caught_21 ),
};

/* Char_Get_Up 0: dm_dizzy_wait */
const Sint32 pl2f_get_up_00[] = {
	CG_FRAME(    0,    0,   3,           0,   227, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   3,           0,   228, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   3,           0,   229, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   6,           0,   230, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   3,           0,   231, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   3,           0,   232, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   3,           0,   233, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   3,           0,   234, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   235, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_NEXT( pl2f_get_up_00 ),
};

/* Char_Get_Up 1 */
const Sint32 pl2f_get_up_01[] = {
	CG_FRAME(    0,    0,   2,           0,   238, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   239, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   240, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   241, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   243, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   244, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   246, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   248, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2f_get_up_01 ),
};

/* Char_Get_Up 2 */
const Sint32 pl2f_get_up_02[] = {
	CG_FRAME(    0,    0,   2,           0,   238, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   239, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   240, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   241, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   242, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   238, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   239, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   240, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   241, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   242, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   243, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   244, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   245, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   246, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   247, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   248, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2f_get_up_02 ),
};

/* Char_Get_Up 3 */
const Sint32 pl2f_get_up_03[] = {
	CG_FRAME(    0,    0,   3,           0,   251, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   252, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   253, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   254, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   255, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   256, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   257, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   258, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   259, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   260, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   261, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2f_get_up_03 ),
};

/* Char_Appear 0: appear_init */
const Sint32 pl2f_appear_00[] = {
	CG_FRAME(    0,    0,   8,           0,   262, 0x80,   0xff,    0,  0x00, 0x05 | CSE,    0,   0,     0,      0,   0 ),
	CG_FRAME( 0x80,    0,   4,           0,   263, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0, 0x80,   4,           0,   263, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8,           0,   264, 0x80,   0xff,    0,  0x00, 0x05 | CSE,    0,   0,     0,      0,   0 ),
	CG_FRAME( 0x80,    0,   4,           0,   265, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0, 0x80,   8,           0,   263, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  32,           0,   266, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   267, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   268, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   269, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   270, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   271, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   272, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   273, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8,           0,   274, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   275, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   276, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   275, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   276, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   275, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   276, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   275, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   276, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   275, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   276, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  12,           0,   277, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8,           0,   278, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   279, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl2f_appear_00_loop[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA,   280, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2f_appear_00_loop ),
};

/* Char_Win 0 */
const Sint32 pl2f_win_00[] = {
	CG_FRAME(    0,    0,   6,           0,   281, 0x80,   0x31,    0,  0x00,       0x0f,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8,           0,   282, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  16,           0,   283, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   284, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl2f_win_00_loop[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   285, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2f_win_00_loop ),
};

/* Char_Win 1 */
const Sint32 pl2f_win_01[] = {
	CG_FRAME(    0,    0,   4,           0,   291, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  12,           0,   292, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl2f_win_01_loop[] = {
	CG_FRAME(    0,    0,   6,           0,   293, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   294, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2f_win_01_loop ),
};

/* Char_Win 2
   Char_Win 3
   Char_Win 5 */
const Sint32 pl2f_win_02[] = {
	CG_FRAME(    0,    0,   4,           0,   591, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   6,           0,   592, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  89 ),
};

const Sint32 pl2f_win_02_loop[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   593, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_NEXT( pl2f_win_02_loop ),
};

/* Char_Win 4 */
const Sint32 pl2f_win_04[] = {
	CG_FRAME(    0,    0,   6,           0,   286, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8,           0,   287, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  16,           0,   288, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl2f_win_04_loop[] = {
	CG_FRAME(    0,    0,   8,           0,   288, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   289, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   290, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2f_win_04_loop ),
};

/* Char_Special 0 */
const Sint32 pl2f_special_00[] = {
	CG_FRAME(    0,    0,   4,           0,   455, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   8,           0,   456, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    1,    0,  10,           0,   457, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   1,           0,   458, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,   -1,   6,           0,   459, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   8,           0,   460, 0x80,   0x31,  ATE,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,  48,           0,   461, 0x80,   0x31,  ATE,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   6,           0,   462, 0x80,   0x31,  ATE,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,  10, END_OF_DATA,   463, 0x80,   0x31,  ATE,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_NEXT( pl2f_special_00 ),
};

/* Char_Special 56 */
const Sint32 pl2f_special_56[] = {
	CG_FRAME(    0,    0,   1,           0,   455, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   2,           0,   456, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    1,    0,  10,           0,   457, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   1,           0,   458, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,   -1,   6,           0,   459, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   8,           0,   460, 0x80,   0x31,  ATE,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,  20,           0,   461, 0x80,   0x31,  ATE,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   6,           0,   462, 0x80,   0x31,  ATE,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,  10, END_OF_DATA,   463, 0x80,   0x31,  ATE,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_NEXT( pl2f_special_56 ),
};

/* Char_Special 57 */
const Sint32 pl2f_special_57[] = {
	CG_FRAME(    0,    0,   1,           0,   455, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   2,           0,   456, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    1,    0,  10,           0,   457, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   1,           0,   458, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,   -1,   6,           0,   459, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   8,           0,   460, 0x80,   0x31,  ATE,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,  40,           0,   461, 0x80,   0x31,  ATE,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   6,           0,   462, 0x80,   0x31,  ATE,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,  10, END_OF_DATA,   463, 0x80,   0x31,  ATE,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_NEXT( pl2f_special_57 ),
};

/* Char_Special 58 */
const Sint32 pl2f_special_58[] = {
	CG_FRAME(    0,    0,   1,           0,   455, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   2,           0,   456, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    1,    0,  20,           0,   457, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   1,           0,   458, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,   -1,   2,           0,   459, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   2,           0,   460, 0x80,   0x31,  ATE,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   4,           0,   461, 0x80,   0x31,  ATE,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   2,           0,   462, 0x80,   0x31,  ATE,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   463, 0x80,   0x31,  ATE,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_NEXT( pl2f_special_58 ),
};

/* Char_Special 59 */
const Sint32 pl2f_special_59[] = {
	CG_FRAME(    0,    0,   1,           0,   455, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   2,           0,   456, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    1,    0,  10,           0,   457, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   1,           0,   458, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,   -1,   6,           0,   459, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   8,           0,   460, 0x80,   0x31,  ATE,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,  20,           0,   461, 0x80,   0x31,  ATE,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   6,           0,   462, 0x80,   0x31,  ATE,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,  10, END_OF_DATA,   463, 0x80,   0x31,  ATE,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_NEXT( pl2f_special_59 ),
};

/* Char_Special 14
   Char_Special 60 */
const Sint32 pl2f_special_14[] = {
	CG_FRAME(    0,    0,   5,           0,   465, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    1,    0,   1,           0,   465, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,   8,           0,   466, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,   -1,  12,           0,   467, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   8,           0,   468, 0x80,   0x31,  ATE,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,  16, END_OF_DATA,   466, 0x80,   0x31,  ATE,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_NEXT( pl2f_special_14 ),
};

/* Char_Special 61 */
const Sint32 pl2f_special_61[] = {
	CG_FRAME(    0,    0,   4,           0,   465, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,   8,           0,   466, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,   -1,   4,           0,   467, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,  48,           0,   468, 0x80,   0x31,  ATE,  0x00,          0,    0,   0,     0,      0,  14 ),
};

const Sint32 pl2f_special_61_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   466, 0x80,   0x31,  ATE,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_NEXT( pl2f_special_61_loop ),
};

/* Char_Special 62 */
const Sint32 pl2f_special_62[] = {
	CG_FRAME(    0,    0,   4,           0,   465, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,  20,           0,   466, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,   -1,   4,           0,   467, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,  32,           0,   468, 0x80,   0x31,  ATE,  0x00,          0,    0,   0,     0,      0,  14 ),
};

const Sint32 pl2f_special_62_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   466, 0x80,   0x31,  ATE,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_NEXT( pl2f_special_62_loop ),
};

/* Char_Special 63 */
const Sint32 pl2f_special_63[] = {
	CG_FRAME(    0,    0,   4,           0,   465, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,   8,           0,   466, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,   -1,   4,           0,   467, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,  32,           0,   468, 0x80,   0x31,  ATE,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   466, 0x80,   0x31,  ATE,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_NEXT( pl2f_special_63 ),
};

/* Char_Special 1: pls2f_00_t0_init, rapid_move (Hyakuretsutou, 1 + char_no.b.no) */
const Sint32 pl2f_special_01[] = {
	CG_FRAME(    0,    0,   4,           0, 32792, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl2f_special_01_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA, 32793, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2f_special_01_loop ),
};

/* Char_Special 2: shl2f_00_init */
const Sint32 pl2f_special_02[] = {
	CG_FRAME(    0,    0,   1,           0, 32794, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 104 ),
	CG_FRAME(    0,    0,   1,           0, 32795, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 104 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA, 32796, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 104 ),
	CG_NEXT( pl2f_special_02 ),
};

/* Char_Special 3: shl2f_00_move */
const Sint32 pl2f_special_03[] = {
	CG_FRAME(    0,    0,   4,           0, 32797, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32798, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32799, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl2f_special_03_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA, 32800, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2f_special_03_loop ),
};

/* Char_Special 4: raimei_init (Raimeiken), homura_init (Homuraken (hyper, fire element)) */
const Sint32 pl2f_special_04[] = {
	CG_FRAME(    0,    0,   5,           0,   511, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   5,           0,   512, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    1,   8,           0,   513, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   4,           0,   514, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    1,    1,   3,           0,   515, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,   3,           0,   516, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
};

const Sint32 pl2f_special_04_loop[] = {
	CG_FRAME(    1,    0,   3,           0,   515, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   516, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_NEXT( pl2f_special_04_loop ),
};

/* Char_Special 40: elem_k_init */
const Sint32 pl2f_special_40[] = {
	CG_FRAME(    0,    0,   5,           0,   511, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   5,           0,   512, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   8,           0,   513, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   4,           0,   514, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME( 0x80,    1,   3,           0,   515, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME( 0x80,    1,   3,           0,   516, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
};

const Sint32 pl2f_special_40_loop[] = {
	CG_FRAME( 0x80,    1,   3,           0,   515, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME( 0x80,    1,   3, END_OF_DATA,   516, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_NEXT( pl2f_special_40_loop ),
};

/* Char_Special 39: t_kick_move */
const Sint32 pl2f_special_39[] = {
	CG_FRAME(    0,    0,   3,           0,   185, 0x80,   0x31,  ATK,  0x00,       0x15,    0,   0,     0,      0,  35 ),
	CG_FRAME(    0,    0,   3,           0,   186, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  35 ),
	CG_FRAME(    0,    0,   3,           0,   187, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  35 ),
	CG_FRAME(    0,    0,   3,           0,   511, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   2,           0,   515, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   2,           0,   516, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  89 ),
};

const Sint32 pl2f_special_39_loop[] = {
	CG_FRAME(    1,    0,   3,           0,   515, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   516, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_NEXT( pl2f_special_39_loop ),
};

/* Char_Special 5: raimei_move (Raimeiken), elem_k_wait, homura_move (Homuraken (hyper, fire element)) */
const Sint32 pl2f_special_05[] = {
	CG_FRAME(    0,    0,   6,           0,   462, 0x80,   0x31,  ATE,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   6,           0,   463, 0x80,   0x31,  ATE,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   443, 0x80,   0x31,  ATE,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_NEXT( pl2f_special_05 ),
};

const Sint32 pl2f_unused_00[] = {	/* nothing reaches it */
	0x00003e60,
};

/* Char_Special 6: pls2f_00_t1_init
   Char_Special 20 */
const Sint32 pl2f_special_06[] = {
	CG_FRAME(    0,    0,   2,           0,   605, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   606, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   607, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2f_special_06 ),
};

/* Char_Super 0: hrapid_turn (Hyper Hyakuretsutou (team hyper)) */
const Sint32 pl2f_super_00[] = {
	CG_FRAME(    0,    0,   4,           0,   456, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   2,           0,   457, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   1,           0,   458, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0, 0x80,   1,           0,   458, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  89 ),
};

const Sint32 pl2f_super_00_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   469, 0x80,   0x31,  ATK,  0x55,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,   4,           0,   470, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(    0,    0,   4,           0,   471, 0x80,   0x31,  ATK,  0x59,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,   4,           0,   472, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(    0,    0,   4,           0,   473, 0x80,   0x31,  ATK,  0x55,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,   4,           0,   474, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(    0,    0,   4,           0,   475, 0x80,   0x31,  ATK,  0x59,          0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,   4,           0,   476, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(    0,    0,   4,           0,   477, 0x80,   0x31,  ATK,  0x55,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,   4,           0,   478, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(    0,    0,   4,           0,   479, 0x80,   0x31,  ATK,  0x59,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   480, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_NEXT( pl2f_super_00_loop ),
};

/* Char_Special 11: sp_dash_init (Running Hyakuretsutou) */
const Sint32 pl2f_special_11[] = {
	CG_FRAME(    0,    0,   4,           0,   469, 0x80,   0x31,  ATK,  0x45,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,   4,           0,   470, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(    0,    0,   4,           0,   471, 0x80,   0x31,  ATK,  0x49,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,   4,           0,   472, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(    0,    0,   4,           0,   473, 0x80,   0x31,  ATK,  0x45,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,   4,           0,   474, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(    0,    0,   4,           0,   475, 0x80,   0x31,  ATK,  0x49,          0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,   4,           0,   476, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(    0,    0,   4,           0,   477, 0x80,   0x31,  ATK,  0x45,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,   4,           0,   478, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(    0,    0,   4,           0,   479, 0x80,   0x31,  ATK,  0x49,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   480, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_NEXT( pl2f_special_11 ),
};

/* Char_Special 64: sp_dash_move (Running Hyakuretsutou) */
const Sint32 pl2f_special_64[] = {
	CG_FRAME( 0x80,    1,   1,           0,   299, 0x80,   0x4c,  ATK,  0x00, 0x19 | CSE,    0,   0,     0, 0x0035,  74 ),
	CG_FRAME(    0,    1,   2,           0,   299, 0x80,   0x4c,  ATE,  0x00,       0x03,    0,   0,     0, 0x0035,  58 ),
	CG_FRAME(    0,    1,  48,           0,   300, 0x80,   0x4c,  ATE,  0x00,          0,    0,   0,     0, 0x0036,  58 ),
	CG_FRAME(    0,    0,   4,           0,   301, 0x80,   0x4c,  ATE,  0x00,          0,    0,   0,     0, 0x0037,  57 ),
	CG_FRAME(    0,    0,   4,           0,   306, 0x80,   0x30,  ATE,  0x00,          0,    0,   0,     0, 0x803c,  30 ),
	CG_FRAME(    0,    0,   4,           0,   307, 0x80,   0x31,  ATE,  0x00,          0,    0,   0,     0, 0x003d,  30 ),
};

const Sint32 pl2f_super_07_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   308, 0x80,   0x31,  ATE,  0x00,          0,    0,   0,     0, 0x003e,  30 ),
	CG_NEXT( pl2f_super_07_loop ),
};

/* Char_Special 12 */
const Sint32 pl2f_special_12[] = {
	CG_FRAME(    0,    0,  16,           0,   456, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   4,           0,   457, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   2,           0,   458, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0, 0x80,   1,           0,   481, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  79 ),
};

const Sint32 pl2f_special_12_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   481, 0x80,   0x31,  ATK,  0x45,          0,    0,   0,     0,      0,  79 ),
	CG_FRAME(    0,    0,   2,           0,   482, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,   2,           0,   483, 0x80,   0x31,  ATK,  0x49,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,   2,           0,   484, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,   2,           0,   485, 0x80,   0x31,  ATK,  0x45,          0,    0,   0,     0,      0,  81 ),
	CG_FRAME(    0,    0,   2,           0,   486, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,   2,           0,   487, 0x80,   0x31,  ATK,  0x49,          0,    0,   0,     0,      0,  78 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   488, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_NEXT( pl2f_special_12_loop ),
};

/* Char_Special 18 */
const Sint32 pl2f_special_18[] = {
	CG_FRAME(    0,    1,  32,           0,   304, 0x80,   0x31,  ATE,  0x00,          0,    0,   0,     0, 0x003a,  39 ),
	CG_FRAME(    0,    0,   4,           0,   305, 0x80,   0x31,  ATE,  0x00,          0,    0,   0,     0, 0x003b,  39 ),
	CG_FRAME(    0,    0,   4,           0,   306, 0x80,   0x31,  ATE,  0x00,          0,    0,   0,     0, 0x803c,  89 ),
	CG_FRAME(    0,    0,   4,           0,   307, 0x80,   0x31,  ATE,  0x00,          0,    0,   0,     0, 0x003d,  89 ),
	CG_FRAME(    0,    0,  10, END_OF_DATA,   308, 0x80,   0x31,  ATE,  0x00,          0,    0,   0,     0, 0x003e,  89 ),
	CG_NEXT( pl2f_special_18 ),
};

/* Char_Special 13: shl2f_01_init */
const Sint32 pl2f_special_13[] = {
	CG_FRAME(    0,    1,   2,           0,   608, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   609, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   610, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   611, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   612, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2,           0,   613, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   614, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   615, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   616, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2,           0,   617, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   618, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2,           0,   619, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   620, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2,           0,   621, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   622, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2,           0,   623, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   624, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2f_special_13 ),
};

/* Char_Extra 3: shl2f_03_move, fly_start5 */
const Sint32 pl2f_extra_03[] = {
	CG_FRAME(    0,    0,   6,           0,   489, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   490, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   491, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   492, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   493, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   494, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   495, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2f_extra_03 ),
};

/* Char_Extra 4: shl2f_03_init */
const Sint32 pl2f_extra_04[] = {
	CG_FRAME(    0,    0,   5,           0,   495, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   494, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   493, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   492, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   491, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   490, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   489, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   210, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   211, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   212, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   213, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,   214, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2f_stand_00 ),
};

/* Char_Special 19: sp_09 */
const Sint32 pl2f_special_19[] = {
	CG_FRAME(    0,    0,   4,           0,   489, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   490, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   491, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   492, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   493, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   494, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   495, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   494, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   493, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   492, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   491, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   490, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   489, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2f_special_19 ),
};

/* Char_Extra 5: hrapid_init (Hyper Hyakuretsutou (team hyper)) */
const Sint32 pl2f_extra_05[] = {
	CG_FRAME(    0,    0,   3,           0,   496, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   3,           0,   497, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME( 0x80,    1,  16,           0,   508, 0x80,   0x31,  ATK,  0x00,       0x03,    0,   0,     0,      0,  89 ),
	CG_FRAME( 0x80,    0,   6,           0,   509, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  56 ),
};

const Sint32 pl2f_extra_05_loop[] = {
	CG_FRAME( 0x80,    0,   6, END_OF_DATA,   510, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_NEXT( pl2f_extra_05_loop ),
};

/* Char_Special 10 */
const Sint32 pl2f_special_10[] = {
	CG_FRAME(    0,    0,   6,           0,   496, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   6,           0,   497, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   2,           0,   498, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,   2,           0,   499, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,   2,           0,   498, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,   2,           0,   499, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,   2,           0,   498, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,   2,           0,   499, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,   2,           0,   498, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,   2,           0,   499, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    1,   2,           0,   500, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,   2,           0,   501, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,   2,           0,   500, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,   2,           0,   501, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,   2,           0,   500, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,   2,           0,   501, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,   2,           0,   500, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,   2,           0,   501, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    2,   2,           0,   502, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,   2,           0,   503, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,   2,           0,   502, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,   2,           0,   503, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,   2,           0,   502, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,   2,           0,   503, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,   2,           0,   502, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,   2,           0,   503, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    3,   2,           0,   504, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,   2,           0,   505, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,   2,           0,   504, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,   2,           0,   505, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,   2,           0,   504, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,   2,           0,   505, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,   2,           0,   504, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,   2,           0,   505, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    4,   2,           0,   506, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,   2,           0,   507, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,   2,           0,   506, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,   2,           0,   507, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,   2,           0,   506, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,   2,           0,   507, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,   2,           0,   506, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,   2,           0,   507, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0, 0x80,   6,           0,   508, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,   6,           0,   509, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   510, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_NEXT( pl2f_special_10 ),
};

/* Char_Special 15: bushin_init (Bushin (hyper): a chain of after-images; its command is never checked) */
const Sint32 pl2f_special_15[] = {
	CG_FRAME(    0,    0,   2,           0,   502, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   503, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_NEXT( pl2f_special_15 ),
};

/* Char_Special 16: bushin_move (Bushin (hyper): a chain of after-images; its command is never checked)
   Char_Special 17 */
const Sint32 pl2f_special_16[] = {
	CG_FRAME(    0,    0,   6,           0,   508, 0x80,   0x31,  ATE,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,   6,           0,   509, 0x80,   0x31,  ATE,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   510, 0x80,   0x31,  ATE,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_NEXT( pl2f_special_16 ),
};

/* Char_Special 7
   Char_Special 21 */
const Sint32 pl2f_special_07[] = {
	CG_FRAME(    0,    0,   6,           0, 32768, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 129 ),
	CG_FRAME(    0,    0,   6,           0, 32769, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 129 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA, 32770, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 129 ),
	CG_NEXT( pl2f_special_07 ),
};

/* Char_Special 29 */
const Sint32 pl2f_special_29[] = {
	CG_FRAME(    0,    0,   6,           0, 32768, 0x80,   0x00,  ATK,  0x00,          0,    1,   0,     0,      0, 129 ),
	CG_FRAME(    0,    0,   6,           0, 32769, 0x80,   0x00,  ATK,  0x00,          0,    1,   0,     0,      0, 129 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA, 32770, 0x80,   0x00,  ATK,  0x00,          0,    1,   0,     0,      0, 129 ),
	CG_NEXT( pl2f_special_29 ),
};

/* Char_Special 8
   Char_Special 22 */
const Sint32 pl2f_special_08[] = {
	CG_FRAME(    0,    0,   6,           0, 32771, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 130 ),
	CG_FRAME(    0,    0,   6,           0, 32772, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 130 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA, 32773, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 130 ),
	CG_NEXT( pl2f_special_08 ),
};

/* Char_Special 30 */
const Sint32 pl2f_special_30[] = {
	CG_FRAME(    0,    0,   6,           0, 32771, 0x80,   0x00,  ATK,  0x00,          0,    1,   0,     0,      0, 130 ),
	CG_FRAME(    0,    0,   6,           0, 32772, 0x80,   0x00,  ATK,  0x00,          0,    1,   0,     0,      0, 130 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA, 32773, 0x80,   0x00,  ATK,  0x00,          0,    1,   0,     0,      0, 130 ),
	CG_NEXT( pl2f_special_30 ),
};

/* Char_Special 9
   Char_Special 23 */
const Sint32 pl2f_special_09[] = {
	CG_FRAME(    0,    0,   6,           0, 32774, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 131 ),
	CG_FRAME(    0,    0,   6,           0, 32775, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 131 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA, 32776, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 131 ),
	CG_NEXT( pl2f_special_09 ),
};

/* Char_Special 31 */
const Sint32 pl2f_special_31[] = {
	CG_FRAME(    0,    0,   6,           0, 32774, 0x80,   0x00,  ATK,  0x00,          0,    1,   0,     0,      0, 131 ),
	CG_FRAME(    0,    0,   6,           0, 32775, 0x80,   0x00,  ATK,  0x00,          0,    1,   0,     0,      0, 131 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA, 32776, 0x80,   0x00,  ATK,  0x00,          0,    1,   0,     0,      0, 131 ),
	CG_NEXT( pl2f_special_31 ),
};

/* Char_Special 24 */
const Sint32 pl2f_special_24[] = {
	CG_FRAME(    0,    0,   6,           0, 32777, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 132 ),
	CG_FRAME(    0,    0,   6,           0, 32778, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 132 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA, 32779, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 132 ),
	CG_NEXT( pl2f_special_24 ),
};

/* Char_Special 32 */
const Sint32 pl2f_special_32[] = {
	CG_FRAME(    0,    0,   6,           0, 32777, 0x80,   0x00,  ATK,  0x00,          0,    1,   0,     0,      0, 132 ),
	CG_FRAME(    0,    0,   6,           0, 32778, 0x80,   0x00,  ATK,  0x00,          0,    1,   0,     0,      0, 132 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA, 32779, 0x80,   0x00,  ATK,  0x00,          0,    1,   0,     0,      0, 132 ),
	CG_NEXT( pl2f_special_32 ),
};

/* Char_Special 25 */
const Sint32 pl2f_special_25[] = {
	CG_FRAME(    0,    0,   6,           0, 32780, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 133 ),
	CG_FRAME(    0,    0,   6,           0, 32781, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 133 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA, 32782, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 133 ),
	CG_NEXT( pl2f_special_25 ),
};

/* Char_Special 33 */
const Sint32 pl2f_special_33[] = {
	CG_FRAME(    0,    0,   6,           0, 32780, 0x80,   0x00,  ATK,  0x00,          0,    1,   0,     0,      0, 133 ),
	CG_FRAME(    0,    0,   6,           0, 32781, 0x80,   0x00,  ATK,  0x00,          0,    1,   0,     0,      0, 133 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA, 32782, 0x80,   0x00,  ATK,  0x00,          0,    1,   0,     0,      0, 133 ),
	CG_NEXT( pl2f_special_33 ),
};

/* Char_Special 26 */
const Sint32 pl2f_special_26[] = {
	CG_FRAME(    0,    0,   6,           0, 32783, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 134 ),
	CG_FRAME(    0,    0,   6,           0, 32784, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 134 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA, 32785, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 134 ),
	CG_NEXT( pl2f_special_26 ),
};

/* Char_Special 34 */
const Sint32 pl2f_special_34[] = {
	CG_FRAME(    0,    0,   6,           0, 32783, 0x80,   0x00,  ATK,  0x00,          0,    1,   0,     0,      0, 134 ),
	CG_FRAME(    0,    0,   6,           0, 32784, 0x80,   0x00,  ATK,  0x00,          0,    1,   0,     0,      0, 134 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA, 32785, 0x80,   0x00,  ATK,  0x00,          0,    1,   0,     0,      0, 134 ),
	CG_NEXT( pl2f_special_34 ),
};

/* Char_Special 27 */
const Sint32 pl2f_special_27[] = {
	CG_FRAME(    0,    0,   6,           0, 32786, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 135 ),
	CG_FRAME(    0,    0,   6,           0, 32787, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 135 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA, 32788, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 135 ),
	CG_NEXT( pl2f_special_27 ),
};

/* Char_Special 35 */
const Sint32 pl2f_special_35[] = {
	CG_FRAME(    0,    0,   6,           0, 32786, 0x80,   0x00,  ATK,  0x00,          0,    1,   0,     0,      0, 135 ),
	CG_FRAME(    0,    0,   6,           0, 32787, 0x80,   0x00,  ATK,  0x00,          0,    1,   0,     0,      0, 135 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA, 32788, 0x80,   0x00,  ATK,  0x00,          0,    1,   0,     0,      0, 135 ),
	CG_NEXT( pl2f_special_35 ),
};

/* Char_Special 36 */
const Sint32 pl2f_special_36[] = {
	CG_FRAME(    0,    0,   6,           0, 32789, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 136 ),
	CG_FRAME(    0,    0,   6,           0, 32790, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 136 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA, 32791, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 136 ),
	CG_NEXT( pl2f_special_36 ),
};

/* Char_Special 28 */
const Sint32 pl2f_special_28[] = {
	CG_FRAME(    0,    0,   6,           0, 32789, 0x80,   0x00,  ATK,  0x00,          0,    1,   0,     0,      0, 136 ),
	CG_FRAME(    0,    0,   6,           0, 32790, 0x80,   0x00,  ATK,  0x00,          0,    1,   0,     0,      0, 136 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA, 32791, 0x80,   0x00,  ATK,  0x00,          0,    1,   0,     0,      0, 136 ),
	CG_NEXT( pl2f_special_28 ),
};

/* Char_Super 1: hyouga_init (Hyougaken (hyper, ice element)) */
const Sint32 pl2f_super_01[] = {
	CG_FRAME(    0,    0,   1,           0,   168, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,   2,           0,   426, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0, 0x00a2, 100 ),
	CG_FRAME(    0,    0,   2,           0,   427, 0x80,   0x31,  ATK,  0x00, 0x19 | CSE,    0,   0,     0, 0x00a3, 100 ),
	CG_FRAME(    0,    0,   2,           0,   428, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0, 0x00a4,   7 ),
	CG_FRAME(    0,    1,  32,           0,   429, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0, 0x00a5,   7 ),
};

const Sint32 pl2f_super_01_loop[] = {
	CG_FRAME(    0,    0,  16, END_OF_DATA,   391, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0, 0x8091,  35 ),
	CG_NEXT( pl2f_super_01_loop ),
};

/* Char_Super 2 */
const Sint32 pl2f_super_02[] = {
	CG_FRAME(    0,    0,   4,           0, 32820, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 159 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA, 32821, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 159 ),
	CG_NEXT( pl2f_super_02 ),
};

/* Char_Super 3 */
const Sint32 pl2f_super_03[] = {
	CG_FRAME(    0,    0,   4,           0, 32829, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 159 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA, 32830, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 159 ),
	CG_NEXT( pl2f_super_03 ),
};

/* Char_Super 4 */
const Sint32 pl2f_super_04[] = {
	CG_FRAME(    0,    0,   4,           0, 32838, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 159 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA, 32839, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 159 ),
	CG_NEXT( pl2f_super_04 ),
};

/* Char_Super 5 */
const Sint32 pl2f_super_05[] = {
	CG_FRAME(    0,    0,   4,           0, 32849, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 159 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA, 32850, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 159 ),
	CG_NEXT( pl2f_super_05 ),
};

/* Char_Super 8: shl2f_04_init (8 + sub_type) */
const Sint32 pl2f_super_08[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA, 32869, 0x80,   0x00,  ATK,  0x00,          0,    1,   0,     0,      0, 159 ),
	CG_NEXT( pl2f_super_08 ),
};

/* Char_Super 9 */
const Sint32 pl2f_super_09[] = {
	CG_FRAME(    0,    0,   1,           0, 32858, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 159 ),
	CG_FRAME(    0,    0,   1,           0, 32859, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 159 ),
	CG_FRAME(    0,    0,   1,           0, 32860, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 159 ),
	CG_FRAME(    0,    0,   1,           0, 32861, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 159 ),
	CG_FRAME(    0,    0,   1,           0, 32862, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 159 ),
	CG_FRAME(    0,    1,   1,           0, 32863, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 159 ),
	CG_FRAME(    0,    0,   2,           0, 32864, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32865, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32866, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32867, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32868, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32858, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2f_super_09 ),
};

/* Char_Super 6: shl2f_05_init */
const Sint32 pl2f_super_06[] = {
	CG_FRAME(    0,    0,   2,           0, 32801, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 160 ),
	CG_FRAME(    0,    0,   8,           0, 32802, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 160 ),
	CG_FRAME(    0,    0,   8,           0, 32803, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 160 ),
	CG_FRAME(    1,    1,   4,           0, 32804, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 160 ),
	CG_FRAME(    0,    1,   4,           0, 32805, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 160 ),
	CG_FRAME(    0,    1,   4,           0, 32806, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 160 ),
};

const Sint32 pl2f_super_06_loop[] = {
	CG_FRAME(    0,    1,   4, END_OF_DATA, 32807, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 160 ),
	CG_NEXT( pl2f_super_06_loop ),
};

/* Char_Special 41 */
const Sint32 pl2f_special_41[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA,   456, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_NEXT( pl2f_special_53 ),
};

/* Char_Special 45 */
const Sint32 pl2f_special_45[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   456, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_NEXT( pl2f_special_53 ),
};

/* Char_Special 49 */
const Sint32 pl2f_special_49[] = {
	CG_FRAME(    0,    0,  16, END_OF_DATA,   456, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_NEXT( pl2f_special_53 ),
};

/* Char_Special 43 */
const Sint32 pl2f_special_43[] = {
	CG_FRAME(    0,    0,  16, END_OF_DATA,   456, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_NEXT( pl2f_special_54 ),
};

/* Char_Special 47 */
const Sint32 pl2f_special_47[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA,   456, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_NEXT( pl2f_special_54 ),
};

/* Char_Special 51 */
const Sint32 pl2f_special_51[] = {
	CG_FRAME(    0,    0,  32, END_OF_DATA,   456, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_NEXT( pl2f_special_54 ),
};

/* Char_Special 53 */
const Sint32 pl2f_special_53[] = {
	CG_FRAME(    0, 0x80,   1,           0,   481, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  77 ),
};

const Sint32 pl2f_special_53_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   481, 0x80,   0x31,  ATK,  0x45,          0,    0,   0,     0,      0,  79 ),
	CG_FRAME(    0,    0,   4,           0,   482, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,   4,           0,   483, 0x80,   0x31,  ATK,  0x49,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,   4,           0,   484, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,   4,           0,   485, 0x80,   0x31,  ATK,  0x45,          0,    0,   0,     0,      0,  81 ),
	CG_FRAME(    0,    0,   4,           0,   486, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,   4,           0,   487, 0x80,   0x31,  ATK,  0x49,          0,    0,   0,     0,      0,  78 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   488, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_NEXT( pl2f_special_53_loop ),
};

/* Char_Special 54 */
const Sint32 pl2f_special_54[] = {
	CG_FRAME(    0, 0x80,   1,           0,   481, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  77 ),
};

const Sint32 pl2f_special_54_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   481, 0x80,   0x31,  ATK,  0x4d,          0,    0,   0,     0,      0,  79 ),
	CG_FRAME(    0,    0,   2,           0,   482, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,   2,           0,   483, 0x80,   0x31,  ATK,  0x51,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,   2,           0,   484, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,   2,           0,   485, 0x80,   0x31,  ATK,  0x4d,          0,    0,   0,     0,      0,  81 ),
	CG_FRAME(    0,    0,   2,           0,   486, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,   2,           0,   487, 0x80,   0x31,  ATK,  0x51,          0,    0,   0,     0,      0,  78 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   488, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_NEXT( pl2f_special_54_loop ),
};

/* Char_Special 42 */
const Sint32 pl2f_special_42[] = {
	CG_FRAME(    0,    0,  16, END_OF_DATA,   304, 0x80,   0x31,  ATE,  0x00,          0,    0,   0,     0, 0x003a,  39 ),
	CG_NEXT( pl2f_special_55 ),
};

/* Char_Special 46 */
const Sint32 pl2f_special_46[] = {
	CG_FRAME(    0,    0,  24, END_OF_DATA,   304, 0x80,   0x31,  ATE,  0x00,          0,    0,   0,     0, 0x003a,  39 ),
	CG_NEXT( pl2f_special_55 ),
};

/* Char_Special 50 */
const Sint32 pl2f_special_50[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA,   304, 0x80,   0x31,  ATE,  0x00,          0,    0,   0,     0, 0x003a,  39 ),
	CG_NEXT( pl2f_special_55 ),
};

/* Char_Special 44 */
const Sint32 pl2f_special_44[] = {
	CG_FRAME(    0,    0,  16, END_OF_DATA,   304, 0x80,   0x31,  ATE,  0x00,          0,    0,   0,     0, 0x003a,  39 ),
	CG_NEXT( pl2f_special_55 ),
};

/* Char_Special 48 */
const Sint32 pl2f_special_48[] = {
	CG_FRAME(    0,    0,  24, END_OF_DATA,   304, 0x80,   0x31,  ATE,  0x00,          0,    0,   0,     0, 0x003a,  39 ),
	CG_NEXT( pl2f_special_55 ),
};

/* Char_Special 52 */
const Sint32 pl2f_special_52[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA,   304, 0x80,   0x31,  ATE,  0x00,          0,    0,   0,     0, 0x003a,  39 ),
	CG_NEXT( pl2f_special_55 ),
};

/* Char_Special 55 */
const Sint32 pl2f_special_55[] = {
	CG_FRAME(    0,    0,   4,           0,   305, 0x80,   0x31,  ATE,  0x00,          0,    0,   0,     0, 0x003b,  39 ),
	CG_FRAME(    0,    0,   4,           0,   306, 0x80,   0x31,  ATE,  0x00,          0,    0,   0,     0, 0x803c,  89 ),
	CG_FRAME(    0,    0,   4,           0,   307, 0x80,   0x31,  ATE,  0x00,          0,    0,   0,     0, 0x003d,  89 ),
};

const Sint32 pl2f_special_55_loop[] = {
	CG_FRAME(    0,    0,  10, END_OF_DATA,   308, 0x80,   0x31,  ATE,  0x00,          0,    0,   0,     0, 0x003e,  89 ),
	CG_NEXT( pl2f_special_55_loop ),
};

/* Char_Super 7: hrapid_move (Hyper Hyakuretsutou (team hyper)) */
const Sint32 pl2f_super_07[] = {
	CG_FRAME( 0x80,    1,   1,           0,   299, 0x80,   0x4c,  ATK,  0x00, 0x19 | CSE,    0,   0,     0, 0x0035, 165 ),
	CG_FRAME(    0,    1,   2,           0,   299, 0x80,   0x4c,  ATE,  0x00,       0x03,    0,   0,     0, 0x0035,  58 ),
	CG_FRAME(    0,    1,  48,           0,   300, 0x80,   0x4c,  ATE,  0x00,          0,    0,   0,     0, 0x0036,  58 ),
	CG_FRAME(    0,    1,   4,           0,   301, 0x80,   0x4c,  ATE,  0x00,          0,    0,   0,     0, 0x0037,  57 ),
	CG_FRAME(    0,    1,   4,           0,   306, 0x80,   0x30,  ATE,  0x00,          0,    0,   0,     0, 0x803c,  30 ),
	CG_FRAME(    0,    1,   4,           0,   307, 0x80,   0x31,  ATE,  0x00,          0,    0,   0,     0, 0x003d,  30 ),
	CG_FRAME(    0,    1,   4, END_OF_DATA,   308, 0x80,   0x31,  ATE,  0x00,          0,    0,   0,     0, 0x003e,  30 ),
	CG_NEXT( pl2f_super_07_loop ),
};

/* Char_Effect 0 */
const Sint32 pl2f_effect_00[] = {
	CG_FRAME(    0,    0,   2,           0,   660, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 151 ),
	CG_FRAME(    0,    0,   2,           0,   662, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   661, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2f_effect_00 ),
};

/* Char_Change 0: act_change_tag_init */
const Sint32 pl2f_change_00[] = {
	CG_FRAME(    0,    0,   4,           0,   286, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   287, 0x80,   0x31,  ATK,  0x00,       0x15,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   288, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   289, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   4,           0,   290, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   288, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   289, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   290, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   288, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   289, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   290, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2f_change_00 ),
};

/* Char_Change 1: act_change_vcounter_init */
const Sint32 pl2f_change_01[] = {
	CG_FRAME( 0x80,    0,   4,           0,   284, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME( 0x82,    0,   4, END_OF_DATA,   285, 0x80,   0x31,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2f_change_01 ),
};

/* Char_Change 3: act_stand, act_change_call */
const Sint32 pl2f_change_03[] = {
	CG_FRAME(    0,    0,   2,           0,   515, 0x80,   0x31,    0,  0x00,       0x15,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   2,           0,   516, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   2,           0,   515, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   2,           0,   516, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   2,           0,   515, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   2,           0,   516, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  89 ),
};

const Sint32 pl2f_change_03_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   515, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   516, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_NEXT( pl2f_change_03_loop ),
};

/* Char_Change 4: act_change_assist_end */
const Sint32 pl2f_change_04[] = {
	CG_FRAME(    0,    0,   5,           0,   286, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   5,           0,   287, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   5,           0,   288, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   5,           0,   289, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   5,           0,   290, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   5,           0,   288, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   5,           0,   289, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,   290, 0x80,   0x31,    0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_NEXT( pl2f_change_04 ),
};

/* group 27 0: pls2f_03_init */
const Sint32 pl2f_grp27_00[] = {
	EF_FRAME(    0,    0,   2,           0,   660 ),
	EF_FRAME(    0,    0,   2,           0,   662 ),
	EF_FRAME(    0,    0,   2,           0,   661 ),
	EF_FRAME(    0,    0,   2,           0,   660 ),
	EF_FRAME(    0,    0,   2,           0,   662 ),
	EF_FRAME(    0,    0,   2,           0,   661 ),
	EF_FRAME(    0,    0,   2,           0,   660 ),
	EF_FRAME(    0,    0,   2,           0,   662 ),
	EF_FRAME(    0,    0,   2,           0,   661 ),
	EF_FRAME(    0,    0,   2,           0,   660 ),
	EF_FRAME(    0,    0,   2,           0,   662 ),
	EF_FRAME(    0,    0,   2,           0,   661 ),
	EF_FRAME(    0,    0,   2,           0,   660 ),
	EF_FRAME(    0,    0,   2,           0,   662 ),
	EF_FRAME(    0,    0,   2,           0,   661 ),
	EF_FRAME(    0,    0,   2,           0,   660 ),
	EF_FRAME(    0,    0,   2,           0,   662 ),
	EF_FRAME(    0,    0,   2, END_OF_DATA,   661 ),
	CG_NEXT( pl2f_grp27_00 ),
};

/* group 27 1 */
const Sint32 pl2f_grp27_01[] = {
	EF_FRAME(    0,    0,   1,           0,   625 ),
	EF_FRAME(    0,    0,   1,           0,   626 ),
	EF_FRAME(    0,    0,   1,           0,   627 ),
	EF_FRAME(    0,    0,   1,           0,   628 ),
	EF_FRAME(    0,    0,   1, END_OF_DATA,   629 ),
	CG_NEXT( pl2f_grp27_01 ),
};

/* group 27 2 */
const Sint32 pl2f_grp27_02[] = {
	EF_FRAME(    0,    0,   1, END_OF_DATA,   629 ),
	CG_NEXT( pl2f_grp27_02 ),
};

/* group 27 3 */
const Sint32 pl2f_grp27_03[] = {
	EF_FRAME(    0,    0,   1,           0,   630 ),
	EF_FRAME(    0,    0,   1,           0,   631 ),
	EF_FRAME(    0,    0,   1,           0,   632 ),
	EF_FRAME(    0,    0,   1,           0,   633 ),
	EF_FRAME(    0,    0,   1,           0,   634 ),
	EF_FRAME(    0,    0,   1,           0,   635 ),
	EF_FRAME(    0,    0,   1,           0,   636 ),
	EF_FRAME(    0,    0,   1,           0,   637 ),
	EF_FRAME(    0,    0,   1,           0,   638 ),
	EF_FRAME(    0,    0,   1,           0,   639 ),
	EF_FRAME(    0,    0,   1, END_OF_DATA,   640 ),
	CG_NEXT( pl2f_grp27_03 ),
};

/* group 27 4: pls2f_01_init, pls2f_05_keep (4 + char_no.b.no), pls2f_05_move2 (4 + char_no.b.no) */
const Sint32 pl2f_grp27_04[] = {
	EF_FRAME(    0,    0,   2,           0,   641 ),
	EF_FRAME(    0,    0,   2,           0,   642 ),
	EF_FRAME(    0,    0,   2,           0,   643 ),
	EF_FRAME(    0,    0,   2,           0,   644 ),
	EF_FRAME(    0,    0,   2,           0,   645 ),
	EF_FRAME(    0,    0,   2,           0,   646 ),
	EF_FRAME(    0,    0,   2,           0,   647 ),
	EF_FRAME(    0,    0,   2,           0,   648 ),
	EF_FRAME(    0,    0,   2,           0,   649 ),
	EF_FRAME(    0,    0,   2,           0,   650 ),
	EF_FRAME(    0,    0,   2,           0,   651 ),
	EF_FRAME(    0,    0,   2,           0,   652 ),
	EF_FRAME(    0,    0,   2,           0,   653 ),
	EF_FRAME(    0,    0,   2,           0,   654 ),
	EF_FRAME(    0,    0,   2,           0,   655 ),
	EF_FRAME(    0,    0,   2,           0,   656 ),
	EF_FRAME(    0,    0,   2,           0,   657 ),
	EF_FRAME(    0,    0,   2,           0,   658 ),
	EF_FRAME(    0,    0,   2, END_OF_DATA,   659 ),
	CG_NEXT( pl2f_grp27_04 ),
};

/* group 27 5 */
const Sint32 pl2f_grp27_05[] = {
	EF_FRAME(    0,    0,   2,           0, 32801 ),
	EF_FRAME(    0,    0,   2,           0, 32802 ),
	EF_FRAME(    0,    0,   2,           0, 32803 ),
	EF_FRAME(    0,    0,   2,           0, 32804 ),
	EF_FRAME(    0,    0,   2,           0, 32805 ),
	EF_FRAME(    0,    0,   2,           0, 32806 ),
	EF_FRAME(    0,    0,   2, END_OF_DATA, 32807 ),
	CG_NEXT( pl2f_grp27_05 ),
};

/* group 27 6: pls2f_02_init (6 + sub_type) */
const Sint32 pl2f_grp27_06[] = {
	EF_FRAME(    0,    0,   4,           0, 32820 ),
	EF_FRAME(    0,    0,   4, END_OF_DATA, 32821 ),
	CG_NEXT( pl2f_grp27_06 ),
};

/* group 27 7 */
const Sint32 pl2f_grp27_07[] = {
	EF_FRAME(    0,    0,   4,           0, 32829 ),
	EF_FRAME(    0,    0,   4, END_OF_DATA, 32830 ),
	CG_NEXT( pl2f_grp27_07 ),
};

/* group 27 8 */
const Sint32 pl2f_grp27_08[] = {
	EF_FRAME(    0,    0,   4,           0, 32838 ),
	EF_FRAME(    0,    0,   4, END_OF_DATA, 32839 ),
	CG_NEXT( pl2f_grp27_08 ),
};

/* group 27 9 */
const Sint32 pl2f_grp27_09[] = {
	EF_FRAME(    0,    0,   4,           0, 32849 ),
	EF_FRAME(    0,    0,   4, END_OF_DATA, 32850 ),
	CG_NEXT( pl2f_grp27_09 ),
};

/* group 27 10: pls2f_02_t1 (10 + sub_type) */
const Sint32 pl2f_grp27_10[] = {
	EF_FRAME(    0,    1,   4,           0, 32822 ),
	EF_FRAME(    0,    0,   4,           0, 32823 ),
	EF_FRAME(    0,    0,   4,           0, 32824 ),
	EF_FRAME(    0,    0,   4,           0, 32825 ),
	EF_FRAME(    0,    0,   4,           0, 32826 ),
	EF_FRAME(    0,    0,   4,           0, 32827 ),
	EF_FRAME(    0,    0,   4, END_OF_DATA, 32828 ),
	CG_NEXT( pl2f_grp27_10 ),
};

/* group 27 11 */
const Sint32 pl2f_grp27_11[] = {
	EF_FRAME(    0,    1,   4,           0, 32831 ),
	EF_FRAME(    0,    0,   4,           0, 32832 ),
	EF_FRAME(    0,    0,   4,           0, 32833 ),
	EF_FRAME(    0,    0,   4,           0, 32834 ),
	EF_FRAME(    0,    0,   4,           0, 32835 ),
	EF_FRAME(    0,    0,   4,           0, 32836 ),
	EF_FRAME(    0,    0,   4, END_OF_DATA, 32837 ),
	CG_NEXT( pl2f_grp27_11 ),
};

/* group 27 12 */
const Sint32 pl2f_grp27_12[] = {
	EF_FRAME(    0,    1,   4,           0, 32840 ),
	EF_FRAME(    0,    0,   4,           0, 32841 ),
	EF_FRAME(    0,    0,   4,           0, 32842 ),
	EF_FRAME(    0,    0,   4,           0, 32843 ),
	EF_FRAME(    0,    0,   4,           0, 32844 ),
	EF_FRAME(    0,    0,   4,           0, 32845 ),
	EF_FRAME(    0,    0,   4, END_OF_DATA, 32846 ),
	CG_NEXT( pl2f_grp27_12 ),
};

/* group 27 13 */
const Sint32 pl2f_grp27_13[] = {
	EF_FRAME(    0,    1,   4,           0, 32851 ),
	EF_FRAME(    0,    0,   4,           0, 32852 ),
	EF_FRAME(    0,    0,   4,           0, 32853 ),
	EF_FRAME(    0,    0,   4,           0, 32854 ),
	EF_FRAME(    0,    0,   4,           0, 32855 ),
	EF_FRAME(    0,    0,   4,           0, 32856 ),
	EF_FRAME(    0,    0,   4, END_OF_DATA, 32857 ),
	CG_NEXT( pl2f_grp27_13 ),
};
