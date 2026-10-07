/* allocate_map_memory: candidate for 1038:ce56 (245 bytes, 2 callers).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Sizes the map (width * height, or 12000 cells for the large map), sets the viewport to
 * 480x384, and allocates and locks the four map planes in turn; any failed handle exits
 * with 1, success returns 0. Each failure exit is a `goto`.
 *
 * RENAMED 2026-09-27 from map_planes_alloc: NOT PROPOSED BY mac_bind; found alongside map_load. Mac allocate_map_memory(short). Callers: Mac load_game(FSSpec*) and map_startup; Windows load_game_file (= load_game(FSSpec*)) and map_load (= map_startup). Arity: 1 (int big). Both callers call it exactly where the Mac seq has allocate_map_memory. Layout: Mac CODE 20 allocate_map_memory, map_check, load_map_file, map_startup; Windows 1038 map_planes_alloc, return_one_1038_cf4b (0 callers, Windows-only), return_zero_1038_cf6e, map_read_planes, [close_log_file, map_alloc: Windows-only, 0 callers], map_load.
 */

#include "colonization.h"

#include "records.h"
extern MapSize g_map_size;   /* DGROUP:0x4c40 */

                     /* DGROUP:0x1c72, 0x1c76, 0x1c7a, 0x1c7e */

int allocate_map_memory(int big)
{
    int err = 1;

    g_map_bytes = g_map_size.w * g_map_size.h;
    if (big != 0)
        g_map_bytes = 12000L;
    g_view_px_w = 480;
    g_view_px_h = 384;
    g_plane0_handle = MSAllocateHandle(g_map_bytes);
    if (g_plane0_handle == 0)
        goto done;
    g_plane0 = (unsigned char *)MSHLock(g_plane0_handle);
    g_plane1_handle = MSAllocateHandle(g_map_bytes);
    if (g_plane1_handle == 0)
        goto done;
    g_plane1 = (unsigned char *)MSHLock(g_plane1_handle);
    g_plane2_handle = MSAllocateHandle(g_map_bytes);
    if (g_plane2_handle == 0)
        goto done;
    g_plane2 = (unsigned char *)MSHLock(g_plane2_handle);
    g_plane3_handle = MSAllocateHandle(g_map_bytes);
    if (g_plane3_handle == 0)
        goto done;
    g_plane3 = (unsigned char *)MSHLock(g_plane3_handle);
    err = 0;
done:
    return err;
}
