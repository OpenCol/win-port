/* ColChild_AddLongToMessage -- shipped at 1048:118b.
 *
 * Ported by tools/port.py from matched/game/ColChild_AddLongToMessage.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void message_append(void *w, char *s);

extern void ltoa_rtl(col_i32 v, char *s, col_i16 radix);
/* same object: 4-byte `push cs; call ` (rule 47) */

void ColChild_AddLongToMessage(void *w, col_i32 v)
{
    char buf[0x14];

    ltoa_rtl(v, buf, 10);
    message_append(w, buf);
}
