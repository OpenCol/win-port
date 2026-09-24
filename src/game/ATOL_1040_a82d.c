/* ATOL_1040_a82d -- shipped at 1040:a82d.
 *
 * Ported by tools/port.py from matched/game/ATOL_1040_a82d.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void * ATOL(void *p);

void * ATOL_1040_a82d(void *p)
{
    return ATOL(p);
}
