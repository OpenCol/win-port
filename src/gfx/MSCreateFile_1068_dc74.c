/* MSCreateFile_1068_dc74 -- shipped at 1068:dc74.
 *
 * Ported by tools/port.py from matched/gfx/MSCreateFile_1068_dc74.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern col_i16 MSCreateFile(void *a, void *b);

col_i16 MSCreateFile_1068_dc74(void *a, void *b)
{
    return MSCreateFile(b, a);
}
