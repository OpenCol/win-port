/* port_update: candidate for 1050:3319 (38 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Repaints the popup at DGROUP:0x2500 through `Popup_draw`, and does nothing when there is
 * no popup. `Popup_draw` (1050:4829) is the popup's full repaint, a direct call, so this is
 * the no-argument wrapper that supplies the current popup.
 *
 * It is itself a HANDLER: `Popup_make_window` pushes its address as a pointer into
 * `set_relayout_handler_1050_857d`, a slot setter, so it is installed rather than called. That is why it
 * takes no arguments and reads a global -- the slot's signature has no room for one.
 *
 * The name is a leftover from when Popup_draw was read as a slot invoker; the bytes
 * establish a direct call to the popup repaint.
 *
 * `Popup_draw` arrives as TLINK's 5-byte NOP/PUSH CS/CALL NEAR, so it stays `extern`;
 * defined in-file the call comes out four bytes wide and the function is one byte short.
 *
 * RENAMED 2026-09-27 from call_slot910_global: it calls Popup_draw on g_cur_dialog (the current popup) when there is one -- no handler slot is involved; the set_relayout_handler pair installs it as the relayout callback.
 *
 * RENAMED 2026-09-27 from redraw_current_popup: BEYOND THE PROPOSALS. Mac port_update(void), CODE 19 0x3bde, 48 bytes. Its only call is bsr Popup::draw. 3319 (void), 38 bytes, calls only Popup_draw. Neither has a direct caller. Layout: after button_click, directly before Popup::make_window (1050:3364, already bound), on both. (Mac edges marked bsr are BSR.L/BSR.W same-segment calls, which tools/mac_callgraph.py does not read; they were decoded from the CODE resource.)
 */

#include "colonization.h"

/* PORT: this file's own Popup removed: include/gen/classes.h define it once (W1 step 2). */
#include "classes.h"   /* the class, for C: the port, the two sprites and the arena (a MemBlock) as their bytes */
extern Popup *g_cur_dialog;   /* DGROUP:0x2500 */
extern void Popup_draw(Popup *w);

void port_update(void)
{
    if (g_cur_dialog != 0)
        Popup_draw(g_cur_dialog);
}
