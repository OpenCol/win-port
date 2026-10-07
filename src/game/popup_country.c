/* popup_country: candidate for 1050:5ef7 (57 bytes, 15 callers).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Builds a nation's word into a local buffer and stores it in name slot `slot`.
 *
 * RENAMED 2026-09-26 from set_nation_name_slot: the Mac build's popup_country(short, short, short), bound by call-graph agreement (tools/mac_callgraph.py): 4 of its Mac call edges already bound land on this function (caller Show; caller add_liberty; caller end_of_game_check; caller intervention), the runner-up gets 2, and the arity agrees (3).
 */

#include "colonization.h"

/* same object: 4-byte `push cs; call ` (rule 47) */

void popup_country(int slot, int flag, int nation)
{
    char buf[0x50];

    buf[0] = 0;
    say_country(nation, flag, buf);
    set_name_slot_1050_8e3a(slot, buf);
}
