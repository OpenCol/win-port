/* PORT: this file's own declarations of GRPort_ChangeColor removed: the header declares it from the definition (W1 step 3). */
/* GRPort_ChangeColor_all: candidate for 1068:517c (44 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: GRPort_ChangeColor_all
 * name-source: descriptive
 *
 * `GRPort_ChangeColor(p, p->rect, from, to)`: replaces one colour index with another over the
 * port's OWN rectangle at +8. It is to GRPort_ChangeColor exactly what `GRPort_FillColor_all`
 * (immediately before it) is to GRPort_FillColor, and is named to match.
 *
 * `from` and `to` are `unsigned char` in GRPort_ChangeColor's definition but pass through
 * here as whole words straight from the frame, so they are `int` in this file.
 *
 * The call has a NOP before its `push cs`, the five-byte form TLINK leaves for an `extern`,
 * so the callee is declared here and not defined (rule 47).
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

void GRPort_ChangeColor_all(GRPort *p, int from, int to)
{
    GRPort_ChangeColor(p, &p->rect, from, to);
}
