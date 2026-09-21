/* call_d214 -- shipped at 1068:9487.
 *
 * Ported by tools/port.py from matched/gfx/call_d214.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void free_space(void);

void call_d214(void)
{
    free_space();
    return;
}
