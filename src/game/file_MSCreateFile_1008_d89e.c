/* file_MSCreateFile_1008_d89e -- shipped at 1008:d89e.
 *
 * Ported by tools/port.py from matched/game/file_MSCreateFile_1008_d89e.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern col_i16 file_MSCreateFile_1070_7f21(void *a, void *b);

col_i16 file_MSCreateFile_1008_d89e(void *a, void *b)
{
    return file_MSCreateFile_1070_7f21(b, a);
}
