/* europe_redraw_main -- shipped at 1048:4ecd.
 *
 * Ported by tools/port.py from matched/game/europe_redraw_main.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern EuropeWin far g_europe_win;                   SEG20:0x2fd2
 *
 * The file's own EuropeWin dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void europe_redraw(EuropeWin *o);

void europe_redraw_main(void)
{
    europe_redraw(&g_europe_win);
}
