/* win_set_text -- shipped at 1070:656f.
 *
 * Ported by tools/port.py from matched/platform/win_set_text.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i16 pascal SetWindowText(col_u16 hwnd, char *text);
#endif

void win_set_text(col_u16 hwnd, char *text)
{
    SetWindowText(hwnd, text);
}
