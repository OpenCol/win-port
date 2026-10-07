/* CursorClass_ctor: candidate for 1008:f592 (70 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: CursorClass::CursorClass()
 * name-source: exception type descriptor
 *
 * The constructor of CursorClass, a FOUR-byte class with no base and no virtual: the cursor
 * handle at +0 that `CursorClass_dtor` releases, and the resource id at +2 that `cursor_load`
 * fills. It sets +2 to -1 before +0 to 0, so the members are written in that order and not in
 * declaration order. Everything else in the seventy bytes is the compiler's -- the conditional
 * `operator new` of 4 bytes when `this` arrives null, and the framework's live-object counter
 * at SS:0x10.
 *
 * THE NAME COMES FROM BORLAND'S OWN TYPE DESCRIPTOR. This was filed as `IdPair_ctor_1008_f592`,
 * a provisional name. Module 1008's static constructor (`create_game_globals`) builds
 * fifteen of these at SEG20:0x7746 through `VNEWVF` with this function's address, its static
 * destructor tears them down through `VDELF` with `CursorClass_dtor`'s, and the exception table
 * after it spells the array's type as "CursorClass[15]" at 1008:f18a.
 *
 * Its census size is 70 and the gap to the next census entry is 1,600, so the gap says
 * nothing here; the function ends where the epilogue does.
 */

#include "colonization.h"

#include "classes.h"

CursorClass::CursorClass()
{
    id = -1;
    hcursor = 0;
}
