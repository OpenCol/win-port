/* on_trade_route_close_click -- shipped at 1058:89ea.
 *
 * Ported by tools/port.py from matched/game/on_trade_route_close_click.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void close_report_window(void *o);

extern char * current_window(void);
/* same object: 4-byte `push cs; call ` (rule 47) */

void on_trade_route_close_click(void)
{
    char *o;
    char *w = current_window();

    o = w != 0 ? w - 0x26 : w;

    close_report_window(o);
}
