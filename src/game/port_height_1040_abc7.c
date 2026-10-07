/* port_height_1040_abc7: candidate for 1040:abc7 (25 bytes).  Verdict: EXACT.
 * name-source: descriptive
 * size-source: 25, not the census 21. The trailing 4 bytes are the dead second epilogue.
 *
 * The drawable's HEIGHT: the `int` at +0x4. A one-line accessor, 20-odd bytes of prologue
 * and epilogue around a single move.
 *
 * The counterpart of `port_width_*` at +0x2; see one of those for what names the pair.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

int port_height_1040_abc7(GRPort *port)
{
    return port->h;
}
