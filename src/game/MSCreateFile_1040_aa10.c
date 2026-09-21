/* MSCreateFile_1040_aa10 -- shipped at 1040:aa10.
 *
 * Ported by tools/port.py from matched/game/MSCreateFile_1040_aa10.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern col_i16 MSCreateFile(void *a, void *b);

col_i16 MSCreateFile_1040_aa10(void *a, void *b)
{
    return MSCreateFile(b, a);
}
