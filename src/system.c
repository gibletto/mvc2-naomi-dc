/* The system: the start-up of the libraries and the screen, the controls read from the JAMMA I/O
   board (or a Dreamcast pad) each vertical blank, the backup data and the coin and credit display. */
#include "player.h"
#include "book.h"
#include "naomi.h"
#include "shinobi.h"

/* SHC's built-ins, as its smachine.h names them */
#define set_imask( mask )	_builtin_set_imask( mask )
#define get_imask()	_builtin_get_imask()

/* Shinobi / Kamui */
extern void syHwInit_NAOMI( void ) ;
extern void syHwInit2( void ) ;
extern Sint32 nlsndInit( void *Address, Sint32 BoardType ) ;
extern Uint32 syTmrGenGetCount( void ) ;
extern void syTmrGenSetCount( Uint32 count ) ;
extern int sprintf( char *s, const char *format, ... ) ;

extern void com_sound_load() ;	/* the arcade passes it a 0 it doesn't read */
extern void sound_init( void ) ;
extern void na_first_req( void ) ;
extern void file_load( Sint32 no, void *adrs, Sint32 a ) ;
extern void sys_open_init( void ) ;
extern void point_add( int a ) ;

/* the peripheral library's receive and send buffers, and the timer's count */
static Uint8 peri_recv_buff[0xc020] ;
static Uint8 peri_send_buff[0xc020] ;
static Uint32 tmr_count ;

static void pad_read( int side ) ;
static void jamma_read( void ) ;
static void backup_init( void ) ;
static void sys_set_init( void ) ;
void sys_set_load( void ) ;

static Uint16 lever_tbl[8] = {
	KEY_DOWN, KEY_DOWN | KEY_RIGHT, KEY_RIGHT, KEY_UP | KEY_RIGHT, KEY_UP, KEY_UP | KEY_LEFT, KEY_LEFT, KEY_DOWN | KEY_LEFT
} ;

/*==============================================================*/
/*			A Dreamcast pad's buttons and stick, as the			*/
/*			game's keys											*/
/*==============================================================*/
static void pad_read( int side )
{
	Uint32 on ;
	Uint16 sw ;
	Uint8 dir ;
	Float x, y ;
	SW_WORK *kw ;
	Sint32 ang ;

	if( rom_dma_flag ) return ;
	if( conf_vms_mode != 2 ) return ;
	if( !side ) pad_peri[side] = pdGetPeripheral( 6 ) ;
	else pad_peri[side] = pdGetPeripheral( 12 ) ;
	if( pad_peri[side]->info->type & PDD_DEVTYPE_CONTROLLER ){
		sw_work[side].trg_l = pad_peri[side]->l ;
		sw_work[side].trg_r = pad_peri[side]->r ;
		sw_work[side].stick_x = pad_peri[side]->x1 ;
		sw_work[side].stick_y = pad_peri[side]->y1 ;
		on = pad_peri[side]->on ;
		kw = ( SW_WORK * )( ( Uint32 )sw_work + side * 20 ) ;
		if( sw_work[side].now & KEY_LEVER ) sw = kw->now ;
		else {
			sw = 0 ;
			if( on & PDD_DGT_KU ) sw = KEY_UP ;
			if( on & PDD_DGT_KD ) sw |= KEY_DOWN ;
			if( on & PDD_DGT_KL ) sw |= KEY_LEFT ;
			if( on & PDD_DGT_KR ) sw |= KEY_RIGHT ;
			if( !sw ){
				x = kw->stick_x ;
				y = kw->stick_y ;
				if( x * x + y * y > 6400.0f ){
					ang = nlArcTan2( x, y ) ;
					dir = ( Uint16 )( ang + 0x1000 ) / 0x2000 & 7 ;
					sw |= lever_tbl[dir] ;
				}
			}
			sw |= sw_work[side].now & ( KEY_BUTTON | KEY_START ) ;
			if( 0 ) sw |= kw->now & sw_work[side].now ;
		}
		if( on & PDD_DGT_ST ) sw |= KEY_START ;
		if( on & PDD_DGT_TX ) sw |= 0x200 ;
		if( on & PDD_DGT_TY ) sw |= 0x100 ;
		if( on & PDD_DGT_TL ) sw |= 0x80 ;
		if( on & PDD_DGT_TA ) sw |= 0x40 ;
		if( on & PDD_DGT_TB ) sw |= 0x20 ;
		if( on & PDD_DGT_TR ) sw |= 0x10 ;
		if( on & PDD_DGT_TZ ) sw |= 0x80 ;
		if( on & PDD_DGT_TC ) sw |= 0x10 ;
		sw_work[side].now = sw ;
	}
}

/*==============================================================*/
/* read the switches of the I/O boards: with one board both sides are on it, with two each has its own
   (two of its statements have no effect) */
static void jamma_read( void )
{
	char *in ;
	char *sw ;
	char *analog ;
	char *sw2 ;
	char *analog2 ;
	char c, hi, lo ;
	Uint16 sys, sys2 ;
	SW_WORK *kw ;

	in = ( char * )&nlaInput6149[5] ;
	kw = &board_sw ;
	c = *in ;
	kw->old = kw->now ;
	kw->now = ~( Uint8 )c & 0xff ;
	kw->on = ~kw->old & kw->now ;
	kw->off = kw->old & ~kw->now ;
	if( !jamma_cnt ) return ;
	sw = ( in = nlaJAMMAInput[0] ) + 512 ;
	sw += nlaJAMMAAcknowledgeIndex[1][NLA_JAMMA_COMMAND_SWITCH_READ] ;
	analog = sw ;
	analog += nlaJAMMAAcknowledgeIndex[1][NLA_JAMMA_COMMAND_ANALOG_READ] ;
	if( jamma_cnt == 1 ){
		if( *analog == NLA_JAMMA_REPORT_OK ){
			sw_work[0].stick_x = 0 ;
			sw_work[0].stick_y = 0 ;
			sw_work[0].stick_y ;
			sw_work[1].stick_x = 0 ;
			sw_work[1].stick_y = 0 ;
		}
		if( *sw == NLA_JAMMA_REPORT_OK ){
			sw ++ ;
			c = *sw ++ ;
			sys_sw.old = sys_sw.now ;
			sys_sw.now = ( Uint8 )c ;
			sys_sw.on = ~sys_sw.old & sys_sw.now ;
			sys_sw.off = sys_sw.old & ~sys_sw.now ;
			hi = *sw ++ ;
			lo = *sw ++ ;
			if( 0 ) hi = 0 ;
			sw_work[0].old = sw_work[0].now ;
			sw_work[0].now = ( ( Uint16 )hi << 8 & 0xff00 ) + ( Uint8 )lo ;
			pad_read( 0 ) ;
			sw_work[0].on = ~sw_work[0].old & sw_work[0].now ;
			sw_work[0].off = sw_work[0].old & ~sw_work[0].now ;
			hi = *sw ++ ;
			lo = *sw ;
			sw_work[1].old = sw_work[1].now ;
			sw_work[1].now = ( ( Uint16 )hi << 8 & 0xff00 ) + ( Uint8 )lo ;
			pad_read( 1 ) ;
			sw_work[1].on = ~sw_work[1].old & sw_work[1].now ;
			sw_work[1].off = sw_work[1].old & ~sw_work[1].now ;
		} else {
			sys_sw.old = sys_sw.now ;
			sys_sw.on = ~sys_sw.old & sys_sw.now ;
			sys_sw.off = sys_sw.old & ~sys_sw.now ;
			sw_work[0].old = sw_work[0].now ;
			sw_work[0].on = ~sw_work[0].old & sw_work[0].now ;
			sw_work[0].off = sw_work[0].old & ~sw_work[0].now ;
			sw_work[1].old = sw_work[1].now ;
			sw_work[1].on = ~sw_work[1].old & sw_work[1].now ;
			sw_work[1].off = sw_work[1].old & ~sw_work[1].now ;
		}
	} else {
		sw2 = in + 1024 ;
		sw2 += nlaJAMMAAcknowledgeIndex[2][NLA_JAMMA_COMMAND_SWITCH_READ] ;
		analog2 = sw2 ;
		analog2 += nlaJAMMAAcknowledgeIndex[2][NLA_JAMMA_COMMAND_ANALOG_READ] ;
		if( *analog2 == NLA_JAMMA_REPORT_OK ){
			sw_work[0].stick_x = 0 ;
			sw_work[0].stick_y = 0 ;
		}
		if( *sw2 == NLA_JAMMA_REPORT_OK ){
			sw2 ++ ;
			c = *sw2 ++ ;
			sys = ( Uint8 )c ;
			hi = *sw2 ++ ;
			lo = *sw2 ;
			sw_work[0].old = sw_work[0].now ;
			sw_work[0].now = ( ( Uint16 )hi << 8 & 0xff00 ) + ( Uint8 )lo ;
			pad_read( 0 ) ;
			sw_work[0].on = ~sw_work[0].old & sw_work[0].now ;
			sw_work[0].off = sw_work[0].old & ~sw_work[0].now ;
		} else {
			sys = 0 ;
			sw_work[0].old = sw_work[0].now ;
			sw_work[0].on = ~sw_work[0].old & sw_work[0].now ;
			sw_work[0].off = sw_work[0].old & ~sw_work[0].now ;
		}
		if( *analog == NLA_JAMMA_REPORT_OK ){
			sw_work[1].stick_x = 0 ;
			sw_work[1].stick_y = 0 ;
		}
		if( *sw == NLA_JAMMA_REPORT_OK ){
			sw ++ ;
			c = *sw ++ ;
			sys2 = ( Uint8 )c ;
			hi = *sw ++ ;
			lo = *sw ;
			sw_work[1].old = sw_work[1].now ;
			sw_work[1].now = ( ( Uint16 )hi << 8 & 0xff00 ) + ( Uint8 )lo ;
			pad_read( 1 ) ;
			sw_work[1].on = ~sw_work[1].old & sw_work[1].now ;
			sw_work[1].off = sw_work[1].old & ~sw_work[1].now ;
		} else {
			sys2 = 0 ;
			sw_work[1].old = sw_work[1].now ;
			sw_work[1].on = ~sw_work[1].old & sw_work[1].now ;
			sw_work[1].off = sw_work[1].old & ~sw_work[1].now ;
		}
		sys |= sys2 ;
		sys_sw.old = sys_sw.now ;
		sys_sw.now = sys ;
		sys_sw.on = ~sys_sw.old & sys_sw.now ;
		sys_sw.off = sys_sw.old & ~sys_sw.now ;
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
/*			The vertical blank: count the frame and read the	*/
/*			controls											*/
/****************************************************************/
Sint32 VBlankInIrq( void )
{
	Sys->vblank_cnt ++ ;
	vsync_count ++ ;
	unk_0C2D96AB = 0 ;
	if( task_wait ) task_wait -- ;
	nlaPutLED( Sys->vblank_cnt ) ;
	if( !rom_dma_flag ) pdExecPeripheralServer() ;
	jamma_read() ;
	return 0 ;
}

/****************************************************************/
/*			Set the system up: the hardware, the libraries,		*/
/*			the screen, the controls, the backup data, the		*/
/*			first files and sounds and the interrupts; test		*/
/*			is set when the test menu is wanted					*/
/****************************************************************/
void system_init( int test )
{
	Float work[129] ;
	int i ;
	NLA_JAMMA_INFO *info ;

	set_imask( 14 ) ;
	syHwInit_NAOMI() ;
	nlaGetBootFlag() ;
	sys_flag = nlaGetSystemFlag() ;
	if( sys_flag->DisplayMode == NLA_DISPLAY_31K ) nlInitLibrary( 0, 0x80, 1 ) ;
	else nlInitLibrary( 9, 0x80, 1 ) ;
	nlSysSetTextureSize( 0x300000 ) ;
	set_imask( 14 ) ;
	syHwInit2() ;
	set_imask( 0 ) ;
	nlSysMetricsInit() ;
	nlSysUseLatency2V() ;
	nlSysSetDirectListTypeLatency2V( 0 ) ;
	nlSysSetVertexBufferBaseAddress( ( void * )0x0cf00000 ) ;
	nlSysCreatePunchBuffer( 0x10000 ) ;
	nlCreateVertexBuffer( 0, 0x4000, 0x24000, 0x4000 ) ;
	nlSysSetTimeOutCount( 10, 0 ) ;
	nlInitPrint() ;
	nlPrnIndentOn() ;
	nlSetBackColor( 0, 0, 0 ) ;
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
	nlSetFontSize( 16, 16 ) ;
	if( !test ){
		nlLocate( 12, 14 ) ;
		nlPrintf( "LOADING GAME NOW...\n" ) ;
	} else {
		nlLocate( 8, 10 ) ;
		nlPrintf( "LOADING GAME TEST NOW...\n" ) ;
	}
	nlWaitVSync() ;
	i = 2000000 ;
	while( i -- ) ;
	nlSetFontSize( 8, 8 ) ;
	set_imask( 14 ) ;
	pdInitPeripheral( 0, peri_recv_buff, peri_send_buff ) ;
	nlaInitSystem() ;
	nlaJAMMAInit() ;
	nlaJAMMACreditInit() ;
	jamma_cnt = nlaJAMMANodes ;
	if( !jamma_cnt ) jamma_err = 1 ;
	else {
		jamma_err = 0 ;
		nlaJAMMASetPrimaryNode( 1 ) ;
		info = nlaJAMMAInfo ;
		if( info[1].Switch[0] < 2 || info[1].Switch[1] < 10 || info[1].Coin < 2 ) jamma_err = 2 ;
		if( jamma_cnt == 2 ){
			if( info[2].Switch[0] < 2 || info[2].Switch[1] < 10 || info[2].Coin < 2 ) jamma_err = 2 ;
		}
	}
	set_imask( 0 ) ;
	if( test ) nlaInTestMode() ;
	nlaROMTypeSet( 1, 0 ) ;
	credit_conf = nlaGetCreditConf() ;
	nlaGetCreditData( &credit_info ) ;
	backup_init() ;
	sys_set_load() ;
	sys_open_init() ;
	file_load( 0, ( void * )0x0cc00000, 1 ) ;
	nlsndInit( ( void * )0x0cc00000, 0x200 ) ;
	com_sound_load( 0 ) ;
	sound_init() ;
	nlObjPutInit() ;
	nlObjPutInitTexture() ;
	nlObjPutMvInit( 0x80 ) ;
	nlObjPutMvSetMode( 1 ) ;
	nlDispStripInit() ;
	rom_dma_flag = 1 ;
	cache_inv( ( void * )0x0dd00000, 0xf0000 ) ;
	nlaReadROMDMA( 0x250000, ( char * )0x0dd00000, 0xf0000, 1 ) ;
	file_load( 0x32e, ( void * )0x0ce00000, 0 ) ;
	file_load( 0x32d, ( void * )0x0ce08000, 0 ) ;
	thd_file_load( 1, ( void * )0x0ce0a000 ) ;
	thd_file_load( 2, ( void * )0x0ce1d000 ) ;
	rom_dma_flag = 0 ;
	nlInitInterrupt() ;
	nlSetIntrFunction( 1, VBlankInIrq ) ;
	nlSetIntrFunction( 0, vblank_func ) ;
}

/****************************************************************/
/*			The test button, of the I/O board or of the			*/
/*			NAOMI board: stop the sound and go to the test		*/
/*			menu												*/
/****************************************************************/
int test_sw_ck( void )
{
	int ret = 0 ;

	if( ( sys_sw.now & 0x80 ) || ( board_sw.now & 0x10 ) ){
		task_wait = 3 ;
		Sys->play_mode = 0x80 ;
		Sys->task_no = 3 ;
		Sys->task_step = 0 ;
		Sys->scene_no = 0 ;
		Sys->routine_0 = 0 ;
		Sys->routine_1 = 0 ;
		Sys->routine_2 = 0 ;
		Sys->routine_3 = 0 ;
		stream_stop() ;
		sound_all_off() ;
		se_all_off() ;
		se_vol_up( 0, 1 ) ;
		se_vol_up( 1, 1 ) ;
		s3l_fade_in_set( 1 ) ;
		ret = 1 ;
	}
	return ret ;
}

/****************************************************************/
/*			Go to the boot ROM's test menu						*/
/****************************************************************/
void test_mode_jump( void )
{
	volatile int i ;

	nlSetFontSize( 16, 16 ) ;
	nlLocate( 8, 10 ) ;
	nlPrintColor( -1 ) ;
	nlSetBackColor( 0, 0, 0 ) ;
	nlPrintf( "LOADING TEST MODE NOW...\n" ) ;
	nlWaitVSync() ;
	i = 2000000 ;
	while( i -- ) ;
	nlInitInterrupt() ;
	nlSetIntrFunction( 1, 0 ) ;
	i = 16500000 ;
	while( i -- ) ;
	nlSetFontSize( 8, 8 ) ;
	nlaJumpTestMode() ;
}

/****************************************************************/
/* both start buttons and the first side's second button held with the service switch clears the
   book-keeping */
void book_clear_ck( void )
{
	if( ( sw_work[0].now & KEY_START ) && ( sw_work[1].now & KEY_START )
	 && ( sw_work[0].now & KEY_LP ) && ( board_sw.now & 0x20 ) ){
		book.coin_all = 0 ;
		book.service_all = 0 ;
		book.play_all = 0 ;
		book.coin = 0 ;
		book.service = 0 ;
		book.play = 0 ;
		book.point = 0 ;
		book.char_open[0] = 0 ;
		book.char_open[1] = 0 ;
		book.char_open_all[0] = 0 ;
		book.char_open_all[1] = 0 ;
		book.play_time = 0 ;
		book_save = 1 ;
	}
}

/****************************************************************/
void book_count_clear( void )
{
	book.coin = 0 ;
	book.service = 0 ;
	book.play = 0 ;
	book_save = 1 ;
}

/****************************************************************/
/*			Count a game played									*/
/****************************************************************/
void book_play_add( void )
{
	book.play ++ ;
	book.play_all ++ ;
	book_save = 1 ;
}

/****************************************************************/
/*			Count the coins and service credits put in since	*/
/*			the last frame, and save the book-keeping			*/
/****************************************************************/
void book_coin_ck( void )
{
	NLA_BACKUP_BANK bank ;

	nlaGetBackupData( &bank ) ;
	if( coin_old != bank.credit.TotalCoin ){
		if( Sys->cpu_level_type ) point_add( 1 ) ;
		book.coin += bank.credit.TotalCoin - coin_old ;
		book.coin_all += bank.credit.TotalCoin - coin_old ;
		coin_old = bank.credit.TotalCoin ;
		book_save = 1 ;
	}
	if( service_old != bank.credit.ServiceCredit ){
		if( Sys->cpu_level_type ) point_add( 1 ) ;
		book.service += bank.credit.ServiceCredit - service_old ;
		book.service_all += bank.credit.ServiceCredit - service_old ;
		service_old = bank.credit.ServiceCredit ;
		book_save = 1 ;
	}
	nlaUpdateSystemBackup() ;
	if( book_save ){
		nlaUpdateUserBackup() ;
		if( book_save == 1 ){
			if( credit_conf->CoinFlag != NLA_FREEPLAY ) na_first_req() ;
		}
	}
	book_save = 0 ;
	Sys->st_demo_flag = 0 ;
}

/*==============================================================*/
/*			The book-keeping in the backup memory: start it		*/
/*			again if it is new or not the game's				*/
/*==============================================================*/
static void backup_init( void )
{
	NLA_BACKUP_BANK bank ;
	int ret ;

	book.id = 0 ;
	ret = nlaInitUserBackup( 64, ( char * )&book ) ;
	if( ret == 3 || book.id != BOOK_ID ){
		book.id = BOOK_ID ;
		book.coin = 0 ;
		book.service = 0 ;
		book.play = 0 ;
		book.coin_all = 0 ;
		book.service_all = 0 ;
		book.play_all = 0 ;
		book.point = 0 ;
		book.char_open[0] = 0 ;
		book.char_open[1] = 0 ;
		book.char_open_all[0] = 0 ;
		book.char_open_all[1] = 0 ;
		book.play_time = 0 ;
		nlaUpdateUserBackup() ;
	}
	book_save = 0 ;
	nlaGetBackupData( &bank ) ;
	coin_old = bank.credit.TotalCoin ;
	service_old = bank.credit.ServiceCredit ;
}

/* the settings, copied a member at a time */
#define SYS_SET_COPY( dst, src )	\
	( dst )->id = ( src )->id ; ( dst )->country = ( src )->country ;	\
	( dst )->stereo = ( src )->stereo ; ( dst )->demo_sound_half = ( src )->demo_sound_half ;	\
	( dst )->cont_enable = ( src )->cont_enable ; ( dst )->voice_type = ( src )->voice_type ;	\
	( dst )->difficulty = ( src )->difficulty ; ( dst )->damage_level = ( src )->damage_level ;	\
	( dst )->time_speed = ( src )->time_speed ; ( dst )->vms_mode = ( src )->vms_mode ;	\
	( dst )->vm_write = ( src )->vm_write ; ( dst )->speed_fix = ( src )->speed_fix ;	\
	( dst )->event_flag = ( src )->event_flag ; ( dst )->no_challenge_flag = ( src )->no_challenge_flag ;	\
	( dst )->open_lv = ( src )->open_lv ;

static const SYS_SET sys_set_tbl[8] = {
	{ BOOK_ID, 0, 1, 0, 1, 0, 1, 1, 1 },
	{ BOOK_ID, 1, 1, 0, 1, 0, 1, 1, 1 },
	{ BOOK_ID, 2, 1, 0, 1, 0, 1, 1, 1 },
	{ BOOK_ID, 3, 1, 0, 1, 0, 3, 1, 1 },
	{ BOOK_ID, 4, 1, 0, 1, 0, 1, 1, 1 },
	{ BOOK_ID, 5, 1, 0, 1, 0, 1, 1, 1 },
	{ BOOK_ID, 6, 1, 0, 1, 0, 1, 1, 1 },
	{ BOOK_ID, 7, 1, 0, 1, 0, 1, 1, 1 },
} ;

/*==============================================================*/
/*			The settings of the country the board is set to		*/
/*==============================================================*/
static void sys_set_init( void )
{
	const SYS_SET *tbl = &sys_set_tbl[sys_flag->Country] ;

	SYS_SET_COPY( &sys_set, tbl )
	conf_country = sys_set.country ;
	conf_stereo = sys_set.stereo ;
	conf_demo_sound_half = sys_set.demo_sound_half ;
	conf_cont_enable = sys_set.cont_enable ;
	conf_voice_type = sys_set.voice_type ;
	conf_difficulty = sys_set.difficulty ;
	conf_damage_level = sys_set.damage_level ;
	conf_time_speed = sys_set.time_speed ;
	conf_vms_mode = sys_set.vms_mode ;
	conf_vm_write = sys_set.vm_write ;
	conf_speed_fix = sys_set.speed_fix ;
	conf_event_flag = sys_set.event_flag ;
	conf_no_challenge_flag = sys_set.no_challenge_flag ;
	conf_open_lv = sys_set.open_lv ;
}

/****************************************************************/
void sys_set_get( SYS_SET *set )
{
	const SYS_SET *tbl = &sys_set_tbl[sys_flag->Country] ;

	SYS_SET_COPY( set, tbl )
}

/****************************************************************/
/*			Set the settings again, and wait with the			*/
/*			interrupts off while they are written				*/
/****************************************************************/
void sys_set_save( void )
{
	volatile int i ;
	int mask ;

	sys_set_init() ;
	mask = get_imask() ;
	set_imask( 14 ) ;
	nlaUpdateUserEEPROM() ;
	i = 20000000 ;
	while( i -- ) ;
	set_imask( mask ) ;
}

/****************************************************************/
/*			Read the settings from the EEPROM; start them		*/
/*			again if they are new, another game's or out of		*/
/*			range												*/
/****************************************************************/
void sys_set_load( void )
{
	int err ;
	int ret ;

	err = 0 ;
	sys_set.id = 0 ;
	ret = nlaInitUserEEPROM( 20, ( char * )&sys_set ) ;
	if( ret == 3 || sys_set.id != BOOK_ID || sys_set.country != sys_flag->Country ) err = 1 ;
	else {
		conf_country = sys_set.country ;
		conf_stereo = sys_set.stereo ;
		conf_demo_sound_half = sys_set.demo_sound_half ;
		conf_cont_enable = sys_set.cont_enable ;
		conf_voice_type = sys_set.voice_type ;
		conf_difficulty = sys_set.difficulty ;
		conf_damage_level = sys_set.damage_level ;
		conf_time_speed = sys_set.time_speed ;
		conf_vms_mode = sys_set.vms_mode ;
		conf_vm_write = sys_set.vm_write ;
		conf_speed_fix = sys_set.speed_fix ;
		conf_event_flag = sys_set.event_flag ;
		conf_no_challenge_flag = sys_set.no_challenge_flag ;
		conf_open_lv = sys_set.open_lv ;
		if( conf_country < 0 || conf_country >= 5 ) err = 1 ;
		if( conf_stereo < 0 || conf_stereo >= 2 ) err = 1 ;
		if( conf_demo_sound_half < 0 || conf_demo_sound_half >= 2 ) err = 1 ;
		if( conf_cont_enable < 0 || conf_cont_enable >= 2 ) err = 1 ;
		if( conf_voice_type < 0 || conf_voice_type >= 2 ) err = 1 ;
		if( conf_difficulty < 0 || conf_difficulty >= 8 ) err = 1 ;
		if( conf_damage_level < 0 || conf_damage_level >= 4 ) err = 1 ;
		if( conf_time_speed < 0 || conf_time_speed >= 4 ) err = 1 ;
		if( conf_vms_mode < 0 || conf_vms_mode >= 3 ) err = 1 ;
		if( conf_vm_write < 0 || conf_vm_write >= 2 ) err = 1 ;
		if( conf_speed_fix < 0 || conf_speed_fix >= 3 ) err = 1 ;
		if( conf_event_flag < 0 || conf_event_flag >= 2 ) err = 1 ;
		if( conf_no_challenge_flag < 0 || conf_no_challenge_flag >= 2 ) err = 1 ;
		if( conf_open_lv < 0 || conf_open_lv >= 5 ) err = 1 ;
	}
	if( err ) sys_set_save() ;
}

/*==============================================================*/
/*			Print a list of messages, up to the one whose x		*/
/*			is past the screen									*/
/*==============================================================*/
static void msg_disp( const MES_WORK *msg )
{
	while( msg->x < 100 ){
		mes_font_disp( msg ) ;
		msg ++ ;
	}
}

static const MES_WORK title_jpn[] = {
	{ 11, 8, 0, "  MARVEL^E VS.CAPCOM ^D 2" },
	{ 11, 10, 0, "    New Age of Heroes" },
	{ 11, 12, 0, "       0 0 0 2 2 4" },
	{ 11, 14, 0, "        J A P A N" },
	{ 0xff }
} ;

static const MES_WORK title_usa[] = {
	{ 11, 8, 1, "  MARVEL^E VS.CAPCOM ^D 2" },
	{ 11, 10, 1, "    New Age of Heroes" },
	{ 11, 12, 1, "       0 0 0 2 2 4" },
	{ 11, 14, 1, "          U S A" },
	{ 0xff }
} ;

static const MES_WORK title_asia[] = {
	{ 11, 8, 4, "  MARVEL^E VS.CAPCOM ^D 2" },
	{ 11, 10, 4, "    New Age of Heroes" },
	{ 11, 12, 4, "       0 0 0 2 2 4" },
	{ 11, 14, 4, "         A S I A" },
	{ 0xff }
} ;

static const MES_WORK title_euro[] = {
	{ 11, 8, 5, "  MARVEL^E VS.CAPCOM ^D 2" },
	{ 11, 10, 5, "    New Age of Heroes" },
	{ 11, 12, 5, "       0 0 0 2 2 4" },
	{ 11, 14, 5, "         E U R O" },
	{ 0xff }
} ;

static const MES_WORK warning_jpn[] = {
	{ 17, 4, 0, "WARNING" },
	{ 4, 7, 0, "This game is for use in Japan only." },
	{ 4, 9, 0, "Sales, export or operation outside this" },
	{ 4, 11, 0, "country may be construed as " },
	{ 4, 13, 0, "copyright and trademark infringement " },
	{ 4, 15, 0, " and is strictly prohibited." },
	{ 4, 17, 0, "Violators are subject to severe penalties" },
	{ 4, 19, 0, "and will be prosecuted to the full extent " },
	{ 4, 21, 0, "of the law." },
	{ 0xff }
} ;

static const MES_WORK warning_usa[] = {
	{ 17, 4, 1, "WARNING" },
	{ 4, 7, 1, "This game is for use in the United States of" },
	{ 4, 9, 1, "America and Canada only." },
	{ 4, 11, 1, "Sales, export or operation outside these" },
	{ 4, 13, 1, "countries may be construed as copyright and" },
	{ 4, 15, 1, "trademark infringement and is strictly" },
	{ 4, 17, 1, "prohibited." },
	{ 4, 19, 1, "Violators are subject to severe penalties" },
	{ 4, 21, 1, "and will be prosecuted to the full extent" },
	{ 4, 23, 1, "of the law." },
	{ 0xff }
} ;

static const MES_WORK warning_euro[] = {
	{ 17, 4, 5, "WARNING" },
	{ 4, 7, 5, "This game is for use in the European " },
	{ 4, 9, 5, "countries only." },
	{ 4, 11, 5, "Sales, export or operation outside these" },
	{ 4, 13, 5, "countries may be construed as copyright and" },
	{ 4, 15, 5, "trademark infringement and is strictly" },
	{ 4, 17, 5, "prohibited." },
	{ 4, 19, 5, "Violators are subject to severe penalties" },
	{ 4, 21, 5, "and will be prosecuted to the full extent" },
	{ 4, 23, 5, "of the law." },
	{ 0xff }
} ;

static const MES_WORK warning_asia[] = {
	{ 17, 4, 4, "WARNING" },
	{ 4, 7, 4, "This game is for use in the South-East Asia " },
	{ 4, 9, 4, " countries only." },
	{ 4, 11, 4, "Sales, export or operation outside these" },
	{ 4, 13, 4, "countries may be construed as copyright and" },
	{ 4, 15, 4, "trademark infringement and is strictly" },
	{ 4, 17, 4, "prohibited." },
	{ 4, 19, 4, "Violators are subject to severe penalties" },
	{ 4, 21, 4, "and will be prosecuted to the full extent" },
	{ 4, 23, 4, "of the law." },
	{ 0xff }
} ;

static const MES_WORK *title_tbl[8] = {
	title_jpn, title_usa, title_euro, title_asia, title_jpn, title_jpn, title_jpn, title_jpn
} ;

static const MES_WORK *warning_tbl[8] = {
	warning_jpn, warning_usa, warning_euro, warning_asia, warning_jpn, warning_jpn, warning_jpn, warning_jpn
} ;

/****************************************************************/
/*			The screens shown at start-up: the title and the	*/
/*			country's warning, each for three seconds or		*/
/*			until a button is pressed; an I/O board that is		*/
/*			missing or too small stops the game here			*/
/****************************************************************/
void warning_disp( void )
{
	const MES_WORK *msg ;
	int i ;

	Palette_Init() ;
	Texture_Init() ;
	if( !jamma_err ){
		nlWaitVSync() ;
		book_clear_ck() ;
		nlWaitVSync() ;
	}
	i = 180 ;
	msg = title_tbl[sys_flag->Country] ;
	do {
		if( test_sw_ck() ) test_mode_jump() ;
		msg_disp( msg ) ;
		nlWaitVSync() ;
		i -- ;
		if( sw_work[0].now & KEY_ATCK ) i = 0 ;
	} while( i ) ;
	if( jamma_err == 1 ){
		while( 1 ){
			if( test_sw_ck() ) test_mode_jump() ;
			nlSetFontSize( 16, 16 ) ;
			nlPrintColor( 0xffff0000 ) ;
			nlLocate( 18, 10 ) ;
			nlPrintf( "ERROR." ) ;
			nlLocate( 9, 12 ) ;
			nlPrintf( "JAMMA I/O BOARD NOT FOUND." ) ;
			nlWaitVSync() ;
		}
	}
	if( jamma_err == 2 ){
		while( 1 ){
			if( test_sw_ck() ) test_mode_jump() ;
			nlSetFontSize( 16, 16 ) ;
			nlPrintColor( 0xffff0000 ) ;
			nlLocate( 18, 10 ) ;
			nlPrintf( "ERROR." ) ;
			nlLocate( 6, 12 ) ;
			nlPrintf( "JAMMA I/O BOARD(S) DON'T HAVE\n" ) ;
			nlPrintf( "ENOUGH SWITCHES OR FUNCTIONS." ) ;
			nlWaitVSync() ;
		}
	}
	i = 180 ;
	msg = warning_tbl[sys_flag->Country] ;
	do {
		if( test_sw_ck() ) test_mode_jump() ;
		msg_disp( msg ) ;
		nlWaitVSync() ;
		i -- ;
		if( sw_work[0].now & KEY_ATCK ) i = 0 ;
	} while( i ) ;
}

/****************************************************************/
/*			The I/O boards' error counts, to watch for a bad	*/
/*			connection											*/
/****************************************************************/
void jamma_err_init( void )
{
	int i ;

	jamma_err_timer = 3600 ;
	for( i = 0 ; i < 8 ; i ++ ){
		jamma_err_cnt1[i] = nlaJAMMAAcknowError[1][i] ;
		if( jamma_cnt == 2 ) jamma_err_cnt2[i] = nlaJAMMAAcknowError[2][i] ;
	}
}

/****************************************************************/
/*			Once a minute after the start, too many new			*/
/*			errors stop the game with a message					*/
/****************************************************************/
void jamma_err_ck( void )
{
	int i ;
	int err ;

	if( !jamma_err_timer ) return ;
	jamma_err_timer -- ;
	if( jamma_err_timer ) return ;
	for( i = 0 ; i < 8 ; i ++ ){
		err = 0 ;
		if( nlaJAMMAAcknowError[1][i] - jamma_err_cnt1[i] > 720 ) err = 1 ;
		if( jamma_cnt == 2 ){
			if( nlaJAMMAAcknowError[2][i] - jamma_err_cnt2[i] > 720 ) err = 1 ;
		}
		if( err ){
			nlSetBackColor( 0, 0, 0 ) ;
			for( ;; ){
				if( test_sw_ck() ) test_mode_jump() ;
				nlSetFontSize( 16, 16 ) ;
				nlPrintColor( 0xffff0000 ) ;
				nlLocate( 18, 10 ) ;
				nlPrintf( "ERROR." ) ;
				nlLocate( 11, 12 ) ;
				nlPrintf( " JAMMA I/O BOARD(S)\n" ) ;
				nlPrintf( "BAD CONNECTION ERROR." ) ;
				nlWaitVSync() ;
			}
		}
	}
}

/****************************************************************/
/*			A side can start: in free play always, which		*/
/*			counts a game, else when it has the credits			*/
/****************************************************************/
Sint8 credit_enough_ck( int side, int cont )
{
	int ret = 0 ;

	if( credit_conf->CoinFlag == NLA_FREEPLAY ){
		ret = 1 ;
		ret <<= side ;
		book_play_add() ;
	} else if( nlaCheckCreditEnough( side, cont ) ) ret = 1 << side ;
	return ret ;
}

/****************************************************************/
/*			The credit display: "CREDIT(S) n" and the coins		*/
/*			towards the next credit, for each side that has		*/
/*			its own credits; nothing in free play				*/
/****************************************************************/
void credit_str_set( void )
{
	char *s ;

	nlaGetCreditData( &credit_info ) ;
	if( credit_conf->CoinFlag != NLA_FREEPLAY ){
		s = credit_str1 ;
		s += sprintf( s, "CREDIT(S) " ) ;
		if( !credit_info.Player[0].Credit ){
			if( credit_info.Player[0].BaseRate != 1 ){
				if( credit_info.Player[0].Remain ) s += sprintf( s, "%1d/%1d", credit_info.Player[0].Remain, credit_info.Player[0].BaseRate ) ;
				else s += sprintf( s, "0/%1d", credit_info.Player[0].BaseRate ) ;
			} else s += sprintf( s, "0" ) ;
		} else {
			s += sprintf( s, "%1d ", credit_info.Player[0].Credit ) ;
			if( credit_info.Player[0].BaseRate != 1 && credit_info.Player[0].Remain ){
				s += sprintf( s, "%1d/%1d", credit_info.Player[0].Remain, credit_info.Player[0].BaseRate ) ;
			}
		}
		*s = 0 ;
		if( !credit_conf->ChuteType ) return ;
		s = credit_str2 ;
		s += sprintf( s, "CREDIT(S) " ) ;
		if( !credit_info.Player[1].Credit ){
			if( credit_info.Player[1].BaseRate != 1 ){
				if( credit_info.Player[1].Remain ) s += sprintf( s, "%1d/%1d", credit_info.Player[1].Remain, credit_info.Player[1].BaseRate ) ;
				else s += sprintf( s, "0/%1d", credit_info.Player[1].BaseRate ) ;
			} else s += sprintf( s, "0" ) ;
		} else {
			s += sprintf( s, "%1d ", credit_info.Player[1].Credit ) ;
			if( credit_info.Player[1].BaseRate != 1 && credit_info.Player[1].Remain ){
				s += sprintf( s, "%1d/%1d", credit_info.Player[1].Remain, credit_info.Player[1].BaseRate ) ;
			}
		}
		*s = 0 ;
	}
}

/****************************************************************/
/*			The playing time from the timer: every hour of		*/
/*			it earns points										*/
/****************************************************************/
void play_time_ck( void )
{
	tmr_count = syTmrGenGetCount() ;
	if( tmr_count < 0xbebc ) return ;
	tmr_count = 0 ;
	syTmrGenSetCount( 0 ) ;
	vms_work.play_time ++ ;
	if( vms_work.play_time >= 36000 ){
		vms_work.play_time = 0 ;
		if( Sys->cpu_level_type == 0 ) point_add( 10 ) ;
		else point_add( 100 ) ;
	}
	book.play_time = vms_work.play_time ;
	book_save = -1 ;
}

/****************************************************************/
/*			The state of a side's memory card, for the			*/
/*			game's use of it									*/
/****************************************************************/
int card_ck( Uint8 side )
{
	Uint32 card ;
	int ret ;

	card = vms_work.card_bit | vms_work.card_bit_keep ;
	switch( side ){
	case 0 :
		switch( card & 0xc ){
		case 4 :
			return vms_task_card( 2, side ) ;
		case 8 :
			if( Sys->vms_mode != 2 ) return 1 ;
			return vms_task_card( 3, side ) ;
		case 0xc :
			if( Sys->vms_mode != 2 ) return 1 ;
			switch( vms_work.card_step[0] ){
			case 0 :
				ret = vms_task_card( 2, side ) ;
				if( ret == 5 && !vms_work.card_task_step[side][0] ){
					ret = 0 ;
					vms_work.card_step[0] ++ ;
				}
				break ;
			case 1 :
				ret = vms_task_card( 3, side ) ;
				break ;
			}
			return ret ;
		}
		break ;
	case 1 :
		switch( card & 0x30 ){
		case 0x10 :
			return vms_task_card( 4, side ) ;
		case 0x20 :
			if( Sys->vms_mode != 2 ) return 1 ;
			return vms_task_card( 5, side ) ;
		case 0x30 :
			if( Sys->vms_mode != 2 ) return 1 ;
			switch( vms_work.card_step[1] ){
			case 0 :
				ret = vms_task_card( 4, side ) ;
				if( ret == 5 && !vms_work.card_task_step[side][0] ){
					vms_work.card_step[1] ++ ;
					ret = 0 ;
				}
				break ;
			case 1 :
				ret = vms_task_card( 5, side ) ;
				break ;
			}
			return ret ;
		}
		break ;
	}
}

/****************************************************************/
/*			Which sides have a memory card: bit 0 the first,	*/
/*			bit 1 the second; 0x80 when none can be read		*/
/****************************************************************/
Uint32 card_side_get( void )
{
	Uint32 ret = 0 ;

	if( ( vms_work.card_bit = vms_ports_card_now_ck() ) == -1 ) return 0x80 ;
	vms_work.card_bit &= 0x3c ;
	if( !( Sys->new_side & 1 ) ) vms_work.card_bit &= ~0xc ;
	if( !( Sys->new_side & 2 ) ) vms_work.card_bit &= ~0x30 ;
	if( vms_work.card_bit & 0xc ) ret = 1 ;
	if( vms_work.card_bit & 0x30 ) ret |= 2 ;
	return ret ;
}

/****************************************************************/
void card_flag_clr( void )
{
	vms_work.card_done_bit = 0 ;
	vms_work.card_task_step[0][0] = vms_work.card_task_step[0][1] = 0 ;
	vms_work.card_task_step[1][0] = vms_work.card_task_step[1][1] = 0 ;
}

/* the points needed for each level */
static Sint32 point_tbl[100] = {
	100, 200, 300, 400, 500, 600, 700, 800, 900, 1000,
	1500, 2000, 2500, 3000, 3500, 4000, 4500, 5000, 5500, 6000,
	6500, 7000, 7500, 8000, 8500, 9000, 9500, 10000, 11000, 12000,
	13000, 14000, 15000, 16000, 17000, 18000, 19000, 20000, 21000, 22000,
	23000, 24000, 25000, 26000, 27000, 28000, 29000, 30000, 31000, 32000,
	33000, 34000, 35000, 36000, 37000, 38000, 39000, 40000, 41000, 42000,
	43000, 44000, 45000, 46000, 47000, 48000, 49000, 50000, 51000, 52000,
	53000, 54000, 55000, 56000, 57000, 58000, 59000, 60000, 61000, 62000,
	63000, 64000, 65000, 66000, 67000, 68000, 69000, 70000, 71000, 72000,
	73000, 74000, 75000, 76000, 77000, 78000, 79000, 80000, 100000, 1000000
} ;

static Sint32 point2_tbl[100] = {
	400, 800, 1200, 1600, 2000, 2400, 2800, 3200, 3600, 4000,
	4400, 4800, 5200, 5600, 6000, 6400, 6800, 7200, 7600, 8000,
	8400, 8800, 9200, 9600, 10000, 10400, 10800, 11200, 11600, 12000,
	12400, 12800, 13200, 13600, 14000, 14400, 14800, 15200, 15600, 16000,
	16400, 16800, 17200, 17600, 18000, 18400, 18800, 19200, 19600, 20000,
	20400, 20800, 21200, 21600, 22000, 22400, 22800, 23200, 23600, 24000,
	24400, 24800, 25200, 25600, 26000, 26400, 26800, 27200, 27600, 28000,
	28400, 28800, 29200, 29600, 30000, 30400, 30800, 31200, 31600, 32000,
	32400, 32800, 33200, 33600, 34000, 34400, 34800, 35200, 35600, 36000,
	36400, 36800, 37200, 37600, 38000, 38400, 38800, 39200, 39600, 1000000
} ;

/* the hidden characters, four to a group: each level of points opens one of a group's */
static Sint8 open_char_tbl[7][4] = {
	8, 10, 48, 42,
	17, 16, 34, 36,
	46, 47, 57, 51,
	28, 29, 38, 27,
	50, 53, 49, 43,
	39, 33, 37, 35,
	54, 52, 32, 58
} ;

/* what each level of points opens: a group of open_char_tbl, 7 for one more of all, -1 nothing */
static Sint8 open_lv_tbl[100] = {
	0, 7, 1, 7, 0, 7, 1, 7, 0, 7, 1, 7, 0, 7, 1, 7, 2, 7, 3, 7,
	2, 7, 3, 7, 2, 7, 3, 7, 2, 7, 3, 7, 4, 7, 5, 7, 4, 7, 5, 7,
	4, 7, 5, 7, 4, 7, 5, 7, 6, 7, 6, 7, 6, 7, 6, 7, 7, 7, 7, 7,
	7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
	7, 7, 7, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
} ;

static Sint8 open_lv2_tbl[100] = {
	0, 7, 7, 1, 7, 7, 0, 7, 7, 1, 7, 7, 0, 7, 7, 1, 7, 7, 0, 7, 7,
	1, 7, 7, 2, 7, 7, 3, 7, 7, 2, 7, 7, 3, 7, 7, 2, 7, 7, 3, 7, 7,
	2, 7, 7, 3, 7, 7, 4, 7, 7, 5, 7, 7, 4, 7, 7, 5, 7, 7, 4, 7, 7,
	5, 7, 7, 4, 7, 7, 5, 7, 7, 6, 7, 7, 6, 7, 7, 6, 7, 7, 6, 7, 7,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
} ;

static Sint8 open_lv3_tbl[100] = {
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
} ;

/*==============================================================*/
/*			Open one character of the group, at random among	*/
/*			those still closed									*/
/*==============================================================*/
static void char_open( Sint8 grp )
{
	Sint8 list[4] ;
	Sint8 *id ;
	int cnt ;
	int i ;
	int no ;
	Sint8 c ;
	int sel ;

	for( i = 0 ; i < 4 ; i ++ ) list[i] = 127 ;
	cnt = 0 ;
	id = open_char_tbl[grp] ;
	for( i = 4 ; i ; i -- ){
		c = *id ++ ;
		no = c ;
		if( no < 32 ){
			if( !( Sys->char_open0 & 1 << no ) ){
				list[cnt] = c ;
				cnt ++ ;
			}
		} else {
			if( !( Sys->char_open1 & 1 << ( no - 32 & 31 ) ) ){
				list[cnt] = c ;
				cnt ++ ;
			}
		}
	}
	if( !cnt ) return ;
	sel = nlRand() % cnt ;
	no = list[sel] ;
	if( no < 32 && no != 127 ) Sys->char_open0 |= 1 << no ;
	else Sys->char_open1 |= 1 << ( no - 32 & 31 ) ;
	book.char_open[0] = Sys->char_open0 ;
	book.char_open[1] = Sys->char_open1 ;
	book_save = -1 ;
}

/*==============================================================*/
/*			Open one more of all the characters that are		*/
/*			open in some way, at random							*/
/*==============================================================*/
static void char_open_all( void )
{
	Sint8 list[56] ;
	int cnt ;
	int i ;
	Uint32 bit ;
	int no ;
	int sel ;

	for( i = 0 ; i < 56 ; i ++ ) list[i] = 127 ;
	bit = 1 ;
	cnt = 0 ;
	for( i = 0 ; i < 32 ; i ++, bit = bit << 1 ){
		if( i != 24 && i != 25 && i != 26 ){
			if( !( Sys->char_open_all0 & bit ) && ( ( Sys->char_open0 | 0xc0fcfaff ) & bit ) ){
				list[cnt] = i ;
				cnt ++ ;
			}
		}
	}
	bit = 1 ;
	for( i = 0 ; i < 27 ; i ++, bit = bit << 1 ){
		if( !( Sys->char_open_all1 & bit ) && ( ( Sys->char_open1 | 0x1803300 ) & bit ) ){
			list[cnt] = i + 32 ;
			cnt ++ ;
		}
	}
	if( !cnt ) return ;
	sel = nlRand() % cnt ;
	no = list[sel] ;
	if( no < 32 && no != 127 ) Sys->char_open_all0 |= 1 << no ;
	else Sys->char_open_all1 |= 1 << ( no - 32 ) ;
	book.char_open_all[0] = Sys->char_open_all0 ;
	book.char_open_all[1] = Sys->char_open_all1 ;
	book_save = -1 ;
}

/*==============================================================*/
static void char_open_req( Sint16 lv )
{
	Sint8 no ;

	if( !Sys->cpu_level_type ) no = open_lv_tbl[lv] ;
	else no = open_lv2_tbl[lv] ;
	if( no == -1 ) return ;
	if( no == 7 ) char_open_all() ;
	else char_open( no ) ;
}

/****************************************************************/
/*			Add points, up to 999999, and open what each new	*/
/*			level opens											*/
/****************************************************************/
void point_add( int pt )
{
	if( vms_work.point < 999999 ){
		vms_work.point += pt ;
		if( vms_work.point > 999999 ) vms_work.point = 999999 ;
		book.point = vms_work.point ;
	}
	if( vms_work.point_lv == 99 ) return ;
	if( !Sys->cpu_level_type ){
		while( vms_work.point >= point_tbl[vms_work.point_lv] ){
			char_open_req( vms_work.point_lv ) ;
			vms_work.point_lv ++ ;
		}
	} else {
		while( vms_work.point >= point2_tbl[vms_work.point_lv] ){
			char_open_req( vms_work.point_lv ) ;
			vms_work.point_lv ++ ;
		}
	}
	book_save = -1 ;
}

/****************************************************************/
/*			The level the points have reached					*/
/****************************************************************/
void point_lv_set( void )
{
	if( Sys->cpu_level_type == 0 ){
		while( vms_work.point >= point_tbl[vms_work.point_lv] ) vms_work.point_lv ++ ;
	} else {
		while( vms_work.point >= point2_tbl[vms_work.point_lv] ) vms_work.point_lv ++ ;
	}
}

static const Sint8 open_set_tbl[5] = { 0, 19, 39, 59, 83 } ;
static const Sint8 open_set2_tbl[5] = { 0, 23, 47, 71, 83 } ;

/****************************************************************/
/*			The settings and the characters opened from the		*/
/*			backup, and the points the setting for opening		*/
/*			characters gives at least							*/
/****************************************************************/
void sys_open_init( void )
{
	Sint32 pt ;

	if( !sys_set.open_lv ) return ;
	Sys->cpu_level_type = conf_country ;
	Sys->vms_mode = conf_vms_mode ;
	Sys->vm_write = conf_vm_write ;
	Sys->char_open0 |= book.char_open[0] ;
	Sys->char_open1 |= book.char_open[1] ;
	Sys->char_open_all0 = book.char_open_all[0] ;
	Sys->char_open_all1 = book.char_open_all[1] ;
	vms_work.point = book.point ;
	point_lv_set() ;
	if( !conf_country ) pt = point_tbl[open_set_tbl[sys_set.open_lv]] ;
	else pt = point2_tbl[open_set2_tbl[sys_set.open_lv]] ;
	if( vms_work.point < pt ){
		pt -= vms_work.point ;
		point_add( pt ) ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "system.c" End of File	*/
