/* CDControlWin_DrawTrack: candidate for 1040:9de6 (197 bytes).  Verdict: RELOCATABLE.
 * symbol: CDControlWin_DrawTrack
 * name-source: sibling-build
 *
 * Paints the small number box at (10, 12)-(58, 60) of the window: filled
 * with the colour byte at DGROUP:0x337e, colour 12 swapped in, the word at
 * +0xa58 written in decimal with the small font (DGROUP:0x4b1e) at (35, 35),
 * and the box copied to the screen (-called, defined in-file as a stub).
 *
 * RENAMED 2026-09-26 from draw_gold_box: the Mac build's CDControlWin::DrawTrack(void): this only, printing the word at +0xa58 -- the track number ButtonProc steps -- in the panel's number box, and ButtonProc redraws it after every track change.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

extern int g_stub_a;

extern void GRPort_FillColor(GRPort *w, RECT *r, int color);

extern void GRPort_String(GRPort *port, unsigned *font, char *s, int x, int y, int align);

extern void copy_rect_to_screen_1040_af03(void *w, RECT *r);

void CDControlWin_DrawTrack(CDControlWin *w)
{
    char buf[10];
    RECT r;

    SetRect(&r, 10, 12, 0x3a, 0x3c);
    OffsetRect(&r, w->rect.left, w->rect.top);
    GRPort_FillColor((GRPort *)w, &r, g_draw_color);
    TextColor(12);
    LTOA1(w->track, buf, 10);
    GRPort_String((GRPort *)w, (unsigned *)&g_font_small, buf, w->rect.left + 0x23, w->rect.top + 0x23, 0);
    copy_rect_to_screen_1040_af03(w, &r);
}
