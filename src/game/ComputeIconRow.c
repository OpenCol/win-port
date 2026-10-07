/* PORT: this file's own declarations of sprite_width_1038_86f0 removed: the header declares it from the definition (W1 step 3). */
/* ComputeIconRow: candidate for 1038:3d92 (267 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: ComputeIconRow
 *
 * Lays n portraits out in a span: the step between them is the free width
 * over n - 1, at most the portrait width + 1 (+2 for style bit 2) and at
 * least 1; while the row still overflows the span, *shift doubles the
 * compression; the leftover (against span2 when that is wider than the
 * row, else against span) goes to *out and, when span2 is set, half of it
 * centres *px.  Returns the step, 0 when n or m is 0.  Three helpers are
 * -called and defined in-file (min/max real, the width reader a stub).
 *
 * `y` IS NEVER READ. Every caller passes one -- `ColonyWin_DrawIndividual` and
 * `ColonyWin_DrawFort` both declare it -- and the body ignores it, which is why the
 * description above accounts for every other parameter and not that one. The signature is
 * the caller's claim rather than this function's (RE_STATUS 119); nothing here can be
 * changed on it, because the argument is pushed either way and makes the CALLER clean
 * the stack.
 *
 * RENAMED 2026-09-26 from layout_row: the Mac build's ComputeIconRow(short, short, short, short*, short, short, short, short*, short*, short), bound by call-sequence alignment (2.0 aligned gaps in ColonyWin_DrawFort, ColonyWin_DrawIndividual leave it as the lone unbound callee, none against); the arity agrees and the Mac body is 1.01x this one.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

extern int g_stub_a;


int ComputeIconRow(int pic, int n, int m, int *px, int y, int span, int span2, int *out, int *shift, int style)
{
    register int pw;
    register int step = 0;
    int avail;
    int off;
    int width;
    int total;

    if (n == 0 || m == 0)
        goto done;
    *shift = 0;
    pw = sprite_width_1038_86f0(&g_icon_sprites[pic]);
    if (style & 2)
        pw += 2;
    avail = span - pw;
    if (n > 1) {
        step = avail / (n - 1);
        step = min_int_1038_8680(step, pw + 1);
        step = max_int_1038_86a6(step, 1);
    } else {
        step = 1;
    }
    total = (n - 1) * step;
    width = total + pw;
    while ((total >> *shift) > avail) {
        (*shift)++;
        width = (total >> *shift) + pw;
    }
    if (span2 - 1 > width)
        off = span2 - width;
    else
        off = span - width;
    if (out != 0)
        *out = off;
    if (span2 != 0)
        *px += off >> 1;
done:
    return step;
}
