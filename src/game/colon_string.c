/* colon_string: candidate for 1040:a1fd (32 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Appends a colon and a space to s. One of twelve 32-byte siblings at 1040:a192..a31d that differ only
 * in their literal, and the literals sit end to end in DGROUP from 0x1f9e -- this one at
 * 0x1fa3 -- so each literal's LENGTH is fixed by where the next one starts. That is what
 * settles \": \" as this one's literal (the \".  \" reading is period_string's).
 *
 * RENAMED 2026-09-26 from append_colon_space: the Mac build's colon_string(char*), bound by call-graph agreement (tools/mac_callgraph.py): 15 of its Mac call edges already bound land on this function (caller DoBuildingsArticle; caller DoCargoArticle; caller DoFathersArticle; caller DoJobArticle; caller DoTerrainArticle), the runner-up gets 13, and the arity agrees (1).
 */

#include "colonization.h"

void colon_string(char *buf)
{
    STRCAT(buf, ": ");
}
