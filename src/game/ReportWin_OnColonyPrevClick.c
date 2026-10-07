/* ReportWin_OnColonyPrevClick: candidate for 1058:4d02 (361 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ReportWin_OnColonyPrevClick
 *
 * The "previous" button of the colony report: the index steps back (not below 0) and the
 * report is redrawn -- the colony report or the Sons of Liberty page by the view flag. The arrow picture (resource 0x131, 142x64, game palette) is drawn into the
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

/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" int port_width_1058_bcfb(GRPort *port);
extern "C" int port_height_1058_bd14(GRPort *port);
extern void GRPort_CopyToPort(GRPort *src, GRPort *dst, RECT *sr, RECT *dr);

extern void ReportWin_ReportsColony(ReportWin *o, int item);
extern void ReportWin_ColonyLibertyReport(ReportWin *o, int item);

extern "C" void ReportWin_OnColonyPrevClick(void)
{
    GRPort port;
    ReportWin *o;
    RECT r;
    char *w = (char *)GetEventWindow();

    o = (ReportWin *)(w != 0 ? w - 0x26 : w);
    if (o == 0)
        return;
    GRPort_LoadPicture(&port, 0x131, 0x8e, 0x40, (struct logpal *)&g_game_palette);
    SetRect(&r, 0, 0, port_width_1058_bcfb(&port), port_height_1058_bd14(&port));
    GRPort_CopyToPort(&port, o, &r, &o->rect);
    GRPort_Init_wh_1058_c064(&port);
    o->page--;
    if (o->page < 0)
        o->page = 0;
    if (o->view == 0)
        ReportWin_ReportsColony(o, o->nation);
    else
        ReportWin_ColonyLibertyReport(o, o->nation);
    copy_all_to_screen_1058_c1bf(o);
}
