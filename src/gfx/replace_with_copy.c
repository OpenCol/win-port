/* replace_with_copy -- shipped at 1068:9393.
 *
 * Ported by tools/port.py from matched/gfx/replace_with_copy.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void handle_replace_with_copy(void *p);

void replace_with_copy(void *p)
{
    handle_replace_with_copy(p);
}
