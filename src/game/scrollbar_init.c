/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 20 of the imported file. */
/* PORT: this file's own declarations of create_MSScrollBarClass_window, dlg_control_init_1060_6b92 removed: the header declares them from the definition (W1 step 3). */
/* scrollbar_init: candidate for 1060:661c (105 bytes).  Verdict: RELOCATABLE.
 * symbol: scrollbar_init
 *
 * The scroll-bar initialiser: dlg control set-up with kind 8
 * (dlg_control_init_1060_6b92), the MSScrollBarClass window over the rectangle, range
 * 1..100 (scroll_set_range), the word at +0x24 set to 1.  The set-up is
 * -called and defined in-file as a stub.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
extern int g_stub_a;


void scrollbar_init(SScrollBar *o, void *win, int id, RECT *r, long l)
{
    dlg_control_init_1060_6b92((DlgControl *)(o), win, 8, id, r);
    o->hwnd = create_MSScrollBarClass_window(r, o, l, 1);
    scroll_set_range(o->hwnd, 1, 100);
    o->f24 = 1;
}
