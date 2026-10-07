/* CheatMenuColonist: candidate for 1048:bbe2 (936 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * size-source: 936, not the census 164 and not the 937-byte gap. The census stops at the jump
 *   table's dispatch. The shipped function has ONE PAD BYTE between its epilogue and the 14-entry
 *   jump table that the object does not emit -- the same thing `check_music`
 *   records -- so 937 is one byte more than the compile produces and 936 is the largest size
 *   that compares. The pad is not alignment within the object: moving the function to an odd
 *   or an even object offset leaves it out either way.
 *
 * The Cheat menu's "Create Unit": puts a unit of the chosen kind on the cursor square, for the
 * player's nation unless another has been picked, as long as the game is under 300 units. The
 * prompt is TEXT resource "CREATE", or "CREATE2" when bit 0 of SEG20:0x7784 is set, and its
 * answer picks the unit:
 *
 *   1-6   unit types 0, 2, 1, 3, 5 and 0xb
 *   7     type 0xc, whose byte +6 is set to the nearest colony
 *   8     type 0xa, with byte +0x17 set to 1
 *   9     "CSHIP" asks for a ship, created as type 0xc + the answer
 *   10-13 with bit 0 clear, a native unit (type answer + 9) for the tribe of the nearest village,
 *         if there is one; with it set, type 9, 7, 6 or 8 for one of two special nations
 *   14    "FOREIGN" or "FOREIGN2" changes the nation, and the prompt comes round again
 *
 * The unit made last, if any, gets 0xf0 ORed into byte +3, and the map is redrawn.
 *
 * `type` in the 10-13 arm is assigned by four separate `if`s rather than a switch, and nothing
 * assigns it on any other path. The FOREIGN2 answer is a two-case `switch` whose case 0 shares
 * the default's body, laid out case 1 first (rule 36).
 *
 * RENAMED 2026-09-27 from create_unit: String witness: CREATE, CREATE2, CSHIP, FOREIGN, FOREIGN2 and GAME, all df 1/1, are in Mac CheatMenuColonist(void), with a score of 82.85 against a runner-up of 0. Call graph: Mac CheatMenuColonist is called by MainWin::MainMenuProc, and bbe2 is called by MainWin_MainMenuProc (Cheat menu id 800, 'Create Unit'). The Mac sequence is create_unit x11, find_close_colony, find_close_village and DrawMap, and bbe2 calls create_unit_1038_2246 x11, find_close_colony, find_close_village, DrawMap and ask_text_resource x4. Arity: (void) against void create_unit(void). Size is 920 Mac against 936 Windows. Why it went wrong: the name was derived correctly from the menu caption 'Create Unit', and the Mac symbol table happens to contain create_unit, which was then read as corroboration. The record itself says 'What this does NOT establish: that the Mac function of that name IS this Windows function'. The Mac create_unit is the allocator at 1038:2246. The name follows the corpus convention for Mac CamelCase free functions (BuildSaveGameName, KingRoom, CheatMenuBlow). It is free.
 */

#include "colonization.h"

#include "jobs.h"
#include "units.h"
#include "gameflag.h"
#include "records.h"

extern GameState g_game;   /* SEG20:0x7782 */

extern int find_close_colony(int x, int y, int nation, int land);
extern int find_close_village(int x, int y, int nation, int owner);
extern void DrawMap(int flag);

/* same object: 4-byte `push cs; call ` (rule 47) */

void CheatMenuColonist(void)
{
    int choice;
    int village;
    int type;
    register int nation;
    register int u;

    if (g_game.unit_count < 300) {
        nation = g_game.turn_nation;
        u = -1;
        do {
            g_popup_dude = 1;
            if (g_game.game_flags & GAMEFLAG_INDEPENDENCE)
                choice = ask_text_resource("CREATE2");
            else
                choice = ask_text_resource("CREATE");
            switch (choice) {
            case 1:
                u = create_unit(UNIT_COLONISTS, nation, g_cursor_x, g_cursor_y);
                break;
            case 2:
                u = create_unit(UNIT_PIONEERS, nation, g_cursor_x, g_cursor_y);
                break;
            case 3:
                u = create_unit(UNIT_SOLDIERS, nation, g_cursor_x, g_cursor_y);
                break;
            case 4:
                u = create_unit(UNIT_MISSIONARIES, nation, g_cursor_x, g_cursor_y);
                break;
            case 5:
                u = create_unit(UNIT_SCOUTS, nation, g_cursor_x, g_cursor_y);
                break;
            case 6:
                u = create_unit(UNIT_ARTILLERY, nation, g_cursor_x, g_cursor_y);
                break;
            case 7:
                u = create_unit(UNIT_WAGON_TRAIN, nation, g_cursor_x, g_cursor_y);
                g_units[u].home = find_close_colony(g_cursor_x, g_cursor_y, nation, -1);
                break;
            case 8:
                u = create_unit(UNIT_TREASURE_TRAIN, nation, g_cursor_x, g_cursor_y);
                g_units[u].spec = JOB_SUGAR_PLANTER;
                break;
            case 9:
                g_popup_dude = 0;
                choice = show_text_dialog_1048_e3ae("CSHIP", 0);
                if (choice > 0)
                    u = create_unit(choice + 0xc, nation, g_cursor_x, g_cursor_y);
                break;
            case 10:
            case 11:
            case 12:
            case 13:
                if ((g_game.game_flags & GAMEFLAG_INDEPENDENCE) == 0) {
                    village = find_close_village(g_cursor_x, g_cursor_y, -1, -1);
                    if (village >= 0)
                        u = create_unit(choice + 9, g_cur_tribe_nation,
                                                  g_cursor_x, g_cursor_y);
                } else {
                    nation = choice < 0xc ? g_game.player_nation : g_game.mother_country_idx;
                    if (choice == 10)
                        type = 9;
                    if (choice == 11)
                        type = 7;
                    if (choice == 12)
                        type = 6;
                    if (choice == 13)
                        type = 8;
                    u = create_unit(type, nation, g_cursor_x, g_cursor_y);
                }
                break;
            case 14:
                g_popup_dude = 2;
                if ((g_game.game_flags & GAMEFLAG_INDEPENDENCE) == 0) {
                    nation = ask_text_resource("FOREIGN") - 1;
                    if (nation < 0)
                        nation = g_game.turn_nation;
                } else {
                    nation = ask_text_resource("FOREIGN2") - 1;
                    switch (nation) {
                    case 1:
                        nation = g_game.mother_country_idx;
                        break;
                    case 0:
                    default:
                        nation = g_game.player_nation;
                        break;
                    }
                }
                break;
            }
        } while (choice == 14);
        if (u >= 0)
            g_units[u].flags |= 0xf0;
        DrawMap(1);
    }
}
