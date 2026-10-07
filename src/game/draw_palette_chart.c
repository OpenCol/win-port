/* draw_palette_chart: candidate for 1050:6959 (93 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * Fills a 256 x 256 area with every palette index as a 16 x 16 block: colour
 * `(y / 16) * 16 + x / 16`, one `SetPixel` per pixel, 65,536 of them, then
 * `copy_all_to_screen_1050_843b` to put the result on screen. A palette test chart.
 *
 * The two divisions are signed `IDIV BX` with BX loaded with 16 each time, not shifts --
 * `x / 16` on a signed `int` cannot be a shift, and the shipped pays for it sixty-five
 * thousand times over.
 *
 * The outer loop is x and the inner is y, which is the wrong way round for locality and is
 * what the register assignment says: DI is the outer counter and the SECOND argument to
 * `SetPixel`, SI the inner and the third.
 */

#include "colonization.h"

#include "classes.h"
/* same object: 4-byte `push cs; call ` */

void draw_palette_chart(void *port)
{
    register int y;
    register int x;

    for (x = 0; x < 0x100; x++)
        for (y = 0; y < 0x100; y++)
            SetPixel(port, x, y, (y / 16) * 16 + x / 16);
    copy_all_to_screen_1050_843b((GameWin *)port);
}
