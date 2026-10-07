/* at_plan: candidate for 1028:01d7 (185 bytes, 7 call sites).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Scans one 64-entry row of a two-dimensional table of four-byte records for every
 * record whose first three bytes match the three arguments, and returns the largest
 * fourth byte among them, or 0 if none match. The row stride is 0x100 -- 64 records of
 * four bytes -- reached with `SHL BX,0x8`, and the record index with `SHL AX,0x2`.
 *
 * Note the running maximum is compared as a char while it is stored as a word:
 * `MOV word ptr [BP-4],0x0` allocates a full int, but the comparison against the
 * candidate is byte-wide. Hence `(signed char)r <= ...` in the source. Ghidra printed the
 * same cast, and it matters: comparing as an int diverges.
 *
 * Verdict RELOCATABLE. Matched first attempt.
 * Re-verified 2026-09-10 at the full 185-byte extent, trailing dead epilogue included: RELOCATABLE.
 *
 * RENAMED 2026-09-26 from table_max_match: the Mac build's at_plan(short x4): (nation, x, y, kind) returns the highest priority the plan row holds for that square and kind, 0 if none. The Mac build's AI list family in its own module: add_/push_/kill_/at_plan over one per-nation table, add_/push_/kill_strategy over a second, add_/push_/clear_pickup over the cargo list. The Windows module 1028:0000..06cd holds the same three families side by side, and the pickup trio -- whose Windows names were already settled from behaviour -- pairs one-for-one by arity (4, 1, 0), which fixes what push means here: open a gap by shifting up.
 */

#include "colonization.h"

#include "records.h"

int at_plan(int nation, int x, int y, int kind)
{
    int r = 0;
    int j;

    for (j = 0; j < 0x40; j++) {
        if (g_plans[nation][j].x == (signed char)x && g_plans[nation][j].y == (signed char)y && g_plans[nation][j].kind == (signed char)kind
            && (signed char)r <= g_plans[nation][j].priority)
            r = g_plans[nation][j].priority;
    }
    return r;
}
