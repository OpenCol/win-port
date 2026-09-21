/* desktop_fits_640 -- shipped at 1048:069b.
 *
 * Ported by tools/port.py from matched/game/desktop_fits_640.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 rect_width_1048_d69b(void *rect);

extern void MSGetDesktopSize(void *rect);
/* same object: 4-byte `push cs; call ` (rule 47) */

col_i16 desktop_fits_640(void *w)
{
    char rect[8];

    MSGetDesktopSize(rect);
    if (rect_width_1048_d69b(rect) <= 0x280)
        return 1;
    return 0;
}
