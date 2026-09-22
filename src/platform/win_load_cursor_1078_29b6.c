/* win_load_cursor_1078_29b6 -- shipped at 1078:29b6.
 *
 * Ported by tools/port.py from matched/platform/win_load_cursor_1078_29b6.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern unsigned far g_hinstance;                     DGROUP:0x56e2
 *
 * The file's own MSWin dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_u16 pascal LoadCursor(col_u16 hinst, const char *name);
#endif

void win_load_cursor_1078_29b6(MSWin *w, col_i16 id)
{
    if (w == 0)
        return;
    if (id < 0) {
        if (id == -1)
            w->hcursor = LoadCursor(0, (const char *)0x7f00L);
        else if (id == -2)
            w->hcursor = LoadCursor(0, (const char *)0x7f02L);
    } else {
        w->hcursor = LoadCursor(g_hinstance, (const char *)(col_u32)(col_u16)id);
    }
}
