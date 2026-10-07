/* palette_entry_rgb: candidate for 1070:dc90 (49 bytes).  Verdict: RELOCATABLE.
 *
 * palette_get_entry_rgb with nine words of arguments passed through.
 */

#include "colonization.h"

#include "records.h"
extern void palette_get_entry_rgb(Palette *pal, int idx, char *r, char *g, char *b);

void palette_entry_rgb(void *p, int idx, char *r, char *g, char *b)
{
    palette_get_entry_rgb((Palette *)p, idx, r, g, b);
}
