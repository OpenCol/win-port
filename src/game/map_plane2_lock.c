/* map_plane2_lock -- shipped at 1038:babf.
 *
 * Ported by tools/port.py from matched/game/map_plane2_lock.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern short g_plane2_handle;                        DGROUP:0x1c6e
 *   extern unsigned char huge *g_plane2;                 DGROUP:0x1c7a
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void * MSHLock(col_i16 handle);

void map_plane2_lock(void)
{
    if (g_plane2 == 0)
        g_plane2 = (unsigned char *)MSHLock(g_plane2_handle);
}
