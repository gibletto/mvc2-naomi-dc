/* The model drawing's work areas, given to nlInitDrawModel. They are in a section of their own
   (BX), which the start-up doesn't clear. */
#include "types.h"

#pragma section X
Uint8 unk_0C358468[24] ;
Float draw_vertex_work[52000] ;	/* the vertices */
Sint32 draw_light_work[26000] ;	/* their lights */
#pragma section
