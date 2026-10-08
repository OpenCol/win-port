/* PORT: a window's DialogWin base is PORT_DIALOG_BASE (sizeof(GRPort)), not the game's 0x26: 12 bytes further on 64-bit (W9). */
/* ReportWin_OnColonyExtraClick: candidate for 1058:4ff8 (363 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ReportWin_OnColonyExtraClick
 *
 * The colony report's view toggle: the flag at +0x7f4 flips, the index resets to 0, and the
 * colony report or the Sons of Liberty page is drawn. The arrow picture (resource 0x131, 142x64, game palette) is drawn into the
 * report's client rectangle through a stack GRPort first.
 *
 * RENAMED 2026-09-27 from toggle_colony_report_view: BEYOND THE PROPOSALS. Mac ReportWin::OnColonyExtraClick(short), CODE 18 0x4bc8, 304 bytes. Its seq is an EH frame, GRPort ctor, GetEventWindow, LoadPicture, GRPort_CopyToPort, GRPort::Init, CopyToScreen, ValidRect, and bsr ReportsColony and ColonyLibertyReport. 4ff8, 363 bytes, calls __InitExceptBlock, GRPort_ctor, GetEventWindow, GRPort_LoadPicture, GRPort_CopyToPort, GRPort_Init_wh, copy_all_to_screen, ReportWin_ReportsColony and report_sons_of_liberty_page (= ColonyLibertyReport). Neither has a direct caller; both are button actions. Layout: Mac OnColonyPrevClick, OnColonyNextClick, OnColonyExtraClick, ColonyLibertyReport; Windows ReportWin_OnColonyPrevClick, ReportWin_OnColonyNextClick, 4ff8, 51e2. Arity: Windows (void) against the Mac (short) button id. That follows the already-bound ReportWin_OnColonyPrevClick(void) / OnColonyPrevClick(short). C++ file (extern "C"). (Mac edges marked bsr are BSR.L/BSR.W same-segment calls, which tools/mac_callgraph.py does not read; they were decoded from the CODE resource.)
 */

#include "colonization.h"

#include "w16types.h"
/* PORT: this file's own CTable, Screen, EventTable, EventWin, DialogWin, GRPort, GameWin, DlgControl, SImageButton, ColChild, ReportWin removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"   /* 0x41e bytes: the palette, its handle and the cycling state */
#pragma option -a

#pragma option -a-
#pragma option -a-


class DlgControl;





class SButton;

extern void *GetEventWindow(void);
struct logpal;
extern int GRPort_LoadPicture(GRPort *p, int res, int w, int h, struct logpal *pal);

/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" int port_width_1058_bcfb(GRPort *port);
extern "C" int port_height_1058_bd14(GRPort *port);
extern void GRPort_CopyToPort(GRPort *src, GRPort *dst, RECT *sr, RECT *dr);

extern void ReportWin_ReportsColony(ReportWin *o, int item);
extern void ReportWin_ColonyLibertyReport(ReportWin *o, int item);

extern "C" void ReportWin_OnColonyExtraClick(void)
{
    GRPort port;
    ReportWin *o;
    RECT r;
    char *w = (char *)GetEventWindow();

    o = (ReportWin *)(w != 0 ? w - PORT_DIALOG_BASE : w);
    if (o == 0)
        return;
    GRPort_LoadPicture(&port, 0x131, 0x8e, 0x40, (struct logpal *)&g_game_palette);
    SetRect(&r, 0, 0, port_width_1058_bcfb(&port), port_height_1058_bd14(&port));
    GRPort_CopyToPort(&port, o, &r, &o->rect);
    GRPort_Init_wh_1058_c064(&port);
    o->view = !o->view;
    o->page = 0;
    if (o->view == 0)
        ReportWin_ReportsColony(o, o->nation);
    else
        ReportWin_ColonyLibertyReport(o, o->nation);
    copy_all_to_screen_1058_c1bf(o);
}
