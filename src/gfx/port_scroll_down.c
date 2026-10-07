/* PORT: this file's own declarations of get_win_1068_d72a, wing_scroll_down removed: the header declares them from the definition (W1 step 3). */
/* port_scroll_down: candidate for 1068:b255 (84 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: port_scroll_down
 * name-source: descriptive
 *
 * Runs the downward scroll transition between the object's two ports: the surface of the GRPort at
 * +0 grows into the surface of the one at +0xc4, over the rectangle the caller supplies and
 * in steps of `step`. The third argument is the drawing port itself, the pointer at
 * +0x2c.
 *
 * One of ELEVEN identical wrappers running from 1068:b201 to 1068:b65a -- 84 bytes each, back
 * to back but for `port_fade_through_black` and a 14-byte empty function at 1068:b351..b40e, differing only in which `wing_*` transition they reach: four scrolls, four wipes,
 * two curtains and a zoom. Same
 * three accessors, same argument order, same evaluation order: one template, eleven
 * transitions, and the regular spacing is what makes the family visible before any of them
 * is read.
 *
 * Three accessors, all reached by the four-byte `PUSH CS / CALL NEAR` form and so stubs here
 * (rule 47); `wing_scroll_down` is a call and is `extern`. They are
 * evaluated right to left, which is why the drawing port is fetched first and the source
 * surface last.
 */

#include "colonization.h"

#include "w16types.h"
#include "records.h"
#include "classes.h"

/* same object: 4-byte `push cs; call ` */
extern "C" GrafRec *GRPort_GetWin_1068_d91d(GRPort *p);

void port_scroll_down(MagicWin *s, int step, RECT *r)
{
    wing_scroll_down(GRPort_GetWin_1068_d91d((GRPort *)s),
                           GRPort_GetWin_1068_d91d(&s->port),
                           (MSWin *)(get_win_1068_d72a(&(DialogWin &)*s)), r, step);
}
