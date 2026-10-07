/* set_repaint_handler_1038_e10b: candidate for 1038:e10b (27 bytes).  Verdict: EXACT.
 * name-source: descriptive
 *
 * Stores a function pointer in the window's slot at +0x762 -- the 32-bit move is
 * `db 0x66,0x26,0x89,0x87,0x62,0x07`, an operand-size prefix on `mov [es:bx+0x762],eax`.
 *
 * Its caller hands it `MapWin_RedrawMap`, so the slot holds a callback. The corpus
 * names this shape after the offset: `set_on_lbutton_down_1058_c2ce` and
 * `set_on_keydown_1058_c307` are the same thing at other offsets.
 *
 * RENAMED 2026-09-26 from set_handler_762_1038_e10b: stores a window's +0x762 repaint handler, the one call_repaint_handler
 * runs: MapWin_Init installs MapWin_RedrawMap, ReportWin_Init ReportWin_DrawReport, StatWin_Init
 * UpdateStats and MiniWin_Init MiniWin_MiniUpdate. ReportWin_Init's own record calls the field repaint.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

void set_repaint_handler_1038_e10b(ColChild *w, void *h)
{
    w->repaint = h;
}
