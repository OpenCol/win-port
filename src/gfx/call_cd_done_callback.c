/* call_cd_done_callback: candidate for 1068:a645 (26 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * Fires the CD-audio-finished hook, if one is installed. The hook lives at DGROUP:0x569c,
 * written only by `SetCDCallback`, whose neighbour above is the wrapper
 * around `MSPlayCDTrack`. The MIDI twin is `call_midi_done_callback` on 0x5698.
 */

#include "colonization.h"

extern void (*g_cd_done_cb)(void);   /* DGROUP:0x569c */

void call_cd_done_callback(void)
{
    if (g_cd_done_cb != 0)
        (*g_cd_done_cb)();
}
