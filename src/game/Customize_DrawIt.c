/* Customize_DrawIt: candidate for 1030:12a1 (246 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: Customize_DrawIt
 * name-source: sibling-build
 *
 * Repaints the four chosen panels of this module's screen: the whole 640x480 port is copied
 * back from the saved one at +0x178 first, then each of the four slots at +0x12c names a
 * rectangle in the twelve-entry table at +0xc4, which is outlined and filled in.
 *
 * The outline colour is the choice: slot `i` drawn in 0x0c when the word at +0x1a2 matches
 * `slot[i] / 3`, and in 0xfa otherwise. Dividing the slot by three is what groups the twelve
 * rectangles into four families, so +0x1a2 holds a family and +0x12c a member of it.
 *
 * `GRPort_CopyToPort` is given the same rectangle twice, which is a straight blit with no offset --
 * the same idiom `port_blit_or_copy` uses.
 *
 * `TextColor` is declared here with an `int` parameter and not a `char` one, and the bytes
 * insist: `TextColor(0xfa)` through a `char` prototype truncates to -6 and compiles to the
 * two-byte `6A FA`, where the shipped code has the three-byte `68 FA 00`. The callee really
 * does take one byte -- this module's prototype is what widened, which is rule 55 and a fact
 * about this translation unit rather than about `TextColor`.
 *
 * `Customize_DrawBoxText` is reached by the five-byte NOP form and is `extern`; `copy_all_to_screen_1030_e504` by
 * the four-byte form and is a stub here (rule 47).
 *
 * RENAMED 2026-09-26 from draw_selected_panels: the Mac build's Customize::DrawIt(void): this only, and it stands exactly where SetNations_DrawIt stands in its twins -- Init calls it before installing the handlers, and the press tracker calls it on release.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

extern void GRPort_CopyToPort(GRPort *src, GRPort *dst, RECT *sr, RECT *dr);

extern void GRPort_DrawRect(GRPort *s, RECT *r);
extern void Customize_DrawBoxText(Customize *s, int choice);

/* same object: 4-byte `push cs; call ` */

void Customize_DrawIt(Customize *s)
{
    RECT r;
    register int i;

    SetRect(&r, 0, 0, 0x280, 0x1e0);
    GRPort_CopyToPort(&s->port, (GRPort *)s, &r, &r);
    for (i = 0; i < 4; i++) {
        CopyRect(&r, &s->boxes[s->choice[i]]);
        if (s->family == s->choice[i] / 3)
            TextColor(0x0c);
        else
            TextColor(0xfa);
        GRPort_DrawRect((GRPort *)s, &r);
        Customize_DrawBoxText(s, s->choice[i]);
    }
    copy_all_to_screen_1030_e504(s);
}
