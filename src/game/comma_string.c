/* comma_string: candidate for 1040:a1dd (32 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Appends a comma and a space to s. One of twelve 32-byte siblings at 1040:a192..a31d that differ only
 * in their literal, and the literals sit end to end in DGROUP from 0x1f9e -- this one at
 * 0x1fa0 -- so each literal's LENGTH is fixed by where the next one starts. That is what
 * settles \", \" as this one's literal (the \".  \" reading is period_string's).
 *
 * RENAMED 2026-09-26 from append_comma_space: the Mac build's comma_string(char*), bound by call-graph agreement (tools/mac_callgraph.py): 6 of its Mac call edges already bound land on this function (caller DrawTitle; caller DrawTitle; caller HallOfFame; caller Stats; caller buy_mercs), the runner-up gets 4, and the arity agrees (1).
 */

#include "colonization.h"

void comma_string(char *buf)
{
    STRCAT(buf, ", ");
}
