/* The select scene: each side joins, chooses its three characters and their assists, then the
   stage; its states run from SelectMainMove_yu. */
#include "player.h"
#include "select.h"

extern void disp_all( void );
extern void stage_decide( void );
extern void sel_join_make( void );
extern PLAYER_WORK *sel_back_set( void );
extern void sel_plate_set( void );
extern void sel_backdrop_make( void );
extern void sel_ring_make( void );
extern void sel_globe_set( void );
extern void ef05_set_all( void );
extern void sel_cell_make_all( void );
extern Sint8 card_side_get( void );

extern void point_add( int a );
extern void face_tex_set( int pl );
extern void face3_tex_set( int side );
extern void face2_tex_set( int pl );
extern void ef05_cur_make( Uint8 side, int a );
extern void sel_cursor_set( PLAYER_WORK *pl_w );
extern void ef05_card_set( Uint8 side, Uint8 no );
extern void sel_turn_set( PLAYER_WORK *tw, Uint8 side );
extern PLAYER_WORK *sel_name_set( PLAYER_WORK *pl_w, Uint8 flag );
extern void ef05_tag_set( PLAYER_WORK *pl_w );
extern void sel_cursor_wrap( Uint8 side );
extern Sint8 sel_cursor_char( Uint8 side );
extern void sel_door_set( PLAYER_WORK *pl_w );
extern void sel_door_speed_set( PLAYER_WORK *pl_w );
extern void ef05_cur_boss_make( Uint8 side );
extern void sel_open_make( void );
extern void scr_demo_set( Uint8 no );
extern void team_color_set( Sint8 side );
extern void card_flag_clr( void );
extern void obj_uv_begin( int *src, void *obj );
extern int obj_uv_next( void );
extern void obj_uv_get( Float *x, Float *y );
extern void obj_uv_put( Float *x, Float *y );
#ifdef TARGET_DC
extern void load_side_set( Uint8 side );	/* am_load.c: the sides whose players' files are read */
extern void pl_data_no_init( int side );
extern void fight_load_start( int no );
extern void load_stop2( void );
#endif

void sel_char_take( PLAYER_WORK *tw, Uint8 side );
Uint32 sel_char_ck( PLAYER_WORK *tw, Uint8 side );
static void sel_pl_mask_set( PLAYER_WORK *pl_w );
void sel_def_char_set( PLAYER_WORK *pl_w );
Sint8 sel_color_get( PLAYER_WORK *pl_w, PLAYER_WORK *tw, Sint16 sw );
void sel_obj_pos_add( PLAYER_WORK *wk, Sint16 no, Float x, Float y );
void sel_obj_pos_set( PLAYER_WORK *wk, Sint16 no, Float x, Float y );
void sel_mes_disp( void );
void sel_dummy_0( void );
void sel_dummy_1( PLAYER_WORK *w );
static void sel_pl_clr( void );
void sel_cpu_team_set( Sint8 side );

static void sel_mask_set( Sint8 side );
static void sel_init( void );
static void sel_in( void );
static void sel_in_init( void );
static void sel_in_wait( void );
static void sel_in_scr_move( void );
static void sel_02( void );
#ifdef TARGET_DC
static int sel_stage_move( PLAYER_WORK *pl_w );
static void sel_stage_init( void );
static void sel_stage_wait( void );
static void sel_stage_end( void );
static void sel_pick_move( void );
#endif
static void sel_pick( void );
static void sel_pl_move( PLAYER_WORK *pl_w );
static void sel_pl_join( PLAYER_WORK *pl_w );
static void sel_pl_face_set( PLAYER_WORK *pl_w );
static void sel_pl_face_wait( PLAYER_WORK *pl_w );
static void sel_pl_member_set( PLAYER_WORK *pl_w );
static void sel_pl_name_set( PLAYER_WORK *pl_w );
static void sel_pl_tag_set( PLAYER_WORK *pl_w );
static void sel_pl_pick( PLAYER_WORK *pl_w );
static void sel_pl_pick_init( PLAYER_WORK *pl_w );
static void sel_pl_pick_char( PLAYER_WORK *pl_w );
static void sel_pl_pick_type( PLAYER_WORK *pl_w );
static void sel_pl_pick_next( PLAYER_WORK *pl_w );
static void sel_pl_speed( PLAYER_WORK *pl_w );
static void sel_pl_stage( PLAYER_WORK *pl_w );
static void sel_pl_09( PLAYER_WORK *pl_w );
static void sel_pl_end( PLAYER_WORK *pl_w );
#ifdef TARGET_DC
static void sel_dummy_join( PLAYER_WORK *pl_w );
#endif
static void sel_card_move( PLAYER_WORK *pl_w );
static void sel_out( void );
static void sel_out_init( void );
static void sel_out_wait( void );
static void sel_out_scr_set( void );
static void sel_out_scr_move( void );
static void sel_out_fade( void );
static void sel_cpu_move( void );
static void sel_cpu_init( void );
static void sel_cpu_wait( void );
static void sel_cpu_face_set( void );
static void sel_cpu_face_wait( void );
static void sel_cpu_out_init( void );
static void sel_cpu_out_wait( void );
static void sel_cpu_open_set( void );
static void sel_cpu_open_wait( void );
static void sel_cpu_scr_move( void );
static void sel_cpu_fade( void );
static void sel_exit( void );
static void sel_main_init( void );
static void sel_main_pl_set( void );
static void sel_main_cpu_set( void );
static void sel_main_out_init( void );
static void sel_main_open_set( void );
static void sel_main_open_wait( void );
static void sel_main_scr_move( void );
static void sel_main_fade( void );
static void sel_main_end( void );

/* a move worked out over a time, each axis at a steady speed, speeding up or slowing down */
typedef struct MOVE_DATA {
	Sint32	time;		/* 000: the frames of the move */
	Sint32	sum;		/* 004: 1 + 2 + ... + time, for a steady acceleration */
	Float	len[3];		/* 008: the distance to go on x, y and z */
	Float	spd_x;		/* 014 */
	Float	acc_x;		/* 018 */
	Float	spd_y;		/* 01c */
	Float	acc_y;		/* 020 */
	Float	spd_z;		/* 024 */
	Float	acc_z;		/* 028 */
	Sint8	x_type;		/* 02c: each axis's kind of move: 0 steady, 1 speeding up, 2 slowing down */
	Sint8	y_type;		/* 02d */
	Sint8	z_type;		/* 02e */
} MOVE_DATA;

SEL_WORK sel_w;
PLAYER_WORK *sel_back_w;
Sint8 sel_speed_side;		/* a side, -1 for none */
Sint32 sel_end_bit;		/* a bit for each side that has chosen */
Uint16 sel_sw_on[2];		/* each player's buttons newly pressed */
Uint16 sel_sw_now[2];		/* and held */
Sint8 sel_char[2][3];	/* each side's members' characters */
Sint8 sel_col[2][3];	/* and colours */
Sint32 unk_0C2FB1D0;		/* nothing reads it */
PLAYER_WORK *sel_char_w;	/* the member whose character is being chosen */
Sint8 sel_char_old;		/* and its character before the cursor moved */
PLAYER_WORK *sel_type_w;	/* the member whose assist type is being chosen */
Sint8 sel_type;		/* and the type */

/* each side's default team, and their bits in the masks of characters */
Uint32 def_team_bit[2][3] = {
	{ 0x00080000, 0x00100000, 0x00200000 },
	{ 0x00800000, 0x00400000, 0x00000040 }
} ;
Sint32 def_team_tbl[2][3] = {
	{ 0x13, 0x14, 0x15 },
	{ 0x17, 0x16, 0x06 }
} ;

/****************************************************************/
void SelectMainMove_yu( void )
{
	static void ( *sel_jmp[7] )( void ) = {
		sel_init, sel_in, sel_02, sel_pick, sel_out, sel_cpu_move, sel_exit
	} ;
#ifdef TARGET_DC
	Sint8 i ;

	/* the training's dummy joining: the other side's buttons work both cursors */
	if( training_work.side != 0 && Sys->join_side ){
		i = ( Sys->join_side - 1 ) ^ 1 ;
		sel_sw_on[0] = sel_sw_on[1] = pl_work[i].side_sw.on ;
		sel_sw_now[0] = sel_sw_now[1] = pl_work[i].side_sw.now ;
	} else
#endif
	{
		sel_sw_on[0] = pl_work[0].side_sw.on ;
		sel_sw_on[1] = pl_work[1].side_sw.on ;
		sel_sw_now[0] = pl_work[0].side_sw.now ;
		sel_sw_now[1] = pl_work[1].side_sw.now ;
	}
	sel_jmp[Sys->routine_0]() ;
}

/*==============================================================*/
/*			A side's masks from both players' and the			*/
/*			system's											*/
/*==============================================================*/
static void sel_mask_set( Sint8 side )
{
	Uint32 on[2], off[2] ;
#ifdef TARGET_DC
	Uint32 full0, full1 ;	/* every character's bits */
#endif

	on[0] = pl_work[0].card_open[0] | pl_work[1].card_open[0] | Sys->char_open0 ;
	on[1] = pl_work[0].card_open[1] | pl_work[1].card_open[1] | Sys->char_open1 ;
	off[0] = pl_work[0].card_ex[0] | pl_work[1].card_ex[0] | Sys->char_open_all0 ;
	off[1] = pl_work[0].card_ex[1] | pl_work[1].card_ex[1] | Sys->char_open_all1 ;
	sel_w.char_ex[side][0] = 0 ;
	sel_w.char_ex[side][1] = 0 ;
#ifdef TARGET_DC
	/* a network game takes the characters both consoles agreed on */
	if( Sys->home_mode == HOME_NETWORK ){
		sel_w.char_open[side][0] = net_w.char_open[0] ;
		sel_w.char_open[side][0] &= net_w.open_and[0] ;
		sel_w.char_free[side][0] = sel_w.char_open[side][0] ;
		sel_w.char_open[side][1] = net_w.char_open[1] ;
		sel_w.char_open[side][1] &= net_w.open_and[1] ;
		sel_w.char_free[side][1] = sel_w.char_open[side][1] ;
		sel_w.char_ex[side][0] |= net_w.char_ex[0] ;
		sel_w.char_ex[side][1] |= net_w.char_ex[1] ;
		sel_w.same_flag = net_w.rule[14] ;
	} else {
		sel_w.char_open[side][0] = on[0] | 0x00ffff77 ;
		sel_w.char_free[side][0] = sel_w.char_open[side][0] ;
		sel_w.char_open[side][1] = on[1] | 0x02000004 ;
		sel_w.char_free[side][1] = sel_w.char_open[side][1] ;
		sel_w.char_ex[side][0] |= off[0] ;
		sel_w.char_ex[side][1] |= off[1] ;
	}
	/* every character open, and opened again: any side may take the same one */
	full0 = 0xf8ffffff ;
	full1 = 0x07ffffff ;
	if( sel_w.char_open[side][0] == full0 ){
		if( sel_w.char_open[side][1] == full1 ){
			if( sel_w.char_ex[side][0] == full0 ){
				if( sel_w.char_ex[side][1] == full1 ) sel_w.same_flag = 1 ;
			}
		}
	}
#else
	sel_w.char_open[side][0] = on[0] | 0xc0fcfaff ;
	sel_w.char_free[side][0] = sel_w.char_open[side][0] ;
	sel_w.char_open[side][1] = on[1] | 0x01803300 ;
	sel_w.char_free[side][1] = sel_w.char_open[side][1] ;
	sel_w.same_flag = 0 ;
	sel_w.char_ex[side][0] |= off[0] ;
	sel_w.char_ex[side][1] |= off[1] ;
#endif
}

/*==============================================================*/
/*			The scene's set-up									*/
/*==============================================================*/
static void sel_init( void )
{
	VEC3 v ;

#ifdef TARGET_DC
	/* the players' files are read first */
	if( !load_ck() ) return ;
	load_side_set( Sys->play_side ) ;
	if( !( Sys->play_side & 1 ) ) pl_data_no_init( 0 ) ;
	if( !( Sys->play_side & 2 ) ) pl_data_no_init( 1 ) ;
#endif
	Sys->routine_0 ++ ;
	Sys->routine_1 = 0 ;
	Sys->routine_2 = 0 ;
	Sys->routine_3 = 0 ;
	Sys->exit_flag = 0 ;
	work_init() ;
	Palette_Init() ;
	Texture_Init() ;
#ifdef TARGET_DC
	sel_w.stage_ex_cnt = vms_work.stage_ex_cnt ;
	if( Sys->home_mode == HOME_VERSUS ){
		sel_w.stage_cur = 0 ;
		stage_no = 17 ;
		tex_req_flag = 1 ;
		tex_req_no = 29 ;
		mode_cnt_up() ;
	}
#endif
	Sys->sel_done_flag = 1 ;
	sel_w.same_flag = 0 ;
#ifdef TARGET_DC
	/* the versus, and the training with the computer on both sides, have no speed to choose */
	if( Sys->home_mode == HOME_VERSUS || training_work.unk_04 == 2 ){
		sel_speed_side = -1 ;
		Sys->join_side = 0 ;
		goto speed_end ;
	}
#endif
	sel_speed_side = 0 ;
	if( Sys->play_side & 1 ) sel_speed_side = 2 ;
	if( Sys->play_side & 2 ) sel_speed_side = 1 ;
	if( !Sys->join_side ){
		if( !Sys->play_side ) sel_speed_side = -1 ;
	}
	else{
		sel_speed_side = Sys->join_side ;
		Sys->join_side = 0 ;
	}
#ifdef TARGET_DC
speed_end :
#endif
	Sys->entry_stop = 0 ;
	Sys->join_flag = 0 ;
	Sys->heart_step = 0 ;
	sel_w.unk_0c = 0 ;
	sel_w.cur_char[0] = sel_w.cur_char[1] = 0 ;
	sel_w.pick_no[0] = sel_w.pick_no[1] = 0 ;
	sel_w.heart_flag = 0 ;
	sel_w.sel_timer = 0xe10 ;
	sel_w.card_flag = 0 ;
	sel_w.card_side = sel_w.card_side_old = 0 ;
	vms_work.card_bit = vms_work.card_bit_keep = 0 ;
	vms_work.get_point[0] = vms_work.get_point[1] = 0 ;
	sel_w.end_flag[0] = 0 ;
	sel_w.end_flag[1] = 0 ;
	sel_end_bit = 0 ;
	if( Sys->play_side & 3 ){
		if( !( Sys->play_side & 1 ) ){
			sel_w.pick_flag[0] = 0 ;
			sel_mask_set( 0 ) ;
			sel_w.char_open[0][0] |= sel_w.char_open[1][0] ;
			sel_w.char_open[0][1] |= sel_w.char_open[1][1] ;
			sel_w.char_free[0][0] = sel_w.char_open[0][0] ;
			sel_w.char_free[0][1] = sel_w.char_open[0][1] ;
			sel_w.char_ex[0][0] |= sel_w.char_ex[1][0] ;
			sel_w.char_ex[0][1] |= sel_w.char_ex[1][1] ;
		}
		if( !( Sys->play_side & 2 ) ){
			sel_w.pick_flag[1] = 0 ;
			sel_mask_set( 1 ) ;
			sel_w.char_open[1][0] |= sel_w.char_open[0][0] ;
			sel_w.char_open[1][1] |= sel_w.char_open[0][1] ;
			sel_w.char_free[1][0] = sel_w.char_open[1][0] ;
			sel_w.char_free[1][1] = sel_w.char_open[1][1] ;
			sel_w.char_ex[1][0] |= sel_w.char_ex[0][0] ;
			sel_w.char_ex[1][1] |= sel_w.char_ex[0][1] ;
		}
	}
	else{
		sel_w.pick_flag[0] = sel_w.pick_flag[1] = 0 ;
		sel_mask_set( 0 ) ;
		sel_mask_set( 1 ) ;
	}
	sel_pl_clr() ;
	TexLoad( 5 ) ;
	fade_init() ;
	fog_init() ;
	bright_init() ;
	scr_work_init() ;
#ifdef TARGET_DC
	/* the versus starts with the camera already down on the stages */
	if( Sys->home_mode == HOME_VERSUS ){
		scr_work.scr_type = 1 ;
		scr_work.scr_routine[1] = 4 ;
		scr_work.eye[0].x = 0.0f ;
		scr_work.eye[0].y = 190.0f ;
		scr_work.eye[0].z = 812.3573f ;
		scr_work.target[0].x = 0.0f ;
		scr_work.target[0].y = 190.0f ;
		scr_work.target[0].z = 0.0f ;
		scr_work.shift_y = 0.0f ;
		Sys->timer0 = 0 ;
		scr_move() ;
	} else
#endif
	{
		scr_work.scr_type = 1 ;
		scr_work.scr_routine[1] = 1 ;
		scr_work.eye[0].x = 0.0f ;
		scr_work.eye[0].y = 1080.0f ;
		scr_work.eye[0].z = 0.1f ;
		scr_work.target[0].x = 0.0f ;
		scr_work.target[0].y = 95.0f ;
		scr_work.target[0].z = 0.0f ;
		scr_work.shift_y = 0.0f ;
		Sys->timer0 = 100 ;
	}
	sel_w.join_flag = 0 ;
	sel_w.cur_go_flag = 0 ;
	sel_join_make() ;
	sel_back_w = sel_back_set() ;
	sel_plate_set() ;
	sel_backdrop_make() ;
	sel_ring_make() ;
	light_init() ;
	v.x = 1.0f ;
	v.y = 1.0f ;
	v.z = 1.0f ;
	light_dir_set( &v ) ;
	SysTransSet( disp_all ) ;
#ifdef TARGET_DC
	fight_load_start( 0 ) ;
#endif
}

/*--------------------------------------------------------------*/
static void sel_in( void )
{
	static void ( *sel_in_jmp[3] )( void ) = { sel_in_init, sel_in_wait, sel_in_scr_move } ;

	sel_in_jmp[Sys->routine_1]() ;
	work_list_move( W_SET ) ;
	work_list_move( W_EF ) ;
	work_list_move( W_SHL ) ;
	SysTransSet( disp_all ) ;
}

/*==============================================================*/
static void sel_in_init( void )
{
	Sys->routine_1 ++ ;
	na_req( 56 ) ;
	if( Sys->new_side & 1 ) sel_w.cpu_side = 1 ;
	else sel_w.cpu_side = 0 ;
	if( ( Sys->new_side | Sys->play_side ) != 3 ){
		if( ( Sys->new_side | Sys->play_side ) == 1 ) sel_w.cpu_side = 1 ;
		else sel_w.cpu_side = 0 ;
		sel_cpu_team_set( sel_w.cpu_side ) ;
		sel_w.pick_flag[sel_w.cpu_side] = 0 ;
	}
#ifdef TARGET_DC
	/* nobody plays yet: the computer's team, or the two new sides', is read ahead */
	if( !Sys->play_side ){
		if( Sys->new_side != 3 ) fight_load_start( sel_w.cpu_side + 15 ) ;
		else fight_load_start( 24 ) ;
	}
#endif
	if( sel_w.pick_flag[0] != 7 ) pl_work[0].route_no = nlRand() % 4 ;
	if( sel_w.pick_flag[1] != 7 ) pl_work[1].route_no = nlRand() % 4 ;
	stage_decide() ;
}

/*--------------------------------------------------------------*/
static void sel_in_wait( void )
{
	Sys->timer0 -- ;
	if( Sys->timer0 <= 0 ){
		Sys->routine_1 ++ ;
		sel_globe_set() ;
	}
#ifdef TARGET_DC
	if( Sys->home_mode == HOME_VERSUS ) return ;
#endif
	scr_move() ;
}

/*==============================================================*/
/*			The camera comes down onto the faces				*/
/*==============================================================*/
static void sel_in_scr_move( void )
{
	switch( Sys->routine_2 ){
	case 0:
		Sys->routine_2 ++ ;
		sel_w.ready_flag = 0 ;
		scene_song_req( 1, 22 ) ;
		ef05_set_all() ;
#ifdef TARGET_DC
		if( scr_work.scr_routine[0] != 2 ){
			if( Sys->home_mode != HOME_VERSUS ) scr_move() ;
		}
#else
		if( scr_work.scr_routine[0] != 2 ) scr_move() ;
#endif
		break ;
	case 1:
		scr_move() ;
		if( scr_work.scr_routine[0] == 2 ) Sys->routine_2 ++ ;
		break ;
	case 2:
		if( sel_w.ready_flag == 1 ){
#ifdef TARGET_DC
			/* the versus chooses its stage first */
			if( Sys->home_mode != HOME_VERSUS ) Sys->routine_0 = 3 ;
			else Sys->routine_0 = 2 ;
#else
			Sys->routine_0 = 3 ;
#endif
			Sys->routine_1 = 0 ;
			Sys->routine_2 = 0 ;
			sel_w.ready_flag = 0 ;
			Sys->entry_stop = 0 ;
			sel_cell_make_all() ;
		}
		break ;
	}
}

#ifdef TARGET_DC
/*==============================================================*/
/*			The versus: both players choose the stage, and		*/
/*			their handicaps										*/
/*==============================================================*/
static void sel_02( void )
{
	static void ( *sel_02_jmp[3] )( void ) = { sel_stage_init, sel_stage_wait, sel_stage_end } ;

	sel_02_jmp[Sys->routine_1]() ;
	work_list_move( W_SET ) ;
	work_list_move( W_EF ) ;
	work_list_move( W_SHL ) ;
	SysTransSet( disp_all ) ;
}

/*--------------------------------------------------------------*/
/*	one player's stage cursor and handicap; nonzero once chosen	*/
static int sel_stage_move( PLAYER_WORK *pl_w )
{
	/* the stages by the cursor, with stage 8's other look opened or not; 0 is a random one */
	static const Uint8 stage_tbl_ex[18] = { 17, 0, 1, 2, 3, 4, 5, 6, 7, 11, 8, 9, 10, 12, 13, 14, 15, 16 } ;
	static const Uint8 stage_tbl[17] = { 17, 0, 1, 2, 3, 4, 5, 6, 7, 11, 9, 10, 12, 13, 14, 15, 16 } ;
	int old ;
	Sint8 max ;

	old = sel_w.stage_cur ;
	max = sel_w.stage_ex_cnt + Sys->arcade_clear + 10 ;
	if( sel_sw_on[pl_w->pl_index] & ( KEY_UP | KEY_DOWN ) ){
		na_req( 1 ) ;
		if( sel_sw_on[pl_w->pl_index] & KEY_UP ) sel_w.stage_cur = ( sel_w.stage_cur + max - 1 ) % max ;
		if( sel_sw_on[pl_w->pl_index] & KEY_DOWN ) sel_w.stage_cur = ( sel_w.stage_cur + max + 1 ) % max ;
	}
	if( sel_sw_on[pl_w->pl_index] & ( KEY_LEFT | KEY_RIGHT ) ){
		na_req( 4 ) ;
		if( sel_sw_on[pl_w->pl_index] & KEY_LEFT ) pl_w->handicap = ( pl_w->handicap + 5 - 1 ) % 5 ;
		if( sel_sw_on[pl_w->pl_index] & KEY_RIGHT ) pl_w->handicap = ( pl_w->handicap + 5 + 1 ) % 5 ;
	}
	if( sel_w.stage_cur != old ){
		tex_req_flag = 1 ;
		tex_req_no = 29 ;
	}
	if( Sys->arcade_clear ) stage_no = stage_tbl_ex[sel_w.stage_cur] ;
	else stage_no = stage_tbl[sel_w.stage_cur] ;
	if( sel_sw_on[pl_w->pl_index] & ( KEY_LP | KEY_HP | KEY_LK | KEY_HK ) ){
		na_req( 2 ) ;
		if( !sel_w.stage_cur ){
			stage_no = nlRand() % 8 ;
			tex_req_flag = 1 ;
			tex_req_no = 29 ;
		}
		return 1 ;
	}
	return 0 ;
}

/*--------------------------------------------------------------*/
static void sel_stage_init( void )
{
	if( sel_stage_move( &pl_work[0] ) || sel_stage_move( &pl_work[1] ) ){
		Sys->routine_1 ++ ;
		Sys->timer0 = 60 ;
	}
}

/*--------------------------------------------------------------*/
static void sel_stage_wait( void )
{
	Uint8 handicap ;

	Sys->timer0 -- ;
	if( Sys->timer0 <= 0 ){
		Sys->routine_1 ++ ;
		/* each side's members take its handicap */
		handicap = pl_work[0].handicap ;
		pl_work[2].handicap = handicap ;
		pl_work[4].handicap = handicap ;
		handicap = pl_work[1].handicap ;
		pl_work[3].handicap = handicap ;
		pl_work[5].handicap = handicap ;
	}
}

/*--------------------------------------------------------------*/
static void sel_stage_end( void )
{
	Sys->routine_0 = 3 ;
	Sys->routine_1 = 0 ;
	Sys->boss_flag = 0 ;
}

/*==============================================================*/
/*			The sides join in, or drop out, until both have		*/
/*			chosen												*/
/*==============================================================*/
static void sel_pick_move( void )
{
	Sint16 i ;

	if( Sys->home_mode == HOME_NETWORK ){
		sel_w.sel_timer -- ;
		if( sel_w.sel_timer <= 0 ) sel_w.sel_timer = 0 ;
	}
	sel_pl_move( &pl_work[0] ) ;
	sel_pl_move( &pl_work[1] ) ;
	if( sel_end_bit == ( Sys->play_side | Sys->new_side ) ){
		if( ( Sys->new_side | Sys->play_side ) == 3 ) Sys->routine_0 = 4 ;
		else Sys->routine_0 = 5 ;
		for( i = 0 ; i < 2 ; i ++ ){
			pl_work[i].routine_1 = 0 ;
			pl_work[i].routine_2 = 0 ;
			pl_work[i].routine_3 = 0 ;
		}
		Sys->join_flag = 0 ;
		Sys->heart_step = 0 ;
	}
	work_list_move( W_SET ) ;
	work_list_move( W_EF ) ;
	work_list_move( W_SHL ) ;
	SysTransSet( disp_all ) ;
}

/*--------------------------------------------------------------*/
static void sel_pick( void )
{
	sel_pick_move() ;
}
#else
/*--------------------------------------------------------------*/
static void sel_02( void )
{
	Sys->routine_0 = 3 ;
}

/*==============================================================*/
/*			The sides join in, or drop out, until both have		*/
/*			chosen												*/
/*==============================================================*/
static void sel_pick( void )
{
	Sint16 i ;
	int ng ;

	if( sel_w.sel_timer != 0 ){
		sel_w.sel_timer -- ;
		if( sel_w.sel_timer <= 0 ) sel_w.sel_timer = 0 ;
	}
	sel_w.card_side_old = sel_w.card_side ;
	if( ( sel_w.card_side = card_side_get() ) != 0 && Sys->vms_mode >= 1 ){
		if( ( sel_w.card_side & 1 ) && !( vms_work.card_done_bit & 1 ) && !sel_w.end_flag[0]
		 && ( Sys->new_side & 1 ) && pl_work[0].act_st == 0 && !( Sys->play_side & 1 ) ){
			if( pl_work[0].card_open[0] == 0 && !pl_work[0].jump_com_cnt
			 && !( sel_w.card_flag & 2 ) && !( sel_w.card_flag & 1 ) && !( sel_w.card_flag & 4 ) ){
				sel_w.card_flag |= 1 ;
				vms_work.card_step[0] = 0 ;
				vms_work.card_bit_keep = vms_work.card_bit ;
				vms_work.card_task_step[0][0] = 0 ;
			}
		}
		else if( ( sel_w.card_flag & 5 ) && !( sel_w.card_side & 1 ) && pl_work[0].act_st != 1 ){
			if( pl_work[0].jump_com_cnt == 0 && pl_work[0].act_st != 2 ){
				sel_w.card_flag &= 0xfa ;
				pl_work[0].act_st = 0 ;
			}
		}
		if( ( sel_w.card_side & 2 ) && !( vms_work.card_done_bit & 2 ) && !sel_w.end_flag[1]
		 && ( Sys->new_side & 2 ) && pl_work[1].act_st == 0 && !( Sys->play_side & 2 ) ){
			if( pl_work[1].card_open[0] == 0 && !pl_work[1].jump_com_cnt
			 && !( sel_w.card_flag & 1 ) && !( sel_w.card_flag & 2 ) && !( sel_w.card_flag & 8 ) ){
				sel_w.card_flag |= 2 ;
				vms_work.card_step[1] = 0 ;
				vms_work.card_bit_keep = vms_work.card_bit ;
				vms_work.card_task_step[1][0] = 0 ;
			}
		}
		else if( ( sel_w.card_flag & 10 ) && !( sel_w.card_side & 2 ) && pl_work[1].act_st != 1 ){
			if( pl_work[1].jump_com_cnt == 0 && pl_work[1].act_st != 2 ){
				pl_work[1].act_st = 0 ;
				sel_w.card_flag &= 0xf5 ;
			}
		}
	}
	else{
		if( !( pl_work[0].jump_com_cnt != 0 && pl_work[0].act_st == 3 ) && pl_work[0].act_st != 1 ){
			if( pl_work[0].jump_com_cnt == 0 && pl_work[0].act_st != 2 ){
				pl_work[0].act_st = 0 ;
				sel_w.card_flag &= 0xfa ;
			}
		}
		if( !( pl_work[1].jump_com_cnt != 0 && pl_work[1].act_st == 3 ) && pl_work[1].act_st != 1 ){
			if( pl_work[1].jump_com_cnt == 0 && pl_work[1].act_st != 2 ){
				pl_work[1].act_st = 0 ;
				sel_w.card_flag &= 0xf5 ;
			}
		}
	}
	ng = vms_work.card_done_bit ;
	if( ng & 1 ) ;
	if( ng & 2 ) ;
	sel_pl_move( &pl_work[0] ) ;
	sel_pl_move( &pl_work[1] ) ;
	if( sel_end_bit == ( Sys->play_side | Sys->new_side ) ){
		if( ( Sys->new_side | Sys->play_side ) == 3 ) Sys->routine_0 = 4 ;
		else Sys->routine_0 = 5 ;
		for( i = 0 ; i < 2 ; i ++ ){
			pl_work[i].routine_1 = 0 ;
			pl_work[i].routine_2 = 0 ;
			pl_work[i].routine_3 = 0 ;
		}
		Sys->join_flag = 0 ;
		Sys->heart_step = 0 ;
	}
	work_list_move( W_SET ) ;
	work_list_move( W_EF ) ;
	work_list_move( W_SHL ) ;
	SysTransSet( disp_all ) ;
}
#endif

/*==============================================================*/
/*			A side's step, or its memory card while one is		*/
/*			being read											*/
/*==============================================================*/
static void sel_pl_move( PLAYER_WORK *pl_w )
{
	static void ( *sel_pl_jmp[11] )( PLAYER_WORK *pl_w ) = {
		sel_pl_join, sel_pl_face_set, sel_pl_face_wait, sel_pl_member_set, sel_pl_name_set, sel_pl_tag_set,
		sel_pl_pick, sel_pl_speed, sel_pl_stage, sel_pl_09, sel_pl_end
	} ;

	if( sel_w.card_flag & pl_w->side_bit ) sel_card_move( pl_w ) ;
	else sel_pl_jmp[pl_w->routine_0]( pl_w ) ;
}

/*==============================================================*/
/*			Waiting for the side to join; its team starts as	*/
/*			the default one										*/
/*==============================================================*/
static void sel_pl_join( PLAYER_WORK *pl_w )
{
	Sint8 i ;
	Uint8 st ;

	if( !( ( Sys->new_side | Sys->play_side ) & pl_w->side_bit ) ) return ;
	pl_w->routine_0 = 1 ;
	pl_w->routine_1 = 0 ;
	pl_w->timer1 = 0 ;
	pl_w->timer0 = 1 ;
	if( sel_speed_side >= 0 && pl_w->side_bit == Sys->join_side ){
		st = pl_work[pl_w->pl_index ^ 1].routine_0 ;
		if( st < 7 || st >= 9 ) sel_speed_side = pl_w->side_bit ;
	}
	if( Sys->play_side & pl_w->side_bit ) return ;
	if( pl_w->side_bit == Sys->join_side ){
		sel_w.sel_timer = 0xe10 ;
		pl_w->route_no = nlRand() % 4 ;
		stage_decide() ;
#ifdef TARGET_DC
		/* the stage's files are read again for the new stage */
		load_stop2() ;
		fight_load_start( 17 ) ;
#endif
	}
#ifdef TARGET_DC
	/* a network game's default teams are the rules' */
	if( Sys->home_mode == HOME_NETWORK ){
		if( sel_w.same_flag ){
			for( i = 0 ; i < 3 ; i ++ ){
				pl_work[pl_w->pl_index + i * 2].load_type = net_w.def_team_tbl[pl_w->pl_index][0] ;
				pl_work[pl_w->pl_index + i * 2].pl_type = pl_work[pl_w->pl_index + i * 2].load_type ;
				pl_work[pl_w->pl_index + i * 2].pl_personal = 0 ;
				pl_work[i * 2 + pl_w->pl_index].team_order = i ;
			}
		}
		else{
			for( i = 0 ; i < 3 ; i ++ ){
				pl_work[pl_w->pl_index + i * 2].load_type = net_w.def_team_tbl[pl_w->pl_index][i] ;
				pl_work[pl_w->pl_index + i * 2].pl_type = pl_work[pl_w->pl_index + i * 2].load_type ;
				pl_work[pl_w->pl_index + i * 2].pl_personal = 0 ;
				pl_work[i * 2 + pl_w->pl_index].team_order = i ;
			}
		}
	}
	else
#endif
	if( sel_w.same_flag ){
		for( i = 0 ; i < 3 ; i ++ ){
			pl_work[pl_w->pl_index + i * 2].load_type = def_team_tbl[pl_w->pl_index][0] ;
			pl_work[pl_w->pl_index + i * 2].pl_type = pl_work[pl_w->pl_index + i * 2].load_type ;
			pl_work[pl_w->pl_index + i * 2].pl_personal = 0 ;
			pl_work[i * 2 + pl_w->pl_index].team_order = i ;
		}
	}
	else{
		for( i = 0 ; i < 3 ; i ++ ){
			pl_work[pl_w->pl_index + i * 2].load_type = def_team_tbl[pl_w->pl_index][i] ;
			pl_work[pl_w->pl_index + i * 2].pl_type = pl_work[pl_w->pl_index + i * 2].load_type ;
			pl_work[pl_w->pl_index + i * 2].pl_personal = 0 ;
			pl_work[i * 2 + pl_w->pl_index].team_order = i ;
		}
	}
}

/*==============================================================*/
/*			The side's faces come in							*/
/*==============================================================*/
static void sel_pl_face_set( PLAYER_WORK *pl_w )
{
	switch( pl_w->routine_1 ){
	case 0:
		pl_w->routine_1 ++ ;
		sel_w.work_step[pl_w->pl_index] = 0 ;
		ef05_cur_make( pl_w->pl_index, 0 ) ;
		face3_tex_set( pl_w->pl_index ) ;
		break ;
	case 1:
		pl_w->routine_1 ++ ;
		tex_req_flag = 1 ;
		tex_req_no |= pl_w->pl_index + 1 ;
	case 2:
		if( sel_w.work_step[pl_w->pl_index] == 0 ){
			pl_w->routine_0 = 2 ;
			pl_w->routine_1 = 0 ;
		}
		break ;
	}
}

/*--------------------------------------------------------------*/
static void sel_pl_face_wait( PLAYER_WORK *pl_w )
{
	if( sel_w.work_step[pl_w->pl_index] == 1 ) pl_w->routine_0 = 3 ;
}

/*==============================================================*/
/*			The next member to choose, or the whole team		*/
/*			when the side's card has it							*/
/*==============================================================*/
static void sel_pl_member_set( PLAYER_WORK *pl_w )
{
	Uint8 step, i ;

	pl_w->routine_0 = 4 ;
	pl_w->master_w = &pl_work[pl_w->timer1 * 2 + pl_w->pl_index] ;
	step = sel_w.pick_flag[pl_w->pl_index] & 7 ;
	if( step != 7 ){
		sel_def_char_set( pl_w ) ;
		sel_cursor_set( pl_w ) ;
		tex_req_flag = 1 ;
		tex_req_no |= pl_w->pl_index + 1 ;
		face_tex_set( pl_w->timer1 * 2 + pl_w->pl_index ) ;
		face2_tex_set( pl_w->timer1 * 2 + pl_w->pl_index ) ;
		sel_w.pick_no[pl_w->pl_index] = pl_w->timer1 + 1 ;
		ef05_card_set( pl_w->pl_index, pl_w->timer1 ) ;
		return ;
	}
	pl_w->timer1 = 2 ;
	pl_w->routine_0 = 9 ;
	for( i = 0 ; i < 3 ; i ++ ){
		pl_w->master_w = &pl_work[pl_w->pl_index + i * 2] ;
		sel_turn_set( &pl_work[pl_w->pl_index + i * 2], pl_w->pl_index ) ;
	}
}

/*--------------------------------------------------------------*/
static void sel_pl_name_set( PLAYER_WORK *pl_w )
{
	pl_w->routine_0 = 5 ;
	sel_name_set( pl_w, 0 ) ;
}

/*--------------------------------------------------------------*/
static void sel_pl_tag_set( PLAYER_WORK *pl_w )
{
	pl_w->routine_0 = 6 ;
	ef05_tag_set( pl_w ) ;
}

/*==============================================================*/
/*			Choosing a member: the character, then the			*/
/*			assist type											*/
/*==============================================================*/
static void sel_pl_pick( PLAYER_WORK *pl_w )
{
	static void ( *sel_pl_pick_jmp[4] )( PLAYER_WORK *pl_w ) = {
		sel_pl_pick_init, sel_pl_pick_char, sel_pl_pick_type, sel_pl_pick_next
	} ;

	pl_w->master_w = &pl_work[pl_w->timer1 * 2 + pl_w->pl_index] ;
	sel_pl_pick_jmp[pl_w->routine_1]( pl_w ) ;
}

/*==============================================================*/
static void sel_pl_pick_init( PLAYER_WORK *pl_w )
{
	pl_w->routine_1 = 1 ;
	sel_w.work_step[pl_w->pl_index] = 0 ;
	if( pl_w->timer1 == 0 ) na_req( pl_w->pl_index + 74 ) ;
}

/*==============================================================*/
/*			The cursor over the faces; a button chooses, and	*/
/*			the time running out chooses for the side			*/
/*==============================================================*/
static void sel_pl_pick_char( PLAYER_WORK *pl_w )
{
	sel_char_w = &pl_work[pl_w->timer1 * 2 + pl_w->pl_index] ;
	sel_char_old = sel_char_w->load_type ;
	if( sel_w.sel_timer == 0 ){
		sel_sw_on[pl_w->pl_index] = KEY_LP ;
		sel_char_w->pl_type = sel_char_w->load_type ;
		goto decide ;
	}
	if( sel_sw_on[pl_w->pl_index] & KEY_LEVER ){
		na_req( pl_w->pl_index * 2 + 64 ) ;
		while( 1 ){
			if( sel_sw_on[pl_w->pl_index] & KEY_LEFT ) sel_w.cur_col[pl_w->pl_index] -- ;
			if( sel_sw_on[pl_w->pl_index] & KEY_RIGHT ) sel_w.cur_col[pl_w->pl_index] ++ ;
			if( sel_sw_on[pl_w->pl_index] & KEY_UP ) sel_w.cur_row[pl_w->pl_index] -- ;
			if( sel_sw_on[pl_w->pl_index] & KEY_DOWN ) sel_w.cur_row[pl_w->pl_index] ++ ;
			sel_cursor_wrap( pl_w->pl_index ) ;
			if( ( sel_char_w->load_type = sel_cursor_char( pl_w->pl_index ) ) < 0 ) continue ;
			if( sel_char_ck( sel_char_w, pl_w->pl_index ) ) break ;
		}
		sel_char_w->pl_type = sel_char_w->load_type ;
	}
	if( sel_char_old != sel_char_w->load_type ){
		tex_req_flag = 1 ;
		tex_req_no |= pl_w->pl_index + 1 ;
		face_tex_set( pl_w->timer1 * 2 + pl_w->pl_index ) ;
		face2_tex_set( pl_w->timer1 * 2 + pl_w->pl_index ) ;
	}
#ifndef TARGET_DC
	if( 0 ) sel_w.sel_timer = 1 ;
#endif
	if( !( sel_sw_on[pl_w->pl_index] & KEY_BUTTON ) ) return ;
decide :
	na_req( pl_w->pl_index * 2 + 65 ) ;
	sel_turn_set( sel_char_w, pl_w->pl_index ) ;
	sel_w.cur_char[pl_w->pl_index] = 0 ;
	pl_w->routine_1 = 2 ;
	pl_w->routine_2 = 0 ;
	sel_door_set( pl_w ) ;
	sel_w.pick_flag[pl_w->pl_index] |= 1 << pl_w->timer1 ;
	sel_char_take( sel_char_w, pl_w->pl_index ) ;
	pl_w->timer0 ++ ;
#ifdef TARGET_DC
	/* nobody plays yet: the member's files are read ahead */
	if( !Sys->play_side ) fight_load_start( pl_w->timer1 * 2 + pl_w->pl_index + 18 ) ;
	pl_cnt_up( sel_char_w->load_type ) ;
#endif
	sel_char_w->load_color = sel_color_get( pl_w, sel_char_w, sel_sw_on[pl_w->pl_index] ) ;
}

/*==============================================================*/
/*			The member's assist type; when the time runs out	*/
/*			the side takes it as it is							*/
/*==============================================================*/
static void sel_pl_pick_type( PLAYER_WORK *pl_w )
{
	switch( pl_w->routine_2 ){
	case 0:
		if( sel_w.work_step[pl_w->pl_index] == 2 ){
			pl_w->routine_2 ++ ;
			na_req( pl_w->pl_index * 3 + 52 ) ;
		}
		break ;
	case 1:
		sel_type_w = &pl_work[pl_w->timer1 * 2 + pl_w->pl_index] ;
		sel_type = sel_type_w->pl_personal ;
		if( sel_w.sel_timer == 0 ){
			sel_sw_on[pl_w->pl_index] = KEY_LP ;
			goto decide ;
		}
		if( sel_sw_on[pl_w->pl_index] & KEY_UP ){
			na_req( pl_w->pl_index * 2 + 64 ) ;
			sel_type -- ;
		}
		if( sel_sw_on[pl_w->pl_index] & KEY_DOWN ){
			na_req( pl_w->pl_index * 2 + 64 ) ;
			sel_type ++ ;
		}
		sel_type = ( sel_type + 3 ) % 3 ;
		sel_type_w->pl_personal = sel_type ;
		if( !( sel_sw_on[pl_w->pl_index] & KEY_BUTTON ) ) break ;
	decide :
		na_req( pl_w->pl_index * 2 + 65 ) ;
		na_req( pl_w->pl_index + 53 ) ;
		sel_w.pick_flag[pl_w->pl_index] |= 16 << pl_w->timer1 ;
		sel_w.cur_go_flag |= 1 << pl_w->pl_index ;
		pl_w->routine_1 = 3 ;
		pl_w->routine_2 = 0 ;
		if( pl_w->timer1 >= 2 ) sel_w.end_flag[pl_w->pl_index] |= 0x80 ;
#ifdef TARGET_DC
		/* a whole team chosen: its files are read ahead, and the arcade's play is counted */
		if( sel_w.pick_flag[pl_w->pl_index] == 0x77 ) fight_load_start( pl_w->pl_index + 13 ) ;
		if( sel_w.pick_flag[0] == 0x77 ){
			if( sel_w.pick_flag[1] == 0x77 ) fight_load_start( 2 ) ;
		}
		if( Sys->home_mode == HOME_ARCADE ) mode_cnt_up() ;
#endif
		break ;
	}
}

/*==============================================================*/
/*			On to the next member, or to the speed once all		*/
/*			three are chosen									*/
/*==============================================================*/
static void sel_pl_pick_next( PLAYER_WORK *pl_w )
{
	if( sel_w.work_step[pl_w->pl_index] == 3 ){
		pl_w->routine_1 = 0 ;
		pl_w->routine_2 = 0 ;
		if( pl_w->timer1 ++ >= 2 ){
			pl_w->master_w = &pl_work[pl_w->timer1 * 2 + pl_w->pl_index] ;
			if( Sys->speed_fix ){
				pl_w->routine_0 = 10 ;
#ifdef TARGET_DC
				Sys->game_speed = Sys->speed_fix ;
				sel_w.speed_flag = 1 ;
				sel_dummy_join( pl_w ) ;
#else
				Sys->game_speed = ( Sys->speed_fix - 1 ) & 1 ;
				sel_w.speed_flag = 1 ;
#endif
			}
			else{
#ifdef TARGET_DC
				/* the training's dummy side never chooses the speed */
				if( training_work.side && Sys->join_side == pl_w->side_bit ){
					pl_w->routine_0 = 10 ;
					return ;
				}
#endif
				if( sel_speed_side < 0 ){
					sel_speed_side = pl_w->side_bit ;
					pl_w->routine_0 = 7 ;
					sel_w.speed_flag = 0 ;
					sel_door_speed_set( pl_w ) ;
				}
				else if( sel_speed_side == pl_w->side_bit ){
					pl_w->routine_0 = 7 ;
					sel_w.speed_flag = 0 ;
					sel_door_speed_set( pl_w ) ;
				}
				else pl_w->routine_0 = 10 ;
			}
		}
		else{
			pl_w->routine_0 = 3 ;
			pl_w->timer0 ++ ;
		}
	}
}

/*==============================================================*/
/*			The speed, normal or turbo							*/
/*==============================================================*/
static void sel_pl_speed( PLAYER_WORK *pl_w )
{
	static char *speed_str[2] = { "NORMAL 0", "TURBO  1" } ;

	switch( pl_w->routine_2 ){
	case 0:
		pl_w->routine_2 ++ ;
		sel_w.speed_flag = 0 ;
		Sys->game_speed = 1 ;
	case 1:
		if( sel_w.work_step[pl_w->pl_index] == 4 ) pl_w->routine_2 ++ ;
		break ;
	case 2:
		if( sel_w.sel_timer == 0 ){
			sel_sw_on[pl_w->pl_index] = KEY_LP ;
		}
		else{
			if( sel_sw_on[pl_w->pl_index] & KEY_UP ){
				Sys->game_speed -- ;
				na_req( pl_w->pl_index * 2 + 64 ) ;
			}
			if( sel_sw_on[pl_w->pl_index] & KEY_DOWN ){
				Sys->game_speed ++ ;
				na_req( pl_w->pl_index * 2 + 64 ) ;
			}
			Sys->game_speed &= 1 ;
			pl_w->sel_speed = Sys->game_speed ;
			if( !( sel_sw_on[pl_w->pl_index] & KEY_BUTTON ) ) break ;
		}
		pl_w->routine_2 ++ ;
		sel_w.speed_flag = 1 ;
		na_req( pl_w->pl_index * 2 + 65 ) ;
		break ;
	case 3:
		if( sel_w.work_step[pl_w->pl_index] == 5 ){
#ifdef TARGET_DC
			sel_dummy_join( pl_w ) ;
#endif
			pl_w->routine_0 = 10 ;
		}
		break ;
	}
}

/*--------------------------------------------------------------*/
/* a debug stage select; the empty if is the game's own */
static void sel_pl_stage( PLAYER_WORK *pl_w )
{
	Sint32 x ;

	if( 0 ) stage_no = 0 ;
	if( sel_sw_on[pl_w->pl_index] & KEY_LEFT ) stage_no -- ;
	if( sel_sw_on[pl_w->pl_index] & KEY_RIGHT ) stage_no ++ ;
#ifdef TARGET_DC
	stage_no = ( stage_no + 17 ) % 17 ;	/* with the stages' other looks */
#else
	stage_no = ( stage_no + 9 ) % 9 ;
#endif
	if( sel_sw_on[pl_w->pl_index] & KEY_BUTTON ) pl_w->routine_0 = 10 ;
	if( pl_w->pl_index == 1 ){ }
	if( pl_w->pl_index == 1 ) x = 22 ;
	else x = 4 ;
	STrPrint( x, 22, 0, "STAGE   : %02X", stage_no ) ;
}

/*--------------------------------------------------------------*/
static void sel_pl_09( PLAYER_WORK *pl_w )
{
	pl_w->routine_0 = 10 ;
}

/*==============================================================*/
/*			The side has chosen									*/
/*==============================================================*/
static void sel_pl_end( PLAYER_WORK *pl_w )
{
	sel_end_bit |= pl_w->side_bit ;
}

#ifdef TARGET_DC
/*--------------------------------------------------------------*/
/* the training: once a side has its team, the other joins as the dummy */
static void sel_dummy_join( PLAYER_WORK *pl_w )
{
	if( training_work.side != 0 && !Sys->join_side ){
		if( pl_w->pl_index ){
			Sys->join_side = 1 ;
			Sys->new_side |= 1 ;
		}
		else{
			Sys->join_side = 2 ;
			Sys->new_side |= 2 ;
		}
	}
}
#endif

/*==============================================================*/
/*			The side's memory card: its result, and the			*/
/*			characters and points it holds; the console reads	*/
/*			its cards elsewhere									*/
/*==============================================================*/
static void sel_card_move( PLAYER_WORK *pl_w )
{
#ifndef TARGET_DC
	Uint32 m[2] ;
	int ck ;

	ck = card_ck( pl_w->side_bit - 1 ) ;
	sel_w.card_flag |= 1 << ( pl_w->side_bit + 1 ) ;
	switch( ck ){
	case 2:
		pl_w->jump_com_cnt = 60 ;
		pl_w->act_st = 2 ;
		sel_w.card_flag |= 1 << ( pl_w->side_bit + 1 ) ;
		break ;
	case 5:
	case 4:
	case 3:
		pl_w->jump_com_cnt = 60 ;
		pl_w->act_st = 4 ;
		sel_w.card_flag &= ~pl_w->side_bit ;
		break ;
	case 6:
		if( pl_w->act_st != 3 ){
			pl_w->jump_com_cnt = 60 ;
			pl_w->act_st = 3 ;
			sel_w.card_flag &= ~pl_w->side_bit ;
		}
		break ;
	case 1:
		break ;
	case 0:
		if( pl_w->act_st == 3 ) break ;
		pl_w->jump_com_cnt = 60 ;
		pl_w->act_st = 1 ;
		break ;
	}
	if( ck != 2 ) return ;
	sel_w.card_flag &= ~pl_w->side_bit ;
	switch( pl_w->side_bit ){
	case 1:
		point_add( vms_work.add_point[0] ) ;
		vms_work.add_point[0] = 0 ;
		m[0] = ~sel_w.char_free[0][0] & sel_w.char_open[0][0] ;
		m[1] = ~sel_w.char_free[0][1] & sel_w.char_open[0][1] ;
		pl_w->card_open[0] = vms_work.card_open[0] ;
		pl_w->card_open[1] = vms_work.card_open[1] ;
		pl_w->card_ex[0] = vms_work.card_ex[0] ;
		pl_w->card_ex[1] = vms_work.card_ex[1] ;
		sel_w.char_open[0][0] |= vms_work.card_open[0] ;
		sel_w.char_open[0][1] |= vms_work.card_open[1] ;
		sel_w.char_free[0][0] |= ~m[0] & vms_work.card_open[0] ;
		sel_w.char_free[0][1] |= ~m[1] & vms_work.card_open[1] ;
		sel_w.char_ex[0][0] |= vms_work.card_ex[0] ;
		sel_w.char_ex[0][1] |= vms_work.card_ex[1] ;
		m[0] = ~sel_w.char_free[1][0] & sel_w.char_open[1][0] ;
		m[1] = ~sel_w.char_free[1][1] & sel_w.char_open[1][1] ;
		sel_w.char_open[1][0] |= sel_w.char_open[0][0] ;
		sel_w.char_open[1][1] |= sel_w.char_open[0][1] ;
		sel_w.char_free[1][0] |= ~m[0] & vms_work.card_open[0] ;
		sel_w.char_free[1][1] |= ~m[1] & vms_work.card_open[1] ;
		sel_w.char_ex[1][0] |= sel_w.char_ex[0][0] ;
		sel_w.char_ex[1][1] |= sel_w.char_ex[0][1] ;
		break ;
	case 2:
		point_add( vms_work.add_point[1] ) ;
		vms_work.add_point[1] = 0 ;
		m[0] = ~sel_w.char_free[1][0] & sel_w.char_open[1][0] ;
		m[1] = ~sel_w.char_free[1][1] & sel_w.char_open[1][1] ;
		pl_w->card_open[0] = vms_work.card_open[0] ;
		pl_w->card_open[1] = vms_work.card_open[1] ;
		pl_w->card_ex[0] = vms_work.card_ex[0] ;
		pl_w->card_ex[1] = vms_work.card_ex[1] ;
		sel_w.char_open[1][0] |= vms_work.card_open[0] ;
		sel_w.char_open[1][1] |= vms_work.card_open[1] ;
		sel_w.char_free[1][0] |= ~m[0] & vms_work.card_open[0] ;
		sel_w.char_free[1][1] |= ~m[1] & vms_work.card_open[1] ;
		sel_w.char_ex[1][0] |= vms_work.card_ex[0] ;
		sel_w.char_ex[1][1] |= vms_work.card_ex[1] ;
		m[0] = ~sel_w.char_free[0][0] & sel_w.char_open[0][0] ;
		m[1] = ~sel_w.char_free[0][1] & sel_w.char_open[0][1] ;
		sel_w.char_open[0][0] |= sel_w.char_open[1][0] ;
		sel_w.char_open[0][1] |= sel_w.char_open[1][1] ;
		sel_w.char_free[0][0] |= ~m[0] & vms_work.card_open[0] ;
		sel_w.char_free[0][1] |= ~m[1] & vms_work.card_open[1] ;
		sel_w.char_ex[0][0] |= sel_w.char_ex[1][0] ;
		sel_w.char_ex[0][1] |= sel_w.char_ex[1][1] ;
		break ;
	}
#endif
}

/*==============================================================*/
/*			Both sides have chosen: the camera's move to the	*/
/*			fight												*/
/*==============================================================*/
static void sel_out( void )
{
	static void ( *sel_out_jmp[5] )( void ) = { sel_out_init, sel_out_wait, sel_out_scr_set, sel_out_scr_move, sel_out_fade } ;

	sel_out_jmp[Sys->routine_1]() ;
	work_list_move( W_SET ) ;
	work_list_move( W_EF ) ;
	work_list_move( W_SHL ) ;
	SysTransSet( disp_all ) ;
}

/*==============================================================*/
static void sel_out_init( void )
{
	Sys->routine_1 ++ ;
	Sys->timer0 = 20 ;
}

/*--------------------------------------------------------------*/
static void sel_out_wait( void )
{
	Sys->timer0 -- ;
	if( Sys->timer0 <= 0 ){
		Sys->routine_1 ++ ;
		Sys->exit_flag = 1 ;
		Sys->timer0 = 50 ;
	}
}

/*--------------------------------------------------------------*/
static void sel_out_scr_set( void )
{
	Sys->timer0 -- ;
	if( Sys->timer0 <= 0 ){
		Sys->routine_1 ++ ;
		scr_demo_set( 2 ) ;
		na_req( 73 ) ;
	}
}

/*--------------------------------------------------------------*/
static void sel_out_scr_move( void )
{
	scr_move() ;
	if( scr_work.eye[0].z < 40.0f ) Sys->entry_stop = 1 ;
	if( scr_work.scr_routine[0] == 2 ){
		Sys->routine_1 ++ ;
		bright_init() ;
		fade_set( -1, 20, FADE_OUT ) ;
		Sys->timer0 = 20 ;
		Sys->entry_stop = 1 ;
	}
}

/*--------------------------------------------------------------*/
static void sel_out_fade( void )
{
	Sys->timer0 -- ;
	if( Sys->timer0 <= 0 ){
		Sys->routine_1 = 0 ;
		Sys->routine_0 = 6 ;
	}
}

/*==============================================================*/
/*			The other side's button during the one-player		*/
/*			select: it joins in									*/
/*==============================================================*/
static Sint8 sel_join_ck( Sint8 flag )
{
	if( sel_sw_on[sel_w.cpu_side ^ 1] & KEY_BUTTON ){
		fade_set( -1, 20, FADE_OUT ) ;
		Sys->timer0 = 20 ;
		Sys->entry_stop = 1 ;
		if( flag ) Sys->routine_1 = 7 ;
		else Sys->routine_1 = 9 ;
#ifdef TARGET_DC
		/* the computer's team, now the new player's, is read again */
		fight_load_start( sel_w.cpu_side + 13 ) ;
		fight_load_start( 2 ) ;
#endif
		return 1 ;
	}
	return 0 ;
}

/*==============================================================*/
/*			One side has chosen: the computer's team comes		*/
/*			in													*/
/*==============================================================*/
static void sel_cpu_move( void )
{
	static void ( *sel_cpu_move_jmp[10] )( void ) = {
		sel_cpu_init, sel_cpu_wait, sel_cpu_face_set, sel_cpu_face_wait, sel_cpu_out_init, sel_cpu_out_wait, sel_cpu_open_set, sel_cpu_open_wait, sel_cpu_scr_move,
		sel_cpu_fade
	} ;

	if( sel_w.heart_flag ) Sound_Heart() ;
	else{
		if( Sys->join_side ){
			sel_w.heart_flag = 1 ;
			Sys->play_side = Sys->join_side ^ 3 ;
			Sys->join_flag = 0 ;
			Sys->heart_step = 0 ;
		}
		else sel_cpu_move_jmp[Sys->routine_1]() ;
		work_list_move( W_SET ) ;
		work_list_move( W_EF ) ;
		work_list_move( W_SHL ) ;
	}
	SysTransSet( disp_all ) ;
}

/*==============================================================*/
static void sel_cpu_init( void )
{
	Sys->routine_1 ++ ;
	Sys->timer0 = 60 ;
	sel_w.pick_flag[sel_w.cpu_side] = 0x77 ;
	team_color_set( sel_w.cpu_side ) ;
	sel_w.work_step[sel_w.cpu_side] = 0 ;
}

/*--------------------------------------------------------------*/
static void sel_cpu_wait( void )
{
	if( sel_join_ck( 0 ) ) return ;
	Sys->timer0 -- ;
	if( Sys->timer0 <= 0 ){
		Sys->routine_1 ++ ;
		Sys->routine_2 = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void sel_cpu_face_set( void )
{
	switch( Sys->routine_2 ){
	case 0:
		Sys->routine_2 ++ ;
		tex_req_flag = 1 ;
		tex_req_no |= sel_w.cpu_side + 1 ;
		face_tex_set( sel_w.cpu_side ) ;
		face2_tex_set( sel_w.cpu_side ) ;
		if( Sys->stage_cnt == 7 ) ef05_cur_boss_make( sel_w.cpu_side ) ;
		else ef05_cur_make( sel_w.cpu_side, 1 ) ;
		break ;
	case 1:
		if( sel_w.work_step[sel_w.cpu_side] == 0 ){
			Sys->routine_1 ++ ;
			Sys->routine_2 = 0 ;
		}
		break ;
	}
}

/*--------------------------------------------------------------*/
static void sel_cpu_face_wait( void )
{
	if( sel_w.work_step[sel_w.cpu_side] == 1 ) Sys->routine_1 ++ ;
}

/*--------------------------------------------------------------*/
static void sel_cpu_out_init( void )
{
	if( sel_join_ck( 0 ) ) return ;
	Sys->routine_1 ++ ;
	Sys->timer0 = 15 ;
}

/*--------------------------------------------------------------*/
static void sel_cpu_out_wait( void )
{
	Sys->timer0 -- ;
	if( Sys->timer0 <= 0 ){
		Sys->routine_1 ++ ;
		Sys->exit_flag = 1 ;
		Sys->timer0 = 50 ;
	}
	else sel_join_ck( 0 ) ;
}

/*--------------------------------------------------------------*/
static void sel_cpu_open_set( void )
{
	if( sel_join_ck( 0 ) ) return ;
	Sys->timer0 -- ;
	if( Sys->timer0 <= 0 ){
		Sys->routine_1 ++ ;
		sel_w.ready_flag = 0 ;
		sel_open_make() ;
	}
}

/*--------------------------------------------------------------*/
static void sel_cpu_open_wait( void )
{
	if( !sel_join_ck( 0 ) ){
		if( sel_w.ready_flag == 2 ){
			Sys->routine_1 ++ ;
			scr_demo_set( 2 ) ;
			na_req( 73 ) ;
		}
	}
}

/*--------------------------------------------------------------*/
static void sel_cpu_scr_move( void )
{
	scr_move() ;
	if( scr_work.eye[0].z < 40.0f ) Sys->entry_stop = 1 ;
	if( scr_work.scr_routine[0] == 2 ){
		Sys->routine_1 ++ ;
		bright_init() ;
		fade_set( -1, 20, FADE_OUT ) ;
		Sys->timer0 = 20 ;
		Sys->entry_stop = 1 ;
#ifdef TARGET_DC
		fight_load_start( sel_w.cpu_side + 13 ) ;
		fight_load_start( 2 ) ;
#endif
	}
	else sel_join_ck( 0 ) ;
}

/*--------------------------------------------------------------*/
static void sel_cpu_fade( void )
{
	Sys->timer0 -- ;
	if( Sys->timer0 <= 0 ) Sys->routine_0 = 6 ;
}

/*==============================================================*/
/*			The select is over: the teams are set and the		*/
/*			game goes on to its fights							*/
/*==============================================================*/
static void sel_exit( void )
{
	Sys->scene_no ++ ;
	Sys->routine_0 = 0 ;
	Sys->routine_1 = 0 ;
	Sys->routine_2 = 0 ;
	Sys->routine_3 = 0 ;
	Sys->exit_flag = 0 ;
	Sys->timer0 = 0 ;
	if( ( Sys->new_side | Sys->play_side ) & 1 ) sel_pl_mask_set( &pl_work[0] ) ;
	if( ( Sys->new_side | Sys->play_side ) & 2 ) sel_pl_mask_set( &pl_work[1] ) ;
	Sys->play_side |= Sys->new_side ;
	Sys->new_side = 0 ;
	Sys->join_flag = 0 ;
	Sys->heart_step = 0 ;
	Sys->join_side = 0 ;
	sel_w.heart_flag = 0 ;
	sel_pl_clr() ;
	Sys->sel_done_flag = 1 ;
#ifndef TARGET_DC
	card_flag_clr() ;
#endif
	sound_all_off() ;
	stream_stop() ;
}

/****************************************************************/
/*			The select for a side joining the game between		*/
/*			fights: the computer's team comes in again			*/
/****************************************************************/
void sel_main( void )
{
	static void ( *sel_main_jmp[9] )( void ) = {
		sel_main_init, sel_main_pl_set, sel_main_cpu_set, sel_main_out_init, sel_main_open_set, sel_main_open_wait, sel_main_scr_move,
		sel_main_fade, sel_main_end
	} ;

	sel_sw_on[0] = pl_work[0].side_sw.on ;
	sel_sw_on[1] = pl_work[1].side_sw.on ;
	if( sel_w.heart_flag ) Sound_Heart() ;
	else{
		if( Sys->join_side ){
			sel_w.heart_flag = 1 ;
			Sys->play_side = Sys->join_side ^ 3 ;
			Sys->join_flag = 0 ;
			Sys->heart_step = 0 ;
		}
		else sel_main_jmp[Sys->routine_1]() ;
		work_list_move( W_SET ) ;
		work_list_move( W_EF ) ;
		work_list_move( W_SHL ) ;
	}
	SysTransSet( disp_all ) ;
}

/*==============================================================*/
/*			The select for a joining side: its set-up			*/
/*==============================================================*/
static void sel_main_init( void )
{
	VEC3 v ;

#ifdef TARGET_DC
	if( !load_ck() ) return ;
	load_side_set( Sys->play_side ) ;
#endif
	Sys->routine_1 ++ ;
	Sys->routine_2 = 0 ;
	Sys->routine_3 = 0 ;
	Sys->exit_flag = 0 ;
	Sys->join_side = 0 ;
	Sys->entry_stop = 0 ;
	Sys->join_flag = 0 ;
	Sys->heart_step = 0 ;
	Sys->sel_done_flag = 0 ;
	work_init() ;
	Palette_Init() ;
	Texture_Init() ;
	sel_w.unk_0c = 0 ;
	sel_w.cur_char[0] = sel_w.cur_char[1] = 0 ;
	sel_w.pick_no[0] = sel_w.pick_no[1] = 0 ;
	sel_w.heart_flag = 0 ;
	sel_pl_clr() ;
	TexLoad( 5 ) ;
	scene_song_req( 1, 22 ) ;
	sel_w.join_flag = 3 ;
	sel_back_w = sel_back_set() ;
	sel_plate_set() ;
	sel_backdrop_make() ;
	fade_init() ;
	fog_init() ;
	bright_init() ;
	light_init() ;
	v.x = 1.0f ;
	v.y = 1.0f ;
	v.z = 1.0f ;
	light_dir_set( &v ) ;
	scr_work_init() ;
	scr_work.scr_type = 1 ;
	scr_work.scr_routine[1] = 4 ;
	scr_work.eye[0].x = 0.0f ;
	scr_work.eye[0].y = 190.0f ;
	scr_work.eye[0].z = 812.3573f ;
	scr_work.target[0].x = 0.0f ;
	scr_work.target[0].y = 190.0f ;
	scr_work.target[0].z = 0.0f ;
	scr_work.shift_y = 0.0f ;
	scr_move() ;
#ifdef TARGET_DC
	fight_load_start( 0 ) ;
#endif
}

/*==============================================================*/
/*			The side in the game shows its team, and the		*/
/*			joining side's is set up							*/
/*==============================================================*/
static void sel_main_pl_set( void )
{
	PLAYER_WORK *pl_w = &pl_work[Sys->play_side - 1] ;

	switch( pl_w->routine_1 ){
	case 0:
		pl_w->routine_1 ++ ;
		sel_w.work_step[pl_w->pl_index] = 0 ;
		sel_w.cpu_side = pl_w->pl_index ^ 1 ;
		sel_cpu_team_set( sel_w.cpu_side ) ;
		sel_w.pick_flag[sel_w.cpu_side] = 0 ;
		stage_decide() ;
		ef05_cur_make( pl_w->pl_index, 0 ) ;
		tex_req_flag = 1 ;
		tex_req_no |= pl_w->pl_index + 1 ;
		face_tex_set( pl_w->pl_index ) ;
		face2_tex_set( pl_w->pl_index ) ;
		break ;
	case 1:
		if( sel_w.work_step[pl_w->pl_index] == 0 ) pl_w->routine_1 ++ ;
		break ;
	case 2:
		if( sel_w.work_step[pl_w->pl_index] == 1 ){
			pl_w->routine_1 ++ ;
			pl_w->timer0 = 20 ;
		}
		break ;
	case 3:
		pl_w->timer0 -- ;
		if( pl_w->timer0 <= 0 ){
			Sys->routine_1 ++ ;
			pl_w->routine_1 = 0 ;
			sel_w.pick_flag[sel_w.cpu_side] = 0x77 ;
			team_color_set( sel_w.cpu_side ) ;
			sel_w.work_step[sel_w.cpu_side] = 0 ;
			tex_req_flag = 1 ;
			tex_req_no |= sel_w.cpu_side + 1 ;
			face_tex_set( sel_w.cpu_side ) ;
			face2_tex_set( sel_w.cpu_side ) ;
		}
		break ;
	}
}

/*==============================================================*/
/*			The joining side chooses its team					*/
/*==============================================================*/
static void sel_main_cpu_set( void )
{
	switch( Sys->routine_2 ){
	case 0:
		Sys->routine_2 ++ ;
		if( sel_join_ck( 1 ) ){
			Sys->routine_2 = 0 ;
			break ;
		}
		if( Sys->stage_cnt == 7 ) ef05_cur_boss_make( sel_w.cpu_side ) ;
		else ef05_cur_make( sel_w.cpu_side, 1 ) ;
		break ;
	case 1:
		if( !sel_join_ck( 1 ) ){
			if( sel_w.work_step[sel_w.cpu_side] == 0 ){
				Sys->routine_1 ++ ;
				Sys->routine_2 = 0 ;
			}
		}
		break ;
	}
}

/*--------------------------------------------------------------*/
static void sel_main_out_init( void )
{
	if( sel_join_ck( 1 ) ) return ;
	Sys->routine_1 ++ ;
	Sys->timer0 = 15 ;
}

/*--------------------------------------------------------------*/
static void sel_main_open_set( void )
{
	if( sel_join_ck( 1 ) ) return ;
	Sys->routine_1 ++ ;
	sel_w.ready_flag = 0 ;
	sel_open_make() ;
}

/*--------------------------------------------------------------*/
static void sel_main_open_wait( void )
{
	if( !sel_join_ck( 1 ) ){
		if( sel_w.ready_flag == 2 ){
			Sys->routine_1 ++ ;
			scr_demo_set( 2 ) ;
			na_req( 73 ) ;
		}
	}
}

/*--------------------------------------------------------------*/
static void sel_main_scr_move( void )
{
	scr_move() ;
	if( scr_work.scr_routine[0] == 2 ){
		Sys->routine_1 ++ ;
		bright_init() ;
		fade_set( -1, 20, FADE_OUT ) ;
		Sys->timer0 = 20 ;
		Sys->entry_stop = 1 ;
#ifdef TARGET_DC
		fight_load_start( sel_w.cpu_side + 13 ) ;
		fight_load_start( 2 ) ;
#endif
	}
	else sel_join_ck( 1 ) ;
}

/*--------------------------------------------------------------*/
static void sel_main_fade( void )
{
	Sys->timer0 -- ;
	if( Sys->timer0 <= 0 ) Sys->routine_1 ++ ;
}

/*--------------------------------------------------------------*/
static void sel_main_end( void )
{
	Sys->routine_0 ++ ;
	Sys->routine_1 = 0 ;
	Sys->routine_2 = 0 ;
	Sys->routine_3 = 0 ;
	Sys->exit_flag = 0 ;
	Sys->timer0 = 0 ;
	Sys->join_flag = 0 ;
	Sys->heart_step = 0 ;
	Sys->join_side = 0 ;
	sel_w.heart_flag = 0 ;
	Sys->sel_done_flag = 0 ;
	sel_pl_clr() ;
	sound_all_off() ;
	stream_stop() ;
}

/*==============================================================*/
/*			A move's speed and acceleration on one axis, for	*/
/*			each kind of move									*/
/*==============================================================*/
static void move_x_steady( MOVE_DATA *md )
{
	md->spd_x = md->len[0] / md->time ;
	md->acc_x = 0.0f ;
}

/*==============================================================*/
static void move_y_steady( MOVE_DATA *md )
{
	md->spd_y = md->len[1] / md->time ;
	md->acc_y = 0.0f ;
}

/*==============================================================*/
static void move_z_steady( MOVE_DATA *md )
{
	md->spd_z = md->len[2] / md->time ;
	md->acc_z = 0.0f ;
}

/*==============================================================*/
static void move_x_faster( MOVE_DATA *md )
{
	md->spd_x = md->acc_x = md->len[0] / md->sum ;
}

/*==============================================================*/
static void move_y_faster( MOVE_DATA *md )
{
	md->spd_y = md->acc_y = md->len[1] / md->sum ;
}

/*==============================================================*/
static void move_z_faster( MOVE_DATA *md )
{
	md->spd_z = md->acc_z = md->len[2] / md->sum ;
}

/*==============================================================*/
static void move_x_slower( MOVE_DATA *md )
{
	md->acc_x = md->len[0] / md->sum ;
	md->spd_x = md->acc_x * md->time ;
	md->acc_x = -md->acc_x ;
}

/*==============================================================*/
static void move_y_slower( MOVE_DATA *md )
{
	md->acc_y = md->len[1] / md->sum ;
	md->spd_y = md->acc_y * md->time ;
	md->acc_y = -md->acc_y ;
}

/*==============================================================*/
static void move_z_slower( MOVE_DATA *md )
{
	md->acc_z = md->len[2] / md->sum ;
	md->spd_z = md->acc_z * md->time ;
	md->acc_z = -md->acc_z ;
}

/*==============================================================*/
static void move_x_set( MOVE_DATA *md )
{
	switch( md->x_type ){
	case 1:
		move_x_faster( md ) ;
		break ;
	case 2:
		move_x_slower( md ) ;
		break ;
	default:
		move_x_steady( md ) ;
		break ;
	}
}

/*==============================================================*/
static void move_y_set( MOVE_DATA *md )
{
	switch( md->y_type ){
	case 1:
		move_y_faster( md ) ;
		break ;
	case 2:
		move_y_slower( md ) ;
		break ;
	default:
		move_y_steady( md ) ;
		break ;
	}
}

/*==============================================================*/
static void move_z_set( MOVE_DATA *md )
{
	switch( md->z_type ){
	case 1:
		move_z_faster( md ) ;
		break ;
	case 2:
		move_z_slower( md ) ;
		break ;
	default:
		move_z_steady( md ) ;
		break ;
	}
}

/*==============================================================*/
/*			The speed that covers the distance with the			*/
/*			acceleration already set							*/
/*==============================================================*/
static void move_x_acc( MOVE_DATA *md )
{
	md->spd_x = ( md->len[0] - md->sum * md->acc_x ) / md->time + md->acc_x ;
}

/*==============================================================*/
static void move_y_acc( MOVE_DATA *md )
{
	md->spd_y = ( md->len[1] - md->sum * md->acc_y ) / md->time + md->acc_y ;
}

/*==============================================================*/
/*			The acceleration that covers the distance from		*/
/*			the speed already set								*/
/*==============================================================*/
static void move_x_spd( MOVE_DATA *md )
{
	if( md->spd_x ){
		md->acc_x = ( md->len[0] - md->time * md->spd_x ) / md->sum ;
		md->spd_x += md->acc_x ;
	}
	else move_x_set( md ) ;
}

/*==============================================================*/
static void move_y_spd( MOVE_DATA *md )
{
	if( md->spd_y ){
		md->acc_y = ( md->len[1] - md->time * md->spd_y ) / md->sum ;
		md->spd_y += md->acc_y ;
	}
	else move_y_set( md ) ;
}

/****************************************************************/
/*			A work's move to x, y in a time, each axis			*/
/*			steady, speeding up or slowing down					*/
/****************************************************************/
void move_set_xy( PLAYER_WORK *wk, Sint32 time, Float x, Float y, Sint8 x_type, Sint8 y_type )
{
	MOVE_DATA md ;

	md.time = time ;
	md.sum = md.time * ( md.time - 1 ) / 2 + md.time ;
	md.len[0] = x - wk->pos.x ;
	md.len[1] = y - wk->pos.y ;
	md.x_type = x_type ;
	md.y_type = y_type ;
	if( md.time == 0 ) md.spd_x = md.acc_x = md.spd_y = md.acc_y = 0.0f ;
	else{
		move_x_set( &md ) ;
		move_y_set( &md ) ;
	}
	wk->spd.x = md.spd_x ;
	wk->acc.x = md.acc_x ;
	wk->spd.y = md.spd_y ;
	wk->acc.y = md.acc_y ;
}

/****************************************************************/
void move_set_x( PLAYER_WORK *wk, Sint32 time, Float x, Sint8 x_type )
{
	MOVE_DATA md ;

	md.time = time ;
	md.sum = md.time * ( md.time - 1 ) / 2 + md.time ;
	md.len[0] = x - wk->pos.x ;
	md.x_type = x_type ;
	if( md.time == 0 ) md.spd_x = md.acc_x = 0.0f ;
	else move_x_set( &md ) ;
	wk->spd.x = md.spd_x ;
	wk->acc.x = md.acc_x ;
}

/****************************************************************/
void move_set_z( PLAYER_WORK *wk, Sint32 time, Float z, Sint8 z_type )
{
	MOVE_DATA md ;

	md.time = time ;
	md.sum = md.time * ( md.time - 1 ) / 2 + md.time ;
	md.len[2] = z - wk->pos.z ;
	md.z_type = z_type ;
	if( md.time == 0 ) md.spd_z = md.acc_z = 0.0f ;
	else move_z_set( &md ) ;
	wk->spd.z = md.spd_z ;
	wk->acc.z = md.acc_z ;
}

/****************************************************************/
/*			A move to x, y keeping the work's acceleration		*/
/****************************************************************/
void move_set_xy_acc( PLAYER_WORK *wk, Float x, Float y, Sint32 time )
{
	MOVE_DATA md ;

	md.time = time ;
	md.sum = md.time * ( md.time - 1 ) / 2 + md.time ;
	md.len[0] = x - wk->pos.x ;
	md.len[1] = y - wk->pos.y ;
	md.acc_x = wk->acc.x ;
	md.acc_y = wk->acc.y ;
	if( md.time == 0 ) md.spd_x = md.spd_y = 0.0f ;
	else{
		move_x_acc( &md ) ;
		move_y_acc( &md ) ;
	}
	wk->spd.x = md.spd_x ;
	wk->spd.y = md.spd_y ;
}

/****************************************************************/
void move_set_y_acc( PLAYER_WORK *wk, Sint32 time, Uint8 y )
{
	MOVE_DATA md ;

	md.time = time ;
	md.sum = md.time * ( md.time - 1 ) / 2 + md.time ;
	md.len[1] = y - wk->pos.y ;
	md.acc_y = wk->acc.y ;
	if( md.time ) move_y_acc( &md ) ;
	wk->spd.y = md.spd_y ;
}

/****************************************************************/
/*			A move to x, y from the work's speed; the y			*/
/*			acceleration is stored in its y speed				*/
/****************************************************************/
void move_set_xy_spd( PLAYER_WORK *wk, Float x, Float y, Sint32 time, Sint8 x_type, Sint8 y_type )
{
	MOVE_DATA md ;

	md.time = time ;
	md.sum = md.time * ( md.time - 1 ) / 2 + md.time ;
	md.len[0] = x - wk->pos.x ;
	md.len[1] = y - wk->pos.y ;
	md.x_type = x_type ;
	md.y_type = y_type ;
	md.spd_x = wk->spd.x ;
	md.spd_y = wk->spd.y ;
	if( md.time == 0 ) md.acc_x = md.acc_y = 0.0f ;
	else{
		move_x_spd( &md ) ;
		move_y_spd( &md ) ;
	}
	wk->spd.x = md.spd_x ;
	wk->acc.x = md.acc_x ;
	wk->spd.y = md.spd_y ;
	wk->spd.y = md.acc_y ;
}

/****************************************************************/
/*			The y speed that stops the work's fall in a time	*/
/****************************************************************/
void move_stop_y( PLAYER_WORK *wk, Sint32 time )
{
	MOVE_DATA md ;

	md.time = time ;
	md.sum = md.time * ( md.time - 1 ) / 2 + md.time ;
	md.len[1] = 0.0f ;
	md.acc_y = wk->acc.y ;
	if( md.time ) move_y_acc( &md ) ;
	else md.spd_y = 0.0f ;
	wk->spd.y = md.spd_y ;
}

/****************************************************************/
void move_z_add( PLAYER_WORK *wk )
{
	wk->pos.z += wk->spd.z ;
	wk->spd.z += wk->acc.z ;
}

/*==============================================================*/
/*			A character's bit in a mask of two words: the		*/
/*			word is returned									*/
/*==============================================================*/
static Uint8 char_bit_get( Uint32 *bit, Sint8 no )
{
	Sint8 hi ;

	hi = no >= 32 ? 1 : 0 ;
	*bit = 1 << ( no - hi * 32 ) ;
	return hi ;
}

/****************************************************************/
/*			The member's character can't be taken again by		*/
/*			its side											*/
/****************************************************************/
void sel_char_take( PLAYER_WORK *tw, Uint8 side )
{
	Uint32 bit ;
	Uint8 hi ;

	if( sel_w.same_flag == 0 ){
		hi = char_bit_get( &bit, tw->load_type ) ;
		if( sel_w.char_free[side][hi] & bit ) sel_w.char_free[side][hi] ^= bit ;
	}
}

/****************************************************************/
/*			The member's character can still be taken by its	*/
/*			side												*/
/****************************************************************/
Uint32 sel_char_ck( PLAYER_WORK *tw, Uint8 side )
{
	Uint32 bit ;
	Uint8 hi ;

	hi = char_bit_get( &bit, tw->load_type ) ;
	return sel_w.char_free[side][hi] & bit ;
}

/*==============================================================*/
/*			A side's masks from its own player's and the		*/
/*			system's, less the team it has						*/
/*==============================================================*/
static void sel_pl_mask_set( PLAYER_WORK *pl_w )
{
	Uint32 on[2], off[2] ;
	Uint32 bit ;
	Sint8 i ;
	Uint8 hi ;
#ifdef TARGET_DC
	Uint32 full0, full1 ;	/* every character's bits */
#endif

	on[0] = pl_w->card_open[0] | Sys->char_open0 ;
	on[1] = pl_w->card_open[1] | Sys->char_open1 ;
	off[0] = pl_w->card_ex[0] | Sys->char_open_all0 ;
	off[1] = pl_w->card_ex[1] | Sys->char_open_all1 ;
	sel_w.char_ex[pl_w->pl_index][0] = 0 ;
	sel_w.char_ex[pl_w->pl_index][1] = 0 ;
#ifdef TARGET_DC
	sel_w.char_open[pl_w->pl_index][0] = on[0] | 0x00ffff77 ;
	sel_w.char_free[pl_w->pl_index][0] = sel_w.char_open[pl_w->pl_index][0] ;
	sel_w.char_open[pl_w->pl_index][1] = on[1] | 0x02000004 ;
	sel_w.char_free[pl_w->pl_index][1] = sel_w.char_open[pl_w->pl_index][1] ;
	sel_w.char_ex[pl_w->pl_index][0] |= off[0] ;
	sel_w.char_ex[pl_w->pl_index][1] |= off[1] ;
	/* every character open, and opened again: any side may take the same one */
	full0 = 0xf8ffffff ;
	full1 = 0x07ffffff ;
	if( sel_w.char_open[pl_w->pl_index][0] != full0 ) goto team ;
	if( sel_w.char_open[pl_w->pl_index][1] != full1 ) goto team ;
	if( sel_w.char_ex[pl_w->pl_index][0] != full0 ) goto team ;
	if( sel_w.char_ex[pl_w->pl_index][1] != full1 ) goto team ;
	sel_w.same_flag = 1 ;
team :	/* then the team's own characters */
#else
	sel_w.char_open[pl_w->pl_index][0] = on[0] | 0xc0fcfaff ;
	sel_w.char_free[pl_w->pl_index][0] = sel_w.char_open[pl_w->pl_index][0] ;
	sel_w.char_open[pl_w->pl_index][1] = on[1] | 0x01803300 ;
	sel_w.char_free[pl_w->pl_index][1] = sel_w.char_open[pl_w->pl_index][1] ;
	sel_w.same_flag = 0 ;
	sel_w.char_ex[pl_w->pl_index][0] |= off[0] ;
	sel_w.char_ex[pl_w->pl_index][1] |= off[1] ;
#endif
	for( i = 0 ; i < 3 ; i ++ ){
		hi = char_bit_get( &bit, pl_work[pl_w->pl_index + i * 2].load_type ) ;
		sel_w.char_open[pl_w->pl_index][hi] |= bit ;
	}
}

/****************************************************************/
/*			The member starts on the first of the default		*/
/*			team its side can take								*/
/****************************************************************/
void sel_def_char_set( PLAYER_WORK *pl_w )
{
	Uint8 no, i ;

	no = pl_w->timer1 * 2 + pl_w->pl_index ;
#ifdef TARGET_DC
	/* a network game's default teams are the rules' */
	if( Sys->home_mode == HOME_NETWORK ){
		if( sel_w.same_flag ){
			pl_work[no].load_type = pl_work[no].pl_type = net_w.def_team_tbl[pl_w->pl_index][0] ;
			pl_work[no].pl_personal = 0 ;
		}
		else{
			for( i = 0 ; i < 3 ; i ++ ){
				if( sel_w.char_free[pl_w->pl_index][0] & net_w.def_team_bit[pl_w->pl_index][i] ){
					pl_work[no].load_type = pl_work[no].pl_type = net_w.def_team_tbl[pl_w->pl_index][i] ;
					pl_work[no].pl_personal = 0 ;
					break ;
				}
			}
		}
		return ;
	}
#endif
	if( sel_w.same_flag ){
		pl_work[no].load_type = pl_work[no].pl_type = def_team_tbl[pl_w->pl_index][0] ;
		pl_work[no].pl_personal = 0 ;
	}
	else{
		for( i = 0 ; i < 3 ; i ++ ){
			if( sel_w.char_free[pl_w->pl_index][0] & def_team_bit[pl_w->pl_index][i] ){
				pl_work[no].load_type = pl_work[no].pl_type = def_team_tbl[pl_w->pl_index][i] ;
				pl_work[no].pl_personal = 0 ;
				break ;
			}
		}
	}
}

/****************************************************************/
/*			The colour the member starts with: the button's,	*/
/*			or the first one no member with its character		*/
/*			has													*/
/****************************************************************/
Sint8 sel_color_get( PLAYER_WORK *pl_w, PLAYER_WORK *tw, Sint16 sw )
{
	static Sint16 col_sw_tbl[6] = { 0x200, 0x100, 0x80, 0x40, 0x20, 0x10 } ;
	static Uint8 col_tbl[6] = { 0, 2, 4, 1, 3, 5 } ;
	Sint32 card[2] ;
	Uint32 bit ;
	Uint8 hi, ot, used ;
	Sint8 i, col, flag ;

	flag = 1 ;
	card[0] = card[1] = 0 ;
	if( ( Sys->play_side | Sys->new_side ) & 1 ){
		card[0] |= sel_w.char_ex[0][0] ;
		card[1] |= sel_w.char_ex[0][1] ;
	}
	if( ( Sys->play_side | Sys->new_side ) & 2 ){
		card[0] |= sel_w.char_ex[1][0] ;
		card[1] |= sel_w.char_ex[1][1] ;
	}
	hi = char_bit_get( &bit, tw->load_type ) ;
	if( ( card[hi] & bit ) == 0 ){
		if( sw == 0x80 || sw == 0x10 ) sw += sw ;
	}
	else flag = 0 ;
	ot = pl_w->pl_index ^ 1 ;
	used = 0 ;
	for( i = 0 ; i < 3 ; i ++ ){
		if( !( sel_w.pick_flag[ot] & ( 1 << i ) ) ) continue ;
		sel_char[ot][i] = pl_work[ot + i * 2].load_type ;
		sel_col[ot][i] = pl_work[ot + i * 2].load_color ;
		if( pl_work[ot + i * 2].load_type == tw->load_type ) used |= 1 << pl_work[ot + i * 2].load_color ;
	}
	for( i = 0 ; i < 3 ; i ++ ){
		sel_char[pl_w->pl_index][i] = pl_work[pl_w->pl_index + i * 2].load_type ;
		sel_col[pl_w->pl_index][i] = pl_work[pl_w->pl_index + i * 2].load_color ;
		if( pl_w->pl_index + i * 2 != tw->pl_index ){
			if( !( sel_w.pick_flag[pl_w->pl_index] & ( 1 << i ) ) ) continue ;
			if( pl_work[pl_w->pl_index + i * 2].load_type == tw->load_type ) used |= 1 << pl_work[pl_w->pl_index + i * 2].load_color ;
		}
	}
	col = 0 ;
	for( i = 0 ; i < 6 ; i ++ ){
		if( sw == col_sw_tbl[i] ){
			col = col_tbl[i] ;
			break ;
		}
	}
	if( ( 1 << col ) & used ){
		for( i = 0 ; i < 6 ; i ++ ){
			if( !( ( 1 << i ) & used ) ){
				col = i ;
				break ;
			}
		}
	}
	if( flag && col == 4 ) col = 0 ;
	else if( flag && col == 5 ) col = 1 ;
	return col ;
}

/****************************************************************/
/*			Each point of the model, moved by x and y from		*/
/*			the select file's object no							*/
/****************************************************************/
void sel_obj_pos_add( PLAYER_WORK *wk, Sint16 no, Float x, Float y )
{
	Float px, py ;

	obj_uv_begin( obj_dm01->obj[no], wk->obj ) ;
	while( obj_uv_next() == 0 ){
		obj_uv_get( &px, &py ) ;
		px += x ;
		py -= y ;
		obj_uv_put( &px, &py ) ;
	}
}

/****************************************************************/
/*			Each point of the model, set to x and y				*/
/****************************************************************/
void sel_obj_pos_set( PLAYER_WORK *wk, Sint16 no, Float x, Float y )
{
	Float px, py ;

	obj_uv_begin( obj_dm01->obj[no], wk->obj ) ;
	while( obj_uv_next() == 0 ){
		obj_uv_get( &px, &py ) ;
		px = x ;
		py = y ;
		obj_uv_put( &px, &py ) ;
	}
}

/****************************************************************/
/*			Each side's message in the select: press start,		*/
/*			the card's points and the rest						*/
/****************************************************************/
void sel_mes_disp( void )
{
	int i ;

	if( !Sys->vms_mode ) return ;
	for( i = 0 ; i < 2 ; i ++ ){
		if( !( Sys->new_side & ( 1 << i ) ) ) continue ;
		switch( pl_work[i].act_st ){
		case 0:
			if( Sys->play_side & ( 1 << i ) ) break ;
			if( sel_w.end_flag[i] ) break ;
			if( Sys->game_timer0 % 60 > 30 ) break ;
			spr_mes_put( 0x910, i * 6 ) ;
			break ;
		case 1:
			if( pl_work[i].jump_com_cnt == 0 ) break ;
			pl_work[i].jump_com_cnt -- ;
			spr_mes_put( 0x910, i * 6 + 1 ) ;
			break ;
		case 2:
			if( pl_work[i].jump_com_cnt == 0 ) break ;
			pl_work[i].jump_com_cnt -- ;
			spr_mes_put( 0x910, i * 6 + 2 ) ;
			pts_get_disp( i, vms_work.get_point[i] ) ;
			break ;
		case 4:
			if( pl_work[i].jump_com_cnt == 0 ) break ;
			pl_work[i].jump_com_cnt -- ;
			if( Sys->cpu_level_type == 0 ) spr_mes_put( 0x910, i * 6 + 4 ) ;
			else spr_mes_put( 0x910, i * 6 + 5 ) ;
			break ;
		case 3:
			if( pl_work[i].jump_com_cnt == 0 ) break ;
			pl_work[i].jump_com_cnt -- ;
			spr_mes_put( 0x910, i * 6 + 3 ) ;
			break ;
		}
	}
}

/****************************************************************/
void sel_dummy_0( void )
{
}

/****************************************************************/
void sel_dummy_1( PLAYER_WORK *w )
{
}

/*==============================================================*/
/*			Both sides back to their first step, with no		*/
/*			message												*/
/*==============================================================*/
static void sel_pl_clr( void )
{
	pl_work[0].routine_0 = 0 ;
	pl_work[0].routine_1 = 0 ;
	pl_work[0].routine_2 = 0 ;
	pl_work[0].routine_3 = 0 ;
	pl_work[0].act_st = 0 ;
	pl_work[0].old_act_st = 0 ;
	pl_work[0].jump_com_cnt = 0 ;
	pl_work[0].jump_dash_cnt = 0 ;
	pl_work[1].routine_0 = 0 ;
	pl_work[1].routine_1 = 0 ;
	pl_work[1].routine_2 = 0 ;
	pl_work[1].routine_3 = 0 ;
	pl_work[1].act_st = 0 ;
	pl_work[1].old_act_st = 0 ;
	pl_work[1].jump_com_cnt = 0 ;
	pl_work[1].jump_dash_cnt = 0 ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "select.c" End of File	*/
