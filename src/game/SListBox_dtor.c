/* SListBox_dtor: candidate for 1050:8229 (92 bytes, 1 caller).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: SListBox::~SListBox()
 *
 * The destructor of SListBox, derived from DlgControl: release the handle at +0x10 through
 * the module's disposer when set; the base destructor call is the compiler's.
 * DlgControl::~DlgControl is defined in-file first, so the base call is the 4-byte form.
 */

#include "colonization.h"

#include "w16types.h"

extern void DialogWin_RemoveControl(void *list, int id);

#include "classes.h"

SListBox::~SListBox()
{
    if (hwnd != 0)
        listbox_release(hwnd);
}
