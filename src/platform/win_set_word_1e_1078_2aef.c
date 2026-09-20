/* win_set_word_1e_1078_2aef -- shipped at 1078:2aef.
 *
 * Ported by tools/port.py from matched/platform/win_set_word_1e_1078_2aef.c.
 *
 * The file's own MSWin dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void win_set_word_1e_1078_2aef(MSWin *w, col_i16 v)
{
    if (w == 0)
        return;
    w->f1e = v;
}
