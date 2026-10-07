/* get_true_name: candidate for 1030:80b5 (61 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 *
 * The adjective word for a nation: tribes (4 and up) from the 6-byte tribe-name records at
 * SEG20:0xd3da, Europeans from the word table at SEG20:0xd89e -- always the true one, unlike
 * get_name.
 *
 * RENAMED 2026-09-27 from get_true_adjective: LEADS3 (verified by another agent, accepted; checked for conflicts with this batch: none). LEAD 2, the first half. The Mac body settles it. CODE 15 get_name (0x666c) reads the tribe record with 'move.w 0(a0,d0.l)' (30 30 08 00), which is field +0, the plural. get_adjective (0x66d8) reads 30 30 08 02, field +2. get_true_adjective (0x6748) is get_adjective with the override block cut out, and it also reads 30 30 08 02, field +2, with no override. 80b5 reads +0 with no override, so it is get_name minus the override, not get_adjective minus it. The Mac build has no such function. get_adjective's RTS+trailer ends exactly where get_true_adjective's LINK begins, and get_true_adjective is followed directly by get_america_name. 80b5 also has 0 callers on Windows, so the Mac linker most likely dead-stripped it. It therefore has no developers' name. 'get_true_name' is a descriptive coinage on the Mac pattern (get_true_X = X without the independence override), and it is the alternative RE_STATUS.md:4933 and the record's own evidence already name. name_source should be descriptive, not sibling-build. The name has to move because 80f2 takes get_true_adjective. No collision. tools/check_findings.py:1738's check label 'get_true_adjective is get_adjective minus the override block' is by address and still passes, but the prose becomes 'get_true_name is get_name minus the override block'.
 */

#include "colonization.h"

#include "records.h"

int get_true_name(int n)
{
    if (n >= 4)
        return g_tribe[n - 4].name;
    else
        return g_nation_adjective[n];
}
