/* set_relayout_handler_1048_d738: candidate for 1048:d738 (27 bytes).  Verdict: EXACT.
 * name-source: descriptive
 *
 * Stores a pointer into the slot at +0xc0 of a record -- one 32-bit `MOV` through
 * ES and nothing else. One of TWO copies.
 *
 * RENAMED 2026-09-26 from set_p_c0_1048_d738: stores a window's +0xc0 relayout handler, the one call_relayout_handler
 * runs from GameWin_Resize and GameWin_Redraw: ColChild_InitCWin_style_ncparent installs ColChild_DrawAll, which re-lays
 * the client area, and Popup_make_window port_update, which runs Popup_draw.
 */

#include "colonization.h"

#include "classes.h"

void set_relayout_handler_1048_d738(GameWin *r, void *h)
{
    r->relayout = h;
}
