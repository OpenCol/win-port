/* make_fake_unit_2: candidate for 1020:8d3a (163 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Takes the next unit record (count at SEG20:0x779e): owner nibble set, position -1/-1,
 * type 13 when `kind` is set else 0, byte +6 = -1, no cargo, then put_down_unit(u, a, b).
 * Returns the new index.
 *
 * RENAMED 2026-09-26 from unit_record_alloc: the Mac build's make_fake_unit_2(short, short, short, short), bound by call-graph agreement (tools/mac_callgraph.py): 4 of its Mac call edges already bound land on this function (caller DoUnitArticle; caller route_edit; callee put_down_unit; callee set_owner), the runner-up gets 1, the arity agrees (4), and the Mac body is 1.06x this one's size (bound pairs: median 1.01, 80% within 0.76-1.83).
 */

#include "colonization.h"

#include "records.h"

extern GameState g_game;   /* SEG20:0x7782 */
extern void set_owner(int u, int owner);
extern void put_down_unit(int u, int x, int y);

int make_fake_unit_2(int kind, int tile_x, int tile_y, int owner)
{
    int u = g_game.unit_count;

    g_game.unit_count++;
    set_owner(u, owner);
    g_units[u].x = 0xff;
    g_units[u].y = 0xff;
    g_units[u].type = kind != 0 ? 0xd : 0;
    g_units[u].home = 0xff;
    g_units[u].cargo_count = 0;
    put_down_unit(u, tile_x, tile_y);
    return u;
}
