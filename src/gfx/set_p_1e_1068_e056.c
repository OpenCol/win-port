/* set_p_1e_1068_e056 -- shipped at 1068:e056.
 *
 * Ported by tools/port.py from matched/gfx/set_p_1e_1068_e056.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void set_p_1e_1068_e056(Rec *r, void *h)
{
    (*((void * *)((unsigned char *)(r) + 0x1e))) = h;
}
