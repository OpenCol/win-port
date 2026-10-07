/* count_fathers_owned: candidate for 1020:9923 (79 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 * size-source: 79, not the census 73. The last 6 bytes are an unreachable epilogue.
 *
 * How many Founding Fathers of category `cat` the nation already has -- the bit in the
 * 316-byte nation record SET and the category byte matching. The complement of
 * `avail_of_father` directly before it (1020:98b8, 107 bytes), without the weight test: a father already
 * in the congress has no weight left to check.
 */

#include "colonization.h"

#include "fathers.h"

#include "records.h"

extern int has_father(int nation, int bit);

int count_fathers_owned(int nation, int cat)
{
    int n = 0;
    register int i;

    for (i = 0; i < FATHER_COUNT; i++) {
        if (has_father(nation, i) == 0)
            continue;
        if (g_fathers[i].cat != (unsigned char)cat)
            continue;
        n++;
    }
    return n;
}
