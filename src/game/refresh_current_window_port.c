/* refresh_current_window_port -- shipped at 1048:897a.
 *
 * Ported by tools/port.py from matched/game/refresh_current_window_port.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern char * current_window(void);
extern void empty_1070_dad8(void);
extern void end_modal_1060_9fad(void *port);

void refresh_current_window_port(col_i16 unused1)
{
    char *c;
    char *w = current_window();

    c = w != 0 ? w - 0x26 : w;

    if (c == 0)
        return;
    empty_1070_dad8();
    end_modal_1060_9fad(c + 0x26);
}
