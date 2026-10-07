/* force_mood: candidate for 1040:895e (85 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: force_mood
 *
 * SETS THE MUSIC MOOD and picks a track for it. The argument is a mood id, 1..7, and NOT a unit:
 * when it differs from the current mood at DGROUP:0x1d18 the function stops whatever is playing
 * (`SOUND_STOP_MUSIC`), asks for a refresh and sets the CD flag, and it calls
 * `check_music` either way.
 *
 * RENAMED from `select_unit_sound_1040_895e`, whose parameter was read as a unit index. It is not
 * one, and four things say so: every literal argument at all 41 call sites is 1..7; no site passes
 * a unit; the body stops the music and picks a new track, which is not what arriving at a unit
 * does; and the callers are narrative moments rather than unit events -- starvation plays 1, Europe
 * 2, the turn advancing and the declaration of independence 3, meeting another European 4, and
 * entering a native village 5, 6 or 7 by the tribe's class. `raise_taxes` settles it: a tax
 * CUT plays mood 2 through the wrapper, and no unit arrives when taxes fall.
 *
 * DGROUP:0x1d18 is therefore the current mood, not the last unit, and is `g_prev_group` here. The
 * name the reconstructions could not agree on is visible in the old declarations, which spell the
 * parameter `int n`, `int kind`, `int a`, `int u` and `sound` in different files -- five readings
 * of one argument, which is what an unidentified parameter looks like.
 *
 * WHAT EACH MOOD IS is NOT established. The ids are named nowhere and only their call sites
 * suggest a meaning, so no header names them; only the function and the global are renamed.
 *
 * RENAMED 2026-09-26 from music_set_mood: the Mac build's force_mood(short), bound by call-graph agreement (tools/mac_callgraph.py): 6 of its Mac call edges already bound land on this function (caller DoEndofTurn; caller combat_fight; caller intervention; caller move_unit; caller new_turn_colony), the runner-up gets 4, and the arity agrees (1).
 */

#include "colonization.h"

#include "sounds.h"

extern int g_stub_a;
extern void sound_play(int n);

extern void set_mood(int u);
extern void check_music(void);

void force_mood(int mood)
{
    set_mood(mood);
    if (g_prev_group != mood) {
        if (g_sound_event_music != 0 && g_sound_music == 0)
            g_music_change = 1;
        sound_play(SOUND_STOP_MUSIC);
        g_cd_is_game_disc = 1;
    }
    check_music();
}
