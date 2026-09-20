/* control_hwnd -- shipped at 1070:a6d7.
 *
 * Ported by tools/port.py from matched/platform/control_hwnd.c.
 *
 * The file's own DlgControl dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 control_hwnd(DlgControl *c)
{
    col_i16 h;

    if (c == 0)
        return 0;
    h = c->hwnd;
    return h;
}
