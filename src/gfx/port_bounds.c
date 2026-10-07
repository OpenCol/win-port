/* port_bounds: candidate for 1068:d747 (27 bytes).  Verdict: EXACT.
 *
 * The record's member 16 bytes in, as a pointer.
 *
 * RENAMED 2026-09-26 from ptr_plus_16: the address of the port's bounds rectangle at +0x10, which
 * GRPort_InitStruct sets to (0,0,w,h); the sprite loaders and port_copy_full CopyRect it out, and
 * port_grab_from_window hands it to wing_grab_from_window.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: port_bounds
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
RECT *port_bounds(GRPort *p)
{
    return &p->bounds;
}
