/* viceroy_activate_message: candidate for 1038:b1d9 (54 bytes).  Verdict: RELOCATABLE.
 *
 * Activates the viceroy window's message of the given kind and draws it (mode 1).
 */

#include "colonization.h"

#include "classes.h"

extern void ColChild_ActivateMessage(void *child, int kind, long arg);
extern void ColChild_DrawAnyMessage(void *w, int mode, long msg);

void viceroy_activate_message(int kind, long ms)
{
    ColChild_ActivateMessage(&g_map_window, kind, ms);
    ColChild_DrawAnyMessage(&g_map_window, 1, 0L);
}
