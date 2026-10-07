/* ReportWin_dtor: candidate for 1008:e228 (318 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ReportWin::~ReportWin()
 *
 * The destructor of ReportWin (0x8b8 bytes, over ColChild, vtable at DGROUP:0x114) with an
 * SImageButton member at +0x77c: deletes the three heap SButtons at +0x8ac/+0x8b0/+0x8b4
 * when set (each `delete` inside the compiler's exception frame, as in ReportWin_Done),
 * then the compiler's member and base destruction.
 * SButton::~SButton, SImageButton::~SImageButton and ColChild::~ColChild are defined in-file first, for the 4-byte calls.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-
class DlgControl;
class SButton;

ReportWin::~ReportWin()
{
    if (prev != 0)
        delete prev;
    if (next != 0)
        delete next;
    if (extra_btn != 0)
        delete extra_btn;
}
