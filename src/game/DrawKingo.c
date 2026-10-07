/* PORT: this file's own declarations of Sprite_Init, copy_rect_to_screen_1050_8bfa removed: the header declares them from the definition (W1 step 3). */
/* DrawKingo: candidate for 1050:5028 (156 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: DrawKingo
 *
 * One frame of an animation: sprite resource 0x7d + n is loaded into a stack Sprite, drawn at
 * (10, 10), and the 0x50 by 0x92 rectangle from (10, 0x40) copied to the screen. The caller,
 * 1050:50c4, passes a post-incremented counter it tests against 5.
 *
 * RENAMED 2026-09-26 from draw_anim_frame: the Mac build's DrawKingo(GameWin&, short), bound by LAYOUT: both builds keep their source files' function order, and on both sides this is the one function between Popup_draw and Popup_exec, which are already bound; the arity agrees and the Mac body is 1.53x this one.
 */

#include "colonization.h"

#include "w16types.h"
/* PORT: this file's own GameWin, Sprite, GRPort removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"   /* GameWin : GRPort, DialogWin -- the two bases as their bytes, 0xc4 */
#pragma option -a

#pragma option -a-


extern RECT *Sprite_Draw(RECT *out, Sprite *s, GRPort *port, int px, int py);

/* same object: 4-byte `push cs; call ` (rule 47) */

void DrawKingo(GameWin *win, int n)
{
    Sprite s;
    RECT r;
    RECT out;

    Sprite_Init(&s, n + 0x7d);
    Sprite_Draw(&out, &s, (GRPort *)win, 10, 10);
    SetRect(&r, 10, 0x40, 0x5a, 0xd2);
    copy_rect_to_screen_1050_8bfa(win, &r);
}

