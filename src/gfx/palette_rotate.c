/* palette_rotate: candidate for 1068:06b5 (390 bytes).  Verdict: RELOCATABLE.
 *
 * Rotates count palette entries from start (clipped to 256) by n steps -- downwards for a
 * negative n (each step moves the first entry to the end), upwards otherwise -- one entry at
 * a time through the get/set pair, then animates the range on the palette's GDI handle.
 */

#include "colonization.h"

#include "records.h"
extern void palette_get_entry_rgb(Palette *pal, int i, char *r, char *g, char *b);
extern void MSRawSetCTEntry(Palette *pal, int i, char r, char g, char b);
extern void MSAnimatePal(Palette *pal, unsigned hpal, int start, int count);

void palette_rotate(Palette *pal, int start, int count, int n)
{
    int step;
    unsigned char r1, g1, b1, r0, g0, b0;
    int i;

    if (start + count > 0x100)
        count = 0x100 - start;
    if (n < 0) {
        for (step = 0; -n > step; step++) {
            palette_get_entry_rgb(pal, start, &r0, &g0, &b0);
            for (i = start; count + start - 1 > i; i++) {
                palette_get_entry_rgb(pal, i + 1, &r1, &g1, &b1);
                MSRawSetCTEntry(pal, i, r1, g1, b1);
            }
            MSRawSetCTEntry(pal, start + count - 1, r0, g0, b0);
        }
    } else {
        for (step = 0; step < n; step++) {
            palette_get_entry_rgb(pal, count + start - 1, &r0, &g0, &b0);
            for (i = count + start - 1; i > start; i--) {
                palette_get_entry_rgb(pal, i - 1, &r1, &g1, &b1);
                MSRawSetCTEntry(pal, i, r1, g1, b1);
            }
            MSRawSetCTEntry(pal, start, r0, g0, b0);
        }
    }
    MSAnimatePal(pal, pal->hpal, start, count);
}
