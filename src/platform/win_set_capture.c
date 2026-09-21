/* win_set_capture -- shipped at 1078:2d24.
 *
 * Ported by tools/port.py from matched/platform/win_set_capture.c.
 *
 * The file's own MSWin dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_u16 pascal SetCapture(col_u16 hwnd);
#endif

void win_set_capture(MSWin *w) { SetCapture(w->hwnd); }
