/* PORT: this file's own declarations of copy_rect_to_screen_1068_d86d removed: the header declares it from the definition (W1 step 3). */
/* port_blit_or_copy: candidate for 1068:b188 (121 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: port_blit_or_copy
 * name-source: descriptive
 *
 * Puts a rectangle of a port on screen, by one of two routes depending on whether an
 * animation is bound to it. Bound -- the flag at +0xfa that `MagicWin_LoadFlic` sets
 * -- the pixels go through the animation's own port at +0xfc first and then to the screen;
 * unbound, `copy_rect_to_screen_1068_d86d` does it in one step.
 *
 * THE SAME RECTANGLE IS PASSED TWICE, to both of `GRPort_CopyToPort`'s rectangle parameters and
 * again to both of `CopyToScreen`'s -- four pushes of one pointer per call. Source and
 * destination are the same region, which is what a straight blit with no offset looks like
 * when the interface insists on two rectangles.
 *
 * `GRPort_CopyToPort` and `CopyToScreen` are reached by the five-byte NOP form and are `extern`
 * (rule 47); `copy_rect_to_screen_1068_d86d` by the four-byte form and is a stub here.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
extern void GRPort_CopyToPort(GRPort *src, GRPort *dst, RECT *sr, RECT *dr);
extern void CopyToScreen(GRPort *port, Screen *screen, RECT *src, RECT *dst);

/* same object: 4-byte `push cs; call ` */

void port_blit_or_copy(MagicWin *p, RECT *r)
{
    if (p->bound != 0) {
        GRPort_CopyToPort((GRPort *)p->anim, (GRPort *)p, r, r);
        CopyToScreen((GRPort *)p, (Screen *)&(DialogWin &)*p, r, r);
    } else {
        copy_rect_to_screen_1068_d86d(p, r);
    }
}
