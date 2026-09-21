/* remove_font_resource -- shipped at 1078:16b5.
 *
 * Ported by tools/port.py from matched/platform/remove_font_resource.c.
 *
 * The file's own FontRes dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i16 pascal DeleteObject(col_u16 o);
extern col_i16 pascal RemoveFontResource(char *name);
#endif

extern FontRes * MSHLock(col_i16 h);
extern void MSHUnlock(col_i16 h);
extern void MSDisposeHandle(col_i16 h);



void remove_font_resource(col_i16 h)
{
    FontRes *p;
    register col_i16 i;

    p = MSHLock(h);
    for (i = 0; p->count > i; i++) {
        if (((col_u16 *)((unsigned char *)(p) + 0x106))[i] != 0) {
            DeleteObject(((col_u16 *)((unsigned char *)(p) + 0x106))[i]);
            ((col_u16 *)((unsigned char *)(p) + 0x106))[i] = 0;
        }
    }
    RemoveFontResource(((char *)((unsigned char *)(p) + 0x0)));
    MSHUnlock(h);
    MSDisposeHandle(h);
}
