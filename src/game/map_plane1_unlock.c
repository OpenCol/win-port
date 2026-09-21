/* map_plane1_unlock -- shipped at 1038:b903.
 *
 * Ported by tools/port.py from matched/game/map_plane1_unlock.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern short g_plane1_handle;                        DGROUP:0x1c6c
 *   extern unsigned char huge *g_plane1;                 DGROUP:0x1c76
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void * MSHUnlock(col_i16 handle);

void map_plane1_unlock(void)
{
    if (g_plane1 != 0)
        g_plane1 = (unsigned char *)MSHUnlock(g_plane1_handle);
}
