/* clean_up: candidate for 1008:7b2c (393 bytes, 1 caller).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Releases the four map planes (unlock if locked, dispose) and the overlay handle, then
 * frees the string pool (`strings_shutdown`). The fourth block stores its dispose result into the FIRST
 * plane's handle -- a copy-paste slip in the original, reproduced.
 *
 * RENAMED 2026-09-27 from map_planes_free: the Mac build's clean_up, bound by tools/mac_bind.py's proposal and checked against the body and the Mac call graph: Four MSHUnlock/MSDisposeHandle pairs, one MSDisposeHandle, then strings_shutdown: identical to Mac clean_up's sequence; 0 args, ratio 0.72; the caller is the game's shutdown.
 */

#include "colonization.h"

extern void strings_shutdown(void);

void clean_up(void)
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
    if (g_seam_handle != 0)
        g_seam_handle = MSDisposeHandle(g_seam_handle);
    strings_shutdown();
}
