/* PORT: this file's own declarations of DialogWin_RemoveControl removed: the header declares it from the definition (W1 step 3). */
/* SRadioSet_dtor_1050_80a3: candidate for 1050:80a3 (114 bytes, 1 caller).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: SRadioSet::~SRadioSet()
 *
 * The destructor of SRadioSet, derived from DlgControl, as compiled into module 1050:
 * when the memory handle at +0x2c is set, unlock it and dispose the radio group with it.
 * DlgControl::~DlgControl is defined in-file first, so the base call is the 4-byte form.
 */

#include "colonization.h"

#include "w16types.h"

#include "classes.h"

SRadioSet::~SRadioSet()
{
    if (hmem != 0) {
        MSHUnlock(hmem);
        MSDisposeRadio(count, hmem);
    }
}
