/* port_zoom_in: candidate for 1068:b607 (84 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: port_zoom_in
 * name-source: descriptive
 *
 * Runs the zoom-in transition between the object's two ports: the surface of the GRPort at
 * +0xc4 is the image that grows (wing_zoom_in's `source`), stretched into the drawing window; the one at +0 is only null-checked, over the rectangle the caller supplies and
 * in steps of `step`. The third argument is the drawing port itself, the pointer at
 * +0x2c.
 *
 * Three accessors, all reached by the four-byte `PUSH CS / CALL NEAR` form and so stubs here
 * (rule 47); `wing_zoom_in` is a call and is `extern`. They are evaluated
 * right to left, which is why the drawing port is fetched first and the +0 surface last.
 */

#include "colonization.h"

#include "w16types.h"
#include "records.h"
#include "classes.h"
extern void wing_zoom_in(GrafRec *old_graf, GrafRec *new_graf, void *win, RECT *r, int step);

/* same object: 4-byte `push cs; call ` */
extern "C" void *get_win_1068_d72a(DialogWin *o);
extern "C" GrafRec *GRPort_GetWin_1068_d91d(GRPort *p);

void port_zoom_in(MagicWin *s, int step, RECT *r)
{
    wing_zoom_in(GRPort_GetWin_1068_d91d((GRPort *)s),
                           GRPort_GetWin_1068_d91d(&s->port),
                           (MSWin *)(get_win_1068_d72a(&(DialogWin &)*s)), r, step);
}
