/* continent_at: candidate for 1038:ba45 (33 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: continent_at
 *
 * The square's CONNECTED-REGION id: the low nibble of map plane 2 at (x, y). The plane
 * reader is -called and defined in-file as a stub.
 *
 * `map_find_continents` is what puts it there -- it numbers the map's continents and oceans
 * into this nibble, 1..15, once the map is generated -- and `valid_port` is the
 * clearest reader: the whole function is a comparison of two of these. 17 of the 24 callers
 * that keep the result store it in something called `region`.
 *
 * The HIGH nibble of the same byte is the occupying nation (`owner_of`). One
 * byte, two unrelated fields.
 *
 * RENAMED 2026-09-26 from square_region: the Mac build's continent_at(short, short), bound by call-graph agreement (tools/mac_callgraph.py): 6 of its Mac call edges already bound land on this function (caller Stats; caller create_colony; caller label_map; caller land_region_at; caller plan_move), the runner-up gets 4, and the arity agrees (2).
 */

#include "colonization.h"

extern int g_stub_i;

extern unsigned char continent_get(int x, int y);

unsigned char continent_at(int x, int y)
{
    return continent_get(x, y) & 0xf;
}
