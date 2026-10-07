/* call_midi_done_callback: candidate for 1068:a5fc (26 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * Fires the MIDI-finished hook, if one is installed. The hook lives at DGROUP:0x5698 and
 * `set_midi_done_cb` is the only thing that writes it; the function directly
 * above that setter is the one that starts a MIDI file playing.
 *
 * The caller is the MM_MCINOTIFY arm of the MSMMWindow procedure: on 0x3b9 it closes the
 * device, and when the device that finished is the one in `[0x3c78]` it calls this and
 * clears the handle. The CD twin is `call_cd_done_callback` on `[0x3c7a]`.
 */

#include "colonization.h"

extern void (*g_midi_done_cb)(void);   /* DGROUP:0x5698 */

void call_midi_done_callback(void)
{
    if (g_midi_done_cb != 0)
        (*g_midi_done_cb)();
}
