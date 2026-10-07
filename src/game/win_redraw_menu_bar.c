/* win_redraw_menu_bar: candidate for 1048:e28b (48 bytes, 2 callers).  Verdict: RELOCATABLE.
 *
 * MSDrawMenuBar on the window's handle when it has a menu (pointer at +0x6a).
 */

#include "colonization.h"

#include "records.h"
#include "classes.h"
/* same object: 4-byte `push cs; call ` (rule 47) */
extern void *get_win_1048_e6d0(EventWin *w);
extern void MSDrawMenuBar(void *hwnd);

void win_redraw_menu_bar(EventWin *w)
{
    if (w->menu != 0)
        MSDrawMenuBar((MSWin *)(get_win_1048_e6d0(w)));
}
