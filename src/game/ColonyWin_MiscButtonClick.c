/* ColonyWin_MiscButtonClick: candidate for 1010:4690 (337 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ColonyWin_MiscButtonClick
 * name-source: sibling-build
 *
 * Menu commands 100..102 switch the colony window's panel mode
 * (DGROUP:0xc0f = 0, 1 or 2): the pending timer (+0xa10) is reset, the two
 * controls at +0x950/+0x972 are hidden (`MSHideControl_1010_e54d`) or shown
 * (`MSShowControl_1010_e574`) -- this header had the two the wrong way round until
 * 2026-09-25 -- and the two bytes at +0xa08/+0xa09 follow one control each:
 * `buy_shown` tracks +0x950 and `change_shown` tracks +0x972, 0 with a hide and 1
 * with a show, in all three arms. Panel 6 is then repainted.  The three module helpers (hide, show, MSFlushInput) are -called and defined in-file
 * as stubs.
 *
 * RENAMED 2026-09-26 from colony_mode_command: the Mac build's ColonyWin::MiscButtonClick(short): ColonyWin_Init installs it as the action of the three image buttons, and it switches the mode of the bottom-right panel and repaints it with ColonyWin_DrawMiscellaneous.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

extern int g_stub_a;
extern void *GetEventWindow(void);

extern void ColonyWin_DrawMiscellaneous(void *w, int redraw);

extern "C" void MSHideControl_1010_e54d(void *control);
extern "C" void MSShowControl_1010_e574(void *control);
extern "C" void MSFlushInput_1010_e5aa(void);

void ColonyWin_MiscButtonClick(int cmd)
{
    ColonyWin *w;
    EventWin *cw;

    cw = (EventWin *)GetEventWindow();
    w = (ColonyWin *)cw;
    if (w->timer != -1) {
        ResetTimer(w->timer);
        w->timer = -1;
    }
    switch (cmd) {
    case 0x64:
        g_colony_mode = 0;
        MSHideControl_1010_e54d(&w->b[0]);
        MSHideControl_1010_e54d(&w->b[1]);
        w->buy_shown = 0;
        w->change_shown = 0;
        break;
    case 0x65:
        g_colony_mode = 1;
        MSHideControl_1010_e54d(&w->b[0]);
        MSHideControl_1010_e54d(&w->b[1]);
        w->buy_shown = 0;
        w->change_shown = 0;
        break;
    case 0x66:
        g_colony_mode = 2;
        MSShowControl_1010_e574(&w->b[0]);
        MSShowControl_1010_e574(&w->b[1]);
        w->buy_shown = 1;
        w->change_shown = 1;
        break;
    }
    MSFlushInput_1010_e5aa();
    ColonyWin_DrawMiscellaneous(w, 1);
}
