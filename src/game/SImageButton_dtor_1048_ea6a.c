/* PORT: this file's own declarations of DialogWin_RemoveControl removed: the header declares it from the definition (W1 step 3). */
/* SImageButton_dtor_1048_ea6a: candidate for 1048:ea6a (132 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: SImageButton::~SImageButton()
 *
 * This module's copy: releases the button's window handle (image_button_release), then the
 * compiler destroys the two GRPorts (+0x48, +0x22) and the DlgControl base -- the base's
 * destructor is defined here for the `push cs / call ` form.
 */

#include "colonization.h"

#include "w16types.h"


#include "classes.h"

/* PORT: this copy of SImageButton::~SImageButton removed: src/game/SImageButton_dtor_1008_e8ee.c defines it once (W2). */
