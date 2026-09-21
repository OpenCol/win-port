/* message_append -- shipped at 1048:10ef.
 *
 * Ported by tools/port.py from matched/game/message_append.c.
 *
 * STRCAT -> strcat: the C runtime's own, taken from the real header rather than
 * from this file's far-pointer prototype.
 *
 * The file's own Message dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include <string.h>
#include "col/globals.h"

void message_append(Message *m, char *s)
{
    strcat(((char *)((unsigned char *)(m) + 0x149)), s);
    strcat(((char *)((unsigned char *)(m) + 0x149)), " ");
}
