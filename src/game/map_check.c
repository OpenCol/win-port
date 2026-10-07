/* map_check: candidate for 1038:cf6e (35 bytes).  Verdict: EXACT.
 * name-source: sibling-build
 *
 * Returns a local set to 0: a stubbed-out routine left in the build.
 *
 * RENAMED 2026-09-27 from return_zero_1038_cf6e: NOT PROPOSED BY mac_bind. Mac map_check(void): the Mac body clears a local and returns it, so it returns 0, which is what this does. Callers: Mac load_map_file and map_startup; Windows map_read_planes and map_load (and map_alloc, 0 callers). Each call sits where the Mac seq has map_check (after the reads, before allocate_map_memory). Layout: Mac CODE 20 allocate_map_memory, map_check, load_map_file, map_startup; Windows 1038 map_planes_alloc, return_one_1038_cf4b (0 callers, Windows-only), return_zero_1038_cf6e, map_read_planes, [close_log_file, map_alloc: Windows-only, 0 callers], map_load.
 */

#include "colonization.h"

int map_check(void)
{
    int r = 0;

    return r;
}
