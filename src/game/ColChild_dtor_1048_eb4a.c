/* ColChild_dtor_1048_eb4a: candidate for 1048:eb4a (94 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ColChild::~ColChild()
 *
 * This module's copy of ColChild's destructor (vtable at DGROUP:0xe4): the compiler-generated
 * sequence -- the SImageButton member at +0x6ea, then the GameWin base. Every same-module
 * destructor it reaches is defined here for the `push cs / call ` form.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-
class DlgControl;

extern void DialogWin_RemoveControl(void *list, int id);

/* The bases' destructors are defined in this file because the shipped calls to them are the
 * compiler's own `push cs / call `, which it only emits for a callee defined in the
 * translation unit; each is the compiler-generated body (see the module's matched copies). */

ColChild::~ColChild()
{
}
