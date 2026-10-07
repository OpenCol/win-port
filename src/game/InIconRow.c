/* PORT: this file's own declarations of ComputeIconRow, sprite_height_1038_8752, sprite_width_1038_86f0 removed: the header declares them from the definition (W1 step 3). */
/* InIconRow: candidate for 1038:3f9c (232 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: InIconRow
 *
 * Which of n portraits laid out in a row (ComputeIconRow gives the step) the
 * point (px, py) falls on, walking from the last back to the first with
 * the sprite's width (plus 2 for style bit 2) and height; -1 when none or
 * when the row has no step.  The three helpers are -called and defined
 * in-file as stubs.
 *
 * RENAMED 2026-09-26 from hit_test_row: the Mac build's InIconRow(short, short, short, short, short, short, short, short, short, short), bound by layout (between ComputeIconRow and DrawIconRow on both sides); the arity agrees and the Mac body is 1.26x this one.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

extern int g_stub_a;


int InIconRow(int pic, int a, int n, int x, int y, int span, int span2, int style, int px, int py)
{
    register int i;
    int found = -1;
    int step;
    int pw;
    int ph;
    int dummy;

    step = ComputeIconRow(pic, a, n, &x, y, span, span2, 0L, &dummy, style);
    if (step == 0)
        goto done;
    pw = sprite_width_1038_86f0(&g_icon_sprites[pic]);
    if (style & 2)
        pw += 2;
    ph = sprite_height_1038_8752(&g_icon_sprites[pic]);
    x += (n - 1) * step;
    x -= pw >> 1;
    for (i = n - 1; found < 0 && i >= 0; i--) {
        if (px > x && x + pw - 1 > px && py > y && y + ph > py)
            found = i;
        x -= step;
    }
done:
    return found;
}
