/* read_file_1038_e4b8 -- shipped at 1038:e4b8.
 *
 * Ported by tools/port.py from matched/game/read_file_1038_e4b8.c.
 *
 * The file's own MSFile dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern col_i16 MSReadFile(MSFile *f, void *buf, col_i32 n);

col_i16 read_file_1038_e4b8(MSFile *f, void *buf, col_i32 n)
{
    return MSReadFile(f, buf, n);
}
