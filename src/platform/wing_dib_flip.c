/* wing_dib_flip -- shipped at 1070:a736.
 *
 * Ported by tools/port.py from matched/platform/wing_dib_flip.c.
 *
 * The file's own Wing dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern void pascal hmemcpy(void *dst, void *src, col_i32 n);
#endif

/* +0xe, +0x10 */
extern unsigned char * wing_dib_pointer(Wing *w);
extern col_u16 MSAllocateHandle(col_u32 size);
extern void * MSHLock(col_i16 h);
extern void MSDisposeHandle(col_u16 h);


void wing_dib_flip(Wing *w)
{
    unsigned char *top;
    unsigned char *bot;
    unsigned char *tmp;
    unsigned char *base;
    register col_i16 y;
    register col_i16 h;

    base = wing_dib_pointer(w);
    h = MSAllocateHandle((col_u16)w->rowbytes);
    tmp = (unsigned char *)MSHLock(h);
    top = base;
    bot = base + (col_i32)w->rowbytes * (w->height - 1);
    for (y = 0; w->height / 2 > y; y++) {
        hmemcpy(tmp, bot, (col_i32)w->rowbytes);
        hmemcpy(bot, top, (col_i32)w->rowbytes);
        hmemcpy(top, tmp, (col_i32)w->rowbytes);
        top += w->rowbytes;
        bot -= w->rowbytes;
    }
    MSDisposeHandle(h);
}
