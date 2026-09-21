/* edit_set_rows -- shipped at 1050:8a55.
 *
 * Ported by tools/port.py from matched/game/edit_set_rows.c.
 *
 * The file's own EditBox dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void win_send_415(col_u16 hwnd, col_i16 rows);

void edit_set_rows(EditBox *e, col_i16 rows)
{
    win_send_415(e->hwnd, rows);
}
