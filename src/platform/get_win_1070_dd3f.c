/* get_win_1070_dd3f: candidate for 1070:dd3f (29 bytes).  Verdict: EXACT.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: get_win_1070_dd3f
 * name-source: descriptive
 * size-source: 29, not the census 25. The trailing 4 bytes are the dead second epilogue.
 *
 * The pointer at +0x6 of the record. A one-line accessor: 20-odd bytes of prologue and epilogue around a single move.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-
class DlgControl;

#include "records.h"
MSWin *get_win_1070_dd3f(Screen *r)
{
    return (MSWin *)r->win;
}
