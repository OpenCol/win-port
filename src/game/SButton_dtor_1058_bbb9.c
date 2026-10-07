/* PORT: this file's own declarations of DialogWin_RemoveControl removed: the header declares it from the definition (W1 step 3). */
/* SButton_dtor_1058_bbb9: candidate for 1058:bbb9 (92 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: SButton::~SButton()
 *
 * The destructor of SButton, derived from DlgControl, as compiled into module 1058:
 * release the handle at +0x10 through MSDisposeButton when set; the base destructor call
 * (with flag 0) is the compiler's.
 * DlgControl::~DlgControl is defined in-file first, so the base call is the 4-byte form.
 */

#include "colonization.h"

#include "w16types.h"


#include "classes.h"

SButton::~SButton()
{
    if (hwnd != 0)
        MSDisposeButton(hwnd);
}
