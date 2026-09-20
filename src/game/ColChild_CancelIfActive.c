/* ColChild_CancelIfActive -- shipped at 1048:128e.
 *
 * Ported by tools/port.py from matched/game/ColChild_CancelIfActive.c.
 *
 * The file's own ColChild dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void ColChild_CancelMessage(ColChild *w, col_i16 a);

/* same object: 4-byte `push cs; call ` (rule 47) */

col_i16 ColChild_CancelIfActive(ColChild *w)
{
    register col_i16 cancelled = 0;

    if (w->active == 0)
        goto done;
    ColChild_CancelMessage(w, 0);
    cancelled = 1;
done:
    return cancelled;
}
