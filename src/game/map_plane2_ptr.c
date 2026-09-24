/* map_plane2_ptr -- shipped at 1038:b92d.
 *
 * Ported by tools/port.py from matched/game/map_plane2_ptr.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern short g_plane2_handle;                        DGROUP:0x1c6e
 *   extern unsigned char huge *g_plane2;                 DGROUP:0x1c7a
 *   extern int g_map_width;                              DGROUP:0x4c40
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void * MSHLock(col_i16 h);

unsigned char * map_plane2_ptr(col_i16 x, col_i16 y)
{
    unsigned char *p;

    if (g_plane2 == 0)
        g_plane2 = (unsigned char *)MSHLock(g_plane2_handle);
    p = g_plane2;
    p += x;
    p += y * (*g_map_width);
    return p;
}
