/* SImageButton_ctor_1048_e9a9: candidate for 1048:e9a9 (110 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: SImageButton::SImageButton()
 *
 * Module 1048's copy of the image-button constructor (0x74 bytes): the DlgControl base, the
 * two GRPorts at +0x22 and +0x48, and the flag byte at +0x72 cleared. DlgControl's constructor
 * is defined here for the `push cs / call ` form.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

extern void DialogWin_RemoveControl(void *list, int id);

SImageButton::SImageButton()
{
    reinit_faces = 0;
}
