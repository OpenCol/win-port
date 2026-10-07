/* ColChild_Center: candidate for 1048:0520 (379 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ColChild_Center
 * name-source: sibling-build
 *
 * Puts a window where it will fit. On a desktop exactly 640 wide, a window wider than 640 or
 * taller than 480 is MOVED so its client area starts at the screen's corner -- the frame's own
 * left and top are subtracted off, so the border hangs outside the desktop rather than eating
 * into the client area. Anything else is simply centred.
 *
 * THREE DIFFERENT TOPS, chosen by the frame's height. Over 0x1ca the frame's own top is used;
 * between 0x1c1 and 0x1ca a flat -10; at 0x1c0 or under, zero. The three are the menu bar being
 * present, being absent, and the window being short enough not to matter.
 *
 * The test is written `<= 0x1ca` with the tall case in the `else`: written the other way round
 * -- `> 0x1ca` with the tall case first -- the shipped's direct `JG NEAR` into the tall arm
 * becomes a `JLE SHORT` over it, which is a byte shorter and in the wrong place (rule 12).
 *
 * The desktop rectangle is reused as the scratch for the new position, so `MSGetDesktopSize`'s
 * answer is gone by the time the window is moved.
 *
 * RENAMED 2026-09-26 from place_or_center: the Mac build's ColChild::Center(void): this only, placing the window where it fits (moved to the corner on a 640 desktop, otherwise centred). The width matches.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

/* same object: 4-byte `push cs; call ` (rule 47) */

extern "C" void win_move_to_1048_d7e4(DialogWin *w, int x, int y);
extern "C" void win_center_1048_d80b(DialogWin *w);

void ColChild_Center(ColChild *w)
{
    RECT r;

    MSGetDesktopSize(&r);
    if (rect_width_1048_d69b(&r) == 0x280
            && (port_width_1048_d4f6(w) > 0x280 || port_height_1048_d7c5(w) > 0x1e0)) {
        if (rect_height_1048_d6ba(&w->rect) <= 0x1ca) {
            if (rect_height_1048_d6ba(&w->rect) > 0x1c0) {
                SetRect(&r, -w->rect.left, -10,
                        port_width_1048_d4f6(w) - w->rect.left,
                        port_height_1048_d7c5(w) - 10);
            } else {
                SetRect(&r, -w->rect.left, 0,
                        port_width_1048_d4f6(w) - w->rect.left,
                        port_height_1048_d7c5(w));
            }
        } else {
            SetRect(&r, -w->rect.left, -w->rect.top,
                    port_width_1048_d4f6(w) - w->rect.left,
                    port_height_1048_d7c5(w) - w->rect.top);
        }
        win_move_to_1048_d7e4(&(DialogWin &)*w, r.left, r.top);
    } else {
        win_center_1048_d80b(&(DialogWin &)*w);
    }
}
