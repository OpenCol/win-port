/* palette_entry_rgb -- shipped at 1070:dc90.
 *
 * Ported by tools/port.py from matched/platform/palette_entry_rgb.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void palette_get_entry_rgb(void *p, col_i16 idx, char *r,
                                      char *g, char *b);

void palette_entry_rgb(void *p, col_i16 idx, char *r, char *g, char *b)
{
    palette_get_entry_rgb(p, idx, r, g, b);
}
