/* PORT: a callback is stored as intptr_t, not long: a pointer is 8 bytes on 64-bit Windows (W9). */
/* set_region_click_1010_e93f: candidate for 1010:e93f (26 bytes).  Verdict: EXACT.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: set_region_click_1010_e93f
 * name-source: descriptive
 *
 * Stores a pointer into the slot at +0x22 of a record -- the same shape as the
 * `set_p_1a`, `set_p_1c` and `set_p_1e` families, and the third function to write slot 0x22
 * after 1048:e033 and 1050:89cd, which is why the address stays in the name.
 *
 * One 32-bit `MOV` through ES, the cost of a pointer written through a record pointer,
 * with no "return the previous value" -- that is the longer `set_handler_slot_*` shape.
 *
 * ITS ONE CALLER SAYS WHAT THE SLOT IS FOR: `ColonyWin_Init` installs
 * `ColonyWin_OnOKClick` into the region at `(char *)w + 0x77a`, so +0x22 of that
 * region is a callback the window machinery invokes.
 *
 * RENAMED 2026-09-26 from set_p_22_1010_e93f: stores a region control's +0x22 click callback, the one
 * region_wndproc fires through call_region_click. The three callers pass a region: ColonyWin's at
 * +0x77a and EuropeWin's refresh the port, the popup's regions call sprite_click.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

void set_region_click_1010_e93f(SRegion *r, void *h)
{
    r->on_click = (intptr_t)h;
}
