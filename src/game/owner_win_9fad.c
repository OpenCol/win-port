/* owner_win_9fad -- shipped at 1050:780d.
 *
 * Ported by tools/port.py from matched/game/owner_win_9fad.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern char * current_window(void);
extern void end_modal_1060_9fad(void *win);

void owner_win_9fad(void)
{
    char *o;
    char *w = current_window();

    o = w != 0 ? w - 0x26 : w;
    end_modal_1060_9fad(o + 0x26);
}
