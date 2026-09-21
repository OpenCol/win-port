/* MSCloseWindow -- shipped at 1070:088b.
 *
 * Ported by tools/port.py from matched/platform/MSCloseWindow.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_u16 get_w_10_1070_9681(void *w);

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i16 pascal DestroyWindow(col_u16 hwnd);;
extern col_i16 pascal IsWindow(col_u16 hwnd);;
#endif

extern col_i16 get_w_0_1070_9669(void *w);
/* same object: 4-byte `push cs; call ` (rule 47) */




void MSCloseWindow(void *w0)
{
    void *w = w0;

    if (get_w_0_1070_9669(w) != 3) {
        if (IsWindow(get_w_10_1070_9681(w)))
            DestroyWindow(get_w_10_1070_9681(w));
    } else
        ;
}
