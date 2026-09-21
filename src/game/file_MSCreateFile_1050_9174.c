/* file_MSCreateFile_1050_9174 -- shipped at 1050:9174.
 *
 * Ported by tools/port.py from matched/game/file_MSCreateFile_1050_9174.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern col_i16 MSCreateFile(void *a, void *b);

col_i16 file_MSCreateFile_1050_9174(void *a, void *b)
{
    return MSCreateFile(b, a);
}
