/* Customize_TrackMouse: candidate for 1030:19b2 (119 bytes).  Verdict: EXACT.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: Customize_TrackMouse
 * name-source: sibling-build
 *
 * The first of twelve rectangles stored at +0xc4 of a record that contains the point, reported one-based, or 0 when none does.
 *
 * RENAMED 2026-09-27 from rect12_hit: the Mac build's Customize::TrackMouse(short, short), bound by source layout (tools/mac_bind.py): the Customize dialog's hit test over its twelve boxes at +0xc4, arity 3 with this, body ratio 1.19.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
int Customize_TrackMouse(Customize *o, int x, int y)
{
    int found = 0;
    int i;
    for (i = 0; i < 12; i++) {
        if (o->boxes[i].left <= x && o->boxes[i].right >= x &&
            o->boxes[i].top <= y && o->boxes[i].bottom >= y) {
            found = i + 1;
            break;
        }
    }
    return found;
}
