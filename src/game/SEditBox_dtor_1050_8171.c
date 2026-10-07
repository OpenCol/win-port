/* SEditBox_dtor_1050_8171: candidate for 1050:8171 (92 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: SEditBox::~SEditBox()
 *
 * The destructor of SEditBox, derived from DlgControl: release the handle at +0x10 through
 * empty_1070_5a82 when set; the base destructor call is the compiler's.
 * DlgControl::~DlgControl is defined in-file first, so the base call is the 4-byte form.
 */

#include "colonization.h"

#include "w16types.h"

extern void DialogWin_RemoveControl(void *list, int id);

#include "classes.h"

SEditBox::~SEditBox()
{
    if (hwnd != 0)
        empty_1070_5a82(hwnd);
}
