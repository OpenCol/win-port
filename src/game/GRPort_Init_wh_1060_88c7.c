/* GRPort_Init_wh_1060_88c7 -- shipped at 1060:88c7.
 *
 * Ported by tools/port.py from matched/game/GRPort_Init_wh_1060_88c7.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void GRPort_Init_wh(void *p, col_i16 w, col_i16 h);

void GRPort_Init_wh_1060_88c7(void *port)
{
    GRPort_Init_wh(port, 0, 0);
}
