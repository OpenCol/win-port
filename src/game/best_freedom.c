/* best_freedom: candidate for 1020:99cd (112 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * size-source: 112, not the census 103. The trailing 9 bytes are a third, unreachable
 * epilogue -- one for the early `return 5`, one for the final `return`, one for the end.
 *
 * Which of the five Founding Father categories the nation should work toward: the one with
 * the most candidates still available, counted by `avail_of_father` forty bytes
 * above. Ties go to the LATER category, because the test is `>=`.
 *
 * A nation holding all twenty-five gets 5, which is one past the last category and is how
 * `choose_freedom` learns there is nothing left to offer.
 *
 * The count of fathers held is +0x14 of the 316-byte nation record at SEG20:0xc2f4.
 *
 * RENAMED 2026-09-26 from pick_father_category: the Mac build's best_freedom(short), bound by LAYOUT inside an uneven gap: both builds keep source-file order, and this sits between set_father and show_fathers, where the Mac gap holds 4 functions and the Windows one 8, so the pairing inside it is by size and arity (the Mac body is 0.95x this one, the arity agrees). The names corroborate it, which is what the pairing needed: an uneven gap alone does not settle order.
 */

#include "colonization.h"

#include "records.h"
extern Nation g_nations[];   /* 316 bytes each at SEG20:0xc2f4 */

/* same object: 4-byte `push cs; call ` */
extern int avail_of_father(int nation, int cat);

int best_freedom(int nation)
{
    int best = -1;
    int bestn = -1;
    register int i;
    register int n;

    if (g_nations[nation].fathers >= 0x19)
        return 5;
    for (i = 0; i < 5; i++) {
        n = avail_of_father(nation, i);
        if (n >= bestn) {
            bestn = n;
            best = i;
        }
    }
    return best;
}
