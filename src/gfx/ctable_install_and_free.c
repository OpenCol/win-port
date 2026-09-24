/* ctable_install_and_free -- shipped at 1068:09c3.
 *
 * Ported by tools/port.py from matched/gfx/ctable_install_and_free.c.
 *
 * The file's own CTable dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void palette_set_range(CTable *c, col_i16 first, col_i16 count, void *rgb);
void sub_1068_05cf(CTable *c, col_i16 first, col_i16 count);

extern void * MSHLock(col_i16 h);
extern void * MSHUnlock(col_i16 h);
extern col_i16 MSDisposeHandle(col_u16 h);

/* same object: 4-byte `push cs; call ` */



void ctable_install_and_free(CTable *c)
{
    void *p;

    p = MSHLock(c->handle);
    palette_set_range(c, c->first, c->count, p);
    p = MSHUnlock(c->handle);
    sub_1068_05cf(c, c->first, c->count);
    c->handle = MSDisposeHandle(c->handle);
}
