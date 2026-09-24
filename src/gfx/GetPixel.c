/* GetPixel -- shipped at 1068:5674.
 *
 * Ported by tools/port.py from matched/gfx/GetPixel.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_error_ctx;                         DGROUP:0x2ac0
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
void empty_1068_98bc(void *ctx, char *msg, col_i16 x, col_i16 y);

extern char rect_contains_1068_62b8(void *port, col_i16 x, col_i16 y);
/* same object: 4-byte `push cs; call ` (rule 47) */


extern unsigned char * GRPort_PixelMap(void *port, col_i16 x, col_i16 y);

col_i16 GetPixel(void *port, col_i16 x, col_i16 y)
{
    unsigned char *q;

    if (!rect_contains_1068_62b8(port, x, y)) {
        empty_1068_98bc(&(*g_error_ctx), "Warning: GetPixel out of clip region", x, y);
        return 0;
    }
    q = GRPort_PixelMap(port, x, y);
    return *q;
}
