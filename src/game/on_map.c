/* on_map: candidate for 1038:b4b6 (57 bytes).  Verdict: RELOCATABLE.
 *
 * 1 when 1 <= x <= width - 1 and 1 <= y <= height - 1: inside the map's border cells.
 */

#include "colonization.h"

#include "records.h"
extern MapSize g_map_size;   /* DGROUP:0x4c40 */

int on_map(int x, int y)
{
    int r = 1;

    if (x < 1 || y < 1 || g_map_size.w - 1 < x || g_map_size.h - 1 < y)
        r = 0;
    return r;
}
