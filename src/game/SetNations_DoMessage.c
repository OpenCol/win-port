/* SetNations_DoMessage: candidate for 1030:063b (490 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: SetNations_DoMessage
 * name-source: sibling-build
 * size-source: 490, through the end of the 4-entry switch table after the epilogue.
 *
 * A press on one of the 4 nation-choice panels, held until the button comes up -- the
 * same loop as Customize_DoMessage without the per-family arrays. The panel's
 * rectangle is picked by a `switch` on the 1-based panel number rather than indexed, a press on
 * nothing (0) returns at once, and while the button stays down the mouse is hit-tested with
 * `SetNations_TrackMouse` and the panel redrawn pressed or released only when the answer changes.
 * Releasing on the panel stores it at +0xe6; either way `SetNations_DrawIt` repaints.
 *
 * RENAMED 2026-09-26 from track_nation_choice: the Mac build's SetNations::DoMessage(short), bound by LAYOUT inside an uneven gap: both builds keep source-file order, and this sits between SetNations_Init and SetDifficulty_DrawIt, where the Mac gap holds 4 functions and the Windows one 5, so the pairing inside it is by size and arity (the Mac body is 1.13x this one, the arity agrees). The names corroborate it, which is what the pairing needed: an uneven gap alone does not settle order.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

extern void GRPort_FillColor(GRPort *s, RECT *r, int colour);
extern void GRPort_CopyToPort(GRPort *src, GRPort *dst, RECT *sr, RECT *dr);
extern int Screen_LButtonPressed(Screen *w, char own, char remove, char wait);

/* same object: 4-byte `push cs; call ` */

extern "C" int SetNations_TrackMouse(SetNations *s, int x, int y);
extern "C" void copy_rect_to_screen_1030_e699(SetNations *s, RECT *r);
extern "C" void SetNations_DrawIt(SetNations *s);

void SetNations_DoMessage(SetNations *s, register int n)
{
    int x;
    int y;
    RECT inner;
    RECT r;
    RECT outer;
    int prev;

    if (n == 0)
        goto out;
    switch (n) {
    case 1:
        CopyRect(&r, &s->boxes[0]);
        break;
    case 2:
        CopyRect(&r, &s->boxes[1]);
        break;
    case 3:
        CopyRect(&r, &s->boxes[2]);
        break;
    case 4:
        CopyRect(&r, &s->boxes[3]);
        break;
    }
    prev = 0;
    while (Screen_LButtonPressed(&(Screen &)*s, 0, 1, 0) == 0) {
        get_mouse_pos_for(&(DialogWin &)*s, &x, &y);
        s->hit = SetNations_TrackMouse(s, x, y);
        if (s->hit != prev && s->hit == n) {
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
        } else if (s->hit != prev && s->hit != n) {
            GRPort_CopyToPort(&s->port, (GRPort *)s, &r, &r);
            copy_rect_to_screen_1030_e699(s, &r);
        }
        prev = s->hit;
    }
    if (prev == n)
        s->choice = n;
    SetNations_DrawIt(s);
out:
    ;
}
