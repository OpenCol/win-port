/* set_p_1a_1058_bb68 -- shipped at 1058:bb68.
 *
 * Ported by tools/port.py from matched/game/set_p_1a_1058_bb68.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void set_p_1a_1058_bb68(Rec *r, void *h)
{
    (*((void * *)((unsigned char *)(r) + 0x1a))) = h;
}
