/* hwnd_is_ms_control -- shipped at 1070:4f9e.
 *
 * Ported by tools/port.py from matched/platform/hwnd_is_ms_control.c.
 *
 * STRCMP -> strcmp: the C runtime's own, taken from the real header rather than
 * from this file's far-pointer prototype.
 *
 * The file's own Ctl dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include <string.h>
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
Ctl * object_from_hwnd(col_u16 hwnd);

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern void pascal GetClassName(col_u16 hwnd, char *buf, col_i16 n);
#endif

extern char * STRUPR(char *s);

/* same object: 4-byte `push cs; call ` (rule 47) */

col_i16 hwnd_is_ms_control(col_u16 hwnd)
{
    Ctl *o;
    char name[50];

    GetClassName(hwnd, name, 50);
    if (strcmp(STRUPR(name), "MSCONTROLCLASS") == 0) {
        o = object_from_hwnd(hwnd);
        if ((*((col_i16 *)((unsigned char *)(o) + 0x1c))) == 3)
            return 1;
    }
    return 0;
}
