/* SCheckBox_dtor: candidate for 1050:8115 (92 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: SCheckBox::~SCheckBox()
 *
 * The destructor of SCheckBox, derived from DlgControl: release the handle at +0x10 through
 * MSDisposeCheck when set; the base destructor call is the compiler's.
 * DlgControl::~DlgControl is defined in-file first, so the base call is the 4-byte form.
 */

#include "colonization.h"

#include "w16types.h"

extern void DialogWin_RemoveControl(void *list, int id);

#include "classes.h"

SCheckBox::~SCheckBox()
{
    if (hwnd != 0)
        MSDisposeCheck(hwnd);
}
