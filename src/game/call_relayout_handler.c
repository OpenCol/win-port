/* call_relayout_handler: candidate for 1060:ae50 (34 bytes).  Verdict: EXACT.
 *
 * Invokes handler slot +192 (0xc0) of the object -- a GameWin's long at +0xc0 -- without
 * arguments, when a handler is installed.
 *
 * RENAMED 2026-09-26 from call_handler_slot_192: runs the window's +0xc0 relayout handler (decimal 192), the slot
 * set_relayout_handler fills; GameWin_Resize and GameWin_Redraw call it before copying to the screen.
 */

#include "colonization.h"

#include "classes.h"

void call_relayout_handler(GameWin *w)
{
    if (w->relayout != 0)
        (*(void (*)(void))w->relayout)();
}
