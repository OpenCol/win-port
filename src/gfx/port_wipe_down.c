/* PORT: this file's own declarations of get_win_1068_d72a, wing_wipe_down removed: the header declares them from the definition (W1 step 3). */
/* port_wipe_down: candidate for 1068:b50b (84 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: port_wipe_down
 * name-source: descriptive
 *
 * Runs the downward wipe transition between the object's two ports: the surface of the GRPort at
 * +0 grows into the surface of the one at +0xc4, over the rectangle the caller supplies and
 * in steps of `step`. The third argument is the destination window (an MSWin), the 
 * pointer at +0x2c.
 *
 * One of SEVEN identical wrappers at 1068:b40f, b463, b4b7, b50b, b55f, b5b3 and b607 --
 * 84 bytes each, back to back, differing only in which `wing_*` transition they reach. Same
 * three accessors, same argument order, same evaluation order: one template, seven
 * transitions, and the regular spacing is what makes the family visible before any of them
 * is read.
 *
 * Three accessors, all reached by the four-byte `PUSH CS / CALL NEAR` form and so stubs here
 * (rule 47); `wing_wipe_down` is a call and is `extern`. They are
 * evaluated right to left, which is why the drawing port is fetched first and the source
 * surface last.
 */

#include "colonization.h"

#include "w16types.h"
#include "records.h"
#include "classes.h"

/* same object: 4-byte `push cs; call ` */
extern "C" GrafRec *GRPort_GetWin_1068_d91d(GRPort *p);

void port_wipe_down(MagicWin *s, int step, RECT *r)
{
    wing_wipe_down(GRPort_GetWin_1068_d91d((GRPort *)s),
                           GRPort_GetWin_1068_d91d(&s->port),
                           (MSWin *)(get_win_1068_d72a(&(DialogWin &)*s)), r, step);
}
