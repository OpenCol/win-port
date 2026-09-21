/* destroy_cursor -- shipped at 1078:28b1.
 *
 * Ported by tools/port.py from matched/platform/destroy_cursor.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i16 pascal DestroyCursor(col_u16 hcur);;
#endif

void destroy_cursor(col_u16 hcur) { if (hcur != 0) DestroyCursor(hcur); }
