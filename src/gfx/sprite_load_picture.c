/* sprite_load_picture -- shipped at 1068:ac02.
 *
 * Ported by tools/port.py from matched/gfx/sprite_load_picture.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
RECT * ptr_plus_16(void *p);
void * port_bits(void *p);
void sprite_remap_to_palette(void *s, col_i16 first, col_i16 count);

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern void pascal CopyRect(RECT *dst, RECT *src);
#endif

typedef struct {
    char pad0[0x26];
    char sub[0xc4 - 0x26];
    char port2[0xea - 0xc4];
    void *pal; /* +0xea */
    RECT rect; /* +0xee */
    col_i16 first; /* +0xf6 */
    col_i16 count; /* +0xf8 */
} Pic;

extern void load_picture_resource(void *p, col_i16 res, col_i16 w, col_i16 h, void *pal);
extern void port_select_if_changed(void *p, void *page);

/* same object: 4-byte `push cs; call ` */




void sprite_load_picture(Pic *o, col_i16 res, register col_i16 first, register col_i16 count, void *pal)
{
    o->first = first;
    o->count = count;
    o->pal = pal;
    load_picture_resource(o->port2, res, first, count, pal);
    port_select_if_changed(o->port2, port_bits(o->sub));
    if (port_bits(o->sub) != o->pal)
        sprite_remap_to_palette(o, first, count);
    CopyRect(&o->rect, ptr_plus_16(o->port2));
}
