/* GRPort_GetWin_1070_960d: candidate for 1070:960d (29 bytes).  Verdict: EXACT.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: GRPort_GetWin_1070_960d
 *
 * The port's window (pointer at +0x22); the widget layer's copy.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-
class DlgControl;
#include "records.h"

GrafRec *GRPort_GetWin_1070_960d(GRPort *p)
{
    return (GrafRec *)p->win;
}
