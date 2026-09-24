/* popup_run_thunk_1010_e4b1 -- shipped at 1010:e4b1.
 *
 * Ported by tools/port.py from matched/game/popup_run_thunk_1010_e4b1.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern col_i16 popup_run(void *p, char *text);

col_i16 popup_run_thunk_1010_e4b1(void *p)
{
    return popup_run(p, 0);
}
