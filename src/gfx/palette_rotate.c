/* palette_rotate -- shipped at 1068:06b5.
 *
 * Ported by tools/port.py from matched/gfx/palette_rotate.c.
 *
 * The file's own Palette dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void palette_get_entry_rgb(Palette *p, col_i16 i, unsigned char *r, unsigned char *g, unsigned char *b);
extern void palette_set_entry_rgb(Palette *p, col_i16 i, char r, char g, char b);
extern void palette_animate_range(Palette *p, col_u16 hpal, col_i16 start, col_i16 count);

void palette_rotate(Palette *pal, col_i16 start, col_i16 count, col_i16 n)
{
    col_i16 k;
    unsigned char r1, g1, b1, r0, g0, b0;
    col_i16 i;

    if (start + count > 0x100)
        count = 0x100 - start;
    if (n < 0) {
        for (k = 0; -n > k; k++) {
            palette_get_entry_rgb(pal, start, &r0, &g0, &b0);
            for (i = start; count + start - 1 > i; i++) {
                palette_get_entry_rgb(pal, i + 1, &r1, &g1, &b1);
                palette_set_entry_rgb(pal, i, r1, g1, b1);
            }
            palette_set_entry_rgb(pal, start + count - 1, r0, g0, b0);
        }
    } else {
        for (k = 0; k < n; k++) {
            palette_get_entry_rgb(pal, count + start - 1, &r0, &g0, &b0);
            for (i = count + start - 1; i > start; i--) {
                palette_get_entry_rgb(pal, i - 1, &r1, &g1, &b1);
                palette_set_entry_rgb(pal, i, r1, g1, b1);
            }
            palette_set_entry_rgb(pal, start, r0, g0, b0);
        }
    }
    palette_animate_range(pal, pal->hpal, start, count);
}
