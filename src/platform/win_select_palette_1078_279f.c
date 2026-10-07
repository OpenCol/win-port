/* win_select_palette_1078_279f: candidate for 1078:279f (54 bytes).  Verdict: RELOCATABLE.
 *
 * In palette mode 1 (DGROUP:0x3380) remembers the palette at +0xa and
 * selects it into the window's DC (+4).
 */

#include "colonization.h"

#include "records.h"

void win_select_palette_1078_279f(MSWin *win, int pal)
{
    if (g_palette_mode == 1) {
        win->hpal = pal;
        SelectPalette(win->hdc, pal, 0);
    }
}
