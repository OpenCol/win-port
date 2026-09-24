/* colony_copy_backdrop -- shipped at 1010:0eb3.
 *
 * Ported by tools/port.py from matched/game/colony_copy_backdrop.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void CopyToPort(void *src, void *dst, RECT *sr, RECT *dr);

void colony_copy_backdrop(char *o)
{
    CopyToPort(o + 0xea, o + 0xc4, (RECT *)(o + 0x552), (RECT *)(o + 0x55a));
}
