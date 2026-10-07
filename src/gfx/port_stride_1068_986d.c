/* port_stride_1068_986d: candidate for 1068:986d (25 bytes).  Verdict: EXACT.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: port_stride_1068_986d
 *
 * A port's STRIDE (+6): every caller passes a GRPort (the fill, copy and sprite blits
 * multiply it by the DIB orientation), never a RECT. Graded EXACT over the class, which also
 * places GRPort's vptr at +0. RENAMED 2026-10-02 from rect_bottom; byte-identical to
 * port_stride (1068:d7db), so it takes that name with its address.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
int port_stride_1068_986d(GRPort *p)
{
    return p->stride;
}
