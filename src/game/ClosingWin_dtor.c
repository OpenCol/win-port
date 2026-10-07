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
class Flic : public GRPort {
public:
    char pad_26[0xa6 - 0x26];
    DiskFile file;
    char pad_132[0x574 - 0x132];
    void *loop_hook;              /* +0x574, set_handler_574 */
    Flic();
    ~Flic();
};
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
