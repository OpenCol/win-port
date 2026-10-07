/* PORT: this file's own declarations of port_bounds removed: the header declares it from the definition (W1 step 3). */
/* port_copy_full: candidate for 1068:a926 (83 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: port_copy_full
 * name-source: descriptive
 *
 * `port_copy_rect`'s wider sibling: the same blit from the port at +0xc4 into the
 * object with source and destination the same region, but over the PORT'S OWN rectangle
 * rather than the object's. `port_bounds` is what fetches it -- the port's second
 * rectangle, at +0x10 of the port -- and it is copied into a local first.
 *
 * The local copy is not redundant: `GRPort_CopyToPort` is given the same pointer for both of its
 * rectangle parameters and writes through it, so handing it the port's own rectangle would
 * have the blit modify what it is reading.
 *
 * `port_bounds` is reached by the four-byte form and is a stub here;
 * `GRPort_CopyToPort` by the five-byte NOP form and is `extern` (rule 47).
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

extern void GRPort_CopyToPort(GRPort *src, GRPort *dst, RECT *sr, RECT *dr);

/* same object: 4-byte `push cs; call ` */

void port_copy_full(MagicWin *o)
{
    RECT r;

    CopyRect(&r, port_bounds(&o->port));
    GRPort_CopyToPort(&o->port, (GRPort *)o, &r, &r);
}
