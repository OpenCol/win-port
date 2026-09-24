/* file_MSOpenFile_1070_7e50 -- shipped at 1070:7e50.
 *
 * Ported by tools/port.py from matched/platform/file_MSOpenFile_1070_7e50.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern col_i16 MSOpenFile(col_i16 a, col_i16 b, col_i16 c, col_i16 d);

col_i16 file_MSOpenFile_1070_7e50(col_i16 a, col_i16 b, col_i16 c, col_i16 d)
{
    return MSOpenFile(a, b, c, d);
}
