/* PORT: this file's own declarations of MSEnableItem_1008_cec2 removed: the header declares it from the definition (W1 step 3). */
/* FreezeMenus: candidate for 1008:2cdc (49 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Greys menu commands 400 through 416 inclusive on the window whose handle pointer sits at
 * SEG20:0x656, one call each through `MSEnableItem_1008_cec2`, which dereferences that pointer and
 * forwards to `menu_enable_item` with the enable flag 0.
 *
 * Seventeen consecutive command ids and nothing in `re/menus.json` claims any of them, so
 * the name says the range rather than guessing what it is.
 *
 * The command id is widened to a `long` before the push -- `MOVSX EAX,SI` -- because that
 * is the parameter's type all the way down to the Windows call.
 *
 * RENAMED 2026-09-26 from disable_menu_items_400_416: the Mac build's FreezeMenus(void), bound by LAYOUT: both builds keep their source files' function order, and on both sides this is the one function between GameBrains_DoViceroy and GameBrains_SetViewMode, which are already bound; the arity agrees and the Mac body is 1.35x this one.
 */

#include "colonization.h"

#include "classes.h"

/* same object: 4-byte `push cs; call ` */

void FreezeMenus(void)
{
    register int i;

    for (i = 0x190; i <= 0x1a0; i++)
        MSEnableItem_1008_cec2(&g_menu_owner, (long)i, 0);
}
