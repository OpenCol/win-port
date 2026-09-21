/* ColChild_AddToMessage -- shipped at 1048:112b.
 *
 * Ported by tools/port.py from matched/game/ColChild_AddToMessage.c.
 *
 * The file's own Message dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void message_append(Message *m, char *s);

extern char * nth_text_string(col_i16 n);
/* same object: 4-byte `push cs; call ` (rule 47) */

void ColChild_AddToMessage(Message *m, col_i16 idx)
{
    message_append(m, nth_text_string(idx));
}
