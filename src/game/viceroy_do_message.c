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
extern void ColChild_ActivateMessage(void *child, int kind, long arg);
extern void ColChild_DrawAnyMessage(void *w, int mode, long msg);

void viceroy_do_message(int idx, int kind)
{
    ColChild_CancelMessage((ColChild *)&g_map_window, 1);
    viceroy_message(idx);
    viceroy_activate_message(kind, 120L);
}
