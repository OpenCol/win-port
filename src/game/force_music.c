/* force_music: candidate for 1040:88e1 (85 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: force_music
 *
 * REQUESTS A PIECE OF MUSIC. `t` is a track code -- every caller passes one, from
 * SOUND_MEETKING and SOUND_MUSIC_JINE_THE_CAVALRY to the bare 0x32 and 0x33 of
 * `woodcut` -- and it is left at DGROUP:0x1d10 for `check_music`
 * to take, unless it is already the track playing (0x1d12). A change is then requested
 * at 0x1d1a when music is on (DGROUP:0xb8 set, 0xba clear), the current piece is
 * stopped, and the CD flag at 0x1d1c is set. `check_music` runs either
 * way (-called, defined in-file as a stub).
 *
 * THE OLD NAME SAID UNIT. It was `select_unit_1040_88e1` and called 0x1d10 the selected
 * unit; `check_music` had already written down that these three globals
 * are music state and that this function leaves a REQUESTED TRACK. The callers settle
 * it: not one of them passes a unit index.
 *
 * RENAMED 2026-09-26 from music_request_track: the Mac build's force_music(short), bound by call-graph agreement (tools/mac_callgraph.py): 4 of its Mac call edges already bound land on this function (caller combat_fight; caller new_turn_bookkeeping; caller resolve_raid; callee check_music), the runner-up gets 3, and the arity agrees (1). The margin is one edge, so the binding also rests on the two names describing the same behaviour: the descriptive name here was derived from the body before the Mac table was consulted.
 */

#include "colonization.h"

#include "sounds.h"

extern int g_stub_a;
extern void sound_play(int n);

extern void check_music(void);

void force_music(int track)
{
    if (track != g_cur_track) {
        g_pending_track = track;
        if (g_sound_event_music != 0 && g_sound_music == 0)
            g_music_change = 1;
        if (track >= 0)
            sound_play(SOUND_STOP_MUSIC);
        g_cd_is_game_disc = 1;
    }
    check_music();
}
