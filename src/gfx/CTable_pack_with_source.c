/* CTable_pack_with_source -- shipped at 1068:0c8b.
 *
 * Ported by tools/port.py from matched/gfx/CTable_pack_with_source.c.
 *
 * The file's own CTable dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: src_a -> h418, src_b -> h41a, steps2 -> mode2
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
struct logpal * identity_ptr_1068_9625(void *src);
void palette_mark_and_install(struct logpal *pal, col_i16 first, col_i16 count);

struct logpal;
extern col_u16 MSAllocateHandle(col_u32 size);
extern void * MSHLock(col_i16 h);
extern void * MSHUnlock(col_i16 h);
extern void pack_palette_rgb(struct logpal *pal, unsigned char *dst, col_i16 first, col_i16 count);
/* same object: 4-byte `push cs; call ` (rule 47) */


void CTable_pack_with_source(CTable *t, col_i16 first, col_i16 count, col_i16 mode, void *src)
{
    col_i16 f = first;
    col_i16 c = count;
    unsigned char *p;

    t->mode2 = mode;
    t->first2 = f;
    t->count2 = c;
    t->h418 = MSAllocateHandle((col_i32)(c * 3));
    t->h41a = MSAllocateHandle((col_i32)(c * 3));
    p = (unsigned char *)MSHLock(t->h418);
    pack_palette_rgb((struct logpal *)t, p, f, c);
    p = (unsigned char *)MSHUnlock(t->h418);
    p = (unsigned char *)MSHLock(t->h41a);
    pack_palette_rgb(identity_ptr_1068_9625(src), p, f, c);
    p = (unsigned char *)MSHUnlock(t->h41a);
    palette_mark_and_install((struct logpal *)t, f, c);
}
