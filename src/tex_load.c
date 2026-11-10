/* The loading of the players' data and pictures, and the textures of each scene's models
   (TexLoad), with the start-up's load of the system files. */
#include "player.h"
#include "naomi.h"

#define SEL_OBJ		( ( OBJ_FILE * )0x0d082000 )	/* the model file of scene 4 */
#define VS_OBJ		( ( OBJ_FILE * )0x0d2bb000 )	/* the model file of scene 10 */

/* a part of a loaded file, from the table of offsets at its start */
#define FILE_PART( p, n )	( ( void * )&( ( Uint8 * )( p ) )[( ( Sint32 * )( p ) )[n]] )

extern int file_load( int no, void *adrs, int flag );
extern void load_speed_set( int time );
extern void load_init( void );
extern void obj_mat_save( int *obj, Float *col );
extern void obj_mat_load( int *obj, Float *col );
extern void col_set( void *col, int no, int cnt );

/* by stage_no, the stage's two files (am_load.c loads them): the second, and its models */
Sint32 stg_file_tbl[9] = { 0x316, 0x318, 0x31a, 0x31c, 0x31e, 0x320, 0x322, 0x324, 0x326 } ;
Sint32 stg_obj_tbl[9] = { 0x315, 0x317, 0x319, 0x31b, 0x31d, 0x31f, 0x321, 0x323, 0x325 } ;

static Uint8 pl_pic_tbl[3] = { 1, 0, 3 } ;
static Uint8 pl_pic_pos[6] = { 0, 3, 1, 4, 2, 5 } ;

static NLthd tex05_thd[] = {
	{ 0x100, 0x100, 0x100, ( void * )0x0ce60000, 0 },
	{ -1, -1, -1, ( void * )-1, -1 },
} ;

static NLthd tex09_thd[] = {
	{ 0x100, 0x100, 0x101, 0, 0 },
	{ 0x100, 0x100, 0x101, 0, 0 },
	{ 0x80, 0x80, 0x102, ( void * )0x200000, 0 },
	{ 0x80, 0x80, 0x102, ( void * )0x208000, 0 },
	{ -1, -1, -1, ( void * )-1, -1 },
} ;

static Sint8 pl_pic_no[6] = { 0, 3, 1, 4, 2, 5 } ;

static NLthd tex0e_thd[] = {
	{ 0x80, 0x80, 0x102, ( void * )0x0cc80000, 0 },
	{ -1, -1, -1, ( void * )-1, -1 },
} ;

/****************************************************************/
/*			Load a player's data file to buffer no, and set		*/
/*			up the player from it								*/
/****************************************************************/
void pl_data_load( PLAYER_WORK *pl_w, int no )
{
	Uint8 *chr ;
	Uint8 *adrs ;
	Sint16 *src ;

	adrs = pl_data_adrs_tbl[no] ;
	file_load2( pl_file_tbl[pl_w->load_type], adrs, 1 ) ;
	pl_w->pl_data_adrs = ( Sint32 * )adrs ;
	pl_w->cell_adrs = FILE_PART( adrs, 0 ) ;
	pl_w->spr_adrs = FILE_PART( adrs, 1 ) ;
	pl_w->col_adrs = FILE_PART( adrs, 2 ) ;
	pl_w->catch_pos_tbl = FILE_PART( adrs, 3 ) ;
	pl_w->ef_pos_tbl = FILE_PART( adrs, 4 ) ;
	pl_w->spr_set_buff = spr_set_buff_tbl[no] ;
	pl_w->col_flag = 1 ;
	pl_w->col_no = col_no_tbl[pl_w->pl_index] ;
	pl_w->col_cnt = col_cnt_tbl[pl_w->load_type] ;
	pl_w->char_ofs = 0x100 ;
	if( pl_w->load_type == 0x34 ) pl_w->char_ofs = 0x200 ;
	col_set( ( Uint16 * )pl_w->col_adrs + pl_w->load_color * 0x80, pl_w->col_no, 8 ) ;
	src = FILE_PART( 0x0dd00000, pl_w->load_type ) ;
	lz_unpack16( src, ( Uint16 * )( chr = adrs + 0x130000 ) ) ;
	pl_w->char_adrs = FILE_PART( chr, 0 ) ;
	pl_w->frame_box_tbl = FILE_PART( chr, 1 ) ;
	pl_w->hit_box_tbl = FILE_PART( chr, 2 ) ;
	pl_w->hit_data_tbl = FILE_PART( chr, 3 ) ;
	pl_w->cpu_adrs[0] = FILE_PART( chr, 4 ) ;
	pl_w->cpu_adrs[1] = FILE_PART( chr, 5 ) ;
	pl_w->cpu_answer_adrs = FILE_PART( chr, 7 ) ;
	pl_w->cpu_adrs[2] = FILE_PART( chr, 6 ) ;
	pl_w->cpu_ex_adrs = FILE_PART( chr, 8 ) ;
}

/****************************************************************/
/*			Empty in the arcade build							*/
/****************************************************************/
void tex_dummy_0()
{
}

/****************************************************************/
void tex_dummy_1( void )
{
}

/****************************************************************/
void tex_dummy_2( void )
{
}

/****************************************************************/
void tex_dummy_3( void )
{
}

/****************************************************************/
void tex_dummy_4( void )
{
}

/****************************************************************/
void tex_dummy_5( void )
{
}

/****************************************************************/
void tex_dummy_6( void )
{
}

/****************************************************************/
void tex_load_stage( void )
{
	int i ;

	nlSetCommonTextureOffset( 0xc10 ) ;
	obj_stg = ( OBJ_FILE * )0x0d82d000 ;
	for( i = 0 ; i < obj_stg->cnt ; i ++ ){
		nlObjPutLoadTexture( obj_stg->obj[i], obj_stg->thd ) ;
	}
}

/****************************************************************/
/*			Unpack a picture into texture no of a model file	*/
/****************************************************************/
void obj_tex_unpack( int no, Sint16 *src, OBJ_FILE *of )
{
	NLthd *thd ;
	Uint16 *dst ;

	thd = of->thd ;
	dst = thd[no].address ;
	lz_unpack16( src, dst ) ;
}

/****************************************************************/
/*			Move a model file's pointers from where it was		*/
/*			built to where it was loaded, with its textures'	*/
/*			data at adrs										*/
/****************************************************************/
void obj_file_reloc( void *data, Uint8 *adrs )
{
	Sint32 *p = data ;
	OBJ_FILE *of ;
	Sint32 ofs ;
	NLthd *thd ;
	Sint32 *tbl ;
	int i ;

	ofs = p[0] - ( Sint32 )data - 16 ;	/* the file was built to sit at p[0] - 16 */
	p[0] -= ofs ;
	p[2] -= ofs ;
	of = data ;
	tbl = ( Sint32 * )p[0] ;
	for( i = 0 ; i < of->cnt ; i ++ ) tbl[i] -= ofs ;
	thd = of->thd ;
	ofs = ( Sint32 )thd->address - ( Sint32 )adrs ;
	while( thd->width ){
		thd->address = ( Uint8 * )thd->address - ofs ;
		thd ++ ;
	}
}

/****************************************************************/
/*			Load the textures of a model file					*/
/****************************************************************/
void obj_tex_load( Sint32 ofs, OBJ_FILE **ofp, OBJ_FILE *of )
{
	int i ;

	nlSetCommonTextureOffset( ofs ) ;
	*ofp = of ;
	for( i = 0 ; i < ( *ofp )->cnt ; i ++ ){
		nlObjPutLoadTexture( ( *ofp )->obj[i], ( *ofp )->thd ) ;
	}
}

/****************************************************************/
void tex_load_select( void )
{
	Sys->tex_buff = ( Uint8 * )0x0c420000 ;
	if( Sys->play_side == 1 ) Sys->tex_buff = ( Uint8 * )0x0c810000 ;
	file_load( 0x8a, ( void * )0x0cc00000, 0 ) ;
	lz_unpack16( ( Sint16 * )0x0cc00000, ( Uint16 * )Sys->tex_buff ) ;
	file_load( 0x329, ( void * )0x0d0c6000, 0 ) ;
	if( Sys->cpu_level_type ) file_load( 0x8c, ( void * )0x0ce60000, 0 ) ;
	else file_load( 0x8b, ( void * )0x0ce60000, 0 ) ;
}

/****************************************************************/
/*			Load each player's picture file						*/
/****************************************************************/
void pl_pic_load( void )
{
	PLAYER_WORK *pl_w ;
	int i ;

	for( i = 0 ; i < 6 ; i ++ ){
		pl_w = &pl_work[i] ;
		pl_w->pic_adrs = pic_adrs_tbl[i] ;
		file_load2( pic_file_tbl[pl_w->load_type], pl_w->pic_adrs, 1 ) ;
	}
}

/****************************************************************/
void tex_load_st_demo( void )
{
	pl_pic_load() ;
}

/****************************************************************/
void tex_load_staff( void )
{
	Sys->tex_buff = ( Uint8 * )0x0c810000 ;
	if( Sys->play_side == 1 ) Sys->tex_buff = ( Uint8 * )0x0c420000 ;
	file_load( 62, ( void * )0x0cc00000, 0 ) ;
	lz_unpack16( ( Sint16 * )0x0cc00000, ( Uint16 * )Sys->tex_buff ) ;
}

/****************************************************************/
/* load the pictures of the side's team; em_w is unused */
void tex_load_ranking( void )
{
	PLAYER_WORK *pl_w, *em_w ;
	int side ;
	int i ;

	file_load( 0x332, ( void * )0x0d2e9000, 0 ) ;
	side = Sys->rank_side ;
	if( side ^ 1 ) Sys->tex_buff = ( Uint8 * )0x0c810000 ;
	else Sys->tex_buff = ( Uint8 * )0x0c420000 ;
	pl_w = vs_sys_work.team_w[side][0] ;
	Sys->rank_char = pl_w->load_type ;
	for( i = 0 ; i < 3 ; i ++ ){
		pl_w = vs_sys_work.team_w[side][i] ;
		em_w = vs_sys_work.team_w[side ^ 1][i] ;
		file_load2( vs_file_tbl[pl_w->load_type], pl_data_adrs_tbl[i] + 0x130000, 1 ) ;
	}
	lz_unpack16( ( Sint16 * )0x0d31f000, ( Uint16 * )Sys->tex_buff ) ;
}

/****************************************************************/
void tex_load_opening( void )
{
	const FILE_DATA *ft ;
	char *dst ;

	file_load( 0x341, ( void * )0x0c420000, 0 ) ;
	ft = &stream_tbl[141] ;
	dst = ( char * )0x0c520000 ;
	rom_dma_flag = 1 ;
	cache_inv( dst, 0x200000 ) ;
	nlaReadROMDMA( ft->adrs, dst, 0x200000, 1 ) ;
	rom_dma_flag = 0 ;
}


/****************************************************************/
/*			Unpack the texture of scene 14's model, and			*/
/*			point the model at it								*/
/****************************************************************/
void tex_load_0e( void )
{
	Uint16 *dst ;

	if( 0 ) *( Uint16 ** )0x0d720020 = dst ;
	dst = ( Uint16 * )0x0cc00000 ;
	if( Sys->cpu_level_type == 0 ){ }	/* tested, and nothing done either way */
	*( Uint16 ** )0x0d720020 = dst ;
	lz_unpack16( ( Sint16 * )0x0d721000, dst ) ;
}

/****************************************************************/
/*			Unpack each player's picture into the textures,		*/
/*			and load the model file's							*/
/****************************************************************/
void tex_load_gauge( void )
{
	PLAYER_WORK *pl_w ;
	NLthd *thd ;
	Uint32 *src, *dst ;
	void *pic ;
	int i, j, n ;

	obj_dm00 = SEL_OBJ ;
	thd = SEL_OBJ->thd ;
	for( i = 0 ; i < 6 ; i ++ ){
		pl_w = &pl_work[i] ;
		if( !Sys->cpu_level_type ){
			pic = FILE_PART( pl_w->pic_adrs, 0 ) ;
		} else {
			n = 0 ;
			if( pl_w->load_type < 32 ){
				if( ( 1 << pl_w->load_type ) & 0xd0010000 ) n = 3 ;
			} else {
				if( ( 1 << ( pl_w->load_type - 32 ) ) & 0x04000042 ) n = 3 ;
			}
			pic = FILE_PART( pl_w->pic_adrs, n ) ;
		}
		lz_unpack16( pic, ( Uint16 * )0x0ce60000 ) ;
		dst = thd[pl_pic_pos[i] + 10].address ;
		src = ( Uint32 * )( 0x0ce60000 + pl_pic_tbl[pl_w->pl_personal] * 0x800 ) ;
		for( j = 0x200 ; j ; j -- ) *dst ++ = *src ++ ;
		dst = thd[pl_pic_pos[i] + 16].address ;
		src = ( Uint32 * )0x0ce61000 ;
		for( j = 0x200 ; j ; j -- ) *dst ++ = *src ++ ;
	}
	nlSetCommonTextureOffset( 0xc90 ) ;
	for( i = 0 ; i < obj_dm00->cnt ; i ++ ){
		nlObjPutLoadTexture( obj_dm00->obj[i], obj_dm00->thd ) ;
	}
}

/****************************************************************/
void tex_set_select( void )
{
	int i ;

	obj_dm01 = ( OBJ_FILE * )0x0d0c6000 ;
	nlSetCommonTextureOffset( 0x810 ) ;
	for( i = 0 ; i < obj_dm01->cnt ; i ++ ){
		nlObjPutLoadTexture( obj_dm01->obj[i], obj_dm01->thd ) ;
	}
	nlSetCommonTextureOffset( 0x910 ) ;
	nlSpriteLoadTexture( tex05_thd, 0 ) ;
}

/****************************************************************/
void tex_set_staff( void )
{
	int i ;

	obj_dm05 = ( OBJ_FILE * )0x0d7cc000 ;
	obj_dm07 = ( OBJ_FILE * )0x0d7cc000 ;
	nlSetCommonTextureOffset( 0x810 ) ;
	for( i = 0 ; i < obj_dm05->cnt ; i ++ ){
		nlObjPutLoadTexture( obj_dm05->obj[i], obj_dm05->thd ) ;
	}
	tex09_thd[0].address = Sys->tex_buff ;
	tex09_thd[1].address = Sys->tex_buff ;
	tex09_thd[2].address = Sys->tex_buff + 0x200000 ;
	tex09_thd[3].address = Sys->tex_buff + 0x208000 ;
	nlSetCommonTextureOffset( 0x910 ) ;
	nlSpriteLoadTexture( tex09_thd, 0 ) ;
}

/****************************************************************/
void tex_set_st_demo( void )
{
	PLAYER_WORK *pl_w ;
	void *pic ;
	int i ;

	obj_dm02 = ( OBJ_FILE * )0x0d25c000 ;
	for( i = 0 ; i < 6 ; i ++ ){
		pl_w = &pl_work[i] ;
		pic = FILE_PART( pl_w->pic_adrs, 2 ) ;
		obj_tex_unpack( pl_pic_no[i] + 2, pic, obj_dm02 ) ;
	}
	nlSetCommonTextureOffset( 0x810 ) ;
	for( i = 0 ; i < obj_dm02->cnt ; i ++ ){
		nlObjPutLoadTexture( obj_dm02->obj[i], obj_dm02->thd ) ;
	}
}

/****************************************************************/
/* copy the versus model's texture headers to the buffer, and build its pictures there */
void tex_set_ranking( void )
{
	static const Uint8 score_pos_tbl[16] = {	/* the score's cells, by digit */
		0x05, 0x07, 0x0d, 0x0f, 0x04, 0x06, 0x0c, 0x0e, 0x01, 0x03, 0x09, 0x0b, 0x00, 0x02, 0x08, 0x0a
	} ;
	NLthd *src, *dst, *thd ;
	PLAYER_WORK *pl_w ;
	int side ;
	int i ;

	obj_dm06 = VS_OBJ ;
	thd = ( NLthd * )( Sys->tex_buff + 0x14000 ) ;
	src = VS_OBJ->thd ;
	dst = thd ;
	while( src->width ) *dst ++ = *src ++ ;
	*dst = *src ;
	side = Sys->rank_side ;
	for( i = 0 ; i < 3 ; i ++ ){
		pl_w = vs_sys_work.team_w[side][i] ;	/* unused: left from tex_load_ranking's loop */
		obj_tex_unpack( i + 17, ( Sint16 * )( pl_data_adrs_tbl[i] + 0x130000 ), obj_dm06 ) ;
	}
	pl_w = &pl_work[side] ;
	if( Sys->vs_flag ){
		win_tex_set( pl_w->win_max, thd ) ;
	} else {
		score_tex_set( pl_w->old_score_hi, pl_w->old_score, thd[16].address, score_pos_tbl ) ;
		memset( ( Uint8 * )thd[16].address + 0x1600, 0, 0x200 ) ;
	}
	if( !Sys->vs_flag ) thd[5].address = Sys->tex_buff + 0x10000 ;
	nlSetCommonTextureOffset( 0x810 ) ;
	for( i = 0 ; i < obj_dm06->cnt ; i ++ ){
		nlObjPutLoadTexture( obj_dm06->obj[i], thd ) ;
	}
}

/****************************************************************/
void tex_set_0e( void )
{
	obj_tex_load( 0xd10, &obj_dm0a, ( OBJ_FILE * )0x0d720000 ) ;
	nlSetCommonTextureOffset( 0xd50 ) ;
	nlSpriteLoadTexture( tex0e_thd, 0 ) ;
}

void obj_col_get( OBJ_FILE *of ) ;

/****************************************************************/
/*			Load the textures of scene no						*/
/****************************************************************/
void TexLoad( int no )
{
	switch( no ){
	case 1:
		tex_load_stage() ;
		break ;
	case 2:
		obj_tex_load( 0xc50, &obj_efky, ( OBJ_FILE * )0x0da00000 ) ;
		break ;
	case 4:
		tex_load_gauge() ;
		obj_col_get( obj_dm00 ) ;
		break ;
	case 5:
		tex_load_select() ;
		tex_set_select() ;
		break ;
	case 6:
		tex_load_st_demo() ;
		tex_set_st_demo() ;
		break ;
	case 8:
		cache_purge( ( void * )0x0d2b1000, 0xa000 ) ;
		obj_tex_load( 0xd10, &obj_dm04, ( OBJ_FILE * )0x0d2b1000 ) ;
		break ;
	case 9:
		tex_load_staff() ;
		tex_set_staff() ;
		break ;
	case 10:
		tex_load_ranking() ;
		tex_set_ranking() ;
		break ;
	case 11:
		obj_tex_load( 0x810, &obj_dm07, ( OBJ_FILE * )0x0da28000 ) ;
		break ;
	case 12:
		obj_tex_load( 0x810, &obj_dm08, ( OBJ_FILE * )0x0d4ce000 ) ;
		tex_load_opening() ;
		break ;
	case 13:
		obj_tex_load( 0x810, &obj_dm09, ( OBJ_FILE * )0x0d6cc000 ) ;
		break ;
	case 14:
		tex_load_0e() ;
		tex_set_0e() ;
		break ;
	case 15:
		obj_tex_load( 0x850, &obj_dm0b, ( OBJ_FILE * )0x0d7a9000 ) ;
		break ;
	}
}

/****************************************************************/
/*			Fetch the colours of a model file's first four		*/
/*			objects												*/
/****************************************************************/
void obj_col_get( OBJ_FILE *of )
{
	int i ;

	for( i = 0 ; i <= 3 ; i ++ ) obj_mat_save( of->obj[i], obj_col_buff[i] ) ;
}

/****************************************************************/
/*			Give an object colour no							*/
/****************************************************************/
void obj_col_set( int *obj, int no )
{
	obj_mat_load( obj, obj_col_buff[no] ) ;
}

/****************************************************************/
/* at start-up: time the ROM's reads, then load the system's files */
void sys_data_load( void )
{
	Sint32 total ;
	Sint32 time ;
	int i ;

	total = 0 ;
	for( i = 0 ; i < 100 ; i ++ ){
		while( !nlaCheckWaitROMDMA() ) ;
		rom_dma_flag = 1 ;
		time = nlSysMetricsCpuPowerGet() ;
		cache_purge( ( void * )0x0da00000, 0x800 ) ;
		nlaReadROMDMA( 0x600000, ( char * )0x0da00000, 0x800, 0 ) ;
		cache_inv( ( void * )0x0da00000, 0x800 ) ;
		total += nlSysMetricsCpuPowerGet() - time ;
		rom_dma_flag = 0 ;
	}
	load_speed_set( total / 100 ) ;
	file_load( 0x313, ( void * )0x0da00000, 0 ) ;
	obj_file_reloc( ( OBJ_FILE * )0x0da00000, ( Uint8 * )0x0d026000 ) ;
	file_load( 0x314, ( void * )0x0d026000, 0 ) ;
	file_load( 0x327, ( void * )0x0d082000, 0 ) ;
	file_load( 0x328, ( void * )0x0d099000, 0 ) ;
	file_load( 0x329, ( void * )0x0d0c6000, 0 ) ;
	file_load( 0x32a, ( void * )0x0d0e5000, 0 ) ;
	file_load( 0x32b, ( void * )0x0d25c000, 0 ) ;
	file_load( 0x32c, ( void * )0x0d268000, 0 ) ;
	file_load( 0x32d, ( void * )0x0d2b1000, 0 ) ;
	file_load( 0x32e, ( void * )0x0d2b3000, 0 ) ;
	file_load( 0x331, ( void * )0x0d2bb000, 0 ) ;
	file_load( 0x332, ( void * )0x0d2e9000, 0 ) ;
	file_load( 0x333, ( void * )0x0da28000, 0 ) ;
	obj_file_reloc( ( OBJ_FILE * )0x0da28000, ( Uint8 * )0x0d3ce000 ) ;
	file_load( 0x334, ( void * )0x0d3ce000, 0 ) ;
	file_load( 0x335, ( void * )0x0d4ce000, 0 ) ;
	file_load( 0x336, ( void * )0x0d4f5000, 0 ) ;
	file_load( 0x337, ( void * )0x0d6cc000, 0 ) ;
	file_load( 0x338, ( void * )0x0d6e0000, 0 ) ;
	if( !Sys->cpu_level_type ){
		file_load( 0x339, ( void * )0x0d720000, 0 ) ;
		file_load( 0x33a, ( void * )0x0d721000, 0 ) ;
	} else {
		file_load( 0x33d, ( void * )0x0d720000, 0 ) ;
		file_load( 0x33e, ( void * )0x0d721000, 0 ) ;
	}
	file_load( 0x33b, ( void * )0x0d7a9000, 0 ) ;
	file_load( 0x33c, ( void * )0x0d7b6000, 0 ) ;
	file_load( 0x32f, ( void * )0x0d7cc000, 0 ) ;
	file_load( 0x330, ( void * )0x0d7d7000, 0 ) ;
	load_init() ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "tex_load.c" End of File	*/
