/* SButton_ctor_1008_e799: candidate for 1008:e799 (83 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: SButton::SButton()
 *
 * Module 1008's copy of SButton's constructor (0x22 bytes): the hot-key byte +0x17 cleared and the
 * word at +0x20 set to -1. THAT WORD IS NOT THE CLICK ID, which this header said
 * until 2026-09-25: the click id is `DlgControl.id` at +2, which `sbutton_init_1010_eeac`
 * fills from its `id` argument (0xc8 for "Buy", 0xc9 for "Change" in `ColonyWin_Init`).
 * SButton+0x20 is a SEPARATE word, `colour`, that only `sbutton_init_f20` takes (both of
 * its callers pass 0x7c). `MSInitButton` reads it as the face colour: -1, the default set
 * here, paints the light-grey stock brush, and any other value is a palette index. DlgControl's constructor is defined here for the
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
