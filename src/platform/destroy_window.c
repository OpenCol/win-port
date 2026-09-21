/* destroy_window -- shipped at 1078:18ef.
 *
 * Ported by tools/port.py from matched/platform/destroy_window.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i16 pascal DestroyWindow(col_u16 hwnd);
#endif

void destroy_window(col_u16 hwnd) { if (hwnd != 0) DestroyWindow(hwnd); }
