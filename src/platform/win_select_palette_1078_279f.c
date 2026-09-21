/* win_select_palette_1078_279f -- shipped at 1078:279f.
 *
 * Ported by tools/port.py from matched/platform/win_select_palette_1078_279f.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int far g_palette_mode;                       DGROUP:0x3380
 *
 * The file's own MSWin dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_u16 pascal SelectPalette(col_u16 hdc, col_u16 hpal, col_i16 force);
#endif

void win_select_palette_1078_279f(MSWin *w, col_i16 pal)
{
    if (g_palette_mode == 1) {
        w->hpal = pal;
        SelectPalette(w->hdc, pal, 0);
    }
}
