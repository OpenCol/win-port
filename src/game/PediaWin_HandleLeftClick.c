/* PediaWin_HandleLeftClick: candidate for 1060:2c0a (1353 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * A CLICK ON THE COLONIZOPEDIA INDEX, for `PediaWin_LeftMouseClick`. With an article already open
 * (+0x84c not -1) the click closes it: the article id goes back to -1 and the index page is redrawn.
 *
 * Otherwise it hit-tests the sixteen slots `PediaWin_DrawListArea` drew -- eight rows of two, split
 * over the frame height less 80 -- and the slot's entry is `page * 16 + row * 2 - 16` for the left column
 * and one more for the right. A slot past the entry count is not a hit, so the click falls through to
 * the other column and then to the next row.
 *
 * ON A HIT the slot is redrawn as pressed: the background is copied out of the port at +0x818, put back
 * two pixels down and right, given a sunken bevel and the entry's name again, and the whole window goes
 * to the screen. The article id is then set, which is what makes the tail draw the article.
 *
 * Both name positions are quarter-width offsets, spelled `(width - 85) / 2 / 2` and, in the right-hand
 * column, `(width - 85) / 2 + left + (width - 85) / 2 / 2 + 31`: the shipped code divides twice rather
 * than by four. It compiled RELOCATABLE on the first build.
 *
 * RENAMED 2026-09-26 from pedia_click: the Mac build's PediaWin::HandleLeftClick(short, short): this plus x and y; PediaWin_LeftMouseClick (the installed handler) hands it the click. The width matches.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

extern void GRPort_CopyToPort(GRPort *src, GRPort *dst, RECT *sr, RECT *dr);
extern void FrameInfoRect(GRPort *p, RECT *r, int depth, int light, int dark);
extern void GRPort_String(GRPort *port, unsigned *font, char *s, int x, int y, int size);

/* same object: 4-byte `push cs; call ` */

extern void PediaWin_DrawListArea(PediaWin *w);

extern void PediaWin_DrawArticle(PediaWin *w);
#include "records.h"
extern PediaEntry *PediaWin_CurrentEntry(PediaWin *w);
extern char *PediaWin_PediaName(PediaWin *w, void *entry);

void PediaWin_HandleLeftClick(PediaWin *w, int x, int y)
{
    RECT r;
    RECT save;
    register int i;
    register int px;

    px = x;
    if (w->sel != -1) {
        w->sel = -1;
        PediaWin_DrawListArea(w);
        copy_all_to_screen_1060_610c(w);
        return;
    }
    w->sel = -1;
    for (i = 0; i < 8; i++) {
        SetRect(&r, w->rect.left + 0x20,
                (rect_height_1060_63d4(&w->rect) - 0x50) / 8 * i + w->rect.top + 0x2a,
                (rect_width_1060_63b5(&w->rect) - 0x55) / 2 + w->rect.left + 0x1c,
                (rect_height_1060_63d4(&w->rect) - 0x50) / 8 * (i + 1) + w->rect.top + 0x27);
        if (px > r.left && px < r.right && y > r.top && y < r.bottom
            && w->page * 16 + i * 2 - 16 <= w->count - 1) {
            w->sel = w->page * 16 + i * 2 - 16;
            save = r;
            GRPort_CopyToPort((GRPort *)&w->port, (GRPort *)w, &save, &r);
            OffsetRect(&save, -w->rect.left, -w->rect.top);
            OffsetRect(&r, 2, 2);
            GRPort_CopyToPort((GRPort *)&w->port, (GRPort *)w, &save, &r);
            FrameInfoRect((GRPort *)w, &r, -2, 0xff, 0xf8);
            GRPort_String((GRPort *)w, (unsigned *)&g_heading_font,
                                 PediaWin_PediaName(w, PediaWin_CurrentEntry(w)),
                                 (rect_width_1060_63b5(&w->rect) - 0x55) / 2 / 2
                                   + w->rect.left + 0x1f,
                                 (rect_height_1060_63d4(&w->rect) - 0x50) / 8 / 2
                                   + ((rect_height_1060_63d4(&w->rect) - 0x50) / 8 * i
                                      + w->rect.top) + 0x29,
                                 0);
            copy_all_to_screen_1060_610c(w);
            goto done;
        }
        SetRect(&r, (rect_width_1060_63b5(&w->rect) - 0x55) / 2 + w->rect.left + 0x20,
                (rect_height_1060_63d4(&w->rect) - 0x50) / 8 * i + w->rect.top + 0x2a,
                rect_width_1060_63b5(&w->rect) + w->rect.left - 0x39,
                (rect_height_1060_63d4(&w->rect) - 0x50) / 8 * (i + 1) + w->rect.top + 0x27);
        if (px > r.left && px < r.right && y > r.top && y < r.bottom
            && w->page * 16 + i * 2 - 15 <= w->count - 1) {
            w->sel = w->page * 16 + i * 2 - 15;
            save = r;
            GRPort_CopyToPort((GRPort *)&w->port, (GRPort *)w, &save, &r);
            OffsetRect(&save, -w->rect.left, -w->rect.top);
            OffsetRect(&r, 2, 2);
            GRPort_CopyToPort((GRPort *)&w->port, (GRPort *)w, &save, &r);
            FrameInfoRect((GRPort *)w, &r, -2, 0xff, 0xf8);
            GRPort_String((GRPort *)w, (unsigned *)&g_heading_font,
                                 PediaWin_PediaName(w, PediaWin_CurrentEntry(w)),
                                 (rect_width_1060_63b5(&w->rect) - 0x55) / 2 + w->rect.left
                                   + (rect_width_1060_63b5(&w->rect) - 0x55) / 2 / 2 + 0x1f,
                                 (rect_height_1060_63d4(&w->rect) - 0x50) / 8 / 2
                                   + ((rect_height_1060_63d4(&w->rect) - 0x50) / 8 * i
                                      + w->rect.top) + 0x29,
                                 0);
            copy_all_to_screen_1060_610c(w);
            goto done;
        }
    }
done:
    if (w->sel != -1)
        PediaWin_DrawArticle(w);
}
