/* file_open_dialog -- shipped at 1070:8309.
 *
 * Ported by tools/port.py from matched/platform/file_open_dialog.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern col_i16 file_dialog(void *a, void *b, void *c, col_i16 d, col_i16 save);

col_i16 file_open_dialog(void *a, void *b, col_i16 d)
{
    return file_dialog(a, b, 0, d, 0);
}
