/* GRPort_NextScanLine: candidate for 1068:61f2 (93 bytes, 3 callers).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: GRPort_NextScanLine
 * name-source: sibling-build
 *
 * The pixel pointer one row back or forward by the port's stride (+6), depending on the
 * orientation word of the surface record at +0x22 (MSGrafOrient); -pointer arithmetic both ways.
 *
 * RENAMED 2026-09-26 from port_row_step: the Mac build's GRPort::NextScanLine(unsigned char*): this plus a pixel pointer, stepped one row down the picture -- back by the stride in the normal bottom-up orientation. The width matches.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
#include "records.h"
extern int MSGrafOrient(GrafRec *graf);

unsigned char * GRPort_NextScanLine(GRPort *p, unsigned char *row)
{
    if (MSGrafOrient((GrafRec *)p->win) == 0)
        return row - p->stride;
    else
        return row + p->stride;
}
