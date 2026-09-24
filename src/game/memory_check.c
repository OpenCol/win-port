/* memory_check -- shipped at 1048:b9a4.
 *
 * Ported by tools/port.py from matched/game/memory_check.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void ask_text_resource(char *id);

void memory_check(void)
{
    ask_text_resource("MEMORY");
}
