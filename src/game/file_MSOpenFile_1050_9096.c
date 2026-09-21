/* file_MSOpenFile_1050_9096 -- shipped at 1050:9096.
 *
 * Ported by tools/port.py from matched/game/file_MSOpenFile_1050_9096.c.
 *
 * The file's own MSFile dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern col_i16 MSOpenFile(const char *name, MSFile *f);

col_i16 file_MSOpenFile_1050_9096(MSFile *f, const char *name)
{
    return MSOpenFile(name, f);
}
