/* GRPort_Init_wh_1040_b0c8: candidate for 1040:b0c8 (32 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * `GRPort_Init_wh` with a zero width and height: this module's one-line wrapper for
 * initialising a port at no size. One of NINE copies, one per module.
 *
 * The callee is a call into segment 1068 and is `extern`.
 */

#include "colonization.h"

void GRPort_Init_wh_1040_b0c8(void *p)
{
    GRPort_Init_wh(p, 0, 0);
}
