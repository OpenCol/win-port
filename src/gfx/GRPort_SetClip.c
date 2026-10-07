/* PORT: this file's own declarations of MSSetClip removed: the header declares it from the definition (W1 step 3). */
/* GRPort_SetClip: candidate for 1068:4e10 (120 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: GRPort_SetClip
 * name-source: sibling-build
 *
 * Sets the port's live rectangle from the caller's, clips it to the port's bounds at +0x10, and applies it to the window.
 *
 * RENAMED 2026-09-26 from port_set_clip: the Mac build's GRPort::SetClip(Rect*): this plus a rectangle, clipped to the bounds and applied. The width matches.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
#include "records.h"

void GRPort_SetClip(GRPort *port, RECT *r)
{
    SetRect(&port->rect, r->left, r->top, r->right, r->bottom);
    IntersectRect(&port->rect, &port->rect, &port->bounds);
    MSSetClip((GrafRec *)port->win, &port->rect);
}
