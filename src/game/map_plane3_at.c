/* map_plane3_at -- shipped at 1038:bcc6.
 *
 * Ported by tools/port.py from matched/game/map_plane3_at.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern short g_plane3_handle;                        DGROUP:0x1c70
 *   extern unsigned char huge *g_plane3;                 DGROUP:0x1c7e
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

unsigned char map_plane3_at(col_i16 x, col_i16 y)
{
    unsigned char *p;

    if (!map_index_in_range(x, y))
        return 0;
    if (g_plane3 == 0)
        g_plane3 = (unsigned char *)MSHLock(g_plane3_handle);
    p = g_plane3;
    p += x;
    p += y * (*g_map_width);
    return *p;
}
