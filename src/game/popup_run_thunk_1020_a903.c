/* popup_run_thunk_1020_a903 -- shipped at 1020:a903.
 *
 * Ported by tools/port.py from matched/game/popup_run_thunk_1020_a903.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern col_i16 popup_run(void *p, char *text);

col_i16 popup_run_thunk_1020_a903(void *p)
{
    return popup_run(p, 0);
}
