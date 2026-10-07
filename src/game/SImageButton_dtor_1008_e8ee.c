/* SImageButton_dtor_1008_e8ee: candidate for 1008:e8ee (132 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: SImageButton::~SImageButton()
 *
 * This module's copy: releases the button's window handle (image_button_release), then the
 * compiler destroys the two GRPorts (+0x48, +0x22) and the DlgControl base -- the base's
 * destructor is defined here for the `push cs / call ` form.
 */

#include "colonization.h"

#include "w16types.h"

extern void DialogWin_RemoveControl(void *list, int id);

#include "classes.h"

SImageButton::~SImageButton()
{
    if (hwnd != 0)
        image_button_release(hwnd);
}
