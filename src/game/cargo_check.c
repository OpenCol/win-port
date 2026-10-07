/* cargo_check: candidate for 1038:364f (433 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * size-source: 433, not the census 424. The trailing 9 bytes are the dead second epilogue.
 *
 * HOW MUCH HOLD SPACE THE SHIPS IN u'S STACK HAVE LEFT once every land unit in the stack is on
 * board. The ships (types 0xd..0x12) are listed first, each with its free holds -- the unit
 * type's capacity at +7 less the cargo it carries at +0xc -- in a 64-byte local. Then every
 * non-ship with a size (+8 of its type) under 99 is packed first-fit into that list; a unit
 * that fits on no ship ends it, and the answer is 0.
 *
 * What is returned depends on the third argument. Set, the carrier is unloaded first and only
 * the LAST ship in the list is asked: its room, when at least `need`. Clear, the first ship
 * with at least `need` room answers. `capture_cargo` asks `(u, 1, 1)` and compares the answer
 * with the prize's cargo.
 *
 * `room[i] -= (unsigned char)size` is the in-place `SUB [BP+SI-0x4C],AL`; without the cast the
 * int subtraction loads, operates and stores (rule 60). `room[i] >= (unsigned char)size` is a
 * byte compare for the same reason, and `need` is an `unsigned char` parameter because the
 * shipped reads `[BP+8]` as a byte.
 *
 * THE SAME `room[n - 1]` IS ADDRESSED TWO WAYS: compared, it is `LEA BX / ADD BX,n / [BX-1]`;
 * loaded for the int return, `LEA BX / DEC BX / ADD BX,n / [BX]`. Both come from the one
 * spelling, so neither says anything about how the source wrote the index (compare rule 52).
 *
 * RENAMED 2026-09-26 from stack_cargo_room: the Mac build's cargo_check(short, short, short), bound by call-graph agreement (tools/mac_callgraph.py): 3 of its Mac call edges already bound land on this function (caller capture_cargo; caller special_kill; callee next_unit), the runner-up gets 1, and the arity agrees (3).
 *
 * `need` is an int (2026-10-02): every caller widens it before the push (`MOV AH,0`), and the
 * compares are byte-wide, through `(unsigned char)need`.
 */

#include "colonization.h"

#include "units.h"
#include "records.h"

/* same object: 4-byte `push cs; call ` */
extern void move_to_bottom(int u);
extern int top_unit(int u);
extern int next_unit(int u);

int cargo_check(int u, int need, int last_ship)
{
    int r;
    int n;
    int space;
    int size;
    int unplaced;
    unsigned char room[0x40];
    register int i;
    register int unit;

    r = 0;
    n = 0;
    if (last_ship != 0)
        move_to_bottom(u);
    for (unit = top_unit(u); unit >= 0; unit = next_unit(unit)) {
        if (g_units[unit].type >= UNIT_SHIP_FIRST && g_units[unit].type <= UNIT_SHIP_LAST) {
            space = g_unit_types[g_units[unit].type].holds - g_units[unit].cargo_count;
            room[n] = space;
            n++;
        }
    }
    for (unit = top_unit(u); unit >= 0; unit = next_unit(unit)) {
        if (g_units[unit].type < UNIT_SHIP_FIRST || g_units[unit].type > UNIT_SHIP_LAST) {
            size = g_unit_types[g_units[unit].type].size;
            if (size < 0x63) {
                unplaced = 1;
                for (i = 0; unplaced != 0 && i < n; i++) {
                    if (room[i] >= (unsigned char)size) {
                        room[i] -= (unsigned char)size;
                        unplaced = 0;
                    }
                }
                if (unplaced != 0)
                    goto out;
            }
        }
    }
    if (n == 0)
        goto out;
    if (last_ship != 0) {
        if (room[n - 1] >= (unsigned char)need)
            r = room[n - 1];
    } else {
        for (i = 0; r == 0 && i < n; i++)
            if (room[i] >= (unsigned char)need)
                r = room[i];
    }
out:
    return r;
}
