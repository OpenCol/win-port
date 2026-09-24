/* palette_fade -- shipped at 1068:0abe.
 *
 * Ported by tools/port.py from matched/gfx/palette_fade.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_error_ctx;                         DGROUP:0x2ac0
 *
 * The file's own CTable dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void empty_1068_94ea(void *ctx, char *msg, col_i32 v);

/* same object: 4-byte `push cs; call ` (rule 47) */

extern unsigned char * MSHLock(col_i16 h);
extern unsigned char * MSHUnlock(col_i16 h);
extern void palette_set_entry_rgb(CTable *t, col_i16 i, char r, char g, char b);
extern void palette_animate_range(CTable *t, col_u16 hpal, col_i16 start, col_i16 count);

void palette_fade(CTable *t, col_i16 step)
{
    unsigned char r, g, b;
    unsigned char *src;
    col_i16 i;

    if (step < 0 || t->steps < step) {
        empty_1068_94ea(&(*g_error_ctx), "Color Scale factor out of range", (col_i32)step);
        return;
    }
    src = MSHLock(t->handle);
    for (i = 0; t->count * 3 > i; i += 3) {
        r = src[i] / t->steps * step + t->r / t->steps * (t->steps - step);
        g = src[i + 1] / t->steps * step + t->g / t->steps * (t->steps - step);
        b = src[i + 2] / t->steps * step + t->b / t->steps * (t->steps - step);
        palette_set_entry_rgb(t, t->first + i / 3, r, g, b);
    }
    palette_animate_range(t, t->hpal, t->first, t->count);
    src = MSHUnlock(t->handle);
}
