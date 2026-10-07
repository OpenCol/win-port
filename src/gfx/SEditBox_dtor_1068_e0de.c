/* SEditBox_dtor_1068_e0de: candidate for 1068:e0de (92 bytes, 1 caller).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: SEditBox::~SEditBox()
 *
 * The destructor of SEditBox, derived from DlgControl: release the handle at +0x10 through
 * the module's disposer when set; the base destructor call is the compiler's.
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
