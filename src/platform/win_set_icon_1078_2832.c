/* win_set_icon_1078_2832 -- shipped at 1078:2832.
 *
 * Ported by tools/port.py from matched/platform/win_set_icon_1078_2832.c.
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
extern col_u16 pascal LoadIcon(col_u16 hinst, const char *name);
extern col_u16 pascal SetClassWord(col_u16 hwnd, col_i16 idx, col_u16 v);
#endif

void win_set_icon_1078_2832(MSWin *w, col_i16 icon)
{
    if (w == 0)
        return;
    SetClassWord(w->hwnd, -14, LoadIcon(g_hinstance, (const char *)(col_u32)(col_u16)icon));
}
