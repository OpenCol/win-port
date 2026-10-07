/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 16 of the imported file. */
/* PORT: this file's own declarations of MSDrawMenuBar, get_win_1048_e6d0 removed: the header declares them from the definition (W1 step 3). */
/* win_redraw_menu_bar: candidate for 1048:e28b (48 bytes, 2 callers).  Verdict: RELOCATABLE.
 *
 * MSDrawMenuBar on the window's handle when it has a menu (pointer at +0x6a).
 */

#include "colonization.h"

#include "records.h"
#include "classes.h"
/* same object: 4-byte `push cs; call ` (rule 47) */

void win_redraw_menu_bar(EventWin *w)
{
    if (w->menu != 0)
        MSDrawMenuBar((MSWin *)(get_win_1048_e6d0((Screen *)(w))));
}
