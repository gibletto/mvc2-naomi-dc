/* The module after vms.c, which nothing calls or reads: the name of a
   card's icon file and a buffer. */
#include "types.h"

const char vms_icon_file[] = "ICONDATA_VMS" ;
#ifdef TARGET_DC
Uint8 unk_0C312A34[36] ;	/* all the room the disc has between vms's work and net_vms's */
#else
Uint8 unk_0C312A34[620] ;
#endif
