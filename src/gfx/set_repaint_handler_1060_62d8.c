/* set_repaint_handler_1060_62d8: candidate for 1060:62d8 (27 bytes).  Verdict: EXACT.
 * name-source: descriptive
 *
 * Stores a function pointer at +0x762 of the window -- byte for byte the same function as
 * `set_repaint_handler_1038_e10b` in another segment, which is how the corpus already carries
 * several such pairs. Its caller passes `MiniWin_MiniUpdate`.
 *
 * RENAMED 2026-09-26 from set_handler_762_1060_62d8: stores a window's +0x762 repaint handler, the one call_repaint_handler
 * runs: MapWin_Init installs MapWin_RedrawMap, ReportWin_Init ReportWin_DrawReport, StatWin_Init
 * UpdateStats and MiniWin_Init MiniWin_MiniUpdate. ReportWin_Init's own record calls the field repaint.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

void set_repaint_handler_1060_62d8(ColChild *w, void *h)
{
    w->repaint = h;
}
