/* PORT: the owner stored as the dialog's ok_btn by name, not through a local view's 0x90 pad: the offset moves on 64-bit Windows (W9). */
/* link_derived_to_ctl: candidate for 1050:8b5c (46 bytes, 1 caller).  Verdict: RELOCATABLE.
 *
 * Derives an object from the control and points the object's +0x90 back at the control.
 */

#include "colonization.h"

#include "w16types.h"
typedef struct { char pad[0x90]; void *owner; } Derived;
#include "classes.h"
/* same object: 4-byte `push cs; call ` (rule 47) */
extern void *ctl_window_1050_948e(DlgControl *ctl);
void link_derived_to_ctl(void *owner)
{
    ((DialogWin *)ctl_window_1050_948e((DlgControl *)owner))->ok_btn = (DlgControl *)owner;   /* PORT: the dialog's ok_btn, its +0x90 on 16 and 32 bits (W9) */
}
