/* spacer: candidate for 1040:a1b2 (43 bytes, 8 callers).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Appends n spaces to s.
 *
 * RENAMED 2026-09-26 from append_spaces: the Mac build's spacer(char*, short), bound by call-graph agreement (tools/mac_callgraph.py): 3 of its Mac call edges already bound land on this function (caller DoTerrainArticle; caller DoUnitArticle; callee space_string), the runner-up gets 2, and the arity agrees (2). The margin is one edge, so the binding also rests on the two names describing the same behaviour: the descriptive name here was derived from the body before the Mac table was consulted.
 */

#include "colonization.h"

/* same object: 4-byte `push cs; call ` (rule 47) */
extern void space_string(char *buf);
void spacer(char *buf, int n)
{
    int i;

    for (i = 0; i < n; i++)
        space_string(buf);
}
