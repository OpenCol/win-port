/* win_release_capture -- shipped at 1078:2d3e.
 *
 * Ported by tools/port.py from matched/platform/win_release_capture.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i16 pascal ReleaseCapture(void);
#endif

void win_release_capture(col_i32 unused1) { ReleaseCapture(); }
