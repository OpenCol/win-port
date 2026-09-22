/* map_plane0_at -- shipped at 1038:b6cf.
 *
 * Ported by tools/port.py from matched/game/map_plane0_at.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern short g_plane0_handle;                        DGROUP:0x1c6a
 *   extern unsigned char huge *g_plane0;                 DGROUP:0x1c72
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

unsigned char map_plane0_at(col_i16 x, col_i16 y)
{
    unsigned char *p;

    if (!map_index_in_range(x, y))
        return 0;
    if (g_plane0 == 0)
        g_plane0 = (unsigned char *)MSHLock(g_plane0_handle);
    p = g_plane0;
    p += x;
    p += y * (*g_map_width);
    return *p;
}
