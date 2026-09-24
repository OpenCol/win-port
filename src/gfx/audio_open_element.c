/* audio_open_element -- shipped at 1068:d560.
 *
 * Ported by tools/port.py from matched/gfx/audio_open_element.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void * window_of_1068_d72a(void *o);

typedef struct { char pad[0x8c]; void *mci; } Audio;
/* same object: 4-byte `push cs; call ` (rule 47) */

extern void * mci_open_element(void *a, char *name);

col_i16 audio_open_element(Audio *o, char *name)
{
    o->mci = mci_open_element(window_of_1068_d72a(o), name);
    if (o->mci != 0)
        return 1;
    else
        return 0;
}
