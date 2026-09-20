/* return_null -- shipped at 1070:a89b.
 *
 * Ported by tools/port.py from matched/platform/return_null.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void * return_null(void *p)
{
    if (p == 0)
        return 0;
    return 0;
}
