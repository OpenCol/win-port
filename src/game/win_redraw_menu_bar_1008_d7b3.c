/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 16 of the imported file. */
/* PORT: this file's own declarations of MSDrawMenuBar, get_win_1008_e4e5 removed: the header declares them from the definition (W1 step 3). */
/* win_redraw_menu_bar_1008_d7b3: candidate for 1008:d7b3 (48 bytes, 1 caller).  Verdict: RELOCATABLE.
 *
 * The module-1008 copy of win_redraw_menu_bar. MSDrawMenuBar on the window's handle when it has a menu (pointer at +0x6a).
 */

#include "colonization.h"

#include "records.h"
#include "classes.h"
/* same object: 4-byte `push cs; call ` (rule 47) */

void win_redraw_menu_bar_1008_d7b3(EventWin *w)
{
    if (w->menu != 0)
        MSDrawMenuBar((MSWin *)(get_win_1008_e4e5((Screen *)(w))));
}
