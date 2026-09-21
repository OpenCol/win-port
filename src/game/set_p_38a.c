/* set_p_38a -- shipped at 1050:1045.
 *
 * Ported by tools/port.py from matched/game/set_p_38a.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void set_p_38a(Rec *r, void *h)
{
    (*((void * *)((unsigned char *)(r) + 0x38a))) = h;
}
