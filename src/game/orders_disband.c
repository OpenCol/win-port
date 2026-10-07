/* PORT: this file's own declarations of delete_colony removed: the header declares it from the definition (W1 step 3). */
/* orders_disband: candidate for 1008:0e8d (672 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * The Disband order on the cursor square. Allowed on the player's own square (map plane 2
 * high nibble) or in god mode (SEG20:0x7785 bit 5). A unit there (the selected unit when
 * not in view mode): ships (types 13..18) first unload their cargo, and a ship carrying
 * more than one unit is refused with DISBANDSHIP; otherwise SUREDISBAND names the unit
 * type and answer 1 frees it, re-selects (or shows the pieces when none are left, or
 * recentres on the new selection). No unit: in god mode a colony there is deleted after
 * SUREDISBAND with its name, else a native settlement is deleted outright.
 *
 * RENAMED 2026-09-26 from disband_unit: the Mac build's orders_disband(void), bound by call-graph agreement (tools/mac_callgraph.py): 5 of its Mac call edges already bound land on this function (caller KeyboardEvent; callee delete_colony; callee popm; callee popup_say; callee top_unit_in_square), the runner-up gets 3, and the arity agrees (0).
 */

#include "colonization.h"

#include "w16types.h"
#include "gameflag.h"
#include "units.h"
#include "records.h"

extern GameState g_game;   /* SEG20:0x7782 */
extern GameBrains g_game_brains;           /* SEG20:0x40d6 */
extern char owner_of(int x, int y);
extern int top_unit_in_square(int x, int y);
extern void stack_ship(int u);
extern int stack_check(int u, int kind);
extern void relocate_stacked_units(int u, int x, int y);
extern int popm(char *msg, int n);
extern void popup_say(int slot, int word);

extern void delete_unit(int u);
extern void DrawMap(int flag);
extern void unit_activate(int u);
extern void GameBrains_SetViewMode(void *rec);
extern void set_center(int x, int y, int flag);

/* same object: 4-byte `push cs; call ` (rule 47) */

extern int find_village(int x, int y);
extern void delete_village(int s);

void orders_disband(void)
{
    int owner;
    int u;
    int n;

    owner = owner_of(g_cursor_x, g_cursor_y);
    if ((HI_BYTE(g_game.game_flags) & GAMEFLAG_HI_CHEAT_MENU) == 0 && g_game.view_nation != owner)
        goto done;
    u = top_unit_in_square(g_cursor_x, g_cursor_y);
    if (g_game.view_mode == 0)
        u = g_game.selected_unit;
    if (u >= 0) {
        if (g_units[u].type >= UNIT_SHIP_FIRST && g_units[u].type <= UNIT_SHIP_LAST) {
            stack_ship(u);
            n = stack_check(u, 2);
            relocate_stacked_units(u, g_cursor_x, g_cursor_y);
            if (n > 1) {
                popm("DISBANDSHIP", 0);
                goto done;
            }
        }
        popup_say(0, g_unit_types[g_units[u].type].name);
        if (show_text_dialog_1008_cdf3("SUREDISBAND") != 1)
            goto done;
        delete_unit(u);
        DrawMap(1);
        if (g_game.selected_unit >= u) {
            if (g_game.selected_unit > u)
                g_game.selected_unit--;
            if (g_game.selected_unit >= g_game.unit_count)
                unit_activate(0);
            if (g_game.unit_count < 1)
                GameBrains_SetViewMode(&g_game_brains);
            else if (g_game.view_mode == 0)
                set_center(g_units[g_game.selected_unit].x, g_units[g_game.selected_unit].y, 1);
        }
    } else {
        if ((HI_BYTE(g_game.game_flags) & GAMEFLAG_HI_CHEAT_MENU) == 0)
            goto done;
        n = find_colony(g_cursor_x, g_cursor_y);
        if (n >= 0) {
            set_name_slot_1008_cdca(0, g_colonies[n].name);
            if (show_text_dialog_1008_cdf3("SUREDISBAND") != 1)
                goto done;
            delete_colony(n);
            DrawMap(1);
        } else {
            n = find_village(g_cursor_x, g_cursor_y);
            if (n >= 0) {
                delete_village(n);
                DrawMap(1);
            }
        }
    }
done:
    ;
}
