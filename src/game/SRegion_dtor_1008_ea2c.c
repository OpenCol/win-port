/* SRegion_dtor_1008_ea2c: candidate for 1008:ea2c (92 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: SRegion::~SRegion()
 *
 * The destructor of SRegion, derived from DlgControl, as compiled into module 1008:
 * release the handle at +0x10 through MSDisposeRegion when set; the base destructor call
 * (with flag 0) is the compiler's.
 * DlgControl::~DlgControl is defined in-file first, so the base call is the 4-byte form.
 */

#include "colonization.h"

#include "w16types.h"

extern void DialogWin_RemoveControl(void *list, int id);

#include "classes.h"

SRegion::~SRegion()
{
    if (hwnd != 0)
        MSDisposeRegion(hwnd);
}
