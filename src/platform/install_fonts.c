/* install_fonts -- shipped at 1078:1512.
 *
 * Ported by tools/port.py from matched/platform/install_fonts.c.
 *
 * STRCPY -> strcpy, STRNCPY -> strncpy: the C runtime's own, taken from the real header rather than
 * from this file's far-pointer prototype.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include <string.h>
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i16 pascal AddFontResource(const char *name);
#endif

extern col_u16 MSAllocateHandle(col_u32 size);
extern char * MSHLock(col_i16 h);
extern void MSHUnlock(col_i16 h);


extern col_i16 enumerate_font_families(char *rec);

col_u16 install_fonts(const char *name)
{
    col_u16 h = 0;
    char *f;
    col_i16 n;

    if ((n = AddFontResource(name)) != 0) {
        h = MSAllocateHandle(0x186L);
        f = MSHLock(h);
        strcpy(f, name);
        strncpy(f + 0x102, name, 3);
        if (enumerate_font_families(f) != n)
            ;
        MSHUnlock(h);
    } else
        ;
    return h;
}
