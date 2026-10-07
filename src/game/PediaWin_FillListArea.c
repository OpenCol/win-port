/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 44, 47, 55, 63 of the imported file. */
/* PORT: this file's own declarations of PediaWin_PediaName, VList_Head_1060_68db, VList_Next_1060_68fb removed: the header declares them from the definition (W1 step 3). */
/* PediaWin_FillListArea: candidate for 1060:282e (537 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Draws one page of the Colonizopedia's index: two columns of eight entry names, white, in the
 * heading font. The entries come from the `VList` member at +0x83e, walked with `VList_Head` /
 * `VList_Next`; page N starts by skipping `N * 16 - 16` of them, so a page is sixteen entries and
 * page 1 skips nothing (the constructor sets +0x852 to 1).
 *
 * The layout is computed from the window frame on every call, twice per entry: the usable area
 * is the frame less 0x55 across and 0x50 down; a row is an eighth of the height, and each name
 * sits half a row below its row's top, 0x28 below the frame. The left column starts a quarter
 * of the usable width in, plus 0x1e; the right column adds another half.
 *
 * The loop stops at eight rows or at the end of the list, whichever is first -- and the right
 * column checks for the end on its own, so an odd final entry leaves the right half empty.
 *
 * `/ 2` is `CWD / SUB AX,DX / SAR AX,1`, Borland's signed halving, and `w->page * 16 - 16 > i`
 * is written with the count on the left because the shipped compares `CMP AX,SI / JG`. Five
 * module helpers come through the 4-byte `PUSH CS` form and are stubs here (rule 47).
 *
 * RENAMED 2026-09-26 from pedia_draw_list_page: the Mac build's PediaWin::DrawListArea(void): this only, drawing one page of the index list. The width matches.
 *
 * RENAMED 2026-09-27 from PediaWin_DrawListArea: BEYOND THE PROPOSALS. It vacates PediaWin_DrawListArea for 1060:2116. Mac PediaWin::FillListArea(void), CODE 20 0x1326, 536 bytes. Its seq is VList Head/Next x3, TextColor, PediaName x2, String x2, and its only caller (bsr) is DrawListArea. 282e (PediaWin *w), 537 bytes, calls VList_Head, VList_Next x3, TextColor, entry_display_name x2 (= PediaName) and GRPort_String x2. Its only caller is pedia_redraw (= DrawListArea). Layout: directly after PediaName / entry_display_name on both. The sizes differ by one byte. Apply it first in its chain. (Mac edges marked bsr are BSR.L/BSR.W same-segment calls, which tools/mac_callgraph.py does not read; they were decoded from the CODE resource.)
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

extern void GRPort_String(GRPort *port, unsigned *font, const char *s,
                                     int x, int y, int align);

#include "records.h"
/* same object: 4-byte `push cs; call ` */


void PediaWin_FillListArea(PediaWin *w)
{
    PediaEntry *e;
    register int i;

    e = (PediaEntry *)VList_Head_1060_68db((VList *)(&w->list));
    if (w->page != 1)
        for (i = 0; w->page * 16 - 16 > i; i++)
            e = (PediaEntry *)VList_Next_1060_68fb((VList *)(&w->list));
    for (i = 0; i < 8 && e != 0; i++) {
        TextColor(0xff);
        GRPort_String((GRPort *)w, (unsigned *)&g_heading_font, PediaWin_PediaName(w, e),
            (rect_width_1060_63b5(&w->rect) - 0x55) / 2 / 2 + w->rect.left + 0x1e,
            (rect_height_1060_63d4(&w->rect) - 0x50) / 8 * i + w->rect.top
                + (rect_height_1060_63d4(&w->rect) - 0x50) / 8 / 2 + 0x28,
            0);
        e = (PediaEntry *)VList_Next_1060_68fb((VList *)(&w->list));
        if (e != 0) {
            GRPort_String((GRPort *)w, (unsigned *)&g_heading_font, PediaWin_PediaName(w, e),
                (rect_width_1060_63b5(&w->rect) - 0x55) / 2 + w->rect.left
                    + (rect_width_1060_63b5(&w->rect) - 0x55) / 2 / 2 + 0x1e,
                (rect_height_1060_63d4(&w->rect) - 0x50) / 8 * i + w->rect.top
                    + (rect_height_1060_63d4(&w->rect) - 0x50) / 8 / 2 + 0x28,
                0);
            e = (PediaEntry *)VList_Next_1060_68fb((VList *)(&w->list));
        }
    }
}
