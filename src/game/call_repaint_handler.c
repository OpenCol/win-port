/* call_repaint_handler: candidate for 1048:d941 (34 bytes).  Verdict: EXACT.
 *
 * Invokes the ColChild's repaint handler (+0x762, the pointer StatWin_Init stores)
 * without arguments, when one is installed.
 *
 * RENAMED 2026-09-26 from call_handler_slot_1890: runs the window's +0x762 repaint handler (decimal 1890), the slot
 * set_repaint_handler fills, when one is set.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

void call_repaint_handler(ColChild *w)
{
    if (w->repaint != 0)
        (*(void (*)(void))w->repaint)();
}
