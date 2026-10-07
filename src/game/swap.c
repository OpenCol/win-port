/* swap: candidate for 1040:7b16 (50 bytes).  Verdict: EXACT.
 * name-source: sibling-build
 *
 * Swaps two ints through their pointers.
 *
 * RENAMED 2026-09-26 from swap_ints: the Mac build's swap(short*, short*), bound by LAYOUT: both builds keep their source files' function order, and on both sides this is the one function between minimax and xy_dist, which are already bound; the arity agrees and the Mac body is 0.72x this one.
 */

#include "colonization.h"

void swap(int *a, int *b)
{
    int t = *a;

    *a = *b;
    *b = t;
}
