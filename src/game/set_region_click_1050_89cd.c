/* PORT: a callback is stored as intptr_t, not long: a pointer is 8 bytes on 64-bit Windows (W9). */
/* set_region_click_1050_89cd: candidate for 1050:89cd (26 bytes).  Verdict: EXACT.
 * name-source: descriptive
 *
 * Stores a pointer into the slot at +0x22 of a record -- one 32-bit `MOV` through
 * ES and nothing else. One of TWO copies.
 *
 * RENAMED 2026-09-26 from set_p_22_1050_89cd: stores a region control's +0x22 click callback, the one
 * region_wndproc fires through call_region_click. The three callers pass a region: ColonyWin's at
 * +0x77a and EuropeWin's refresh the port, the popup's regions call sprite_click.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

void set_region_click_1050_89cd(SRegion *r, void *h)
{
    r->on_click = (intptr_t)h;
}
