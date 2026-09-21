/* EuropeWin_Close -- shipped at 1048:8d8e.
 *
 * Ported by tools/port.py from matched/game/EuropeWin_Close.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void europe_release_focus(void *o);

extern char * current_window(void);
extern void empty_1070_dad8(void);
/* same object: 4-byte `push cs; call ` (rule 47) */

col_i16 EuropeWin_Close(void)
{
    char *o;
    char *w = current_window();

    o = w != 0 ? w - 0x26 : w;

    empty_1070_dad8();
    if (o != 0)
        europe_release_focus(o);
    return 0;
}
