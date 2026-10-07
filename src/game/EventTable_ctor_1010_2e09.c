/* EventTable_ctor_1010_2e09: candidate for 1010:2e09 (66 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: EventTable::EventTable()
 *
 * The constructor of EventTable (0x5c bytes) as compiled into module 1010: hands the
 * object to the module's own initialiser. One copy per module.
 */

#include "colonization.h"

#include "classes.h"
/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void ClearEventTable_1010_3036(EventTable *table);
EventTable::EventTable()
{
    ClearEventTable_1010_3036(this);
}
