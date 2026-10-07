/* midi_play_thunk: candidate for 1068:a5ca (28 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * This module's one-line forwarder to `midi_play`, named for the routine it reaches.
 *
 * The callee is a call into another segment and is `extern`.
 */

#include "colonization.h"

void midi_play_thunk(void *p)
{
    midi_play((const char *)p);
}
