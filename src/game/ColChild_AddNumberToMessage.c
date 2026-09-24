/* ColChild_AddNumberToMessage -- shipped at 1048:1153.
 *
 * Ported by tools/port.py from matched/game/ColChild_AddNumberToMessage.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void message_append(void *w, char *s);

extern void LTOA1(col_i16 v, char *s, col_i16 radix);
/* same object: 4-byte `push cs; call ` (rule 47) */

void ColChild_AddNumberToMessage(void *w, col_i16 v)
{
    char buf[0x14];

    LTOA1(v, buf, 10);
    message_append(w, buf);
}
