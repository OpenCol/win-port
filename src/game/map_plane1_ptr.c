/* map_plane1_ptr -- shipped at 1038:b76c.
 *
 * Ported by tools/port.py from matched/game/map_plane1_ptr.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern short g_plane1_handle;                        DGROUP:0x1c6c
 *   extern unsigned char huge *g_plane1;                 DGROUP:0x1c76
 *   extern int g_map_width;                              DGROUP:0x4c40
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void * MSHLock(col_i16 handle);

unsigned char * map_plane1_ptr(col_i16 x, col_i16 y)
{
    unsigned char *p;

    if (g_plane1 == 0)
        g_plane1 = (unsigned char *)MSHLock(g_plane1_handle);
    p = g_plane1;
    p += x;
    p += y * (*g_map_width);
    return p;
}
