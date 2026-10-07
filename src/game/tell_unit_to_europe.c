/* tell_unit_to_europe: candidate for 1038:b28b (185 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: tell_unit_to_europe
 *
 * Puts a unit message on the map window (SEG20:0x658): the unit's nation
 * adjective (SEG20:0xd89e table by nation nibble), its type name, viceroy
 * text 5, the word for the current nation (SEG20:0xd8ae by SEG20:0x7796),
 * shown as kind 1 for 120 ticks.  The two viceroy helpers are -called
 * and defined in-file as stubs.
 *
 * RENAMED 2026-09-26 from viceroy_unit_msg5: the map-window message "<adjective> <unit type> Sailing For <nation word>": MISC text 5
 * of LABELS.TXT (load_data fills g_misc_words from it) is "Sailing For". Callers: orders_europe, perform_orders_move_to, execute_move_unit.
 *
 * RENAMED 2026-09-26 from viceroy_msg_sailing_for: the Mac build's tell_unit_to_europe(short), bound by call-graph agreement (tools/mac_callgraph.py): 6 of its Mac call edges already bound land on this function (caller execute_move_unit; caller perform_orders_move_to; callee AddToMessage; callee CancelMessage; callee viceroy_activate_message), the runner-up gets 4, and the arity agrees (1).
 */

#include "colonization.h"

#include "units.h"
#include "misc.h"
#include "w16types.h"
#include "records.h"

#include "classes.h"

extern GameState g_game;   /* SEG20:0x7782 */
extern int g_stub_a;
extern void ColChild_CancelMessage(ColChild *child, int a);
extern void ColChild_AddToMessage(ColChild *child, int idx);

void tell_unit_to_europe(int u)
{
    ColChild_CancelMessage((ColChild *)&g_map_window, 1);
    ColChild_AddToMessage((ColChild *)&g_map_window, g_nation_adjective[(unsigned char)(g_units[u].flags & UNIT_NATION_MASK)]);
    ColChild_AddToMessage((ColChild *)&g_map_window, g_unit_types[g_units[u].type].name);
    viceroy_message(MISC_SAILING_FOR);
    ColChild_AddToMessage((ColChild *)&g_map_window, g_nation_name_word[g_game.turn_nation]);
    viceroy_activate_message(1, 120L);
}

