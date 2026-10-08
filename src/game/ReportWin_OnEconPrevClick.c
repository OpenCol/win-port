/* PORT: a window's DialogWin base is PORT_DIALOG_BASE (sizeof(GRPort)), not the game's 0x26: 12 bytes further on 64-bit (W9). */
/* ReportWin_OnEconPrevClick: candidate for 1058:454e (360 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ReportWin_OnEconPrevClick
 *
 * The "previous" button of the economy report: the page index steps back (not below 0);
 * page 0 is the main report, later pages the paged detail. The arrow picture (resource 0x130, 142x64, game palette) is drawn into the
 * report's client rectangle through a stack GRPort first.
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

extern "C" int port_width_1058_bcfb(GRPort *port);
extern "C" int port_height_1058_bd14(GRPort *port);
extern void GRPort_CopyToPort(GRPort *src, GRPort *dst, RECT *sr, RECT *dr);

extern void ReportWin_EconReport2(ReportWin *o, int item);
/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void ReportWin_ReportsEcon(ReportWin *o, int item);

extern "C" void ReportWin_OnEconPrevClick(void)
{
    GRPort port;
    ReportWin *o;
    RECT r;
    char *w = (char *)GetEventWindow();

    o = (ReportWin *)(w != 0 ? w - PORT_DIALOG_BASE : w);
    if (o == 0)
        return;
    GRPort_LoadPicture(&port, 0x130, 0x8e, 0x40, (struct logpal *)&g_game_palette);
    SetRect(&r, 0, 0, port_width_1058_bcfb(&port), port_height_1058_bd14(&port));
    GRPort_CopyToPort(&port, o, &r, &o->rect);
    GRPort_Init_wh_1058_c064(&port);
    o->page--;
    if (o->page < 0)
        o->page = 0;
    if (o->page > 0)
        ReportWin_EconReport2(o, o->nation);
    else
        ReportWin_ReportsEcon(o, o->nation);
    copy_all_to_screen_1058_c1bf(o);
}
