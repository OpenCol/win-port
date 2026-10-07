/* port_height_1068_9886: candidate for 1068:9886 (25 bytes).  Verdict: EXACT.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: port_height_1068_9886
 *
 * A port's HEIGHT (+4): every caller passes a GRPort, never a RECT. RENAMED 2026-10-02 from
 * rect_right; byte-identical to the port_height family (analysis/re-exact-dupes.json).
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
int port_height_1068_9886(GRPort *p)
{
    return p->h;
}
