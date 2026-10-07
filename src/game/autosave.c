/* autosave: candidate for 1008:0000 (81 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: autosave
 *
 * Writes the autosave: slot 8, auto02.sav, when the YEAR is a multiple of ten and two
 * other globals allow it, and slot 9, auto01.sav, otherwise.
 *
 * It read `g_game.turn_counter` (SEG20:0x7790) until 2026-09-20; the shipped immediate is
 * 0x778c, which is `g_game.year`. Two adjacent counters, one masked fixup, no way for the
 * byte match to tell them apart.
 *
 * RENAMED 2026-09-26 from autosave_to_slot: the Mac build's autosave(void), bound by call-graph agreement (tools/mac_callgraph.py): 2 of its Mac call edges already bound land on this function (caller DoHuman; caller DoViceroy), the runner-up gets 1, and the arity agrees (0). The margin is small, so the binding also rests on the names describing the same behaviour: the descriptive name here was derived from the body before the Mac table was consulted.
 */

#include "colonization.h"

#include "records.h"
extern GameState g_game;   /* SEG20:0x7782 */
extern void perform_save(int slot);

void autosave(void)
{
    if (g_game.year % 10 == 0 && g_game.season == 0 && g_game.turn_counter > 2)
        perform_save(8);
    else
        perform_save(9);
}
