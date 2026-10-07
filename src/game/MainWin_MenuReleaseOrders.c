/* MainWin_MenuReleaseOrders: candidate for 1048:a959 (183 bytes, 1 caller).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Sets up the orders menu inside a window: enables items 400..416, marks 17 flag bytes,
 * and gives items 405, 406 and 411 their text. The loop counter is a `long`, as the item
 * ids are.
 *
 * THE TWO CALLEES WERE NAMED AFTER THE WRONG FUNCTIONS. This file called them
 * `menu_enable` and `menu_set_text`, but the call graph reaches 1048:e2dd and
 * 1048:e300, whose own reconstructions are `MSEnableItem_1048_e2dd` and
 * `MSModifyMenu_1048_e300`. The corpus's `menu_set_text` is 1070:d926 and takes ten bytes of
 * arguments where this pushes twelve, which is what `analysis/param-count-conflicts.json`
 * reported as its ONE "different bytes, one side is wrong" row. Neither side was
 * wrong; the name was. Both are now defined in-file, before the caller, for the 4-byte
 * calls.
 *
 * RENAMED 2026-09-26 from orders_menu_init: the Mac build's MainWin::MenuReleaseOrders(void), bound by call-sequence alignment (tools/mac_callgraph.py's seq): in MainWin_CheckMenuItems -- bound callers on both sides -- the calls on either side of this one align, and this is the one unbound callee left in the gap on each side (0.5 votes, the next candidate 0.0); the arity agrees (1).
 */

#include "colonization.h"

#include "classes.h"
/* same object: 4-byte `push cs; call ` (rule 47) */
extern void MSEnableItem_1048_e2dd(MenuClass *menu, long id, int on);
extern void MSModifyMenu_1048_e300(MenuClass *menu, long id, char *text);

void MainWin_MenuReleaseOrders(MainWin *w)
{
    long i = 400;

    for (; i < 417; i++)
        MSEnableItem_1048_e2dd(&w->menu, i, 1);
    for (i = 0; i < 17; i++)
        g_order_flags[i] = 1;
    MSModifyMenu_1048_e300(&w->menu, 405L, "Build Colony\tB");
    MSModifyMenu_1048_e300(&w->menu, 406L, "Plow Fields\tP");
    MSModifyMenu_1048_e300(&w->menu, 411L, "&Go to Port\tG");
}
