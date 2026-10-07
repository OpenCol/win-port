/* port_stride: candidate for 1068:d7db (25 bytes).  Verdict: EXACT.
 * name-source: descriptive
 * size-source: 25, not the census 21. The trailing 4 bytes are the dead second epilogue.
 *
 * The `int` at +0x6 of the record. A one-line accessor: 20-odd bytes of prologue and epilogue around a single move.
 *
 * RENAMED 2026-09-26 from get_w_6_1068_d7db: the port's stride, the signed bytes per scanline MSLockGraf steps
 * the row table by; dissolve_transition multiplies a row by it to address a pixel.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

int port_stride(GRPort *port)
{
    return port->stride;
}
