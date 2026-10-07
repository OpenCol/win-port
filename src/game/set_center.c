/* PORT: this file's own declarations of GameBrains_CursorUpdate removed: the header declares it from the definition (W1 step 3). */
/* set_center: candidate for 1038:afe2 (155 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: set_center
 *
 * Moves the map to (x, y): in game state 1 (SEG20:0x7792) the cursor is
 * placed there (set_cursor_pos), with `flag` the unit-cycle word at
 * DGROUP:0x76 is cleared around a re-sort of game record 2 (SEG20:0x40d6);
 * then the centre words are set and DrawMap(1) redraws.
 * set_cursor_pos is -called and defined in-file as a stub.
 *
 * RENAMED 2026-09-26 from map_goto: the Mac build's set_center(short, short, short), bound by call-graph agreement (tools/mac_callgraph.py): 6 of its Mac call edges already bound land on this function (caller allied_invasion; caller king_invasion; caller negotiate_tribe; caller orders_disband; caller tribe_camp), the runner-up gets 3, and the arity agrees (3).
 */

#include "colonization.h"

#include "records.h"
extern GameState g_game;   /* SEG20:0x7782 */

extern GameBrains g_game_brains;          /* SEG20:0x40d6 */

extern int g_stub_a;

extern void DrawMap(int flag);

void set_center(int x, int y, int flag)
{
    if (on_map(x, y) != 0) {
        if (g_game.view_mode == 1) {
            if (flag != 0 && g_cycle_flag != 0)
                GameBrains_CursorUpdate(&g_game_brains);
            set_cursor_pos(x, y);
            if (flag != 0) {
                g_cycle_flag = 0;
                GameBrains_CursorUpdate(&g_game_brains);
            }
        }
        g_map_center_x = x;
        g_map_center_y = y;
        DrawMap(1);
    }
}
