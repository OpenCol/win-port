/* VList_ctor: candidate for 1068:9d21 (88 bytes, 1 caller).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: VList::VList()
 *
 * The constructor of VList: three longs zeroed; the counter says it has a destructor.
 */

#include "colonization.h"

#include "classes.h"

VList::VList()
{
    first = 0;
    last = 0;
    cur = 0;
}
