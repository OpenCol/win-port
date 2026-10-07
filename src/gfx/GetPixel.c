/* PORT: this file's own declarations of rect_contains removed: the header declares it from the definition (W1 step 3). */
/* GetPixel: candidate for 1068:5674 (119 bytes).  Verdict: RELOCATABLE.
 * name-source: original
 * size-source: 119, not the census 110. The trailing 9 bytes are the dead second epilogue
 * the closing `return` produces (rule 44), exactly as in `SetPixel`.
 *
 * Reads the colour byte at (x, y) of the port through `GRPort_PixelMap`; outside the clip
 * region it reports "Warning: GetPixel out of clip region" and returns 0. The mirror of
 * `SetPixel`, which is the very next function in the segment and was reconstructed first.
 *
 * The name is the game's own: the warning string at DGROUP:0x2fad spells it.
 */

#include "colonization.h"

#include "w16types.h"
/* same object: 4-byte `push cs; call ` (rule 47) */
extern void report_error_1068_98bc(void *ctx, char *msg, int x, int y);

#include "classes.h"
extern char *GRPort_PixelMap(GRPort *port, int x, int y);

int GetPixel(void *port, int x, int y)
{
    unsigned char *q;

    if (!rect_contains((GRPort *)port, x, y)) {
        report_error_1068_98bc(&g_error_log, "Warning: GetPixel out of clip region", x, y);
        return 0;
    }
    q = GRPort_PixelMap((GRPort *)port, x, y);
    return *q;
}
