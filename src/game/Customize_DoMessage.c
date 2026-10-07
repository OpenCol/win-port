/* PORT: this file's own declarations of copy_rect_to_screen_1030_e699 removed: the header declares it from the definition (W1 step 3). */
/* Customize_DoMessage: candidate for 1030:1a29 (504 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: Customize_DoMessage
 * name-source: sibling-build
 *
 * A press on one of the twelve world-option panels, held until the button comes up. The
 * caller hands over the panel `Customize_TrackMouse` says the press landed on, 1-based, and
 * zero -- a press on nothing -- returns at once.
 *
 * While `Screen_LButtonPressed` keeps saying the button is still down the mouse is hit-tested
 * again each time round. The panel is redrawn ONLY WHEN THE ANSWER CHANGES: back onto the
 * pressed panel it is filled black and the saved port blitted back through a rectangle inset
 * three pixels on the left and top and three short on the right and bottom, which is the
 * pressed look; off it, the saved port is blitted back whole. An unchanged answer redraws
 * nothing, so the loop costs one hit test per message and no blits while the mouse sits still.
 *
 * Releasing ON the panel commits it: `slot[family]` takes `n - 1` and `family` is set from it,
 * and `Customize_DrawIt` repaints all four. Releasing off it leaves the previous
 * choice standing but STILL rewrites `family` and repaints -- the commit is conditional, the
 * repaint is not.
 *
 * THE RECTANGLE ARRAY IS INDEXED FROM ZERO AND THIS FUNCTION'S PANEL NUMBER IS NOT. The shipped
 * address is `s + 0xbc + n * 8` and the array's base is 0xc4 (`Customize_DrawIt`,
 * which reads it with `slot[i]`), so the source is `&s->rects[n - 1]` with Borland folding the
 * minus one into the displacement -- a constant the comparator cannot check either way (§119).
 * `slot[family] = n - 1` at the end is the same off-by-one made explicit.
 *
 * `family` is the register local in SI and `n` the register parameter in DI (rule 53).
 *
 * RENAMED 2026-09-27 from track_panel_choice: the Mac build's Customize_DoMessage, bound by tools/mac_bind.py's proposal and checked against the body and the Mac call graph: The press-and-track loop: mouse pos, hit test, FillColor + GRPort_CopyToPort pressed look, Screen_LButtonPressed, Customize_DrawIt. Same sequence as Mac Customize::DoMessage (SetNations/SetDifficulty have their own copies, already named so); this + short; ratio 1.22.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

extern void GRPort_FillColor(GRPort *s, RECT *r, int colour);
extern void GRPort_CopyToPort(GRPort *src, GRPort *dst, RECT *sr, RECT *dr);
extern int Screen_LButtonPressed(Screen *w, char own, char remove, char wait);

/* same object: 4-byte `push cs; call ` */

extern "C" int Customize_TrackMouse(Customize *s, int x, int y);
extern "C" void Customize_DrawIt(Customize *s);

void Customize_DoMessage(Customize *s, register int n)
{
    int x;
    int y;
    RECT inner;
    RECT r;
    RECT outer;
    int prev;
    register int fam;

    if (n == 0)
        goto out;
    CopyRect(&r, &s->boxes[n - 1]);
    prev = 0;
    fam = (n - 1) / 3;
    while (Screen_LButtonPressed(&(Screen &)*s, 0, 1, 0) == 0) {
        get_mouse_pos_for(&(DialogWin &)*s, &x, &y);
        s->hit[fam] = Customize_TrackMouse(s, x, y);
        if (s->hit[fam] != prev && s->hit[fam] == n) {
            CopyRect(&inner, &r);
            CopyRect(&outer, &r);
            inner.left += 3;
            inner.top += 3;
            inner.bottom++;
            inner.right++;
            outer.right -= 3;
            outer.bottom -= 3;
            GRPort_FillColor((GRPort *)s, &r, 0);
            GRPort_CopyToPort(&s->port, (GRPort *)s, &outer, &inner);
            copy_rect_to_screen_1030_e699(s, &r);
        } else if (s->hit[fam] != prev && s->hit[fam] != n) {
            GRPort_CopyToPort(&s->port, (GRPort *)s, &r, &r);
            copy_rect_to_screen_1030_e699(s, &r);
        }
        prev = s->hit[fam];
    }
    if (prev == n)
        s->choice[fam] = n - 1;
    s->family = s->choice[fam] / 3;
    Customize_DrawIt(s);
out:
    ;
}
