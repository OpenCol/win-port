/* load_color_resource -- shipped at 1068:0180.
 *
 * Ported by tools/port.py from matched/gfx/load_color_resource.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_error_ctx;                         DGROUP:0x2ac0
 *   extern void far *g_color_resource_type;              DGROUP:0x2af2
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 swap_word_identity_1068_9518(col_i16 v);
void * MSGetNamedResource_1068_94c7(void *type, col_i16 id);
void * lock_resource_1068_94f8(void *h);
void empty_1068_94ea(void *ctx, char *msg, col_i32 v);
void free_resource_1068_9549(void *h);
void unlock_resource_1068_952d(void *h);

extern void * MSGetNamedResource(void *type, col_i16 id);
extern void * lock_resource(void *h);
extern void unlock_resource(void *h);
extern void free_resource(void *h);
extern void palette_set_range(void *pal, col_i16 first, col_i16 count, void *rgb);













void load_color_resource(void *pal, col_i16 id)
{
    void *res;
    void *type;
    char *p;
    col_i16 first;
    col_i16 count;

    type = g_color_resource_type;
    res = MSGetNamedResource_1068_94c7(&type, id);
    if (res == 0) {
        empty_1068_94ea(&(*g_error_ctx), "Error: Color resource not found - ", (col_i32)(col_u16)id);
        return;
    }
    p = (char *)lock_resource_1068_94f8(res);
    first = swap_word_identity_1068_9518(*(col_i16 *)p);
    p += 2;
    count = swap_word_identity_1068_9518(*(col_i16 *)p);
    p += 2;
    palette_set_range(pal, first, count, p);
    unlock_resource_1068_952d(res);
    free_resource_1068_9549(res);
}
