/* make_fake_unit: candidate for 1020:8ca7 (147 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * size-source: 147, not the census 142. The trailing 5 bytes are the dead second epilogue.
 *
 * Appends a unit of type 0x17 to the array at SEG20:0xa208 and puts it on the map: the
 * record at +0x7c0a is initialised first, the count at SEG20:0x779e is taken and then
 * incremented -- so the new unit is the one the OLD count named -- the owner is set, four
 * bytes of the record are stamped, and `put_down_unit` puts it at (x, y). The
 * index comes back.
 *
 * `u = g_game.unit_count; g_game.unit_count++;` is two statements, not `g_game.unit_count++` used as a
 * value: the shipped code loads the count into SI and then increments MEMORY, which a
 * post-increment expression does not do.
 *
 * The four stamps are `type = 0x17`, `+6 = 0xff`, `+7 = 0x2d` and `cargo_count = 0`, each
 * through its own `IMUL BX,BX,0x1c` because nothing survives between them. +7 is the mission
 * byte `order_to` writes.
 *
 * RENAMED 2026-09-26 from create_unit_type23: appends a throwaway unit of type UNIT_TEMPORARY (0x17) after
 * make_fake_type has set that type's icon and strength.
 *
 * RENAMED 2026-09-26 from create_temporary_unit: the Mac build's make_fake_unit(short, short, short, short, short), bound by call-graph agreement (tools/mac_callgraph.py): 5 of its Mac call edges already bound land on this function (caller colony_fortress; caller colony_plan; caller combat_fight; caller try_connect; callee put_down_unit), the runner-up gets 4, and the arity agrees (5). The margin is one edge, so the binding also rests on the two names describing the same behaviour: the descriptive name here was derived from the body before the Mac table was consulted.
 */

#include "colonization.h"

#include "units.h"
#include "records.h"

extern GameState g_game;   /* SEG20:0x7782 */
extern void set_owner(int u, int owner);
extern void put_down_unit(int u, int x, int y);

/* same object: 4-byte `push cs; call ` */
extern void make_fake_type(int icon, int strength);

int make_fake_unit(int icon, int x, int y, int owner, int strength)
{
    register int u;

    make_fake_type(icon, strength);
    u = g_game.unit_count;
    g_game.unit_count++;
    set_owner(u, owner);
    g_units[u].type = UNIT_TEMPORARY;
    g_units[u].home = 0xff;
    g_units[u].mission = 0x2d;
    g_units[u].cargo_count = 0;
    put_down_unit(u, x, y);
    return u;
}
