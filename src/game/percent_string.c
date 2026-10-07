/* percent_string: candidate for 1040:a23d (32 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Appends a percent sign to s. One of twelve 32-byte siblings at 1040:a192..a31d that differ only
 * in their literal, and the literals sit end to end in DGROUP from 0x1f9e -- this one at
 * 0x1faa -- so each literal's LENGTH is fixed by where the next one starts. That is what
 * settles \"%\" as this one's literal (the \".  \" reading is period_string's).
 *
 * RENAMED 2026-09-26 from append_percent: the Mac build's percent_string(char*), bound by call-graph agreement (tools/mac_callgraph.py): 7 of its Mac call edges already bound land on this function (caller DoTerrainArticle; caller DrawColonists; caller DrawTitle; caller HallOfFame; caller Score), the runner-up gets 6, and the arity agrees (1). The margin is one edge, so the binding also rests on the two names describing the same behaviour: the descriptive name here was derived from the body before the Mac table was consulted.
 */

#include "colonization.h"

void percent_string(char *buf)
{
    STRCAT(buf, "%");
}
