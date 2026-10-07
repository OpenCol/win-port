/* popup_run_default: candidate for 1050:8e63 (34 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 * size-source: 34. The last 8 are a second, unreachable epilogue.
 *
 * Runs a popup with a null answer buffer: `Popup_exec(p, 0)` -- the call for a popup
 * that has buttons and no edit field, where the caller only wants which button was picked.
 */

#include "colonization.h"

/* PORT: this file's own Popup removed: include/gen/classes.h define it once (W1 step 2). */
#include "classes.h"   /* the class, for C: the port, the two sprites and the arena (a MemBlock) as their bytes */
/* same object: 4-byte `push cs; call ` (rule 47) */
extern int g_stub_a;
extern int Popup_exec(Popup *popup, char *text);

int popup_run_default(Popup *popup)
{
    return Popup_exec(popup, 0);
}
