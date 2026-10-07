/* map_index_in_range: candidate for 1038:b4ef (53 bytes).  Verdict: RELOCATABLE.
 *
 * 1 when 0 <= x <= width - 1 and 0 <= y <= height - 1 (DGROUP:0x4c40 / 0x4c42).
 */

#include "colonization.h"

#include "records.h"
extern MapSize g_map_size;   /* DGROUP:0x4c40 */

int map_index_in_range(int x, int y)
{
    if (x < 0 || y < 0 || g_map_size.w - 1 < x || g_map_size.h - 1 < y)
        return 0;
    else
        return 1;
}
