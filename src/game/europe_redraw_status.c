/* europe_redraw_status -- shipped at 1048:3c2b.
 *
 * Ported by tools/port.py from matched/game/europe_redraw_status.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_europe_win;                        SEG20:0x2fd2
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void europe_draw_status(void *w);

void europe_redraw_status(void)
{
    europe_draw_status(&g_europe_win);
}
