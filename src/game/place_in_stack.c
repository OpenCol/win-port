/* place_in_stack: candidate for 1038:1a59 (48 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Counts the links from u along Unit+0x18 (-1 ends the chain); -1 for a
 * negative u.
 *
 * RENAMED 2026-09-26 from unit_chain_length: the Mac build's place_in_stack(short), bound by call-sequence alignment (1.0 aligned gaps in colony_select_profession leave it as the lone unbound callee, none against); the arity agrees and the Mac body is 1.25x this one.
 */

#include "colonization.h"

#include "records.h"

int place_in_stack(int u)
{
    int n = -1;

    while (u >= 0) {
        u = g_units[u].link;
        n++;
    }
    return n;
}
