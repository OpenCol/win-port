/* GRPort_ResetClip: candidate for 1068:4dba (86 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: GRPort_ResetClip
 * name-source: sibling-build
 *
 * Copies the port's wanted rectangle at +0x10 into its live rectangle at +8 and applies it to the window.
 *
 * RENAMED 2026-09-26 from port_apply_rect: the Mac build's GRPort::ResetClip(void): this only, putting the wanted rectangle (+0x10) back as the live clip (+8) -- the undo of GRPort_SetClip every drawer pairs it with. The width matches.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
#include "records.h"

extern void MSSetClip(GrafRec *w, RECT *r);
void GRPort_ResetClip(GRPort *port)
{
    SetRect(&port->rect, port->bounds.left, port->bounds.top, port->bounds.right, port->bounds.bottom);
    MSSetClip((GrafRec *)port->win, &port->rect);
}
