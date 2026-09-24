/* file_open_dialog_ex -- shipped at 1070:8337.
 *
 * Ported by tools/port.py from matched/platform/file_open_dialog_ex.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern col_i16 file_dialog(void *a, void *b, void *c, col_i16 d, col_i16 save);

col_i16 file_open_dialog_ex(void *a, void *b, void *c, col_i16 d)
{
    return file_dialog(a, b, c, d, 0);
}
