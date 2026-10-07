/* ReportWin_OnShipNextClick: candidate for 1058:6350 (305 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ReportWin_OnShipNextClick
 *
 * The "next ship" button of the ship report: the arrow picture (resource 0x132, 142x64,
 * game palette) is loaded into a stack GRPort and copied into the report's client rectangle,
 * the page number at +0x7f2 advanced, and the report redrawn for the nation at +0x7f0.
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

extern void ReportWin_ReportsShip(ReportWin *o, int nation);

extern "C" void ReportWin_OnShipNextClick(void)
{
    GRPort port;
    ReportWin *o;
    RECT r;
    char *w = (char *)GetEventWindow();

    o = (ReportWin *)(w != 0 ? w - 0x26 : w);
    if (o == 0)
        return;
    GRPort_LoadPicture(&port, 0x132, 0x8e, 0x40, (struct logpal *)&g_game_palette);
    SetRect(&r, 0, 0, port_width_1058_bcfb(&port), port_height_1058_bd14(&port));
    GRPort_CopyToPort(&port, o, &r, &o->rect);
    GRPort_Init_wh_1058_c064(&port);
    o->page++;
    ReportWin_ReportsShip(o, o->nation);
    copy_all_to_screen_1058_c1bf(o);
}
