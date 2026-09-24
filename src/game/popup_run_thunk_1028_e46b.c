/* popup_run_thunk_1028_e46b -- shipped at 1028:e46b.
 *
 * Ported by tools/port.py from matched/game/popup_run_thunk_1028_e46b.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern col_i16 popup_run(void *p, char *text);

col_i16 popup_run_thunk_1028_e46b(void *p)
{
    return popup_run(p, 0);
}
