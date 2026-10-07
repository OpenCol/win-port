/* PORT: this file's own declarations of rect_contains removed: the header declares it from the definition (W1 step 3). */
/* SetPixel: candidate for 1068:56eb (118 bytes).  Verdict: RELOCATABLE.
 *
 * Writes the colour byte at (x, y) of the port through GRPort_PixelMap; outside the clip
 * region (rect_contains) it reports "Warning: SetPixel out of clip region" instead. The
 * trailing `return;` is what produces the shipped dead second epilogue.
 */

#include "colonization.h"

#include "w16types.h"
/* same object: 4-byte `push cs; call ` (rule 47) */
extern void report_error_1068_98bc(void *ctx, char *msg, int x, int y);

#include "classes.h"
extern char *GRPort_PixelMap(GRPort *port, int x, int y);

void SetPixel(void *port, int x, int y, int color)
{
    unsigned char *q;

    if (!rect_contains((GRPort *)port, x, y)) {
        report_error_1068_98bc(&g_error_log, "Warning: SetPixel out of clip region", x, y);
        return;
    }
    q = GRPort_PixelMap((GRPort *)port, x, y);
    *q = color;
    return;
}
