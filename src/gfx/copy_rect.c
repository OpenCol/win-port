/* copy_rect -- shipped at 1068:996f.
 *
 * Ported by tools/port.py from matched/gfx/copy_rect.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void copy_rect(RECT *src, RECT *dst)
{
    *dst = *src;
}
