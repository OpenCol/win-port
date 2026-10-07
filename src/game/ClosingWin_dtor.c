/* ClosingWin_dtor: candidate for 1010:029f (115 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ClosingWin::~ClosingWin()
 *
 * The destructor of ClosingWin (over MagicWin, vtable at DGROUP:0x998) with a Flic member at
 * +0x100 and an EventWin member at +0x678 (module 1010's copy): nothing beyond the compiler's
 * member and base destruction. Flic's size is not established; it is padded to place the
 * EventWin.
 * EventWin::~EventWin is defined in-file first, for the 4-byte call.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-
class DlgControl;
#include "records.h"
/* PORT: this file's own Flic removed: include/gen/classes.h define it once (W1 step 2). */
class ClosingWin : public MagicWin {
public:
    Flic flic;
    EventWin win;
    char palette[0x9c8 - 0x704];
    ClosingWin();
    ~ClosingWin();
};

ClosingWin::~ClosingWin()
{
}
