/* ColChild_PackQueue: candidate for 1048:13c7 (170 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ColChild_PackQueue
 * name-source: sibling-build
 *
 * Retires the message that was up and shows the next one. The count at +0x778 comes down
 * first; if any are left the sixteen queue slots shift down one, slot 0 is copied into a
 * local buffer, drawn, and the 2000ms timer restarted.
 *
 * The shift copies slot i+1 over slot i by reaching +0x239 and +0x1e9 with the same
 * `i * 0x50` -- the two addresses are one stride apart, which is the whole reason the queue
 * layout in ColChild_DrawAnyMessage can be trusted.
 *
 * RENAMED 2026-09-26 from ColChild_NextMessage: the Mac build's ColChild::PackQueue(void): this only; it retires the message that was up and shifts the sixteen queue slots down one before showing slot 0. The Mac ColChild has no NextMessage; the old name had the class prefix but not the method. The width matches.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

extern "C" void win_set_caption_1048_d48b(ColChild *w, char *s);

void ColChild_PackQueue(ColChild *w)
{
    register int i;
    char buf[0x50];

    w->queued--;
    if (w->queued != 0) {
        for (i = 0; i < w->queued; i++)
            STRCPY(w->queue[i], w->queue[i + 1]);
        STRCPY(buf, w->queue[0]);
        win_set_caption_1048_d48b(w, buf);
        w->msg_timer = start_timer(message_timer_expired, 0x7d0, 1);
    }
}
