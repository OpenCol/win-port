/* create_palette: candidate for 1070:0442 (47 bytes).  Verdict: RELOCATABLE.
 *
 * CreatePalette when the palette mode at DGROUP:0x3380 is 1; 0 otherwise.
 */

#include "colonization.h"

int create_palette(void *lp)
{
    register int r;

    if (g_palette_mode == 1)
        r = CreatePalette(lp);
    else
        r = 0;
    return r;
}
