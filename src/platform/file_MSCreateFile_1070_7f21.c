/* file_MSCreateFile_1070_7f21 -- shipped at 1070:7f21.
 *
 * Ported by tools/port.py from matched/platform/file_MSCreateFile_1070_7f21.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern col_i16 MSCreateFile(char *name, void *f);

col_i16 file_MSCreateFile_1070_7f21(char *name, void *f)
{
    return MSCreateFile(name, f);
}
