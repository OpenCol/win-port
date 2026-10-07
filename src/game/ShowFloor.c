/* ShowFloor: candidate for 1058:6ff7 (28 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * The congress report's "view floor" button: opens the Continental Congress view for the
 * current selection (DGROUP:0x4e06) with -1.
 *
 * RENAMED 2026-09-27 from on_view_congress_floor_click: BEYOND THE PROPOSALS. Mac ShowFloor(short), a free function, CODE 18 0x691a, 40 bytes. Its only call is show_fathers(short, short). 6ff7 (void), 28 bytes, calls only show_fathers. Neither has a direct caller: it is the button action that ReportsContinental installs, and report_congress_activities installs exactly one button (set_button_action x1). Layout: directly before ReportsContinental on both, after ReportsShip. Arity: a button handler, (void) against (short), as for the ReportWin_On* handlers.
 */

#include "colonization.h"

extern void show_fathers(int sel, int mode);

void ShowFloor(void)
{
    show_fathers(g_congress_nation, -1);
}
