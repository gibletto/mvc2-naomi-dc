/* Roll (character 1d): command checks, normal attacks, appearance, win and lose poses, special
   moves and hyper combos, throws and the partner moves. */
#include "player.h"
#include "pl_char.h"
#include "pl_color.h"
#include "o_sub1.h"
#include "pl1d_def.h"

void pl1d_main( PLAYER_WORK *pl_w );
void pl1d_com_ck( PLAYER_WORK *pl_w );
void pl1d_atck_set( PLAYER_WORK *pl_w );
void pl1d_atck( PLAYER_WORK *pl_w );
void pl1d_dash( PLAYER_WORK *pl_w );
void pl1d_noki( PLAYER_WORK *pl_w );
void pl1d_appear( PLAYER_WORK *pl_w );
void pl1d_win( PLAYER_WORK *pl_w );
void pl1d_sp_atck( PLAYER_WORK *pl_w );
PLAYER_WORK *pl1d_catch_ck( PLAYER_WORK *pl_w );
void pl1d_catch_set( PLAYER_WORK *pl_w );
void pl1d_catch( PLAYER_WORK *pl_w );
void pl1d_hold( PLAYER_WORK *pl_w );
int pl1d_delay_ck( PLAYER_WORK *pl_w );
void pl1d_t_kick( PLAYER_WORK *pl_w );
void pl1d_upper( PLAYER_WORK *pl_w );
void pl1d_sp_unison( PLAYER_WORK *pl_w );
void pl1d_sp_gattai( PLAYER_WORK *pl_w );
void pl1d_sp_assist( PLAYER_WORK *pl_w );
void pl1d_sp_zero( PLAYER_WORK *pl_w );
int pl1d_shot_end( PLAYER_WORK *pl_w );
int pl1d_item_end( PLAYER_WORK *pl_w );
void pl1d_tlocal_init( PLAYER_WORK *pl_w );

/* each attack's table: standing and crouching, jumping, super jumping */
static Sint8 lp_tbl[4] = {  1,  2,  1,  2 } ;
static Sint8 mp_tbl[4] = { -1,  2, -1,  2 } ;
static Sint8 hp_tbl[4] = { -1, -1, -1, -1 } ;
static Sint8 lk_tbl[4] = {  1,  2,  1,  2 } ;
static Sint8 mk_tbl[4] = { -1,  2, -1,  2 } ;
static Sint8 hk_tbl[4] = { -1, -1, -1, -1 } ;
static Sint8 j_lp_tbl[4] = {  1,  2,  1,  2 } ;
static Sint8 j_mp_tbl[4] = { -1,  2, -1,  2 } ;
static Sint8 j_hp_tbl[4] = { -1, -1, -1, -1 } ;
static Sint8 j_lk_tbl[4] = {  1,  2,  1,  2 } ;
static Sint8 j_mk_tbl[4] = { -1,  2, -1,  2 } ;
static Sint8 j_hk_tbl[4] = { -1, -1, -1, -1 } ;
static Sint8 sj_lp_tbl[4] = {  1,  2,  0,  2 } ;
static Sint8 sj_mp_tbl[4] = { -1,  2,  1,  2 } ;
static Sint8 sj_hp_tbl[4] = { -1, -1, -1,  2 } ;
static Sint8 sj_lk_tbl[4] = {  1,  2,  1,  2 } ;
static Sint8 sj_mk_tbl[4] = { -1,  2, -1,  2 } ;
static Sint8 sj_hk_tbl[4] = { -1, -1, -1, -1 } ;

/* commands: time or steps, kind, the special move, then the stick and buttons */
static Uint16 com_buster[] = { 3, 0, pl1d_sp_buster, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ; /* Roll Buster */
static Uint16 com_flower[] = { 3, 0, pl1d_sp_flower, 0, KEY_FORWARD, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_PUNCH } ; /* Flowers */
static Uint16 com_hyper_roll[] = { 3, COM_HYPER | COM_ALL_SW, pl1d_sp_hyper_roll, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ; /* Hyper Roll (hyper) */
static Uint16 com_drill[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, pl1d_sp_drill, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_KICK } ; /* Rush Drill (hyper) */
static Uint16 com_beat[] = { 3, COM_GROUND | COM_HYPER | COM_ALL_SW, pl1d_sp_beat, 0, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_KICK } ; /* Beat Plane */
static Uint16 com_item[] = { 3, 0, pl1d_sp_item, 0, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_PUNCH } ; /* Weapon Activation */
static Uint16 com_item_sel_ball[] = { 3, COM_GROUND, pl1d_sp_item_sel, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_KICK } ; /* Summon power-up: Rock Ball */
static Uint16 com_item_sel_tornado[] = { 3, COM_GROUND, pl1d_sp_item_sel, 0, KEY_FORWARD, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_KICK } ; /* Summon power-up: Tornado Hold */
static Uint16 com_item_sel_shield[] = { 3, COM_GROUND, pl1d_sp_item_sel, 0, KEY_DOWN, KEY_DOWN | KEY_BACK, KEY_BACK, KEY_KICK } ; /* Summon power-up: Leaf Shield */

static COLOR_STEP col_00[] = {
	{  1,  2, 0x38,  1,   1 },
	{  1,  2, 0x00,  1,   1 },
	{  1,  0, 0x07,  1,   1 },
	{  1,  0, 0x02,  1,   1 },
	{  1,  0, 0x00,  1,   1 },
	{  1,  0, 0x00,  1,   1 },
	{  4,  0, 0x0a,  1,   1 },
	{  4,  0, 0x09,  1,   1 },
	{  4,  0, 0x08,  1,   1 },
	{  4,  0, 0x07,  1,   1 },
	{  4,  0, 0x06,  1,   1 },
	{ 16,  0, 0x05,  1,   0 },
} ;
static COLOR_STEP col_01[] = {
	{  4,  0, 0x09,  1,   1 },
	{  4,  0, 0x08,  1,   1 },
	{  4,  0, 0x07,  1,   1 },
	{  4,  0, 0x06,  1,   1 },
	{  3,  0, 0x05,  1,   0 },
} ;
static COLOR_STEP col_02[] = {
	{  4,  0, 0x02,  1,   1 },
	{  4,  0, 0x03,  1,   1 },
	{  4,  0, 0x04,  1,   1 },
	{  4,  0, 0x05,  1,   1 },
	{  4,  0, 0x06,  1,   1 },
	{  4,  0, 0x07,  1,   1 },
	{  4,  0, 0x08,  1,   1 },
	{  4,  0, 0x09,  1,   1 },
	{  8,  0, 0x0a,  1,   0 },
} ;
static COLOR_STEP col_03[] = {
	{  0,  0, 0x06,  1,   1 },
	{  0,  1, 0x0f,  1,   1 },
	{  4,  2, 0x18,  1,   1 },
	{  0,  0, 0x07,  1,   1 },
	{  0,  1, 0x10,  1,   1 },
	{  4,  2, 0x19,  1,   1 },
	{  0,  0, 0x08,  1,   1 },
	{  0,  1, 0x11,  1,   1 },
	{  4,  2, 0x1a,  1,   1 },
	{  0,  0, 0x09,  1,   1 },
	{  0,  1, 0x12,  1,   1 },
	{  4,  2, 0x1b,  1,   1 },
	{  0,  0, 0x0a,  1,   1 },
	{  0,  1, 0x13,  1,   1 },
	{ 16,  2, 0x1c,  1,  -2 },
} ;
static COLOR_STEP col_04[] = {
	{  3,  0, 0x0a,  1,   1 },
	{  3,  0, 0x09,  1,   1 },
	{  3,  0, 0x08,  1,   1 },
	{  3,  0, 0x07,  1,   1 },
	{  3,  0, 0x06,  1,   1 },
	{  3,  0, 0x05,  1,   1 },
	{  3,  0, 0x04,  1,   1 },
	{  3,  0, 0x03,  1,   1 },
	{  3,  0, 0x02,  1,   1 },
	{  3,  0, 0x03,  1,   1 },
	{  3,  0, 0x04,  1,   1 },
	{  3,  0, 0x05,  1,   1 },
	{  3,  0, 0x06,  1,   1 },
	{  3,  0, 0x07,  1,   1 },
	{  3,  0, 0x08,  1,   1 },
	{  3,  0, 0x09,  1, -15 },
} ;
static COLOR_STEP col_05[] = {
	{  3,  0, 0x03,  1,   1 },
	{  3,  0, 0x04,  1,   1 },
	{  3,  0, 0x05,  1,   1 },
	{  3,  0, 0x06,  1,   1 },
	{  3,  0, 0x07,  1,   1 },
	{  3,  0, 0x08,  1,   1 },
	{  3,  0, 0x09,  1,   1 },
	{  3,  0, 0x0a,  1,   1 },
	{  3,  0, 0x09,  1,   1 },
	{  3,  0, 0x08,  1,   1 },
	{  3,  0, 0x07,  1,   1 },
	{  3,  0, 0x06,  1,   1 },
	{  3,  0, 0x05,  1,   1 },
	{  3,  0, 0x04,  1,   1 },
	{  3,  0, 0x03,  1,   1 },
	{  3,  0, 0x02,  1, -15 },
} ;
static COLOR_STEP col_06[] = {
	{  0,  0, 0x0a,  1,   1 },
	{  0,  1, 0x13,  1,   1 },
	{ 10,  2, 0x1c,  1,   1 },
	{  0,  0, 0x09,  1,   1 },
	{  0,  1, 0x12,  1,   1 },
	{  3,  2, 0x1b,  1,   1 },
	{  0,  0, 0x08,  1,   1 },
	{  0,  1, 0x11,  1,   1 },
	{  3,  2, 0x1a,  1,   1 },
	{  0,  0, 0x07,  1,   1 },
	{  0,  1, 0x10,  1,   1 },
	{  3,  2, 0x19,  1,   1 },
	{  0,  0, 0x06,  1,   1 },
	{  0,  1, 0x0f,  1,   1 },
	{  3,  2, 0x18,  1,   1 },
	{  0,  0, 0x05,  1,   1 },
	{  0,  1, 0x0e,  1,   1 },
	{ 255,  2, 0x17,  1,  -2 },
} ;
static COLOR_STEP col_07[] = {
	{  0,  0, 0x06,  1,   1 },
	{  4,  1, 0x0f,  1,   1 },
	{  0,  0, 0x07,  1,   1 },
	{  4,  1, 0x10,  1,   1 },
	{  0,  0, 0x08,  1,   1 },
	{  4,  1, 0x11,  1,   1 },
	{  0,  0, 0x09,  1,   1 },
	{  4,  1, 0x12,  1,   1 },
	{  0,  0, 0x0a,  1,   1 },
	{ 16,  1, 0x13,  1,  -1 },
} ;
static COLOR_STEP col_08[] = {
	{  0,  0, 0x0a,  1,   1 },
	{  0,  1, 0x37,  1,   1 },
	{  4,  2, 0x54,  1,   1 },
	{  0,  0, 0x09,  1,   1 },
	{  0,  1, 0x36,  1,   1 },
	{  4,  2, 0x53,  1,   1 },
	{  0,  0, 0x08,  1,   1 },
	{  0,  1, 0x36,  1,   1 },
	{  4,  2, 0x52,  1,   1 },
	{  0,  0, 0x07,  1,   1 },
	{  0,  1, 0x34,  1,   1 },
	{  4,  2, 0x51,  1,   1 },
	{  0,  0, 0x06,  1,   1 },
	{  0,  1, 0x33,  1,   1 },
	{  4,  2, 0x50,  1,   1 },
	{  0,  0, 0x06,  1,   1 },
	{  0,  1, 0x32,  1,   1 },
	{ 255,  2, 0x00,  1,  -2 },
} ;
static COLOR_STEP col_09[] = {
	{  0,  0, 0x06,  1,   1 },
	{  0,  1, 0x33,  1,   1 },
	{  3,  2, 0x50,  1,   1 },
	{  0,  0, 0x07,  1,   1 },
	{  0,  1, 0x34,  1,   1 },
	{  3,  2, 0x51,  1,   1 },
	{  0,  0, 0x08,  1,   1 },
	{  0,  1, 0x35,  1,   1 },
	{  3,  2, 0x52,  1,   1 },
	{  0,  0, 0x09,  1,   1 },
	{  0,  1, 0x36,  1,   1 },
	{  3,  2, 0x53,  1,   1 },
	{  0,  0, 0x0a,  1,   1 },
	{  0,  1, 0x37,  1,   1 },
	{ 14,  2, 0x54,  1,  -2 },
} ;
static COLOR_STEP col_10[] = {
	{  0,  0, 0x0a,  1,   1 },
	{ 10,  1, 0x13,  1,   1 },
	{  0,  0, 0x09,  1,   1 },
	{  3,  1, 0x12,  1,   1 },
	{  0,  0, 0x08,  1,   1 },
	{  3,  1, 0x11,  1,   1 },
	{  0,  0, 0x07,  1,   1 },
	{  3,  1, 0x10,  1,   1 },
	{  0,  0, 0x06,  1,   1 },
	{  3,  1, 0x0f,  1,   1 },
	{  0,  0, 0x05,  1,   1 },
	{ 255,  1, 0x0e,  1,  -1 },
} ;
static COLOR_STEP col_11[] = {
	{  0,  0, 0x06,  1,   1 },
	{  4,  2, 0x18,  1,   1 },
	{  0,  0, 0x07,  1,   1 },
	{  4,  2, 0x19,  1,   1 },
	{  0,  0, 0x08,  1,   1 },
	{  4,  2, 0x1a,  1,   1 },
	{  0,  0, 0x09,  1,   1 },
	{  4,  2, 0x1b,  1,   1 },
	{  0,  0, 0x0a,  1,   1 },
	{ 16,  2, 0x1c,  1,  -1 },
} ;
static COLOR_STEP col_12[] = {
	{  0,  0, 0x0a,  1,   1 },
	{  4,  2, 0x25,  1,   1 },
	{  0,  0, 0x09,  1,   1 },
	{  4,  2, 0x24,  1,   1 },
	{  0,  0, 0x08,  1,   1 },
	{  4,  2, 0x23,  1,   1 },
	{  0,  0, 0x07,  1,   1 },
	{  4,  2, 0x22,  1,   1 },
	{  0,  0, 0x06,  1,   1 },
	{  4,  2, 0x21,  1,   1 },
	{  0,  0, 0x05,  1,   1 },
	{ 255,  2, 0x20,  1,  -1 },
} ;
static COLOR_STEP col_13[] = {
	{  0,  0, 0x06,  1,   1 },
	{  3,  2, 0x21,  1,   1 },
	{  0,  0, 0x07,  1,   1 },
	{  3,  2, 0x22,  1,   1 },
	{  0,  0, 0x08,  1,   1 },
	{  3,  2, 0x23,  1,   1 },
	{  0,  0, 0x09,  1,   1 },
	{  3,  2, 0x24,  1,   1 },
	{  0,  0, 0x0a,  1,   1 },
	{ 14,  2, 0x25,  1,  -1 },
} ;
static COLOR_STEP col_14[] = {
	{  0,  0, 0x0a,  1,   1 },
	{ 10,  2, 0x1c,  1,   1 },
	{  0,  0, 0x09,  1,   1 },
	{  3,  2, 0x1b,  1,   1 },
	{  0,  0, 0x08,  1,   1 },
	{  3,  2, 0x1a,  1,   1 },
	{  0,  0, 0x07,  1,   1 },
	{  3,  2, 0x19,  1,   1 },
	{  0,  0, 0x06,  1,   1 },
	{  3,  2, 0x18,  1,   1 },
	{  0,  0, 0x05,  1,   1 },
	{ 255,  2, 0x17,  1,  -1 },
} ;
static COLOR_STEP col_15[] = {
	{  0,  0, 0x02,  1,   1 },
	{  0,  1, 0x02,  1,   1 },
	{  1,  2, 0x02,  1,   1 },
	{  0,  0, 0x03,  1,   1 },
	{  0,  1, 0x03,  1,   1 },
	{  1,  2, 0x03,  1,   1 },
	{  0,  0, 0x04,  1,   1 },
	{  0,  1, 0x04,  1,   1 },
	{  1,  2, 0x04,  1,   1 },
	{  0,  0, 0x05,  1,   1 },
	{  0,  1, 0x05,  1,   1 },
	{  1,  2, 0x05,  1,   1 },
	{  0,  0, 0x06,  1,   1 },
	{  0,  1, 0x06,  1,   1 },
	{  1,  2, 0x06,  1,   1 },
	{  0,  0, 0x07,  1,   1 },
	{  0,  1, 0x07,  1,   1 },
	{  1,  2, 0x07,  1,   1 },
	{  0,  0, 0x08,  1,   1 },
	{  0,  1, 0x08,  1,   1 },
	{  1,  2, 0x08,  1,   1 },
	{  0,  0, 0x09,  1,   1 },
	{  0,  1, 0x09,  1,   1 },
	{  1,  2, 0x09,  1,   1 },
	{  0,  0, 0x0a,  1,   1 },
	{  0,  1, 0x0a,  1,   1 },
	{  1,  2, 0x0a,  1,   1 },
	{  0,  0, 0x09,  1,   1 },
	{  0,  1, 0x09,  1,   1 },
	{  1,  2, 0x09,  1,   1 },
	{  0,  0, 0x08,  1,   1 },
	{  0,  1, 0x08,  1,   1 },
	{  1,  2, 0x08,  1,   1 },
	{  0,  0, 0x07,  1,   1 },
	{  0,  1, 0x07,  1,   1 },
	{  1,  2, 0x07,  1,   1 },
	{  0,  0, 0x06,  1,   1 },
	{  0,  1, 0x06,  1,   1 },
	{  1,  2, 0x06,  1,   1 },
	{  0,  0, 0x05,  1,   1 },
	{  0,  1, 0x05,  1,   1 },
	{  1,  2, 0x05,  1,   1 },
	{  0,  0, 0x04,  1,   1 },
	{  0,  1, 0x04,  1,   1 },
	{  1,  2, 0x04,  1,   1 },
	{  0,  0, 0x03,  1,   1 },
	{  0,  1, 0x03,  1,   1 },
	{  1,  2, 0x03,  1, -47 },
} ;
static COLOR_STEP col_16[] = {
	{  1,  0, 0x0a,  1,   1 },
	{  1,  0, 0x09,  1,   1 },
	{  1,  0, 0x08,  1,   1 },
	{  1,  0, 0x07,  1,   1 },
	{  1,  0, 0x06,  1,   0 },
} ;
static COLOR_STEP col_17[] = {
	{  4,  0, 0x05,  1,   1 },
	{  4,  0, 0x06,  1,   1 },
	{  4,  0, 0x07,  1,   1 },
	{  4,  0, 0x08,  1,   1 },
	{  4,  0, 0x09,  1,   1 },
	{  4,  0, 0x0a,  1,   0 },
} ;
static COLOR_STEP col_18[] = {
	{  4,  0, 0x0a,  1,   1 },
	{  4,  0, 0x09,  1,   1 },
	{  4,  0, 0x08,  1,   1 },
	{  4,  0, 0x07,  1,   1 },
	{  4,  0, 0x06,  1,   1 },
	{  4,  0, 0x05,  1,   0 },
} ;
/* the computer's steps in sp_drill's main part, each with how long to keep it */
static Sint16 cpu_tbl_0[] = { 1, 0x20, 2, 0x20, 3, 1, 4, 1, 5, 1, 0, 0x7fff } ;
static Sint16 cpu_tbl_1[] = { 7, 1, 5, 1, 2, 0x7fff } ;
static Sint16 cpu_tbl_2[] = { 4, 1, 4, 1, 5, 1, 1, 0x7fff } ;
static Sint16 cpu_tbl_3[] = { 2, 0x20, 5, 1, 7, 1, 1, 0x7fff } ;

/* the computer's steps in sp_beat's main part: the buttons, the direction, how long to keep it */
static Sint16 cpu_beat_tbl_0[] = {
	0xff, 0x18, 0x10,  0xff, 0x1c, 0x10,  0xff, 0x00, 0x10,  0xff, 0x04, 0x10,
	0xff, 0x08, 0x10,  0xff, 0x0c, 0x10,  0xff, 0x10, 0x10,  0xff, 0x14, 0x10,
	0xff, 0x18, 0x7fff,
} ;
static Sint16 cpu_beat_tbl_1[] = {
	0xff, 0x1c, 0x30,  0xff, 0x10, 0x10,  0xff, 0x04, 0x10,  0xff, 0x10, 0x10,
	0xff, 0x04, 0x10,  0xff, 0x08, 0x7fff,
} ;
static Sint16 cpu_beat_tbl_2[] = {
	0xff, 0x00, 0x30,  0xff, 0x10, 0x10,  0xff, 0x18, 0x10,  0xff, 0x10, 0x10,
	0xff, 0x00, 0x10,  0xff, 0x18, 0x7fff,
} ;
static Sint16 cpu_beat_tbl_3[] = {
	0xff, 0x14, 0x10,  0xff, 0x18, 0x10,  0xff, 0x00, 0x30,  0xff, 0x0c, 0x10,
	0xff, 0x08, 0x10,  0xff, 0x04, 0x7fff,
} ;

static void ( *pl1d_tbl[28])( PLAYER_WORK *pl_w ) = {
	pl1d_main,
	pl1d_com_ck,
	pl1d_atck_set,
	pl1d_atck,
	pl1d_dash,
	pl1d_noki,
	pl1d_appear,
	pl1d_win,
	pl1d_sp_atck,
	( void (*)( PLAYER_WORK *pl_w ) )pl1d_catch_ck,	/* returns a value */
	pl1d_catch_set,
	pl1d_catch,
	pl1d_hold,
	( void ( * )( PLAYER_WORK *pl_w ) )pl1d_delay_ck,
	pl1d_t_kick,
	pl1d_upper,
	pl1d_sp_unison,
	pl1d_sp_gattai,
	pl1d_sp_assist,
	pl1d_sp_zero,
	0,
	0,
	0,
	pl1d_tlocal_init,
	( void ( * )( PLAYER_WORK *pl_w ) )pl1d_shot_end,
	( void ( * )( PLAYER_WORK *pl_w ) )pl1d_item_end,
} ;

static Uint8 hyper_roll_ck( PLAYER_WORK *pl_w );
static Uint8 drill_ck( PLAYER_WORK *pl_w );
static Uint8 beat_ck( PLAYER_WORK *pl_w );
static Uint8 buster_ck( PLAYER_WORK *pl_w );
static Uint8 flower_ck( PLAYER_WORK *pl_w );
static Uint8 item_ck( PLAYER_WORK *pl_w );
static Uint8 item_sel_ball_ck( PLAYER_WORK *pl_w );
static Uint8 item_sel_tornado_ck( PLAYER_WORK *pl_w );
static Uint8 item_sel_shield_ck( PLAYER_WORK *pl_w );
static int snapback_ck( PLAYER_WORK *pl_w );
static Uint8 chouhatsu_ck( PLAYER_WORK *pl_w );
static int hyper_roll_delay_ck( PLAYER_WORK *pl_w );
static int drill_delay_ck( PLAYER_WORK *pl_w );
static int beat_delay_ck( PLAYER_WORK *pl_w );

/****************************************************************/
void pl1d_tbl_set( PLAYER_WORK *pl_w )
{
	int i ;
	void ( **tbl )( PLAYER_WORK *pl_w ) = pl_w->pl_tbl ;

	for( i = 0 ; i < 28 ; i++ ) tbl[i] = pl1d_tbl[i] ;
}

/****************************************************************/
/*			Commands											*/
/****************************************************************/
void pl1d_com_ck( PLAYER_WORK *pl_w )
{
	if( pl_v_combi_ck( pl_w ) ) return ;
	if( pl_assist_ck( pl_w ) ) return ;
	if( pl_change_ck( pl_w ) ) return ;
	if( pl_vcounter_ck( pl_w ) ) return ;
	if( hyper_roll_ck( pl_w ) ) return ;
	if( drill_ck( pl_w ) ) return ;
	if( beat_ck( pl_w ) ) return ;
	if( buster_ck( pl_w ) ) return ;
	if( flower_ck( pl_w ) ) return ;
	if( item_ck( pl_w ) ) return ;
	if( item_sel_ball_ck( pl_w ) ) return ;
	if( item_sel_tornado_ck( pl_w ) ) return ;
	if( item_sel_shield_ck( pl_w ) ) return ;
	if( snapback_ck( pl_w ) ) return ;
	if( chouhatsu_ck( pl_w ) ) return ;
	pl_dash_ck( pl_w ) ;
	pl_sjump_ck( pl_w ) ;
}

/*==============================================================*/
/*			Once a jump in the air								*/
/*==============================================================*/
static Uint8 buster_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_buster, &pl_w->com_w ) ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR && pl_w->sjump_flag == 0 ){
		if( pl_w->jump_com_cnt ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	com_sw_set( pl_w, &pl_w->com_w ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl1d_sp_buster ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
/*			Not while shot_cnt is set on the ground, nor		*/
/*			past 2 in the air									*/
/*==============================================================*/
static Uint8 flower_ck( PLAYER_WORK *pl_w )
{
	PL1D_LOCAL_W *lw ;

	if( !com_nml_ck( pl_w, com_flower, &pl_w->com_w0 ) ) return 0 ;
	lw = ( PL1D_LOCAL_W * )pl_w->tlocal ;
	if( pl_w->pl_stance != STANCE_AIR ){
		if( lw->shot_cnt ) return 0 ;
	} else {
		if( lw->shot_cnt > 2 ) return 0 ;
		if( pl_w->sjump_flag == 0 ){
			if( pl_w->jump_com_cnt ) return 0 ;
			pl_w->jump_com_cnt ++ ;
		}
	}
	com_sw_set( pl_w, &pl_w->com_w0 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl1d_sp_flower ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 hyper_roll_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_hyper_roll, &pl_w->com_w4 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR ){
		if( pl_w->jump_com_cnt && !pl_w->sjump_flag ) return 0 ;
		pl_w->jump_com_cnt ++ ;
	}
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl1d_sp_hyper_roll ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 drill_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_drill, &pl_w->com_w1 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl1d_sp_drill ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 beat_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_beat, &pl_w->com_w2 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl1d_sp_beat ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
/*			On the ground only while nothing else is out; in	*/
/*			the air once a jump									*/
/*==============================================================*/
static Uint8 item_ck( PLAYER_WORK *pl_w )
{
	PL1D_LOCAL_W *lw = ( PL1D_LOCAL_W * )pl_w->tlocal ;

	if( lw->shield_flag == 0 ){
		if( !com_nml_ck( pl_w, com_item, &pl_w->com_w3 ) ) return 0 ;
		if( pl_w->pl_stance == STANCE_AIR ) return 0 ;
		if( lw->ball_flag ) return 0 ;
		if( lw->tornado_flag ) return 0 ;
		if( lw->shield_st ) return 0 ;
		com_sw_set( pl_w, &pl_w->com_w3 ) ;
		pl_w->routine_1 = PL_NORM ;
		pl_w->routine_2 = pl_w->routine_3 = 0 ;
		pl_w->pl_sp_type = pl1d_sp_item ;
		act_st_set( pl_w, N_SPECIAL ) ;
		return 1 ;
	}
	if( !com_nml_ck( pl_w, com_item, &pl_w->com_w3 ) ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR ){
		if( pl_w->jump_com_cnt & !pl_w->sjump_flag ) return 0 ;
		pl_w->jump_com_cnt ++ ;
		pl_w->routine_1 = PL_NORM ;
		pl_w->routine_2 = pl_w->routine_3 = 0 ;
		pl_w->pl_sp_type = pl1d_sp_item ;
		act_st_set( pl_w, N_SPECIAL ) ;
		return 1 ;
	}
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl1d_sp_item ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/*==============================================================*/
/*			The three items: the command picks one, and for		*/
/*			the computer atck_pk does							*/
/*==============================================================*/
static Uint8 item_sel_ball_ck( PLAYER_WORK *pl_w )
{
	PL1D_LOCAL_W *lw = ( PL1D_LOCAL_W * )pl_w->tlocal ;

	if( lw->item_ef_flag != 0 ) return 0 ;
	if( !com_nml_ck( pl_w, com_item_sel_ball, &pl_w->com_w5 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w5 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl1d_sp_item_sel ;
	act_st_set( pl_w, N_SPECIAL ) ;
	if( !pl_w->cpu_flag ) lw->item_sel = 0 ;
	else lw->item_sel = pl_w->atck_pk ;
	return 1 ;
}

/*==============================================================*/
static Uint8 item_sel_tornado_ck( PLAYER_WORK *pl_w )
{
	PL1D_LOCAL_W *lw = ( PL1D_LOCAL_W * )pl_w->tlocal ;

	if( lw->item_ef_flag != 0 ) return 0 ;
	if( !com_nml_ck( pl_w, com_item_sel_tornado, &pl_w->com_w6 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w6 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl1d_sp_item_sel ;
	act_st_set( pl_w, N_SPECIAL ) ;
	if( pl_w->cpu_flag == 0 ) lw->item_sel = 1 ;
	else lw->item_sel = pl_w->atck_pk ;
	return 1 ;
}

/*==============================================================*/
static Uint8 item_sel_shield_ck( PLAYER_WORK *pl_w )
{
	PL1D_LOCAL_W *lw = ( PL1D_LOCAL_W * )pl_w->tlocal ;

	if( lw->item_ef_flag != 0 ) return 0 ;
	if( !com_nml_ck( pl_w, com_item_sel_shield, &pl_w->com_w7 ) ) return 0 ;
	com_sw_set( pl_w, &pl_w->com_w7 ) ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl1d_sp_item_sel ;
	act_st_set( pl_w, N_SPECIAL ) ;
	if( pl_w->cpu_flag == 0 ) lw->item_sel = 2 ;
	else lw->item_sel = pl_w->atck_pk ;
	return 1 ;
}

/*==============================================================*/
static int snapback_ck( PLAYER_WORK *pl_w )
{
	if( !pl_snapback_ck( pl_w ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->pl_sp_type = pl1d_sp_snapback ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	act_st_set( pl_w, N_SUPER ) ;
	return 1 ;
}

/*==============================================================*/
static Uint8 chouhatsu_ck( PLAYER_WORK *pl_w )
{
	if( !pl_chouhatsu_ck( pl_w, 3 ) ) return 0 ;
	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	pl_w->pl_sp_type = pl1d_sp_chouhatsu ;
	act_st_set( pl_w, N_SPECIAL ) ;
	return 1 ;
}

/****************************************************************/
/*			Hyper combos entered during another move, done		*/
/*			when it ends										*/
/****************************************************************/
int pl1d_delay_ck( PLAYER_WORK *pl_w )
{
	if( hyper_roll_delay_ck( pl_w ) || drill_delay_ck( pl_w ) || beat_delay_ck( pl_w ) ) return 1 ;
	return 0 ;
}

/*==============================================================*/
static int hyper_roll_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_hyper_roll, &pl_w->com_w4 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = 7 ;
	return 1 ;
}

/*==============================================================*/
static int drill_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_drill, &pl_w->com_w1 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = 4 ;
	return 1 ;
}

/*==============================================================*/
static int beat_delay_ck( PLAYER_WORK *pl_w )
{
	if( !com_nml_ck( pl_w, com_beat, &pl_w->com_w2 ) ) return 0 ;
	if( !*pl_w->gauge_level ) return 0 ;
	pl_w->delay_sp_type = 5 ;
	return 1 ;
}

/****************************************************************/
/*			Counts sp_time down, and shakes the body while		*/
/*			shake_flag is set									*/
/****************************************************************/
void pl1d_main( PLAYER_WORK *pl_w )
{
	static Sint16 shake_tbl[4] = { 4, -4, 4, -4 } ;
	PL1D_LOCAL_W *lw = ( PL1D_LOCAL_W * )pl_w->tlocal ;

	if( !pl_w->hit_stop && lw->sp_time ) lw->sp_time -- ;
	if( lw->shake_flag ){
		if( pl_w->hit_stop ) pl_w->pos.x += DOT_X( shake_tbl[Sys->game_timer0 & 3] ) ;
		else lw->shake_flag = 0 ;
	}
}

static void nml_atck( PLAYER_WORK *pl_w );
static void stand_punch( PLAYER_WORK *pl_w );
static void crouch_punch( PLAYER_WORK *pl_w );
static void stand_kick( PLAYER_WORK *pl_w );
static void crouch_kick( PLAYER_WORK *pl_w );
static void jump_atck_ck( PLAYER_WORK *pl_w );
static void jump_atck( PLAYER_WORK *pl_w );
static void jump_punch( PLAYER_WORK *pl_w );
static void jump_kick( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Normal attacks										*/
/****************************************************************/
void pl1d_atck_set( PLAYER_WORK *pl_w )
{
	static void ( *atck_set_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_atck, jump_atck_ck, jump_atck, nml_atck,
	} ;

	atck_set_jmp[pl_w->atck_type]( pl_w ) ;
}

/*==============================================================*/
static void nml_atck( PLAYER_WORK *pl_w )
{
	pl_brake_set( pl_w ) ;
	if( pl_w->atck_pk == 1 ){
		if( pl_w->pl_stance == STANCE_CROUCH ) crouch_kick( pl_w ) ;
		else stand_kick( pl_w ) ;
	} else {
		if( pl_w->pl_stance == STANCE_CROUCH ) crouch_punch( pl_w ) ;
		else stand_punch( pl_w ) ;
	}
}

/*==============================================================*/
static void stand_punch( PLAYER_WORK *pl_w )
{
	switch( pl_w->nml_no ){
	case 0:
		pl_w->char_no.b.no = 0 ;
		pl_w->atck_no = ATCK_STAND_LP ;
		pl_se_req( pl_w, SE_SWING_L ) ;
		pl_w->chain_tbl = lp_tbl ;
		pl_w->nml_level = 0 ;
		break ;
	case 1:
		pl_w->char_no.b.no = 1 ;
		pl_w->atck_no = ATCK_STAND_MP ;
		pl_se_req( pl_w, SE_SWING_M ) ;
		pl_w->chain_tbl = mp_tbl ;
		pl_w->nml_level = 1 ;
		break ;
	case 2:
		pl_w->char_no.b.no = 2 ;
		pl_w->atck_no = ATCK_STAND_HP ;
		pl_se_req( pl_w, SE_SWING_H ) ;
		pl_w->chain_tbl = hp_tbl ;
		pl_w->nml_level = 2 ;
		break ;
	}
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Stand_P, pl_w->char_no.b.no ) ;
}

/*==============================================================*/
static void crouch_punch( PLAYER_WORK *pl_w )
{
	switch( pl_w->nml_no ){
	case 0:
		pl_w->char_no.b.no = 0 ;
		pl_w->atck_no = ATCK_CROUCH_LP ;
		pl_se_req( pl_w, SE_SWING_L ) ;
		pl_w->chain_tbl = lp_tbl ;
		pl_w->nml_level = 0 ;
		break ;
	case 1:
		pl_w->char_no.b.no = 1 ;
		pl_w->atck_no = ATCK_CROUCH_MP ;
		pl_se_req( pl_w, SE_SWING_M ) ;
		pl_w->chain_tbl = mp_tbl ;
		pl_w->nml_level = 1 ;
		break ;
	case 2:
		pl_w->char_no.b.no = 2 ;
		pl_w->atck_no = ATCK_CROUCH_HP ;
		pl_se_req( pl_w, SE_SWING_H ) ;
		pl_w->chain_tbl = hp_tbl ;
		pl_w->nml_level = 2 ;
		break ;
	}
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Crouch_P, pl_w->char_no.b.no ) ;
}

/*==============================================================*/
static void stand_kick( PLAYER_WORK *pl_w )
{
	switch( pl_w->nml_no ){
	case 0:
		pl_w->char_no.b.no = 0 ;
		pl_w->atck_no = ATCK_STAND_LK ;
		pl_se_req( pl_w, SE_SWING_L ) ;
		pl_w->chain_tbl = lk_tbl ;
		pl_w->nml_level = 0 ;
		break ;
	case 1:
		pl_w->char_no.b.no = 1 ;
		pl_w->atck_no = ATCK_STAND_MK ;
		pl_se_req( pl_w, SE_SWING_M ) ;
		pl_w->chain_tbl = mk_tbl ;
		pl_w->nml_level = 1 ;
		break ;
	case 2:
		pl_w->char_no.b.no = 2 ;
		pl_w->atck_no = ATCK_STAND_HK ;
		pl_w->chain_tbl = hk_tbl ;
		pl_w->nml_level = 2 ;
		break ;
	}
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Stand_K, pl_w->char_no.b.no ) ;
}

/*==============================================================*/
static void crouch_kick( PLAYER_WORK *pl_w )
{
	switch( pl_w->nml_no ){
	case 0:
		pl_w->char_no.b.no = 0 ;
		pl_w->atck_no = ATCK_CROUCH_LK ;
		pl_se_req( pl_w, SE_SWING_L ) ;
		pl_w->chain_tbl = lk_tbl ;
		pl_w->nml_level = 0 ;
		break ;
	case 1:
		pl_w->char_no.b.no = 1 ;
		pl_w->atck_no = ATCK_CROUCH_MK ;
		pl_se_req( pl_w, SE_SWING_M ) ;
		pl_w->chain_tbl = mk_tbl ;
		pl_w->nml_level = 1 ;
		break ;
	case 2:
		pl_w->char_no.b.no = 2 ;
		pl_w->atck_no = ATCK_CROUCH_HK ;
		pl_w->chain_tbl = hk_tbl ;
		pl_w->nml_level = 2 ;
		break ;
	}
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Crouch_K, pl_w->char_no.b.no ) ;
}

/*==============================================================*/
/*			In the air, only while that button's jump			*/
/*			attacks are left									*/
/*==============================================================*/
static void jump_atck_ck( PLAYER_WORK *pl_w )
{
	if( ( pl_w->atck_pk == 0 && ( pl_w->jump_atk_cnt & JATK_P_MASK ) )
	 || ( pl_w->atck_pk != 0 && ( pl_w->jump_atk_cnt & JATK_K_MASK ) ) ) jump_atck( pl_w ) ;
}

/*==============================================================*/
static void jump_atck( PLAYER_WORK *pl_w )
{
	if( pl_w->atck_pk == 1 ) jump_kick( pl_w ) ;
	else jump_punch( pl_w ) ;
}

/*==============================================================*/
static void jump_punch( PLAYER_WORK *pl_w )
{
	switch( pl_w->nml_no ){
	case 0:
		pl_w->char_no.b.no = 0 ;
		pl_w->atck_no = ATCK_JUMP_LP ;
		pl_se_req( pl_w, SE_SWING_L ) ;
		if( !pl_w->sjump_flag ) pl_w->chain_tbl = j_lp_tbl ;
		else pl_w->chain_tbl = sj_lp_tbl ;
		pl_w->nml_level = 0 ;
		break ;
	case 1:
		pl_w->char_no.b.no = 1 ;
		pl_w->atck_no = ATCK_JUMP_MP ;
		pl_se_req( pl_w, SE_SWING_M ) ;
		if( !pl_w->sjump_flag ) pl_w->chain_tbl = j_mp_tbl ;
		else pl_w->chain_tbl = sj_mp_tbl ;
		pl_w->nml_level = 1 ;
		break ;
	case 2:
		pl_w->char_no.b.no = 2 ;
		pl_w->atck_no = ATCK_JUMP_HP ;
		pl_se_req( pl_w, SE_SWING_H ) ;
		if( !pl_w->sjump_flag ) pl_w->chain_tbl = j_hp_tbl ;
		else pl_w->chain_tbl = sj_hp_tbl ;
		pl_w->nml_level = 2 ;
		break ;
	}
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Jump_P, pl_w->char_no.b.no ) ;
	if( pl_w->jump_atk_cnt & JATK_P_MASK ) pl_w->jump_atk_cnt -= 0x01 ;
}

/*==============================================================*/
static void jump_kick( PLAYER_WORK *pl_w )
{
	switch( pl_w->nml_no ){
	case 0:
		pl_w->char_no.b.no = 0 ;
		pl_w->atck_no = ATCK_JUMP_LK ;
		pl_se_req( pl_w, SE_SWING_L ) ;
		if( !pl_w->sjump_flag ) pl_w->chain_tbl = j_lk_tbl ;
		else pl_w->chain_tbl = sj_lk_tbl ;
		pl_w->nml_level = 0 ;
		break ;
	case 1:
		pl_w->char_no.b.no = 1 ;
		pl_w->atck_no = ATCK_JUMP_MK ;
		pl_se_req( pl_w, SE_SWING_M ) ;
		if( !pl_w->sjump_flag ) pl_w->chain_tbl = j_mk_tbl ;
		else pl_w->chain_tbl = sj_mk_tbl ;
		pl_w->nml_level = 1 ;
		break ;
	case 2:
		pl_w->char_no.b.no = 2 ;
		pl_w->atck_no = ATCK_JUMP_HK ;
		if( pl_w->atck_lever & KEY_UP ){
			pl_w->char_no.b.no = 6 ;
			pl_w->atck_no = pl1d_at_jup_hk ;
		}
		pl_se_req( pl_w, SE_SWING_H ) ;
		if( !pl_w->sjump_flag ) pl_w->chain_tbl = j_hk_tbl ;
		else pl_w->chain_tbl = sj_hk_tbl ;
		pl_w->nml_level = 2 ;
		break ;
	}
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Jump_K, pl_w->char_no.b.no ) ;
	if( pl_w->jump_atk_cnt & JATK_K_MASK ) pl_w->jump_atk_cnt -= 0x10 ;
}

static void nml_move_dash( PLAYER_WORK *pl_w );
static void nml_move( PLAYER_WORK *pl_w );
static void stand_punch_move( PLAYER_WORK *pl_w );
static void crouch_punch_move( PLAYER_WORK *pl_w );
static void stand_kick_move( PLAYER_WORK *pl_w );
static void crouch_kick_move( PLAYER_WORK *pl_w );
static void sjump_move( PLAYER_WORK *pl_w );
static void jump_move( PLAYER_WORK *pl_w );
static void jump_punch_move( PLAYER_WORK *pl_w );
static void jump_kick_move( PLAYER_WORK *pl_w );

/****************************************************************/
void pl1d_atck( PLAYER_WORK *pl_w )
{
	static void ( *atck_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_move, jump_move, sjump_move, nml_move_dash,
	} ;

	atck_jmp[pl_w->atck_type]( pl_w ) ;
}

/*==============================================================*/
static void nml_move_dash( PLAYER_WORK *pl_w )
{
	pl_smoke_set( pl_w ) ;
	nml_move( pl_w ) ;
}

/*==============================================================*/
static void nml_move( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_brake_stop( pl_w ) ;
	if( pl_w->atck_pk == 1 ){
		if( pl_w->pl_stance == STANCE_CROUCH ) crouch_kick_move( pl_w ) ;
		else stand_kick_move( pl_w ) ;
	} else {
		if( pl_w->pl_stance == STANCE_CROUCH ) crouch_punch_move( pl_w ) ;
		else stand_punch_move( pl_w ) ;
	}
}

/*==============================================================*/
/*			The heavy punch dashes forward once, and stops		*/
/*			when it lets go										*/
/*==============================================================*/
static void stand_punch_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	switch( pl_w->nml_no ){
	case 2:
		if( pl_w->routine_2 == 0 ){
			if( pl_w->cg_type ){
				pl_w->routine_2 ++ ;
				if( !pl_w->disp_dir ){
					pl_w->spd.x = -1.6666666f ;
					pl_w->acc.x = 0.0f ;
				} else {
					pl_w->spd.x = 1.6666666f ;
					pl_w->acc.x = -0.0f ;
				}
			}
		} else if( !pl_w->cg_type ) pl_w->spd.x = pl_w->acc.x = 0.0f ;
		break ;
	case 0:
		break ;
	case 1:
		break ;
	}
}

/*==============================================================*/
static void crouch_punch_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	switch( pl_w->nml_no ){
	case 2:
		break ;
	case 0:
		break ;
	case 1:
		break ;
	}
}

/*==============================================================*/
static void stand_kick_move( PLAYER_WORK *pl_w )
{
	switch( pl_w->nml_no ){
	case 0:
	case 1:
	case 2:
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
		break ;
	}
}

/*==============================================================*/
/*			The heavy kick slides								*/
/*==============================================================*/
static void crouch_kick_move( PLAYER_WORK *pl_w )
{
	switch( pl_w->nml_no ){
	case 2:
		if( pl_char_move( pl_w ) < 0 ){
			pl_stand_ret( pl_w ) ;
			return ;
		}
		if( pl_w->routine_2 == 0 ){
			if( pl_w->cg_type ){
				pl_w->cg_type = 0 ;
				pl_w->routine_2 ++ ;
				if( !pl_w->disp_dir ){
					pl_w->spd.x = -DOT_X( 6.5f ) ;
					pl_w->acc.x = DOT_X( 0.25f ) ;
				} else {
					pl_w->spd.x = DOT_X( 6.5f ) ;
					pl_w->acc.x = -DOT_X( 0.25f ) ;
				}
			}
		} else pl_smoke_set2( pl_w, 7 ) ;
		break ;
	case 0:
	case 1:
		if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
		break ;
	}
}

/*==============================================================*/
static void sjump_move( PLAYER_WORK *pl_w )
{
	pl_jump_hit_ck( pl_w ) ;
	pl_sjump_ctrl( pl_w ) ;
	jump_move( pl_w ) ;
}

/*==============================================================*/
static void jump_move( PLAYER_WORK *pl_w )
{
	pl_jump_move( pl_w ) ;
	pl_sjump_top_ck( pl_w ) ;
	if( pl_w->atck_pk == 1 ) jump_kick_move( pl_w ) ;
	else jump_punch_move( pl_w ) ;
	if( pl_land_ck( pl_w ) ) pl_land_set( pl_w ) ;
}

/*==============================================================*/
static void jump_punch_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_jump_ret( pl_w ) ;
}

/*==============================================================*/
static void jump_kick_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_jump_ret( pl_w ) ;
}

/****************************************************************/
void pl1d_upper( PLAYER_WORK *pl_w )
{
	if( !pl_w->routine_2 ){
		pl_brake_set( pl_w ) ;
		pl_w->routine_2 ++ ;
		pl_w->atck_no = pl1d_at_upper ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_char_set( pl_w, Char_Extra, 3 ) ;
		HIT_RESET( pl_w ) ;
		pl_gauge_add( pl_w, 5 ) ;
	}
	if( pl_w->atck_type == 3 ) pl_smoke_set( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	pl_brake_stop( pl_w ) ;
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void dash_init( PLAYER_WORK *pl_w );
static void dash_jump( PLAYER_WORK *pl_w );
static void dash_land( PLAYER_WORK *pl_w );

/****************************************************************/
/*			The dash is a low hop								*/
/****************************************************************/
void pl1d_dash( PLAYER_WORK *pl_w )
{
	static void ( *dash_jmp[])( PLAYER_WORK *pl_w ) = {
		dash_init, dash_jump, dash_land,
	} ;

	dash_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void dash_init( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		if( !pl_w->pl_dir ){
			pl_w->spd.x = -10.0f ;
			pl_w->acc.x = 0.15625f ;
		} else {
			pl_w->spd.x = 10.0f ;
			pl_w->acc.x = -0.15625f ;
		}
		pl_w->spd.y = DOT_Y( 2.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.125f ) ;
	}
}

/*--------------------------------------------------------------*/
static void dash_jump( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->ground_y >= pl_w->pos.y ){
		pl_w->routine_2 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_char_set( pl_w, Char_Dash, 2 ) ;
	}
}

/*--------------------------------------------------------------*/
static void dash_land( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pl_w->spd.x = 0.0f ;
		pl_w->acc.x = 0.0f ;
	}
}

static void noki_init( PLAYER_WORK *pl_w );
static void noki_jump( PLAYER_WORK *pl_w );
static void noki_land( PLAYER_WORK *pl_w );

/****************************************************************/
/*			The back dash, a hop as well						*/
/****************************************************************/
void pl1d_noki( PLAYER_WORK *pl_w )
{
	static void ( *noki_jmp[])( PLAYER_WORK *pl_w ) = {
		noki_init, noki_jump, noki_land,
	} ;

	noki_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void noki_init( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		if( !pl_w->pl_dir ){
			pl_w->spd.x = 10.0f ;
			pl_w->acc.x = -DOT_X( 0.125f ) ;
		} else {
			pl_w->spd.x = -10.0f ;
			pl_w->acc.x = DOT_X( 0.125f ) ;
		}
		pl_w->spd.y = DOT_Y( 2.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.125f ) ;
	}
}

/*--------------------------------------------------------------*/
static void noki_jump( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->ground_y >= pl_w->pos.y ){
		pl_w->routine_2 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_w->pl_stance = STANCE_STAND ;
		pl_char_set( pl_w, Char_Dash, 3 ) ;
	}
}

/*--------------------------------------------------------------*/
static void noki_land( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		pl_w->spd.x = 0.0f ;
		pl_w->acc.x = 0.0f ;
	}
}

static void color_start( PLAYER_WORK *pl_w, COLOR_STEP *step );
static int color_move( PLAYER_WORK *pl_w );
static void color_set( PLAYER_WORK *pl_w, int no );

static void appear_init( PLAYER_WORK *pl_w );
static void appear_move( PLAYER_WORK *pl_w );
static void appear_ride( PLAYER_WORK *pl_w );
static void appear_ride_init( PLAYER_WORK *pl_w );
static void appear_ride_wait( PLAYER_WORK *pl_w );
static void appear_ride_fall( PLAYER_WORK *pl_w );
static void appear_ride_end( PLAYER_WORK *pl_w );
static void appear_drop( PLAYER_WORK *pl_w );
static void appear_drop_init( PLAYER_WORK *pl_w );
static void appear_drop_wait( PLAYER_WORK *pl_w );
static void appear_drop_move( PLAYER_WORK *pl_w );
static void appear_drop_fall( PLAYER_WORK *pl_w );
static void appear_drop_end( PLAYER_WORK *pl_w );
static void appear_stand( PLAYER_WORK *pl_w );
static void appear_stand_init( PLAYER_WORK *pl_w );
static void appear_stand_wait( PLAYER_WORK *pl_w );
static void appear_stand_move( PLAYER_WORK *pl_w );
static void appear_stand_end( PLAYER_WORK *pl_w );

/****************************************************************/
/*			The appearance: one of three, picked at random		*/
/****************************************************************/
void pl1d_appear( PLAYER_WORK *pl_w )
{
	static void ( *appear_jmp[])( PLAYER_WORK *pl_w ) = {
		appear_init, appear_move,
	} ;

	appear_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void appear_init( PLAYER_WORK *pl_w )
{
	static Uint8 type_tbl[4][2] = { { 0, 0 }, { 0, 0 }, { 1, 0 }, { 2, 0 } } ;

	pl_w->routine_2 ++ ;
	pl_w->type = type_tbl[Rnd() & 3][0] ;
	appear_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void appear_move( PLAYER_WORK *pl_w )
{
	static void ( *appear_move_jmp[])( PLAYER_WORK *pl_w ) = {
		appear_ride, appear_drop, appear_stand,
	} ;

	appear_move_jmp[pl_w->type]( pl_w ) ;
}

/*==============================================================*/
/*			Comes in from the far side, turns and jumps down	*/
/*==============================================================*/
static void appear_ride( PLAYER_WORK *pl_w )
{
	static void ( *appear_ride_jmp[])( PLAYER_WORK *pl_w ) = {
		appear_ride_init, appear_ride_wait, appear_ride_fall, appear_ride_end,
	} ;

	appear_ride_jmp[pl_w->routine_3]( pl_w ) ;
}

/*==============================================================*/
static void appear_ride_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_3 ++ ;
	pl_w->timer0 = 93 ;
	pl_w->spd.z = pl_w->pos.x ;
	if( pl_w->pl_no == 0 ){
		pl_w->pl_dir = 0 ;
		pl_w->pos.x += DOT_X( 370.0f ) ;
	} else {
		pl_w->pl_dir = 1 ;
		pl_w->pos.x -= DOT_X( 370.0f ) ;
	}
	pl_w->disp_dir = pl_w->pl_dir ;
	pl_w->pos.y = pl_w->ground_y ;
	pls1c_07_set( pl_w, 11 ) ;
	pl_char_set( pl_w, Char_Appear, 0 ) ;
}

/*--------------------------------------------------------------*/
static void appear_ride_wait( PLAYER_WORK *pl_w )
{
	if( --pl_w->timer0 == 0 ){
		pl_w->routine_3 ++ ;
		pl_w->pl_dir ^= 1 ;
		pl_w->disp_dir = pl_w->pl_dir ;
		pl_w->spd.x = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
		pl_char_set( pl_w, Char_Appear, 2 ) ;
		return ;
	}
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void appear_ride_fall( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y <= pl_w->ground_y ){
		pl_w->routine_3 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_char_set( pl_w, Char_Appear, 3 ) ;
		return ;
	}
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void appear_ride_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_1 ++ ;
		pl_w->routine_2 = 0 ;
		pl_w->pos.x = pl_w->spd.z ;
	}
}

/*==============================================================*/
/*			Drops in from above the near side, then hops		*/
/*			over												*/
/*==============================================================*/
static void appear_drop( PLAYER_WORK *pl_w )
{
	static void ( *appear_drop_jmp[])( PLAYER_WORK *pl_w ) = {
		appear_drop_init, appear_drop_wait, appear_drop_move, appear_drop_fall, appear_drop_end,
	} ;

	appear_drop_jmp[pl_w->routine_3]( pl_w ) ;
}

/*==============================================================*/
static void appear_drop_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_3 ++ ;
	pl_w->timer0 = 38 ;
	pl_w->spd.z = pl_w->pos.x ;
	if( pl_w->pl_no == 0 ){
		pl_w->pl_dir = 1 ;
		pl_w->pos.x += -160.0f ;
	} else {
		pl_w->pl_dir = 0 ;
		pl_w->pos.x -= -160.0f ;
	}
	pl_w->disp_dir = pl_w->pl_dir ;
	pl_w->pos.y = pl_w->ground_y + DOT_Y( 128.0f ) ;
	pls1c_07_set( pl_w, 12 ) ;
	pl_char_set( pl_w, Char_Appear, 4 ) ;
}

/*--------------------------------------------------------------*/
static void appear_drop_wait( PLAYER_WORK *pl_w )
{
	if( --pl_w->timer0 == 0 ){
		pl_w->routine_3 ++ ;
		pl_w->timer0 = 16 ;
		if( !( pl_w->pl_dir & 1 ) ){
			pl_w->pos.x += -20.0f ;
			pl_w->spd.x = DOT_X( 2.0f ) ;
		} else {
			pl_w->pos.x -= -20.0f ;
			pl_w->spd.x = -DOT_X( 2.0f ) ;
		}
		pl_w->pos.y -= DOT_Y( 50.0f ) ;
		pl_w->acc.x = 0.0f ;
		pl_w->spd.y = DOT_Y( 4.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
		pl_w->pl_dir ^= 1 ;
		pl_char_set( pl_w, Char_Appear, 5 ) ;
		return ;
	}
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void appear_drop_move( PLAYER_WORK *pl_w )
{
	if( --pl_w->timer0 == 0 ){
		pl_w->routine_3 ++ ;
		pl_w->pl_dir ^= 1 ;
		pl_char_set( pl_w, Char_Appear, 6 ) ;
	} else {
		pl_w->pos.x += pl_w->spd.x ;
		pl_w->spd.x += pl_w->acc.x ;
		pl_w->pos.y += pl_w->spd.y ;
		pl_w->spd.y += pl_w->acc.y ;
		pl_char_move( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void appear_drop_fall( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y <= pl_w->ground_y ){
		pl_w->routine_3 ++ ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_land_ef_set( pl_w ) ;
		pl_char_set( pl_w, Char_Appear, 7 ) ;
		return ;
	}
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void appear_drop_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_1 ++ ;
		pl_w->routine_2 = 0 ;
		pl_w->pos.x = pl_w->spd.z ;
	}
}

/*==============================================================*/
/*			Stands in place										*/
/*==============================================================*/
static void appear_stand( PLAYER_WORK *pl_w )
{
	static void ( *appear_stand_jmp[])( PLAYER_WORK *pl_w ) = {
		appear_stand_init, appear_stand_wait, appear_stand_move, appear_stand_end,
	} ;

	appear_stand_jmp[pl_w->routine_3]( pl_w ) ;
}

/*==============================================================*/
static void appear_stand_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_3 ++ ;
	pl_w->timer0 = 64 ;
	pls1c_07_set( pl_w, 13 ) ;
	pl_char_set( pl_w, Char_Stand, 0 ) ;
}

/*--------------------------------------------------------------*/
static void appear_stand_wait( PLAYER_WORK *pl_w )
{
	if( --pl_w->timer0 == 0 ){
		pl_w->routine_3 ++ ;
		pl_char_set( pl_w, Char_Stand_K, 2 ) ;
		return ;
	}
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void appear_stand_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_3 ++ ;
		pl_char_set( pl_w, Char_Extra, 2 ) ;
	}
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void appear_stand_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_1 ++ ;
		pl_w->routine_2 = 0 ;
	}
}

static int partner_ck( PLAYER_WORK *pl_w, Uint8 no );

static void win_init( PLAYER_WORK *pl_w );
static void win_move( PLAYER_WORK *pl_w );
static void win_held( PLAYER_WORK *pl_w );
static void win_turn( PLAYER_WORK *pl_w );
static void win_pose( PLAYER_WORK *pl_w );
static void win_shine( PLAYER_WORK *pl_w );
static void win_roll( PLAYER_WORK *pl_w );
static void win_roll_2( PLAYER_WORK *pl_w );
static void win_time_ko( PLAYER_WORK *pl_w );
static void lose_time_ko( PLAYER_WORK *pl_w );
static void win_type_set( PLAYER_WORK *pl_w );

/****************************************************************/
/*			The win and lose poses								*/
/****************************************************************/
void pl1d_win( PLAYER_WORK *pl_w )
{
	static void ( *win_jmp[])( PLAYER_WORK *pl_w ) = {
		win_init, win_move,
	} ;

	pl_w->com_w.step = 0 ;
	win_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void win_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	if( !pl_w->type ) win_type_set( pl_w ) ;
	win_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void win_move( PLAYER_WORK *pl_w )
{
	static void ( *win_type_jmp[])( PLAYER_WORK *pl_w ) = {
		win_time_ko, lose_time_ko, win_time_ko, lose_time_ko, lose_time_ko,
		win_held, win_turn, win_pose, win_shine, win_roll, win_roll_2,
	} ;

	if( pl_win_end_ck( pl_w ) ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	win_type_jmp[pl_w->type]( pl_w ) ;
}

static void win_held_init( PLAYER_WORK *pl_w );
static void win_held_move( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			The plain pose, also taken when the partner			*/
/*			can't join in										*/
/*==============================================================*/
static void win_held( PLAYER_WORK *pl_w )
{
	static void ( *win_held_jmp[])( PLAYER_WORK *pl_w ) = {
		win_held_init, win_held_move,
	} ;

	win_held_jmp[pl_w->routine_3]( pl_w ) ;
}

/*==============================================================*/
static void win_held_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_3 ++ ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_char_set( pl_w, Char_Win, 1 ) ;
	win_held_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void win_held_move( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
}

static void win_turn_init( PLAYER_WORK *pl_w );
static void win_turn_wait( PLAYER_WORK *pl_w );
static void win_turn_pose( PLAYER_WORK *pl_w );
static void win_turn_end( PLAYER_WORK *pl_w );

/*==============================================================*/
static void win_turn( PLAYER_WORK *pl_w )
{
	static void ( *win_turn_jmp[])( PLAYER_WORK *pl_w ) = {
		win_turn_init, win_turn_wait, win_turn_pose, win_turn_end,
	} ;

	win_turn_jmp[pl_w->routine_3]( pl_w ) ;
}

/*==============================================================*/
static void win_turn_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_3 ++ ;
	pl_w->timer0 = 1 ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pls1c_07_set( pl_w, 6 ) ;
	pl_char_set( pl_w, Char_Stand, 0 ) ;
	win_turn_wait( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void win_turn_wait( PLAYER_WORK *pl_w )
{
	if( !pl_w->timer0 ){
		pl_w->routine_3 ++ ;
		pl_char_set( pl_w, Char_Win, 0 ) ;
	}
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void win_turn_pose( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ) pl_w->routine_3 ++ ;
}

/*--------------------------------------------------------------*/
static void win_turn_end( PLAYER_WORK *pl_w )
{
	pl_w->win_wait = 0xff ;
	pl_char_move( pl_w ) ;
}

static void win_pose_init( PLAYER_WORK *pl_w );
static void win_pose_wait( PLAYER_WORK *pl_w );
static void win_pose_color( PLAYER_WORK *pl_w );
static void win_pose_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			Poses, then flashes									*/
/*==============================================================*/
static void win_pose( PLAYER_WORK *pl_w )
{
	static void ( *win_pose_jmp[])( PLAYER_WORK *pl_w ) = {
		win_pose_init, win_pose_wait, win_pose_color, win_pose_end,
	} ;

	win_pose_jmp[pl_w->routine_3]( pl_w ) ;
}

/*==============================================================*/
static void win_pose_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_3 ++ ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_char_set( pl_w, Char_Win, 7 ) ;
	win_pose_wait( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void win_pose_wait( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_3 ++ ;
		color_start( pl_w, col_17 ) ;
		color_move( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void win_pose_color( PLAYER_WORK *pl_w )
{
	if( !color_move( pl_w ) ){
		pl_w->routine_3 ++ ;
		color_start( pl_w, col_18 ) ;
		color_move( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void win_pose_end( PLAYER_WORK *pl_w )
{
	color_move( pl_w ) ;
	pl_char_move( pl_w ) ;
}

static void win_shine_init( PLAYER_WORK *pl_w );
static void win_shine_wait( PLAYER_WORK *pl_w );
static void win_shine_move( PLAYER_WORK *pl_w );
static void win_shine_pose_wait( PLAYER_WORK *pl_w );
static void win_shine_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			With the partner, if they are there					*/
/*==============================================================*/
static void win_shine( PLAYER_WORK *pl_w )
{
	static void ( *win_shine_jmp[])( PLAYER_WORK *pl_w ) = {
		win_shine_init, win_shine_wait, win_shine_move, win_shine_pose_wait, win_shine_end,
	} ;

	pl_w->win_wait = 0xff ;
	win_shine_jmp[pl_w->routine_3]( pl_w ) ;
}

/*==============================================================*/
static void win_shine_init( PLAYER_WORK *pl_w )
{
	if( partner_ck( pl_w, 28 ) ){
		pl_w->type = 5 ;
		win_held( pl_w ) ;
		return ;
	}
	pl_w->routine_3 ++ ;
	pl_w->timer0 = 1 ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	ef_color_set( pl_w, 7, 57 + pl_w->pl_color * 87, 1 ) ;
	pls1c_07_set( pl_w, 8 ) ;
	pl_char_set( pl_w, Char_Stand, 0 ) ;
	win_shine_wait( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void win_shine_wait( PLAYER_WORK *pl_w )
{
	if( !pl_w->timer0 ){
		pl_w->routine_3 ++ ;
		pl_char_set( pl_w, Char_Win, 6 ) ;
	}
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void win_shine_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_3 ++ ;
		pl_w->timer0 = 1 ;
		pl_char_set( pl_w, Char_Stand, 0 ) ;
	}
}

/*--------------------------------------------------------------*/
static void win_shine_pose_wait( PLAYER_WORK *pl_w )
{
	if( !pl_w->timer0 ){
		pl_w->routine_3 ++ ;
		pl_char_set( pl_w, Char_Win, 1 ) ;
	}
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void win_shine_end( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
}

static void win_roll_init( PLAYER_WORK *pl_w );
static void win_roll_wait( PLAYER_WORK *pl_w );
static void win_roll_move( PLAYER_WORK *pl_w );

/*==============================================================*/
static void win_roll( PLAYER_WORK *pl_w )
{
	static void ( *win_roll_jmp[])( PLAYER_WORK *pl_w ) = {
		win_roll_init, win_roll_wait, win_roll_move,
	} ;

	pl_w->win_wait = 0xff ;
	win_roll_jmp[pl_w->routine_3]( pl_w ) ;
}

/*==============================================================*/
static void win_roll_init( PLAYER_WORK *pl_w )
{
	if( partner_ck( pl_w, 28 ) ){
		pl_w->type = 5 ;
		win_held( pl_w ) ;
		return ;
	}
	pl_w->routine_3 ++ ;
	pl_w->timer0 = 1 ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	ef_color_set( pl_w, 1, 85 + pl_w->pl_color * 87, 1 ) ;
	ef_color_set( pl_w, 2, 32 + pl_w->pl_color * 87, 1 ) ;
	pls1c_07_set( pl_w, 9 ) ;
	pl_char_set( pl_w, Char_Stand, 0 ) ;
	win_roll_wait( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void win_roll_wait( PLAYER_WORK *pl_w )
{
	if( !pl_w->timer0 ){
		pl_w->routine_3 ++ ;
		pl_char_set( pl_w, Char_Win, 8 ) ;
	}
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void win_roll_move( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
}

static void win_roll_2_init( PLAYER_WORK *pl_w );
static void win_roll_2_wait( PLAYER_WORK *pl_w );
static void win_roll_2_move( PLAYER_WORK *pl_w );

/*==============================================================*/
static void win_roll_2( PLAYER_WORK *pl_w )
{
	static void ( *win_roll_2_jmp[])( PLAYER_WORK *pl_w ) = {
		win_roll_2_init, win_roll_2_wait, win_roll_2_move,
	} ;

	pl_w->win_wait = 0xff ;
	win_roll_2_jmp[pl_w->routine_3]( pl_w ) ;
}

/*==============================================================*/
static void win_roll_2_init( PLAYER_WORK *pl_w )
{
	if( partner_ck( pl_w, 28 ) ){
		pl_w->type = 5 ;
		win_held( pl_w ) ;
		return ;
	}
	pl_w->routine_3 ++ ;
	pl_w->timer0 = 1 ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	ef_color_set( pl_w, 2, 85 + pl_w->pl_color * 87, 1 ) ;
	pls1c_07_set( pl_w, 10 ) ;
	pl_char_set( pl_w, Char_Stand, 0 ) ;
	win_roll_2_wait( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void win_roll_2_wait( PLAYER_WORK *pl_w )
{
	if( !pl_w->timer0 ){
		pl_w->routine_3 ++ ;
		pl_char_set( pl_w, Char_Dizzy, 0 ) ;
	}
	pl_char_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void win_roll_2_move( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
}

static void win_ko_init( PLAYER_WORK *pl_w );
static void win_ko_move( PLAYER_WORK *pl_w );

/*==============================================================*/
static void win_time_ko( PLAYER_WORK *pl_w )
{
	static void ( *win_ko_jmp[])( PLAYER_WORK *pl_w ) = {
		win_ko_init, win_ko_move,
	} ;

	win_ko_jmp[pl_w->routine_3]( pl_w ) ;
}

/*==============================================================*/
static void win_ko_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_3 ++ ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->sjump_flag = 0 ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_char_set( pl_w, Char_Win, 4 ) ;
	win_ko_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void win_ko_move( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
}

static void lose_init( PLAYER_WORK *pl_w );
static void lose_move( PLAYER_WORK *pl_w );

/*==============================================================*/
static void lose_time_ko( PLAYER_WORK *pl_w )
{
	static void ( *lose_jmp[])( PLAYER_WORK *pl_w ) = {
		lose_init, lose_move,
	} ;

	lose_jmp[pl_w->routine_3]( pl_w ) ;
}

/*==============================================================*/
static void lose_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_3 ++ ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->sjump_flag = 0 ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_char_set( pl_w, Char_Win, 5 ) ;
	lose_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void lose_move( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
}

/*==============================================================*/
/*			The win pose from the buttons held, or at random	*/
/*==============================================================*/
static void win_type_set( PLAYER_WORK *pl_w )
{
	static Uint16 type_tbl[16] = { 5, 5, 5, 6, 6, 7, 7, 7, 8, 8, 8, 8, 9, 10, 10, 10 } ;

	if( pl_w->pad_sw.now & KEY_BUTTON ){
		pl_w->type = 5 ;
		if( pl_w->pad_sw.now & KEY_HP ) pl_w->type = 6 ;
		if( pl_w->pad_sw.now & KEY_A1 ) pl_w->type = 7 ;
		if( pl_w->pad_sw.now & KEY_LK ) pl_w->type = 8 ;
		if( pl_w->pad_sw.now & KEY_HK ) pl_w->type = 9 ;
		if( pl_w->pad_sw.now & KEY_A2 ) pl_w->type = 10 ;
	}else{
		pl_w->type = type_tbl[Rnd() & 15] ;
	}
	if( !vs_sys_work.vs_end_flag ){
		if( pl_w->type == 6 || pl_w->type == 8 || pl_w->type == 9 || pl_w->type == 10 ) pl_w->type = 5 ;
	}
}

static void sp_buster( PLAYER_WORK *pl_w );
static void sp_flower( PLAYER_WORK *pl_w );
static void sp_snapback( PLAYER_WORK *pl_w );
static void sp_chouhatsu( PLAYER_WORK *pl_w );
static void sp_drill( PLAYER_WORK *pl_w );
static void sp_beat( PLAYER_WORK *pl_w );
static void sp_item( PLAYER_WORK *pl_w );
static void sp_hyper_roll( PLAYER_WORK *pl_w );
static void sp_item_sel( PLAYER_WORK *pl_w );

/****************************************************************/
void pl1d_sp_atck( PLAYER_WORK *pl_w )
{
	static void ( *sp_atck_jmp[])( PLAYER_WORK *pl_w ) = {
		sp_buster, sp_buster, sp_flower, sp_chouhatsu, sp_drill, sp_beat, sp_item, sp_hyper_roll, sp_snapback, sp_hyper_roll, sp_hyper_roll, sp_hyper_roll, sp_item_sel,
	} ;

	sp_atck_jmp[pl_w->pl_sp_type]( pl_w ) ;
}

static void buster_stand( PLAYER_WORK *pl_w );
static void buster_air( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			The shot, standing or in the air					*/
/*==============================================================*/
static void sp_buster( PLAYER_WORK *pl_w )
{
	static void ( *buster_jmp[])( PLAYER_WORK *pl_w ) = {
		buster_stand, buster_air,
	} ;

	buster_jmp[pl_w->routine_2]( pl_w ) ;
}

static void buster_stand_init( PLAYER_WORK *pl_w );
static void buster_stand_shot( PLAYER_WORK *pl_w );
static void buster_stand_end( PLAYER_WORK *pl_w );

/*==============================================================*/
static void buster_stand( PLAYER_WORK *pl_w )
{
	static void ( *buster_stand_jmp[])( PLAYER_WORK *pl_w ) = {
		buster_stand_init, buster_stand_shot, buster_stand_end,
	} ;

	buster_stand_jmp[pl_w->routine_3]( pl_w ) ;
}

/*==============================================================*/
static void buster_stand_init( PLAYER_WORK *pl_w )
{
	if( pl_w->pl_stance == STANCE_AIR ){
		pl_w->routine_2 = 1 ;
		buster_air( pl_w ) ;
		return ;
	}
	pl_w->routine_3 ++ ;
	pl_dir_set( pl_w ) ;
	pl_color_set( pl_w, 1 ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->atck_no = pl1d_at_buster ;
	HIT_RESET( pl_w ) ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_char_set( pl_w, Char_Special, 0 ) ;
	pl_sp_ef_set( pl_w ) ;
	buster_stand_shot( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void buster_stand_shot( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_3 ++ ;
		pl_w->cg_type = 0 ;
		color_start( pl_w, col_01 ) ;
		color_move( pl_w ) ;
		pls1c_00_set( pl_w, 3 ) ;
		shl1c_00_set( pl_w, 3 ) ;
	}
}

/*--------------------------------------------------------------*/
static void buster_stand_end( PLAYER_WORK *pl_w )
{
	color_move( pl_w ) ;
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void buster_air_init( PLAYER_WORK *pl_w );
static void buster_air_shot( PLAYER_WORK *pl_w );
static void buster_air_end( PLAYER_WORK *pl_w );
static void air_move( PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void buster_air( PLAYER_WORK *pl_w )
{
	static void ( *buster_air_jmp[])( PLAYER_WORK *pl_w ) = {
		buster_air_init, buster_air_shot, buster_air_end,
	} ;

	buster_air_jmp[pl_w->routine_3]( pl_w ) ;
}

/*==============================================================*/
static void buster_air_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_3 ++ ;
	pl_dir_set( pl_w ) ;
	pl_color_set( pl_w, 1 ) ;
	pl_w->spd.x /= 8.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->spd.y /= 8.0f ;
	pl_w->acc.y /= 64.0f ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_w->atck_no = pl1d_at_buster_air ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 2 ) ;
	buster_air_shot( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void buster_air_shot( PLAYER_WORK *pl_w )
{
	air_move( pl_w ) ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_3 ++ ;
		pl_w->cg_type = 0 ;
		color_start( pl_w, col_01 ) ;
		color_move( pl_w ) ;
		pls1c_00_set( pl_w, 5 ) ;
		shl1c_00_set( pl_w, 4 ) ;
	}
}

/*--------------------------------------------------------------*/
static void buster_air_end( PLAYER_WORK *pl_w )
{
	air_move( pl_w ) ;
	color_move( pl_w ) ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->spd.y = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_jump_ret( pl_w ) ;
	}
}

/*==============================================================*/
/*			Moves through the air, not below the ground			*/
/*==============================================================*/
static void air_move( PLAYER_WORK *pl_w )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->ground_y >= pl_w->pos.y ) pl_w->pos.y = pl_w->ground_y ;
}

static void flower_ground( PLAYER_WORK *pl_w );
static void flower_air( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A thrown shot, standing or in the air				*/
/*==============================================================*/
static void sp_flower( PLAYER_WORK *pl_w )
{
	static void ( *flower_jmp[])( PLAYER_WORK *pl_w ) = {
		flower_ground, flower_air,
	} ;

	flower_jmp[pl_w->routine_2]( pl_w ) ;
}

static void flower_init( PLAYER_WORK *pl_w );
static void flower_shot( PLAYER_WORK *pl_w );
static void flower_end( PLAYER_WORK *pl_w );

/*==============================================================*/
static void flower_ground( PLAYER_WORK *pl_w )
{
	static void ( *flower_ground_jmp[])( PLAYER_WORK *pl_w ) = {
		flower_init, flower_shot, flower_end,
	} ;

	flower_ground_jmp[pl_w->routine_3]( pl_w ) ;
}

/*==============================================================*/
static void flower_init( PLAYER_WORK *pl_w )
{
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_color_trans( pl_w, 6, 0, 2 ) ;
	if( pl_w->pl_stance == STANCE_AIR ){
		pl_w->routine_2 = 1 ;
		flower_air( pl_w ) ;
		return ;
	}
	pl_w->routine_3 ++ ;
	pl_dir_set( pl_w ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->atck_no = pl1d_at_flower ;
	HIT_RESET( pl_w ) ;
	pl_gauge_add( pl_w, 10 ) ;
	pl_char_set( pl_w, Char_Special, 3 ) ;
	pl_sp_ef_set( pl_w ) ;
	flower_shot( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void flower_shot( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( !pl_w->cg_type ){
		pl_w->routine_3 ++ ;
		pl_w->cg_type = 0 ;
		shl1d_00_set( pl_w, pl_w->atck_level ) ;
	}
}

/*--------------------------------------------------------------*/
static void flower_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void flower_air_init( PLAYER_WORK *pl_w );
static void flower_air_shot( PLAYER_WORK *pl_w );
static void flower_air_end( PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void flower_air( PLAYER_WORK *pl_w )
{
	static void ( *flower_air_jmp[])( PLAYER_WORK *pl_w ) = {
		flower_air_init, flower_air_shot, flower_air_end,
	} ;

	flower_air_jmp[pl_w->routine_3]( pl_w ) ;
}

/*==============================================================*/
static void flower_air_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_3 ++ ;
	pl_dir_set( pl_w ) ;
	pl_w->spd.x /= 8.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->spd.y /= 8.0f ;
	pl_w->acc.y /= 64.0f ;
	pl_gauge_add( pl_w, 10 ) ;
	pl_w->atck_no = pl1d_at_flower_air ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 5 ) ;
	flower_air_shot( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void flower_air_shot( PLAYER_WORK *pl_w )
{
	air_move( pl_w ) ;
	pl_char_move( pl_w ) ;
	if( !pl_w->cg_type ){
		pl_w->routine_3 ++ ;
		pl_w->cg_type = 0 ;
		shl1d_00_set( pl_w, pl_w->atck_level + 2 ) ;
	}
}

/*--------------------------------------------------------------*/
static void flower_air_end( PLAYER_WORK *pl_w )
{
	air_move( pl_w ) ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->spd.y = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_jump_ret( pl_w ) ;
	}
}

static void snapback_init( PLAYER_WORK *pl_w );
static void snapback_move( PLAYER_WORK *pl_w );

/*==============================================================*/
static void sp_snapback( PLAYER_WORK *pl_w )
{
	static void ( *snapback_jmp[])( PLAYER_WORK *pl_w ) = {
		snapback_init, snapback_move,
	} ;

	snapback_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void snapback_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_color_set( pl_w, 1 ) ;
	pl_dir_set( pl_w ) ;
	pl_sp_ef_set( pl_w ) ;
	pl_w->atck_no = pl1d_at_snapback ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 15 ) ;
}

/*--------------------------------------------------------------*/
static void snapback_move( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	if( pl_char_move( pl_w ) < 0 ){
		pl_stand_ret( pl_w ) ;
		return ;
	}
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		ofs.x = 35.0f ;
		ofs.y = DOT_Y( 48.0f ) ;
		pl_snap_flash( pl_w, &ofs ) ;
	}
}

static void chouhatsu_init( PLAYER_WORK *pl_w );
static void chouhatsu_move( PLAYER_WORK *pl_w );

/*==============================================================*/
static void sp_chouhatsu( PLAYER_WORK *pl_w )
{
	static void ( *chouhatsu_jmp[])( PLAYER_WORK *pl_w ) = {
		chouhatsu_init, chouhatsu_move,
	} ;

	chouhatsu_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void chouhatsu_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_dir_set( pl_w ) ;
	pl_color_set( pl_w, 1 ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->sjump_flag = 0 ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_char_set( pl_w, Char_Extra, 2 ) ;
	chouhatsu_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void chouhatsu_move( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void drill_init( PLAYER_WORK *pl_w );
static void drill_wait( PLAYER_WORK *pl_w );
static void drill_charge( PLAYER_WORK *pl_w );
static void drill_main( PLAYER_WORK *pl_w );
static void drill_finish( PLAYER_WORK *pl_w );
static void drill_end( PLAYER_WORK *pl_w );
static void drill_ctrl( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			The hyper combo where Roll grows huge and is		*/
/*			steered about										*/
/*==============================================================*/
static void sp_drill( PLAYER_WORK *pl_w )
{
	static void ( *drill_jmp[])( PLAYER_WORK *pl_w ) = {
		drill_init, drill_wait, drill_charge, drill_main, drill_finish, drill_end,
	} ;

	pl_w->com_w.step = 0 ;
	drill_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void drill_init( PLAYER_WORK *pl_w )
{
	PL1D_LOCAL_W *lw = ( PL1D_LOCAL_W * )pl_w->tlocal ;

	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_2 ++ ;
	lw->sp_step = 0 ;
	lw->ball_end_flag = 0xff ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->sjump_flag = 0 ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->atck_no = pl1d_at_drill ;
	HIT_RESET( pl_w ) ;
	pls1c_07_set2( pl_w, 3 ) ;
	pl_char_set( pl_w, Char_Super, 7 ) ;
	pl_sp_ef_set( pl_w ) ;
	drill_wait( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void drill_wait( PLAYER_WORK *pl_w )
{
	PL1D_LOCAL_W *lw = ( PL1D_LOCAL_W * )pl_w->tlocal ;
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = pl_w->team_type == 6 ? 2 : 0 ;
	pl_char_move( pl_w ) ;
	if( !pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		lw->join_sp = 2 ;
		lw->apart_flag = 0xff ;
		color_start( pl_w, col_07 ) ;
		ofs.x = -DOT_X( 8.0f ) ;
		ofs.y = 105.0f ;
		ofs.z = 0.0f ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void drill_charge( PLAYER_WORK *pl_w )
{
	static Sint16 *cpu_tbl[4] = { cpu_tbl_0, cpu_tbl_1, cpu_tbl_2, cpu_tbl_3 } ;
	PL1D_LOCAL_W *lw = ( PL1D_LOCAL_W * )pl_w->tlocal ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( !color_move( pl_w ) ){
		pl_w->routine_2 ++ ;
		lw->apart_flag = 0 ;
		lw->turn = 0 ;
		lw->sp_time = 180 ;
		Timer_Gauge_Set( pl_w, &lw->sp_time, 6 ) ;
		lw->cpu_step = cpu_tbl[Rnd() & 3] ;
		lw->cpu_timer = 1 ;
		pl_dir_set( pl_w ) ;
		pl_color_set( pl_w, PC_DEFAULT ) ;
		shl1c_07_set( pl_w, 0 ) ;
		shl1c_07_set( pl_w, 1 ) ;
		shl1c_07_set( pl_w, 3 ) ;
		shl1c_07_set( pl_w, 4 ) ;
		color_start( pl_w, col_08 ) ;
		pl_char_set( pl_w, Char_Super, 8 ) ;
		pl_voice_req( pl_w, 30 ) ;
	}
}

static void drill_stand( PLAYER_WORK *pl_w, PL1D_LOCAL_W *lw );
static void drill_walk( PLAYER_WORK *pl_w, PL1D_LOCAL_W *lw );
static void drill_back( PLAYER_WORK *pl_w, PL1D_LOCAL_W *lw );
static void drill_jump( PLAYER_WORK *pl_w, PL1D_LOCAL_W *lw );
static void drill_jump_f( PLAYER_WORK *pl_w, PL1D_LOCAL_W *lw );
static void drill_jump_b( PLAYER_WORK *pl_w, PL1D_LOCAL_W *lw );
static void drill_turn( PLAYER_WORK *pl_w, PL1D_LOCAL_W *lw );
static void drill_shot( PLAYER_WORK *pl_w, PL1D_LOCAL_W *lw );

/*==============================================================*/
/*			Steered by the stick until the time runs out		*/
/*==============================================================*/
static void drill_main( PLAYER_WORK *pl_w )
{
	static void ( *drill_main_jmp[])( PLAYER_WORK *pl_w, PL1D_LOCAL_W *lw ) = {
		drill_stand, drill_walk, drill_back, drill_jump, drill_jump_f, drill_jump_b, drill_turn, drill_shot,
	} ;
	PL1D_LOCAL_W *lw = ( PL1D_LOCAL_W * )pl_w->tlocal ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	color_move( pl_w ) ;
	if( vs_sys_work.vs_st >= VS_END_WAIT ) lw->sp_time = 0 ;
	if( !lw->sp_time ){
		pl_w->routine_2 ++ ;
		pl_w->routine_3 = 0 ;
		lw->turn = 0 ;
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		color_start( pl_w, col_09 ) ;
		pl_char_set( pl_w, Char_Super, 34 ) ;
		pl_voice_req( pl_w, 43 ) ;
	}else{
		drill_main_jmp[pl_w->routine_3]( pl_w, lw ) ;
	}
}

/*==============================================================*/
/*			Whether the opponent is behind						*/
/*==============================================================*/
static int behind_ck( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em = pl_w->target_w ;

	if( ( pl_w->pl_dir == 0 && pl_w->pos.x < em->pos.x ) || ( pl_w->pl_dir != 0 && pl_w->pos.x > em->pos.x ) ) return 1 ;
	return 0 ;
}

/*==============================================================*/
static void drill_stand( PLAYER_WORK *pl_w, PL1D_LOCAL_W *lw )
{
	if( behind_ck( pl_w ) ){
		pl_w->routine_3 = 6 ;
		lw->sp_step = 0 ;
		drill_turn( pl_w, lw ) ;
	}else{
		drill_ctrl( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void drill_turn( PLAYER_WORK *pl_w, PL1D_LOCAL_W *lw )
{
	if( !lw->sp_step ){
		lw->sp_step ++ ;
		pl_w->pl_dir ^= 1 ;
		pl_w->disp_dir = pl_w->pl_dir ;
		lw->turn = 1 ;
		pl_char_set( pl_w, Char_Super, 10 ) ;
	}
	if( behind_ck( pl_w ) ){
		lw->sp_step = 0 ;
		drill_turn( pl_w, lw ) ;
		return ;
	}
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_3 = 0 ;
		lw->sp_step = 0 ;
		lw->turn = 0 ;
		pl_char_set( pl_w, Char_Super, 8 ) ;
	}
	drill_ctrl( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void drill_walk( PLAYER_WORK *pl_w, PL1D_LOCAL_W *lw )
{
	if( behind_ck( pl_w ) ){
		pl_w->routine_3 = 6 ;
		lw->sp_step = 0 ;
		drill_turn( pl_w, lw ) ;
		return ;
	}
	if( lw->sp_step == 0 ){
		lw->sp_step ++ ;
		lw->turn = 0 ;
		pl_char_set( pl_w, Char_Super, 8 ) ;
	}
	if( !pl_w->pl_dir ) pl_w->pos.x += -5.0f ;
	else pl_w->pos.x += 5.0f ;
	drill_ctrl( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void drill_back( PLAYER_WORK *pl_w, PL1D_LOCAL_W *lw )
{
	if( behind_ck( pl_w ) ){
		pl_w->routine_3 = 6 ;
		lw->sp_step = 0 ;
		drill_turn( pl_w, lw ) ;
		return ;
	}
	if( lw->sp_step == 0 ){
		lw->sp_step ++ ;
		lw->turn = 0 ;
		pl_char_set( pl_w, Char_Super, 8 ) ;
	}
	if( !pl_w->pl_dir ) pl_w->pos.x += DOT_X( 2.5f ) ;
	else pl_w->pos.x += -DOT_X( 2.5f ) ;
	drill_ctrl( pl_w ) ;
}

static void drill_jump_init( PLAYER_WORK *pl_w, PL1D_LOCAL_W *lw );
static void drill_jump_wait( PLAYER_WORK *pl_w, PL1D_LOCAL_W *lw );
static void drill_jump_f_init( PLAYER_WORK *pl_w, PL1D_LOCAL_W *lw );
static void drill_jump_f_wait( PLAYER_WORK *pl_w, PL1D_LOCAL_W *lw );
static void drill_jump_b_init( PLAYER_WORK *pl_w, PL1D_LOCAL_W *lw );
static void drill_jump_b_wait( PLAYER_WORK *pl_w, PL1D_LOCAL_W *lw );
static void drill_rise( PLAYER_WORK *pl_w, PL1D_LOCAL_W *lw );
static void drill_fall( PLAYER_WORK *pl_w, PL1D_LOCAL_W *lw );
static void drill_land( PLAYER_WORK *pl_w, PL1D_LOCAL_W *lw );

/*--------------------------------------------------------------*/
static void drill_jump( PLAYER_WORK *pl_w, PL1D_LOCAL_W *lw )
{
	static void ( *drill_jump_jmp[])( PLAYER_WORK *pl_w, PL1D_LOCAL_W *lw ) = {
		drill_jump_init, drill_jump_wait, drill_rise, drill_fall, drill_land,
	} ;

	drill_jump_jmp[lw->sp_step]( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void drill_jump_f( PLAYER_WORK *pl_w, PL1D_LOCAL_W *lw )
{
	static void ( *drill_jump_f_jmp[])( PLAYER_WORK *pl_w, PL1D_LOCAL_W *lw ) = {
		drill_jump_f_init, drill_jump_f_wait, drill_rise, drill_fall, drill_land,
	} ;

	drill_jump_f_jmp[lw->sp_step]( pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void drill_jump_b( PLAYER_WORK *pl_w, PL1D_LOCAL_W *lw )
{
	static void ( *drill_jump_b_jmp[])( PLAYER_WORK *pl_w, PL1D_LOCAL_W *lw ) = {
		drill_jump_b_init, drill_jump_b_wait, drill_rise, drill_fall, drill_land,
	} ;

	drill_jump_b_jmp[lw->sp_step]( pl_w, lw ) ;
}

/*==============================================================*/
static void drill_jump_init( PLAYER_WORK *pl_w, PL1D_LOCAL_W *lw )
{
	lw->sp_step ++ ;
	pl_w->timer0 = 6 ;
	lw->turn = 0 ;
	pl_w->spd.x = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_char_set( pl_w, Char_Super, 8 ) ;
	drill_jump_wait( pl_w, lw ) ;
}

/*==============================================================*/
static void drill_jump_wait( PLAYER_WORK *pl_w, PL1D_LOCAL_W *lw )
{
	if( --pl_w->timer0 == 0 ){
		lw->sp_step ++ ;
		pl_w->spd.y = DOT_Y( 8.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
		shl1c_07_set( pl_w, 5 ) ;
		shl1c_07_set( pl_w, 6 ) ;
		shl1c_07_set( pl_w, 7 ) ;
	}
}

/*==============================================================*/
static void drill_jump_f_init( PLAYER_WORK *pl_w, PL1D_LOCAL_W *lw )
{
	lw->sp_step ++ ;
	pl_w->timer0 = 6 ;
	lw->turn = 0 ;
	if( !pl_w->pl_dir ) pl_w->spd.x = -5.0f ;
	else pl_w->spd.x = 5.0f ;
	pl_w->acc.x = 0.0f ;
	pl_char_set( pl_w, Char_Super, 8 ) ;
	drill_jump_f_wait( pl_w, lw ) ;
}

/*==============================================================*/
static void drill_jump_f_wait( PLAYER_WORK *pl_w, PL1D_LOCAL_W *lw )
{
	if( --pl_w->timer0 == 0 ){
		lw->sp_step ++ ;
		pl_w->spd.y = DOT_Y( 8.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
		shl1c_07_set( pl_w, 8 ) ;
		shl1c_07_set( pl_w, 9 ) ;
		shl1c_07_set( pl_w, 10 ) ;
	}
}

/*==============================================================*/
static void drill_jump_b_init( PLAYER_WORK *pl_w, PL1D_LOCAL_W *lw )
{
	lw->sp_step ++ ;
	pl_w->timer0 = 6 ;
	lw->turn = 0 ;
	if( !pl_w->pl_dir ) pl_w->spd.x = DOT_X( 2.5f ) ;
	else pl_w->spd.x = -DOT_X( 2.5f ) ;
	pl_w->acc.x = 0.0f ;
	pl_char_set( pl_w, Char_Super, 8 ) ;
	drill_jump_b_wait( pl_w, lw ) ;
}

/*==============================================================*/
static void drill_jump_b_wait( PLAYER_WORK *pl_w, PL1D_LOCAL_W *lw )
{
	if( --pl_w->timer0 == 0 ){
		lw->sp_step ++ ;
		pl_w->spd.y = DOT_Y( 8.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
		shl1c_07_set( pl_w, 11 ) ;
		shl1c_07_set( pl_w, 12 ) ;
		shl1c_07_set( pl_w, 13 ) ;
	}
}

/*==============================================================*/
static void drill_rise( PLAYER_WORK *pl_w, PL1D_LOCAL_W *lw )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->spd.y <= 0.0f ){
		lw->sp_step ++ ;
		pl_w->acc.y = -DOT_Y( 0.75f ) ;
	}
}

/*==============================================================*/
static void drill_fall( PLAYER_WORK *pl_w, PL1D_LOCAL_W *lw )
{
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y <= pl_w->ground_y ){
		lw->sp_step ++ ;
		pl_w->timer0 = 10 ;
		pl_w->pos.y = pl_w->ground_y ;
		pl_land_ef_set( pl_w ) ;
	}
}

/*==============================================================*/
static void drill_land( PLAYER_WORK *pl_w, PL1D_LOCAL_W *lw )
{
	if( --pl_w->timer0 == 0 ) drill_ctrl( pl_w ) ;
}

static void drill_shot_init( PLAYER_WORK *pl_w, PL1D_LOCAL_W *lw );
static void drill_shot_wait( PLAYER_WORK *pl_w, PL1D_LOCAL_W *lw );
static void drill_shot_slide( PLAYER_WORK *pl_w, PL1D_LOCAL_W *lw );
static void drill_shot_stop( PLAYER_WORK *pl_w, PL1D_LOCAL_W *lw );
static void drill_shot_end( PLAYER_WORK *pl_w, PL1D_LOCAL_W *lw );

/*==============================================================*/
/*			A button: a dash with a shot						*/
/*==============================================================*/
static void drill_shot( PLAYER_WORK *pl_w, PL1D_LOCAL_W *lw )
{
	static void ( *drill_shot_jmp[])( PLAYER_WORK *pl_w, PL1D_LOCAL_W *lw ) = {
		drill_shot_init, drill_shot_wait, drill_shot_slide, drill_shot_stop, drill_shot_end,
	} ;

	drill_shot_jmp[lw->sp_step]( pl_w, lw ) ;
}

/*==============================================================*/
static void drill_shot_init( PLAYER_WORK *pl_w, PL1D_LOCAL_W *lw )
{
	lw->sp_step ++ ;
	pl_w->timer0 = 6 ;
	lw->turn = 0 ;
	if( !pl_w->pl_dir ) pl_w->spd.x = -DOT_X( 8.0f ) ;
	else pl_w->spd.x = DOT_X( 8.0f ) ;
	pl_w->acc.x = 0.0f ;
	pl_char_set( pl_w, Char_Super, 8 ) ;
	shl1c_07_set( pl_w, 2 ) ;
	drill_shot_wait( pl_w, lw ) ;
}

/*==============================================================*/
static void drill_shot_wait( PLAYER_WORK *pl_w, PL1D_LOCAL_W *lw )
{
	if( --pl_w->timer0 == 0 ){
		lw->sp_step ++ ;
		pl_w->timer0 = 24 ;
	}
}

/*==============================================================*/
static void drill_shot_slide( PLAYER_WORK *pl_w, PL1D_LOCAL_W *lw )
{
	if( --pl_w->timer0 == 0 ){
		lw->sp_step ++ ;
		pl_w->timer0 = 10 ;
		/* the same both ways, as in the arcade */
		if( !pl_w->pl_dir ) pl_w->acc.x = DOT_X( 0.25f ) ;
		else pl_w->acc.x = DOT_X( 0.25f ) ;
	}
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
}

/*==============================================================*/
static void drill_shot_stop( PLAYER_WORK *pl_w, PL1D_LOCAL_W *lw )
{
	if( --pl_w->timer0 == 0 ){
		lw->sp_step ++ ;
		pl_w->timer0 = 10 ;
	}
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
}

/*==============================================================*/
static void drill_shot_end( PLAYER_WORK *pl_w, PL1D_LOCAL_W *lw )
{
	if( --pl_w->timer0 == 0 ){
		if( behind_ck( pl_w ) ){
			pl_w->routine_3 = 6 ;
			lw->sp_step = 0 ;
			drill_turn( pl_w, lw ) ;
		}else{
			drill_ctrl( pl_w ) ;
		}
	}
}

/*--------------------------------------------------------------*/
static void drill_finish( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_w->routine_3 = 0 ;
	}
	color_move( pl_w ) ;
}

static void drill_end_init( PLAYER_WORK *pl_w );
static void drill_end_stand( PLAYER_WORK *pl_w );
static void drill_end_fall( PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void drill_end( PLAYER_WORK *pl_w )
{
	static void ( *drill_end_jmp[])( PLAYER_WORK *pl_w ) = {
		drill_end_init, drill_end_stand, drill_end_fall,
	} ;

	drill_end_jmp[pl_w->routine_3]( pl_w ) ;
}

/*==============================================================*/
static void drill_end_init( PLAYER_WORK *pl_w )
{
	PL1D_LOCAL_W *lw = ( PL1D_LOCAL_W * )pl_w->tlocal ;

	pl_w->routine_3 ++ ;
	lw->apart_flag = 0xff ;
	color_start( pl_w, col_10 ) ;
	if( pl_w->pos.y > pl_w->ground_y ){
		pl_w->routine_3 = 2 ;
		if( !pl_w->pl_dir ) pl_w->spd.x = 2.5f ;
		else pl_w->spd.x = -2.5f ;
		pl_w->acc.x = 0.0f ;
		pl_w->spd.y = DOT_Y( 2.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
		pl_char_set( pl_w, Char_Super, 16 ) ;
		drill_end_fall( pl_w ) ;
	}else{
		pl_char_set( pl_w, Char_Super, 15 ) ;
		drill_end_stand( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void drill_end_stand( PLAYER_WORK *pl_w )
{
	color_move( pl_w ) ;
	if( pl_char_move( pl_w ) < 0 ){
		PL1D_LOCAL_W *lw = ( PL1D_LOCAL_W * )pl_w->tlocal ;

		lw->join_sp = 0 ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_color_set( pl_w, PC_DEFAULT ) ;
		pl_stand_ret( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void drill_end_fall( PLAYER_WORK *pl_w )
{
	color_move( pl_w ) ;
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y <= pl_w->ground_y ){
		PL1D_LOCAL_W *lw = ( PL1D_LOCAL_W * )pl_w->tlocal ;

		pl_w->pos.y = pl_w->ground_y ;
		lw->join_sp = 0 ;
		pl_color_set( pl_w, PC_DEFAULT ) ;
		pl_jump_ret( pl_w ) ;
	}
}

static void drill_cpu( PLAYER_WORK *pl_w );
static void drill_cpu_chase( PLAYER_WORK *pl_w );
static void drill_cpu_tbl( PLAYER_WORK *pl_w );
static void drill_cpu_back( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			The next step from the stick and buttons, or		*/
/*			from the computer									*/
/*==============================================================*/
static void drill_ctrl( PLAYER_WORK *pl_w )
{
	PL1D_LOCAL_W *lw = ( PL1D_LOCAL_W * )pl_w->tlocal ;
	Uint16 sw ;

	if( pl_w->cpu_flag ){
		drill_cpu( pl_w ) ;
		return ;
	}
	if( ( sw = pl_w->pl_sw_new & ( KEY_LEVER | KEY_ATCK ) ) ){
		if( pl_w->pl_sw_now & KEY_ATCK ){
			pl_w->routine_3 = 7 ;
			lw->sp_step = 0 ;
			return ;
		}
		if( sw & KEY_UP ){
			if( sw & KEY_FORWARD ) pl_w->routine_3 = 4 ;
			else if( sw & KEY_BACK ) pl_w->routine_3 = 5 ;
			else pl_w->routine_3 = 3 ;
			lw->sp_step = 0 ;
			return ;
		}
		if( sw & KEY_FORWARD ){
			if( pl_w->routine_3 == 1 ) return ;
			pl_w->routine_3 = 1 ;
			lw->sp_step = 0 ;
			return ;
		}
		if( sw & KEY_BACK ){
			if( pl_w->routine_3 == 2 ) return ;
			pl_w->routine_3 = 2 ;
			lw->sp_step = 0 ;
			return ;
		}
	}
	if( lw->turn ) return ;
	if( !pl_w->routine_3 ) return ;
	pl_w->routine_3 = 0 ;
	lw->sp_step = 0 ;
}

/*==============================================================*/
/*			The computer's next step							*/
/*==============================================================*/
static void drill_cpu( PLAYER_WORK *pl_w )
{
	static void ( *drill_cpu_jmp[])( PLAYER_WORK *pl_w ) = {
		drill_cpu_chase, drill_cpu_tbl, drill_cpu_back,
	} ;

	drill_cpu_jmp[pl_w->atck_level]( pl_w ) ;
}

/*==============================================================*/
/*			The computer: shoot from afar, jump at one			*/
/*			above, else walk in									*/
/*==============================================================*/
static void drill_cpu_chase( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em = pl_w->target_w ;
	PL1D_LOCAL_W *lw = ( PL1D_LOCAL_W * )pl_w->tlocal ;
	Float dx = em->pos.x - pl_w->pos.x ;

	if( dx < -160.0f || dx > 160.0f ){
		pl_w->routine_3 = 7 ;
		lw->sp_step = 0 ;
	}else if( em->pos.y >= pl_w->pos.y + DOT_Y( 32.0f ) ){
		pl_w->routine_3 = 4 ;
		lw->sp_step = 0 ;
	}else if( pl_w->routine_3 != 1 ){
		pl_w->routine_3 = 1 ;
		lw->sp_step = 0 ;
	}
}

/*==============================================================*/
/*			The computer: the steps of a table					*/
/*==============================================================*/
static void drill_cpu_tbl( PLAYER_WORK *pl_w )
{
	PL1D_LOCAL_W *lw = ( PL1D_LOCAL_W * )pl_w->tlocal ;
	Sint16 *tbl ;

	if( --lw->cpu_timer == 0 ){
		tbl = lw->cpu_step ;
		pl_w->routine_3 = *tbl++ ;
		lw->cpu_timer = *tbl++ ;
		lw->cpu_step = tbl ;
		lw->sp_step = 0 ;
	}
}

/*==============================================================*/
/*			The computer: walk back								*/
/*==============================================================*/
static void drill_cpu_back( PLAYER_WORK *pl_w )
{
	PL1D_LOCAL_W *lw = ( PL1D_LOCAL_W * )pl_w->tlocal ;

	if( pl_w->routine_3 != 2 ){
		pl_w->routine_3 = 2 ;
		lw->sp_step = 0 ;
	}
}

static void beat_init( PLAYER_WORK *pl_w );
static void beat_wait( PLAYER_WORK *pl_w );
static void beat_charge( PLAYER_WORK *pl_w );
static void beat_main( PLAYER_WORK *pl_w );
static void beat_finish( PLAYER_WORK *pl_w );
static void beat_end( PLAYER_WORK *pl_w );
static Uint8 beat_dir( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			The hyper combo where Roll flies about, steered		*/
/*			by the stick										*/
/*==============================================================*/
static void sp_beat( PLAYER_WORK *pl_w )
{
	static void ( *beat_jmp[])( PLAYER_WORK *pl_w ) = {
		beat_init, beat_wait, beat_charge, beat_main, beat_finish, beat_end,
	} ;

	pl_w->com_w.step = 0 ;
	beat_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void beat_init( PLAYER_WORK *pl_w )
{
	PL1D_LOCAL_W *lw = ( PL1D_LOCAL_W * )pl_w->tlocal ;

	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_2 ++ ;
	lw->sp_step = 0 ;
	lw->ball_end_flag = 0xff ;
	lw->apart_flag = 0xff ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->atck_no = pl1d_at_beat ;
	pl_w->hit_flag = pl_w->atck_attr = 0 ;
	pl_w->unk_1c4 = 0 ;
	vs_sys_w->atck_cnt[pl_w->pl_no] ++ ;
	pls1c_07_set2( pl_w, 1 ) ;
	pl_char_set( pl_w, Char_Super, 17 ) ;
	if( pl_w->pl_stance != STANCE_AIR ) pl_sp_ef_set( pl_w ) ;
	beat_wait( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void beat_wait( PLAYER_WORK *pl_w )
{
	PL1D_LOCAL_W *lw = ( PL1D_LOCAL_W * )pl_w->tlocal ;
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	if( !pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		lw->join_sp = 4 ;
		lw->apart_flag = 0xff ;
		color_start( pl_w, col_11 ) ;
		ofs.x = -DOT_X( 8.0f ) ;
		ofs.y = 105.0f ;
		ofs.z = 0.0f ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void beat_charge( PLAYER_WORK *pl_w )
{
	static Sint16 *cpu_beat_tbl[4] = { cpu_beat_tbl_0, cpu_beat_tbl_1, cpu_beat_tbl_2, cpu_beat_tbl_3 } ;
	PL1D_LOCAL_W *lw = ( PL1D_LOCAL_W * )pl_w->tlocal ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( !color_move( pl_w ) ){
		pl_w->routine_2 ++ ;
		lw->apart_flag = 0 ;
		lw->turn = 0 ;
		lw->arm_fire = 0 ;
		pl_w->fly_flag = 1 ;
		lw->sp_time = 180 ;
		Timer_Gauge_Set( pl_w, &lw->sp_time, 6 ) ;
		lw->cpu_step = cpu_beat_tbl[Rnd() & 3] ;
		lw->cpu_timer = 1 ;
		pl_dir_set( pl_w ) ;
		pl_color_set( pl_w, PC_DEFAULT ) ;
		shl1c_08_set( pl_w, 0 ) ;
		shl1c_08_set( pl_w, 1 ) ;
		shl1c_08_set( pl_w, 2 ) ;
		shl1c_08_set( pl_w, 3 ) ;
		shl1c_08_set( pl_w, 4 ) ;
		color_start( pl_w, col_12 ) ;
		pl_char_set( pl_w, Char_Super, 18 ) ;
	}
}

/*--------------------------------------------------------------*/
static void beat_main( PLAYER_WORK *pl_w )
{
	PL1D_LOCAL_W *lw = ( PL1D_LOCAL_W * )pl_w->tlocal ;
	Uint8 dir ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	color_move( pl_w ) ;
	if( vs_sys_work.vs_st >= VS_END_WAIT ) lw->sp_time = 0 ;
	if( !lw->sp_time ){
		pl_w->routine_2 ++ ;
		pl_w->routine_3 = 0 ;
		lw->turn = 0 ;
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
		color_start( pl_w, col_13 ) ;
		pl_char_set( pl_w, Char_Super, 33 ) ;
		return ;
	}
	if( !lw->arm_fire && behind_ck( pl_w ) ){
		lw->sp_step = 1 ;
		pl_w->pl_dir ^= 1 ;
		pl_w->disp_dir = pl_w->pl_dir ;
		lw->turn = 1 ;
		pl_char_set( pl_w, Char_Super, 20 ) ;
	}
	if( lw->sp_step && pl_char_move( pl_w ) < 0 ){
		lw->sp_step = 0 ;
		lw->turn = 0 ;
		pl_char_set( pl_w, Char_Super, 18 ) ;
	}
	if( ( dir = beat_dir( pl_w ) ) != 0xff ){
		pl_w->dir = dir ;
		dir_move( pl_w, 700 ) ;
		if( pl_w->pos.y < pl_w->ground_y ) pl_w->pos.y = pl_w->ground_y ;
	}
}

/*--------------------------------------------------------------*/
static void beat_finish( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_w->routine_3 = 0 ;
	}
	color_move( pl_w ) ;
}

static void beat_end_init( PLAYER_WORK *pl_w );
static void beat_end_stand( PLAYER_WORK *pl_w );
static void beat_end_fall( PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void beat_end( PLAYER_WORK *pl_w )
{
	static void ( *beat_end_jmp[])( PLAYER_WORK *pl_w ) = {
		beat_end_init, beat_end_stand, beat_end_fall,
	} ;

	beat_end_jmp[pl_w->routine_3]( pl_w ) ;
}

/*==============================================================*/
static void beat_end_init( PLAYER_WORK *pl_w )
{
	PL1D_LOCAL_W *lw = ( PL1D_LOCAL_W * )pl_w->tlocal ;

	pl_w->routine_3 ++ ;
	pl_w->fly_flag = 0 ;
	lw->apart_flag = 0xff ;
	color_start( pl_w, col_14 ) ;
	if( pl_w->pos.y > pl_w->ground_y ){
		pl_w->routine_3 = 2 ;
		if( !pl_w->pl_dir ) pl_w->spd.x = 2.5f ;
		else pl_w->spd.x = -2.5f ;
		pl_w->acc.x = 0.0f ;
		pl_w->spd.y = DOT_Y( 2.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
		pl_char_set( pl_w, Char_Super, 26 ) ;
		beat_end_fall( pl_w ) ;
	}else{
		pl_char_set( pl_w, Char_Super, 25 ) ;
		beat_end_stand( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void beat_end_stand( PLAYER_WORK *pl_w )
{
	color_move( pl_w ) ;
	if( pl_char_move( pl_w ) < 0 ){
		PL1D_LOCAL_W *lw = ( PL1D_LOCAL_W * )pl_w->tlocal ;

		lw->join_sp = 0 ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_color_set( pl_w, PC_DEFAULT ) ;
		pl_stand_ret( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void beat_end_fall( PLAYER_WORK *pl_w )
{
	color_move( pl_w ) ;
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y <= pl_w->ground_y ){
		PL1D_LOCAL_W *lw = ( PL1D_LOCAL_W * )pl_w->tlocal ;

		pl_w->pos.y = pl_w->ground_y ;
		lw->join_sp = 0 ;
		pl_color_set( pl_w, PC_DEFAULT ) ;
		pl_jump_ret( pl_w ) ;
	}
}

static Uint8 beat_cpu( PLAYER_WORK *pl_w );
static Uint8 beat_cpu_chase( PLAYER_WORK *pl_w );
static Uint8 beat_cpu_tbl( PLAYER_WORK *pl_w );
static Uint8 beat_cpu_away( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			The way to fly, 0 to 31, or 0xff for none; the		*/
/*			buttons for a shot go in lw->sw_shot				*/
/*==============================================================*/
static Uint8 beat_dir( PLAYER_WORK *pl_w )
{
	static Uint8 dir_tbl[16] = {
		0xff, 0x08, 0x18, 0x08, 0x10, 0x0c, 0x14, 0x0c, 0x00, 0x04, 0x1c, 0x04, 0x00, 0x04, 0x1c, 0x04,
	} ;
	PL1D_LOCAL_W *lw = ( PL1D_LOCAL_W * )pl_w->tlocal ;
	Uint16 sw ;

	if( pl_w->cpu_flag ) return beat_cpu( pl_w ) ;
	lw->sw_shot = 0 ;
	sw = pl_w->pl_now_lvbt ;
	if( sw & 0x0300 ) lw->sw_shot |= 1 ;
	if( sw & 0x0060 ) lw->sw_shot |= 2 ;
	sw = pl_w->pl_new_lvbt ;
	sw = ( sw & 0x3c00 ) >> 10 ;
	return dir_tbl[sw] ;
}

/*==============================================================*/
/*			The computer's way to fly							*/
/*==============================================================*/
static Uint8 beat_cpu( PLAYER_WORK *pl_w )
{
	static Uint8 ( *beat_cpu_jmp[])( PLAYER_WORK *pl_w ) = {
		beat_cpu_chase, beat_cpu_tbl, beat_cpu_away,
	} ;

	return beat_cpu_jmp[pl_w->atck_level]( pl_w ) ;
}

/*==============================================================*/
/*			The computer: straight at the opponent				*/
/*==============================================================*/
static Uint8 beat_cpu_chase( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em = pl_w->target_w ;
	PL1D_LOCAL_W *lw = ( PL1D_LOCAL_W * )pl_w->tlocal ;
	Uint8 dir ;

	lw->sw_shot = 0xff ;
	dir = caldir_pos_256( &pl_w->pos, &em->pos ) >> 3 ;
	return dir ;
}

/*==============================================================*/
/*			The computer: the steps of a table					*/
/*==============================================================*/
static Uint8 beat_cpu_tbl( PLAYER_WORK *pl_w )
{
	PL1D_LOCAL_W *lw = ( PL1D_LOCAL_W * )pl_w->tlocal ;
	Uint8 dir = pl_w->dir ;
	Sint16 *tbl ;

	if( --lw->cpu_timer == 0 ){
		tbl = lw->cpu_step ;
		lw->sw_shot = *tbl++ ;
		dir = *tbl++ ;
		if( pl_w->pl_dir ) dir = -dir & 0x1f ;
		lw->cpu_timer = *tbl++ ;
		lw->cpu_step = tbl ;
	}
	return dir ;
}

/*==============================================================*/
/*			The computer: straight away from the opponent		*/
/*==============================================================*/
static Uint8 beat_cpu_away( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em = pl_w->target_w ;
	PL1D_LOCAL_W *lw = ( PL1D_LOCAL_W * )pl_w->tlocal ;
	Uint8 dir ;

	lw->sw_shot = 0xff ;
	dir = caldir_pos_256( &pl_w->pos, &em->pos ) >> 3 ;
	dir = ( dir + 0xf0 ) & 0x1f ;
	return dir ;
}
static void item_ball( PLAYER_WORK *pl_w );
static void item_ball_init( PLAYER_WORK *pl_w );
static void item_ball_move( PLAYER_WORK *pl_w );
static void item_ball_end( PLAYER_WORK *pl_w );
static void item_tornado( PLAYER_WORK *pl_w );
static void item_tornado_init( PLAYER_WORK *pl_w );
static void item_tornado_move( PLAYER_WORK *pl_w );
static void item_tornado_end( PLAYER_WORK *pl_w );
static void item_shield( PLAYER_WORK *pl_w );
static void item_shield_set( PLAYER_WORK *pl_w );
static void item_shield_init( PLAYER_WORK *pl_w );
static void item_shield_move( PLAYER_WORK *pl_w );
static void item_shield_end( PLAYER_WORK *pl_w );
static void item_throw( PLAYER_WORK *pl_w );
static void item_throw_init( PLAYER_WORK *pl_w );
static void item_throw_move( PLAYER_WORK *pl_w );
static void item_throw_end( PLAYER_WORK *pl_w );
static void item_air_throw( PLAYER_WORK *pl_w );
static void item_air_throw_init( PLAYER_WORK *pl_w );
static void item_air_throw_move( PLAYER_WORK *pl_w );
static void item_air_throw_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			Uses the item Roll has chosen						*/
/*==============================================================*/
static void sp_item( PLAYER_WORK *pl_w )
{
	static void ( *item_jmp[])( PLAYER_WORK *pl_w ) = {
		item_ball, item_tornado, item_shield,
	} ;
	PL1D_LOCAL_W *lw = ( PL1D_LOCAL_W * )pl_w->tlocal ;

	item_jmp[lw->item]( pl_w ) ;
}

/*==============================================================*/
static void item_ball( PLAYER_WORK *pl_w )
{
	static void ( *item_ball_jmp[])( PLAYER_WORK *pl_w ) = {
		item_ball_init, item_ball_move, item_ball_end,
	} ;

	item_ball_jmp[pl_w->routine_3]( pl_w ) ;
}

/*==============================================================*/
static void item_ball_init( PLAYER_WORK *pl_w )
{
	PL1D_LOCAL_W *lw = ( PL1D_LOCAL_W * )pl_w->tlocal ;

	pl_w->routine_3 ++ ;
	lw->ball_end_flag = 0xff ;
	pl_dir_set( pl_w ) ;
	pl_color_set( pl_w, 1 ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->sjump_flag = 0 ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_w->atck_no = pl1d_at_rockball ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 9 ) ;
	pl_sp_ef_set( pl_w ) ;
	item_ball_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void item_ball_move( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_3 ++ ;
		pl_w->cg_type = 0 ;
		shl1c_02_set( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void item_ball_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
static void item_tornado( PLAYER_WORK *pl_w )
{
	static void ( *item_tornado_jmp[])( PLAYER_WORK *pl_w ) = {
		item_tornado_init, item_tornado_move, item_tornado_end,
	} ;

	item_tornado_jmp[pl_w->routine_3]( pl_w ) ;
}

/*==============================================================*/
static void item_tornado_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_3 ++ ;
	pl_dir_set( pl_w ) ;
	pl_color_set( pl_w, 1 ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->sjump_flag = 0 ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_w->atck_no = pl1d_at_item ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 10 ) ;
	pl_sp_ef_set( pl_w ) ;
	item_tornado_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void item_tornado_move( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_3 ++ ;
		pl_w->cg_type = 0 ;
		shl1c_03_set( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void item_tornado_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*==============================================================*/
/*			The third item is put up first and thrown on the	*/
/*			next use											*/
/*==============================================================*/
static void item_shield( PLAYER_WORK *pl_w )
{
	static void ( *item_shield_jmp[])( PLAYER_WORK *pl_w ) = {
		item_shield_set, item_throw, item_air_throw,
	} ;

	item_shield_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void item_shield_set( PLAYER_WORK *pl_w )
{
	static void ( *item_shield_set_jmp[])( PLAYER_WORK *pl_w ) = {
		item_shield_init, item_shield_move, item_shield_end,
	} ;

	item_shield_set_jmp[pl_w->routine_3]( pl_w ) ;
}

/*==============================================================*/
static void item_shield_init( PLAYER_WORK *pl_w )
{
	PL1D_LOCAL_W *lw = ( PL1D_LOCAL_W * )pl_w->tlocal ;

	if( lw->shield_flag ){
		lw->shield_st = -1 ;
		if( pl_w->pl_stance == STANCE_AIR ){
			pl_w->routine_2 = 2 ;
			item_air_throw( pl_w ) ;
			return ;
		}
		pl_w->routine_2 = 1 ;
		item_throw( pl_w ) ;
		return ;
	}
	pl_w->routine_3 ++ ;
	pl_dir_set( pl_w ) ;
	pl_color_set( pl_w, 1 ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->sjump_flag = 0 ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_w->atck_no = pl1d_at_item ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 11 ) ;
	pl_sp_ef_set( pl_w ) ;
	item_shield_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void item_shield_move( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_3 ++ ;
		pl_w->cg_type = 0 ;
		shl1c_04_set( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void item_shield_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void item_throw( PLAYER_WORK *pl_w )
{
	static void ( *item_throw_jmp[])( PLAYER_WORK *pl_w ) = {
		item_throw_init, item_throw_move, item_throw_end,
	} ;

	item_throw_jmp[pl_w->routine_3]( pl_w ) ;
}

/*==============================================================*/
static void item_throw_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_3 ++ ;
	pl_dir_set( pl_w ) ;
	pl_color_set( pl_w, 1 ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->sjump_flag = 0 ;
	pl_w->pl_stance = STANCE_STAND ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_w->atck_no = pl1d_at_shield_throw ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 13 ) ;
	pl_sp_ef_set( pl_w ) ;
	item_throw_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void item_throw_move( PLAYER_WORK *pl_w )
{
	PL1D_LOCAL_W *lw = ( PL1D_LOCAL_W * )pl_w->tlocal ;

	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_3 ++ ;
		lw->shield_throw_flag = 1 ;
	}
}

/*--------------------------------------------------------------*/
static void item_throw_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void item_air_throw( PLAYER_WORK *pl_w )
{
	static void ( *item_air_throw_jmp[])( PLAYER_WORK *pl_w ) = {
		item_air_throw_init, item_air_throw_move, item_air_throw_end,
	} ;

	item_air_throw_jmp[pl_w->routine_3]( pl_w ) ;
}

/*==============================================================*/
static void item_air_throw_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_3 ++ ;
	pl_dir_set( pl_w ) ;
	pl_color_set( pl_w, 1 ) ;
	pl_w->spd.x /= 8.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->spd.y /= 8.0f ;
	pl_w->acc.y /= 64.0f ;
	pl_gauge_add( pl_w, 5 ) ;
	pl_w->atck_no = pl1d_at_shield_throw ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Special, 14 ) ;
	item_air_throw_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void item_air_throw_move( PLAYER_WORK *pl_w )
{
	PL1D_LOCAL_W *lw = ( PL1D_LOCAL_W * )pl_w->tlocal ;

	air_move( pl_w ) ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_3 ++ ;
		lw->shield_throw_flag = 1 ;
	}
}

/*--------------------------------------------------------------*/
static void item_air_throw_end( PLAYER_WORK *pl_w )
{
	air_move( pl_w ) ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->spd.y = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_jump_ret( pl_w ) ;
	}
}

static void hyper_roll_init( PLAYER_WORK *pl_w );
static void hyper_roll_wait( PLAYER_WORK *pl_w );
static void hyper_roll_ready( PLAYER_WORK *pl_w );
static void hyper_roll_call( PLAYER_WORK *pl_w );
static void hyper_roll_glow( PLAYER_WORK *pl_w );
static void hyper_roll_fire( PLAYER_WORK *pl_w );
static void hyper_roll_burst( PLAYER_WORK *pl_w );
static void hyper_roll_main_init( PLAYER_WORK *pl_w );
static void hyper_roll_main( PLAYER_WORK *pl_w );
static void hyper_roll_finish( PLAYER_WORK *pl_w );
static void hyper_roll_end( PLAYER_WORK *pl_w );
static Sint16 hyper_roll_eff_set( PLAYER_WORK *pl_w );
static Sint16 hyper_roll_hit_set( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			Hyper Roll; pl_sp_type 9 to 11 run it too			*/
/*==============================================================*/
static void sp_hyper_roll( PLAYER_WORK *pl_w )
{
	static void ( *hyper_roll_jmp[])( PLAYER_WORK *pl_w ) = {
		hyper_roll_init, hyper_roll_wait, hyper_roll_ready, hyper_roll_call, hyper_roll_glow, hyper_roll_fire,
		hyper_roll_burst, hyper_roll_main_init, hyper_roll_main, hyper_roll_finish, hyper_roll_end,
	} ;

	pl_w->com_w.step = 0 ;
	hyper_roll_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void hyper_roll_init( PLAYER_WORK *pl_w )
{
	PL1D_LOCAL_W *lw = ( PL1D_LOCAL_W * )pl_w->tlocal ;

	if( pl_w->team_type == 6 ){
		pl_w->stop_move_flag = 0xff ;
		pl_w->stop_move_timer = 16 ;
	}
	pl_w->routine_2 ++ ;
	lw->ball_end_flag = 0xff ;
	lw->apart_flag = 0xff ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_dir_set( pl_w ) ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	pl_w->atck_no = pl1d_at_hyper_roll ;
	HIT_RESET( pl_w ) ;
	if( pl_w->team_type != 5 ){
		pls1c_07_set2( pl_w, 4 ) ;
		pls1c_07_set2( pl_w, 0 ) ;
		pls1c_07_set2( pl_w, 2 ) ;
	}
	pl_char_set( pl_w, Char_Super, 0 ) ;
	if( pl_w->pl_stance != STANCE_AIR ){
		pl_w->pos.y = pl_w->ground_y ;
		pl_sp_ef_set( pl_w ) ;
	}
	hyper_roll_wait( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void hyper_roll_wait( PLAYER_WORK *pl_w )
{
	PL1D_LOCAL_W *lw = ( PL1D_LOCAL_W * )pl_w->tlocal ;
	VEC3 ofs ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->stop_move_timer = pl_w->team_type == 6 ? 2 : 0 ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_w->stop_move_flag = 0 ;
		pl_w->stop_move_timer = 0 ;
		lw->join_sp = 6 ;
		color_start( pl_w, col_03 ) ;
		pl_char_set( pl_w, Char_Super, pl_w->pl_stance != STANCE_AIR ? 1 : 2 ) ;
		scr_excep_set( pl_w, 13, !pl_w->pl_no ? 3 : 4 ) ;
		ofs.x = -DOT_X( 8.0f ) ;
		ofs.y = 105.0f ;
		ofs.z = 0.0f ;
		pl_hyper_start( pl_w, &ofs, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void hyper_roll_ready( PLAYER_WORK *pl_w )
{
	PL1D_LOCAL_W *lw = ( PL1D_LOCAL_W * )pl_w->tlocal ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->routine_2 ++ ;
	lw->apart_flag = 0 ;
	hyper_roll_call( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void hyper_roll_call( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		pl_voice_req( pl_w, 45 ) ;
		pls1c_00_set( pl_w, 26 ) ;
	}
}

/*--------------------------------------------------------------*/
static void hyper_roll_glow( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_char_move( pl_w ) ;
	if( !pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		color_start( pl_w, col_15 ) ;
	}
}

/*--------------------------------------------------------------*/
static void hyper_roll_fire( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	color_move( pl_w ) ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		scr_excep_clr() ;
		pl_w->timer1 = 4 ;
		pl_w->routine_3 = 0 ;
		color_start( pl_w, col_04 ) ;
		pls1c_00_set( pl_w, 9 ) ;
	}
}

/*--------------------------------------------------------------*/
static void hyper_roll_burst( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	pl_w->dir = Sys->game_timer0 & 1 ;
	pl_char_move( pl_w ) ;
	if( !pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		color_set( pl_w, 3 ) ;
		color_set( pl_w, 4 ) ;
		color_set( pl_w, 5 ) ;
	}
	hyper_roll_eff_set( pl_w ) ;
	color_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void hyper_roll_main_init( PLAYER_WORK *pl_w )
{
	PL1D_LOCAL_W *lw = ( PL1D_LOCAL_W * )pl_w->tlocal ;

	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_w->timer1 = 18 ;
		pl_w->routine_3 = 0 ;
		lw->sw_shot = 0 ;
		pl_w->timer0 = 60 ;
		color_start( pl_w, col_05 ) ;
		pl_w->atck_no = pl1d_at_hyper_roll_1 ;
		HIT_RESET( pl_w ) ;
		shl1d_05_set( pl_w ) ;
		pl_se_req( pl_w, 72 ) ;
		pl_char_set( pl_w, Char_Super, 3 ) ;
	}
}

/*--------------------------------------------------------------*/
static void hyper_roll_main( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( --pl_w->timer0 == 0 ){
		pl_w->routine_2 ++ ;
		color_start( pl_w, col_05 ) ;
		pl_char_set( pl_w, Char_Super, 4 ) ;
		return ;
	}
	if( pl_w->timer0 & 1 ) pl_w->atck_no = pl1d_at_hyper_roll_2 ;
	else pl_w->atck_no = pl1d_at_hyper_roll_1 ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->cg_type = 0 ;
		shl1d_06_set( pl_w, 4 ) ;
		shl1d_06_set( pl_w, 3 ) ;
	}
	hyper_roll_hit_set( pl_w ) ;
	color_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void hyper_roll_finish( PLAYER_WORK *pl_w )
{
	pl_w->delay_timer = 2 ;
	pl_w->hyper_bg_tm = 5 ;
	if( pl_char_move( pl_w ) < 0 ){
		pl_w->routine_2 ++ ;
		pl_w->routine_3 = 0 ;
		pl_w->delay_flag = 0 ;
		pl_w->delay_timer = 0 ;
		pl_w->hyper_bg = 0 ;
		pl_w->hyper_bg_tm = 0 ;
	}
	color_move( pl_w ) ;
}

static void hyper_roll_end_init( PLAYER_WORK *pl_w );
static void hyper_roll_end_stand( PLAYER_WORK *pl_w );
static void hyper_roll_end_fall( PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void hyper_roll_end( PLAYER_WORK *pl_w )
{
	static void ( *hyper_roll_end_jmp[])( PLAYER_WORK *pl_w ) = {
		hyper_roll_end_init, hyper_roll_end_stand, hyper_roll_end_fall,
	} ;

	hyper_roll_end_jmp[pl_w->routine_3]( pl_w ) ;
}

/*==============================================================*/
static void hyper_roll_end_init( PLAYER_WORK *pl_w )
{
	PL1D_LOCAL_W *lw = ( PL1D_LOCAL_W * )pl_w->tlocal ;

	pl_w->routine_3 ++ ;
	lw->apart_flag = 0xff ;
	color_start( pl_w, col_06 ) ;
	if( pl_w->pos.y > pl_w->ground_y ){
		pl_w->routine_3 = 2 ;
		if( !pl_w->pl_dir ) pl_w->spd.x = 2.5f ;
		else pl_w->spd.x = -2.5f ;
		pl_w->acc.x = 0.0f ;
		pl_w->spd.y = DOT_Y( 2.0f ) ;
		pl_w->acc.y = -DOT_Y( 0.375f ) ;
		pl_char_set( pl_w, Char_Super, 6 ) ;
		hyper_roll_end_fall( pl_w ) ;
	}else{
		pl_char_set( pl_w, Char_Super, 5 ) ;
		hyper_roll_end_stand( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void hyper_roll_end_stand( PLAYER_WORK *pl_w )
{
	color_move( pl_w ) ;
	if( pl_char_move( pl_w ) < 0 ){
		PL1D_LOCAL_W *lw = ( PL1D_LOCAL_W * )pl_w->tlocal ;

		lw->join_sp = 0 ;
		pl_w->spd.x = 0.0f ;
		pl_w->spd.y = 0.0f ;
		pl_w->acc.x = 0.0f ;
		pl_w->acc.y = 0.0f ;
		pl_color_set( pl_w, PC_DEFAULT ) ;
		pl_stand_ret( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void hyper_roll_end_fall( PLAYER_WORK *pl_w )
{
	color_move( pl_w ) ;
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_w->pos.y <= pl_w->ground_y ){
		PL1D_LOCAL_W *lw = ( PL1D_LOCAL_W * )pl_w->tlocal ;

		pl_w->pos.y = pl_w->ground_y ;
		lw->join_sp = 0 ;
		pl_color_set( pl_w, PC_DEFAULT ) ;
		pl_jump_ret( pl_w ) ;
	}
}

/* sp_hyper_roll's effects, each with the time to the next, ending at -1 */
static Sint16 hyper_roll_eff_tbl[] = {
	35, 3, 36, 3, 43, 3, 44, 3, 45, 3, 46, 3, 32, 3, 38, 3,
	40, 3, 33, 3, 34, 3, 41, 3, 39, 3, 42, 3, 37, 3, 31, 3,
	35, 3, 36, 3, 43, 3, 44, 3, 45, 3, 46, 3, 32, 3, 38, 3,
	40, 3, 33, 3, 34, 3, 41, 3, 39, 3, 42, 3, 37, 3, 31, 3,
	35, 3, 36, 3, 43, 3, 44, 3, 45, 3, 46, 3, 32, 3, 38, 3,
	40, 3, 33, 3, 34, 3, 41, 3, 39, 3, 42, 3, 37, 3, 31, 3,
	35, 3, 36, 3, 43, 3, 44, 3, 45, 3, 46, 3, 32, 3, 38, 3,
	40, 3, 33, 3, 34, 3, 41, 3, 39, 3, 42, 3, 37, 3, 31, 3,
	-1,
} ;
/* sp_hyper_roll's hits, each with the time to the next, ending at -1 */
static Sint16 hyper_roll_hit_tbl[] = {
	0, 8, 1, 8, 2, 8, 0, 8, 1, 8, 2, 8, 0, 8, 1, 8, 2, 8,
	0, 8, 1, 8, 2, 8, 0, 8, 1, 8, 2, 8, 0, 8, 1, 8, 2, 8,
	0, 8, 1, 8, 2, 8, 0, 8, 1, 8, 2, 8, 0, 8, 1, 8, 2, 8,
	0, 8, 1, 8, 2, 8, 0, 8, 1, 8, 2, 8, 0, 8, 1, 8, 2, 8,
	0, 8, 1, 8, 2, 8, 0, 8, 1, 8, 2, 8, 0, 8, 1, 8, 2, 8,
	0, 8, 1, 8, 2, 8, 0, 8, 1, 8, 2, 8, 0, 8, 1, 8, 2, 8,
	-1,
} ;

/*==============================================================*/
static Sint16 hyper_roll_eff_set( PLAYER_WORK *pl_w )
{
	Sint16 no = 0 ;

	if( --pl_w->timer1 == 0 ){
		do{
			no = hyper_roll_eff_tbl[pl_w->routine_3 * 2] ;
			if( no == -1 ) break ;
			pls1c_00_set( pl_w, no ) ;
			pl_w->timer1 = hyper_roll_eff_tbl[pl_w->routine_3 * 2 + 1] ;
			pl_w->routine_3 ++ ;
		}while( pl_w->timer1 ) ;
	}
	return no ;
}

/*==============================================================*/
/*			Hits faster while they connect						*/
/*==============================================================*/
static Sint16 hyper_roll_hit_set( PLAYER_WORK *pl_w )
{
	PL1D_LOCAL_W *lw ;
	Sint16 no = 0 ;

	if( --pl_w->timer1 == 0 ) goto set ;
	if( !sw_renda_ck( pl_w ) ) goto end ;
	lw = ( PL1D_LOCAL_W * )pl_w->tlocal ;
	lw->sw_shot ++ ;
	if( lw->sw_shot < 1 ) goto end ;
	lw->sw_shot = 0 ;
	pl_w->timer1 -= 4 ;
	if( pl_w->timer1 > 0 ) goto end ;
set :
	do{
		no = hyper_roll_hit_tbl[pl_w->routine_3 * 2] ;
		if( no == -1 ) break ;
		shl1d_06_set( pl_w, no ) ;
		pl_w->timer1 = hyper_roll_hit_tbl[pl_w->routine_3 * 2 + 1] ;
		pl_w->routine_3 ++ ;
	}while( !pl_w->timer1 ) ;
end :
	return no ;
}

static void item_sel_init( PLAYER_WORK *pl_w );
static void item_sel_move( PLAYER_WORK *pl_w );
static void item_sel_end( PLAYER_WORK *pl_w );

/*==============================================================*/
/*			Changes the item									*/
/*==============================================================*/
static void sp_item_sel( PLAYER_WORK *pl_w )
{
	static void ( *item_sel_jmp[])( PLAYER_WORK *pl_w ) = {
		item_sel_init, item_sel_move, item_sel_end,
	} ;

	item_sel_jmp[pl_w->routine_3]( pl_w ) ;
}

/*==============================================================*/
static void item_sel_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_3 ++ ;
	pl_dir_set( pl_w ) ;
	pl_color_set( pl_w, 1 ) ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_char_set( pl_w, Char_Special, 12 ) ;
	pl_sp_ef_set( pl_w ) ;
	item_sel_move( pl_w ) ;
}

/*--------------------------------------------------------------*/
static void item_sel_move( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_3 ++ ;
		pl_w->cg_type = 0 ;
		pls1c_04_set( pl_w, PL1D_LW( pl_w )->item_sel ) ;
	}
}

/*--------------------------------------------------------------*/
static void item_sel_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void t_kick_init( PLAYER_WORK *pl_w );
static void t_kick_move( PLAYER_WORK *pl_w );
static void t_kick_land( PLAYER_WORK *pl_w );

/****************************************************************/
/*			A kick that leaps off the wall						*/
/****************************************************************/
void	pl1d_t_kick( PLAYER_WORK *pl_w )
{
	static void ( *t_kick_jmp[])( PLAYER_WORK *pl_w ) = {
		t_kick_init, t_kick_move, t_kick_land,
	} ;

	t_kick_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void t_kick_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 ++ ;
	pl_w->pl_stance = STANCE_AIR ;
	pl_color_set( pl_w, PC_DEFAULT ) ;
	if( !pl_w->disp_dir ) pl_w->spd.x = -30.0f ;
	else pl_w->spd.x = 30.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->spd.y = DOT_Y( 0.125f ) ;
	pl_w->acc.y = -DOT_Y( 0.375f ) ;
	pl_w->atck_no = pl1d_at_t_kick ;
	HIT_RESET( pl_w ) ;
	pl_char_set( pl_w, Char_Extra, 0 ) ;
}

/*--------------------------------------------------------------*/
static void t_kick_move( PLAYER_WORK *pl_w )
{
	pl_char_move( pl_w ) ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->spd.x += pl_w->acc.x ;
	pl_w->pos.y += pl_w->spd.y ;
	pl_w->spd.y += pl_w->acc.y ;
	if( pl_land_ck( pl_w ) ){
		pl_w->routine_2 ++ ;
		pl_char_set( pl_w, Char_Extra, 1 ) ;
		pl_land_ef_set( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void t_kick_land( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stop_stand_ret( pl_w ) ;
}

static PLAYER_WORK *nml_catch_ck( PLAYER_WORK *pl_w );
static PLAYER_WORK *crouch_catch_ck( PLAYER_WORK *pl_w );
static PLAYER_WORK *jump_catch_ck( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Can a throw start?									*/
/****************************************************************/
PLAYER_WORK *pl1d_catch_ck( PLAYER_WORK *pl_w )
{
	static PLAYER_WORK *( *catch_ck_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_catch_ck, crouch_catch_ck, jump_catch_ck, nml_catch_ck,
	} ;

	return catch_ck_jmp[pl_w->pl_stance]( pl_w ) ;
}

/*==============================================================*/
static PLAYER_WORK *nml_catch_ck( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	if( !( pl_w->dir = ( pl_w->atck_lever & ( KEY_FORWARD | KEY_BACK ) ) >> 10 ) ) return 0 ;
	if( pl_w->atck_pk == 0 && pl_w->atck_level == 1 ){
		if( ( em = pl_catch_ck( pl_w ) ) ){
			pl_w->catch_no = 0 ;
			return em ;
		}
		return 0 ;
	}
	return 0 ;
}

/*==============================================================*/
static PLAYER_WORK *crouch_catch_ck( PLAYER_WORK *pl_w )
{
	return 0 ;
}

/*==============================================================*/
static PLAYER_WORK *jump_catch_ck( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	if( !( pl_w->dir = ( pl_w->atck_lever & ( KEY_DOWN | KEY_FORWARD | KEY_BACK ) ) >> 10 ) ) return 0 ;
	if( pl_w->atck_pk == 0 && pl_w->atck_level == 1 && pl_w->pos.y > DOT_Y( 64.0f ) ){
		if( ( em = pl_catch_ck( pl_w ) ) ){
			pl_w->catch_no = 1 ;
			return em ;
		}
		return 0 ;
	}
	return 0 ;
}

static void nml_set( PLAYER_WORK *pl_w );
static void jump_set( PLAYER_WORK *pl_w );

/****************************************************************/
/*			Start the throw that pl1d_catch_ck chose			*/
/****************************************************************/
void	pl1d_catch_set( PLAYER_WORK *pl_w )
{
	static void ( *catch_set_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_set, jump_set,
	} ;

	catch_set_jmp[pl_w->catch_no & CATCH_NO_MASK]( pl_w ) ;
}

/*==============================================================*/
static void nml_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	if( pl_w->dir & 1 ){
		pl_w->pl_dir ^= 1 ;
		pl_w->disp_dir = pl_w->pl_dir ;
	}
	ofs.x = -DOT_X( 44.0f ) ;
	ofs.y = DOT_Y( 50.0f ) ;
	nage_hit_set( pl_w, &ofs ) ;
	pl_w->hit_stop = 10 ;
	catch_gauge_add( pl_w ) ;
	pl_char_set( pl_w, Char_Catch, 0 ) ;
}

/*==============================================================*/
/*			The speed is worked out in ofs.x before it is		*/
/*			used for the offset									*/
/*==============================================================*/
static void jump_set( PLAYER_WORK *pl_w )
{
	VEC3 ofs ;

	ofs.x = 0.0f ;
	if( pl_w->jump_dir >= 0 ){
		ofs.x = -DOT_X( 4.0f ) ;
		if( pl_w->pl_dir ) ofs.x = -ofs.x ;
		if( pl_w->jump_dir ) ofs.x = -ofs.x ;
	}
	pl_w->spd.x = ofs.x ;
	pl_w->acc.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.y = -DOT_Y( 0.25f ) ;
	if( pl_w->dir & 1 ){
		pl_w->pl_dir ^= 1 ;
		pl_w->disp_dir = pl_w->pl_dir ;
	}
	ofs.x = -DOT_X( 44.0f ) ;
	ofs.y = DOT_Y( 50.0f ) ;
	nage_hit_set( pl_w, &ofs ) ;
	pl_w->hit_stop = 10 ;
	pl_w->spd.x = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->spd.y /= 2.0f ;
	pl_w->acc.y = -DOT_Y( 0.375f ) ;
	catch_gauge_add( pl_w ) ;
	pl_char_set( pl_w, Char_Catch, 1 ) ;
}

static void nml_catch( PLAYER_WORK *pl_w );
static void jump_catch( PLAYER_WORK *pl_w );

/****************************************************************/
/*			A throw in progress									*/
/****************************************************************/
void	pl1d_catch( PLAYER_WORK *pl_w )
{
	static void ( *catch_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_catch, jump_catch,
	} ;

	pl_w->catch_flag = 1 ;
	catch_jmp[pl_w->catch_no & CATCH_NO_MASK]( pl_w ) ;
}

static void nml_catch_throw( PLAYER_WORK *pl_w );
static void nml_catch_end( PLAYER_WORK *pl_w );

/*==============================================================*/
static void nml_catch( PLAYER_WORK *pl_w )
{
	static void ( *nml_catch_jmp[])( PLAYER_WORK *pl_w ) = {
		nml_catch_throw, nml_catch_end,
	} ;

	nml_catch_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void nml_catch_throw( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	pl_char_move( pl_w ) ;
	if( pl_w->cg_type == 1 ){
		pl_w->routine_2 ++ ;
		pl_w->cg_type = 0 ;
		em = pl_w->catch_w ;
		em->dm_hit_w = pl_w ;
		em->pl_dir = pl_w->pl_dir ^ 1 ;
		em->atck_no = pl1d_at_throw ;
		em->catch_type = 1 ;
		scr_excep_clr() ;
		catch_se_req( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void nml_catch_end( PLAYER_WORK *pl_w )
{
	if( pl_char_move( pl_w ) < 0 ) pl_stand_ret( pl_w ) ;
}

static void jump_catch_throw( PLAYER_WORK *pl_w );
static void jump_catch_end( PLAYER_WORK *pl_w );

/*==============================================================*/
static void jump_catch( PLAYER_WORK *pl_w )
{
	static void ( *jump_catch_jmp[])( PLAYER_WORK *pl_w ) = {
		jump_catch_throw, jump_catch_end,
	} ;

	jump_catch_jmp[pl_w->routine_2]( pl_w ) ;
}

/*==============================================================*/
static void jump_catch_throw( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	air_move( pl_w ) ;
	pl_char_move( pl_w ) ;
	if( pl_w->cg_type ){
		pl_w->routine_2 ++ ;
		em = pl_w->catch_w ;
		em->dm_hit_w = pl_w ;
		em->catch_type = 1 ;
		em->pl_stance = STANCE_AIR ;
		em->atck_no = pl1d_at_air_throw ;
		em->pl_dir = pl_w->pl_dir ^ 1 ;
		if( !pl_w->cg_rival ) pl_jump_ret( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void jump_catch_end( PLAYER_WORK *pl_w )
{
	air_move( pl_w ) ;
	if( pl_char_move( pl_w ) < 0 ) pl_jump_ret( pl_w ) ;
}

/****************************************************************/
/*			Holding the opponent								*/
/****************************************************************/
void	pl1d_hold( PLAYER_WORK *pl_w )
{
	catch_pos_set( pl_w->catch_w, pl_w ) ;
}

/****************************************************************/
/*			The moves done with a partner: a team hyper			*/
/*			combo, a combined hyper and an assist				*/
/****************************************************************/
void	pl1d_sp_unison( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl1d_sp_hyper_roll ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl1d_sp_hyper_roll ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl1d_sp_hyper_roll ;
		break ;
	}
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void	pl1d_sp_gattai( PLAYER_WORK *pl_w )
{
	pl_w->routine_2 = pl_w->routine_3 = pl_w->routine_1 = PL_NORM ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl1d_sp_hyper_roll ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl1d_sp_hyper_roll ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl1d_sp_hyper_roll ;
		break ;
	}
	act_st_set( pl_w, N_SUPER ) ;
}

/****************************************************************/
void	pl1d_sp_assist( PLAYER_WORK *pl_w )
{
	PL1D_LOCAL_W *lw = ( PL1D_LOCAL_W * )pl_w->tlocal ;

	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl1d_sp_buster ;
		pl_w->pl_stance = STANCE_STAND ;
		lw->charge_lv = 2 ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl1d_sp_flower ;
		pl_w->atck_level = 1 ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl1d_sp_flower ;
		pl_w->atck_level = 1 ;
		break ;
	}
	act_st_set( pl_w, N_SPECIAL ) ;
}

/****************************************************************/
void	pl1d_sp_zero( PLAYER_WORK *pl_w )
{
	PL1D_LOCAL_W *lw = ( PL1D_LOCAL_W * )pl_w->tlocal ;

	pl_w->routine_1 = PL_NORM ;
	pl_w->routine_2 = pl_w->routine_3 = 0 ;
	switch( pl_w->pl_personal ){
	case 0:
		pl_w->pl_sp_type = pl1d_sp_buster ;
		pl_w->pl_stance = STANCE_STAND ;
		lw->charge_lv = 2 ;
		break ;
	case 1:
		pl_w->pl_sp_type = pl1d_sp_flower ;
		pl_w->atck_level = 1 ;
		break ;
	case 2:
		pl_w->pl_sp_type = pl1d_sp_buster ;
		pl_w->pl_stance = STANCE_STAND ;
		lw->charge_lv = 2 ;
		break ;
	}
	act_st_set( pl_w, N_SPECIAL ) ;
}

/*==============================================================*/
/*			Starts a colour cycle: steps with no time are		*/
/*			done at once										*/
/*==============================================================*/
static void color_start( PLAYER_WORK *pl_w, COLOR_STEP *step )
{
	PL1D_LOCAL_W *lw = ( PL1D_LOCAL_W * )pl_w->tlocal ;
	int col ;

	do {
		lw->col_timer = step->time ;
		col = step->col + pl_w->pl_color * 87 ;
		ef_color_set( pl_w, step->no, col, step->cnt ) ;
		if( step->next == 1 ) lw->col_last_flag = 0 ;
		else lw->col_last_flag = 1 ;
		step += step->next ;
	} while( lw->col_timer == 0 ) ;
	lw->col_step = step ;
}

/*==============================================================*/
/*			Runs the colour cycle: 0 once its last step is		*/
/*			done												*/
/*==============================================================*/
static int color_move( PLAYER_WORK *pl_w )
{
	PL1D_LOCAL_W *lw = ( PL1D_LOCAL_W * )pl_w->tlocal ;
	int ret = 1 ;

	if( !--lw->col_timer ){
		if( lw->col_last_flag ) ret = 0 ;
		color_start( pl_w, lw->col_step ) ;
	}
	return ret ;
}

/*==============================================================*/
/*			One colour of col_00								*/
/*==============================================================*/
static void color_set( PLAYER_WORK *pl_w, int no )
{
	COLOR_STEP *step = &col_00[no] ;
	int col = step->col + pl_w->pl_color * 87 ;

	ef_color_set( pl_w, step->no, col, step->cnt ) ;
}

/****************************************************************/
int pl1d_shot_end( PLAYER_WORK *pl_w )
{
	PL1D_LOCAL_W *lw = ( PL1D_LOCAL_W * )pl_w->tlocal ;

	if( lw->sp_time ){
		if( pl_w->pl_sp_type == pl1d_sp_beat ){
			lw->sp_time = 0 ;
			lw->join_sp = 0 ;
			lw->apart_flag = 0xff ;
			pl_w->fly_flag = 0 ;
		} else {
			if( ( lw->sp_time -= 8 ) <= 0 ) lw->sp_time = 1 ;
			return 1 ;
		}
	}
	return 0 ;
}

/****************************************************************/
int pl1d_item_end( PLAYER_WORK *pl_w )
{
	PL1D_LOCAL_W *lw = ( PL1D_LOCAL_W * )pl_w->tlocal ;

	if( lw->shield_flag ){
		lw->shield_flag = 0 ;
		lw->shake_flag ++ ;
		pl_w->hit_stop += 6 ;
		return 1 ;
	}
	return 0 ;
}

/****************************************************************/
/*			Clear the work, all but the item					*/
/****************************************************************/
void pl1d_tlocal_init( PLAYER_WORK *pl_w )
{
	PL1D_LOCAL_W *lw = ( PL1D_LOCAL_W * )pl_w->tlocal ;
	Uint8 keep = lw->item ;

	memset( pl_w->tlocal, 0, sizeof( pl_w->tlocal ) ) ;
	lw->item = keep ;
}

/*==============================================================*/
/*			Whether a partner on the player's side is			*/
/*			character no										*/
/*==============================================================*/
static int partner_ck( PLAYER_WORK *pl_w, Uint8 no )
{
	int i ;
	PLAYER_WORK *w ;

	for( i = 0 ; i <= 2 ; i ++ ){
		w = vs_sys_work.team_w[pl_w->pl_no][i] ;
		if( w != pl_w && w->pl_type == no ) return 1 ;
	}
	return 0 ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "s_pl1d.c" End of File	*/
