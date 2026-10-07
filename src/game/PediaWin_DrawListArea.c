/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 68 of the imported file. */
/* PORT: this file's own declarations of MSShowControl_1060_68a5 removed: the header declares it from the definition (W1 step 3). */
/* PediaWin_DrawListArea: candidate for 1060:2116 (717 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * REDRAWS THE COLONIZOPEDIA WINDOW. With an article selected (+0x84c not -1) that is all
 * `PediaWin_DrawArticle` does. Otherwise it draws the index page.
 *
 * INDEX PAGE: the background port at +0x818 is copied whole into the frame at +0x76a and the prompt
 * drawn. A sunken bevel (depth -3) goes round the list area, 25 pixels wider on the right when there
 * is more than one page (+0x850) than when there is one. Inside it, eight rows of two
 * raised slots (depth 2) are split evenly over the frame's height less 80. The scrollbar at +0x7f0 is
 * shown for more than one page and hidden otherwise, and `PediaWin_FillListArea` fills the
 * slots.
 *
 * The `SetRect` corners are and go left to right, so the first bevel starts at (27, 37).
 * `PediaWin_DrawArticle` and the list painter are reached by the NOP form and are extern; the rest are
 * stubs (rule 47).
 *
 * RENAMED 2026-09-27 from pedia_redraw: CONST PROPOSAL; 282e vacates the name first. Mac PediaWin::DrawListArea(void), CODE 20 0x0c26, 710 bytes. Callers: DrawArticle, HandleLeftClick, OnScroll, and (bsr) Show and ShowModal. Windows: PediaWin_DrawArticle, PediaWin_HandleLeftClick, PediaWin_OnScroll, PediaWin_Show, PediaWin_ShowModal (the ShowModal slot). Callees: GRPort_CopyToPort, DrawTitle, FrameInfoRect x3, Show/Draw/HideControl, and bsr DrawArticle and FillListArea. 2116 calls GRPort_CopyToPort, PediaWin_DrawTitle, FrameInfoRect x3, control_show/hide, PediaWin_DrawArticle and 282e. Arity: this. Layout: Mac Show, KeyClick, DrawTitle, DrawListArea, DrawArticle, FillStringList; Windows PediaWin_Show, _KeyClick, _DrawTitle, 2116, _DrawArticle, _FillStringList. Size 710 against 717. The existing binding on 282e was the error. (Mac edges marked bsr are BSR.L/BSR.W same-segment calls, which tools/mac_callgraph.py does not read; they were decoded from the CODE resource.)
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
extern void GRPort_CopyToPort(GRPort *src, GRPort *dst, RECT *sr, RECT *dr);
extern void FrameInfoRect(GRPort *p, RECT *r, int depth, int light, int dark);

extern void PediaWin_DrawArticle(PediaWin *w);
extern void PediaWin_FillListArea(PediaWin *w);

/* same object: 4-byte `push cs; call ` */
extern int port_width_1060_65ca(void *port);
extern int port_height_1060_65e3(void *port);
extern void PediaWin_DrawTitle(PediaWin *o);

extern void MSHideControl_1060_669f(void *control);

void PediaWin_DrawListArea(PediaWin *w)
{
    RECT r;
    register int i;

    if (w->sel != -1) {
        PediaWin_DrawArticle(w);
        return;
    }
    SetRect(&r, 0, 0, port_width_1060_65ca(&w->port), port_height_1060_65e3(&w->port));
    GRPort_CopyToPort((GRPort *)&w->port, (GRPort *)w, &r, &w->rect);
    PediaWin_DrawTitle(w);
    SetRect(&r, 0x1b, 0x25, rect_width_1060_63b5(&w->rect) + (w->pages == 1 ? 0 : 0x19) - 0x34,
            rect_height_1060_63d4(&w->rect) - 0x24);
    OffsetRect(&r, w->rect.left, w->rect.top);
    FrameInfoRect((GRPort *)w, &r, -3, 0xff, 0xf8);
    for (i = 0; i < 8; i++) {
        SetRect(&r, w->rect.left + 0x20,
                (rect_height_1060_63d4(&w->rect) - 0x50) / 8 * i + w->rect.top + 0x2a,
                (rect_width_1060_63b5(&w->rect) - 0x55) / 2 + w->rect.left + 0x1c,
                (rect_height_1060_63d4(&w->rect) - 0x50) / 8 * (i + 1) + w->rect.top + 0x27);
        FrameInfoRect((GRPort *)w, &r, 2, 0xff, 0xf8);
        SetRect(&r, (rect_width_1060_63b5(&w->rect) - 0x55) / 2 + w->rect.left + 0x20,
                (rect_height_1060_63d4(&w->rect) - 0x50) / 8 * i + w->rect.top + 0x2a,
                rect_width_1060_63b5(&w->rect) + w->rect.left - 0x39,
                (rect_height_1060_63d4(&w->rect) - 0x50) / 8 * (i + 1) + w->rect.top + 0x27);
        FrameInfoRect((GRPort *)w, &r, 2, 0xff, 0xf8);
    }
    if (w->pages != 1)
        MSShowControl_1060_68a5((DlgControl *)(&w->bar));
    else
        MSHideControl_1060_669f(&w->bar);
    PediaWin_FillListArea(w);
}
