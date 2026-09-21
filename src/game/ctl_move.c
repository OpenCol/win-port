/* ctl_move -- shipped at 1048:d830.
 *
 * Ported by tools/port.py from matched/game/ctl_move.c.
 *
 * The file's own DlgControl dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void move_hwnd_keeping_size(col_i16 hwnd, col_i16 a, col_i16 b);

void ctl_move(DlgControl *c, col_i16 a, col_i16 b)
{
    move_hwnd_keeping_size(c->hwnd, a, b);
}
