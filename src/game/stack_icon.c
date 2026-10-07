/* stack_icon: candidate for 1040:0b6c (408 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 * size-source: 408, not the census 399. The trailing 9 bytes are the dead second epilogue.
 *
 * THE ICON TO DRAW FOR u'S STACK, or -1: the icon byte (+2) of a unit type. `*more` is set to
 * whether the stack holds more than one unit. Nothing calls it statically -- Ghidra and the
 * relocation table agree -- so the two mode arguments are named by what they do, not by use.
 *
 * With `own` clear or `self` clear, a colony on the square answers -1 first. Then:
 *  - `own` clear: the stack's tail unit stands in for u, and the answer is the last ship
 *    (types 0xd..0x11) in the stack if there is one, else that tail unit.
 *  - `own` and `self` both set: u's own icon, with no colony test.
 *  - `own` set, `self` clear: -1 when u is itself a ship or the stack has no ship; otherwise the
 *    last ship's icon.
 *
 * Ships here are 0xd..0x11, one type short of the 0xd..0x12 range the combat and cargo code
 * uses -- the last type is left out of the carrier test.
 *
 * `*more = next_unit(tail) >= 0` is a relational stored as a value: `OR AX,AX / JL /
 * MOV AX,1 / JMP / XOR AX,AX`. `unit` is a `register` local and takes SI, the `register`
 * parameter `u` DI (rule 53); the written-out `return` leaves a dead epilogue (rule 44).
 */

#include "colonization.h"

#include "units.h"
#include "records.h"

extern int top_unit(int u);
extern int next_unit(int u);

int stack_icon(register int u, int own, int self, int *more)
{
    int ship;
    int icon;
    int tail;
    int col;
    register int unit;

    ship = -1;
    icon = -1;
    if (own == 0 || self == 0) {
        col = find_colony(g_units[u].x, g_units[u].y);
        if (col >= 0)
            goto out;
    }
    tail = top_unit(u);
    *more = next_unit(tail) >= 0;
    if (own == 0) {
        u = tail;
    } else if (self != 0) {
        icon = g_unit_types[g_units[u].type].icon;
        goto out;
    }
    for (unit = tail; unit >= 0; unit = next_unit(unit))
        if (g_units[unit].type >= UNIT_SHIP_FIRST && g_units[unit].type <= UNIT_FRIGATE)
            ship = unit;
    if (own != 0) {
        if (g_units[u].type >= UNIT_SHIP_FIRST && g_units[u].type <= UNIT_FRIGATE)
            goto out;
        if (ship < 0)
            goto out;
    }
    if (ship >= 0)
        icon = g_unit_types[g_units[ship].type].icon;
    else
        icon = g_unit_types[g_units[u].type].icon;
out:
    return icon;
}
