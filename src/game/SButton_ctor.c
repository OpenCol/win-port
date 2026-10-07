/* SButton_ctor: candidate for 1058:c0c8 (83 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: SButton::SButton()
 *
 * The constructor of SButton (0x22 bytes, over the control base constructed by the
 * module's DlgControl_ctor_1058_c632): the hot-key byte +0x17 cleared, the face colour at +0x20 set to -1.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

/* Defined here, not merely declared: rule 47 covers the calls the compiler writes,
 * so the base constructor is reached through the four-byte `PUSH CS / CALL NEAR` the
 * shipped has rather than a five-byte call the comparator has to collapse. */

SButton::SButton()
{
    hotkey = 0;
    colour = -1;
}
