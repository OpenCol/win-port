/* SButton_ctor_1068_da80: candidate for 1068:da80 (83 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: SButton::SButton()
 *
 * Module 1068's copy of SButton's constructor (0x22 bytes): the hot-key byte +0x17 cleared and the
 * face colour at +0x20 set to -1 (the light-grey default `MSInitButton` paints). DlgControl's constructor is defined here for the
 * `push cs / call ` form.
 */

#include "colonization.h"

#include "w16types.h"

#include "classes.h"

SButton::SButton()
{
    hotkey = 0;
    colour = -1;
}
