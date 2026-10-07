/* PORT: this file's own declarations of DialogWin_RemoveControl removed: the header declares it from the definition (W1 step 3). */
/* SRadioSet_dtor_1068_de09: candidate for 1068:de09 (114 bytes, 1 caller).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: SRadioSet::~SRadioSet()
 *
 * The destructor of SRadioSet, derived from DlgControl, as compiled into module 1068:
 * when the memory handle at +0x2c is set, unlock it and dispose the radio group with it.
 * DlgControl::~DlgControl is defined in-file first, so the base call is the 4-byte form.
 */

#include "colonization.h"

#include "w16types.h"

#include "classes.h"

/* PORT: this copy of SRadioSet::~SRadioSet removed: src/game/SRadioSet_dtor_1050_80a3.c defines it once (W2). */
