/* map_plane2_at -- shipped at 1038:b9a8.
 *
 * Ported by tools/port.py from matched/game/map_plane2_at.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern short g_plane2_handle;                        DGROUP:0x1c6e
 *   extern unsigned char huge *g_plane2;                 DGROUP:0x1c7a
 *   extern int g_map_width;                              DGROUP:0x4c40
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 map_index_in_range(col_i16 x, col_i16 y);

/* same object: 4-byte `push cs; call ` (rule 47) */

extern void * MSHLock(col_i16 handle);

unsigned char map_plane2_at(col_i16 x, col_i16 y)
{
    unsigned char *p;

    if (!map_index_in_range(x, y))
        return 0;
    if (g_plane2 == 0)
        g_plane2 = (unsigned char *)MSHLock(g_plane2_handle);
    p = g_plane2;
    p += x;
    p += y * (*g_map_width);
    return *p;
}
