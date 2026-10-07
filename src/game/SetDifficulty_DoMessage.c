/* SetDifficulty_DoMessage: candidate for 1030:0f4c (514 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: SetDifficulty_DoMessage
 * name-source: sibling-build
 * size-source: 514, through the end of the 5-entry switch table after the epilogue.
 *
 * A press on one of the 5 difficulty panels, held until the button comes up -- the
 * same loop as Customize_DoMessage without the per-family arrays. The panel's
 * rectangle is picked by a `switch` on the 1-based panel number rather than indexed, a press on
 * nothing (0) returns at once, and while the button stays down the mouse is hit-tested with
 * `SetDifficulty_TrackMouse` and the panel redrawn pressed or released only when the answer changes.
 * Releasing on the panel stores it at +0xee; either way `SetDifficulty_DrawIt` repaints.
 *
 * RENAMED 2026-09-26 from track_difficulty: the Mac build's SetDifficulty::DoMessage(short), bound by LAYOUT inside an uneven gap: both builds keep source-file order, and this sits between SetDifficulty_Init and Customize_OkButtonClick, where the Mac gap holds 4 functions and the Windows one 3, so the pairing inside it is by size and arity (the Mac body is 1.13x this one, the arity agrees). The names corroborate it, which is what the pairing needed: an uneven gap alone does not settle order.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

extern void GRPort_FillColor(GRPort *s, RECT *r, int colour);
extern void GRPort_CopyToPort(GRPort *src, GRPort *dst, RECT *sr, RECT *dr);
extern int Screen_LButtonPressed(Screen *w, char own, char remove, char wait);

/* same object: 4-byte `push cs; call ` */

extern "C" int SetDifficulty_TrackMouse(SetDifficulty *s, int x, int y);
extern "C" void copy_rect_to_screen_1030_e699(SetDifficulty *s, RECT *r);
extern "C" void SetDifficulty_DrawIt(SetDifficulty *s);

void SetDifficulty_DoMessage(SetDifficulty *s, register int n)
{
    int x;
    int y;
    RECT dst_rect;
    RECT r;
    RECT src_rect;
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
    case 5:
        CopyRect(&r, &s->boxes[4]);
        break;
    }
    prev = 0;
    while (Screen_LButtonPressed(&(Screen &)*s, 0, 1, 0) == 0) {
        get_mouse_pos_for(&(DialogWin &)*s, &x, &y);
        s->hit = SetDifficulty_TrackMouse(s, x, y);
        if (s->hit != prev && s->hit == n) {
            CopyRect(&dst_rect, &r);
            CopyRect(&src_rect, &r);
            dst_rect.left += 3;
            dst_rect.top += 3;
            dst_rect.bottom++;
            dst_rect.right++;
            src_rect.right -= 3;
            src_rect.bottom -= 3;
            GRPort_FillColor((GRPort *)s, &r, 0);
            GRPort_CopyToPort(&s->port, (GRPort *)s, &src_rect, &dst_rect);
            copy_rect_to_screen_1030_e699(s, &r);
        } else if (s->hit != prev && s->hit != n) {
            GRPort_CopyToPort(&s->port, (GRPort *)s, &r, &r);
            copy_rect_to_screen_1030_e699(s, &r);
        }
        prev = s->hit;
    }
    if (prev == n)
        s->choice = n;
    SetDifficulty_DrawIt(s);
out:
    ;
}
