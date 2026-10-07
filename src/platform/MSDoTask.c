/* MSDoTask -- shipped at 1078:115b.
 *
 * Ported by tools/port.py from matched/platform/MSDoTask.c.
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
extern col_i16 pascal TranslateMessage(MSG *m);
extern col_i32 pascal DispatchMessage(MSG *m);
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




col_i16 MSDoTask(void)
{
    MSG m;

    if (PeekMessage(&m, 0, 0, 0, 1)) {
        TranslateMessage(&m);
        DispatchMessage(&m);
        return 1;
    } else {
        return 0;
    }
}
