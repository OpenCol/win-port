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
extern col_i16 pascal PeekMessage(MSG *m, col_u16 hwnd, col_u16 lo, col_u16 hi, col_u16 remove);
#endif

/* 18 bytes, and every member is
   WINDOWS.H's: HWND and UINT and WPARAM are `unsigned`, LPARAM is `signed long`,
   DWORD is `unsigned long`, POINT is two `int` -- kept under the corpus's own names
   `pt` and `y` for those two offsets, because `check_field_names` holds one name
   per offset and WINDOWS.H's `x` is not the name this tree already used. The four
   files that declare MSG
   had four different spellings and `analysis/signedness.json` could not test two
   of them; nothing here READS a member, so the types were free and the header the
   game was compiled against decides them (2026-09-24). */


void drain_messages(void)
{
    MSG msg;

    while (PeekMessage(&msg, 0, 0, 0, 1) != 0)
        ;
}
