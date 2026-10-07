/* avail_of_father: candidate for 1020:98b8 (107 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * size-source: 107, not the census 101. The last 6 bytes are an unreachable epilogue.
 *
 * How many Founding Fathers of category `cat` the nation could still be offered: the ones
 * whose bit in the 316-byte nation record is CLEAR, whose category byte matches, and whose
 * weight for the current era (`Century()`, from the year) is non-zero. The same three tests
 * `choose_freedom` applies when it sums a category's weights, counted instead of
 * summed -- so this is that function's "is there anything left in this category" question.
 *
 * Twenty-five fathers, five categories. `count_fathers_owned`, directly after it at 1020:9923, is the
 * complement, and drops the weight test because an owned father's weight is irrelevant.
 *
 * All three tests are `continue`s, not a nest of positive `if`s: every one is the shipped
 * invert-and-jump-over-a-jump of rule 12, here at the shortest possible distance.
 *
 * RENAMED 2026-09-26 from count_fathers_available: the Mac build's avail_of_father(short, short), bound by LAYOUT inside an uneven gap: both builds keep source-file order, and this sits between set_father and show_fathers, where the Mac gap holds 4 functions and the Windows one 8, so the pairing inside it is by size and arity (the Mac body is 1.12x this one, the arity agrees). The names corroborate it, which is what the pairing needed: an uneven gap alone does not settle order.
 */

#include "colonization.h"

#include "fathers.h"

#include "records.h"

extern int has_father(int nation, int bit);

/* same object: 4-byte `push cs; call ` */
extern int Century(void);

int avail_of_father(int nation, int cat)
{
    int n = 0;
    register int i;

    for (i = 0; i < FATHER_COUNT; i++) {
        if (has_father(nation, i) != 0)
            continue;
        if (g_fathers[i].cat != (unsigned char)cat)
            continue;
        if (g_fathers[i].weight[Century()] == 0)
            continue;
        n++;
    }
    return n;
}
