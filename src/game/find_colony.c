/* find_colony: candidate for 1018:471a (149 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * The colony at (x, y): -1 off the map or on a square without a European owner; the
 * colony table is searched by position, and a miss with an owner present reports
 * COLONYFLAG.
 *
 * RENAMED 2026-09-27 from colony_at: String witness: COLONYFLAG is in Mac find_colony(short, short) and, on Windows, only here. Call graph: Mac find_colony calls on_map, is_colony and the error popup, and 471a calls on_map, is_colony and show_text_dialog. Its callers correspond almost one for one. Mac has CheckMenuItems, GenerateUnitStack, GameBrains::KeyboardEvent, MapWin::LeftMouseUp, StatWin::Stats, check_tut, collect_stats, combat_fight, coronado_colony, create_unit, explore_map_square, get_defense, indian_move, move_unit, negotiate_europe, negotiate_tribe, new_turn_bookkeeping, orders_build/disband/load/unload, plan_move, say_where and treaty_encounters. Windows has all of these under the same names, plus MapWin_LeftMouseClick, ReportWin_DoExtendedLaborReport and stack_icon. Arity: (short, short) against int colony_at(int x, int y). Size is 170 Mac against 149 Windows. The old name is descriptive. Apply this only after 1048:a253 has moved to find_city. It has 28 callers, so expect a wide rename footprint.
 */

#include "colonization.h"

#include "records.h"

extern GameState g_game;   /* SEG20:0x7782 */

extern int is_colony(int x, int y);
/* same object: 4-byte `push cs; call ` (rule 47) */

int find_colony(int x, int y)
{
    int i;
    int found = -1;

    if (!on_map(x, y))
        goto done;
    if (is_colony(x, y) < 0)
        goto done;
    for (i = 0; found < 0 && g_game.colony_count > i; i++)
        if (g_colonies[i].x == (unsigned char)x && g_colonies[i].y == (unsigned char)y)
            found = i;
    if (found < 0)
        show_text_dialog_1018_b3d1("COLONYFLAG");
done:
    return found;
}
