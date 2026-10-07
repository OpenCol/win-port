/* EventTable_ctor_1038_8bd9: candidate for 1038:8bd9 (66 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: EventTable::EventTable()
 *
 * The constructor of EventTable (0x5c bytes) as compiled into module 1038: hands the
 * object to the module's own initialiser. One copy per module.
 */

#include "colonization.h"

#include "classes.h"
/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void ClearEventTable_1038_8c71(EventTable *table);
EventTable::EventTable()
{
    ClearEventTable_1038_8c71(this);
}
