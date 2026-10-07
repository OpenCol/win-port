/* popup_setup_plain_1048_e387: candidate for 1048:e387 (39 bytes).  Verdict: RELOCATABLE.
 *
 * Popup_init with three zero longs and a zero word after the pointer.
 *
 * RENAMED 2026-09-26 from text_call_0859_1048_e387: Popup_init with every option zero -- three null longs and a
 * zero word after the popup.
 */

#include "colonization.h"

#include "w16types.h"
/* PORT: this file's own Popup removed: include/gen/classes.h define it once (W1 step 2). */   /* the class, for C: the port, the two sprites and the arena (a MemBlock) as their bytes */
#include "classes.h"
extern void Popup_init(Popup *popup, GameWin *win, GRPort *port, RECT *r, int kind);

void popup_setup_plain_1048_e387(void *p)
{
    Popup_init((Popup *)p, 0L, 0L, 0L, 0);
}
