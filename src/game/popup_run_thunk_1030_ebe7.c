/* popup_run_thunk_1030_ebe7 -- shipped at 1030:ebe7.
 *
 * Ported by tools/port.py from matched/game/popup_run_thunk_1030_ebe7.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern col_i16 popup_run(void *p, char *text);

col_i16 popup_run_thunk_1030_ebe7(void *p)
{
    return popup_run(p, 0);
}
