/* file_MSOpenFile_1070_7e50 -- shipped at 1070:7e50.
 *
 * Ported by tools/port.py from matched/platform/file_MSOpenFile_1070_7e50.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern col_i16 MSOpenFile(char *name, void *f);

col_i16 file_MSOpenFile_1070_7e50(char *name, void *f)
{
    return MSOpenFile(name, f);
}
