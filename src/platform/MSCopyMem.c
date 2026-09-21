/* MSCopyMem -- shipped at 1070:d134.
 *
 * Ported by tools/port.py from matched/platform/MSCopyMem.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern void pascal hmemcpy(void *dst, void *src, col_i32 n);;
#endif

void MSCopyMem(void *src, void *dst, col_i32 n)
{
    hmemcpy(dst, src, n);
}
