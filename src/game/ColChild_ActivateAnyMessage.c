/* ColChild_ActivateAnyMessage: candidate for 1048:1471 (52 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ColChild_ActivateAnyMessage
 * name-source: sibling-build
 *
 * Activates a ColChild's message of the given kind and draws it (mode 1, no argument). The
 * same two calls as colony_activate_message at 1010:c04f, which hard-codes the colony
 * window; this one takes the window, so it is the general form and that one is the caller's
 * convenience wrapper.
 *
 * Both callees are -called and so live in this object; stubs place the calls.
 *
 * RENAMED 2026-09-27 from activate_and_draw_message: 10 bytes, calls ActivateMessage then DrawAnyMessage, only caller move_unit, as on the Mac
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

extern "C" void ColChild_ActivateMessage(ColChild *child, int kind, long arg);
extern "C" void ColChild_DrawAnyMessage(ColChild *w, int mode, long msg);

void ColChild_ActivateAnyMessage(ColChild *w, int kind, long ms)
{
    ColChild_ActivateMessage(w, kind, ms);
    ColChild_DrawAnyMessage(w, 1, 0L);
}
