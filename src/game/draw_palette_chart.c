/* draw_palette_chart -- shipped at 1050:6959.
 *
 * Ported by tools/port.py from matched/game/draw_palette_chart.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
/* The game has its own GetPixel and SetPixel, on a GRPort rather than
 * an HDC. windows.h declares the GDI pair, which this unit does not
 * use, and Borland will not take both spellings of the name. */
#define NOGDI
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void copy_all_to_screen_1050_843b(void *port);

extern void SetPixel(void *port, col_i16 x, col_i16 y, col_i16 colour);

/* same object: 4-byte `push cs; call ` */


void draw_palette_chart(void *port)
{
    register col_i16 y;
    register col_i16 x;

    for (x = 0; x < 0x100; x++)
        for (y = 0; y < 0x100; y++)
            SetPixel(port, x, y, (y / 16) * 16 + x / 16);
    copy_all_to_screen_1050_843b(port);
}
