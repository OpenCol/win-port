/* PORT: this file's own declarations of SetPopupParent, route_box_parse removed: the header declares them from the definition (W1 step 3). */
/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 81 of the imported file. */
/* ReportWin_OnTradeRouteClick: candidate for 1058:8868 (386 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ReportWin_OnTradeRouteClick
 * name-source: sibling-build
 * symbol: ReportWin_OnTradeRouteClick
 *
 * A click in the trade-route report (re-entry flag at DGROUP:0x2668): inside the client
 * rectangle the window is selected; a click in the route list (y 105..404) goes to
 * route_box_parse and the window is rebuilt; a click on the name line (below the header, above
 * twice the font height + 29) opens the TRADENAME text dialog on the route name (pointer
 * at DGROUP:0x4c18); a non-cancelled dialog copies the entered text (data segment 31 offset
 * 0xa4) back and rebuilds. Afterwards MSDoAllTasks_1058_bc15 and SetPopupParent on the game record.
 *
 * RENAMED 2026-09-26 from prompt_trade_route_name: The trade-route report's whole left-click handler: ReportWin_TradeRoutes installs it with set_on_lclick at +0x33. It ignores clicks outside the window rect, selects the window, sends a click in the route list (y 0x69..0x194) to route_box_parse and redraws, and only for a click on the name line opens the TRADENAME popup_ask and copies the result back. The old name makes one of its two branches sound like the function's whole job and hides that it dispatches route-list clicks. Its siblings are named the same way (ReportWin_OnLaborClick, ReportWin_LeftMouseClick).
 *
 * RENAMED 2026-09-27 from trade_route_report_click: CONST PROPOSAL. Mac ReportWin::OnTradeRouteClick(short, short), CODE 18 0x7f82, 320 bytes. Decoded, it reads 8(a6) and $a(a6) as its two shorts with no this (a static member), which matches 8868's (int x, int y), like the bound ReportWin_OnLaborClick(int x, int y). Its seq is GetEventWindow, SetPopupParent, route_box_parse, Redraw, popup_ask, Redraw, MSDoAllTasks, SetPopupParent. 8868 calls GetEventWindow, SetPopupParent x2, route_box_parse, GameWin_Redraw x2, popup_ask and MSDoAllTasks. Layout: Mac TradeRoutes, route_order, route_box_parse, OnTradeRouteClick, OnTradeRouteExtraClick; Windows ReportWin_TradeRoutes, route_order, route_box_parse, 8868, 89ea. Size 320 against 386. Constants: overlap 1.0.
 */

#include "colonization.h"

/* PORT: this file's own Font, DlgControl, SButton, Screen, EventTable, EventWin, DialogWin, GRPort, GameWin, SImageButton, ColChild, ReportWin removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"
#include "w16types.h"


#pragma option -a

#pragma option -a-







class SButton;
#include "records.h"
extern void *GetEventWindow(void);

typedef struct { char pad[1]; } MainWin;
extern MainWin g_main_win;         /* SEG20:0x5ca */

/* same object: 4-byte `push cs; call ` (rule 47) */
extern void GameWin_Redraw(GameWin *o);
/* Two same-module helpers are defined here because the shipped calls to them are the
 * compiler's own `push cs / call ` (4 bytes): only a callee defined in the translation
 * unit gets that form, and the `jl` over the name-line block only stays short with it.
 * font_height_1058_be4f (1058:be4f, 14 bytes) reads the word at +2 of an object; MSDoAllTasks_1058_bc15
 * (1058:bc15, 14 bytes) is MSDoAllTasks behind a call. */

extern "C" int font_height_1058_be4f(Font *font);

extern int popup_ask(void *ctx, void *name, int max, void *dflt);

void ReportWin_OnTradeRouteClick(int x, int y)
{
    ReportWin *o;
    EventWin *w;
    char buf[0x50];

    if (g_trade_click_busy != 0)
        return;
    g_trade_click_busy = 1;
    w = (EventWin *)GetEventWindow();
    o = (ReportWin *)w;
    if (o->rect.left > x || o->rect.right < x || o->rect.top > y || o->rect.bottom < y)
        goto done;
    x = x - o->rect.left;
    y = y - o->rect.top;
    SetPopupParent((DialogWin *)o);
    if (y >= 0x69 && y < 0x195) {
        route_box_parse(x, y);
        GameWin_Redraw((GameWin *)o);
        goto done;
    }
    if (y >= 0x19) {
        if (font_height_1058_be4f(&g_font_small) * 2 + 0x1d > y) {
            STRCPY(buf, g_cur_route_rec->name);
            if (popup_ask(g_game_txt, (void*)("TRADENAME"), 0x1f, buf) != 0)
                goto done;
            STRCPY(g_cur_route_rec->name, g_name_slots[0]);
            GameWin_Redraw((GameWin *)o);
            goto done;
        }
    }
done:
    g_trade_click_busy = 0;
    MSDoAllTasks_1058_bc15();
    SetPopupParent((EventWin *)&g_main_win);
}
