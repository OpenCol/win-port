/* minus_string: candidate for 1040:a2fd (32 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Appends a minus sign to s. One of twelve 32-byte siblings at 1040:a192..a31d that differ only
 * in their literal, and the literals sit end to end in DGROUP from 0x1f9e -- this one at
 * 0x1fb6 -- so each literal's LENGTH is fixed by where the next one starts. That is what
 * settles \"-\" as this one's literal (the \".  \" reading is period_string's).
 *
 * RENAMED 2026-09-26 from append_minus: the Mac build's minus_string(char*), bound by LAYOUT inside an uneven gap: both builds keep source-file order, and this sits in the same uneven gap as its siblings (the Mac body is 1.31x this one, the arity agrees). The names corroborate it, which is what the pairing needed: an uneven gap alone does not settle order. Paired by what it appends (see append_plus's twin, plus_string).
 */

#include "colonization.h"

void minus_string(char *s)
{
    STRCAT(s, "-");
}
