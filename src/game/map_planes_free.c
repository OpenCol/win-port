/* map_planes_free -- shipped at 1008:7b2c.
 *
 * Ported by tools/port.py from matched/game/map_planes_free.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern short far g_plane0_handle;                    DGROUP:0x1c6a
 *   extern short far g_plane1_handle;                    DGROUP:0x1c6c
 *   extern short far g_plane2_handle;                    DGROUP:0x1c6e
 *   extern short far g_plane3_handle;                    DGROUP:0x1c70
 *   extern unsigned char huge *far g_plane0;             DGROUP:0x1c72
 *   extern unsigned char huge *far g_plane1;             DGROUP:0x1c76
 *   extern unsigned char huge *far g_plane2;             DGROUP:0x1c7a
 *   extern unsigned char huge *far g_plane3;             DGROUP:0x1c7e
 *   extern short g_map_pixels;                           DGROUP:0x4b2e
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void * MSHUnlock(col_i16 h);
extern col_i16 MSDisposeHandle(col_u16 h);
extern void free_map_block(void);

void map_planes_free(void)
{
    if (g_plane0_handle != 0) {
        if (g_plane0 != 0)
            g_plane0 = (unsigned char *)MSHUnlock(g_plane0_handle);
        g_plane0_handle = MSDisposeHandle(g_plane0_handle);
    }
    if (g_plane1_handle != 0) {
        if (g_plane1 != 0)
            g_plane1 = (unsigned char *)MSHUnlock(g_plane1_handle);
        g_plane1_handle = MSDisposeHandle(g_plane1_handle);
    }
    if (g_plane2_handle != 0) {
        if (g_plane2 != 0)
            g_plane2 = (unsigned char *)MSHUnlock(g_plane2_handle);
        g_plane2_handle = MSDisposeHandle(g_plane2_handle);
    }
    if (g_plane3_handle != 0) {
        if (g_plane3 != 0)
            g_plane3 = (unsigned char *)MSHUnlock(g_plane3_handle);
        g_plane0_handle = MSDisposeHandle(g_plane3_handle);
    }
    if (g_map_pixels != 0)
        g_map_pixels = MSDisposeHandle(g_map_pixels);
    free_map_block();
}
