/* ColonyWin_Close -- shipped at 1010:afa9.
 *
 * Ported by tools/port.py from matched/game/ColonyWin_Close.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern char * current_window(void);
extern void win_close_and_reset(void *o);

col_i16 ColonyWin_Close(void)
{
    char *o;
    char *w = current_window();

    o = w != 0 ? w - 0x26 : w;

    if (o != 0)
        win_close_and_reset(o);
    return 0;
}
