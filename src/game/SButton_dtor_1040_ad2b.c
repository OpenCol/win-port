/* SButton_dtor_1040_ad2b: candidate for 1040:ad2b (92 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: SButton::~SButton()
 *
 * The destructor of SButton, derived from DlgControl, as compiled into module 1040:
 * release the handle at +0x10 through MSDisposeButton when set; the base destructor call
 * (with flag 0) is the compiler's.
 * DlgControl::~DlgControl is defined in-file first, so the base call is the 4-byte form.
 */

#include "colonization.h"

#include "w16types.h"

extern void DialogWin_RemoveControl(void *list, int id);

#include "classes.h"

SButton::~SButton()
{
    if (hwnd != 0)
        MSDisposeButton(hwnd);
}
