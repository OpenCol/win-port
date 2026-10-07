/* tell_unit_in_europe: candidate for 1038:b344 (185 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: tell_unit_in_europe
 *
 * Puts a unit message on the map window (SEG20:0x658): the unit's nation
 * adjective (SEG20:0xd89e table by nation nibble), its type name, viceroy
 * text 7, the word for the current nation (SEG20:0xd8ae by SEG20:0x7796),
 * shown as kind 1 for 120 ticks.  The two viceroy helpers are -called
 * and defined in-file as stubs.
 *
 * RENAMED 2026-09-26 from viceroy_unit_msg7: the map-window message "<adjective> <unit type> Now Arriving In <nation word>": MISC text 7
 * of LABELS.TXT (load_data fills g_misc_words from it) is "Now Arriving In". Callers: process_off_board_ships.
 *
 * RENAMED 2026-09-26 from viceroy_msg_now_arriving: the Mac build's tell_unit_in_europe(short), bound by call-graph agreement: 5 of its Mac call edges already bound land on this function (caller process_off_board_ships; callee AddToMessage; callee CancelMessage; callee viceroy_activate_message), the runner-up gets 4; the arity agrees (1), and the body does what the Mac name says. The Mac pair tell_unit_from_europe/tell_unit_in_europe are each other's runner-up (5 vs 4); this is viceroy text 7, the now-arriving line.
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

void tell_unit_in_europe(int u)
{
    ColChild_CancelMessage((ColChild *)&g_map_window, 1);
    ColChild_AddToMessage((ColChild *)&g_map_window, g_nation_adjective[(unsigned char)(g_units[u].flags & UNIT_NATION_MASK)]);
    ColChild_AddToMessage((ColChild *)&g_map_window, g_unit_types[g_units[u].type].name);
    viceroy_message(MISC_NOW_ARRIVING_IN);
    ColChild_AddToMessage((ColChild *)&g_map_window, g_nation_name_word[g_game.turn_nation]);
    viceroy_activate_message(1, 120L);
}

