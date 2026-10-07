/* set_midi_done_cb: candidate for 1068:a5e6 (22 bytes).  Verdict: RELOCATABLE.
 *
 * Stores the function pointer at DGROUP:0x5698.
 */

#include "colonization.h"

extern void (*g_midi_done_cb)(void);                /* DGROUP:0x5698 */

void set_midi_done_cb(void *fn)
{
    g_midi_done_cb = (void (*)(void))fn;
}
