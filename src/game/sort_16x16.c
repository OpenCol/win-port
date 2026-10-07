/* sort_16x16: candidate for 1040:75f7 (184 bytes, 17 call sites).  Verdict: EXACT.
 * name-source: sibling-build
 *
 * reconstructed/sort_pairs_by_key.c (1040:7560) over `int` arrays instead of `char`.
 * Same restart-on-swap bubble sort, same `int swapped = 1;` do/while, same
 * `key[i] > key[i + 1]` operand order; only the element type differs.
 *
 * Verdict EXACT, first attempt, from changing three declarations in an already-verified
 * function. Two sizes of the same routine both reaching EXACT is a stronger check on
 * that reconstruction than either verdict alone: the shared source shape is what
 * carried over, and it survived a change of element width without adjustment.
 * Re-verified 2026-09-10 at the full 184-byte extent, trailing dead epilogue included: EXACT.
 *
 * RENAMED 2026-09-26 from sort_int_pairs_by_key: the Mac build's sort_16x16(short, short*, short*): (count, payload, key) on both sides, and the Mac europe_recruit_any -- bound in the same batch to the King's recruit screen that calls this -- calls sort_16x16 where this screen calls it.
 */

#include "colonization.h"

void sort_16x16(int n, int *payload, int *key)
{
    int swapped = 1;
    int i;
    int t;

    do {
        swapped = 0;
        for (i = 0; !swapped && i < n - 1; i++) {
            if (key[i] > key[i + 1]) {
                t = key[i];
                key[i] = key[i + 1];
                key[i + 1] = t;
                t = payload[i];
                payload[i] = payload[i + 1];
                payload[i + 1] = t;
                swapped = 1;
            }
        }
    } while (swapped);
}
