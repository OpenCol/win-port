/* period_string: candidate for 1040:a21d (32 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Appends a full stop and TWO spaces to s. One of twelve 32-byte siblings at 1040:a192..a31d that differ only
 * in their literal, and the literals sit end to end in DGROUP from 0x1f9e -- this one at
 * 0x1fa6 -- so each literal's LENGTH is fixed by where the next one starts. That is what
 * settles \".  \" as a full stop and two spaces rather than one.
 *
 * RENAMED 2026-09-26 from append_period_spaces: the Mac build's period_string(char*), bound by LAYOUT: both builds keep their source files' function order, and on both sides this is the one function between colon_string and percent_string, which are already bound; the arity agrees and the Mac body is 1.38x this one.
 */

#include "colonization.h"

void period_string(char *buf)
{
    STRCAT(buf, ".  ");
}
