/* The system's set-up and its frame: System_Init reads the settings into Sys, the palettes and the
   system textures are loaded, and each frame runs the system task Sys->task_no picks. */
#include "player.h"
#include "naomi.h"

extern void vms_init( void ) ;

extern NLthd sys_font_thd[];	/* the system font */
extern NLthd sys_spr_thd[];

extern void SortModeSet( int mode ) ;
extern void SysTransReset( void ) ;
extern void sprite_init( void ) ;
extern void col_send( void ) ;
extern void stereo_set( void ) ;
extern void mono_set( void ) ;
extern void vib_init( void ) ;
extern void dbg_flag_init( void ) ;
extern void vib_move( void ) ;
extern void task_init( void ) ;
extern void task_attract( void ) ;
extern void task_game( void ) ;
extern void test_mode_jump( void ) ;

void Palette_Init( void ) ;
void Texture_Init( void ) ;
static void sys_task( void ) ;

/****************************************************************/
/*			The system's set-up, once at power on				*/
/****************************************************************/
void System_Init( void )
{
	SortModeSet( 1 ) ;
	Sys = &sys_work ;
	memset( &sys_work, 0, SYS_HALF ) ;
	Sys->play_mode = 32 ;
	Sys->monitor_31k = 0 ;
	Sys->cpu_level_type = sys_flag->Country ;
	if( sys_flag->DisplayMode == NLA_DISPLAY_31K ) Sys->monitor_31k = 1 ;
	Sys->attract_step = 0 ;
	Sys->stereo = conf_stereo ;
	Sys->demo_sound = sys_flag->SoundFlag ;
	if( Sys->demo_sound && conf_demo_sound_half ) Sys->demo_sound = 2 ;
	Sys->cont_enable = conf_cont_enable ;
	Sys->voice_type = conf_voice_type ;
	Sys->renda_level = conf_difficulty ;
	Sys->time_speed = conf_time_speed ;
	Sys->damage_level = conf_damage_level ;
	Sys->vms_mode = conf_vms_mode ;
	Sys->vm_write = conf_vm_write ;
	Sys->speed_fix = conf_speed_fix ;
	Sys->event_flag = conf_event_flag ;
	Sys->no_challenge_flag = conf_no_challenge_flag ;
	Sys->open_lv = conf_open_lv ;
	Sys->unk_4d = 2 ;
	vib_init() ;
	Sys->unk_2b = 0 ;
	Sys->develop_flag = 0 ;
	task_wait = 3 ;
	stage_no = 0 ;
	dbg_mode = 0 ;
	tex_req_flag = 0 ;
	tex_req_no = 0 ;
	sprite_keep_flag = 0 ;
	Sys->char_open0 = 0xc0fcfaff ;
	Sys->char_open1 = 0x01803300 ;
	Palette_Init() ;
	Texture_Init() ;
	fade_init() ;
	bright_init() ;
	fog_init( 10 ) ;
	dbg_flag_init() ;
	rank_data_init() ;
	if( Sys->stereo ) stereo_set() ;
	else mono_set() ;
	vms_init() ;
}

/****************************************************************/
/*			Free the palettes, and give each of col_work's		*/
/*			lines its own										*/
/****************************************************************/
void Palette_Init( void )
{
	int i ;

	nlFreePaletteAll() ;
	for( i = 0 ; i < 128 ; i ++ ){
		col_work[i].pal_no = i ;
		col_work[i].cnt = 16 ;
		col_work[i].trans_flag = 0 ;
	}
}

/****************************************************************/
/*			Load the system's textures							*/
/****************************************************************/
void Texture_Init( void )
{
	nlFreeCommonTextureAll() ;
	nlSetCommonTextureOffset( 0 ) ;
	nlDispStripLoadTexture( 0, sys_font_thd ) ;
	nlSpriteLoadTextureOnce( sys_font_thd, 0, 1 ) ;
	nlSetCommonTextureOffset( 2 ) ;
	nlSpriteLoadTexture( sys_spr_thd, 0 ) ;
	nlSetCommonTextureOffset( 0 ) ;
	nlSetCommonTextureOffset( 0 ) ;
	sprite_init() ;
	sprite_keep_flag = 0 ;
	obj_stg = NULL ;
	obj_efky = NULL ;
	obj_dm00 = NULL ;
	obj_dm01 = NULL ;
	obj_dm02 = NULL ;
	obj_dm03 = NULL ;
	obj_dm04 = NULL ;
	obj_dm05 = NULL ;
	obj_dm06 = NULL ;
	obj_dm07 = NULL ;
	obj_dm08 = NULL ;
	obj_dm09 = NULL ;
	obj_dm0a = NULL ;
	obj_dm0b = NULL ;
}

/****************************************************************/
/*			The system's frame: its task, unless it is held		*/
/*			this frame, and the palettes						*/
/****************************************************************/
void System_Main( void )
{
	if( !Sys->task_stop ) sys_task() ;
	Sys->task_stop = 0 ;
	col_send() ;
}

/*==============================================================*/
static void sys_task( void )
{
	static void ( *sys_task_jmp[] )( void ) = {
		task_init,
		task_attract,
		task_game,
		test_mode_jump,
	} ;

	if( !task_wait ){
		SysTransReset() ;
		bright_stop_set() ;
		sys_task_jmp[Sys->task_no]() ;
		back_color_disp() ;
		fade_move() ;
	}
	Sys->game_timer0 ++ ;
	if( ( Sys->wave_ang += 80 ) >= 0x10000 ) Sys->wave_ang = 0 ;
	vib_move() ;
}

/****************************************************************/
/*			The game task is in its scene 2 (the one at			*/
/*			0C0302C0)											*/
/****************************************************************/
int game_play_ck( void )
{
	if( Sys->task_no == 2 && Sys->task_step == 1 && Sys->scene_no == 2 ) return 1 ;
	return 0 ;
}

/****************************************************************/
int cpu_max_ck( void )
{
	return Sys->cpu_level_max & 1 ;
}

/****************************************************************/
/*			And in its scene 4 (0C031704)						*/
/****************************************************************/
int game_staff_ck( void )
{
	if( Sys->task_no == 2 && Sys->task_step == 1 && Sys->scene_no == 4 ) return 1 ;
	return 0 ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "sys_task.c" End of File	*/
