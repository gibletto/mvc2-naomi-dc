/* The vertical blank's routine and the wait for it, and the vibration packs in the controllers'
   expansion sockets: their requests, their start and their stop. */
#include "player.h"
#include "shinobi.h"

/* Shinobi / Kamui */
extern Uint32 syTmrGetCount( void ) ;
extern Uint32 syTmrDiffCount( Uint32 a, Uint32 b ) ;
extern Uint32 syTmrCountToMicro( Uint32 count ) ;

#define VSYNC_WAIT	167000	/* the longest wait for the vertical blank, in microseconds */
#define VIB_POW_MAX	10000	/* the power the controllers and packs of a side may draw */

/* a kind of vibration and its frames */
typedef struct VIB_DATA {
	PDS_VIBPARAM	param;
	Sint32	time;
} VIB_DATA;

static Uint8 vib_ok[2] ;	/* the side's packs may vibrate: no more than four of them */
static Uint8 vib_unit[2][2] ;	/* each socket's pack may vibrate: within the power */
static Sint32 vib_pow ;
static Sint32 vib_num ;
extern void tex_req_send( void ) ;

VIB_DATA vib_tbl[9] = {
	{ { 1, 0x81, 0, 15, 1, { 0, 0, 0 } }, 0 },
	{ { 1, 0x81, -7, 15, 1, { 0, 0, 0 } }, 4 },
	{ { 1, 0x81, 7, 15, 1, { 0, 0, 0 } }, 8 },
	{ { 1, 0x81, -4, 15, 1, { 0, 0, 0 } }, 8 },
	{ { 1, 0x81, -7, 15, 1, { 0, 0, 0 } }, 8 },
	{ { 1, 0x09, 2, 18, 1, { 0, 0, 0 } }, 30 },
	{ { 1, 0x01, 4, 20, 1, { 0, 0, 0 } }, 80 },
	{ { 1, 0x01, 5, 15, 1, { 0, 0, 0 } }, 80 },
	{ { 1, 0x81, -7, 15, 1, { 0, 0, 0 } }, 30 },
} ;

/* each side's two expansion sockets */
Sint32 vib_port[2][2] = { { 7, 8 }, { 13, 14 } } ;

/****************************************************************/
/*			The vertical blank									*/
/****************************************************************/
Sint32 vblank_func( void )
{
	vsync_flag = 1 ;
	nlLoadCorePaletteData() ;
	fog_disp() ;
	tex_req_send() ;
	return 0 ;
}

/****************************************************************/
/*			Wait for the vertical blank, or give up after		*/
/*			VSYNC_WAIT											*/
/****************************************************************/
void vsync_wait( void )
{
	Uint32 start ;
	Uint32 us ;

	start = syTmrGetCount() ;
	while( !vsync_flag ){
		us = syTmrDiffCount( start, syTmrGetCount() ) ;
		us = syTmrCountToMicro( us ) ;
		if( us > VSYNC_WAIT ) break ;
	}
}

/****************************************************************/
/*			No vibration, and every pack free to vibrate		*/
/****************************************************************/
void vib_init( void )
{
	int i ;

	for( i = 0 ; i < 2 ; i ++ ){
		vib_w[i].no = vib_w[i].cnt = 0 ;
		vib_unit[i][0] = 1 ;
		vib_unit[i][1] = 1 ;
		vib_ok[i] = 1 ;
	}
}

/****************************************************************/
/*			Ask for vibration no on side pl_no, unless a		*/
/*			lower one is running								*/
/****************************************************************/
void vib_req( int pl_no, int no )
{
	if( !vib_w[pl_no].cnt || vib_w[pl_no].no >= no ){
		vib_w[pl_no].cnt = -1 ;
		vib_w[pl_no].no = no ;
	}
}

/****************************************************************/
/*			Stop every pack										*/
/****************************************************************/
void vib_stop( void )
{
	int i, j ;

	for( i = 0 ; i < 2 ; i ++ ){
		for( j = 0 ; j < 2 ; j ++ ){
			if( pdVibMxIsReady( vib_port[i][j] ) ) pdVibMxStop( vib_port[i][j] ) ;
		}
		vib_w[i].cnt = 0 ;
		vib_w[i].no = 0 ;
	}
}

/****************************************************************/
/*			Each frame: which packs may vibrate, then start		*/
/*			the vibrations asked for and stop the ones whose	*/
/*			time is up											*/
/****************************************************************/
void vib_move( void )
{
	PDS_PERIPHERALINFO *info ;
	int i, j ;
	int no ;

	if( conf_vms_mode != 2 ) return ;
	vib_num = vib_pow = 0 ;
	for( i = 0 ; i < 2 ; i ++ ){
		vib_pow += pad_peri[i]->info->max_pow ;
		for( j = 0 ; j < 2 ; j ++ ){
			info = pdGetPeripheralInfo( vib_port[i][j] ) ;
			vib_pow += info->max_pow ;
			if( vib_pow < VIB_POW_MAX ) vib_unit[i][j] = 1 ;
			else vib_unit[i][j] = 0 ;
			if( info->type & PDD_DEVTYPE_VIBRATION ) vib_num ++ ;
		}
		/* reads past vib_port, as in the arcade's code; the index is worked out unsigned */
		info = pdGetPeripheralInfo( vib_port[i][i * 6U + 5] ) ;
		if( info->type & PDD_DEVTYPE_VIBRATION ) vib_num += 2 ;
		if( vib_num > 4 ) vib_ok[i] = 0 ;
		else vib_ok[i] = 1 ;
	}
	for( i = 0 ; i < 2 ; i ++ ){
		if( vib_w[i].cnt < 0 ){
			/* no is still the last side's here, as in the arcade's code */
			vib_w[i].cnt = vib_tbl[no].time ;
			if( ( Sys->task_no == 2 && !pl_work[i].cpu_flag ) || Sys->task_no == 3 ){
				for( j = 0 ; j < 2 ; j ++ ){
					if( pdVibMxIsReady( vib_port[i][j] ) && vib_unit[i][j] && vib_ok[i] ){
						no = vib_w[i].no ;
						vib_w[i].cnt = vib_tbl[no].time ;
						pdVibMxStart( vib_port[i][j], &vib_tbl[no].param ) ;
					}
				}
			}
		}
		if( vib_w[i].cnt > 0 ){
			vib_w[i].cnt -- ;
			if( !vib_w[i].cnt ){
				for( j = 0 ; j < 2 ; j ++ ){
					if( pdVibMxIsReady( vib_port[i][j] ) ) pdVibMxStop( vib_port[i][j] ) ;
				}
			}
		}
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "vib.c" End of File	*/
