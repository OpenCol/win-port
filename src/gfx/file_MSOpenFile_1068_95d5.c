/* file_MSOpenFile_1068_95d5 -- shipped at 1068:95d5.
 *
 * Ported by tools/port.py from matched/gfx/file_MSOpenFile_1068_95d5.c.
 *
 * The file's own MSFile dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern col_i16 MSOpenFile(const char *name, MSFile *f);

col_i16 file_MSOpenFile_1068_95d5(MSFile *f, const char *name)
{
    return MSOpenFile(name, f);
}
