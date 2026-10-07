/* PORT: this file's own declarations of copy_rect_to_screen removed: the header declares it from the definition (W1 step 3). */
/* EuropeWin_DrawOutBound: candidate for 1048:978c (383 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: EuropeWin_DrawOutBound
 * name-source: sibling-build
 *
 * The left-hand twin of EuropeWin_DrawInbound: the 0x8b by 0x79 cell at (2, 0x102)
 * repainted, a heading from the text string the word at SEG20:0xdcda indexes, and every unit in
 * the nation's two pseudo-squares at index minus 0x10 and minus 0xc laid along one row, the
 * slot counter carried from the first chain into the second by `EuropeWin_DrawTransitUnit`. There is no
 * second line with the nation's name.
 *
 * RENAMED 2026-09-26 from europe_draw_left: the Mac build's EuropeWin::DrawInbound(short): this plus a refresh flag; its panel is headed MISC_EXPECTED_SOON -- the ships on their way IN to Europe. The width matches.
 *
 * RENAMED 2026-09-27 from EuropeWin_DrawInbound: CONST PROPOSAL, SETTLED AS A SWAP with 95c1; the evidence is in 95c1's row. 978c ('Expected soon', squares nation-0x10 and -0xc, x 3/0x47) matches Mac DrawOutBound: its callees (strings, String x1), its position (second), its call order in DrawTransit (second) and its unique constants (0x47, -0x10, -0xc).
 */

#include "colonization.h"

#include "w16types.h"
#include "misc.h"
/* PORT: this file's own DlgControl, SButton, Screen, EventTable, EventWin, DialogWin, GRPort, GameWin, SImageButton, ColChild, SRegion, EuropeWin, Font removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"

#pragma option -a

#pragma option -a-











extern char *strings(int n);

extern void GRPort_String(GRPort *port, unsigned *font, char *s,
                                     int x, int y, int align);
extern int top_unit_in_square(int x, int y);
extern int next_unit(int u);

/* 5-byte NOP form, so extern (rule 47) */
extern void EuropeWin_UpdateEurope(EuropeWin *w, int x, int y, int cx, int cy);
extern void EuropeWin_DrawTransitUnit(EuropeWin *w, int u, int x, int count3, int extra,
                              int *slot, int colour);

/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void set_europe(int n);

void EuropeWin_DrawOutBound(EuropeWin *w, int refresh)
{
    int slot;
    RECT r;
    char buf[0x50];
    register int u;
    register int x;

    set_europe(w->nation);
    EuropeWin_UpdateEurope(w, 2, 0x102, 0x8b, 0x79);
    STRCPY(buf, strings(g_misc_words[MISC_EXPECTED_SOON]));
    TextColor(0x77);
    GRPort_String((GRPort *)w, (unsigned *)&g_report_font_small, buf, w->rect.left + 0x47, w->rect.top + 0x10c, 0);
    x = 3;
    slot = 0;
    u = top_unit_in_square(g_cur_nation - 0x10, g_cur_nation - 0x10);
    while (u >= 0) {
        EuropeWin_DrawTransitUnit(w, u, x, 0xd, 1, &slot, -1);
        u = next_unit(u);
    }
    u = top_unit_in_square(g_cur_nation - 0xc, g_cur_nation - 0xc);
    while (u >= 0) {
        EuropeWin_DrawTransitUnit(w, u, x, 0xd, 1, &slot, -1);
        u = next_unit(u);
    }
    if (refresh != 0) {
        SetRect(&r, w->rect.left + 2, w->rect.top + 0x102, w->rect.left + 0x8d, w->rect.top + 0x17b);
        copy_rect_to_screen(w, &r);
    }
}
