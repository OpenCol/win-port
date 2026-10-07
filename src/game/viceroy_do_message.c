/* PORT: this file's own declarations of ColChild_ActivateMessage, ColChild_DrawAnyMessage removed: the header declares them from the definition (W1 step 3). */
/* viceroy_do_message: candidate for 1038:b23a (53 bytes).  Verdict: RELOCATABLE.
 * symbol: viceroy_do_message
 *
 * Cancels the viceroy window's current message, appends text `idx` and activates it as the
 * given kind with argument 120. The two same-module helpers are defined here because the
 * shipped calls to them are the compiler's own `push cs / call `.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

extern void ColChild_CancelMessage(ColChild *child, int a);
extern void ColChild_AddToMessage(ColChild *child, int idx);

void viceroy_do_message(int idx, int kind)
{
    ColChild_CancelMessage((ColChild *)&g_map_window, 1);
    viceroy_message(idx);
    viceroy_activate_message(kind, 120L);
}
