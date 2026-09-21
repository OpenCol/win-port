/* overlay_window -- shipped at 1008:4bd3.
 *
 * Ported by tools/port.py from matched/game/overlay_window.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern EventWin far *g_overlay;                      DGROUP:0x00c6
 *
 * The file's own EventWin dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

EventWin * overlay_window(void)
{
    return g_overlay;
}
