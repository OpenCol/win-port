/* audio_play_element -- shipped at 1068:d5b3.
 *
 * Ported by tools/port.py from matched/gfx/audio_play_element.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void * window_of_1068_d72a(void *o);
void win_set_focus_1068_e278(void *o);

typedef struct { char pad[0x8c]; void *mci; } Audio;
extern void mci_play(void *win, void *mci);

/* same object: 4-byte `push cs; call ` */



void audio_play_element(Audio *o)
{
    if (o->mci != 0) {
        mci_play(window_of_1068_d72a(o), o->mci);
        win_set_focus_1068_e278(o);
    }
}
