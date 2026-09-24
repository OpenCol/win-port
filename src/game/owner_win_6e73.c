/* owner_win_6e73 -- shipped at 1050:6ec7.
 *
 * Ported by tools/port.py from matched/game/owner_win_6e73.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void win_hide_1050_6e73(void *o);

extern char * current_window(void);

/* same object: 4-byte `push cs; call ` */


col_i16 owner_win_6e73(void)
{
    char *o;
    char *w;

    w = current_window();
    o = w != 0 ? w - 0x26 : w;
    if (o != 0)
        win_hide_1050_6e73(o);
    return 0;
}
