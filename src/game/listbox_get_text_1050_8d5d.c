/* listbox_get_text_1050_8d5d -- shipped at 1050:8d5d.
 *
 * Ported by tools/port.py from matched/game/listbox_get_text_1050_8d5d.c.
 *
 * The file's own DlgControl dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void listbox_get_text_1070_54b4(col_i16 h, col_i16 i, col_i16 a, col_i16 b);

void listbox_get_text_1050_8d5d(DlgControl *c, col_i16 i, col_i16 a, col_i16 b)
{
    listbox_get_text_1070_54b4(c->hwnd, i, a, b);
}
