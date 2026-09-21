/* drain_messages -- shipped at 1078:112f.
 *
 * Ported by tools/port.py from matched/platform/drain_messages.c.
 *
 * The file's own MSG dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i16 pascal PeekMessage(MSG *m, col_u16 hwnd, col_u16 lo, col_u16 hi, col_u16 remove);;
#endif

/* 18 bytes */


void drain_messages(void)
{
    MSG msg;

    while (PeekMessage(&msg, 0, 0, 0, 1) != 0)
        ;
}
