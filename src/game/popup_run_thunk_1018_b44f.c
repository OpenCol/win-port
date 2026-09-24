/* popup_run_thunk_1018_b44f -- shipped at 1018:b44f.
 *
 * Ported by tools/port.py from matched/game/popup_run_thunk_1018_b44f.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern col_i16 popup_run(void *p, char *text);

col_i16 popup_run_thunk_1018_b44f(void *p)
{
    return popup_run(p, 0);
}
