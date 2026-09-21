/* display_config_init -- shipped at 1040:72e0.
 *
 * Ported by tools/port.py from matched/game/display_config_init.c.
 *
 * The file's own Cfg dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 rect_height_1040_aa80(void *rect);
col_i16 rect_width_1040_a903(void *rect);

extern void MSGetDesktopSize(void *rect);
/* same object: 4-byte `push cs; call ` (rule 47) */


void display_config_init(Cfg *c)
{
    char rect[8];
    col_i16 i;

    MSGetDesktopSize(rect);
    c->f4 = rect_width_1040_a903(rect);
    c->f6 = rect_height_1040_aa80(rect);
    c->f0 = 7;
    c->f8 = 0;
    c->fa = 1;
    for (i = 0; i < 8; i++)
        ((col_i16 *)((unsigned char *)(c) + 0x18c))[i] = 0;
}
