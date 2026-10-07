/* Opening_UnDoShip: candidate for 1010:0eb3 (62 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: Opening_UnDoShip
 * name-source: sibling-build
 *
 * Copies the Opening object's saved backdrop port (+0xea) onto its own port
 * (+0xc4), rectangles at +0x552 and +0x55a.
 *
 * RENAMED 2026-09-26 from colony_copy_backdrop: the Mac build's Opening::UnDoShip(void), bound by LAYOUT: both builds keep their source files' function order, and on both sides this is the first of the three functions between Opening_RefreshUpdatePort and Opening_OpeningKeys, which are already bound, paired in order; the arity agrees and the Mac body is 1.06x this one.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-
class DlgControl;
#include "records.h"
extern void GRPort_CopyToPort(GRPort *src, GRPort *dst, RECT *sr, RECT *dr);

void Opening_UnDoShip(Opening *o)
{
    GRPort_CopyToPort(&o->port2, &o->port1, &o->source_rect, &o->dest_rect);
}
