/* destroy_window: candidate for 1078:18ef (29 bytes).  Verdict: RELOCATABLE.
 *
 * Destroys a window handle, ignoring a null one.
 */

#include "colonization.h"

void destroy_window(unsigned hwnd) { if (hwnd != 0) DestroyWindow(hwnd); }
