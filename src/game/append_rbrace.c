/* append_rbrace: candidate for 1040:a2bd (32 bytes).  Verdict: RELOCATABLE.
 *
 * Appends a closing brace to s. One of twelve 32-byte siblings at 1040:a192..a31d that differ only
 * in their literal, and the literals sit end to end in DGROUP from 0x1f9e -- this one at
 * 0x1fb2 -- so each literal's LENGTH is fixed by where the next one starts. That is what
 * settles "}" as the single closing brace.
 */

#include "colonization.h"

void append_rbrace(char *s)
{
    STRCAT(s, "}");
}
