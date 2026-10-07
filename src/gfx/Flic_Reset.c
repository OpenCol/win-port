/* Flic_Reset: candidate for 1068:2715 (112 bytes, 3 callers).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: Flic_Reset
 * name-source: sibling-build
 *
 * Restores the saved 32-bit field, clears two counters, hands the object's 32-bit datum
 * (a long: pushed as one dword, where a pointer is two words) to MSPositionFile on the flic's file at +0xa6 -- the file goes back to that offset -- and cancels the timer at +0x56e when one is set.
 *
 * RENAMED 2026-09-26 from anim_stop: the Mac build's Flic::Reset(void): this only, restoring the saved state, clearing the counters and cancelling the timer. The width matches.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
#include "records.h"
/* PORT: this file's own Flic removed: include/gen/classes.h define it once (W1 step 2). */
/* same object: 4-byte `push cs; call ` (rule 47) */

void Flic_Reset(Flic *o)
{
    o->pos = (unsigned char *)o->frame2;
    o->frame_no = o->rect_index = 0;
    MSPositionFile_1068_9705(&o->file, o->frame_offset);
    if (o->timer != -1) {
        ResetTimer(o->timer);
        o->timer = -1;
    }
}
