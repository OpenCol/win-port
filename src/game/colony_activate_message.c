/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 15, 16 of the imported file. */
/* PORT: this file's own declarations of ColChild_ActivateMessage, ColChild_DrawAnyMessage removed: the header declares them from the definition (W1 step 3). */
/* colony_activate_message: candidate for 1010:c04f (54 bytes).  Verdict: RELOCATABLE.
 *
 * Activates the colony window's message of the given kind and draws it (mode 1).
 */

#include "colonization.h"

/* PORT: this file's own ColonyWin removed: include/gen/classes.h define it once (W1 step 2). */
#include "classes.h"


void colony_activate_message(int kind, long ms)
{
    ColChild_ActivateMessage((ColChild *)(&g_colony_window), kind, ms);
    ColChild_DrawAnyMessage((ColChild *)(&g_colony_window), 1, 0L);
}
