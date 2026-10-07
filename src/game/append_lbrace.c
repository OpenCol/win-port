/* append_lbrace: candidate for 1040:a29d (32 bytes).  Verdict: RELOCATABLE.
 *
 * Appends an opening brace to s. One of twelve 32-byte siblings at 1040:a192..a31d that differ only
 * in their literal, and the literals sit end to end in DGROUP from 0x1f9e -- this one at
 * 0x1fb0 -- so each literal's LENGTH is fixed by where the next one starts. That is what
 * settles \"{\" as this one's literal (the \".  \" reading is period_string's).
 */

#include "colonization.h"

void append_lbrace(char *s)
{
    STRCAT(s, "{");
}
