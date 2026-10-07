/* call_handler_slot_1894: candidate for 1048:d885 (40 bytes).  Verdict: EXACT.
 *
 * Calls the ColChild's resize handler (+0x766, next to the +0x762 repaint handler) with the
 * size code, when one is installed. Its one caller is ColChild_OnSize, which passes its own
 * size code on after re-laying the client rectangle.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

void call_handler_slot_1894(ColChild *w, int size_code)
{
    if (w->resize != 0)
        (*(void (*)(int))w->resize)(size_code);
}
