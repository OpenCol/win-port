/* port_surface_1068_989f: candidate for 1068:989f (29 bytes).  Verdict: EXACT.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: port_surface_1068_989f
 * name-source: descriptive
 * size-source: 29, not the census 25. The trailing 4 bytes are the dead second epilogue.
 *
 * The pointer at +0x18 of the record. A one-line accessor: 20-odd bytes of prologue and epilogue around a single move.
 *
 * RENAMED 2026-09-26 from get_p_18_1068_989f: the port's locked DIB surface. MSLockGraf fills +0x18 from
 * wing_dib_pointer and GRPort_CopyToPort warns when it is null; GRPort_CopyToPort, wipe_rows and the scaled sprite
 * blitters pass it to the sub_1090 assembly blitters as the pixel base.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

void *port_surface_1068_989f(GRPort *port)
{
    return port->bits;
}
