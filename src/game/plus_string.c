/* plus_string: candidate for 1040:a2dd (32 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Appends a plus sign to s. One of twelve 32-byte siblings at 1040:a192..a31d that differ only
 * in their literal, and the literals sit end to end in DGROUP from 0x1f9e -- this one at
 * 0x1fb4 -- so each literal's LENGTH is fixed by where the next one starts. That is what
 * settles \"+\" as this one's literal (the \".  \" reading is period_string's).
 *
 * RENAMED 2026-09-26 from append_plus: the Mac build's plus_string(char*), bound by LAYOUT inside an uneven gap: both builds keep source-file order, and this sits between percent_string and say_string, where the Mac gap holds 5 functions and the Windows one 7, so the pairing inside it is by size and arity (the Mac body is 1.31x this one, the arity agrees). The names corroborate it, which is what the pairing needed: an uneven gap alone does not settle order. The alignment by size alone had scrambled the five punctuation appenders; they are paired by what they append: open/close -> ( ), plus/minus -> + -, times -> x.
 */

#include "colonization.h"

void plus_string(char *buf)
{
    STRCAT(buf, "+");
}
