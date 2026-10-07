/* ReportWin_ReallyDrawReport: candidate for 1058:0738 (986 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ReportWin_ReallyDrawReport
 * name-source: sibling-build
 *
 * Paints a report page: the page's picture (resource 299 + page for pages 1..8, the 16x24
 * picture 202 for the score) is copied into the window unless the page is the trade-route
 * editor (0x63), the caption is set from the page's title, the player's nation stored at
 * +0x7f0, both click handlers dismiss the report, and the page's own drawing routine runs
 * (the score page keeps its result at +0x7f6). The module's wrappers and set_window_caption
 * are defined here because the shipped calls to them are the compiler's own
 * `push cs / call `; the page routines are calls TLINK converted.
 *
 * RENAMED 2026-09-26 from draw_report_window: the Mac build's ReportWin::ReallyDrawReport(void): this only -- the page actually painted (picture, caption, the page's own drawer), what ReportWin_DrawReport forwards to; 'Really' marks the worker behind the entry. The width matches.
 */

#include "colonization.h"

#include "w16types.h"
/* PORT: this file's own CTable, DlgControl, SButton, Screen, EventTable, EventWin, DialogWin, GRPort, GameWin, SImageButton, ColChild, ReportWin removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"   /* 0x41e bytes: the palette, its handle and the cycling state */


#pragma option -a

#pragma option -a-







class SButton;

#include "records.h"
extern GameState g_game;   /* SEG20:0x7782 */
struct logpal;
extern int GRPort_LoadPicture(GRPort *p, int res, int w, int h, struct logpal *pal);

extern void GRPort_CopyToPort(GRPort *src, GRPort *dst, RECT *sr, RECT *dr);

extern void MSSetWinTitle(MSWin *win, char *s);
extern void ColChild_DrawTitleBar(ColChild *w);
extern void ReportWin_LeftMouseClick(void);
extern void ReportWin_ReportsIndian(ReportWin *o, int nation);
extern void ReportWin_ReportsReligious(ReportWin *o, int nation);
extern void ReportWin_ReportsContinental(ReportWin *o, int nation);
extern void ReportWin_ReportsLabor(ReportWin *o, int nation);
extern void ReportWin_ReportsEcon(ReportWin *o, int nation);
extern void ReportWin_ReportsColony(ReportWin *o, int nation);
extern void ReportWin_ReportsShip(ReportWin *o, int nation);
extern void ReportWin_ReportsForeign(ReportWin *o, int nation);
extern int ReportWin_Score(ReportWin *o, int draw);
extern void ReportWin_TradeRoutes(ReportWin *o);

extern "C" int port_width_1058_bcfb(GRPort *port);

extern "C" int port_height_1058_bd14(GRPort *port);

extern "C" void get_window_rect_1058_bcd5(ColChild *o, RECT *r);

extern "C" void *set_on_lclick_1058_bdb7(EventTable *table, void *h);

extern "C" void *set_on_rclick_1058_bdf0(EventTable *table, void *h);

extern "C" void MSSetWinTitle_of_1058_c555(DialogWin *w, char *s);

extern "C" void set_window_caption(ColChild *w, char *s);

void ReportWin_ReallyDrawReport(ReportWin *o)
{
    GRPort port;
    RECT rc;
    char title[0x24];

    get_window_rect_1058_bcd5(o, &rc);
    title[0] = 0;
    if (o->kind < 9)
        GRPort_LoadPicture(&port, o->kind + 299, 142, 64, (struct logpal *)&g_game_palette);
    else if (o->kind == 9)
        GRPort_LoadPicture(&port, 202, 16, 24, (struct logpal *)&g_game_palette);
    if (o->kind != 0x63) {
        SetRect(&rc, 0, 0, port_width_1058_bcfb(&port), port_height_1058_bd14(&port));
        GRPort_CopyToPort(&port, (GRPort *)o, &rc, &o->rect);
    } else
        SetRect(&rc, 0, 0, rect_width_1058_bd2d(&o->rect), rect_height_1058_bd4c(&o->rect));
    switch (o->kind) {
    case 1: STRCAT(title, "Indian Advisor Report"); break;
    case 2: STRCAT(title, "Religious Advisor Report"); break;
    case 3: STRCAT(title, "Continental Congress Activities"); break;
    case 4: STRCAT(title, "Labor Advisor Report"); break;
    case 5: STRCAT(title, "Economic Advisor Report"); break;
    case 6: STRCAT(title, "Colony Advisor Report"); break;
    case 7: STRCAT(title, "Naval Advisor Report"); break;
    case 8: STRCAT(title, "Foreign Affairs Report"); break;
    case 9: STRCAT(title, "Colonization Score"); break;
    case 0x63: STRCAT(title, "Trade Routes"); break;
    }
    set_window_caption(o, title);
    o->nation = g_game.view_nation;
    set_on_lclick_1058_bdb7(&(EventTable &)*o, (void *)ReportWin_LeftMouseClick);
    set_on_rclick_1058_bdf0(&(EventTable &)*o, (void *)ReportWin_LeftMouseClick);
    switch (o->kind) {
    case 1: ReportWin_ReportsIndian(o, o->nation); break;
    case 2: ReportWin_ReportsReligious(o, o->nation); break;
    case 3: ReportWin_ReportsContinental(o, o->nation); break;
    case 4: ReportWin_ReportsLabor(o, o->nation); break;
    case 5: ReportWin_ReportsEcon(o, o->nation); break;
    case 6: ReportWin_ReportsColony(o, o->nation); break;
    case 7: ReportWin_ReportsShip(o, o->nation); break;
    case 8: ReportWin_ReportsForeign(o, o->nation); break;
    case 9: o->score = ReportWin_Score(o, 1); break;
    case 0x63: ReportWin_TradeRoutes(o); break;
    }
}

