/* port_height_1070_98bc: candidate for 1070:98bc (25 bytes).  Verdict: EXACT.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: port_height_1070_98bc
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
#pragma option -a
#include "classes.h"
#pragma option -a-
class DlgControl;

int port_height_1070_98bc(GRPort *port)
{
    return port->h;
}
