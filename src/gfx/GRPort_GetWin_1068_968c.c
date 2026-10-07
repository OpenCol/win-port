/* GRPort_GetWin_1068_968c: candidate for 1068:968c (29 bytes).  Verdict: EXACT.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: GRPort_GetWin_1068_968c
 *
 * The port's window (pointer at +0x22); module 1068's copy.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
#include "records.h"

GrafRec *GRPort_GetWin_1068_968c(GRPort *p)
{
    return (GrafRec *)p->win;
}
