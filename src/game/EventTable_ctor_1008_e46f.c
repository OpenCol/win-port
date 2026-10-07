/* EventTable_ctor_1008_e46f: candidate for 1008:e46f (66 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: EventTable::EventTable()
 *
 * The constructor of EventTable (0x5c bytes) as compiled into module 1008: hands the
 * object to the module's own initialiser. One copy per module.
 */

#include "colonization.h"

#include "classes.h"
/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void ClearEventTable_1008_ea88(EventTable *table);
EventTable::EventTable()
{
    ClearEventTable_1008_ea88(this);
}
