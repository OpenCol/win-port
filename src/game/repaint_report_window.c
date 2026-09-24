/* repaint_report_window -- shipped at 1058:06d0.
 *
 * Ported by tools/port.py from matched/game/repaint_report_window.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern char * current_window(void);
extern void draw_report_window(void *o);

void repaint_report_window(void)
{
    char *o;
    char *w = current_window();

    o = w != 0 ? w - 0x26 : w;
    draw_report_window(o);
}
