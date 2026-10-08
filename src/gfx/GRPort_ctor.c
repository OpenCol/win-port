/* PORT: this file's own declarations of GRPort_InitStruct removed: the header declares it from the definition (W1 step 3). */
/* GRPort_ctor: candidate for 1068:349f (78 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: GRPort::GRPort()
 *
 * The constructor of GRPort (0x26 bytes, vtable at DGROUP:0x2b24): GRPort_InitStruct with
 * a null rectangle.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

GRPort::GRPort()
{
    vptr = 0;   /* PORT: the class tag Borland's constructor set as its vtable pointer: a GRPort (W4, gen_headers DEVIRTUALIZE) */
    GRPort_InitStruct(this, 0);
}
