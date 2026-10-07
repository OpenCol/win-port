/* midi_play_thunk -- shipped at 1068:a5ca.
 *
 * Ported by tools/port.py from matched/gfx/midi_play_thunk.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void midi_play(void *p);

void midi_play_thunk(void *p)
{
    midi_play(p);
}
