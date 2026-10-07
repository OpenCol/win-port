/* set_repaint_handler_1058_ba08: candidate for 1058:ba08 (27 bytes).  Verdict: EXACT.
 * name-source: descriptive
 *
 * Stores a function pointer at +0x762 of the window -- the third copy of this function in
 * the image, byte for byte identical to `set_repaint_handler_1038_e10b` and
 * `set_repaint_handler_1060_62d8` in their own segments.
 *
 * Its callers pass `UpdateStats` and `ReportWin_DrawReport`, so the slot
 * holds a repaint callback. The other two copies are handed repaint callbacks too (MapWin_Init gives the 1038 copy
 * MapWin_RedrawMap, MiniWin_Init gives the 1060 copy MiniWin_MiniUpdate), so the name says "repaint".
 *
 * RENAMED 2026-09-26 from set_handler_762_1058_ba08: stores a window's +0x762 repaint handler, the one call_repaint_handler
 * runs: MapWin_Init installs MapWin_RedrawMap, ReportWin_Init ReportWin_DrawReport, StatWin_Init
 * UpdateStats and MiniWin_Init MiniWin_MiniUpdate. ReportWin_Init's own record calls the field repaint.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

void set_repaint_handler_1058_ba08(ColChild *w, void *h)
{
    w->repaint = h;
}
