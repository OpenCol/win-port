/* terrain_at: candidate for 1040:42e8 (86 bytes).  Verdict: RELOCATABLE.
 *
 * Terrain id at (x, y): 0x19 off the map, else terrain_type of the plane-0 byte.
 */

#include "colonization.h"

#include "terrain.h"

extern unsigned char map_get(int x, int y);
/* same object: 4-byte `push cs; call ` (rule 47) */
extern int terrain_type(int map_byte);
int terrain_at(int x, int y)
{
    int terrain = TERRAIN_OCEAN;

    if (!on_map(x, y))
        goto done;
    terrain = terrain_type(map_get(x, y));
done:
    return terrain;
}
