/* set_mood: candidate for 1040:8936 (20 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Stores the mood -- the music group -- into `g_mood` (DGROUP:0x1d16), the word `music_set_next_group`
 * (immediately after) sits beside: that one sets `g_next_mood` at 0x1d14, which the player
 * later copies into this one and clears -- so this is the direct setter, bypassing the queue.
 *
 * RENAMED 2026-09-26 from music_mood_store: the Mac build's set_mood(short), bound by call-graph agreement (tools/mac_callgraph.py): 8 of its Mac call edges already bound land on this function (caller allied_invasion; caller europe_new_turn; caller force_mood; caller new_mood; caller new_turn_colony), the runner-up gets 6, and the arity agrees (1).
 */

#include "colonization.h"

extern int g_mood;   /* DGROUP:0x1d16 */

void set_mood(int mood)
{
    g_mood = mood;
}
