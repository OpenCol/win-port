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
extern void GRPort_InitStruct(GRPort *p, void *rect);

GRPort::GRPort()
{
    GRPort_InitStruct(this, 0);
}
