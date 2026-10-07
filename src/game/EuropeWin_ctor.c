/* PORT: this file's own declarations of DialogWin_RemoveControl removed: the header declares it from the definition (W1 step 3). */
/* EuropeWin_ctor: candidate for 1008:de35 (178 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: EuropeWin::EuropeWin()
 *
 * The constructor of EuropeWin (0x8aa bytes, over ColChild, vtable at DGROUP:0xfc): the base,
 * then its members in order -- the SRegion at +0x77c, three SButtons (+0x7aa, +0x7cc, +0x7ee),
 * the SImageButton at +0x810 and the GRPort at +0x884. Every same-module constructor it
 * reaches is defined here for the compiler's `push cs / call ` form.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-
class DlgControl;


/* The bases' and members' constructors are defined in this file because the shipped calls to
 * them are the compiler's own `push cs / call ` (GRPort's and MenuClass's are calls). */

EuropeWin::EuropeWin()
{
}
