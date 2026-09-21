/* set_p_1a_1060_6685 -- shipped at 1060:6685.
 *
 * Ported by tools/port.py from matched/game/set_p_1a_1060_6685.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void set_p_1a_1060_6685(Rec *r, void *h)
{
    (*((void * *)((unsigned char *)(r) + 0x1a))) = h;
}
