/* mci_set_time_format -- shipped at 1078:2f32.
 *
 * Ported by tools/port.py from matched/platform/mci_set_time_format.c.
 *
 * The file's own MciRec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void mci_set_time_format(MciRec *r, char frames)
{
    if (frames != 0)
        r->fmt = 0x20000L;
    else
        r->fmt = 0x10000L;
}
