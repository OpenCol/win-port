/* ocean: candidate for 1020:047e (229 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * size-source: 229, not the census 223. The trailing 6 bytes are the dead last epilogue.
 *
 * A step of the map generator (`build_map`): square (x, y) of the generator's map
 * buffer (handle at DGROUP:0x1c6a) becomes terrain 0x19 -- water -- and 1 is returned, unless
 * the square is off the map or any of its four DIAGONAL neighbours is already 0x19, in which
 * case nothing changes and the answer is 0. The effect is that new water never meets old water
 * only at a corner.
 *
 * Six written-out `return`s, six inline epilogues, and a seventh for the end that nothing
 * reaches (rule 44). `x` and `y` are both `register` parameters and take SI and DI in order.
 *
 * RENAMED 2026-09-27 from set_water_unless_diag: BEYOND THE PROPOSALS: where the Mac ocean() really is (see 1040:433e). Mac ocean(short, short), CODE 11 0x507e, 232 bytes. Its seq is on_map, buffer_get x4, buffer_put, and its only caller is build_map. 047e (register int x, register int y), 229 bytes, calls on_map, buffer_get x4 and buffer_put; its only caller is build_map. Layout: Mac CODE 11 runs blob, blob2, island, elevate, ocean, do_rivers, build_map; Windows 1020 runs blob, blob2, island, elevate, 047e, do_rivers, build_map. Apply this only after 1040:433e has vacated 'ocean'. 'ocean' is a prefix of ocean_tile_yield, so rename by exact word.
 */

#include "colonization.h"

extern unsigned char buffer_get(short handle, int x, int y);
extern void buffer_put(short handle, int x, int y, char v);

int ocean(register int x, register int y)
{
    if (on_map(x, y) == 0)
        return 0;
    if (buffer_get(g_plane0_handle, x - 1, y - 1) == 0x19)
        return 0;
    if (buffer_get(g_plane0_handle, x - 1, y + 1) == 0x19)
        return 0;
    if (buffer_get(g_plane0_handle, x + 1, y - 1) == 0x19)
        return 0;
    if (buffer_get(g_plane0_handle, x + 1, y + 1) == 0x19)
        return 0;
    buffer_put(g_plane0_handle, x, y, 0x19);
    return 1;
}
