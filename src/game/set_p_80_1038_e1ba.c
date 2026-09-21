/* set_p_80_1038_e1ba -- shipped at 1038:e1ba.
 *
 * Ported by tools/port.py from matched/game/set_p_80_1038_e1ba.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void set_p_80_1038_e1ba(Rec *r, void *h)
{
    (*((void * *)((unsigned char *)(r) + 0x80))) = h;
}
