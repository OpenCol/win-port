/* PORT: this file's own declarations of DialogWin_RemoveControl removed: the header declares it from the definition (W1 step 3). */
/* SRadioSet_ctor_1068_ddbe: candidate for 1068:ddbe (75 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: SRadioSet::SRadioSet()
 *
 * The radio-set constructor (0x32 bytes): the DlgControl base (its constructor defined here
 * for the `push cs / call ` form) and the memory handle at +0x2c cleared.
 */

#include "colonization.h"

#include "w16types.h"


#include "classes.h"

SRadioSet::SRadioSet()
{
    hmem = 0;
}
