/* RGBTree_dtor: candidate for 1068:12dc (117 bytes, 2 callers).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: RGBTree::~RGBTree()
 *
 * The destructor the RTTI descriptor names. Unlocks and disposes the node handle.
 * Compiled as C++: Borland adds the `this == 0` guard, decrements the unwinder's
 * constructed-object counter, and calls operator delete when bit 0 of the hidden
 * flags argument is set.
 */

#include "colonization.h"

#include "records.h"

#include "classes.h"

RGBTree::~RGBTree()
{
    if (nodes != 0)
        nodes = (RGBTreeNode *)MSHUnlock(handle);
    if (handle != 0)
        handle = MSDisposeHandle(handle);
}
