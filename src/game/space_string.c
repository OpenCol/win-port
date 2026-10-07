/* space_string: candidate for 1040:a192 (32 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Appends a single space to s. One of twelve 32-byte siblings at 1040:a192..a31d that differ only
 * in their literal, and the literals sit end to end in DGROUP from 0x1f9e -- this one at
 * 0x1f9e -- so each literal's LENGTH is fixed by where the next one starts. That is what
 * settles \" \" as this one's literal (the \".  \" reading is period_string's).
 *
 * RENAMED 2026-09-26 from append_space: the Mac build's space_string(char*), bound by call-graph agreement (tools/mac_callgraph.py): 16 of its Mac call edges already bound land on this function (caller CargoLine; caller DoTerrainArticle; caller DoUnitArticle; caller DrawColonists; caller DrawEuropeShips), the runner-up gets 11, and the arity agrees (1).
 */

#include "colonization.h"

void space_string(char *buf)
{
    STRCAT(buf, " ");
}
