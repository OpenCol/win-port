/* map_planes_alloc -- shipped at 1038:ce56.
 *
 * Ported by tools/port.py from matched/game/map_planes_alloc.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_map_width;                              DGROUP:0x4c40
 *   extern int g_map_h;                                  DGROUP:0x4c42
 *   extern long g_map_bytes;                             DGROUP:0x1c96
 *   extern int g_view_px_w;                              DGROUP:0x4c5c
 *   extern int g_view_px_h;                              DGROUP:0x4c5e
 *   extern short g_plane0_handle;                        DGROUP:0x1c6a
 *   extern short g_plane1_handle;                        DGROUP:0x1c6c
 *   extern short g_plane2_handle;                        DGROUP:0x1c6e
 *   extern short g_plane3_handle;                        DGROUP:0x1c70
 *   extern void far *g_plane0, far *g_plane1, far *g_plane2, far *g_plane3; DGROUP:0x1c72, DGROUP:0x1c76, DGROUP:0x1c7a, DGROUP:0x1c7e
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* DGROUP:0x1c72, 0x1c76, 0x1c7a, 0x1c7e */
extern col_u16 MSAllocateHandle(col_u32 size);
extern void * MSHLock(col_i16 h);

col_i16 map_planes_alloc(col_i16 big)
{
    col_i16 err = 1;

    g_map_bytes = (*g_map_width) * g_map_h;
    if (big != 0)
        g_map_bytes = 12000L;
    g_view_px_w = 480;
    g_view_px_h = 384;
    g_plane0_handle = MSAllocateHandle(g_map_bytes);
    if (g_plane0_handle == 0)
        goto done;
    g_plane0 = MSHLock(g_plane0_handle);
    g_plane1_handle = MSAllocateHandle(g_map_bytes);
    if (g_plane1_handle == 0)
        goto done;
    g_plane1 = MSHLock(g_plane1_handle);
    g_plane2_handle = MSAllocateHandle(g_map_bytes);
    if (g_plane2_handle == 0)
        goto done;
    g_plane2 = MSHLock(g_plane2_handle);
    g_plane3_handle = MSAllocateHandle(g_map_bytes);
    if (g_plane3_handle == 0)
        goto done;
    g_plane3 = MSHLock(g_plane3_handle);
    err = 0;
done:
    return err;
}
