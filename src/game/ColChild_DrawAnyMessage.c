/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 43 of the imported file. */
/* ColChild_DrawAnyMessage: candidate for 1048:12c6 (257 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ColChild_DrawAnyMessage
 *
 * Shows one of a colony child window's messages, or QUEUES it if one is already up. The
 * text is the window's own message at +0x149 when +0x75e says it has one, otherwise the
 * caller's string; either way it goes through a 0x50-byte local buffer first, which is what
 * makes the queue slots and the live message the same width.
 *
 * The queue is sixteen slots of 0x50 bytes at +0x1e9 and +0x778 is how many are waiting.
 * When nothing is up the message is drawn and a 2000ms timer started; when something is,
 * the text is appended and nothing else happens, and a full queue silently drops it.
 *
 * The layout came out of the offsets and would not close any other way: +0x1e9 is BEFORE
 * +0x75e, so the queue sits between the own-message buffer and the flags rather than after
 * them, and `ColChild_PackQueue` right after this function confirms the 0x50 stride by reaching
 * slot one at +0x239.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

extern "C" void ColChild_GetMessageColors(ColChild *w, int kind, int *a, int *b);
extern "C" void win_set_caption_1048_d48b(ColChild *w, char *s);

void ColChild_DrawAnyMessage(ColChild *w, int mode, char *msg)
{
    int a;
    int b;
    char buf[0x50];

    if (w->active != 0 || msg != 0) {
        ColChild_GetMessageColors(w, w->kind, &a, &b);
        if (w->active != 0)
            STRCPY(buf, w->own_msg);
        else
            STRCPY(buf, msg);
        if (w->queued == 0) {
            if (mode != 0)
                win_set_caption_1048_d48b(w, buf);
            w->msg_timer = start_timer((void*)(message_timer_expired), 0x7d0, 1);
            w->queued++;
        } else {
            if (w->queued <= 0xf) {
                STRCPY(w->queue[w->queued], buf);
                w->queued++;
            }
        }
    }
}
