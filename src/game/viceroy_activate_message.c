/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 14, 15 of the imported file. */
/* PORT: this file's own declarations of ColChild_ActivateMessage, ColChild_DrawAnyMessage removed: the header declares them from the definition (W1 step 3). */
/* viceroy_activate_message: candidate for 1038:b1d9 (54 bytes).  Verdict: RELOCATABLE.
 *
 * Activates the viceroy window's message of the given kind and draws it (mode 1).
 */

#include "colonization.h"

#include "classes.h"


void viceroy_activate_message(int kind, long ms)
{
    ColChild_ActivateMessage((ColChild *)(&g_map_window), kind, ms);
    ColChild_DrawAnyMessage((ColChild *)(&g_map_window), 1, 0L);
}
