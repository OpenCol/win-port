/* MSUnlockGraf: candidate for 1068:5010 (76 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: MSUnlockGraf
 *
 * Unlocks a graphics port: drops its surface and frees the row table
 * `MSLockGraf` built, doing nothing when the port is not locked. The two are
 * adjacent and share the record; the Macintosh build names both halves
 * (`MSLockGraf(CGrafPort*)`, `MSUnlockGraf(CGrafPort*)`) and this build spells the rest
 * of its graphics layer the same way. `mem_free` is `operator delete(void*)`.
 *
 * The surface is a pointer, established by the lock half. That correction settles
 * this file's flags too: read as `` it only matched with `-3` off, which made the two
 * halves of one pair disagree about the module; read as `` it matches under the
 * default `-ml -WS -Od -Ot -3`, the same flags as the lock. The earlier flags line was an
 * artifact of the wrong pointer type, not evidence about the build.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
#include "records.h"

void MSUnlockGraf(GRPort *port)
{
    if (port->bits != 0) {
        port->bits = (unsigned char *)return_null((GrafRec *)port->win);
        mem_free(port->rows);
    }
}
