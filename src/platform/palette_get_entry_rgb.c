/* palette_get_entry_rgb: candidate for 1070:02b1 (77 bytes).  Verdict: EXACT.
 *
 * Reads r, g, b of palette entry i (4-byte entries from +4) into the three out pointers.
 */

#include "colonization.h"

#include "records.h"

void palette_get_entry_rgb(Palette *pal, int i, char *r, char *g, char *b)
{
    *r = pal->e[i].r;
    *g = pal->e[i].g;
    *b = pal->e[i].b;
}
