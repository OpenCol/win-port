/* grport_init_wh0: candidate for 1068:974e (31 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: grport_init_wh0
 * name-source: descriptive
 *
 * `GRPort_Init_wh(p, 0, 0)`: initialises a drawing port with its width and height both
 * given as zero, leaving the size to whatever GRPort_Init_wh does with zero.
 *
 * The call is the four-byte `push cs; call ` with no NOP, so the callee is defined here
 * as a stub (rule 47).
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

/* same object: 4-byte `push cs; call ` (rule 47) */

void grport_init_wh0(GRPort *port)
{
    GRPort_Init_wh(port, 0, 0);
}
