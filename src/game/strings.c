/* strings: candidate for 1040:774e (124 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * The n-th NUL-terminated string of the packed string block (pointer at
 * SEG29:0x10e0); the empty string at DGROUP:0x1cf6 when n exceeds the count at 0x1cf4.
 *
 * RENAMED 2026-09-26 from nth_text_string: the Mac build's strings(int), bound by call-graph agreement (tools/mac_callgraph.py): 21 of its Mac call edges already bound land on this function (caller AddToMessage; caller ColonyReportTop; caller DrawColonyShips; caller DrawColonyUnits; caller DrawEuropeShips), the runner-up gets 15, and the arity agrees (1).
 */

#include "colonization.h"

#include "records.h"

char *strings(int n)
{
    char *p;

    if (n > g_text_count)
        return g_str_null;
    p = (char *)g_text_pool.ptr;
    while (n != 0) {
        while (*p != 0)
            p++;
        p++;
        n--;
    }
    return p;
}
