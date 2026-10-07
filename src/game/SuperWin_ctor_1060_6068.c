/* SuperWin_ctor_1060_6068: candidate for 1060:6068 (92 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: SuperWin::SuperWin()
 *
 * Module 1060's copy of the constructor of a 0x414-byte GameWin subclass with an EventWin
 * member at +0xc4 (vtable at DGROUP:0x28c6). GameWin's and DialogWin's constructors are defined here for the
 * `push cs / call ` form; the EventWin member's constructor is the module's own copy.
 *
 * THE CLASS IS `SuperWin` (renamed from `EventDialog` 2026-09-28). The shipped image has no
 * `EventDialog` type-name string at all; it has `SuperWin` four times, in the RTTI descriptors
 * of segments 1020, 1038, 1050 and 1060 -- the three modules with a copy of this constructor
 * plus SuperWin's own module -- and the object this builds is destroyed by SuperWin::~SuperWin
 * (1050:6b7d) and driven by SuperWin_Init/Execute/GoAway. SuperWin has no out-of-line
 * constructor in 1050: this per-module copy is its inline constructor.
 *
 * RENAMED 2026-09-27 from EventDialog_ctor_1060_6068: the class is SuperWin: COLONIZE.EXE holds no 'EventDialog' type string, 'SuperWin' is in the RTTI of exactly the modules with these copies, and the destructor they pair with is SuperWin::~SuperWin (1050:6b7d). These are its inline constructor, compiled into each module (symbol: SuperWin::SuperWin()).
 */

#include "colonization.h"

#pragma option -a
#include "w16types.h"
#include "classes.h"
#pragma option -a-
class DlgControl;

SuperWin::SuperWin()
{
}
