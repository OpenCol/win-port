/* set_midi_done_cb -- shipped at 1068:a5e6.
 *
 * Ported by tools/port.py from matched/gfx/set_midi_done_cb.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern void far *g_midi_done_cb;                     DGROUP:0x5698
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void set_midi_done_cb(void *fn)
{
    g_midi_done_cb = fn;
}
