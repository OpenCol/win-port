/* new_mood: candidate for 1040:89b3 (63 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: new_mood
 *
 * force_mood(u) when the word at DGROUP:0xb8 is set and 0xba clear,
 * set_mood(u) otherwise.  Both are -called and defined in-file as
 * stubs.
 *
 * RENAMED 2026-09-26 from music_mood_if_playing: the Mac build's new_mood(short), bound by call-graph agreement (tools/mac_callgraph.py): 5 of its Mac call edges already bound land on this function (caller new_turn_colony; caller produce_something; caller raise_taxes; caller special_kill; callee force_mood), the runner-up gets 3, and the arity agrees (1).
 */

#include "colonization.h"

extern int g_stub_a;

extern void force_mood(int u);
extern void set_mood(int u);

void new_mood(int mood)
{
    if (g_sound_event_music != 0 && g_sound_music == 0)
        force_mood(mood);
    else
        set_mood(mood);
}
