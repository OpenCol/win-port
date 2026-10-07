/* ReportWin_ctor: candidate for 1008:dd08 (92 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ReportWin::ReportWin()
 *
 * The constructor of ReportWin (0x8b8 bytes, over ColChild, vtable at DGROUP:0x114): the base
 * and the SImageButton member at +0x77c constructed.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-
class DlgControl;

extern void DialogWin_RemoveControl(void *list, int id);

/* The bases' and members' constructors are defined in this file because the shipped calls to
 * them are the compiler's own `push cs / call ` (GRPort's and MenuClass's are calls). */

class SButton;

ReportWin::ReportWin()
{
}
