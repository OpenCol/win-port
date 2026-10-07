/* EventTable_ctor_1060_8a29: candidate for 1060:8a29 (66 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: EventTable::EventTable()
 *
 * The constructor of EventTable (0x5c bytes) as compiled into module 1060: hands the
 * object to the module's own initialiser. One copy per module.
 */

#include "colonization.h"

#include "classes.h"
/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void ClearEventTable_1060_8b2b(EventTable *table);
/* PORT: this copy of EventTable::EventTable removed: src/game/EventTable_ctor_1008_e46f.c defines it once (W2). */
