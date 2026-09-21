/* MSCreateFile_1068_9565 -- shipped at 1068:9565.
 *
 * Ported by tools/port.py from matched/gfx/MSCreateFile_1068_9565.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern col_i16 MSCreateFile(void *a, void *b);

col_i16 MSCreateFile_1068_9565(void *a, void *b)
{
    return MSCreateFile(b, a);
}
