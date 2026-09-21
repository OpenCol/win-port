/* peek_message_only -- shipped at 1078:10e7.
 *
 * Ported by tools/port.py from matched/platform/peek_message_only.c.
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


void peek_message_only(void)
{
    MSG m;

    while (PeekMessage(&m, 0, 0x200, 0x209, 1))
        ;
    while (PeekMessage(&m, 0, 0x100, 0x108, 1))
        ;
}
