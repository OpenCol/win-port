/* win_minimize -- shipped at 1078:2158.
 *
 * Ported by tools/port.py from matched/platform/win_minimize.c.
 *
 * The file's own MSWin dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i16 pascal ShowWindow(col_u16 hwnd, col_i16 cmd);;
#endif

void win_minimize(MSWin *w) { if (w != 0) ShowWindow(w->hwnd, 6); }
