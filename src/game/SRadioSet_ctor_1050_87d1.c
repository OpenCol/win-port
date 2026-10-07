/* SRadioSet_ctor_1050_87d1: candidate for 1050:87d1 (75 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: SRadioSet::SRadioSet()
 *
 * Module 1050's copy of the radio-set constructor (0x32 bytes): the DlgControl base (its
 * constructor defined here for the `push cs / call ` form) and the memory handle at
 * +0x2c cleared.
 */

#include "colonization.h"

#include "w16types.h"

#include "classes.h"

SRadioSet::SRadioSet()
{
    hmem = 0;
}
