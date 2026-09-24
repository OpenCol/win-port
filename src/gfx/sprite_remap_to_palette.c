/* sprite_remap_to_palette -- shipped at 1068:ab60.
 *
 * Ported by tools/port.py from matched/gfx/sprite_remap_to_palette.c.
 *
 * The file's own Sprite dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void * port_bits(void *port);

struct logpal;
extern col_u16 MSAllocateHandle(col_u32 size);
extern void * MSHLock(col_i16 h);
extern void * MSHUnlock(col_i16 h);
extern col_i16 MSDisposeHandle(col_u16 h);
/* same object: 4-byte `push cs; call ` (rule 47) */

extern void palette_remap_table(struct logpal *a, unsigned char *out, struct logpal *b, col_i16 first, col_i16 n);
extern void port_remap_pixels(void *port, unsigned char *table, col_i16 d, col_i16 count);

void sprite_remap_to_palette(Sprite *s, col_i16 first, col_i16 count)
{
    col_i16 handle;
    unsigned char *p;

    handle = MSAllocateHandle((col_i32)count);
    p = (unsigned char *)MSHLock(handle);
    palette_remap_table((struct logpal *)port_bits(((char *)((unsigned char *)(s) + 0x26))), p, (*((void * *)((unsigned char *)(s) + 0xea))), first, count);
    port_remap_pixels(((char *)((unsigned char *)(s) + 0xc4)), p, first, count);
    p = (unsigned char *)MSHUnlock(handle);
    handle = MSDisposeHandle(handle);
}
