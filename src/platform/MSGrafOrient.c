/* MSGrafOrient: candidate for 1070:a70f (39 bytes).  Verdict: EXACT.
 * name-source: sibling-build
 *
 * Whether the WinG surface record (GrafRec, built by MSInitGraf) is TOP-DOWN: its +0xa
 * orientation word is 1. Its callers are the scan-line walkers (GRPort_NextScanLine,
 * GRPort_PrevScanLine, GRPort_CopyToPort, GRPort_FillColor, MSLockGraf), which step rows the other way
 * for a bottom-up DIB.
 *
 * RENAMED 2026-09-27 from is_state_one: it tests the +0xa orientation word of the WinG surface record (GrafRec) that MSInitGraf builds, for the scan-line walkers that step rows the other way on a bottom-up DIB.
 *
 * RENAMED 2026-09-27 from graf_is_topdown: the Mac build's MSGrafOrient(CGrafPort*), bound by call-sequence alignment (2.0 to 0): the orientation of the WinG surface record (GrafRec +0xa), which the scan-line walkers consult.
 */

#include "colonization.h"

#include "records.h"

int MSGrafOrient(GrafRec *graf)
{
    if (graf->topdown == 1)
        return 1;
    else
        return 0;
}
