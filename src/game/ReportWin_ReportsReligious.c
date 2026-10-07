/* ReportWin_ReportsReligious: candidate for 1058:1bd8 (341 bytes).  Verdict: RELOCATABLE.
 * symbol: ReportWin_ReportsReligious
 *
 * The religious-advisor report: the title (word SEG20:0xdd04) shadowed in colour 10 at
 * (320, 15), then the crosses bar (DrawIconRow, kind 0x39) from the market record's
 * +0x30 / +0x2e over the window width less 30; in god mode (SEG20:0x7785 bit 5) the raw
 * "(%d of %d)" is added in colour 15. The rectangle-width helper is defined here because
 * the shipped call to it is the compiler's own `push cs / call `.
 */

#include "colonization.h"

#include "gameflag.h"
#include "w16types.h"
#include "misc.h"
#include "classes.h"
#include "records.h"

extern GameState g_game;   /* SEG20:0x7782 */

extern void set_europe(int n);

extern char *strings(int n);
extern void GRPort_String(GRPort *port, unsigned *font, char *s, int x, int y, int size);
extern void DrawIconRow(GRPort *w, int kind, int a, int b, int x, int y, int width, int c, int d, int e);

void ReportWin_ReportsReligious(ReportWin *o, int nation)
{
    char buf[0x28];
    int x, y;

    set_europe(nation);
    TextColor(10);
    GRPort_String((GRPort *)o, (unsigned *)&g_report_font, strings(g_misc_words[MISC_RELIGIOUS_ADVISOR_REPORT]), o->rect.left + 320, o->rect.top + 15, 16);
    x = 20;
    y = 50;
    g_draw_labels = 1;
    DrawIconRow((GRPort *)o, 0x39, g_cur_nation_rec->need, g_cur_nation_rec->crosses, o->rect.left + x, o->rect.top + y, rect_width_1058_bd2d(&o->rect) - x - 10, 0, 0, 1);
    if (HI_BYTE(g_game.game_flags) & GAMEFLAG_HI_CHEAT_MENU) {
        sprintf_rtl(buf, "(%d of %d)", g_cur_nation_rec->crosses, g_cur_nation_rec->need);
        TextColor(15);
        GRPort_String((GRPort *)o, (unsigned *)&g_report_font_small, buf, o->rect.left + x, o->rect.top + y, 5);
    }
}
