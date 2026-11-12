/* The Dreamcast program's system: the start-up and shutdown of the libraries, the pads and the
   keyboard read each vertical blank, the disc's door, the options, the reset keys, the memory card
   at start-up, the points and the menus' keys. */
#include "player.h"
#include "shinobi.h"
#include "adx.h"
#include "dm.h"

/* SHC's built-ins, as its smachine.h names them */
#define set_imask( mask )	_builtin_set_imask( mask )

extern Sint32 nlsndInit( void *Address, Sint32 BoardType ) ;
extern void nlsndStreamInit( Sint32 a, Sint32 b, Sint32 c ) ;
extern Uint32 syTmrGenGetCount( void ) ;
extern void syTmrGenSetCount( Uint32 count ) ;

extern void com_sound_load() ;	/* it is passed a 0 it doesn't read */
extern void sound_init( void ) ;
extern void stereo_set( void ) ;
extern void mono_set( void ) ;
extern void file_load( Sint32 no, void *adrs, Sint32 a ) ;
extern void file_load3( Sint32 no, void *adrs, Sint32 a ) ;
extern void kbd_read( void ) ;

extern void dm0c_wall_paper( int no ) ;
extern void dm0c_vm_win_set( int st ) ;
extern void dm0c_vm_msg_set( void ) ;
extern void dm11_msg_set( int no, int port ) ;
extern void SysTransReset( void ) ;
extern void vib_stop( void ) ;
extern void load_stop( void ) ;
extern void load_stop2( void ) ;
extern int vms_check_unit( void ) ;
extern int vms_check_datafile( void ) ;
extern int vms_ports_net_ck( void ) ;
extern int vms_ports_dow_ck( void ) ;
extern int vms_check_execfile( void ) ;
extern int vms_check_free_data( void ) ;

static void pad_read( void ) ;
void system_exit( void ) ;
static void gd_err_func( void *obj, Sint32 err ) ;
Sint32 VBlankInIrq( void ) ;
void sys_set_init( int kind ) ;
void soft_reset( int trans ) ;
void soft_reset_point( int trans ) ;
void disp_adjust_set( void ) ;
int vms_boot_card_ck( void ) ;
int vms_boot_state_ck( Sint8 st ) ;
void secret_work_init( void ) ;
void d_point_get( void ) ;
void n_point_get( void ) ;
int decide_ck_nose( void ) ;
int cancel_ck( void ) ;
int cancel_ck_nose( void ) ;
int decide_cancel_ck( void ) ;
void pl_sw_config_sub( int side ) ;

/* the disc: the library's own handle, a drive request is out, and gdFs's work for 8 open files
   and a directory of 1024 */
static GDFS gd_sys_hn ;
Sint32 gd_drv_req ;
static Uint8 unk_8C1CBE08[0x18] ;
static Uint8 gdfs_work[0x13a0] ;
static Uint8 gdfs_dirbuf[0xb040] ;

/* the music stream's player and its work */
static Uint8 adx_work[ADXT_WORK_SIZE] ;
ADXT adxt ;

static void *cfg_work ;	/* the work of the console's settings */
static Uint32 tmr_count ;	/* the timer's count */
static Uint16 key_on[2][6] ;	/* each side's six buttons, held */
static Uint8 unk_8C1F8AE8[12] ;	/* unused */

static Uint16 lever_tbl[8] = {
	KEY_DOWN, KEY_DOWN | KEY_RIGHT, KEY_RIGHT, KEY_UP | KEY_RIGHT, KEY_UP, KEY_UP | KEY_LEFT, KEY_LEFT, KEY_DOWN | KEY_LEFT
} ;

Float adjust_size_tbl[6] = { 1.0f, 0.99f, 0.98f, 0.97f, 0.96f, 0.95f } ;

/* the card screens' eight ports (A1, A2, B1, B2, C1, C2, D1, D2): from each, the ports to try in
   turn when its card is gone, by the way the lever was last moved */
static Sint32 port_near_tbl[8][8] = {
	{ 1, 2, 3, 4, 5, 6, 7, 0 },
	{ 0, 3, 2, 5, 4, 7, 6, 1 },
	{ 0, 3, 4, 1, 5, 6, 7, 2 },
	{ 1, 2, 5, 0, 4, 6, 7, 3 },
	{ 2, 5, 6, 0, 3, 7, 1, 4 },
	{ 3, 4, 7, 1, 2, 0, 6, 5 },
	{ 7, 4, 5, 2, 3, 0, 1, 6 },
	{ 6, 5, 4, 3, 2, 1, 0, 7 },
} ;

static Sint32 port_up_tbl[8][8] = {
	{ 7, 6, 3, 2, 5, 4, 1, 0 },
	{ 6, 7, 2, 3, 4, 5, 0, 1 },
	{ 1, 0, 5, 4, 7, 6, 3, 2 },
	{ 0, 1, 4, 5, 6, 7, 2, 3 },
	{ 7, 6, 3, 2, 1, 0, 5, 4 },
	{ 6, 7, 2, 3, 0, 1, 4, 5 },
	{ 5, 4, 3, 2, 1, 0, 7, 6 },
	{ 4, 5, 2, 3, 0, 1, 6, 7 },
} ;

static Sint32 port_down_tbl[8][8] = {
	{ 3, 2, 5, 4, 7, 6, 1, 0 },
	{ 2, 3, 4, 5, 6, 7, 0, 1 },
	{ 1, 0, 5, 4, 7, 6, 3, 2 },
	{ 0, 1, 4, 5, 6, 7, 2, 3 },
	{ 3, 2, 7, 6, 1, 0, 5, 4 },
	{ 2, 3, 0, 1, 6, 7, 4, 5 },
	{ 5, 4, 3, 2, 1, 0, 7, 6 },
	{ 4, 5, 2, 3, 0, 1, 6, 7 },
} ;

static Sint32 port_right_tbl[8][8] = {
	{ 4, 6, 3, 5, 7, 1, 2, 0 },
	{ 5, 7, 2, 4, 6, 0, 3, 1 },
	{ 6, 5, 7, 3, 0, 1, 4, 2 },
	{ 7, 4, 6, 2, 1, 0, 5, 3 },
	{ 7, 5, 0, 1, 2, 3, 6, 4 },
	{ 6, 4, 1, 0, 3, 2, 7, 5 },
	{ 2, 4, 7, 1, 3, 5, 0, 6 },
	{ 3, 5, 6, 0, 2, 4, 1, 7 },
} ;

static Sint32 port_left_tbl[8][8] = {
	{ 4, 2, 7, 5, 3, 1, 0, 6 },
	{ 5, 3, 6, 4, 2, 0, 1, 7 },
	{ 1, 3, 4, 5, 6, 7, 2, 0 },
	{ 0, 2, 5, 4, 7, 6, 3, 1 },
	{ 0, 1, 3, 5, 7, 6, 4, 2 },
	{ 1, 0, 2, 4, 6, 7, 5, 3 },
	{ 2, 0, 3, 5, 1, 7, 6, 4 },
	{ 3, 1, 2, 4, 0, 6, 7, 5 },
} ;

/* the pad's six buttons, and what each can be set to do */
static Uint16 key_bit_tbl[6] = { KEY_LP, KEY_HP, KEY_LK, KEY_HK, KEY_A1, KEY_A2 } ;

static Uint16 key_func_tbl[12] = {
	KEY_LP, KEY_HP, KEY_LK, KEY_HK, KEY_A1, KEY_A2,
	KEY_PUNCH, KEY_KICK, KEY_LIGHT, KEY_HEAVY, KEY_ASSIST, KEY_START | KEY_LK
} ;

static Uint8 unk_8C14414C[6] = { 0, 1, 2, 3, 4, 5 } ;

/*==============================================================*/
/*			The pads of ports A and B as each side's keys,		*/
/*			and port D's keyboard								*/
/*==============================================================*/
static void pad_read( void )
{
	Uint8 i ;
	Uint32 on ;
	Uint16 sw ;
	Uint8 dir ;
	Float x, y ;
	Sint32 ang ;

	pdExecPeripheralServer() ;
	port_peri[0] = pdGetPeripheral( PDD_PORT_A0 ) ;
	port_peri[1] = pdGetPeripheral( PDD_PORT_B0 ) ;
	port_peri[2] = pdGetPeripheral( PDD_PORT_C0 ) ;
	port_peri[3] = pdGetPeripheral( PDD_PORT_D0 ) ;
	if( !net_play_flag ){
		for( i = 0 ; i < 2 ; i ++ ){
			if( port_peri[i]->info->type & PDD_DEVTYPE_CONTROLLER ){
				sw_work[i].old = sw_work[i].now ;
				sw_work[i].trg_l = port_peri[i]->l ;
				sw_work[i].trg_r = port_peri[i]->r ;
				sw_work[i].stick_x = port_peri[i]->x1 ;
				sw_work[i].stick_y = port_peri[i]->y1 ;
				on = port_peri[i]->on ;
				sw = 0 ;
				if( on & PDD_DGT_KU ) sw = KEY_UP ;
				if( on & PDD_DGT_KD ) sw |= KEY_DOWN ;
				if( on & PDD_DGT_KL ) sw |= KEY_LEFT ;
				if( on & PDD_DGT_KR ) sw |= KEY_RIGHT ;
				if( !sw ){
					x = sw_work[i].stick_x ;
					y = sw_work[i].stick_y ;
					if( x * x + y * y > 6400.0f ){
						ang = nlArcTan2( x, y ) ;
						dir = ( Uint16 )( ang + 0x1000 ) / 0x2000 & 7 ;
						sw |= lever_tbl[dir] ;
					}
				}
				if( on & PDD_DGT_ST ) sw |= KEY_START ;
				if( on & PDD_DGT_TX ) sw |= KEY_LP ;
				if( on & PDD_DGT_TY ) sw |= KEY_HP ;
				if( on & PDD_DGT_TL ) sw |= KEY_A1 ;
				if( on & PDD_DGT_TA ) sw |= KEY_LK ;
				if( on & PDD_DGT_TB ) sw |= KEY_HK ;
				if( on & PDD_DGT_TR ) sw |= KEY_A2 ;
				if( on & PDD_DGT_TZ ) sw |= KEY_A1 ;
				if( on & PDD_DGT_TC ) sw |= KEY_A2 ;
				sw_work[i].now = sw ;
			} else {
				sw_work[i].now = 0 ;
				sw_work[i].trg_l = 0 ;
				sw_work[i].trg_r = 0 ;
				sw_work[i].stick_x = 0 ;
				sw_work[i].stick_y = 0 ;
			}
			sw_work[i].on = ~sw_work[i].old & sw_work[i].now ;
			sw_work[i].off = sw_work[i].old & ~sw_work[i].now ;
		}
	}
	if( port_peri[3]->info->type & PDD_DEVTYPE_KEYBOARD ){
		kbd_data = pdKbdGetData( PDD_PORT_D0 ) ;
		kbd_read() ;
	} else {
		kbd_data = NULL ;
		kbd_now = kbd_old = kbd_on = 0 ;
	}
}

/****************************************************************/
/*			A held key repeats: after 36 frames it is given		*/
/*			again every 6										*/
/****************************************************************/
void key_repeat( SW_WORK *kw )
{
	kw->rep = 0 ;
	if( kw->old == kw->now ){
		kw->rep_cnt ++ ;
		if( kw->rep_cnt > 30 ){
			if( kw->rep_cnt > 35 ){
				kw->rep_cnt = 30 ;
				kw->rep = kw->now ;
			}
		}
	} else kw->rep_cnt = 0 ;
}

/****************************************************************/
/*			A network game waits one more frame					*/
/****************************************************************/
void net_vsync_wait( void )
{
	if( Sys->home_mode == HOME_NETWORK ) nlWaitVSync() ;
}

/****************************************************************/
/*			Shut the libraries down and go back to the			*/
/*			console's own menu									*/
/****************************************************************/
void system_exit( void )
{
	mdmClose( net_mdm_hn ) ;
	mdmFinish() ;
	syStartGlobalDestructor() ;
	if( adxt != ( ADXT )-1 ) ADXT_Finish() ;
	syCfgExit() ;
	gdFsFinish() ;
	pdExitPeripheral() ;
	pdKbdExit() ;
	nlExitSystem() ;
	syMallocFinish() ;
	syHwFinish() ;
	set_imask( 15 ) ;
	syBtExit() ;
}

/****************************************************************/
/*			Shut the libraries down								*/
/****************************************************************/
void system_finish( void )
{
	mdmClose( net_mdm_hn ) ;
	mdmFinish() ;
	syStartGlobalDestructor() ;
	if( adxt != ( ADXT )-1 ) ADXT_Finish() ;
	syCfgExit() ;
	gdFsFinish() ;
	pdExitPeripheral() ;
	pdKbdExit() ;
	nlExitSystem() ;
	syMallocFinish() ;
	syHwFinish() ;
	set_imask( 15 ) ;
}

/*==============================================================*/
/*			A disc error: an opened door or a changed disc		*/
/*			asks for the way out								*/
/*==============================================================*/
static void gd_err_func( void *obj, Sint32 err )
{
	if( err == GDD_ERR_TRAYOPEND || err == GDD_ERR_UNITATTENT ) exit_req = 1 ;
}

/****************************************************************/
/*			Each frame, ask the drive for its state or read		*/
/*			the answer: an open door or no disc asks for the	*/
/*			way out												*/
/****************************************************************/
void gd_door_ck( void )
{
	Sint32 ret ;

	if( !gd_drv_req ){
		ret = gdFsReqDrvStat() ;
		if( ret == GDD_ERR_OK ) gd_drv_req = 1 ;
		if( ret == GDD_ERR_TRAYOPEND || ret == GDD_ERR_UNITATTENT ) exit_req = 1 ;
	} else {
		gd_sys_hn = gdFsGetSysHn() ;
		ret = gdFsGetStat( gd_sys_hn ) ;
		if( ret != GDD_STAT_BUSY ) gd_drv_req = 0 ;
		ret = gdFsGetDrvStat() ;
		if( ret == GDD_DRVSTAT_OPEN || ret == GDD_DRVSTAT_NODISC ) exit_req = 1 ;
	}
}

/****************************************************************/
/*			The vertical blank: count the frame, run the		*/
/*			music stream and read the controls					*/
/****************************************************************/
Sint32 VBlankInIrq( void )
{
	Sys->vblank_cnt ++ ;
	vsync_count ++ ;
	if( task_wait ) task_wait -- ;
	if( lobby_time_flag ) lobby_time ++ ;
	stream_start_flag = 0 ;
	ADXT_ExecServer() ;
	pad_read() ;
	return 0 ;
}

/****************************************************************/
/*			Set the system up: the hardware, the libraries,		*/
/*			the screen for the cable, the controls, the disc,	*/
/*			the first files, the sound and music and the		*/
/*			interrupts											*/
/****************************************************************/
void system_init( void )
{
	Float work[129] ;
	int i ;
	Sint32 ret ;

	set_imask( 14 ) ;
	nlSetIntrStack( _ISTACK_END ) ;
	syHwInit() ;
	syMallocInit( ( void * )0x8ce2a020, 0x4800 ) ;
	switch( syCblCheck() ){
	case SYE_CBL_VGA :
		nlInitLibraryDC( 0, 0x80, 1 ) ;
		break ;
	case SYE_CBL_NTSC :
		nlInitLibraryDC( 9, 0x80, 1 ) ;
		break ;
	case SYE_CBL_PAL :
		nlInitLibraryDC( 19, 0x80, 1 ) ;
		break ;
	}
	nlSysSetTextureSize( 0x100000 ) ;
	set_imask( 14 ) ;
	syHwInit2() ;
	pdInitPeripheral( 0, peri_recv_buff, peri_send_buff ) ;
	pdKbdInit() ;
	set_imask( 0 ) ;
	nlSysMetricsInit() ;
	syStartGlobalConstructor() ;
	nlSysUseLatency2V() ;
	nlSysSetDirectListTypeLatency2V( 0 ) ;
	nlSysSetVertexBufferBaseAddress( ( void * )0x0cf00000 ) ;
	nlSysCreatePunchBuffer( 0x10000 ) ;
	nlCreateVertexBuffer( 0, 0x4000, 0x24000, 0x4000 ) ;
	nlInitPrint() ;
	nlPrnIndentOn() ;
	nlSetBorderColor( 0 ) ;
	nlInitCommonTexture( common_tex_work, 0xc00, 0x1000 ) ;
	nlInitSprite( sprite_work, 0xc00, 0x1000 ) ;
	nlInitPalette( palette_work, 64 ) ;
	nlSpriteSetImaginaryScreenSize( 640.0f, 480.0f ) ;
	nlPaletteDirectMode( 1 ) ;
	nlSetPaletteMode( 2 ) ;
	nlInitMatrix( matrix_stack, 64 ) ;
	nlMatrixMode( 3 ) ;
	nlUnitMatrix() ;
	nlPerspective( 0x1d28, 1.3333334f, 0.3f, 12000.0f ) ;
	nlMatrixMode( 1 ) ;
	for( i = 0 ; i < 129 ; i ++ ) work[i] = 0.0f ;
	nlFogTableColor( 0x101010 ) ;
	nlFogDensity( 0xff09 ) ;
	adxt = ( ADXT )-1 ;
	for( i = 0x100 ; i > 0 ; i -- ){
		ret = gdFsInit( 8, gdfs_work, 1024, gdfs_dirbuf ) ;
		if( ret == GDD_ERR_TRAYOPEND || ret == GDD_ERR_UNITATTENT ) system_exit() ;
		else if( ret == GDD_ERR_OK ) break ;
	}
	if( !i ) system_exit() ;
	gdFsEntryErrFuncAll( gd_err_func, 0 ) ;
	cfg_work = ( void * )0x0ce26000 ;
	syCfgInit( cfg_work ) ;
	sys_set_init( 0xff ) ;
	file_load3( 0, ( void * )0x0cc00000, 1 ) ;
	nlsndInit( ( void * )0x0cc00000, 0 ) ;
	com_sound_load( 0 ) ;
	sound_init() ;
	adxt = 0 ;
	nlsndStreamInit( 0, 2, 0 ) ;
	ADXT_Init() ;
	if( ( adxt = ADXT_Create( 2, adx_work, ADXT_WORK_SIZE ) ) != NULL ) ADXT_SetAutoRcvr( adxt, ADXT_RMODE_STOP ) ;
	if( conf_stereo ) stereo_set() ;
	else mono_set() ;
	syRtcInit() ;
	nlObjPutInit() ;
	nlObjPutInitTexture() ;
	nlObjPutMvInit( 0x80 ) ;
	nlObjPutMvSetMode( 1 ) ;
	nlDispStripInit() ;
	file_load( 0x299, ( void * )0x0ce00000, 0 ) ;
	file_load( 0x298, ( void * )0x0ce08000, 0 ) ;
	thd_file_load( 1, ( void * )0x0ce0a000 ) ;
	thd_file_load( 2, ( void * )0x0ce1d000 ) ;
	file_load( 0x94, ( void * )0x0ce1e000, 0 ) ;
	file_load( 0x92, ( void * )0x0ce2ea00, 0 ) ;
	nlInitInterruptDC() ;
	nlSetIntrFunction( 1, VBlankInIrq ) ;
	nlSetIntrFunction( 0, vblank_func ) ;
	net_info_flag = 0 ;
	if( ntInfIsNetworkInfo() != NTD_ERR_NOINFO ) net_info_flag = 1 ;
}

/****************************************************************/
/*			The options as they start: the game's (1), the		*/
/*			buttons' (2) and the screen's (4); 0xff all of		*/
/*			them, with the console's own sound output			*/
/****************************************************************/
void sys_set_init( int kind )
{
	int i ;
	Sint32 mode ;
	OPTION_WORK *ow = &option_w ;

	if( kind & 1 ){
		ow->difficulty = 1 ;
		ow->round_time = 2 ;
		ow->damage_level = 1 ;
		ow->speed_fix = 0 ;
		conf_difficulty = ow->difficulty ;
		conf_round_time = ow->round_time ;
		conf_damage_level = ow->damage_level ;
		conf_speed_fix = ow->speed_fix ;
		ow->unk_07 = 0 ;
		ow->unk_1f = 0 ;
	}
	if( kind & 2 ){
		ow->com_time[0] = 0 ;
		ow->com_time[1] = 0 ;
		ow->unk_1c = 0 ;
		for( i = 0 ; i < 6 ; i ++ ){	/* each row through a cast of its own: the ROM addresses the two apart */
			( ( Uint16 * )ow->key_config[0] )[i] = i ;
			( ( Uint16 * )ow->key_config[1] )[i] = i ;
		}
	}
	if( kind & 4 ){
		ow->adjust_x = 0.0f ;
		ow->adjust_y = 0.0f ;
		ow->unk_1d = 0 ;
		ow->unk_1e = 0 ;
	}
	if( kind == 0xff ){
		syCfgGetSoundMode( &mode ) ;
		ow->stereo = mode ^ 1 ;
		ow->unk_05 = 1 ;
		ow->vib_bit = 0 ;
		conf_stereo = ow->stereo ;
	}
}

/****************************************************************/
/*			The settings into the system work					*/
/****************************************************************/
void sys_set_load( void )
{
	Sys->stereo = conf_stereo ;
	Sys->demo_sound = 1 ;
	Sys->cont_enable = 1 ;
	Sys->voice_type = 0 ;
	Sys->renda_level = conf_difficulty ;
	Sys->time_speed = 1 ;
	Sys->damage_level = conf_damage_level ;
	Sys->vms_mode = 0 ;
	Sys->vm_write = 0 ;
	Sys->speed_fix = conf_speed_fix ;
	Sys->event_flag = 0 ;
	Sys->no_challenge_flag = 0 ;
	Sys->open_lv = 0 ;
	Sys->round_time_no = conf_round_time ;
	Sys->gauge_double = 0 ;
	Sys->red_vital_keep = 0 ;
	Sys->vital_drain = 0 ;
	Sys->red_vital_rule = 0 ;
	Sys->guard_dm_rule = 0 ;
	Sys->share_vital = 0 ;
	Sys->one_char = 0 ;
}

/****************************************************************/
/*			The reset keys (START, A, B, X and Y) held: in the	*/
/*			attract (game 0) ask for the way out, in a game		*/
/*			(1) go back to the attract							*/
/****************************************************************/
int soft_reset_ck( int game )
{
	int ret ;
	int i ;

	ret = 0 ;
	if( reset_lock ) return 0 ;
	if( vms_work.task_flag ) return 0 ;
	for( i = 0 ; i < 2 ; i ++ ){
		if( Sys->home_mode == HOME_NETWORK && net_play_flag ){
			if( net_side != ( 1 << i ) ) continue ;
		}
		if( ( sw_work[i].now & ( KEY_START | KEY_ATCK ) ) == ( KEY_START | KEY_ATCK ) ){
			ret = 1 ;
			switch( game ){
			case 0 :
				exit_req = 1 ;
				break ;
			case 1 :
				soft_reset( 1 ) ;
				break ;
			}
		}
	}
	return ret ;
}

/****************************************************************/
/*			Back to the attract: a game gives up its points,	*/
/*			and the sound, the loading and the pads' shaking	*/
/*			stop												*/
/****************************************************************/
void soft_reset( int trans )
{
	if( Sys->play_mode == 96 ) soft_reset_point( trans ) ;
	task_wait = 30 ;
	Sys->play_mode = 64 ;
	Sys->task_no = 1 ;
	Sys->task_step = 0 ;
	Sys->scene_no = 0 ;
	Sys->routine_0 = 0 ;
	Sys->routine_1 = 0 ;
	Sys->routine_2 = 0 ;
	Sys->routine_3 = 0 ;
	Sys->exit_flag = 0 ;
	Sys->unk_5d = 0 ;
	Sys->unk_5e = 0 ;
	Sys->home_mode = HOME_ARCADE ;
	Sys->join_flag = 1 ;
	Sys->attract_step = 7 ;
	Sys->ended_mode = -1 ;
	back_color_set( 0 ) ;
	if( trans ) SysTransReset() ;
	stream_stop() ;
	sound_all_off() ;
	se_all_off() ;
	se_vol_up( 0, 1 ) ;
	se_vol_up( 1, 1 ) ;
	s3l_fade_in_set( 1 ) ;
	load_stop() ;
	load_stop2() ;
	vib_stop() ;
}

/****************************************************************/
/*			A game given up loses points: 100 for each human	*/
/*			side												*/
/****************************************************************/
void soft_reset_point( int trans )
{
	switch( Sys->home_mode ){
	case HOME_ARCADE :
		d_point_add( -100 ) ;
		if( Sys->play_side == 3 ) d_point_add( -100 ) ;
		break ;
	case HOME_VERSUS :
	case HOME_SCORE :
		d_point_add( -100 ) ;
		break ;
	case HOME_NETWORK :
	case HOME_TRAINING :
		break ;
	}
	if( trans ){
		vms_work.d_point = 0 ;
		vms_work.n_point = 0 ;
	}
}

/****************************************************************/
/*			Move the picture to the option's screen position	*/
/****************************************************************/
void disp_adjust_set( void )
{
	nlSetWindowSize( 0.0f, 0.0f, 640.0f, 480.0f ) ;
	nlSetBackColorZ( 0, 0, 0, 0.001f ) ;
	nlWaitVSync() ;
	nlSetWindowSize( 0.0f, 0.0f, 640.0f, 480.0f ) ;
	nlSetBackColorZ( 0, 0, 0, 0.001f ) ;
	nlWaitVSync() ;
	nlSetWindowSize( option_w.adjust_x, option_w.adjust_y, 640.0f, 480.0f ) ;
}

/****************************************************************/
/*			And back to the middle								*/
/****************************************************************/
void disp_adjust_clear( void )
{
	nlSetWindowSize( 0.0f, 0.0f, 640.0f, 480.0f ) ;
	nlSetBackColorZ( 0, 0, 0, 0.001f ) ;
	nlWaitVSync() ;
	nlSetWindowSize( 0.0f, 0.0f, 640.0f, 480.0f ) ;
	nlSetBackColorZ( 0, 0, 0, 0.001f ) ;
	nlWaitVSync() ;
	nlSetWindowSize( 0.0f, 0.0f, 640.0f, 480.0f ) ;
}

/****************************************************************/
/*			The cards at start-up: 2 one has the game's file,	*/
/*			3 one has room for it, 4 none has room, 5 no card,	*/
/*			7 the cards are busy								*/
/****************************************************************/
int vms_boot_card_ck( void )
{
	int bit ;
	Uint32 i ;
	int ret ;

	bit = vms_check_datafile() ;
	vms_work.card_bit_keep = bit ;
	if( bit != -1 && bit != 0 ){
		i = 0 ;
		while( !( bit & ( 1 << i ) ) && i < 8 ) i ++ ;
		if( i != 8 ){
			vms_work.card_bit = ( 1 << i ) & bit ;
			vms_work.cursor_port = i ;
			return 2 ;
		}
	}
	bit = vms_check_unit() ;
	if( bit == 0 || bit == -1 ) return 5 ;
	i = 0 ;
	while( !( bit & ( 1 << i ) ) && i < 8 ) i ++ ;
	if( i == 8 ) return 5 ;
	vms_work.card_bit = ( 1 << i ) & bit ;
	vms_work.cursor_port = i ;
	ret = vms_check_free_data() ;
	if( ret != -1 ){
		if( ret & vms_work.card_bit ) return 3 ;
		return 4 ;
	}
	return 7 ;
}

/****************************************************************/
/*			The cards are no longer as a step found them: go	*/
/*			to the step for what they are now					*/
/****************************************************************/
int vms_boot_state_ck( Sint8 st )
{
	Sint8 now ;

	now = vms_boot_card_ck() ;
	if( now != st && now != 7 ){
		Sys->scene_no = now ;
		Sys->routine_0 = Sys->routine_1 = 0 ;
		return 1 ;
	}
	return 0 ;
}

/****************************************************************/
/*			The attract's card screen at start-up: load the		*/
/*			game's file from the card that has it, or make it	*/
/*			on a card with room									*/
/****************************************************************/
void vms_boot_load( void )
{
	Uint16 key ;
	Sint32 port ;
	Sint32 old ;
	int ret ;
	int bit ;
	Sint8 st ;

	key = menu_sw_get( 3 ) ;
	switch( port = vms_work.cursor_port, Sys->scene_no ){
	case 0 :
		Sys->scene_no ++ ;
		Sys->routine_0 = 0 ;
		Sys->timer0 = 60 ;
		Palette_Init() ;
		Texture_Init() ;
		TexLoad( 20 ) ;
		work_init() ;
		secret_work_init() ;
		vms_work.card_task_step[0][0] = 0 ;
		vms_work.disp_adjust_req = 0 ;
		vms_work.file_flag = 0 ;
		dm11_msg_set( 0, 0 ) ;
		fade_set( 0xff000000, 10, FADE_IN ) ;
	case 1 :
		if( !fade_cnt() ){
			if( Sys->timer0 -- == 0 ) Sys->scene_no = vms_boot_card_ck() ;
			else {
				ret = vms_boot_card_ck() ;
				if( ret != 5 ) Sys->scene_no = ret ;
			}
		}
		break ;
	case 2 :
		switch( Sys->routine_0 ){
		case 0 :
			Sys->routine_0 ++ ;
			dm11_msg_set( 1, vms_work.cursor_port ) ;
		case 1 :
			ret = vms_task_load_all( vms_work.cursor_port, 0 ) ;
			if( ret == 1 ){
				Sys->routine_0 ++ ;
				Sys->timer0 = 60 ;
				vms_work.file_flag = 1 ;
				dm11_msg_set( 2, vms_work.cursor_port ) ;
				vms_work.file_port = vms_work.cursor_port ;
				vms_date_keep( vms_work.file_port ) ;
			} else if( ret == 0 ){
				Sys->routine_0 = 3 ;
				vms_work.file_flag = 0 ;
				dm11_msg_set( 9, vms_work.cursor_port ) ;
			}
			break ;
		case 2 :
			goto wait ;
		case 3 :
			if( key & KEY_START ){
				Sys->scene_no = 1 ;
				Sys->routine_0 = Sys->routine_1 = 0 ;
				Sys->timer0 = 60 ;
				dm11_msg_set( 0, 0 ) ;
				na_req( 2 ) ;
			}
			break ;
		}
		break ;
	case 3 :
		switch( Sys->routine_0 ){
		case 0 :
			Sys->routine_0 ++ ;
			dm11_msg_set( 3, vms_work.cursor_port ) ;
		case 1 :
			old = vms_work.cursor_port ;
			if( vms_boot_state_ck( Sys->scene_no ) ) break ;
			if( old != vms_work.cursor_port ){
				Sys->routine_0 = Sys->routine_1 = 0 ;
				break ;
			}
			if( !( key & KEY_START ) ) break ;
			na_req( 2 ) ;
			Sys->routine_0 ++ ;
			vms_work.card_task_step[0][0] = 0 ;
			break ;
		case 2 :
			dm11_msg_set( 4, port ) ;
			ret = vms_task_auto_save() ;
			if( ret == 1 ){
				bit = vms_check_execfile() ;
				if( !( bit & ( 1 << vms_work.cursor_port ) ) || bit == -1 ){
					bit = vms_ports_dow_ck() ;
					if( ( bit & ( 1 << vms_work.cursor_port ) ) && bit != -1 ) goto state_ck ;
					bit = vms_ports_net_ck() ;
					if( !( bit & ( 1 << vms_work.cursor_port ) ) || bit == -1 ){
						Sys->routine_0 ++ ;
						Sys->timer0 = 60 ;
						vms_work.file_flag = 1 ;
						dm11_msg_set( 5, vms_work.cursor_port ) ;
						vms_work.file_port = vms_work.cursor_port ;
						vms_date_keep( vms_work.file_port ) ;
						return ;
					}
				}
			} else if( ret == 0 ){
				Sys->routine_0 = 4 ;
				vms_work.file_flag = 0 ;
				dm11_msg_set( 6, vms_work.cursor_port ) ;
				break ;
			} else if( ret == 2 ){
				st = vms_boot_card_ck() ;
				if( st == 3 || st == 2 ) break ;
				Sys->scene_no = st ;
				Sys->routine_0 = Sys->routine_1 = 0 ;
				break ;
			}
		state_ck :
			vms_boot_state_ck( Sys->scene_no ) ;
			break ;
		case 3 :
		wait :
			if( Sys->timer0 -- == 0 ) goto fade_out ;
			else break ;
		case 4 :
			if( key & KEY_START ){
				Sys->routine_0 ++ ;
				dm11_msg_set( 0, 0 ) ;
				na_req( 2 ) ;
			}
			break ;
		case 5 :
			Sys->scene_no = vms_boot_card_ck() ;
			Sys->routine_0 = 0 ;
			break ;
		}
		break ;
	case 4 :
		switch( Sys->routine_0 ){
		case 0 :
			dm11_msg_set( 8, port ) ;
			Sys->routine_0 ++ ;
		case 1 :
			if( vms_boot_state_ck( Sys->scene_no ) ) break ;
			else if( !( key & KEY_START ) ) break ;
			na_req( 2 ) ;
			Sys->scene_no = 6 ;
			fade_set( 0xff000000, 10, FADE_OUT ) ;
			break ;
		}
		break ;
	fade_out :
		Sys->scene_no = 6 ;
		fade_set( 0xff000000, 10, FADE_OUT ) ;
		break ;
	case 5 :
		switch( Sys->routine_0 ){
		case 0 :
			dm11_msg_set( 7, 0 ) ;
			Sys->routine_0 ++ ;
		case 1 :
			if( vms_boot_state_ck( Sys->scene_no ) ) break ;
			else if( !( key & KEY_START ) ) break ;
			na_req( 2 ) ;
			Sys->scene_no = 6 ;
			fade_set( 0xff000000, 10, FADE_OUT ) ;
			break ;
		}
		break ;
	case 6 :
		if( !fade_cnt() ){
			Sys->task_step ++ ;
			Sys->scene_no = 0 ;
			Sys->routine_0 = 0 ;
			Sys->routine_1 = 0 ;
			Sys->routine_2 = 0 ;
			if( vms_work.disp_adjust_req ){
				vms_work.disp_adjust_req = 0 ;
				disp_adjust_set() ;
			}
		}
		break ;
	default :
		vms_boot_state_ck( Sys->scene_no ) ;
	}
}

/****************************************************************/
/*			The attract's screen after a game: show the points	*/
/*			earned, add them and save them to the card			*/
/****************************************************************/
void vms_point_save( void )
{
	int ret ;

	if( 0 ) ret = Sys->scene_no ;	/* left out of the build */
	menu_sw_get( 3 ) ;
	switch( Sys->scene_no ){
	case 0 :
		Sys->scene_no ++ ;
		Sys->routine_0 = 0 ;
		bright_init() ;
		fog_init() ;
		Palette_Init() ;
		Texture_Init() ;
		TexLoad( 16 ) ;
		work_init() ;
		Sys->timer0 = 120 ;
		vms_work.card_task_step[0][0] = vms_work.card_task_step[0][1] = 0 ;
		vms_work.card_task_step[1][0] = vms_work.card_task_step[1][1] = 0 ;
		if( vms_work.d_point && vms_work.n_point ) vms_work.vm_msg[0] = 3 ;
		else if( vms_work.d_point ) {
			vms_work.vm_msg[0] = 1 ;
			if( 0 ) vms_work.vm_msg[0] = 1 ;	/* left out of the build */
		}
		else if( !vms_work.n_point ) goto none ;
		else vms_work.vm_msg[0] = 2 ;
		dm0c_vm_msg_set() ;
		vms_work.vm_msg[1] = 1 ;
		dm0c_vm_win_set( vms_work.vm_msg[1] ) ;
		goto wall ;
	none :
		if( vms_work.file_flag == 1 ){
			Sys->timer0 = 1 ;
			goto wall ;
		} else goto end ;
	wall :
		dm0c_wall_paper( 9 ) ;
		fade_set( 0xff000000, 10, FADE_IN ) ;
	case 1 :
		if( !fade_cnt() ) Sys->scene_no ++ ;
		break ;
	case 2 :
		if( cancel_ck() ) Sys->timer0 = 1 ;
		if( Sys->timer0 -- == 0 ){
			if( vms_work.d_point ) d_point_get() ;
			if( vms_work.n_point ) n_point_get() ;
			if( vms_work.file_flag == 1 ){
				Sys->scene_no ++ ;
				vms_work.vm_msg[0] = 4 ;
				dm0c_vm_msg_set() ;
				vms_work.vm_msg[1] = 2 ;
				dm0c_vm_win_set( vms_work.vm_msg[1] ) ;
			} else {
				Sys->scene_no = 6 ;
				fade_set( 0xff000000, 10, FADE_OUT ) ;
			}
		}
		break ;
	case 3 :
		ret = vms_task_save_all( vms_work.file_port, 0 ) ;
		if( ret == 2 ) break ;
		else if( ret == 1 ){
			Sys->scene_no = 4 ;
			Sys->timer0 = 60 ;
			vms_work.vm_msg[0] = 5 ;
		} else if( ret == 0 ){
			Sys->scene_no = 5 ;
			vms_work.vm_msg[0] = 6 ;
		} else {
			if( ret != 3 ) break ;
			Sys->scene_no = 5 ;
			vms_work.vm_msg[0] = 7 ;
		}
		dm0c_vm_msg_set() ;
		return ;
	case 4 :
		if( decide_cancel_ck() ) Sys->timer0 = 1 ;
		if( Sys->timer0 -- == 0 ){
			Sys->scene_no = 6 ;
			fade_set( 0xff000000, 10, FADE_OUT ) ;
		}
		break ;
	fade_out :
		Sys->scene_no = 6 ;
		fade_set( 0xff000000, 10, FADE_OUT ) ;
		break ;
	case 5 :
		if( !decide_cancel_ck() ) break ;
		vms_work.file_flag = 0 ;
		Sys->scene_no = 6 ;
		fade_set( 0xff000000, 10, FADE_OUT ) ;
		break ;
	case 6 :
		if( !fade_cnt() ) goto end ;
		break ;
	end :
		Sys->task_step = 0 ;
		Sys->scene_no = 0 ;
		Sys->routine_0 = 0 ;
		Sys->ended_mode = -1 ;
		break ;
	}
}

/****************************************************************/
/*			Count the time played, a second at a time, up to	*/
/*			99:59:59											*/
/****************************************************************/
void play_time_ck( void )
{
	tmr_count = syTmrGenGetCount() ;
	if( tmr_count < 0xbebc ) return ;
	tmr_count = 0 ;
	syTmrGenSetCount( 0 ) ;
	if( CS_Secret_Work.play_hour == 99 && CS_Secret_Work.play_min == 59 && CS_Secret_Work.play_sec == 59 ) return ;
	CS_Secret_Work.play_sec ++ ;
	if( CS_Secret_Work.play_sec >= 60 ){
		CS_Secret_Work.play_sec = 0 ;
		CS_Secret_Work.play_min ++ ;
		if( CS_Secret_Work.play_min >= 60 ){
			CS_Secret_Work.play_min = 0 ;
			CS_Secret_Work.play_hour ++ ;
			if( CS_Secret_Work.play_hour > 99 ){
				CS_Secret_Work.play_hour = 99 ;
				CS_Secret_Work.play_min = 59 ;
				CS_Secret_Work.play_sec = 59 ;
			}
		}
	}
}

/****************************************************************/
/*			The secret factor's work as a new game has it		*/
/****************************************************************/
void secret_work_init( void )
{
	CS_Secret_Work.vm_level = 1 ;
	CS_Secret_Work.vm_exp = 0 ;
	CS_Secret_Work.d_pts = 0 ;
	CS_Secret_Work.unk_04 = 0 ;
	CS_Secret_Work.n_pts = 0 ;
	CS_Secret_Work.unk_08 = 0 ;
	CS_Secret_Work.unk_0a = 0 ;
	CS_Secret_Work.unk_0c = 0 ;
	CS_Secret_Work.unk_10 = 0 ;
	CS_Secret_Work.unk_11 = 2 ;
	CS_Secret_Work.play_hour = 0 ;
	CS_Secret_Work.play_min = 0 ;
	CS_Secret_Work.play_sec = 0 ;
}

/****************************************************************/
/*			Points earned in a game, kept until it is over:		*/
/*			0 to 9999											*/
/****************************************************************/
void d_point_add( int pt )
{
	vms_work.d_point += pt ;
	if( vms_work.d_point < 0 ) vms_work.d_point = 0 ;
	if( vms_work.d_point > 9999 ) vms_work.d_point = 9999 ;
}

/****************************************************************/
/*			The points earned go to the secret factor: a		*/
/*			tenth of them raises the level, 100 a level			*/
/****************************************************************/
void d_point_get( void )
{
	int exp ;

	CS_Secret_Work.d_pts += vms_work.d_point ;
	if( CS_Secret_Work.d_pts > 9999 ) CS_Secret_Work.d_pts = 9999 ;
	exp = CS_Secret_Work.vm_exp ;
	exp += vms_work.d_point / 10 ;
	while( exp >= 100 ){
		CS_Secret_Work.vm_level ++ ;
		exp -= 100 ;
		if( CS_Secret_Work.vm_level > 99 ){
			CS_Secret_Work.vm_level = 99 ;
			exp = 99 ;
		}
	}
	CS_Secret_Work.vm_exp = exp ;
	vms_work.d_point = 0 ;
}

/****************************************************************/
/*			Points of the second kind							*/
/****************************************************************/
void n_point_add( int pt )
{
	vms_work.n_point += pt ;
	if( vms_work.n_point < 0 ) vms_work.n_point = 0 ;
	if( vms_work.n_point > 9999 ) vms_work.n_point = 9999 ;
}

/****************************************************************/
/*			And they go to the secret factor: a quarter of		*/
/*			them raises the level								*/
/****************************************************************/
void n_point_get( void )
{
	int exp ;

	CS_Secret_Work.n_pts += vms_work.n_point ;
	if( CS_Secret_Work.n_pts > 9999 ) CS_Secret_Work.n_pts = 9999 ;
	exp = CS_Secret_Work.vm_exp ;
	exp += vms_work.n_point / 4 ;
	while( exp >= 100 ){
		CS_Secret_Work.vm_level ++ ;
		exp -= 100 ;
		if( CS_Secret_Work.vm_level > 99 ){
			CS_Secret_Work.vm_level = 99 ;
			exp = 99 ;
		}
	}
	CS_Secret_Work.vm_exp = exp ;
	vms_work.n_point = 0 ;
}

/****************************************************************/
/*			The ports with a card, for the card screens			*/
/****************************************************************/
void vms_card_bit_set( void )
{
	vms_work.card_bit = vms_ports_card_now_ck() ;
}

/****************************************************************/
/*			Point at a port to start with: the game's own		*/
/*			card, or the first there is; 0 when there is none	*/
/****************************************************************/
int vms_port_first( void )
{
	int bit ;
	int i ;

	vms_work.cursor_port = -1 ;
	if( bit = vms_work.card_bit, vms_work.file_flag == 1 ){
		if( 0 ){	/* a check left out of the build */
			vms_card_bit_set() ;
			return bit ;
		}
		if( bit == -1 ) return 0 ;
		if( bit & ( 1 << vms_work.file_port ) ){
			vms_work.cursor_port = vms_work.file_port ;
			return 1 ;
		}
	}
	if( bit == -1 ) return 0 ;
	if( !bit ) return 0 ;
	i = 0 ;
	while( !( bit & ( 1 << i ) ) && i < 8 ) i ++ ;
	if( i == 8 ) return 0 ;
	vms_work.cursor_port = i ;
	return 1 ;
}

/****************************************************************/
/*			The lever moves the card screens' pointer: left		*/
/*			and right between the ports, up and down between	*/
/*			a port's two sockets; a port without a card is		*/
/*			passed over											*/
/****************************************************************/
void vms_port_move( Uint16 key )
{
	Sint32 port ;
	int i ;

	port = vms_work.cursor_port ;
	if( port != -1 ){
		if( key & ( KEY_UP | KEY_DOWN ) ){
			na_req( 1 ) ;
			if( port % 2 == 0 ) port = ( port + 1 ) & 7 ;
			else port = ( port - 1 ) & 7 ;
		} else if( key & KEY_LEFT ){
			na_req( 1 ) ;
			if( port == 0 || port == 1 ) port += 6 ;
			else port -= 2 ;
		} else if( key & KEY_RIGHT ){
			na_req( 1 ) ;
			if( port == 6 || port == 7 ) port -= 6 ;
			else port += 2 ;
		}
	}
	i = 0 ;
	while( !( vms_work.card_bit & ( 1 << port ) ) && port != -1 && i < 8 ){
		if( key & KEY_LEVER ){
			if( key & KEY_UP ) port = port_up_tbl[vms_work.cursor_port][i] ;
			else if( key & KEY_DOWN ) port = port_down_tbl[vms_work.cursor_port][i] ;
			else if( key & KEY_RIGHT ) port = port_right_tbl[vms_work.cursor_port][i] ;
			else if( key & KEY_LEFT ) port = port_left_tbl[vms_work.cursor_port][i] ;
		} else port = port_near_tbl[vms_work.cursor_port][i] ;
		i ++ ;
	}
	vms_work.cursor_port = port ;
}

/****************************************************************/
/*			The buttons that decide on a side's pad: A, and		*/
/*			START on a pad that has one; in a network game		*/
/*			only this console's pad								*/
/****************************************************************/
Uint16 decide_sw_get( Sint8 side )
{
	Uint16 sw ;

	if( port_peri[side]->support & ( PDD_DGT_TC | PDD_DGT_TZ ) ) sw = KEY_LK | KEY_A2 ;
	else sw = KEY_LK ;
	if( Sys->home_mode == HOME_NETWORK && !( net_pad_bit & ( 1 << side ) ) ) sw = 0 ;
	return sw ;
}

/****************************************************************/
/*			A menu's choice is decided, with its sound			*/
/****************************************************************/
int decide_ck( void )
{
	Uint16 sw0, sw1 ;

	sw0 = decide_sw_get( 0 ) ;
	sw1 = decide_sw_get( 1 ) ;
	if( ( sw_work[0].on & sw0 ) || ( sw_work[1].on & sw1 ) ){
		na_req( 2 ) ;
		return 1 ;
	}
	return 0 ;
}

/****************************************************************/
/*			And without it										*/
/****************************************************************/
int decide_ck_nose( void )
{
	Uint16 sw0, sw1 ;

	sw0 = decide_sw_get( 0 ) ;
	sw1 = decide_sw_get( 1 ) ;
	if( ( sw_work[0].on & sw0 ) || ( sw_work[1].on & sw1 ) ) return 1 ;
	return 0 ;
}

/****************************************************************/
/*			Decided, by a pad or the keyboard					*/
/****************************************************************/
int decide_kbd_ck( void )
{
	Uint16 sw0, sw1 ;

	sw0 = decide_sw_get( 0 ) ;
	sw1 = decide_sw_get( 1 ) ;
	if( ( sw_work[0].on & sw0 ) || ( sw_work[1].on & sw1 ) || ( kbd_on & KEY_LK ) ){
		na_req( 2 ) ;
		return 1 ;
	}
	return 0 ;
}

/****************************************************************/
int decide_kbd_ck_nose( void )
{
	Uint16 sw0, sw1 ;

	sw0 = decide_sw_get( 0 ) ;
	sw1 = decide_sw_get( 1 ) ;
	if( ( sw_work[0].on & sw0 ) || ( sw_work[1].on & sw1 ) || ( kbd_on & KEY_LK ) ) return 1 ;
	return 0 ;
}

/****************************************************************/
/*			A menu is cancelled (B), with its sound; in a		*/
/*			network game only by this console's pad				*/
/****************************************************************/
int cancel_ck( void )
{
	if( Sys->home_mode == HOME_NETWORK ){
		if( net_pad_bit & 1 ){
			if( sw_work[0].on & KEY_HK ) goto hit ;
		} else {
			if( sw_work[1].on & KEY_HK ) goto hit ;
		}
	} else if( ( sw_work[0].on | sw_work[1].on ) & KEY_HK ) goto hit ;
	return 0 ;
hit :
	na_req( 3 ) ;
	return 1 ;
}

/****************************************************************/
int cancel_ck_nose( void )
{
	if( Sys->home_mode == HOME_NETWORK ){
		if( net_pad_bit & 1 ){
			if( sw_work[0].on & KEY_HK ) goto hit ;
		} else {
			if( sw_work[1].on & KEY_HK ) goto hit ;
		}
	} else if( ( sw_work[0].on | sw_work[1].on ) & KEY_HK ) goto hit ;
	return 0 ;
hit :
	return 1 ;
}

/****************************************************************/
/*			Cancelled, by a pad or the keyboard					*/
/****************************************************************/
int cancel_kbd_ck( void )
{
	if( Sys->home_mode == HOME_NETWORK ){
		if( net_pad_bit & 1 ){
			if( ( sw_work[0].on | kbd_on ) & KEY_HK ) goto hit ;
		} else {
			if( ( sw_work[1].on | kbd_on ) & KEY_HK ) goto hit ;
		}
	} else if( ( sw_work[0].on | sw_work[1].on | kbd_on ) & KEY_HK ) goto hit ;
	return 0 ;
hit :
	na_req( 3 ) ;
	return 1 ;
}

/****************************************************************/
int cancel_kbd_ck_nose( void )
{
	if( Sys->home_mode == HOME_NETWORK ){
		if( net_pad_bit & 1 ){
			if( ( sw_work[0].on | kbd_on ) & KEY_HK ) goto hit ;
		} else {
			if( ( sw_work[1].on | kbd_on ) & KEY_HK ) goto hit ;
		}
	} else if( ( sw_work[0].on | sw_work[1].on | kbd_on ) & KEY_HK ) goto hit ;
	return 0 ;
hit :
	return 1 ;
}

/****************************************************************/
/*			Either ends a message								*/
/****************************************************************/
int decide_cancel_ck( void )
{
	if( decide_ck_nose() ) return 1 ;
	return cancel_ck_nose() ;
}

/****************************************************************/
/*			Each side's buttons as its key config has them,		*/
/*			for its three players								*/
/****************************************************************/
void pl_sw_config( void )
{
	pl_sw_config_sub( 0 ) ;
	pl_sw_config_sub( 1 ) ;
	pl_work[2].pad_sw = pl_work[0].pad_sw ;
	pl_work[3].pad_sw = pl_work[1].pad_sw ;
	pl_work[4].pad_sw = pl_work[0].pad_sw ;
	pl_work[5].pad_sw = pl_work[1].pad_sw ;
}

/****************************************************************/
void pl_sw_config_sub( int side )
{
	int i ;
	Uint16 sw ;
	Uint16 no ;
	Uint16 *conf ;

	for( i = 0 ; i < 6 ; i ++ ) key_on[side][i] = sw_work[side].now & key_bit_tbl[i] ;
	conf = option_w.key_config[side] ;
	sw = 0 ;
	for( i = 0 ; i < 6 ; i ++ ){
		if( key_on[side][i] ){
			no = conf[i] ;
			sw |= key_func_tbl[no] ;
		}
	}
	pl_work[side].pad_sw.old = pl_work[side].pad_sw.now ;
	pl_work[side].pad_sw.now = sw | ( sw_work[side].now & ( KEY_START | KEY_LEVER ) ) ;
	pl_work[side].pad_sw.on = ~pl_work[side].pad_sw.old & pl_work[side].pad_sw.now ;
	pl_work[side].pad_sw.off = ~pl_work[side].pad_sw.now & pl_work[side].pad_sw.old ;
}

/****************************************************************/
void pl_sw_config_sub2( int side )
{
	int i ;
	Uint16 sw ;
	Uint16 no ;
	Uint16 *conf ;

	for( i = 0 ; i < 6 ; i ++ ) key_on[side][i] = sw_work[side].now & key_bit_tbl[i] ;
	conf = button_cfg[side] ;
	sw = 0 ;
	for( i = 0 ; i < 6 ; i ++ ){
		if( key_on[side][i] ){
			no = conf[i] ;
			sw |= key_func_tbl[no] ;
		}
	}
	pl_work[side].pad_sw.old = pl_work[side].pad_sw.now ;
	pl_work[side].pad_sw.now = sw | ( sw_work[side].now & ( KEY_START | KEY_LEVER ) ) ;
	pl_work[side].pad_sw.on = ~pl_work[side].pad_sw.old & pl_work[side].pad_sw.now ;
	pl_work[side].pad_sw.off = ~pl_work[side].pad_sw.now & pl_work[side].pad_sw.old ;
}
