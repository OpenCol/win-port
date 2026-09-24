/* PediaWin_Init_default -- shipped at 1060:15d0.
 *
 * Ported by tools/port.py from matched/game/PediaWin_Init_default.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void PediaWin_Init(void *o, col_i16 flag, void *rec);

void PediaWin_Init_default(void *o)
{
    PediaWin_Init(o, 0, 0);
}
