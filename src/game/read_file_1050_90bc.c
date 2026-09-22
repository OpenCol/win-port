/* read_file_1050_90bc -- shipped at 1050:90bc.
 *
 * Ported by tools/port.py from matched/game/read_file_1050_90bc.c.
 *
 * The file's own MSFile dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern col_i16 MSReadFile(MSFile *f, void *buf, col_i32 n);

col_i16 read_file_1050_90bc(MSFile *f, void *buf, col_i32 n)
{
    return MSReadFile(f, buf, n);
}
