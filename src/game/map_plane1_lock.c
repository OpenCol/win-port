/* map_plane1_lock -- shipped at 1038:b8d9.
 *
 * Ported by tools/port.py from matched/game/map_plane1_lock.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern short g_plane1_handle;                        DGROUP:0x1c6c
 *   extern unsigned char huge *g_plane1;                 DGROUP:0x1c76
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void * MSHLock(col_i16 handle);

void map_plane1_lock(void)
{
    if (g_plane1 == 0)
        g_plane1 = (unsigned char *)MSHLock(g_plane1_handle);
}
