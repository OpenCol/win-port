/* port_wipe_right: candidate for 1068:b463 (84 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: port_wipe_right
 * name-source: descriptive
 *
 * Runs the wipe transition that reveals the new image from the left edge rightwards between the object's two ports: the surface of the GRPort at
 * +0 grows into the surface of the one at +0xc4, over the rectangle the caller supplies and
 * in steps of `step`. The third argument is the drawing port itself, the pointer at
 * +0x2c.
 *
 * One of SEVEN identical wrappers at 1068:b40f, b463, b4b7, b50b, b55f, b5b3 and b607 --
 * 84 bytes each, back to back, differing only in which `wing_*` transition they reach. Same
 * three accessors, same argument order, same evaluation order: one template, seven
 * transitions, and the regular spacing is what makes the family visible before any of them
 * is read.
 *
 * Three accessors, all reached by the four-byte `PUSH CS / CALL NEAR` form and so stubs here
 * (rule 47); `wing_wipe_right` is a call and is `extern`. They are
 * evaluated right to left, which is why the drawing port is fetched first and the source
 * surface last.
 */

#include "colonization.h"

#include "w16types.h"
#include "records.h"
#include "classes.h"
extern void wing_wipe_right(GrafRec *old_graf, GrafRec *new_graf, void *win, RECT *r, int step);

/* same object: 4-byte `push cs; call ` */
extern "C" void *get_win_1068_d72a(DialogWin *o);
extern "C" GrafRec *GRPort_GetWin_1068_d91d(GRPort *p);

void port_wipe_right(MagicWin *s, int step, RECT *r)
{
    wing_wipe_right(GRPort_GetWin_1068_d91d((GRPort *)s),
                           GRPort_GetWin_1068_d91d(&s->port),
                           (MSWin *)(get_win_1068_d72a(&(DialogWin &)*s)), r, step);
}
