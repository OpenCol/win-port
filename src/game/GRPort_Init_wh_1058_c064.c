/* GRPort_Init_wh_1058_c064 -- shipped at 1058:c064.
 *
 * Ported by tools/port.py from matched/game/GRPort_Init_wh_1058_c064.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void GRPort_Init_wh(void *p, col_i16 w, col_i16 h);

void GRPort_Init_wh_1058_c064(void *p)
{
    GRPort_Init_wh(p, 0, 0);
}
