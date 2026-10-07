/* GRPort_PrevScanLine: candidate for 1068:6255 (93 bytes, 2 callers).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: GRPort_PrevScanLine
 * name-source: sibling-build
 *
 * GRPort_NextScanLine with the directions swapped: forward by the stride for the normal
 * orientation, back for the other.
 *
 * RENAMED 2026-09-26 from port_row_step_rev: the Mac build's GRPort::PrevScanLine(unsigned char*): GRPort_NextScanLine with the directions swapped. The width matches.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
#include "records.h"
extern int MSGrafOrient(GrafRec *graf);

unsigned char * GRPort_PrevScanLine(GRPort *port, unsigned char *row)
{
    if (MSGrafOrient((GrafRec *)port->win) == 0)
        return row + port->stride;
    else
        return row - port->stride;
}
