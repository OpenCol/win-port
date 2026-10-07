/* PORT: this file's own declarations of GRPort_InitStruct, dispose_wing_surface removed: the header declares them from the definition (W1 step 3). */
/* GRPort_dtor: candidate for 1068:3767 (170 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: GRPort::~GRPort()
 *
 * The destructor of GRPort (vtable at DGROUP:0x2b24): releases the surface when
 * GRPort_IsLocked says so, hands the pointer at +0x22 to dispose_wing_surface, resets the struct
 * with a null rectangle, and clears the current-port pointer at DGROUP:0x2ac2 if it was this.
 */

#include "colonization.h"

#include "w16types.h"
#include "records.h"
#include "classes.h"
extern "C" char GRPort_IsLocked(GRPort *p);
extern "C" void MSUnlockGraf(GRPort *port);
/* same object: 4-byte `push cs; call ` (rule 47) */
extern GRPort * g_current_port;   /* DGROUP:0x2ac2, declared */

GRPort::~GRPort()
{
    if (GRPort_IsLocked(this))
        MSUnlockGraf(this);
    win = dispose_wing_surface((WinGRec *)win);
    GRPort_InitStruct(this, 0);
    if (g_current_port == this)
        g_current_port = 0;
}
