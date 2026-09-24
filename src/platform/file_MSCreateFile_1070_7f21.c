/* file_MSCreateFile_1070_7f21 -- shipped at 1070:7f21.
 *
 * Ported by tools/port.py from matched/platform/file_MSCreateFile_1070_7f21.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern col_i16 MSCreateFile(col_i16 a, col_i16 b, col_i16 c, col_i16 d);

col_i16 file_MSCreateFile_1070_7f21(col_i16 a, col_i16 b, col_i16 c, col_i16 d)
{
    return MSCreateFile(a, b, c, d);
}
