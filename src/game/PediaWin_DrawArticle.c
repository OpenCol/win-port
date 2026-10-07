/* PediaWin_DrawArticle: candidate for 1060:23e3 (311 bytes).  Verdict: RELOCATABLE.
 *
 * Draws the encyclopedia page: with no article selected (+0x84c == -1) the index is drawn
 * (PediaWin_DrawListArea); otherwise the article port (+0x818) is copied into the window's client
 * rectangle (+0x76a), the scroll bar (+0x7f0) hidden, and the article kind at +0x84a
 * dispatched through a 6-entry jump table to the cargo, unit, terrain, job, buildings or
 * founding-fathers page. copy_all_to_screen_1060_610c finishes either way.

 * SIZE: the census says 299 and the function is 311. The extra bytes are the CS-relative
 * jump table for this function's `switch`, which Borland puts in the CODE segment and
 * Ghidra's reachable-code size excludes. The reconstruction grades over the whole
 * extent unchanged; only the number moved.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

extern void GRPort_CopyToPort(GRPort *src, GRPort *dst, RECT *sr, RECT *dr);
/* same object: 4-byte `push cs; call ` (rule 47) */
extern void PediaWin_DrawListArea(PediaWin *w);

extern int port_width_1060_65ca(void *port);
extern int port_height_1060_65e3(void *port);
extern void MSHideControl_1060_669f(void *bar);
extern void PediaWin_DoCargoArticle(PediaWin *w);
extern void PediaWin_DoUnitArticle(PediaWin *w);
extern void PediaWin_DoTerrainArticle(PediaWin *w);
extern void PediaWin_DoJobArticle(PediaWin *w);
extern void PediaWin_DoBuildingsArticle(PediaWin *w);
extern void PediaWin_DoFathersArticle(PediaWin *w);

void PediaWin_DrawArticle(PediaWin *w)
{
    RECT r;

    if (w->sel == -1) {
        PediaWin_DrawListArea(w);
        copy_all_to_screen_1060_610c(w);
        return;
    }
    SetRect(&r, 0, 0, port_width_1060_65ca(&w->port), port_height_1060_65e3(&w->port));
    GRPort_CopyToPort((GRPort *)&w->port, (GRPort *)w, &r, &w->rect);
    MSHideControl_1060_669f(&w->bar);
    switch (w->kind) {
    case 0:
        PediaWin_DoCargoArticle(w);
        break;
    case 1:
        PediaWin_DoUnitArticle(w);
        break;
    case 2:
        PediaWin_DoTerrainArticle(w);
        break;
    case 3:
        PediaWin_DoJobArticle(w);
        break;
    case 4:
        PediaWin_DoBuildingsArticle(w);
        break;
    case 5:
        PediaWin_DoFathersArticle(w);
        break;
    }
    copy_all_to_screen_1060_610c(w);
}
