/* load_string_resource -- shipped at 1040:78b8.
 *
 * Ported by tools/port.py from matched/game/load_string_resource.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern void far *g_resource_ctx3;                    DGROUP:0x1cfb
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void * lock_resource_1040_a99c(void *res);
void unlock_resource_1040_a9bc(void *res);

extern void * MSGetNamedResource(void * *ctx, col_i16 id);
/* same object: 4-byte `push cs; call ` (rule 47) */


extern void free_resource(void *res);
extern void unlock_resource(void *res);
extern void * memcpy_rtl(void *dst, void *src, col_u16 n);

void load_string_resource(unsigned char *dst, col_i16 id, col_i16 index)
{
    col_i16 idx = index;
    void *res;
    unsigned char *p;
    col_i16 count;
    unsigned char n;
    unsigned char hi;
    unsigned char lo;
    void *ctx = g_resource_ctx3;

    if (idx < 0)
        goto fail;
    res = MSGetNamedResource(&ctx, id);
    if (res == 0)
        goto fail;
    p = (unsigned char *)lock_resource_1040_a99c(res);
    hi = *p++;
    lo = *p++;
    count = (hi << 8) + lo;
    if (count < idx) {
        unlock_resource_1040_a9bc(res);
        free_resource(res);
        goto fail;
    }
    idx--;
    while (idx != 0) {
        n = *p++;
        p += n;
        idx--;
    }
    n = *p++;
    *dst = n;
    dst++;
    memcpy_rtl(dst, p, n);
    unlock_resource(res);
    free_resource(res);
    return;
fail:
    *dst = 0;
    return;
}
