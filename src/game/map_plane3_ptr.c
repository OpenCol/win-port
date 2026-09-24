/* map_plane3_ptr -- shipped at 1038:bc4b.
 *
 * Ported by tools/port.py from matched/game/map_plane3_ptr.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern short g_plane3_handle;                        DGROUP:0x1c70
 *   extern unsigned char huge *g_plane3;                 DGROUP:0x1c7e
 *   extern int g_map_width;                              DGROUP:0x4c40
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void * MSHLock(col_i16 h);

unsigned char * map_plane3_ptr(col_i16 x, col_i16 y)
{
    unsigned char *p;

    if (g_plane3 == 0)
        g_plane3 = (unsigned char *)MSHLock(g_plane3_handle);
    p = g_plane3;
    p += x;
    p += y * (*g_map_width);
    return p;
}
