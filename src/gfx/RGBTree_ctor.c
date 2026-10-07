/* RGBTree_ctor: candidate for 1068:1259 (131 bytes, 2 callers).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: RGBTree::RGBTree()
 *
 * The constructor of RGBTree, the class whose destructor the RTTI descriptor names.
 * Zeroes every scalar field and the node pointer; the stack array is left as it is. Compiled as C++ (-P): Borland emits the `this == 0 -> operator new`
 * prologue and returns `this` in DX:AX. The `INC DWORD [SS:0x10]` before the return is the
 * compiler's constructed-object counter for exception unwinding: it appears only when the
 * class declares a destructor, which is why `~RGBTree()` is declared here though not
 * defined. Without it the candidate stops 16 bytes short; no -x/-xd/-RT flag adds it.
 */

#include "colonization.h"

#include "records.h"

#include "classes.h"

RGBTree::RGBTree()
{
    built = 0;
    visited = 0;
    sp = 0;
    nodes = 0;
    handle = 0;
    count = 0;
    base = 0;
    nil = 0;
}
