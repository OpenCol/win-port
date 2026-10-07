/* VListItem_ctor: candidate for 1068:9c48 (88 bytes, 3 callers).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: VListItem::VListItem()
 *
 * The constructor of VListItem: three 32-bit fields zeroed; the counter says it has a
 * destructor.
 */

#include "colonization.h"

#include "classes.h"

VListItem::VListItem()
{
    value = 0;
    next = 0;
    prev = 0;
}
