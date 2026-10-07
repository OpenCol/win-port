/* GRPort_Init_wh_1060_88c7: candidate for 1060:88c7 (32 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * `GRPort_Init_wh` with a zero width and height: this module's one-line wrapper for
 * initialising a port at no size. One of NINE copies, one per module.
 *
 * The callee is a call into segment 1068 and is `extern`.
 */

#include "colonization.h"

void GRPort_Init_wh_1060_88c7(void *port)
{
    GRPort_Init_wh(port, 0, 0);
}
