/* port_copy_rect: candidate for 1068:a979 (58 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: port_copy_rect
 * name-source: descriptive
 *
 * Copies the object's own rectangle at +0xee from the port at +0xc4 into the object, source
 * and destination the same region -- the rectangle pointer is pushed TWICE, once for each of
 * `GRPort_CopyToPort`'s two rectangle parameters, the way `port_blit_or_copy` does it.
 *
 * The address is computed twice, `MOV AX,[bp+6] / ADD AX,0xee` in full each time, because
 * `-Od` keeps nothing between two pushes.
 *
 * `GRPort_CopyToPort` is reached by the five-byte NOP form and is `extern` (rule 47).
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
extern void GRPort_CopyToPort(GRPort *src, GRPort *dst, RECT *sr, RECT *dr);

void port_copy_rect(MagicWin *o)
{
    GRPort_CopyToPort(&o->port, (GRPort *)o, &o->rect, &o->rect);
}
