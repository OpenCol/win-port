/* VList_dtor: candidate for 1068:9d79 (66 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: VList::~VList()
 *
 * The list destructor: frees the items (VList_ClearList).
 */

#include "colonization.h"

#include "classes.h"
extern "C" void VList_ClearList(VList *l);

VList::~VList()
{
    VList_ClearList(this);
}
