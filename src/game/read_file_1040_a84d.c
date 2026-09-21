/* read_file_1040_a84d -- shipped at 1040:a84d.
 *
 * Ported by tools/port.py from matched/game/read_file_1040_a84d.c.
 *
 * The file's own MSFile dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern col_i16 MSReadFile(MSFile *f, void *buf, col_i32 n);

col_i16 read_file_1040_a84d(MSFile *f, void *buf, col_i32 n)
{
    return MSReadFile(f, buf, n);
}
