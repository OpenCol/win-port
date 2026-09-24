/* GRPort_Init_wh_1010_ee8c -- shipped at 1010:ee8c.
 *
 * Ported by tools/port.py from matched/game/GRPort_Init_wh_1010_ee8c.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void GRPort_Init_wh(void *p, col_i16 w, col_i16 h);

void GRPort_Init_wh_1010_ee8c(void *p)
{
    GRPort_Init_wh(p, 0, 0);
}
