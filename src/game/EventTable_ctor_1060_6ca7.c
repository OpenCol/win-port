/* EventTable_ctor_1060_6ca7: candidate for 1060:6ca7 (66 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: EventTable::EventTable()
 *
 * The constructor of EventTable (0x5c bytes) as compiled into module 1060: hands the
 * object to the module's own initialiser. One copy per module.
 */

#include "colonization.h"

#include "classes.h"
/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void ClearEventTable_1060_6d0a(EventTable *table);
EventTable::EventTable()
{
    ClearEventTable_1060_6d0a(this);
}
