/* ColChild_BackspaceMessage -- shipped at 1048:1227.
 *
 * Ported by tools/port.py from matched/game/ColChild_BackspaceMessage.c.
 *
 * STRLEN -> strlen: the C runtime's own, taken from the real header rather than
 * from this file's far-pointer prototype.
 *
 * The file's own Win dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include <string.h>
#include "col/globals.h"

void ColChild_BackspaceMessage(Win *w)
{
    if (strlen(((char *)((unsigned char *)(w) + 0x149))) != 0)
        ((char *)((unsigned char *)(w) + 0x149))[strlen(((char *)((unsigned char *)(w) + 0x149))) - 1] = 0;
}
