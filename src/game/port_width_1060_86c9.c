/* port_width_1060_86c9: candidate for 1060:86c9 (25 bytes).  Verdict: EXACT.
 * name-source: descriptive
 * size-source: 25, not the census 21. The trailing 4 bytes are the dead second epilogue.
 *
 * The drawable's WIDTH: the `int` at +0x2. A one-line accessor, 20-odd bytes of prologue
 * and epilogue around a single move.
 *
 * Named from the pair. Every module has a +0x2 reader and a +0x4 reader, and the callers
 * use them together: `SetRect(&r, 0, 0, port_width(p), port_height(p))`, or as
 * `port_width(p) + x + 2` for a right edge, or `port_width(p) / 2` to centre. Two
 * callers name the result `width` outright, and `Opening_DoShip` centres a sprite
 * with `port_width(o) >> 1`. The argument is a port, a surface, a picture or a flic --
 * or a window, which the per-module FORWARDER passes straight through to the same
 * accessor, so a window's +0x2 is the same field.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

int port_width_1060_86c9(GRPort *port)
{
    return port->w;
}
