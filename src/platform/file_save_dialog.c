/* file_save_dialog -- shipped at 1070:8368.
 *
 * Ported by tools/port.py from matched/platform/file_save_dialog.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern col_i16 file_dialog(void *a, void *b, void *c, col_i16 d, col_i16 save);

col_i16 file_save_dialog(void *a, void *b, void *c, col_i16 d)
{
    return file_dialog(a, b, c, d, 1);
}
