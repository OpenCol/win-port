/* port_curtain_open_h: candidate for 1068:b5b3 (84 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: port_curtain_open_h
 * name-source: descriptive
 *
 * Runs the horizontal curtain-open transition between the object's two ports: the surface of the GRPort at
 * +0 grows into the surface of the one at +0xc4, over the rectangle the caller supplies and
 * in steps of `step`. The third argument is the drawing port itself, the pointer at
 * +0x2c.
 *
 * The twin of `port_zoom_in`, 84 bytes above it and identical in every respect but the transition it reaches: same three
 * accessors, same argument order, same evaluation order. Two transitions written from one
 * template.
 *
 * Three accessors, all reached by the four-byte `PUSH CS / CALL NEAR` form and so stubs here
 * (rule 47); `wing_curtain_open_h` is a call and is `extern`. They are
 * evaluated right to left, which is why the drawing port is fetched first and the source
 * surface last.
 */

#include "colonization.h"

#include "w16types.h"
#include "records.h"
#include "classes.h"
extern void wing_curtain_open_h(GrafRec *old_graf, GrafRec *new_graf, void *win, RECT *r, int step);

/* same object: 4-byte `push cs; call ` */
extern "C" void *get_win_1068_d72a(DialogWin *o);
extern "C" GrafRec *GRPort_GetWin_1068_d91d(GRPort *p);

void port_curtain_open_h(MagicWin *s, int step, RECT *r)
{
    wing_curtain_open_h(GRPort_GetWin_1068_d91d((GRPort *)s),
                           GRPort_GetWin_1068_d91d(&s->port),
                           (MSWin *)(get_win_1068_d72a(&(DialogWin &)*s)), r, step);
}
