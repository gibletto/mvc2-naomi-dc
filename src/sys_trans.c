/* The frame's late work: routines queued to run after the scene is drawn, and lines of text
   queued with STrPrint and printed then. A second, smaller pair of queues works the same way. */
#include "player.h"

#define TRANS_MAX	16	/* routines in a queue */
#define STR_MAX		128	/* lines of text */
#define STR2_MAX	16

/* the C library's */
typedef char *va_list ;
extern int vsprintf( char *s, const char *fmt, va_list ap ) ;

/* the arguments after last, kept on the stack and four-byte aligned */
#define VA_START( ap, last )	( ( ap ) = ( Uint32 )( ( char * )&( last ) + sizeof( last ) ) % 4u \
	? ( char * )&( last ) + sizeof( last ) + ( 4u - sizeof( last ) % 4u ) \
	: ( char * )&( last ) + sizeof( last ) )

static int trans_cnt ;
static int str_cnt ;
static int str2_cnt ;
static int trans2_cnt ;
static MES_WORK str_buf[STR_MAX] ;
static MES_WORK str2_buf[STR2_MAX] ;
static void ( *trans_tbl[TRANS_MAX] )( void ) ;
static void ( *trans2_tbl[TRANS_MAX] )( void ) ;

/****************************************************************/
void SysTransReset( void )
{
	trans_cnt = 0 ;
	str_cnt = 0 ;
}

/****************************************************************/
void SysTransSet( void ( *func )( void ) )
{
	if( trans_cnt == TRANS_MAX ) return ;
	trans_tbl[trans_cnt] = func ;
	trans_cnt ++ ;
}

/****************************************************************/
void STrPrint( Uint8 x, Uint8 y, Uint8 col, const char *fmt, ... )
{
	va_list ap ;
	MES_WORK *mes ;
	char *s ;

	if( str_cnt == STR_MAX ) return ;
	VA_START( ap, fmt ) ;
	mes = &str_buf[str_cnt] ;
	mes->x = x ;
	mes->y = y ;
	mes->type = col ;
	s = mes->str ;
	s += vsprintf( mes->str, fmt, ap ) ;
	*s = 0 ;
	str_cnt ++ ;
}

/****************************************************************/
/*			Run the queued routines, then print the queued		*/
/*			text												*/
/****************************************************************/
void SysTrans( void )
{
	int i ;
	MES_WORK *mes ;
	void ( **func )( void ) ;

	i = trans_cnt ;
	func = trans_tbl ;
	while( i ){
		( *func )() ;
		i -- ;
		func ++ ;
	}
	i = str_cnt ;
	mes = str_buf ;
	while( i ){
		mes_font_disp( mes ) ;
		i -- ;
		mes ++ ;
	}
	nlSetFontSize( 8, 8 ) ;
	nlPrintColor( -1 ) ;
}

/****************************************************************/
void SysTransReset2( void )
{
	trans2_cnt = 0 ;
	str2_cnt = 0 ;
}

/****************************************************************/
void SysTransSet2( void ( *func )( void ) )
{
	if( trans2_cnt == TRANS_MAX ) return ;
	trans2_tbl[trans2_cnt] = func ;
	trans2_cnt ++ ;
}

/****************************************************************/
void STrPrint2( Uint8 x, Uint8 y, int col, const char *fmt, ... )
{
	va_list ap ;
	MES_WORK *mes ;
	char *s ;

	if( str2_cnt == STR2_MAX ) return ;
	VA_START( ap, fmt ) ;
	mes = &str2_buf[str2_cnt] ;
	mes->x = x ;
	mes->y = y ;
	mes->type = col ;
	s = mes->str ;
	s += vsprintf( mes->str, fmt, ap ) ;
	*s = 0 ;
	str2_cnt ++ ;
}

/****************************************************************/
void SysTrans2( void )
{
	int i ;
	MES_WORK *mes ;
	void ( **func )( void ) ;

	i = trans2_cnt ;
	func = trans2_tbl ;
	while( i ){
		( *func )() ;
		i -- ;
		func ++ ;
	}
	i = str2_cnt ;
	mes = str2_buf ;
	while( i ){
		mes_puts( mes ) ;
		i -- ;
		mes ++ ;
	}
	nlSetFontSize( 8, 8 ) ;
	nlPrintColor( -1 ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "sys_trans.c" End of File	*/
