/* ColChild_AddBucksToMessage -- shipped at 1048:11c4.
 *
 * Ported by tools/port.py from matched/game/ColChild_AddBucksToMessage.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void message_append(void *w, char *s);

extern void say_bucks(char *s, col_i32 v);
/* same object: 4-byte `push cs; call ` (rule 47) */

void ColChild_AddBucksToMessage(void *w, col_i32 bucks)
{
    char buf[0x14];

    buf[0] = 0;
    say_bucks(buf, bucks);
    message_append(w, buf);
}
