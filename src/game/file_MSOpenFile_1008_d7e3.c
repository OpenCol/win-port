/* file_MSOpenFile_1008_d7e3 -- shipped at 1008:d7e3.
 *
 * Ported by tools/port.py from matched/game/file_MSOpenFile_1008_d7e3.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern col_i16 file_MSOpenFile_1070_7e50(void *a, void *b);

col_i16 file_MSOpenFile_1008_d7e3(void *a, void *b)
{
    return file_MSOpenFile_1070_7e50(b, a);
}
