/* map_plane3_lock -- shipped at 1038:bd63.
 *
 * Ported by tools/port.py from matched/game/map_plane3_lock.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern short g_plane3_handle;                        DGROUP:0x1c70
 *   extern unsigned char huge *g_plane3;                 DGROUP:0x1c7e
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void * MSHLock(col_i16 handle);

void map_plane3_lock(void)
{
    if (g_plane3 == 0)
        g_plane3 = (unsigned char *)MSHLock(g_plane3_handle);
}
