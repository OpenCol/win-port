/* GRPort_FillColor_all: candidate for 1068:5154 (40 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: GRPort_FillColor_all
 *
 * GRPort_FillColor over the port's own rectangle (+8).
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void GRPort_FillColor(GRPort *p, RECT *rect, int color);
void GRPort_FillColor_all(GRPort *p, int color)
{
    GRPort_FillColor(p, &p->rect, color);
}
