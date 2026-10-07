/* port_scroll_left: candidate for 1068:b2a9 (84 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: port_scroll_left
 * name-source: descriptive
 *
 * Runs the leftward scroll transition between the object's two ports: the surface of the GRPort at
 * +0 grows into the surface of the one at +0xc4, over the rectangle the caller supplies and
 * in steps of `step`. The third argument is the drawing port itself, the pointer at
 * +0x2c.
 *
 * One of ELEVEN identical wrappers running from 1068:b201 to 1068:b65a -- 84 bytes each, back
 * to back but for `port_fade_through_black` and `empty_1068_b401` at b351..b40e, differing only in which `wing_*` transition they reach: four scrolls, four wipes,
 * two curtains and a zoom. Same
 * three accessors, same argument order, same evaluation order: one template, eleven
 * transitions, and the regular spacing is what makes the family visible before any of them
 * is read.
 *
 * Three accessors, all reached by the four-byte `PUSH CS / CALL NEAR` form and so stubs here
 * (rule 47); `wing_scroll_left` is a call and is `extern`. They are
 * evaluated right to left, which is why the drawing port is fetched first and the source
 * surface last.
 */

#include "colonization.h"

#include "w16types.h"
#include "records.h"
#include "classes.h"
extern void wing_scroll_left(GrafRec *old_graf, GrafRec *new_graf, void *win, RECT *r, int step);

/* same object: 4-byte `push cs; call ` */
extern "C" void *get_win_1068_d72a(DialogWin *o);
extern "C" GrafRec *GRPort_GetWin_1068_d91d(GRPort *p);

void port_scroll_left(MagicWin *s, int step, RECT *r)
{
    wing_scroll_left(GRPort_GetWin_1068_d91d((GRPort *)s),
                           GRPort_GetWin_1068_d91d(&s->port),
                           (MSWin *)(get_win_1068_d72a(&(DialogWin &)*s)), r, step);
}
