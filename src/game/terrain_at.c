/* terrain_at -- shipped at 1040:42e8.
 *
 * Ported by tools/port.py from matched/game/terrain_at.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 terrain_from_map_byte(col_i16 b);

#include "terrain.h"

extern col_i16 on_map(col_i16 x, col_i16 y);
extern unsigned char map_plane0_at(col_i16 x, col_i16 y);
/* same object: 4-byte `push cs; call ` (rule 47) */

col_i16 terrain_at(col_i16 x, col_i16 y)
{
    col_i16 t = TERRAIN_OCEAN;

    if (!on_map(x, y))
        goto done;
    t = terrain_from_map_byte(map_plane0_at(x, y));
done:
    return t;
}
