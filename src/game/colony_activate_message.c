/* colony_activate_message: candidate for 1010:c04f (54 bytes).  Verdict: RELOCATABLE.
 *
 * Activates the colony window's message of the given kind and draws it (mode 1).
 */

#include "colonization.h"

/* PORT: this file's own ColonyWin removed: include/gen/classes.h define it once (W1 step 2). */
#include "classes.h"

extern void ColChild_ActivateMessage(void *child, int kind, long arg);
extern void ColChild_DrawAnyMessage(void *w, int mode, long msg);

void colony_activate_message(int kind, long ms)
{
    ColChild_ActivateMessage(&g_colony_window, kind, ms);
    ColChild_DrawAnyMessage(&g_colony_window, 1, 0L);
}
