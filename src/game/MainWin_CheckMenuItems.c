/* MainWin_CheckMenuItems: candidate for 1048:aa10 (995 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Brings the Orders menu of the window up to date for the selected unit (SEG20:0x7794), outside
 * view mode. `MainWin_MenuReleaseOrders` first enables items 400..416 and sets their 17 flag bytes; then the
 * items that do not apply are disabled again, clearing the matching flag at SEG30:0x50 + (id - 400):
 *
 *   405  off for a unit without the colonist class flag, or for specialty 0x1b; on a colony square
 *        it is relabelled "Join Colony\tB" instead
 *   406, 407  off for a unit without the second class flag; 406 relabelled "Clear Forest\tP" on
 *        terrain 8..0x17, and switched off again on terrain 0x1b or 0x1c
 *   413  off for a ship (types 0xd..0x12) unless it stands on terrain 0x1a, and for every non-ship,
 *        which also has 411 relabelled "Go To Place\tG"
 *   408, 409  off for a unit type whose byte +7 is zero, or when no European owns the square;
 *   415  off with them, or when the unit's byte +0xc is zero; 412 off for the zero-byte types too
 *
 * Flag 410 is always cleared. The forest test is written as two ranges, 8..0xf and 0x10..0x17,
 * with an empty `else` after it that leaves its `EB 00` (rule 62a).
 *
 * The name is the curated anchor's in re/menus.json.
 *
 * RENAMED 2026-09-26 from orders_menu_context_variant: the Mac build's MainWin::CheckMenuItems(void), bound by call-graph agreement (tools/mac_callgraph.py): 3 of its Mac call edges already bound land on this function (caller MoveUnit; callee job_of_unit; callee terrain_at), the runner-up gets 1, and the arity agrees (1).
 */

#include "colonization.h"

#include "terrain.h"
#include "jobs.h"
#include "units.h"
#include "classes.h"
#include "records.h"

extern GameState g_game;   /* SEG20:0x7782 */

extern int job_of_unit(int u);
extern void check_for_colonist(int u, int *is_colonist, int *is_type2);

extern int is_colony(int x, int y);

/* same object: 4-byte `push cs; call ` (rule 47) */
extern void MainWin_MenuReleaseOrders(MainWin *w);
extern void MSEnableItem_1048_e2dd(MenuClass *menu, unsigned long id, char enable);
extern void MSModifyMenu_1048_e300(MenuClass *menu, long id, char *text);

void MainWin_CheckMenuItems(MainWin *w)
{
    int is_col;
    int is_t2;
    int y;
    unsigned char terr;
    register int u;
    register int x;

    u = g_game.selected_unit;
    if (g_game.view_mode == 0) {
        MainWin_MenuReleaseOrders(w);
        x = g_units[u].x;
        y = g_units[u].y;
        terr = terrain_at(x, y);
        is_col = 0;
        is_t2 = 0;
        if (job_of_unit(u) >= 0)
            check_for_colonist(u, &is_col, &is_t2);
        g_order_flags[10] = 0;
        if (is_col == 0) {
            MSEnableItem_1048_e2dd(&w->menu, 405L, 0);
            g_order_flags[5] = 0;
        } else if (find_colony(x, y) >= 0) {
            MSModifyMenu_1048_e300(&w->menu, 405L, "Join Colony\tB");
        } else if ((int)g_units[u].spec == JOB_CONVERT) {
            MSEnableItem_1048_e2dd(&w->menu, 405L, 0);
            g_order_flags[5] = 0;
        }
        if (is_t2 == 0) {
            MSEnableItem_1048_e2dd(&w->menu, 406L, 0);
            g_order_flags[6] = 0;
            MSEnableItem_1048_e2dd(&w->menu, 407L, 0);
            g_order_flags[7] = 0;
        }
        if ((terr >= TERRAIN_FOREST_FIRST && terr < TERRAIN_FOREST_2_FIRST) || (terr >= TERRAIN_FOREST_2_FIRST && terr < TERRAIN_ARCTIC))
            MSModifyMenu_1048_e300(&w->menu, 406L, "Clear Forest\tP");
        else {
        }
        if (terr == TERRAIN_MOUNTAINS || terr == TERRAIN_HILLS) {
            MSEnableItem_1048_e2dd(&w->menu, 406L, 0);
            g_order_flags[6] = 0;
        }
        if (g_units[g_game.selected_unit].type >= UNIT_SHIP_FIRST && g_units[g_game.selected_unit].type <= UNIT_SHIP_LAST) {
            if (terr != TERRAIN_SEA_LANE) {
                MSEnableItem_1048_e2dd(&w->menu, 413L, 0);
                g_order_flags[13] = 0;
            }
        } else {
            MSEnableItem_1048_e2dd(&w->menu, 413L, 0);
            g_order_flags[13] = 0;
            MSModifyMenu_1048_e300(&w->menu, 411L, "Go To Place\tG");
        }
        if (g_unit_types[g_units[g_game.selected_unit].type].holds > 0) {
            if (is_colony(x, y) < 0) {
                MSEnableItem_1048_e2dd(&w->menu, 408L, 0);
                g_order_flags[8] = 0;
                MSEnableItem_1048_e2dd(&w->menu, 409L, 0);
                g_order_flags[9] = 0;
            }
            if (g_units[g_game.selected_unit].cargo_count == 0) {
                MSEnableItem_1048_e2dd(&w->menu, 415L, 0);
                g_order_flags[15] = 0;
            }
        } else {
            MSEnableItem_1048_e2dd(&w->menu, 408L, 0);
            g_order_flags[8] = 0;
            MSEnableItem_1048_e2dd(&w->menu, 409L, 0);
            g_order_flags[9] = 0;
            MSEnableItem_1048_e2dd(&w->menu, 412L, 0);
            g_order_flags[12] = 0;
            MSEnableItem_1048_e2dd(&w->menu, 415L, 0);
            g_order_flags[15] = 0;
        }
    }
}
