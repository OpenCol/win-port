/* map_plane0_unlock -- shipped at 1038:b6a5.
 *
 * Ported by tools/port.py from matched/game/map_plane0_unlock.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern short g_plane0_handle;                        DGROUP:0x1c6a
 *   extern unsigned char huge *g_plane0;                 DGROUP:0x1c72
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void * MSHUnlock(col_i16 handle);

void map_plane0_unlock(void)
{
    if (g_plane0 != 0)
        g_plane0 = (unsigned char *)MSHUnlock(g_plane0_handle);
}
