/* get_valid_square: candidate for 1020:b624 (135 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * size-source: 135, not the census 126. The trailing 9 bytes are the dead second epilogue.
 *
 * Searches the two-by-two block whose top-left corner is (x, y) for a square whose `terrain_is_ocean`
 * answer is the one asked for, and reports the first through the two out-pointers. When the
 * caller asks for a non-zero answer the square must also have a `continent_at` of 1;
 * when it asks for 0 that test is skipped entirely.
 *
 * Both loops carry the found flag in their CONDITION -- `!found && x <= x0 + 1` -- which is
 * why the flag is tested twice per square and why the search stops on the first hit with no
 * `break` anywhere. The bound is recomputed from the parameter every time (`MOV AX,[bp+6] /
 * INC AX`), so it is `x <= x0 + 1` and not a limit computed once.
 *
 * THE SECOND TEST IS A `continue`, NOT AN `else`, and the jump form is what says so. Written
 * as `if (want == 0 || nibble == 1) { store }` the second operand compiles to one `JNZ` past
 * the block; the shipped has `JZ +2 / JMP`, which is the inverted pair Borland emits for a
 * bare jump OUT of the body -- and the target is the inner loop's own increment, which is
 * `continue` and nothing else. Six spellings of the condition were tried and only this one
 * and its `goto` twin come out.
 *
 * `continent_at` returns a BYTE: the compare is `CMP AL,1`, not `CMP AX,1`.
 *
 * RENAMED 2026-09-26 from find_adjacent_tile: the Mac build's get_valid_square(short, short, short*, short*, short), bound by call-graph agreement (tools/mac_callgraph.py): 2 of its Mac call edges already bound land on this function (caller move_to; callee continent_at), the runner-up gets 1, and the arity agrees (5). The margin is small, so the binding also rests on the names describing the same behaviour: the descriptive name here was derived from the body before the Mac table was consulted.
 */

#include "colonization.h"

extern int terrain_is_ocean(int x, int y);
extern unsigned char continent_at(int x, int y);

int get_valid_square(int corner_x, int y0, int *px, int *py, int want)
{
    int x;
    register int y;
    int found;

    found = 0;
    for (x = corner_x; !found && x <= corner_x + 1; x++) {
        for (y = y0; !found && y <= y0 + 1; y++) {
            if (terrain_is_ocean(x, y) == want) {
                if (want != 0 && continent_at(x, y) != 1)
                    continue;
                *px = x;
                *py = y;
                found = 1;
            }
        }
    }
    return found;
}
