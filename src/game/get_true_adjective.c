/* get_true_adjective: candidate for 1030:80f2 (67 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: get_true_adjective
 * name-source: sibling-build
 *
 * The second word of a nation's name record: tribes (4 and up) from the 6-byte records that
 * start with the first tribe at SEG20:0xd3f2 (field +2; the index is taken from the table
 * moved back four records), Europeans from the adjective table at SEG20:0xd89e.
 *
 * RENAMED 2026-09-27 from nation_text_field2: LEADS3 (verified by another agent, accepted; checked for conflicts with this batch: none). LEAD 2. The lead's premise does not hold: get_america_name is already correctly at 1030:8135, whose single caller StatWin_Stats matches the Mac's single caller StatWin::Stats. The real finding is that 80f2 is the Mac get_true_adjective. Call graph: Mac get_true_adjective(short) has exactly 3 callers, HallOfFame (2 calls), ReportWin::Score (3 calls) and anon_18_6942 (1 call; it also calls get_adjective twice, freedom_level and has_father, so it is the Continental Congress report). 80f2 has exactly those 3 callers with the same call counts: HallOfFame 1050:6f8e x2, ReportWin_Score 1058:1dad x3, report_congress_activities 1058:7056 x1, which also calls get_adjective x2. The current get_true_adjective (80b5) has 0 callers. Body: the Mac reads +2 of the 6-byte tribe record with no override (30 30 08 02), and 80f2 reads +2 (0xd3f2-0x16) with no override, falling back to the nation adjective table. Arity: (short) against (int n). Layout: Mac get_name, get_adjective, get_true_adjective, get_america_name; Windows get_name 7fb7, get_adjective 8033, [80b5, no Mac twin], 80f2, get_america_name 8135. Size: 48-byte Mac body (0x6748..0x6778) against 67. Apply after 80b5 has been renamed.
 */

#include "colonization.h"

#include "records.h"

int get_true_adjective(int n)
{
    if (n >= 4)
        return g_tribe[n - 4].adjective;
    else
        return g_nation_adjective[n];
}
