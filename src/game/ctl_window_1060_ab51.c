/* ctl_window_1060_ab51 -- shipped at 1060:ab51.
 *
 * Ported by tools/port.py from matched/game/ctl_window_1060_ab51.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void * ctl_window_1060_ab51(Rec *r)
{
    return (*((void * *)((unsigned char *)(r) + 0x4)));
}
