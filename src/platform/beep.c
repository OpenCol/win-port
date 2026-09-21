/* beep -- shipped at 1078:057f.
 *
 * Ported by tools/port.py from matched/platform/beep.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern void pascal MessageBeep(col_u16 type);;
#endif

void beep(void)
{
    MessageBeep(-1);
}
